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
    size_t j = 0;
    for (; j < from->size(); j++) {
        RuleWithActions *rule = dynamic_cast<RuleWithActions*>(from->at(j).get());
    }
    m_rules.insert(m_rules.end(), from->m_rules.begin(), from->m_rules.end());
    return j;
}


bool Rules::insert(std::shared_ptr<Rule> rule) {
    RuleWithActions*r = dynamic_cast<RuleWithActions*>(rule.get());
    m_rules.push_back(rule);
    return true;
}


size_t Rules::size() {
    return m_rules.size();
}


void Rules::clean() {
    m_rules.clear();
}


std::shared_ptr<Rule> Rules::operator[](int index) {
    return m_rules[index];
}


std::shared_ptr<Rule> Rules::at(int index) {
    return m_rules[index];
}


void Rules::dump(std::stringstream &out) {
    for (auto &r : m_rules) {
        r->dump(out);
    }
}

}  // namespace modsecurity

