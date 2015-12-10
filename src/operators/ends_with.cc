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

#include "operators/ends_with.h"

#include <string>

#include "operators/operator.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace operators {


bool EndsWith::evaluate(Assay *assay, const std::string &input) {
    bool ret = false;
    std::string p = MacroExpansion::expand(param, assay);

    if (input.length() >= p.length()) {
        ret = (0 == input.compare(input.length() - p.length(),
            p.length(), p));
    }

    if (negation) {
        return !ret;
    }

    return ret;
}


}  // namespace operators
}  // namespace modsecurity
