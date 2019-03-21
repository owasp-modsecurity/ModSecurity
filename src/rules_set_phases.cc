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

#include "modsecurity/rules_set_phases.h"
#include "src/rule_with_operator.h"
#include <iterator>

namespace modsecurity {


void RulesSetPhases::insert(std::shared_ptr<Rule> rule) {
    if (rule->getPhase() >= size()) {
        return;
    }
    m_rulesAtPhase[rule->getPhase()].insert(rule);
}


void RulesSetPhases::append(RulesSetPhases *from) {
    int phase = 0;
    for (auto &a : *from) {
         m_rulesAtPhase[phase++].append(&a);
    }
}


void RulesSetPhases::dump() {
    int phase = 0;
    for (auto &rules : m_rulesAtPhase) {
        std::cout << "Phase: " << std::to_string(phase++);
        std::cout << " (" << std::to_string(rules.size());
        std::cout << " rules)" << std::endl;
        rules.dump();
    }
}


Rules *RulesSetPhases::operator[](int index) {
    return &m_rulesAtPhase[index];
}


Rules *RulesSetPhases::at(int index) {
    return &m_rulesAtPhase[index];
}


}  // namespace modsecurity
