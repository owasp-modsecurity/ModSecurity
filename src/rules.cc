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

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#include "src/parser/driver.h"
#include "src/rule_marker.h"
#include "src/rule_with_actions.h"
#include "src/utils/https_client.h"



namespace modsecurity {

void Rules::fixDefaultActions(RulesWarnings *warnings, RulesErrors *errors) {
    for (size_t i = 0; i < m_rules.size(); i++) {
        auto &rule = m_rules[i];
        RuleWithActions *r = dynamic_cast<RuleWithActions *>(rule.get());
        if (!r) {
            continue;
        }
        if (dynamic_cast<RuleWithOperator *>(rule.get())) {
            RuleWithOperator *op = new RuleWithOperator(*dynamic_cast<RuleWithOperator *>(rule.get()));
            std::unique_ptr<RuleWithOperator> nrp(op);
            m_rules[i] = std::move(nrp);
        } else if (dynamic_cast<RuleUnconditional *>(rule.get())) {
            RuleUnconditional *un = new RuleUnconditional(*dynamic_cast<RuleUnconditional *>(rule.get()));
            std::unique_ptr<RuleUnconditional> nrp(un);
            m_rules[i] = std::move(nrp);
        } else if (dynamic_cast<RuleScript *>(rule.get())) {
            RuleScript *rs = new RuleScript(*dynamic_cast<RuleScript *>(rule.get()));
            std::unique_ptr<RuleScript> nrp(rs);
            m_rules[i] = std::move(nrp);
        } else {
            RuleWithActions *nr = new RuleWithActions(*dynamic_cast<RuleWithActions *>(rule.get()));
            std::unique_ptr<RuleWithActions> nrp(nr);
            m_rules[i] = std::move(nrp);
        }
        RuleWithActions *nr = dynamic_cast<RuleWithActions *>(m_rules[i].get());
        nr->clearDefaultActions();
        for (auto &a : m_defaultActions) {
            nr->addDefaultAction(a);
        }
        for (auto a : m_defaultTransformations) {
            nr->addDefaulTransformation(a);
        }
    }
}

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


void Rules::dump(std::stringstream &out) {
    for (auto &r : m_rules) {
        r->dump(out);
    }
}

}  // namespace modsecurity

