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

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "modsecurity/rules_set_phases.h"
#include "modsecurity/rule.h"
#include "modsecurity/rules.h"
#include "modsecurity/modsecurity.h"



namespace modsecurity {

RulesSetPhases::~RulesSetPhases() {
    /** Cleanup the rules */
    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        Rules *rules = &m_rules[i];
        while (rules->empty() == false) {
            Rule *rule = rules->back();
            rules->pop_back();
            if (rule->refCountDecreaseAndCheck()) {
                rule = NULL;
            }
        }
    }
}

bool RulesSetPhases::insert(Rule *rule) {
    if (rule->m_phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
        return false;
    }
    m_rules[rule->m_phase].push_back(rule);
    return true;
}


int RulesSetPhases::append(RulesSetPhases *from, std::ostringstream *err) {
    int amount_of_rules = 0;
    std::vector<int64_t> v;

    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        v.reserve(m_rules[i].size());
        for (size_t z = 0; z < m_rules[i].size(); z++) {
            Rule *rule_ckc = m_rules[i].at(z);
            if (rule_ckc->m_secMarker == true) {
                continue;
            }
            v.push_back(rule_ckc->m_ruleId);
        }
    }
    std::sort (v.begin(), v.end());

    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        for (size_t j = 0; j < from->at(i)->size(); j++) {
            Rule *rule = from->at(i)->at(j);
            if (std::binary_search(v.begin(), v.end(), rule->m_ruleId)) {
                if (err != NULL) {
                    *err << "Rule id: " << std::to_string(rule->m_ruleId) \
                        << " is duplicated" << std::endl;
                }
                return -1;
            }
            amount_of_rules++;
            rule->refCountIncrease();
            m_rules[i].push_back(rule);
        }
    }

    return amount_of_rules;
}

void RulesSetPhases::dump() const {
    for (int i = 0; i <= modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::cout << "Phase: " << std::to_string(i);
        std::cout << " (" << std::to_string(m_rules[i].size());
        std::cout << " rules)" << std::endl;
        for (int j = 0; j < m_rules[i].size(); j++) {
            std::cout << "    Rule ID: " << std::to_string(m_rules[i][j]->m_ruleId);
            std::cout << "--" << m_rules[i][j] << std::endl;
        }
    }
}


}  // namespace modsecurity

