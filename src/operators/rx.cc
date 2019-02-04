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

#include "src/operators/rx.h"

#include <string>
#include <list>
#include <memory>

#include "src/operators/operator.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"

namespace modsecurity {
namespace operators {


bool Rx::init(const std::string &arg, std::string *error) {
    if (m_string->m_containsMacro == false) {
        std::string regex_error;
        m_re = new Regex(m_param);
        if (!m_re->ok(&regex_error)) {
            *error = "Failed to compile regular expression " + m_re->getPattern() + ": " + regex_error;
            return false;
        }
    }

    return true;
}


bool Rx::evaluate(Transaction *transaction, Rule *rule,
    const std::string& input, std::shared_ptr<RuleMessage> ruleMessage) {
    Regex *re;

    if (m_param.empty() && !m_string->m_containsMacro) {
        return true;
    }

    if (m_string->m_containsMacro) {
        std::string eparam(m_string->evaluate(transaction));
        re = new Regex(eparam);
        std::string regex_error;
        if (!re->ok(&regex_error)) {
            ms_dbg_a(transaction, 2,
                    "Failed to compile regular expression with macro "
                    + re->getPattern() + ": " + regex_error);
            delete re;
            return false;
        }
    } else {
        re = m_re;
    }

    regex::RegexMatch m;
    bool matched = re->search(input, &m, 9);

    if (matched && rule && rule->m_containsCaptureAction && transaction) {
        for (int i = 0; i < m.num_groups(); i++) {
            auto key = std::to_string(i);
            const std::string &value = m.group(i).string;
            transaction->m_collections.m_tx_collection->storeOrUpdateFirst(
                key, value);
            ms_dbg_a(transaction, 7, "Added regex subexpression TX." +
                key + ": " + value);
            transaction->m_matched.push_back(value);
        }
    }
    for (int i = 0; i < m.num_groups(); i++) {
        const regex::MatchGroup &g = m.group(i);
        logOffset(ruleMessage, g.offset, g.string.size());
    }

    if (m_string->m_containsMacro) {
        delete re;
    }

    return matched;
}


}  // namespace operators
}  // namespace modsecurity
