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

#include "src/actions/set_var.h"

#include <iostream>
#include <string>
#include <memory>

#include "modsecurity/transaction.h"
#include "modsecurity/rules.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"
#include "src/utils/string.h"
#include "src/variables/global.h"
#include "src/variables/ip.h"
#include "src/variables/resource.h"
#include "src/variables/session.h"
#include "src/variables/tx.h"
#include "src/variables/user.h"
#include "src/variables/variable.h"

namespace modsecurity {
namespace actions {


bool SetVar::init(std::string *error) {
    return true;
}


bool SetVar::evaluate(Rule *rule, Transaction *t) {
    std::string targetValue;
    std::string resolvedPre;

    if (m_string) {
        resolvedPre = m_string->evaluate(t);
    }

    std::string m_variableNameExpanded;
    std::vector<const collection::Variable *> l;

    auto *v = m_variable.get();
    Variables::Tx_DynamicElement *tx = dynamic_cast<Variables::Tx_DynamicElement *> (v);
    Variables::Session_DynamicElement *session = dynamic_cast<Variables::Session_DynamicElement *> (v);
    Variables::Ip_DynamicElement *ip = dynamic_cast<Variables::Ip_DynamicElement *> (v);
    Variables::Resource_DynamicElement *resource = dynamic_cast<Variables::Resource_DynamicElement *> (v);
    Variables::Global_DynamicElement *global = dynamic_cast<Variables::Global_DynamicElement *> (v);
    Variables::User_DynamicElement *user = dynamic_cast<Variables::User_DynamicElement *> (v);
    if (tx) {
        m_variableNameExpanded = tx->m_string->evaluate(t);
    } else if (session) {
        m_variableNameExpanded = session->m_string->evaluate(t);
    } else if (ip) {
        m_variableNameExpanded = ip->m_string->evaluate(t);
    } else if (resource) {
        m_variableNameExpanded = resource->m_string->evaluate(t);
    } else if (global) {
        m_variableNameExpanded = global->m_string->evaluate(t);
    } else if (user) {
        m_variableNameExpanded = user->m_string->evaluate(t);
    } else {
        m_variableNameExpanded = m_variable->m_name;
    }

    if (m_operation == setOperation) {
        targetValue = resolvedPre;
    } else if (m_operation == setToOneOperation) {
        targetValue = std::string("1");
    } else if (m_operation == unsetOperation) {
        //m_variable
        t->m_collections.del(m_variable->m_collectionName + ":" +
            m_variableNameExpanded);
        goto end;
    } else {
        int pre = 0;
        int value = 0;

        try {
            pre = stoi(resolvedPre);
        } catch (...) {
            pre = 0;
        }

        try {
            std::vector<const collection::Variable *> l;
            m_variable->evaluate(t, rule, &l);
            if (l.size() == 0) {
                value = 0;
            } else {
                value = stoi(l[0]->m_value);
                for (auto &i : l) {
                    delete i;
                }
            }
        } catch (...) {
            value = 0;
        }

        if (m_operation == sumAndSetOperation) {
            targetValue = std::to_string(value + pre);
        } else if (m_operation == substractAndSetOperation) {
            targetValue = std::to_string(value - pre);
        }
    }

#ifndef NO_LOGS
    t->debug(8, "Saving variable: " + m_variable->m_collectionName \
        + ":" + m_variableNameExpanded + " with value: " + targetValue);
#endif
        t->m_collections.storeOrUpdateFirst(m_variable->m_collectionName,
            m_variableNameExpanded,
            t->m_rules->m_secWebAppId.m_value, targetValue);
end:
    return true;
}

}  // namespace actions
}  // namespace modsecurity
