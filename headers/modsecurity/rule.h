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
#include <memory>
#include <utility>
#endif

#ifndef HEADERS_MODSECURITY_RULE_H_
#define HEADERS_MODSECURITY_RULE_H_

#include "modsecurity/transaction.h"
#include "modsecurity/collection/variable.h"


#ifdef __cplusplus

namespace modsecurity {
namespace Variables {
class Variable;
}
namespace actions {
class Action;
}
namespace operators {
class Operator;
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

    bool evaluate(Transaction *transaction, std::shared_ptr<RuleMessage> rm);
    bool evaluateActions(Transaction *transaction);
    std::vector<std::unique_ptr<collection::Variable>>
	getFinalVars(Transaction *trasn);
    void executeActionsAfterFullMatch(Transaction *trasn,
        bool containsDisruptive, std::shared_ptr<RuleMessage> ruleMessage);

    std::list<std::pair<std::shared_ptr<std::string>,
        std::shared_ptr<std::string>>> executeDefaultTransformations(
        Transaction *trasn, const std::string &value, bool multiMatch);

    bool executeOperatorAt(Transaction *trasn, std::string key,
        std::string value, std::shared_ptr<RuleMessage> rm);
    void executeActionsIndependentOfChainedRuleResult(Transaction *trasn,
        bool *b, std::shared_ptr<RuleMessage> ruleMessage);
    std::string resolveMatchMessage(std::string key, std::string value);
    void updateMatchedVars(Transaction *trasn, std::string key,
        std::string value);
    void cleanMatchedVars(Transaction *trasn);
    void updateRulesVariable(Transaction *trasn);

    std::vector<std::string> getActionNames();
    std::vector<actions::Action *> getActionsByName(const std::string& name);
    bool containsTag(const std::string& name, Transaction *t);


    int refCountDecreaseAndCheck() {
        m_referenceCount--;
        if (m_referenceCount == 0) {
            delete this;
            return 1;
        }
        return 0;
    }


    void refCountIncrease() {
        m_referenceCount++;
    }


    int m_accuracy;
    std::vector<actions::Action *> m_actionsConf;
    std::vector<actions::Action *> m_actionsRuntimePos;
    std::vector<actions::Action *> m_actionsRuntimePre;
    bool m_chained;
    Rule *m_chainedRule;
    std::string m_fileName;
    int m_lineNumber;
    std::string m_logData;
    std::string m_marker;
    int m_maturity;
    operators::Operator *m_op;
    int m_phase;
    std::string m_rev;
    long m_ruleId;
    bool m_secMarker;
    std::vector<Variables::Variable *> *m_variables;
    std::string m_ver;

 private:
    bool m_unconditional;
    int m_referenceCount;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_H_
