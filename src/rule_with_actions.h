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

#ifndef SRC_RULE_WITH_ACTIONS_H_
#define SRC_RULE_WITH_ACTIONS_H_

#include "modsecurity/transaction.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/variable_value.h"
#include "modsecurity/rule.h"


#ifdef __cplusplus

namespace modsecurity {

namespace actions {
class Action;
class Severity;
class LogData;
class Msg;
class Rev;
class SetVar;
class Tag;
class XmlNS;
namespace transformations {
class Transformation;
}
}

class TransformationResult {
 public:
    TransformationResult(
        ModSecStackString *after,
        std::string *transformation = nullptr)
        : m_transformation(transformation),
        m_after(*after) { };

    TransformationResult(const TransformationResult &t2) {
            m_after = t2.m_after;
            m_transformation = t2.m_transformation;
        };

    ModSecStackString *getAfter() {
        return &m_after;
    }

    std::string *getTransformationName() {
        return m_transformation;
    }
 private:
    ModSecStackString m_after;
    std::string *m_transformation;
};
using TransformationsResults = std::list<TransformationResult>;

class RuleWithActions : public Rule {
 public:

    using Transformation = actions::transformations::Transformation;
    using Transformations = std::vector<Transformation *>;

    using Action = actions::Action;
    using Actions = std::vector<Action *>;

    using Tags = std::vector<actions::Tag *>;
    using SetVars = std::vector<actions::SetVar *>;
    using MatchActions = std::vector<actions::Action *>;

    using XmlNSs = std::vector<actions::XmlNS *>;

    RuleWithActions(
        Actions *a,
        Transformations *t,
        std::unique_ptr<std::string> fileName,
        int lineNumber);
    ~RuleWithActions();


    virtual bool evaluate(Transaction *transaction) override;

    void executeActionsIndependentOfChainedRuleResult(
        Transaction *trasn);

    void executeActionsAfterFullMatch(
        Transaction *trasn);

    void executeAction(Transaction *trans,
        Action *a,
        bool context);


    inline void executeTransformation(
        Transaction *transaction,
        TransformationsResults *ret,
        Transformation *transformation);

    inline void executeTransformation(
        Transaction *transaction,
        ModSecStackString &in,
        TransformationsResults *ret,
        Transformation *transformation);

    void executeTransformations(
        Transaction *transaction,
        const std::string &value,
        TransformationsResults &results);


    void performLogging(Transaction *trans, bool lastLog = true);

    bool containsTag(const std::string& name, Transaction *t);
    bool containsMsg(const std::string& name, Transaction *t);


    void performLogging(Transaction *trans,
        RuleMessage &ruleMessage,
        bool lastLog = true);


    inline bool isChained() { return m_isChained == true; }
    inline void setChained(bool b) { m_isChained = b; }

    inline bool hasCaptureAction() { return m_containsCaptureAction == true; }
    inline bool hasDisruptiveAction() { return m_disruptiveAction != NULL; }
    inline bool hasMultimatch() { return m_containsMultiMatchAction == true; }

    void setBlockAction(bool a) { m_containsBlockAction = a; }
    inline bool hasBlockAction() { return m_containsBlockAction == true; }

    inline bool hasLogData() { return m_logData != NULL; }
    std::string getLogData(Transaction *t);

    inline bool hasMsg() { return m_msg != NULL; }
    std::string getMsg(Transaction *t);

    inline bool hasSeverity() { return m_severity != NULL; }
    int getSeverity();

    inline std::string *getRevision() { return &m_revision; }
    void setRevision(const std::string &revision) {
        m_revision.assign(revision);
    }

    inline std::string *getVersion() { return &m_version; }
    void setVersion(const std::string &version) {
        m_version.assign(version);
    }

    inline int getAccuracy() { return m_accuracy; }
    void setAccuracy(int accuracy) {
        m_accuracy = accuracy;
    }

    inline int getMaturity() { return m_maturity; }
    void setMaturity(int maturity) {
        m_maturity = maturity;
    }

    inline int64_t getId() { return m_ruleId; }
    void setId(int id) {
        m_ruleId = id;
    }

    void setChainedNext(std::unique_ptr<RuleWithActions> r) {
        m_chainedRuleChild = std::move(r);
    }

    inline RuleWithActions *getChainedNext() {
        return m_chainedRuleChild.get();
    }

    void setChainedParent(RuleWithActions *r) {
        m_chainedRuleParent = r;
    }

    inline RuleWithActions *getChainedParent() {
        return m_chainedRuleParent;
    }

    XmlNSs getXmlNSs() {
        /**
         * FIXME: this is not conteplating SecRuleUpdateActionBy* yet.
         *
         */
        return m_XmlNSs;
    }

    Tags getTags() {
        Tags tags = m_actionsTag;
        if (getChainedParent()) {
            auto parent = getChainedParent()->getTags();
            if (parent.size() > 1) {
                tags.insert( tags.end(), parent.begin(), parent.end() );
            }
        }
        return tags;
    }

 private:
    /* actions */
    actions::Action *m_disruptiveAction;
    actions::LogData *m_logData;
    actions::Msg *m_msg;
    actions::Severity *m_severity;

    MatchActions m_actionsRuntimePos;
    SetVars m_actionsSetVar;
    Tags m_actionsTag;
    XmlNSs m_XmlNSs;

    /* actions > transformations */
    Transformations m_transformations;

    int64_t m_ruleId;
    int m_accuracy;
    int m_maturity;
    std::string m_revision;
    std::string m_version;

    std::unique_ptr<RuleWithActions> m_chainedRuleChild;
    RuleWithActions *m_chainedRuleParent;

    bool m_containsCaptureAction:1;
    bool m_containsMultiMatchAction:1;
    bool m_containsBlockAction:1;
    bool m_isChained:1;
};

}  // namespace modsecurity
#endif


#endif  // SRC_RULE_WITH_ACTIONS_H_

