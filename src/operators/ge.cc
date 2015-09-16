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

#include "operators/ge.h"

#include <string>

#include "operators/operator.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace operators {

bool Ge::evaluate(Assay *assay, const std::string &input) {
    std::string p = MacroExpansion::expand(param, assay);

    bool ge = atoll(p.c_str()) >= atoll(p.c_str());

    if (negation) {
        return !ge;
    }

    return ge;
}


}  // namespace operators
}  // namespace ModSecurity
