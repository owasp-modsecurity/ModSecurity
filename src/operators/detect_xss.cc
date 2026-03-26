/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/operators/detect_xss.h"

#include <string>

#include "src/operators/operator.h"
#include "libinjection/src/libinjection.h"
#include "libinjection/src/libinjection_error.h"


namespace modsecurity {
namespace operators {


bool DetectXSS::evaluate(Transaction *t, RuleWithActions *rule,
    const std::string& input, RuleMessage &ruleMessage) {
    injection_result_t xss_result = libinjection_xss(input.c_str(),
        input.length());
    bool is_match = false;

    if (t) {
        switch (xss_result) {
            case LIBINJECTION_RESULT_TRUE:
                is_match = true;
                ms_dbg_a(t, 5, "detected XSS using libinjection.");
                if (rule && rule->hasCaptureAction()) {
                    t->m_collections.m_tx_collection->storeOrUpdateFirst(
                        "0", std::string(input));
                    ms_dbg_a(t, 7, "Added DetectXSS match TX.0: " + \
                        std::string(input));
                }
                break;
            case LIBINJECTION_RESULT_ERROR:
                is_match = true;
                ms_dbg_a(t, 4, "libinjection parser error during XSS "
                    "analysis; treating as match (fail-safe). Input: " + input);
                if (rule && rule->hasCaptureAction()) {
                    t->m_collections.m_tx_collection->storeOrUpdateFirst(
                        "0", std::string(input));
                    ms_dbg_a(t, 7, "Added DetectXSS error input TX.0: " + \
                        std::string(input));
                }
                break;
            case LIBINJECTION_RESULT_FALSE:
                ms_dbg_a(t, 9, "libinjection was not able to " \
                    "find any XSS in: " + input);
                break;
        }
    }

    if (t == nullptr) {
        is_match = xss_result == LIBINJECTION_RESULT_TRUE
            || xss_result == LIBINJECTION_RESULT_ERROR;
    }

    return is_match;
}


}  // namespace operators
}  // namespace modsecurity
