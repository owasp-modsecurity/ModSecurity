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

#include "src/operators/rx_global.h"

#include <string>
#include <list>
#include <memory>

#include "src/operators/operator.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"

namespace modsecurity {
namespace operators {

#ifdef MODSEC_REGEX_DOLLAR_ENDONLY
static const bool kRxGlobalMultiLine = false;
#else
static const bool kRxGlobalMultiLine = true;
#endif

bool RxGlobal::init(const std::string &arg, std::string *error) {
    if (m_string->m_containsMacro == false) {
        m_re = new Regex(m_param, false, kRxGlobalMultiLine);
        if (m_re->hasError()) {
            if (error) {
                *error = "Invalid regular expression: " + m_param;
            }
            return false;
        }
    }

    return true;
}


bool RxGlobal::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string& input, RuleMessage &ruleMessage) {
    const Regex *re;
    std::unique_ptr<Regex> re_ptr;

    if (m_param.empty() && !m_string->m_containsMacro) {
        return true;
    }

    if (m_string->m_containsMacro) {
        std::string eparam(m_string->evaluate(transaction));
        re_ptr = std::make_unique<Regex>(eparam, false, kRxGlobalMultiLine);
        re = re_ptr.get();
    } else {
        re = m_re;
    }

    if (re->hasError()) {
        reportRegexCompileError(transaction, *re);
        /* A regex failure is neither a match nor a mismatch. Return the
         * value that the negation in Operator::evaluateInternal turns
         * into "no match", so that the negated form does not fire on
         * errors either (#3123). */
        return m_negation;
    }

    Utils::RegexResult regex_result;
    std::vector<Utils::SMatchCapture> captures;
    if (transaction && transaction->m_rules->m_pcreMatchLimit.m_set) {
        unsigned long match_limit = transaction->m_rules->m_pcreMatchLimit.m_value;
        regex_result = re->searchGlobal(input, captures, match_limit);
    } else {
        regex_result = re->searchGlobal(input, captures);
    }

    if (regex_result != Utils::RegexResult::Ok) {
        reportRegexSearchError(transaction, *re, regex_result, "rxGlobal");

        /* A regex failure is neither a match nor a mismatch. Return the
         * value that the negation in Operator::evaluateInternal turns
         * into "no match", so that the negated form does not fire on
         * errors either (#3123). */
        return m_negation;
    }


    if (rule && rule->hasCaptureAction() && transaction) {
        for (const Utils::SMatchCapture& capture : captures) {
            const std::string capture_substring(input.substr(capture.m_offset,capture.m_length));
            transaction->m_collections.m_tx_collection->storeOrUpdateFirst(
                std::to_string(capture.m_group), capture_substring);
            ms_dbg_a(transaction, 7, "Added regex subexpression TX." +
                std::to_string(capture.m_group) + ": " + capture_substring);
            transaction->m_matched.push_back(capture_substring);
        }
    }

    for (const auto & capture : captures) {
        logOffset(ruleMessage, capture.m_offset, capture.m_length);
    }

    if (captures.size() > 0) {
        return true;
    }

    return false;
}


}  // namespace operators
}  // namespace modsecurity
