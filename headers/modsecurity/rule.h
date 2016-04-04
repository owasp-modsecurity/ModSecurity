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

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
#include <list>
#endif

#ifndef HEADERS_MODSECURITY_RULE_H_
#define HEADERS_MODSECURITY_RULE_H_

#include "modsecurity/modsecurity.h"


#ifdef __cplusplus

namespace modsecurity {
namespace Variables {
class Variable;
}

class Rule {
 public:
    Rule(operators::Operator *_op,
            std::vector<Variables::Variable *> *_variables,
            std::vector<actions::Action *> *_actions,
            std::string fileName,
            int lineNumber);
    explicit Rule(std::string marker);

    ~Rule();
    bool evaluate(Transaction *transaction);
    bool evaluateActions(Transaction *transaction);

    operators::Operator *op;
    std::vector<actions::Action *> actions_conf;
    std::vector<actions::Action *> actions_runtime_pre;
    std::vector<actions::Action *> actions_runtime_pos;

    std::vector<std::string> getActionNames();

    std::vector<Variables::Variable *> *variables;
    int phase;
    long rule_id;

    Rule *chainedRule;
    bool chained;

    void refCountDecreaseAndCheck() {
        this->m_referenceCount--;
        if (this->m_referenceCount == 0) {
            delete this;
        }
    }

    void refCountIncrease() {
        this->m_referenceCount++;
    }

    std::string m_rev;
    std::string m_ver;

    std::string m_marker;
    bool m_secmarker;
    std::string m_fileName;
    int m_lineNumber;

    std::string m_log_message;
    std::string m_log_data;
    int m_accuracy;
    int m_maturity;

 private:
    bool m_unconditional;
    int m_referenceCount;
};

class RuleMessage {
 public:
    explicit RuleMessage(Rule *rule) :
        m_ruleFile(rule->m_fileName),
        m_ruleLine(rule->m_lineNumber),
        m_ruleId(rule->rule_id),
        m_rev(rule->m_rev),
        m_accuracy(rule->m_accuracy),
        m_message(std::string("")),
        m_data(std::string("")),
        m_severity(0),
        m_ver(rule->m_ver),
        m_maturity(rule->m_maturity),
        m_rule(rule),
        m_match(std::string(""))
    { }

    RuleMessage(Rule *rule, std::string message) :
        m_ruleFile(rule->m_fileName),
        m_ruleLine(rule->m_lineNumber),
        m_ruleId(rule->rule_id),
        m_rev(rule->m_rev),
        m_accuracy(rule->m_accuracy),
        m_message(message),
        m_data(std::string("")),
        m_severity(0),
        m_ver(rule->m_ver),
        m_maturity(rule->m_maturity),
        m_rule(rule),
        m_match(std::string(""))
    { }

    std::string m_match;
    std::string m_ruleFile;
    int m_ruleLine;
    int m_ruleId;
    std::string m_message;
    std::string m_data;
    int m_severity;
    std::string m_ver;
    std::string m_rev;
    int m_maturity;
    int m_accuracy;

    std::list<std::string> m_tags;

    Rule *m_rule;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_H_


