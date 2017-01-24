/*
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

#include "modsecurity/transaction.h"

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/regex.h"

namespace modsecurity {


void AnchoredSetVariable::resolveRegularExpression(const std::string &var,
    std::vector<const collection::Variable *> *l) {
    Utils::Regex *r = new Utils::Regex(var);
    for (const auto& x : *this) {
        int ret = Utils::regex_search(x.first, *r);
        if (ret <= 0) {
            continue;
        }
        l->insert(l->begin(), x.second);
    }
    delete r;
}


}  // namespace modsecurity
