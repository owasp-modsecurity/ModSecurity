/**
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/variable_env.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <map>

#include "modsecurity/assay.h"
#include "src/utils.h"

extern char **environ;

namespace ModSecurity {

std::list<std::pair<std::string, std::string>>
    VariableEnv::evaluate(Assay *assay) {
    std::list<std::pair<std::string, std::string>> resl;

    std::map<std::string, std::string> envs;
    for (char **current = environ; *current; current++) {
        std::string env = std::string(*current);
        std::vector<std::string> key_value = split(env, '=');
        envs.insert(std::pair<std::string, std::string>("ENV:" + key_value[0],
            key_value[1]));
        if ("env:" + key_value[0] == name) {
            std::pair<std::string, std::string> pair;
            pair = std::make_pair(name, key_value[1]);
            resl.push_back(pair);
            return resl;
        }
    }

    for (auto& x : envs) {
        if ((x.first.substr(0, name.size() + 1).compare(name + ":") != 0)
            && (x.first != name)) {
            continue;
        }
        std::pair<std::string, std::string> pair;
        pair = std::make_pair(x.first, x.second);
        resl.push_back(pair);
    }

    return resl;
}


}  // namespace ModSecurity
