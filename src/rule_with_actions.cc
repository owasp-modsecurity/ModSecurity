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
#include "src/rule_with_actions.h"
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
#include "src/actions/transformations/none.h"
#include "src/actions/xmlns.h"
#include "src/utils/string.h"
#include "src/actions/action_with_run_time_string.h"
#include "src/actions/phase.h"
#include "src/actions/chain.h"
#include "src/actions/rule_id.h"
#include "src/actions/ver.h"
#include "src/actions/action_type_rule_metadata.h"



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
    m_containsAuditLogAction(false),
    m_containsNoAuditLogAction(false),
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
    m_defaultContainsAuditLogAction(false),
    m_defaultContainsNoAuditLogAction(false),
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

    actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(a.get());
    if (arts != nullptr) {
        a = std::unique_ptr<actions::Action>(arts->clone());
        arts = dynamic_cast<actions::ActionWithRunTimeString *>(a.get());
        arts->populate(this);
    }

    if (dynamic_cast<ActionTypeRuleMetaData *>(a.get())) {
        ActionTypeRuleMetaData *conf = dynamic_cast<ActionTypeRuleMetaData *>(a.get());
        conf->configure(this);
        return;
    }

    if (dynamic_cast<actions::LogData *>(a.get())) {
        m_defaultActionLogData.reset(dynamic_cast<actions::LogData *>(a.get()));
    } else if (dynamic_cast<actions::Msg *>(a.get())) {
        m_defaultActionMsg.reset(dynamic_cast<actions::Msg *>(a.get()));
    } else if (dynamic_cast<actions::SetVar *>(a.get())) {
        actions::SetVar *var = dynamic_cast<actions::SetVar *>(a.get());
        m_actionsSetVar.push_back(std::unique_ptr<actions::SetVar>(var));
    } else if (dynamic_cast<actions::Tag *>(a.get())) {
        m_defaultActionActionsTag.push_back(std::dynamic_pointer_cast<actions::Tag>(a));
    } else if (dynamic_cast<actions::Block *>(a.get())) {
        m_defaultActionActionsRuntimePos.push_back(a);
        m_defaultContainsStaticBlockAction = true;
    } else if (a->isDisruptive() == true) {
        m_defaultActionDisruptiveAction = a;
    } else {
        m_defaultActionActionsRuntimePos.push_back(a);
    }
}

void RuleWithActions::addAction(actions::Action *a) {
    actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(a);
    if (arts != nullptr) {
        a = arts->clone();
        arts = dynamic_cast<actions::ActionWithRunTimeString *>(a);
        arts->populate(this);
    }

    if (dynamic_cast<ActionTypeRuleMetaData *>(a)) {
        ActionTypeRuleMetaData *conf = dynamic_cast<ActionTypeRuleMetaData *>(a);
        conf->configure(this);
        delete a;
        return;
    }


    if (dynamic_cast<actions::LogData *>(a)) {
        m_logData = std::unique_ptr<actions::LogData>(dynamic_cast<actions::LogData*>(a));
    } else if (dynamic_cast<actions::Msg *>(a)) {
        m_msg = std::unique_ptr<actions::Msg>(dynamic_cast<actions::Msg*>(a));
    } else if (dynamic_cast<actions::SetVar *>(a)) {
        actions::SetVar *var = dynamic_cast<actions::SetVar *>(a);
        m_actionsSetVar.push_back(std::unique_ptr<actions::SetVar>(var));
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
            "action: " + *a->getName());

        a->execute(trans);
    }

    if (m_logData) {
        m_logData->execute(trans);
    } else if (m_defaultActionLogData) {
        m_defaultActionLogData->execute(trans);
    }

    if (m_msg) {
        m_msg->execute(trans);
    } else if (m_defaultActionMsg) {
        m_defaultActionMsg->execute(trans);
    }
}


void RuleWithActions::executeActionsAfterFullMatch(Transaction *trans) {
    bool disruptiveAlreadyExecuted = false;

    for (actions::Tag *a : getTagsActionPtr()) {
        ms_dbg_a(trans, 4, "Running (non-disruptive) action: " \
            + a->getTagName(trans));
        a->execute(trans);
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
        if (a->isDisruptive()) {
            trans->messageGetLast()->setRule(this);
        }
        executeAction(trans, a, false);
        if (a->isDisruptive()) {
            disruptiveAlreadyExecuted = true;
        }
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
    if (a->isDisruptive() == false) {
        ms_dbg_a(trans, 9, "Running action: " + *a->getName());
        a->execute(trans);
        return;
    }

    if (defaultContext && !hasBlockAction()) {
        ms_dbg_a(trans, 4, "Ignoring action: " + *a->getName() + \
            " (rule does not cotains block)");
        return;
    }

    if (trans->getRuleEngineState() == RulesSet::EnabledRuleEngine) {
        ms_dbg_a(trans, 4, "Running (disruptive)     action: " + 
            *a->getName() + ".");
        a->execute(trans);
        return;
    }

    ms_dbg_a(trans, 4, "Not running disruptive action: " \
        + *a->getName() + ". SecRuleEngine is not On.");
}


void RuleWithActions::executeTransformations(
    Transaction *trans,
    const std::string &in,
    TransformationsResults &results) {
    int none = 0;

    ModSecString ssin;
    ssin.assign(in.c_str(), in.size());
    results.push_back(TransformationResult(&ssin));


    std::string path("");
    std::shared_ptr<std::string> value =
        std::shared_ptr<std::string>(new std::string(in));

    for (Transformation *action : getTransformationPtr()) {
        if (dynamic_cast<actions::transformations::None *>(action)) {
            none++;
        }
    }

    for (Transformation *t : getTransformationPtr()) {
        if (none == 0) {
            executeTransformation(trans, &results, t);
        }
        if (dynamic_cast<actions::transformations::None *>(t)) {
            none--;
        }
    }

    // FIXME: It can't be something different from transformation. Sort this
    //        on rules compile time.
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_transformation_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        Transformation *t = b.second.get();
        if (dynamic_cast<actions::transformations::None *>(t)) {
            none++;
        }
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_transformation_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        Transformation *t = b.second.get();
        if (none == 0) {
            executeTransformation(trans, &results, t);
        }
        if (dynamic_cast<actions::transformations::None *>(t)) {
            none--;
        }
    }
}



void RuleWithActions::executeTransformation(
    Transaction *transaction,
    TransformationsResults *ret,
    Transformation *transformation) {
    executeTransformation(
        transaction,
        *ret->back().getAfter(),
        ret,
        transformation
    );
}


void RuleWithActions::executeTransformation(
    Transaction *transaction,
    ModSecString &in,
    TransformationsResults *ret,
    Transformation *transformation) {

    ModSecString out;
    transformation->execute(transaction, in, out);

    ms_dbg_a(transaction, 9, " T (" + std::to_string(ret->size() - 1) + ") " + \
        *transformation->getName() + ": \"" + \
        utils::string::limitTo(80, out.c_str()) + "\"");

    ret->push_back(
        TransformationResult(
            out,
            transformation->getName()
        )
    );
}


bool RuleWithActions::containsTag(const std::string& name, Transaction *t) const {
    for (auto &tag : getTagsAction()) {
        if (tag != NULL && tag->getTagName(t) == name) {
            return true;
        }
    }
    return false;
}


bool RuleWithActions::containsMsg(const std::string& name, Transaction *t) {
    return m_msg && m_msg->getEvaluatedRunTimeString(t) == name;
}


std::string RuleWithActions::getLogData(Transaction *t) const { return m_logData->getEvaluatedRunTimeString(t); }
std::string RuleWithActions::getMessage(Transaction *t) const { return m_msg->getEvaluatedRunTimeString(t); }


}  // namespace modsecurity
