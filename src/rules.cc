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

#include "modsecurity/rules.h"
#include "src/rule_with_actions.h"


namespace modsecurity {


int Rules::append(Rules *from) {
    m_rules.insert(m_rules.end(), from->m_rules.begin(), from->m_rules.end());
    if (!from->m_defaultActions.empty() || !from->m_defaultTransformations.empty()) {
        m_defaultActions.clear();
        m_defaultTransformations.clear();
        for (auto &a : from->m_defaultActions) {
            m_defaultActions.push_back(a);
        }
        for (auto &a : from->m_defaultTransformations) {
            m_defaultTransformations.push_back(a);
        }
    }
    return from->size();
}


bool Rules::insert(const std::shared_ptr<Rule> &rule) {
    m_rules.push_back(rule);
    return true;
}


size_t Rules::size() const {
    return m_rules.size();
}


std::shared_ptr<Rule> Rules::operator[](int index) const {
    return m_rules[index];
}


std::shared_ptr<Rule> Rules::at(int index) const {
    return m_rules[index];
}


void Rules::dump() {
    for (int j = 0; j < m_rules.size(); j++) {
        std::cout << "    Rule ID: " << m_rules.at(j)->getReference();
        std::cout << "--" << m_rules.at(j) << std::endl;
    }
}


}  // namespace modsecurity

