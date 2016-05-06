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

#include "actions/set_uid.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {


bool SetUID::init(std::string *error) {
    m_collection_key = std::string(action, 0, action.length());

    if (m_collection_key.empty()) {
        return false;
    }

    return true;
}


bool SetUID::evaluate(Rule *rule, Transaction *t) {
    std::string colNameExpanded = MacroExpansion::expand(m_collection_key, t);

#ifndef NO_LOGS
    t->debug(8, "User collection initiated with value: \'"
        + colNameExpanded + "\'.");
#endif

    t->m_collections.m_user_collection_key = colNameExpanded;
    t->m_collections.storeOrUpdateFirst("USERID", colNameExpanded);

    return true;
}

}  // namespace actions
}  // namespace modsecurity
