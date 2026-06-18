/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "src/operators/inspect_file.h"

#include <array>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "src/operators/operator.h"
#include "src/utils/system.h"

#ifdef WIN32
#include "src/compat/msvc.h"
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace modsecurity {
namespace operators {


bool InspectFile::init(const std::string &param2, std::string *error) {
    std::ifstream *iss;
    std::string err;
    std::string err_lua;

    m_file = utils::find_resource(m_param, param2, &err);
    iss = new std::ifstream(m_file, std::ios::in);
    if (iss->is_open() == false) {
        error->assign("Failed to open file: " + m_param + ". " + err);
        delete iss;
        return false;
    }

    if (engine::Lua::isCompatible(m_file, &m_lua, &err_lua) == true) {
        m_isScript = true;
    }

    delete iss;
    return true;
}


bool InspectFile::evaluate(Transaction *transaction, const std::string &str) {
    if (m_isScript) {
        return m_lua.run(transaction, str);
    }

#ifndef WIN32
    /*
     * Use fork()+execv() with the resolved m_file path to avoid shell
     * interpretation and PATH-lookup ambiguity.
     */
    std::array<int, 2> pipefd{};
    if (pipe(pipefd.data()) == -1) {
        return false;
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process: wire stdout to the pipe then exec the script.
        close(pipefd[0]);                // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe write end
        close(pipefd[1]);

        // Mutable copies required by execv()'s char* const argv[] signature.
        std::string file_copy = m_file;
        std::string str_copy  = str;

        std::vector<char *> argv;
        argv.push_back(file_copy.data());
        argv.push_back(str_copy.data());
        argv.push_back(nullptr);

        // execv() uses an exact path — no PATH lookup, no shell.
        execv(file_copy.data(), argv.data());

        // Only reached if execv() fails.
        _exit(1);
    }

    // Parent process: read all child output, retrying on EINTR.
    close(pipefd[1]);  // Close unused write end

    std::array<char, 512> buff{};
    std::stringstream s;
    ssize_t count = 0;

    do {
        count = read(pipefd[0], buff.data(), buff.size());
        if (count > 0) {
            s.write(buff.data(), count);
        } else if (count < 0 && errno == EINTR) {
            count = 1;  // Signal interrupted — keep looping.
        }
    } while (count > 0);

    close(pipefd[0]);

    // Reap child and treat abnormal exit or exec failure as no-match.
    int wstatus = 0;
    pid_t waited = 0;
    do {
        waited = waitpid(pid, &wstatus, 0);
    } while (waited == -1 && errno == EINTR);

    if (waited == -1 || !WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
        return false;
    }

    if (const std::string res = s.str(); res.size() > 1 && res[0] != '1') {
        return true;
    }

    return false;

#else  // WIN32
    /*
     * Windows: no fork()/execv(); use _popen() via the popen() alias
     * provided by src/compat/msvc.h. Command injection risk here is
     * accepted as a pre-existing platform limitation on Windows.
     */
    std::array<char, 512> buff{};
    std::stringstream s;

    const std::string openstr = m_param + " " + str;

    FILE *in = popen(openstr.c_str(), "r");
    if (in == nullptr) {
        return false;
    }

    while (fgets(buff.data(), static_cast<int>(buff.size()), in) != nullptr) {
        s << buff.data();
    }

    pclose(in);

    if (const std::string res = s.str(); res.size() > 1 && res[0] != '1') {
        return true;
    }

    return false;
#endif
}


}  // namespace operators
}  // namespace modsecurity
