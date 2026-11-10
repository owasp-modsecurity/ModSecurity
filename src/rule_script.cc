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

#include "src/rule_script.h"


namespace modsecurity {

bool RuleScript::init(std::string *err) {
    return m_lua.load(m_name, err);
}

bool RuleScript::evaluate(Transaction *trans,
    RuleMessage &ruleMessage) {

    ms_dbg_a(trans, 4, " Executing script: " + m_name + ".");

    bool containsDisruptive = false;

    executeActionsIndependentOfChainedRuleResult(trans,
        &containsDisruptive, ruleMessage);

    if (m_lua.run(trans) == false) {
        return false;
    }

    if (isChained()) {
        if (m_chainedRuleChild == nullptr) {
            ms_dbg_a(trans, 4, "Rule is marked as chained but there " \
                "isn't a subsequent rule.");
            return false;
        }
        ms_dbg_a(trans, 4, "Executing chained rule.");
        if (m_chainedRuleChild->evaluate(trans, ruleMessage) == false) {
            return false;
        }
    }

    executeActionsAfterFullMatch(trans, containsDisruptive, ruleMessage);

    return true;
}

}  // namespace modsecurity
