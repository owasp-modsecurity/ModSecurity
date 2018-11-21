
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
            Rules *rules = &m_rulesAtPhase[i];
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
        m_rulesAtPhase[rule->m_phase].push_back(rule);

        return true;
    }

    int append(RulesSetPhases *from, std::ostringstream *err) {
        int amount_of_rules = 0;
        std::vector<int64_t> v;

        for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
            v.reserve(m_rulesAtPhase[i].size());
            for (size_t z = 0; z < m_rulesAtPhase[i].size(); z++) {
                Rule *rule_ckc = m_rulesAtPhase[i].at(z);
                if (rule_ckc->m_secMarker == true) {
                    continue;
                }
                v.push_back(rule_ckc->m_ruleId);
            }
        }
        std::sort (v.begin(), v.end());

        for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
            int res = m_rulesAtPhase[i].append(from->at(i), v, err);
            if (res < 0) {
                return res;
            }
            amount_of_rules = amount_of_rules + res;
        }

        return amount_of_rules;
    }

    void dump() {
        for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
            Rules *rules = &m_rulesAtPhase[i];
            std::cout << "Phase: " << std::to_string(i);
            std::cout << " (" << std::to_string(rules->size());
            std::cout << " rules)" << std::endl;
            rules->dump();
        }
    }

    Rules *operator[](int index) { return &m_rulesAtPhase[index]; }
    Rules *at(int index) { return &m_rulesAtPhase[index]; }

 private:
    Rules m_rulesAtPhase[8];
};

}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_SET_PHASES_H_
