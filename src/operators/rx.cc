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

#include "src/operators/rx.h"

#include <string>
#include <list>
#include <memory>

#include "src/operators/operator.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"
#include "src/rule_with_actions.h"


namespace modsecurity {
namespace operators {


bool Rx::init(const std::string &file, std::string *error) {
    if (m_string->containsMacro() == false) {
        m_re = new Regex(m_param);
    }

    return true;
}


bool Rx::evaluate(Transaction *transaction,
    const RuleWithActions *rule,
    const bpstd::string_view &input,
    RuleMessage *ruleMessage) {
    std::list<SMatch> matches;
    Regex *re;

    if (m_param.empty() && !m_string->containsMacro()) {
        return true;
    }

    if (m_string->containsMacro()) {
        std::string eparam(m_string->evaluate(transaction));
        re = new Regex(eparam);
    } else {
        re = m_re;
    }

    matches = re->searchAll(input.c_str());
    if (rule && rule->hasCaptureAction() && transaction) {
        int i = 0;
        matches.reverse();
        for (const SMatch& a : matches) {
            transaction->m_collections.m_tx_collection->storeOrUpdateFirst(
                std::to_string(i), a.str());
            ms_dbg_a(transaction, 7, "Added regex subexpression TX." +
                std::to_string(i) + ": " + a.str());
            transaction->m_matched.push_back(a.str());
            i++;
        }
    }

    for (const auto & i : matches) {
        logOffset(ruleMessage, i.offset(), i.str().size());
    }

    if (m_string->containsMacro()) {
        delete re;
    }

    if (!matches.empty()) {
        return true;
    }

    return false;
}


}  // namespace operators
}  // namespace modsecurity
