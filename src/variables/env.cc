/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/variables/env.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <map>

#include "modsecurity/transaction.h"

extern char **environ;

namespace modsecurity {
namespace variables {

void Env::evaluate(const Transaction *transaction, VariableValues *l) const noexcept {
    bool checkForKey = getVariableKey()->length() > 0;

    for (char **current = environ; *current; current++) {
        std::string env = std::string(*current);
        size_t pos = env.find_first_of("=");
        if (pos == std::string::npos) {
            continue;
        }
        std::unique_ptr<std::string> key(new std::string(env, 0, pos));
        std::unique_ptr<std::string> value(new std::string(env, pos+1, env.length() - (pos + 1)));

        if (checkForKey && *key != *getVariableKey()) {
            continue;
        }
        if (m_keyExclusion.toOmit(*key)) {
            continue;
        }

        l->emplace_back(std::make_shared<VariableValue>(
            std::move(value),
            std::move(key),
            getVariableKeyWithCollection()
        ));
    }

}


}  // namespace variables
}  // namespace modsecurity
