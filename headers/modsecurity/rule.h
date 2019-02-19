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
#include "modsecurity/modsecurity.h"
#include "modsecurity/variable_value.h"


#ifdef __cplusplus

namespace modsecurity {
namespace variables {
class Variable;
class Variables;
}
namespace actions {
class Action;
class Severity;
class LogData;
class Msg;
class Rev;
class SetVar;
class Tag;
namespace transformations {
class Transformation;
}
}
namespace operators {
class Operator;
}

using TransformationResult = std::pair<std::shared_ptr<std::string>,
    std::shared_ptr<std::string>>;
using TransformationResults = std::list<TransformationResult>;

using Transformation = actions::transformations::Transformation;
using Transformations = std::vector<Transformation *>;

using Actions = std::vector<actions::Action *>;

using Tags = std::vector<actions::Tag *>;
using SetVars = std::vector<actions::SetVar *>;
using MatchActions = std::vector<actions::Action *>;

class RuleBase {
 public:
    RuleBase(std::unique_ptr<std::string> fileName, int lineNumber)
        : m_fileName(std::move(fileName)),
        m_lineNumber(lineNumber),
        m_phase(modsecurity::Phases::RequestHeadersPhase) {
        }

    virtual bool evaluate(Transaction *transaction,
        std::shared_ptr<RuleMessage> rm) = 0;

    std::shared_ptr<std::string> getFileName() {
        return m_fileName;
    }

    int getLineNumber() {
        return m_lineNumber;
    }

    int getPhase() { return m_phase; }
    void setPhase(int phase) { m_phase = phase; }

    virtual std::string getReference() {
        return *m_fileName + ":" + std::to_string(m_lineNumber);
    }


    virtual bool isMarker() { return false; }

 private:
    std::shared_ptr<std::string> m_fileName;
    int m_lineNumber;
    // FIXME: phase may not be neede to SecMarker.
    int m_phase;
};


class RuleMarker : public RuleBase {
 public:
    RuleMarker(
        std::string name,
        std::unique_ptr<std::string> fileName,
        int lineNumber)
        : RuleBase(std::move(fileName), lineNumber),
        m_name(std::make_shared<std::string>(name)) { }


    virtual bool evaluate(Transaction *transaction,
        std::shared_ptr<RuleMessage> rm) override {

        if (transaction->isInsideAMarker()) {
            if (*transaction->getCurrentMarker() == *m_name) {
                transaction->removeMarker();
                // FIXME: Move this to .cc
                //        ms_dbg_a(transaction, 4, "Out of a SecMarker " + *m_name);
            }
        }

        return true;
    };


    std::shared_ptr<std::string> getName() {
        return m_name;
    }

    bool isMarker() override { return true; }

 private:
    std::shared_ptr<std::string> m_name;
    int m_secmarker_skipped;
};


class RuleWithActions : public RuleBase {
 public:
    RuleWithActions(
        Actions *a,
        Transformations *t,
        std::unique_ptr<std::string> fileName,
        int lineNumber);

    ~RuleWithActions();

    void executeAction(Transaction *trans,
        bool containsBlock,
        std::shared_ptr<RuleMessage> ruleMessage,
        actions::Action *a,
        bool context);


    void executeTransformations(
        Transaction *trasn, const std::string &value, TransformationResults &ret);

    inline void executeTransformation(actions::transformations::Transformation *a,
        std::shared_ptr<std::string> *value,
        Transaction *trans,
        TransformationResults *ret,
        std::string *path,
        int *nth);

    void executeActionsIndependentOfChainedRuleResult(Transaction *trasn,
        bool *b, std::shared_ptr<RuleMessage> ruleMessage);
    void executeActionsAfterFullMatch(Transaction *trasn,
        bool containsDisruptive, std::shared_ptr<RuleMessage> ruleMessage);

    std::vector<actions::Action *> getActionsByName(const std::string& name,
        Transaction *t);
    bool containsTag(const std::string& name, Transaction *t);
    bool containsMsg(const std::string& name, Transaction *t);

    inline bool isChained() { return m_isChained == true; }
    inline bool hasCaptureAction() { return m_containsCaptureAction == true; }
    inline void setChained(bool b) { m_isChained = b; }
    inline bool hasDisruptiveAction() { return m_disruptiveAction != NULL; }
    inline bool hasBlockAction() { return m_containsStaticBlockAction == true; }
    inline bool hasMultimatch() { return m_containsMultiMatchAction == true; }
    inline bool hasBlock() { return m_containsStaticBlockAction == true; }

    inline bool hasLogData() { return m_logData != NULL; }
    std::string logData(Transaction *t);
    inline bool hasMsg() { return m_msg != NULL; }
    std::string msg(Transaction *t);
    inline bool hasSeverity() { return m_severity != NULL; }
    int severity();

    std::string m_marker;
    std::string m_rev;
    std::string m_ver;
    int m_accuracy;
    int m_maturity;
    int m_lineNumber;

    int64_t m_ruleId;

 private:
    /* actions */
    actions::Action *m_disruptiveAction;
    actions::LogData *m_logData;
    actions::Msg *m_msg;
    actions::Severity *m_severity;
    MatchActions m_actionsRuntimePos;
    SetVars m_actionsSetVar;
    Tags m_actionsTag;

    /* actions > transformations */
    Transformations m_transformations;

    bool m_containsCaptureAction:1;
    bool m_containsMultiMatchAction:1;
    bool m_containsStaticBlockAction:1;
    bool m_isChained:1;
};


class RuleWithOperator : public RuleWithActions {
 public:
    RuleWithOperator(operators::Operator *_op,
            variables::Variables *_variables,
            std::vector<actions::Action *> *_actions,
            Transformations *transformations,
            std::unique_ptr<std::string> fileName,
            int lineNumber);
    virtual ~RuleWithOperator();

    bool evaluate(Transaction *transaction,
        std::shared_ptr<RuleMessage> rm) override;

    void getVariablesExceptions(Transaction *t,
        variables::Variables *exclusion, variables::Variables *addition);
    inline void getFinalVars(variables::Variables *vars,
        variables::Variables *eclusion, Transaction *trans);

    bool executeOperatorAt(Transaction *trasn, std::string key,
        std::string value, std::shared_ptr<RuleMessage> rm);

    inline void updateMatchedVars(Transaction *trasn, const std::string &key,
        const std::string &value);
    inline void cleanMatchedVars(Transaction *trasn);

    inline bool isUnconditional() { return m_operator == NULL; }

    std::string getOperatorName();

    virtual std::string getReference() override {
        return std::to_string(m_ruleId);
    }

    std::unique_ptr<RuleWithOperator> m_chainedRuleChild;
    RuleWithOperator *m_chainedRuleParent;

 private:
    modsecurity::variables::Variables *m_variables;
    operators::Operator *m_operator;

    bool m_unconditional:1;
};

}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_H_
