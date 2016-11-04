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

#include "src/actions/set_sid.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"


namespace modsecurity {
namespace actions {


bool SetSID::init(std::string *error) {
    m_collection_key = std::string(m_parser_payload, 0,
        m_parser_payload.length());

    if (m_collection_key.empty()) {
        error->assign("Missing collection key");
        return false;
    }

    return true;
}


bool SetSID::evaluate(Rule *rule, Transaction *t) {
    std::string colNameExpanded = MacroExpansion::expand(m_collection_key, t);

#ifndef NO_LOGS
    t->debug(8, "Session ID initiated with value: \'"
        + colNameExpanded + "\'.");
#endif

    t->m_collections.m_session_collection_key = colNameExpanded;
    t->m_collections.storeOrUpdateFirst("SESSIONID", colNameExpanded);

    return true;
}

}  // namespace actions
}  // namespace modsecurity
