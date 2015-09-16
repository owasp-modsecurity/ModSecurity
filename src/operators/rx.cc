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

#include "operators/rx.h"

#include <string>
#include <list>

#include "operators/operator.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace operators {



bool Rx::evaluate(Assay *assay, const std::string& input) {
    SMatch match;

    Regex re(MacroExpansion::expand(param, assay));
    if (regex_search(input, &match, re) && match.size() >= 1) {
        this->matched.push_back(match.match);
        return true;
    }

    return false;
}


}  // namespace operators
}  // namespace ModSecurity
