/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/operators/detect_sqli.h"

#include <string>
#include <list>

#include "src/operators/operator.h"
#include "others/libinjection/src/libinjection.h"
#include "modsecurity/rule_with_actions.h"


namespace modsecurity {
namespace operators {


bool DetectSQLi::evaluate(Transaction *t, RuleWithActions *rule,
    const std::string& input, RuleMessage *ruleMessage) {
    char fingerprint[8];
    int issqli;

    issqli = libinjection_sqli(input.c_str(), input.length(), fingerprint);

    if (!t) {
        goto tisempty;
    }

    if (issqli) {
        t->m_matched.push_back(fingerprint);
        ms_dbg_a(t, 4, "detected SQLi using libinjection with " \
            "fingerprint '" + std::string(fingerprint) + "' at: '" +
            input + "'");
        if (rule && rule->hasCaptureAction()) {
            t->m_collections.m_tx_collection->storeOrUpdateFirst(
                "0", std::string(fingerprint));
            ms_dbg_a(t, 7, "Added DetectSQLi match TX.0: " + \
                std::string(fingerprint));
        }
    } else {
        ms_dbg_a(t, 9, "detected SQLi: not able to find an " \
            "inject on '" + input + "'");
    }

tisempty:
    return issqli != 0;
}


}  // namespace operators
}  // namespace modsecurity
