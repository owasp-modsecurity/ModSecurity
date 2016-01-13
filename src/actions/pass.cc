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

#include "actions/pass.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"

namespace modsecurity {
namespace actions {

Pass::Pass(std::string action)
    : Action(action) {
    this->action = action;
    this->action_kind = 2;
}


bool Pass::evaluate(Rule *rule, Transaction *transaction) {
    transaction->actions.clear();
    return true;
}


}  // namespace actions
}  // namespace modsecurity
