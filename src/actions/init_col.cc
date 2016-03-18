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

#include "actions/init_col.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {

InitCol::InitCol(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind) {
}


bool InitCol::init(std::string *error) {
    int posEquals = action.find("=");
    int posInit = strlen("initcol:");

    if (action.size() < 8) {
        return false;
    }
    if (posEquals == std::string::npos) {
        return false;
    }

    m_collection_key = std::string(action, posInit,  posEquals - posInit);
    m_collection_value = std::string(action, posEquals + 1);

    return true;
}


bool InitCol::evaluate(Rule *rule, Transaction *transaction) {
    std::string collectionName;
    collectionName = MacroExpansion::expand(m_collection_value, transaction);
    std::cout << "Collection is not implemented yet, here is the ";
    std::cout << "collection name: " << collectionName << std::endl;
    return true;
}

}  // namespace actions
}  // namespace modsecurity
