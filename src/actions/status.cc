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

#include "src/actions/status.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"


namespace modsecurity {
namespace actions {


bool Status::init(std::string *error) {
    try {
        m_status = std::stoi(m_parser_payload);
    } catch (...) {
        error->assign("Not a valid number: " + m_parser_payload);
        return false;
    }

    return true;
}


bool Status::evaluate(Rule *rule, Transaction *transaction, RuleMessage *rm) {
    rm->m_tmp_actions.push_back(this);
    return true;
}


void Status::fillIntervention(ModSecurityIntervention *i) {
    i->status = m_status;
    i->log = "Status";
}


}  // namespace actions
}  // namespace modsecurity
