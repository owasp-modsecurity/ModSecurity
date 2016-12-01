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

#include "src/actions/disruptive/redirect.h"

#include <string.h>
#include <iostream>
#include <string>


#include "modsecurity/transaction.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {
namespace disruptive {


bool Redirect::init(std::string *error) {
    m_url = m_parser_payload;
    m_status = 302;
    return true;
}


bool Redirect::evaluate(Rule *rule, Transaction *transaction,
    RuleMessage *rm) {
    m_urlExpanded = MacroExpansion::expand(m_url, transaction);
    std::string log;

    /* if it was changed before, lets keep it. */
    if (transaction->m_it.status == 200) {
        transaction->m_it.status = m_status;
    }
    log.append("Access denied with code %d");
    log.append(" (phase ");
    log.append(std::to_string(rm->m_rule->phase - 1) + "). ");

    intervention::freeUrl(&transaction->m_it);
    transaction->m_it.url = strdup(m_urlExpanded.c_str());
    transaction->m_it.disruptive = true;
    intervention::freeLog(&transaction->m_it);
    transaction->m_it.log = strdup(
        rm->disruptiveErrorLog(transaction, log).c_str());

    return true;
}


}  // namespace disruptive
}  // namespace actions
}  // namespace modsecurity
