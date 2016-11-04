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

#include "src/actions/redirect.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {


bool Redirect::init(std::string *error) {
    m_url = m_parser_payload;
    m_status = 302;
    return true;
}


bool Redirect::evaluate(Rule *rule, Transaction *transaction) {
    m_urlExpanded = MacroExpansion::expand(m_url, transaction);
    transaction->m_actions.push_back(this);
    return true;
}


void Redirect::fillIntervention(ModSecurityIntervention *i) {
    /* if it was changed before, lets keep it. */
    if (i->status == 200) {
        i->status = m_status;
    }
    i->url = m_urlExpanded.c_str();
    i->log = "Redirecting";
    i->disruptive = true;
}


}  // namespace actions
}  // namespace modsecurity
