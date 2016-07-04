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


bool InitCol::init(std::string *error) {
    int posEquals = m_parser_payload.find("=");

    if (m_parser_payload.size() < 8) {
        error->assign("Something wrong with initcol format: too small");
        return false;
    }
    if (posEquals == std::string::npos) {
        error->assign("Something wrong with initcol format: missing equals sign");
        return false;
    }

    m_collection_key = std::string(m_parser_payload, 0,  posEquals);
    m_collection_value = std::string(m_parser_payload, posEquals + 1);

    if (m_collection_key != "ip" &&
        m_collection_key != "global" &&
        m_collection_key != "resource") {
        error->assign("Something wrong with initcol: collection must be `ip' or `global'");
        return false;
    }

    return true;
}


bool InitCol::evaluate(Rule *rule, Transaction *t) {
    std::string collectionName;
    collectionName = MacroExpansion::expand(m_collection_value, t);


    if (m_collection_key == "ip") {
        t->m_collections.m_ip_collection_key = collectionName;
    } else if (m_collection_key == "global") {
        t->m_collections.m_global_collection_key = collectionName;
    } else if (m_collection_key == "resource") {
        t->m_collections.m_resource_collection_key = collectionName;
    } else {
        return false;
    }

    t->debug(5, "Collection `" + m_collection_key + "' initialized with " \
        "value: " + collectionName);

    return true;
}

}  // namespace actions
}  // namespace modsecurity
