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

#include "src/operators/detect_xss.h"

#include <string>

#include "src/operators/operator.h"
#include "others/libinjection/src/libinjection.h"


namespace modsecurity {
namespace operators {


bool DetectXSS::evaluate(Transaction *t, Rule *rule,
    const std::string& input, std::shared_ptr<RuleMessage> ruleMessage) {
    int is_xss;

    is_xss = libinjection_xss(input.c_str(), input.length());

    if (t) {
        if (is_xss) {
#ifndef NO_LOGS
            t->debug(5, "detected XSS using libinjection.");
#endif
            if (rule && t
                && rule->getActionsByName("capture").size() > 0) {
                t->m_collections.m_tx_collection->storeOrUpdateFirst(
                    "0", std::string(input));
#ifndef NO_LOGS
                t->debug(7, "Added DetectXSS match TX.0: " + \
                    std::string(input));
#endif
            } 
        } else {
#ifndef NO_LOGS
            t->debug(9, "libinjection was not able to " \
            "find any XSS in: " + input);
#endif
            }
    }
    return is_xss != 0;
}


}  // namespace operators
}  // namespace modsecurity
