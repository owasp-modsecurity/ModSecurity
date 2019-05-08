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
#include "modsecurity/rules_set.h"

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
        ModSecStackString &after,
        const std::string *transformation = nullptr)
        : m_transformation(transformation),
        m_after(std::move(after)) { };
/*
    TransformationResult(const TransformationResult &t2) {
            m_after = t2.m_after;
            m_transformation = t2.m_transformation;
        };
*/
    ModSecStackString *getAfter() {
        return &m_after;
    }

    const std::string *getTransformationName() {
        return m_transformation;
    }

 private:
    ModSecStackString m_after;
    const std::string *m_transformation;
};

class TransformationsResults : public std::list<TransformationResult> {
 public:
    ModSecStackString::allocator_type::arena_type m_stackAllocator;
};


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


    virtual bool evaluate(Transaction *transaction) const override;

    void executeActionsIndependentOfChainedRuleResult(
        Transaction *trasn) const;

    void executeActionsAfterFullMatch(
        Transaction *trasn) const;

    void executeAction(Transaction *trans,
        Action *a,
        bool context) const;

    void executeAsDefaulAction(Transaction *trans,
        Action *a,
        bool context) const;

    inline void executeBlockAction(Transaction *transaction) const noexcept {
        for (auto &a : transaction->m_rules->m_defaultActions[getPhase()]) {
            if (a->isDisruptive() == false) {
                continue;
            }
            if (transaction->getRuleEngineState() != RulesSet::EnabledRuleEngine) {
                ms_dbg_a(transaction, 4, "Not running any disruptive action (or block): " \
                    + *a->getName() + ". SecRuleEngine is not On.");
                continue;
            }

            a->execute(transaction);
        }
    };

    inline void executeTransformation(
        Transaction *transaction,
        TransformationsResults *ret,
        Transformation *transformation) const;

    inline void executeTransformation(
        Transaction *transaction,
        ModSecStackString &in,
        TransformationsResults *ret,
        Transformation *transformation) const;

    void executeTransformations(
        Transaction *transaction,
        const std::string &value,
        TransformationsResults &results) const;


    void performLogging(Transaction *trans, bool lastLog = true) const;

    bool containsTag(const std::string& name, Transaction *t) const;
    bool containsMsg(const std::string& name, Transaction *t) const;


    void performLogging(Transaction *trans,
        RuleMessage &ruleMessage,
        bool lastLog = true) const;


    inline const bool isChained() const { return m_isChained == true; }
    inline void setChained(bool b) { m_isChained = b; }

    inline bool hasCaptureAction() const { return m_containsCaptureAction == true; }
    inline bool hasDisruptiveAction() const { return m_disruptiveAction != NULL; }
    inline bool hasMultimatch() const { return m_containsMultiMatchAction == true; }

    void setBlockAction(bool a) { m_containsBlockAction = a; }
    inline bool hasBlockAction() const { return m_containsBlockAction == true; }

    inline bool hasLogData() const { return m_logData != NULL; }
    std::string getLogData(Transaction *t) const;

    inline bool hasMsg() const { return m_msg != NULL; }
    std::string getMsg(Transaction *t) const;

    inline bool hasSeverity() const { return m_severity != NULL; }
    int getSeverity() const;

    inline const std::string *getRevision() const { return &m_revision; }
    void setRevision(const std::string &revision) {
        m_revision.assign(revision);
    }

    inline const std::string *getVersion() const { return &m_version; }
    void setVersion(const std::string &version) {
        m_version.assign(version);
    }

    inline int getAccuracy() const { return m_accuracy; }
    void setAccuracy(int accuracy) {
        m_accuracy = accuracy;
    }

    inline int getMaturity() const { return m_maturity; }
    void setMaturity(int maturity) {
        m_maturity = maturity;
    }

    inline RuleId getId() const { return m_ruleId; }
    void setId(int id) {
        m_ruleId = id;
    }

    void setChainedNext(std::unique_ptr<RuleWithActions> r) {
        m_chainedRuleChild = std::move(r);
    }

    inline const RuleWithActions *getChainedNext() const {
        return m_chainedRuleChild.get();
    }

    inline RuleWithActions *getChainedNextReference() {
        return m_chainedRuleChild.get();
    }

    void setChainedParent(RuleWithActions *r) {
        m_chainedRuleParent = r;
    }

    inline const RuleWithActions *getChainedParent() const {
        return m_chainedRuleParent;
    }

    XmlNSs getXmlNSs() const {
        /**
         * FIXME: this is not conteplating SecRuleUpdateActionBy* yet.
         *
         */
        return m_XmlNSs;
    }

    Tags getTags() const {
        Tags tags = m_actionsTag;
        if (getChainedParent()) {
            auto parent = getChainedParent()->getTags();
            if (parent.size() > 1) {
                tags.insert( tags.end(), parent.begin(), parent.end() );
            }
        }
        return tags;
    }


    virtual void dump(std::stringstream &out) const override {
        out << "RuleWithActions" << std::endl;
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

    RuleId m_ruleId;
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

