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

#include "operators/detect_xss.h"

#include <string>

#include "operators/operator.h"
#include "others/libinjection/src/libinjection.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace operators {


bool DetectXSS::evaluate(Assay *assay, const std::string &input) {
    int is_xss;

    is_xss = libinjection_xss(input.c_str(), input.length());

    if (assay) {
#ifndef NO_LOGS
        if (is_xss) {
            assay->debug(5, "detected XSS using libinjection.");
        } else {
            assay->debug(9, "libinjection was not able to " \
                "find any XSS in: " + input);
        }
#endif
    }

    if (negation) {
        return is_xss == 0;
    }

    return is_xss != 0;
}


}  // namespace operators
}  // namespace ModSecurity
