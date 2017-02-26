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

#include "src/actions/disruptive/deny.h"

#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "modsecurity/transaction.h"

namespace modsecurity {
namespace actions {
namespace disruptive {


bool Deny::evaluate(Rule *rule, Transaction *transaction,
    std::shared_ptr<RuleMessage> rm) {
#ifndef NO_LOGS
    transaction->debug(8, "Running action deny");
#endif
    std::string log;

    if (transaction->m_it.status == 200) {
        transaction->m_it.status = 403;
    }

    log.append("Access denied with code %d");
    log.append(" (phase ");
    log.append(std::to_string(rm->m_rule->m_phase - 1) + "). ");

    rm->m_disruptiveMessage.assign(log);
    transaction->m_it.disruptive = true;
    intervention::freeLog(&transaction->m_it);
    transaction->m_it.log = strdup(
        rm->disruptiveErrorLog().c_str());

    rm->m_isDisruptive = true;
    return true;
}


}  // namespace disruptive
}  // namespace actions
}  // namespace modsecurity
