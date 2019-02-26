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


int Rules::append(Rules *from, const std::vector<int64_t> &ids, std::ostringstream *err) {
    size_t j = 0;
    for (; j < from->size(); j++) {
        RuleWithActions *rule = dynamic_cast<RuleWithActions*>(from->at(j).get());
        if (rule && std::binary_search(ids.begin(), ids.end(), rule->getId())) {
            if (err != NULL) {
                *err << "Rule id: " << std::to_string(rule->getId()) \
                     << " is duplicated" << std::endl;
            }
            return -1;
        }
    }
    m_rules.insert(m_rules.end(), from->m_rules.begin(), from->m_rules.end());
    return j;
}


bool Rules::insert(std::shared_ptr<Rule> rule) {
    return insert(rule, nullptr, nullptr);
}


bool Rules::insert(std::shared_ptr<Rule> rule, const std::vector<int64_t> *ids, std::ostringstream *err) {
    RuleWithActions*r = dynamic_cast<RuleWithActions*>(rule.get());
    if (r && ids != nullptr && err != nullptr
        && std::binary_search(ids->begin(), ids->end(), r->getId())) {
        if (err != NULL) {
            *err << "Rule id: " << std::to_string(r->getId()) \
                << " is duplicated" << std::endl;
        }
        return false;
    }
    m_rules.push_back(rule);
    return true;
}


size_t Rules::size() {
    return m_rules.size();
}


std::shared_ptr<Rule> Rules::operator[](int index) {
    return m_rules[index];
}


std::shared_ptr<Rule> Rules::at(int index) {
    return m_rules[index];
}


void Rules::dump() {
    for (int j = 0; j < m_rules.size(); j++) {
        std::cout << "    Rule ID: " << m_rules.at(j)->getReference();
        std::cout << "--" << m_rules.at(j) << std::endl;
    }
}


}  // namespace modsecurity

