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

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include "src/operators/operator.h"
#include "src/utils/system.h"

#ifdef WIN32
#include "src/compat/msvc.h"
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>
#include <vector>
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
     * Use fork()+execv() to avoid shell interpretation and PATH ambiguity.
     * Execute the resolved m_file path directly instead of m_param.
     */
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return false;
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process
        close(pipefd[0]);                // Close read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipefd[1]);

        // Create mutable copies for execv() argument array
        std::string file_copy = m_file;
        std::string str_copy = str;

        std::vector<char *> argv;
        argv.push_back(file_copy.data());
        argv.push_back(str_copy.data());
        argv.push_back(nullptr);

        // Use execv() with the resolved path — avoids PATH lookup ambiguity
        execv(file_copy.data(), argv.data());

        // execv() failed: exit child immediately
        _exit(1);
    }

    // Parent process
    close(pipefd[1]);  // Close write end

    std::array<char, 512> buff{};
    std::stringstream s;
    ssize_t count;

    // Retry on EINTR so a signal does not silently truncate output
    while (true) {
        count = read(pipefd[0], buff.data(), buff.size());
        if (count > 0) {
            s.write(buff.data(), count);
        } else if (count == 0) {
            // EOF
            break;
        } else {
            if (errno == EINTR) {
                continue;
            }
            // Unrecoverable read error
            break;
        }
    }

    close(pipefd[0]);

    // Check child exit status; treat non-zero exit as no match
    int wstatus = 0;
    if (waitpid(pid, &wstatus, 0) == -1) {
        return false;
    }
    if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
        return false;
    }

    const std::string res = s.str();
    if (res.size() > 1 && res[0] != '1') {
        return true;
    }

    return false;

#else
    /*
     * Windows fallback: use popen() to invoke the script.
     */
    FILE *in;
    std::array<char, 512> buff{};
    std::stringstream s;

    std::string openstr;
    openstr.append(m_param);
    openstr.append(" ");
    openstr.append(str);

    if (!(in = popen(openstr.c_str(), "r"))) {
        return false;
    }

    while (fgets(buff.data(), static_cast<int>(buff.size()), in) != NULL) {
        s << buff.data();
    }

    pclose(in);

    const std::string res = s.str();
    if (res.size() > 1 && res[0] != '1') {
        return true;
    }

    return false;
#endif
}


}  // namespace operators
}  // namespace modsecurity
