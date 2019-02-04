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

#include "src/operators/verify_cc.h"

#include <iostream>
#include <cstring>
#include <vector>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {

/**
 * Luhn Mod-10 Method (ISO 2894/ANSI 4.13)
 */
int VerifyCC::luhnVerify(const char *ccnumber, int len) {
    int sum[2] = { 0, 0 };
    int odd = 0;
    int digits = 0;
    int i;

    /* Weighted lookup table which is just a precalculated (i = index):
     *   i*2 + (( (i*2) > 9 ) ? -9 : 0)
     */
    /* weight lookup table */
    static const int wtable[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};


    /* Add up only digits (weighted digits via lookup table)
     * for both odd and even CC numbers to avoid 2 passes.
     */
    for (i = 0; i < len; i++) {
        if (ccnumber[i] >= (0 + 48) && ccnumber[i] <= (9 + 48)) {
            sum[0] += (!odd ? wtable[ccnumber[i] - '0'] : (ccnumber[i] - '0'));
            sum[1] += (odd ? wtable[ccnumber[i] - '0'] : (ccnumber[i] - '0'));
            odd = 1 - odd; /* alternate weights */
            digits++;
        }
    }

    /* No digits extracted */
    if (digits == 0) {
        return 0;
    }

    /* Do a mod 10 on the sum */
    sum[odd] %= 10;

    /* If the result is a zero the card is valid. */
    return sum[odd] ? 0 : 1;
}



bool VerifyCC::init(const std::string &param2, std::string *error) {
    m_re.reset(new modsecurity::regex::Regex(m_param));

    std::string regex_error;
    if (!m_re->ok(&regex_error)) {
        *error = "Failed to compile regular expression " + m_re->getPattern() + ": " + regex_error;
        return false;
    }
    return true;
}


bool VerifyCC::evaluate(Transaction *t, Rule *rule,
    const std::string& i, std::shared_ptr<RuleMessage> ruleMessage) {

    // ModSecurity v2 chacked for overlapping matches here,
    // so do we here
    for (const auto &m : m_re->searchAll(i, /* overlapping */ true)) {
        const auto &s = m.group(0).string;
        bool is_cc = luhnVerify(s.data(), s.size());
        if (is_cc) {
            if (t) {
                if (rule && t && rule->m_containsCaptureAction) {
                    t->m_collections.m_tx_collection->storeOrUpdateFirst(
                        "0", s);
                    ms_dbg_a(t, 7, "Added VerifyCC match TX.0: " + s);
                }
                ms_dbg_a(t, 9, "CC# match \"" + m_param +
                    "\" at " + i + ". [offset " +
                    std::to_string(m.group(0).offset) + "]");
            }
            return true;
        }
    }

    return false;
}


}  // namespace operators
}  // namespace modsecurity

