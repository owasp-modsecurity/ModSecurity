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


bool RulesSetPhases::insert(std::shared_ptr<Rule> rule) {
    if (rule->getPhase() >= modsecurity::Phases::NUMBER_OF_PHASES) {
        return false;
    }
    m_rulesAtPhase[rule->getPhase()].insert(rule);

    return true;
}


int RulesSetPhases::append(RulesSetPhases *from, std::ostringstream *err) {
    int amount_of_rules = 0;
    std::vector<int64_t> v;

    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        v.reserve(m_rulesAtPhase[i].size());
        for (size_t z = 0; z < m_rulesAtPhase[i].size(); z++) {
            RuleWithOperator *rule_ckc = dynamic_cast<RuleWithOperator *>(m_rulesAtPhase[i].at(z).get());
            if (!rule_ckc) {
                continue;
            }
            v.push_back(rule_ckc->getId());
        }
    }
    std::sort (v.begin(), v.end());

    for (int phase = 0; phase < modsecurity::Phases::NUMBER_OF_PHASES; phase++) {
        int res = m_rulesAtPhase[phase].append(from->at(phase), v, err);
        if (res < 0) {
            return res;
        }
        amount_of_rules = amount_of_rules + res;


        /**
         * An action set in a child will overwrite an action set on a parent.
         *
         */
        std::vector<std::shared_ptr<actions::Action> > *actions_to = &at(phase)->m_defaultActions;
        std::vector<std::shared_ptr<actions::transformations::Transformation> > *actions_t_to = &at(phase)->m_defaultTransformations;
        if (actions_to->size() == 0 || actions_t_to->size() == 0) {
            std::vector<std::shared_ptr<actions::Action> > *actions_from = &from->at(phase)->m_defaultActions;

            actions_to->clear();
            for (size_t j = 0; j < actions_from->size(); j++) {
                actions_to->push_back(actions_from->at(j));
            }

            std::vector<std::shared_ptr<actions::transformations::Transformation> > *actions_t_from = &from->at(phase)->m_defaultTransformations;
            actions_t_to->clear();
            for (size_t j = 0; j < actions_t_from->size(); j++) {
                actions_t_to->push_back(actions_t_from->at(j));
            }
            at(phase)->fixDefaultActions();
        }
    }

    return amount_of_rules;
}

void RulesSetPhases::dump() const {
    for (int i = 0; i <= modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::cout << "Phase: " << std::to_string(i);
        std::cout << " (" << std::to_string(m_rulesAtPhase[i].size());
        std::cout << " rules)" << std::endl;
        m_rulesAtPhase[i].dump();
    }
}


}  // namespace modsecurity

