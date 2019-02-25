/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "modsecurity/rule.h"

#include <stdio.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/rule_with_actions.h"
#include "src/actions/accuracy.h"
#include "src/actions/block.h"
#include "src/actions/capture.h"
#include "src/actions/log_data.h"
#include "src/actions/msg.h"
#include "src/actions/maturity.h"
#include "src/actions/multi_match.h"
#include "src/actions/rev.h"
#include "src/actions/log.h"
#include "src/actions/no_log.h"
#include "src/actions/set_var.h"
#include "src/actions/severity.h"
#include "src/actions/tag.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/xmlns.h"
#include "src/utils/string.h"


namespace modsecurity {

RuleWithActions::RuleWithActions(
    Actions *actions,
    Transformations *transformations,
    std::unique_ptr<std::string> fileName,
    int lineNumber)
    : Rule(std::move(fileName), lineNumber),
    m_ruleId(0),
    m_chainedRuleChild(nullptr),
    m_chainedRuleParent(nullptr),
    m_disruptiveAction(nullptr),
    m_logData(nullptr),
    m_msg(nullptr),
    m_actionsRuntimePos(),
    m_actionsSetVar(),
    m_actionsTag(),
    m_XmlNSs(),
    m_defaultActionDisruptiveAction(nullptr),
    m_defaultActionLogData(nullptr),
    m_defaultActionMsg(nullptr),
    m_defaultActionActionsRuntimePos(),
    m_defaultActionActionsSetVar(),
    m_defaultActionActionsTag(),
    m_transformations(transformations != nullptr ? *transformations : Transformations()),
    m_defaultTransformations(),
    m_severity(SEVERITY_NOT_SET),
    m_revision(""),
    m_version(""),
    m_accuracy(ACCURACY_NOT_SET),
    m_maturity(MATURITY_NOT_SET),
    m_containsCaptureAction(false),
    m_containsLogAction(false),
    m_containsNoLogAction(false),
    m_containsMultiMatchAction(false),
    m_containsStaticBlockAction(false),
    m_defaultSeverity(SEVERITY_NOT_SET),
    m_defaultRevision(""),
    m_defaultVersion(""),
    m_defaultAccuracy(ACCURACY_NOT_SET),
    m_defaultMaturity(MATURITY_NOT_SET),
    m_defaultContainsCaptureAction(false),
    m_defaultContainsLogAction(false),
    m_defaultContainsNoLogAction(false),
    m_defaultContainsMultiMatchAction(false),
    m_defaultContainsStaticBlockAction(false),
    m_isChained(false) {

    if (actions) {
        for (actions::Action *a : *actions) {
            addAction(a);
        }
    }
}

void RuleWithActions::addDefaultAction(std::shared_ptr<actions::Action> a) {
    if (a->action_kind == Action::ConfigurationKind) {
        a->evaluate(this, NULL);
        return;
    }

    if (a->action_kind == Action::ConfigurationKind) {
        if (dynamic_cast<actions::Accuracy *>(a.get())) {
            actions::Accuracy *accuracy = dynamic_cast<actions::Accuracy *>(a.get());
            m_defaultAccuracy = accuracy->getAccuracy();
        } else if (dynamic_cast<actions::Rev *>(a.get())) {
            actions::Rev *rev = dynamic_cast<actions::Rev *>(a.get());
            m_defaultRevision = rev->getRevision();
        } else {
            a->evaluate(this, NULL);
        }
        return;
    }

    if (a->action_kind == Action::RunTimeOnlyIfMatchKind) {
        if (dynamic_cast<actions::Capture *>(a.get())) {
            m_defaultContainsCaptureAction = true;
        } else if (dynamic_cast<actions::MultiMatch *>(a.get())) {
            m_defaultContainsMultiMatchAction = true;
        } else if (dynamic_cast<actions::Severity *>(a.get())) {
            actions::Severity *severity = dynamic_cast<actions::Severity *>(a.get());
            setDefaultActionSeverity(severity->m_severity);
        } else if (dynamic_cast<actions::Maturity *>(a.get())) {
            actions::Maturity *maturity = dynamic_cast<actions::Maturity *>(a.get());
            setDefaultActionMaturity(maturity->getMaturity());
        } else if (dynamic_cast<actions::LogData *>(a.get())) {
            m_defaultActionLogData = std::static_pointer_cast<actions::LogData>(a);
        } else if (dynamic_cast<actions::Msg *>(a.get())) {
            m_defaultActionMsg = std::static_pointer_cast<actions::Msg>(a);
        } else if (dynamic_cast<actions::SetVar *>(a.get())) {
            m_defaultActionActionsSetVar.push_back(std::static_pointer_cast<actions::SetVar>(a));
        } else if (dynamic_cast<actions::Tag *>(a.get())) {
            m_defaultActionActionsTag.push_back(std::static_pointer_cast<actions::Tag>(a));
        } else if (dynamic_cast<actions::Log *>(a.get())) {
            m_defaultContainsLogAction = true;
        } else if (dynamic_cast<actions::NoLog *>(a.get())) {
            m_defaultContainsNoLogAction = true;
        } else if (dynamic_cast<actions::Block *>(a.get())) {
            m_defaultActionActionsRuntimePos.push_back(a);
            m_defaultContainsStaticBlockAction = true;
        } else if (a->isDisruptive() == true) {
            m_defaultActionDisruptiveAction = a;
        } else {
            m_defaultActionActionsRuntimePos.push_back(a);
        }
        return;
    }

    std::cout << "General failure, action: " << *a->m_name;
    std::cout << " has an unknown type." << std::endl;
    throw;

}

void RuleWithActions::addAction(actions::Action *a) {
    if (a->action_kind == Action::ConfigurationKind) {
        if (dynamic_cast<actions::Accuracy *>(a)) {
            actions::Accuracy *accuracy = dynamic_cast<actions::Accuracy *>(a);
            m_accuracy = accuracy->getAccuracy();
        } else if (dynamic_cast<actions::Rev *>(a)) {
            actions::Rev *rev = dynamic_cast<actions::Rev *>(a);
            m_revision = rev->getRevision();
        } else {
            a->evaluate(this, NULL);
        }
        delete a;
        return;
    }

    if (a->action_kind == Action::RunTimeOnlyIfMatchKind) {
        if (dynamic_cast<actions::Capture *>(a)) {
            m_containsCaptureAction = true;
            delete a;
        } else if (dynamic_cast<actions::MultiMatch *>(a)) {
            m_containsMultiMatchAction = true;
            delete a;
        } else if (dynamic_cast<actions::Severity *>(a)) {
            actions::Severity *severity = dynamic_cast<actions::Severity *>(a);
            setSeverity(severity->m_severity);
            delete a;
        } else if (dynamic_cast<actions::LogData *>(a)) {
            m_logData = std::unique_ptr<actions::LogData>(dynamic_cast<actions::LogData*>(a));
        } else if (dynamic_cast<actions::Msg *>(a)) {
            m_msg = std::unique_ptr<actions::Msg>(dynamic_cast<actions::Msg*>(a));
        } else if (dynamic_cast<actions::SetVar *>(a)) {
            m_actionsSetVar.push_back(std::unique_ptr<actions::SetVar>(dynamic_cast<actions::SetVar *>(a)));
        } else if (dynamic_cast<actions::Maturity *>(a)) {
            actions::Maturity *maturity = dynamic_cast<actions::Maturity *>(a);
            m_maturity = maturity->getMaturity();
            delete a;
        } else if (dynamic_cast<actions::Log *>(a)) {
            m_containsLogAction = true;
            delete a;
        } else if (dynamic_cast<actions::NoLog *>(a)) {
            m_containsNoLogAction = true;
            delete a;
        } else if (dynamic_cast<actions::Tag *>(a)) {
            m_actionsTag.push_back(std::unique_ptr<actions::Tag>(dynamic_cast<actions::Tag *>(a)));
        } else if (dynamic_cast<actions::Block *>(a)) {
            m_actionsRuntimePos.push_back(std::unique_ptr<actions::Block>(dynamic_cast<actions::Block *>(a)));
            m_containsStaticBlockAction = true;
        } else if (dynamic_cast<actions::XmlNS *>(a)) {
            m_XmlNSs.push_back(std::unique_ptr<actions::XmlNS>(dynamic_cast<actions::XmlNS *>(a)));
        } else if (a->isDisruptive() == true) {
            m_disruptiveAction = std::unique_ptr<Action>(a);
        } else {
            m_actionsRuntimePos.push_back(std::unique_ptr<Action>(a));
        }
        return;
    }

    std::cout << "General failure, action: " << *a->m_name;
    std::cout << " has an unknown type." << std::endl;
    delete a;
    throw;

}


RuleWithActions::~RuleWithActions() { }


bool RuleWithActions::evaluate(Transaction *transaction) {
    /* Matched vars needs to be clear at every new rule execution */
    transaction->m_matched.clear();

    return true;
}


void RuleWithActions::executeActionsIndependentOfChainedRuleResult(Transaction *trans) {

    for (actions::SetVar *a : getSetVarsActionsPtr()) {
        ms_dbg_a(trans, 4, "Running [independent] (non-disruptive) " \
            "action: " + *a->m_name.get());

        a->evaluate(this, trans);
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        if (a->isDisruptive() == true && *a->m_name.get() == "block") {
            ms_dbg_a(trans, 9, "Rule contains a `block' action");
        } else if (*a->m_name.get() == "setvar") {
            ms_dbg_a(trans, 4, "Running [independent] (non-disruptive) " \
                "action: " + *a->m_name.get());
            a->evaluate(this, trans, *trans->messageGetLast());
        }
    }
}


void RuleWithActions::executeActionsAfterFullMatch(Transaction *trans) {
    bool disruptiveAlreadyExecuted = false;

#if 0
    for (auto &a : trans->m_rules->m_defaultActions[getPhase()]) {
        if (a.get()->action_kind != actions::Action::RunTimeOnlyIfMatchKind) {
            continue;
        }
        if (!a.get()->isDisruptive()) {
            executeAction(trans, a.get(), true);

        }
    }
#endif

    for (actions::Tag *a : getTagsActionPtr()) {
        ms_dbg_a(trans, 4, "Running (non-disruptive) action: " \
            + *a->m_name.get());
        a->evaluate(this, trans, *trans->messageGetLast());
    }

    /**
     * 
     * FIXME: SecRuleUpdateActionBy should be runtime
     * 
     */
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pos_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        executeAction(trans, a, false);
        disruptiveAlreadyExecuted = true;
    }

    if (m_logData) {
        m_logData->evaluate(this, trans, *trans->messageGetLast());
    } else if (m_defaultActionLogData) {
        m_defaultActionLogData->evaluate(this, trans, *trans->messageGetLast());
    }

    if (m_msg) {
        m_msg->evaluate(this, trans, *trans->messageGetLast());
    } else if (m_defaultActionMsg) {
        m_defaultActionMsg->evaluate(this, trans, *trans->messageGetLast());
    }

    for (auto &a : getMatchActionsPtr()) {
        if (!a->isDisruptive()
                && !(disruptiveAlreadyExecuted
                && dynamic_cast<actions::Block *>(a))) {
            executeAction(trans, a, false);
        }
    }

    if (!disruptiveAlreadyExecuted && m_disruptiveAction != nullptr) {
        executeAction(trans,
            m_disruptiveAction.get(), false);
    } else if  (!disruptiveAlreadyExecuted && hasBlockAction()
        && m_defaultActionDisruptiveAction != nullptr) {
        executeAction(trans,
            m_defaultActionDisruptiveAction.get(), false);
    }
}


void RuleWithActions::executeAction(Transaction *trans,
    Action *a, bool defaultContext) {
    if (a->isDisruptive() == false && *a->m_name.get() != "block") {
        ms_dbg_a(trans, 9, "Running " \
            "action: " + *a->m_name.get());
        a->evaluate(this, trans, *trans->messageGetLast());
        return;
    }

    if (defaultContext && !hasBlockAction()) {
        ms_dbg_a(trans, 4, "Ignoring action: " + *a->m_name.get() + \
            " (rule does not cotains block)");
        return;
    }

    if (trans->getRuleEngineState() == RulesSet::EnabledRuleEngine) {
        ms_dbg_a(trans, 4, "Running (disruptive)     action: " + 
            *a->m_name.get() + ".");
        a->evaluate(this, trans, *trans->messageGetLast());
        return;
    }

    ms_dbg_a(trans, 4, "Not running any disruptive action (or block): " \
        + *a->m_name.get() + ". SecRuleEngine is not On.");
}


inline void RuleWithActions::executeTransformation(
    actions::transformations::Transformation *a,
    std::shared_ptr<std::string> *value,
    Transaction *trans,
    TransformationResults *ret,
    std::string *path) const {

    std::string *oldValue = (*value).get();
    std::string newValue = a->evaluate(*oldValue, trans);

    if (newValue != *oldValue) {
        std::shared_ptr<std::string> u(new std::string(newValue));
        if (m_containsMultiMatchAction) {
            ret->push_back(std::make_pair(u, a->m_name));
        }
        *value = u;
    }

    if (path->empty()) {
        path->append(*a->m_name.get());
    } else {
        path->append("," + *a->m_name.get());
    }

    ms_dbg_a(trans, 9, "Transformation " + \
        *a->m_name.get() + ": \"" + \
        utils::string::limitTo(80, newValue) + "\"");
}

void RuleWithActions::executeTransformations(
    Transaction *trans,
    const std::string &in,
    TransformationResults &ret) {
    int none = 0;
    std::string path("");
    std::shared_ptr<std::string> value =
        std::shared_ptr<std::string>(new std::string(in));

    if (hasMultimatchAction()) {
        /* keep the original value */
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(new std::string(*value)),
            std::shared_ptr<std::string>(new std::string(path))));
    }

    for (Action *a : getTransformationPtr()) {
        if (a->m_isNone) {
            none++;
        }
    }

    for (Transformation *a : getTransformationPtr()) {
        if (none == 0) {
            executeTransformation(a, &value, trans, &ret, &path);
        }
        if (a->m_isNone) {
            none--;
        }
    }

    // FIXME: It can't be something different from transformation. Sort this
    //        on rules compile time.
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        Transformation *a = dynamic_cast<Transformation*>(b.second.get());
        if (a->m_isNone) {
            none++;
        }
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        Transformation *a = dynamic_cast<Transformation*>(b.second.get());
        if (none == 0) {
            executeTransformation(a, &value, trans, &ret, &path);
        }
        if (a->m_isNone) {
            none--;
        }
    }

    if (m_containsMultiMatchAction == true) {
        ms_dbg_a(trans, 9, "multiMatch is enabled. " \
            + std::to_string(ret.size()) + \
            " values to be tested.");
    }

    if (!m_containsMultiMatchAction) {
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(new std::string(*value)),
            std::shared_ptr<std::string>(new std::string(path))));
    }
}


bool RuleWithActions::containsTag(const std::string& name, Transaction *t) const {
    for (auto &tag : getTagsAction()) {
        if (tag != NULL && tag->getName(t) == name) {
            return true;
        }
    }
    return false;
}


bool RuleWithActions::containsMsg(const std::string& name, Transaction *t) {
    return m_msg && m_msg->data(t) == name;
}


std::string RuleWithActions::getLogData(Transaction *t) { return m_logData->data(t); }
std::string RuleWithActions::getMessage(Transaction *t) { return m_msg->data(t); }



}  // namespace modsecurity
