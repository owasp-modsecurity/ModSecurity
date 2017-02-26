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
#include "src/macro_expansion.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"

namespace modsecurity {
namespace operators {



bool Rx::evaluate(Transaction *transaction, Rule *rule,
    const std::string& input, std::shared_ptr<RuleMessage> ruleMessage) {
    SMatch match;
    std::list<SMatch> matches;

    if (m_param.empty()) {
        return true;
    }

    matches = m_re->searchAll(input);
    if (rule && rule->getActionsByName("capture").size() > 0 && transaction) {
        int i = 0;
        matches.reverse();
        for (const SMatch& a : matches) {
            transaction->m_collections.storeOrUpdateFirst("TX",
                std::to_string(i), a.match);
            transaction->debug(7, "Added regex subexpression TX." +
                std::to_string(i) + ": " + a.match);
            transaction->m_matched.push_back(a.match);
            i++;
        }
    }

    for (const auto & i : matches) {
        logOffset(ruleMessage, i.m_offset, i.m_length);
    }

    if (matches.size() > 0) {
        return true;
    }

    return false;
}


}  // namespace operators
}  // namespace modsecurity
