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

#include "operators/verify_cc.h"

#include <pcre.h>
#include <iostream>
#include <cstring>
#include <vector>

#include "operators/operator.h"


namespace ModSecurity {
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



bool VerifyCC::init(const std::string &param2, const char **error) {
    std::vector<std::string> vec;
    const char *errptr = NULL;
    int erroffset = 0;

    m_pc = pcre_compile(param.c_str(), PCRE_DOTALL|PCRE_MULTILINE,
        &errptr, &erroffset, NULL);
    m_pce = pcre_study(m_pc, PCRE_STUDY_JIT_COMPILE, &errptr);

    if ((m_pc == NULL) || (m_pce == NULL)) {
        *error = errptr;
        return false;
    }

    return true;
}


bool VerifyCC::evaluate(Assay *assay, const std::string &i) {
    int offset = 0;
    bool is_cc = false;
    int target_length = i.length();

    for (offset = 0; offset < target_length; offset++) {
        std::string match;
        int ovector[33];
        memset(ovector, 0, sizeof(ovector));
        int ret = pcre_exec(m_pc, m_pce, i.c_str(), i.size(), offset,
            0, ovector, 33) > 0;

        /* If there was no match, then we are done. */
        if (ret == PCRE_ERROR_NOMATCH) {
            break;
        }
        if (ret < 0) {
            return false;
        }

        if (ret > 0) {
            match = std::string(i, ovector[0], ovector[1] - ovector[0]);
            is_cc = luhnVerify(match.c_str(), match.size());
            if (is_cc) {
                if (assay) {
#ifndef NO_LOGS
                    assay->debug(9, "CC# match \"" + param +
                        "\" at " + i + ". [offset " +
                        std::to_string(offset) + "]");
#endif
                }
                return true;
            }
        }
    }

    return false;
}


}  // namespace operators
}  // namespace ModSecurity
