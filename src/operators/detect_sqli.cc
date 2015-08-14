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

#include "operators/detect_sqli.h"

#include <string>

#include "operators/operator.h"
#include "others/libinjection/src/libinjection.h"

namespace ModSecurity {
namespace operators {


bool DetectSQLi::evaluate(Assay *assay, const std::string &input) {
    char fingerprint[8];
    int issqli;
    // int capture;

    issqli = libinjection_sqli(input.c_str(), input.length(), fingerprint);
    // capture = apr_table_get(rule->actionset->actions, "capture") ? 1 : 0;

    if (issqli) {
        // set_match_to_tx(msr, capture, fingerprint, 0);
        if (assay) {
            assay->debug(4, "detected SQLi using libinjection with " \
                "fingerprint '" + std::string(fingerprint) + "' at: '" +
                input + "'");
        }
    } else {
        if (assay) {
            assay->debug(9, "detected SQLi: not able to find an inject on '" +
                input + "'");
        }
    }

    if (negation) {
        return issqli == 0;
    }

    return issqli != 0;
}


}  // namespace operators
}  // namespace ModSecurity
