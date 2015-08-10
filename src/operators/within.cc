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

#include "operators/within.h"

#include <string>

#include "operators/operator.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace operators {


bool Within::evaluate(Assay *assay, const std::string &str) {
    bool res = false;
    std::string paramTarget = MacroExpansion::expand(param, assay);

    if (str.empty()) {
        return true;
    }

    res = paramTarget.find(str) != std::string::npos;


    if (negation) {
        return !res;
    }

    return res;
}


}  // namespace operators
}  // namespace ModSecurity
