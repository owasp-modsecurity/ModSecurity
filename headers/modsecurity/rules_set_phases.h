
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#endif


#ifndef HEADERS_MODSECURITY_RULES_SET_PHASES_H_
#define HEADERS_MODSECURITY_RULES_SET_PHASES_H_

#include "modsecurity/rules_set_properties.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "modsecurity/rules.h"

#ifdef __cplusplus

namespace modsecurity {
class Rule;
namespace Parser {
class Driver;
}

/** @ingroup ModSecurity_CPP_API */
class RulesSetPhases {
 public:

    ~RulesSetPhases() {
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

    bool insert(Rule *rule) {
        if (rule->m_phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
            return false;
        }
        m_rules[rule->m_phase].push_back(rule);

        return true;
    }

    int append(RulesSetPhases *from, std::ostringstream *err) {
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

    void dump() {
        for (int i = 0; i <= modsecurity::Phases::NUMBER_OF_PHASES; i++) {
            std::vector<Rule *> rules = m_rules[i];
            std::cout << "Phase: " << std::to_string(i);
            std::cout << " (" << std::to_string(rules.size());
            std::cout << " rules)" << std::endl;
            for (int j = 0; j < rules.size(); j++) {
                std::cout << "    Rule ID: " << std::to_string(rules[j]->m_ruleId);
                std::cout << "--" << rules[j] << std::endl;
            }
        }
    }

    Rules *operator[](int index) { return &m_rules[index]; }
    Rules *at(int index) { return &m_rules[index]; }

    Rules m_rules[8];
};

}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_SET_PHASES_H_
