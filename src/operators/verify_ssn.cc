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

#include "src/operators/verify_ssn.h"

#include <string>
#include <memory>
#include <list>

#include "src/operators/operator.h"
#include "src/rule_with_actions.h"


namespace modsecurity {
namespace operators {


int VerifySSN::convert_to_int(const char c) {
    int n;
    if ((c >= '0') && (c <= '9')) {
        n = c - '0';
    } else if ((c >= 'A') && (c <= 'F')) {
        n = c - 'A' + 10;
    } else if ((c >= 'a') && (c <= 'f')) {
        n = c - 'a' + 10;
    } else {
        n = 0;
    }
    return n;
}


bool VerifySSN::verify(const char *ssnumber, int len) {
    int i;
    int num[9];
    int digits = 0;
    int area, serial, grp;
    int sequencial = 0;
    int repetitions = 0;
    std::string str_area;
    std::string str_grp;
    std::string str_serial;

    for (i = 0; i < len; i++) {
        if (isdigit(ssnumber[i])) {
            if (digits < 9)
                num[digits] = convert_to_int(ssnumber[i]);
            digits++;
        }
    }

    /* Not a valid number */
    if (digits != 9)
        goto invalid;

    for (i=0; i < 8; i++)   {
        if (num[i] == (num[i+1]-1))
            sequencial++;

        if (num[i] == num[i+1])
            repetitions++;
    }

    /* We are blocking when all numbers were sequencial or repeated */
    if (sequencial == 8)
        goto invalid;

    if (repetitions == 8)
        goto invalid;

    str_area.append(std::to_string(num[0]) + std::to_string(num[1]) +
        std::to_string(num[2]));

    str_grp.append(std::to_string(num[3]) + std::to_string(num[4]));

    str_serial.append(std::to_string(num[5]) + std::to_string(num[6])
        + std::to_string(num[7]) + std::to_string(num[8]));

    if (str_area.size() == 0 || str_grp.size() == 0
        || str_serial.size() == 0) {
        goto invalid;
    }

    area = atoi(str_area.c_str());
    grp = atoi(str_grp.c_str());
    serial = atoi(str_serial.c_str());

    /* Cannot has seroed fields */
    if (area == 0 || serial == 0 || grp == 0)
        goto invalid;

    /* More tests */
    if (area >= 740 || area == 666)
        goto invalid;

    return true;

invalid:
    return false;
}


bool VerifySSN::evaluate(Transaction *transaction,
    RuleWithActions *rule,
    const bpstd::string_view &input,
    RuleMessage *ruleMessage) {
    std::list<SMatch> matches;
    bool is_ssn = false;
    int i;

    if (m_param.empty()) {
        return false;
    }

    for (i = 0; i < input.size() - 1 && is_ssn == false; i++) {
        matches = m_re->searchAll(input.substr(i, input.size()).to_string());
        for (const auto & j : matches) {
            is_ssn = verify(j.str().c_str(), j.str().size());
            if (is_ssn) {
                logOffset(ruleMessage, j.offset(), j.str().size());
                if (rule && transaction && rule->hasCaptureAction()) {
                    transaction->m_collections.m_tx_collection->storeOrUpdateFirst(
                        "0", j.str());
                    ms_dbg_a(transaction, 7, "Added VerifySSN match TX.0: " + \
                        j.str());
                }

                goto out;
            }
        }
    }

out:
    return is_ssn;
}


}  // namespace operators
}  // namespace modsecurity
