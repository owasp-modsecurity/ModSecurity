/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include "src/actions/init_col.h"

#include <string>

#include "modsecurity/transaction.h"
/**
 * FIXME: rules_set.h inclusion is here due to ms_dbg_a.
 *        It should be removed.
 */
#include "modsecurity/rules_set.h"


namespace modsecurity {
namespace actions {


bool InitCol::init(std::string *error) {
    int posEquals = m_parserPayload.find("=");

    if (m_parserPayload.size() < 2) {
        error->assign("Something wrong with initcol format: too small");
        return false;
    }
    if (posEquals == std::string::npos) {
        error->assign("Something wrong with initcol format: missing " \
                "equals sign");
        return false;
    }

    m_collection_key = std::string(m_parserPayload, 0,  posEquals);

    if (m_collection_key != "ip" &&
        m_collection_key != "global" &&
        m_collection_key != "resource") {
        error->assign("Something wrong with initcol: collection must be " \
            "`ip' or `global'");
        return false;
    }

    return true;
}


bool InitCol::execute(Transaction *t) noexcept {
    std::string collectionName(getEvaluatedRunTimeString(t));

    if (m_collection_key == "ip") {
        t->m_collections.m_ip_collection_key = collectionName;
    } else if (m_collection_key == "global") {
        t->m_collections.m_global_collection_key = collectionName;
    } else if (m_collection_key == "resource") {
        t->m_collections.m_resource_collection_key = collectionName;
    } else {
        return false;
    }

    ms_dbg_a(t, 5, "Collection `" + m_collection_key + "' initialized with " \
        "value: " + collectionName);

    return true;
}

}  // namespace actions
}  // namespace modsecurity
