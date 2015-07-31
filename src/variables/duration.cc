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

#include "variables/duration.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/assay.h"
#include "src/utils.h"

namespace ModSecurity {
namespace Variables {

std::list<std::pair<std::string, std::string>>
    Duration::evaluate(Assay *assay) {
    std::list<std::pair<std::string, std::string>> resl;
    std::string res;
    std::pair<std::string, std::string> pair;

    double e = cpu_seconds() - assay->start;

    res = std::to_string(e);

    pair = std::make_pair(std::string("DURATION"), std::string(res));
    resl.push_back(pair);

    return resl;
}


}  // namespace Variables
}  // namespace ModSecurity
