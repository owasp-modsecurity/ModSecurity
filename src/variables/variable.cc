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

#include "src/variables/variable.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "modsecurity/transaction.h"
#include "src/utils/string.h"


namespace modsecurity {
namespace Variables {


Variable::Variable(std::string name)
    : m_name(name),
    m_collectionName(""),
    m_isExclusion(false),
    m_isCount(false) {
    size_t a = m_name.find(":");
    if (a == std::string::npos) {
        a = m_name.find(".");
    }
    if (a != std::string::npos) {
        m_collectionName = utils::string::toupper(std::string(m_name, 0, a));
        m_name = std::string(m_name, a + 1, m_name.size());
        m_fullName = std::make_shared<std::string>(m_collectionName + ":" + m_name);
    } else {
        m_fullName = std::make_shared<std::string>(m_name);
        m_collectionName = m_name;
        m_name = "";
    }
}


Variable::Variable(std::string name, VariableKind kind)
    : m_name(name),
    m_collectionName(""),
    m_kind(kind),
    m_isExclusion(false),
    m_isCount(false) {
    size_t a = m_name.find(":");
    if (a == std::string::npos) {
        a = m_name.find(".");
    }
    if (a != std::string::npos) {
        m_collectionName = utils::string::toupper(std::string(m_name, 0, a));
        m_name = std::string(m_name, a + 1, m_name.size());
        m_fullName = std::make_shared<std::string>(m_collectionName + ":" + m_name);
    } else {
        m_fullName = std::make_shared<std::string>(m_name);
    }

}


std::string Variable::to_s(
    std::vector<Variable *> *variables) {
    std::string ret;
    std::string except("");
    for (int i = 0; i < variables->size() ; i++) {
        VariableModificatorExclusion *e =
            dynamic_cast<VariableModificatorExclusion *>(variables->at(i));
        if (e != NULL) {
            if (except.empty()) {
                except = except + *variables->at(i)->m_fullName.get();
            } else {
                except = except + "|" + *variables->at(i)->m_fullName.get();
            }
            continue;
        }

        if (i == 0) {
            ret = ret + *variables->at(i)->m_fullName.get();
        } else {
            ret = ret + "|" + *variables->at(i)->m_fullName.get();
        }
    }

    if (except.empty() == false) {
        ret = ret + ", except for: " + except;
    }
    return ret;
}


}  // namespace Variables
}  // namespace modsecurity
