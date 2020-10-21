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

#include "src/actions/block.h"

#include <iostream>
#include <string>
#include <memory>

#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "modsecurity/intervention.h"
#include "src/actions/data/status.h"

namespace modsecurity {
namespace actions {


bool Block::evaluate(RuleWithActions *rule, Transaction *transaction,
    std::shared_ptr<RuleMessage> rm) {
    ms_dbg_a(transaction, 8, "Marking request as disruptive.");

    for (auto &a : transaction->m_rules->m_rulesSetPhases[rule->getPhase()]->m_defaultActions) {
        if (a->isDisruptive() == false) {
            continue;
        }
        a->evaluate(rule, transaction, rm);
    }

    return true;
}


}  // namespace actions
}  // namespace modsecurity
