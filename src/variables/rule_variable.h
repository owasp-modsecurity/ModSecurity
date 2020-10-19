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

#include <string>
#include <vector>
#include <list>

#ifndef SRC_VARIABLES_RULE_VARIABLE_H_
#define SRC_VARIABLES_RULE_VARIABLE_H_



namespace modsecurity {

class RuleWithActions;
class Transaction;
namespace variables {

class RuleVariable {
 public:
    RuleVariable()
        : m_rule(nullptr)
    { };

    RuleVariable(const RuleVariable &a)
        : m_rule(a.m_rule)
    { };


    void populate(const RuleWithActions *rule) {
        variables::Variable *rulev =
            dynamic_cast<variables::Variable *>(this);
        m_rule = rule;
    }

    const RuleWithActions *getRule() const noexcept {
        return m_rule;
    }

    virtual Variable* clone() = 0;

 private:
    const RuleWithActions *m_rule;
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_RULE_VARIABLE_H_
