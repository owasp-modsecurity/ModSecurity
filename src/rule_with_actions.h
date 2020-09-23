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

#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "modsecurity/variable_value.h"
#include "src/actions/action_allowed_in_sec_default_action.h"
#include "src/actions/action_type_rule_metadata.h"
#include "src/actions/action_with_execution.h"
#include "src/actions/action_with_run_time_string.h"
#include "src/actions/disruptive/disruptive_action.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/xmlns.h"
#include "src/rule_with_actions.h"
#include "src/rule_with_actions_properties.h"
#include "src/utils/string.h"
#include "src/transformation_result.h"

#ifdef __cplusplus


namespace modsecurity {
    using XmlNSs = std::vector<std::shared_ptr<actions::XmlNS> >;
    using XmlNSsPtr = std::vector<actions::XmlNS *>;


namespace actions {
class Action;
class Severity;
class LogData;
class Msg;
class Rev;
class XmlNS;
namespace transformations {
class Transformation;
}
}


class RuleWithActions : public Rule, public RuleWithActionsProperties {
 public:

    using Action = actions::Action;
    using Actions = std::vector<std::shared_ptr<Action>>;
    using ActionTypeRuleMetaData = actions::ActionTypeRuleMetaData;

    const unsigned int SEVERITY_NOT_SET = 10;
    const unsigned int ACCURACY_NOT_SET = 10;
    const unsigned int MATURITY_NOT_SET = 10;

    RuleWithActions(
        Actions *a,
        Transformations *t,
        std::unique_ptr<std::string> fileName,
        int lineNumber);

    RuleWithActions(const RuleWithActions &r);
    RuleWithActions &operator=(const RuleWithActions& r);

    virtual bool evaluate(Transaction *transaction) const override;

    void executeActionsIndependentOfChainedRuleResult(
        Transaction *trasaction) const;

    void executeActionsAfterFullMatch(
        Transaction *transaction) const;

    // FIXME: Pass a callback for the transformation execution.
    static void executeTransformation(
        Transaction *transaction,
        TransformationsResults *ret,
        Transformation *transformation);

    static void executeTransformation(
        Transaction *transaction,
        ModSecString &in,
        TransformationsResults *ret,
        Transformation *transformation);

    void executeTransformations(
        Transaction *transaction,
        const std::string &value,
        TransformationsResults &results) const;

    /* */
    bool containsTag(const std::string& name, const Transaction *t) const noexcept;
    bool containsMsg(const std::string& name, const Transaction *t) const noexcept;


    /* default Actions */
    void clearDefaultActions() {
        m_defaultActions.clear();
    }
    void addDefaultAction(std::shared_ptr<actions::Action> a);
    void addDefaulTransformation(std::shared_ptr<Transformation> t) {
        m_defaultActions.addTransformation(t);
    }


    /* rule id */
    // FIXME: not ever rule has an id. e.g. chained rule. */
    inline const RuleId getId() const noexcept { return m_ruleId; }
    void setId(int id) noexcept {
        m_ruleId = id;
    }


    /* capture */
    inline void setHasCapture(bool b) noexcept {
        m_containsCapture = b;
    }
    inline bool hasCapture() const noexcept {
        return m_containsCapture;
    }


    /* accuracy */
    inline const int getAccuracy() const noexcept {
        return m_accuracy;
    }
    inline void setAccuracy(unsigned int accuracy) noexcept {
        m_accuracy = accuracy;
    }
    inline bool hasAccuracy() const noexcept {
        return m_accuracy != ACCURACY_NOT_SET;
    }


    /* severity */
    inline int getSeverity() const noexcept {
        return m_severity;
    }
    inline void setSeverity(unsigned int severity) noexcept {
        m_severity = severity;
    }
    inline bool hasSeverity() const noexcept {
        return m_severity != SEVERITY_NOT_SET;
    }


    /* revision */
    inline const std::string getRevision() const noexcept {
        return m_revision;
    };
    inline void setRevision(const std::string &revision) noexcept {
        m_revision.assign(revision);
    }
    inline bool hasRevision() const noexcept {
        return m_revision != "";
    }


    /* version */
    inline const std::string getVersion() const noexcept {
        return m_version;
    };
    inline void setVersion(const std::string &version) noexcept {
        m_version.assign(version);
    }
    inline bool hasVersion() const noexcept {
        return m_version != "";
    }


    /* maturity */
    inline const int getMaturity() const noexcept {
        return m_maturity;
    }
    inline void setMaturity(unsigned int maturity) noexcept {
        m_maturity = maturity;
    }
    inline bool hasMaturity() const noexcept {
        return m_maturity != MATURITY_NOT_SET;
    }


    /* logData */
    inline std::shared_ptr<actions::LogData> getLogDataAction() const noexcept {
        return m_actionLogData;
    }
    std::string getLogData(const Transaction *t) const noexcept;
    inline void setLogDataAction(const std::shared_ptr<actions::LogData> &data) noexcept {
        m_actionLogData = data;
    }
    inline bool hasLogDataAction() const noexcept {
        return m_actionLogData != nullptr;
    }


    /* message */
    inline std::shared_ptr<actions::Msg> getMessageAction() const noexcept {
        return m_actionMsg;
    }
    std::string getMessage(const Transaction *t) const noexcept;
    inline void setMessageAction(const std::shared_ptr<actions::Msg> &msg) noexcept {
        m_actionMsg = msg;
    }
    inline bool hasMessageAction() const noexcept {
        return m_actionMsg != nullptr;
    }


    /* multimatch */
    inline bool processMultiMatch() const noexcept {
        return hasMultiMatch() || m_defaultActions.hasMultiMatch();
    }


    /* isDisruptive */
    inline bool isDisruptive() const {
        return hasDisruptiveAction() || ((m_defaultActions.hasBlock() || hasBlock()) && m_defaultActions.hasBlock());
    }


    /* logging */
    inline bool isItToBeLogged() const noexcept {
        if (hasNoLog()) {
            return false;
        }
        if (m_defaultActions.hasNoLog() && !hasNoLog()) {
            return false;
        }
        if (!hasDisruptiveAction() && !(hasBlock() || m_defaultActions.hasBlock())) {
            return false;
        }
        if (!m_defaultActions.hasDisruptiveAction() && !hasDisruptiveAction()) {
            return false;
        }
        return true;
    }

    inline bool isItToBeAuditLogged() const noexcept {
        if (hasAuditLog()) {
            return true;
        }
        if (m_defaultActions.hasAuditLog() && !hasNoAuditLog()) {
            return true;
        }
        if (isItToBeLogged()) {
            return true;
        }
        return false;
    }


    /* chained rule */
    /* FIXME: The chained rule needs to have its own class. */
    void setChainedNext(std::unique_ptr<RuleWithActions> r) {
        m_chainedRuleChild = std::move(r);
    }
    inline RuleWithActions *getChainedNext() const {
        return m_chainedRuleChild.get();
    }
    void setChainedParent(RuleWithActions *r) {
        m_chainedRuleParent = r;
    }
    inline RuleWithActions *getChainedParent() {
        return m_chainedRuleParent;
    }
    inline bool hasChainAction() const { return m_isChained == true; }
    inline void setHasChainAction(bool b) { m_isChained = b; }
    inline bool hasChainedParent() const { return m_chainedRuleParent != nullptr; }
    inline bool hasChainedChild() const { return m_chainedRuleChild.get() != nullptr; }



    XmlNSs getXmlNSs() const {
        XmlNSs dst;
        for (auto &a : m_XmlNSs) {
            dst.push_back(a);
        }
        return dst;
    }

    XmlNSsPtr getXmlNSsPtr() const {
        /**
         * FIXME: this is not conteplating SecRuleUpdateActionBy* yet.
         *
         */
        XmlNSsPtr dst;
        for (auto &a : m_XmlNSs) {
            dst.push_back(a.get());
        }
        
        return dst;
    }


    virtual void dump(std::stringstream &out) const override {
        out << "RuleWithActions" << std::endl;
    }

 private:
    void inline copyActionsWithRunTimeStrings(const RuleWithActions &o);

    RuleId m_ruleId;

    std::shared_ptr<RuleWithActions> m_chainedRuleChild;
    RuleWithActions *m_chainedRuleParent;

    /* xmlns */
    XmlNSs m_XmlNSs;

    /**
     * 1-9 where 9 is very strong and 1 has many false positives
     */
    unsigned int m_accuracy:4;
    /**
     * 1-9 where 9 is extensively tested and 1 is a brand new experimental rule
     */
    unsigned int m_maturity:4;
    /**
     * 0 - EMERGENCY: is generated from correlation of anomaly
     *                scoring data where there is an inbound
     *                attack and an outbound leakage.
     * 1 - ALERT: is generated from correlation where there is
     *                an inbound attack and an outbound application
     *                level error.
     * 2 - CRITICAL: Anomaly Score of 5. Is the highest severity
     *               level possible without correlation. It is
     *               normally generated by the web attack rules
     *               (40 level files).
     * 3 - ERROR: Error - Anomaly Score of 4. Is generated mostly
     *            from outbound leakage rules (50 level files).
     * 4 - WARNING: Anomaly Score of 3. Is generated by malicious
     *              client rules (35 level files).
     * 5 - NOTICE: Anomaly Score of 2. Is generated by the Protocol
     *             policy and anomaly files.
     * 6 - INFO
     * 7 - DEBUG
     **/
    unsigned int m_severity:4;
    bool m_containsCapture:1;
    bool m_isChained:1;
    std::string m_revision;
    std::string m_version;
    std::shared_ptr<actions::Msg> m_actionMsg;
    std::shared_ptr<actions::LogData> m_actionLogData;

    /* SecDefaultAction */
    RuleWithActionsProperties m_defaultActions;
};

}  // namespace modsecurity
#endif


#endif  // SRC_RULE_WITH_ACTIONS_H_

