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

#include "variables/variations/count.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/assay.h"
#include "src/utils.h"

namespace ModSecurity {
namespace Variables {
namespace Variations {

std::list<std::pair<std::string, std::string>>
    Count::evaluate(Assay *assay) {
    std::list<std::pair<std::string, std::string>> reslIn;
    std::list<std::pair<std::string, std::string>> reslOut;
    std::pair<std::string, std::string> pair;
    int count = 0;

    reslIn = var->evaluate(assay);

    for (auto &a : reslIn) {
        count++;
    }

    std::string res = std::to_string(count);

    pair = std::make_pair(std::string(var->name), std::string(res));
    reslOut.push_back(pair);

    return reslOut;
}


}  // namespace Variations
}  // namespace Variables
}  // namespace ModSecurity
