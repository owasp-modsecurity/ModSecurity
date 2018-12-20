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

#include "src/parser/driver.h"

#include "src/parser/seclang-parser.hh"
#include "modsecurity/audit_log.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/parser/default_driver_trail.h"


namespace modsecurity {
namespace Parser {


int DefaultDriverTrail::addSecMarker(std::string marker) {
    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        Rule *rule = new Rule(marker);
        rule->m_phase = i;
        m_rules[i].push_back(rule);
    }
    return 0;
}


int DefaultDriverTrail::addSecAction(Rule *rule) {
    if (rule->m_phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
        m_error << "Unknown phase: " << std::to_string(rule->m_phase);
        m_error << std::endl;
        return false;
    }

    m_rules[rule->m_phase].push_back(rule);

    return true;
}


int DefaultDriverTrail::addSecRuleScript(Rule *rule) {
    m_rules[rule->m_phase].push_back(dynamic_cast<RuleScript *>(rule));
    return true;
}


int DefaultDriverTrail::addSecRule(Rule *rule) {
    if (rule->m_phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
        m_error << "Unknown phase: " << std::to_string(rule->m_phase);
        m_error << std::endl;
        return false;
    }

    if (m_lastRule && m_lastRule->m_chained) {
        if (m_lastRule->m_chainedRuleChild == NULL) {
            rule->m_phase = m_lastRule->m_phase;
            if (rule->m_theDisruptiveAction) {
                m_error << "Disruptive actions can only be specified by";
                m_error << " chain starter rules.";
                return false;
            }
            m_lastRule->m_chainedRuleChild = rule;
            rule->m_chainedRuleParent = m_lastRule;
            return true;
        } else {
            Rule *a = m_lastRule->m_chainedRuleChild;
            while (a->m_chained && a->m_chainedRuleChild != NULL) {
                a = a->m_chainedRuleChild;
            }
            if (a->m_chained && a->m_chainedRuleChild == NULL) {
                a->m_chainedRuleChild = rule;
                rule->m_chainedRuleParent = a;
                if (a->m_theDisruptiveAction) {
                    m_error << "Disruptive actions can only be ";
                    m_error << "specified by chain starter rules.";
                    return false;
                }
                return true;
            }
        }
    }


    /*
     * Checking if the rule has an ID and also checking if this ID is not used
     * by other rule
     */
    if (rule->m_ruleId == 0) {
        m_error << "Rules must have an ID. File: ";
        m_error << rule->m_fileName << " at line: ";
        m_error << std::to_string(rule->m_lineNumber) << std::endl;
        return false;
    }
    for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::vector<Rule *> rules = m_rules[i];
        for (int j = 0; j < rules.size(); j++) {
            if (rules[j]->m_ruleId == rule->m_ruleId) {
                m_error << "Rule id: " << std::to_string(rule->m_ruleId) \
                    << " is duplicated" << std::endl;
                return false;
            }
        }
    }

    m_lastRule = rule;
    m_rules[rule->m_phase].push_back(rule);
    return true;
}


}  // namespace Parser
}  // namespace modsecurity
