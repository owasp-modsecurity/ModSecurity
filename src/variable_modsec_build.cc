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

#include "src/variable_modsec_build.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/assay.h"
#include "modsecurity/modsecurity.h"

namespace ModSecurity {

std::list<std::pair<std::string, std::string>>
    VariableModsecBuild::evaluate(Assay *assay) {
    std::list<std::pair<std::string, std::string>> resl;
    std::pair<std::string, std::string> pair;

    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << MODSECURITY_MAJOR;
    ss << std::setw(2) << std::setfill('0') << MODSECURITY_MINOR;
    ss << std::setw(2) << std::setfill('0') << MODSECURITY_PATCHLEVEL;
    ss << std::setw(2) << std::setfill('0') << MODSECURITY_TAG_NUM;

    pair = std::make_pair(std::string("MODSEC_BUILD"), ss.str());
    resl.push_back(pair);

    return resl;
}


}  // namespace ModSecurity
