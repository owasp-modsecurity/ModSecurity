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

/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc.
 *
 * Licensed under the Apache License, Version 2.0
 */

#include "src/operators/inspect_file.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>

#include "src/operators/operator.h"
#include "src/utils/system.h"

#ifdef WIN32
#include "src/compat/msvc.h"
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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
     * SECURITY HARDENING:
     * Replace shell-based popen() execution with fork()+execvp()
     * to avoid shell interpretation while preserving behavior.
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
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Create mutable copies (avoid const_cast)
        std::string param_copy = m_param;
        std::string str_copy = str;

        std::vector<char*> argv;
        argv.push_back(param_copy.data());
        argv.push_back(str_copy.data());
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());

        _exit(1); // exec failed
    }

    // Parent process
    close(pipefd[1]);

    std::stringstream s;
    std::array<char, 512> buff{};
    ssize_t count;

    while ((count = read(pipefd[0], buff.data(), buff.size())) > 0) {
        s.write(buff.data(), count);
    }

    close(pipefd[0]);
    waitpid(pid, nullptr, 0);

    const std::string res = s.str();

    if (res.size() > 1 && res[0] != '1') {
        return true; /* match */
    }

    return false;

#else
    /*
     * Windows fallback: preserve existing behavior
     */
    FILE *in;
    std::array<char, 512> buff{};
    std::stringstream s;
    std::string res;
    std::string openstr;

    openstr.append(m_param);
    openstr.append(" ");
    openstr.append(str);

    if (!(in = popen(openstr.c_str(), "r"))) {
        return false;
    }

    while (fgets(buff.data(), buff.size(), in) != NULL) {
        s << buff.data();
    }

    pclose(in);

    res.append(s.str());
    if (res.size() > 1 && res[0] != '1') {
        return true; /* match */
    }

    return false;
#endif
}

}  // namespace operators
}  // namespace modsecurity
