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

#include "actions/block.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "modsecurity/intervention.h"

namespace modsecurity {
namespace actions {

Block::Block(std::string action)
    : Action(action) {
    this->action = action;
    this->action_kind = 2;
}


bool Block::evaluate(Rule *rule, Transaction *transaction) {
#ifndef NO_LOGS
    transaction->debug(8, "Running action block");
#endif
    for (Action *a : rule->actions_runtime_pos) {
        if (a->isDisruptive() == true) {
            transaction->m_actions.push_back(a);
        }
    }
    return true;
}

void Block::fill_intervention(ModSecurityIntervention *i) {
    i->disruptive = true;
}

}  // namespace actions
}  // namespace modsecurity
