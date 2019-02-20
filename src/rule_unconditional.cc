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

#include "modsecurity/rule_unconditional.h"
#include "modsecurity/rule_message.h"

namespace modsecurity {


bool RuleUnconditional::evaluate(Transaction *trans,
    std::shared_ptr<RuleMessage> ruleMessage) {
    RuleWithActions::evaluate(trans, ruleMessage);

    // FIXME: This needs to be romeved on the runtime exeption review.
    bool containsBlock = false;

    ms_dbg_a(trans, 4, "(Rule: " + std::to_string(m_ruleId) \
        + ") Executing unconditional rule...");

    executeActionsIndependentOfChainedRuleResult(trans,
        &containsBlock, ruleMessage);

    executeActionsAfterFullMatch(trans, containsBlock, ruleMessage);

    /* last rule in the chain. */
    bool isItToBeLogged = ruleMessage->m_saveMessage;
    if (isItToBeLogged && !hasMultimatch()
        && !ruleMessage->m_message.empty()) {
        /* warn */
        trans->m_rulesMessages.push_back(*ruleMessage);

        /* error */
        if (!ruleMessage->m_isDisruptive) {
            trans->serverLog(ruleMessage);
       }
    }
    else if (hasBlock() && !hasMultimatch()) {
        /* warn */
        trans->m_rulesMessages.push_back(*ruleMessage);
        /* error */
        if (!ruleMessage->m_isDisruptive) {
            trans->serverLog(ruleMessage);
        }
    }

    return true;
}

}  // namespace modsecurity
