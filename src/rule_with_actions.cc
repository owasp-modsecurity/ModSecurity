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
#include "src/actions/disruptive/disruptive_action.h"
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
#include "src/actions/action_allowed_in_sec_default_action.h"


namespace modsecurity {

RuleWithActions::RuleWithActions(
    Actions *actions,
    Transformations *transformations,
    std::unique_ptr<std::string> fileName,
    int lineNumber)
    : Rule(std::move(fileName), lineNumber),
    RuleWithActionsProperties(transformations),
    m_ruleId(0),
    m_accuracy(ACCURACY_NOT_SET),
    m_chainedRuleChild(nullptr),
    m_chainedRuleParent(nullptr),
    m_XmlNSs(),
    m_defaultActions(),
    m_isChained(false),
            m_revision(""),
        m_actionMsg(nullptr),
        m_maturity(MATURITY_NOT_SET),
        m_severity(SEVERITY_NOT_SET),
        m_version(""),
        m_actionLogData(nullptr),
        m_containsCaptureAction(false)
    {

    if (actions) {
        for (auto &a : *actions) {
            addAction(a);
        }
    }
}

void RuleWithActions::addAction(std::shared_ptr<actions::Action> a, int where) {
    /**
     * Set whenever to save as default action or part of the rule.
     *
     */
    RuleWithActionsProperties *p = this;
    if (where == 1) {
        p = &m_defaultActions;
        actions::ActionAllowedAsSecDefaultAction *d = dynamic_cast<actions::ActionAllowedAsSecDefaultAction *>(a.get());
        if (d == nullptr) {
            throw std::runtime_error("Action is being used as DefaultAction but not allowed.");
        }
    }

    /**
     *
     * ActionWithRunTimeString needs to be aware of the Rule that it
     * belongs to. It is necessary to resolve some variables
     * (e.g. Rule); Clone and associate are mandatory.
     *
     */
    actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(a.get());
    if (arts != nullptr) {
        a = std::unique_ptr<actions::Action>(arts->clone());
        arts = dynamic_cast<actions::ActionWithRunTimeString *>(a.get());
        arts->populate(this);
    }


    /**
     *
     * Those are actions that only fit the propose to update something in
     * the rule: META-DATA; e.g. RuleID.
     *
     * The merge action takes care of those properties. Once configured the
     * action can be forgotten.
     *
     */
    if (dynamic_cast<ActionTypeRuleMetaData *>(a.get())) {
        ActionTypeRuleMetaData *conf = dynamic_cast<ActionTypeRuleMetaData *>(a.get());
        conf->configure(this);
        return;
    }


    /**
     * Actions that cannot be set as default action.
     *
     */
    if (dynamic_cast<actions::XmlNS *>(a.get())) {
        auto z = std::dynamic_pointer_cast<actions::XmlNS>(a);
        m_XmlNSs.push_back(z);
        return;
    }
    else if (dynamic_cast<actions::LogData *>(a.get())) {
        auto z = std::dynamic_pointer_cast<actions::LogData>(a);
        m_actionLogData = z;
        return;
    }
    else if (dynamic_cast<actions::Msg *>(a.get())) {
        auto z = std::dynamic_pointer_cast<actions::Msg>(a);
        m_actionMsg = z;
        return;
    }

    /**
     * Actions that can be either: set as default or part of the Rule.
     *
     */
    if (dynamic_cast<actions::SetVar *>(a.get())) {
        p->m_actionsSetVar.push_back(std::dynamic_pointer_cast<actions::SetVar>(a));
    } else if (dynamic_cast<actions::Tag *>(a.get())) {
        p->m_actionsTag.push_back(std::dynamic_pointer_cast<actions::Tag>(a));
    } else if (dynamic_cast<actions::Block *>(a.get())) {
        p->m_actionsRuntimePos.push_back(std::dynamic_pointer_cast<ActionWithExecution>(a));
        p->m_containsStaticBlockAction = true;
    } else if (std::dynamic_pointer_cast<ActionDisruptive>(a) != NULL) {
        p->m_actionDisruptiveAction = std::dynamic_pointer_cast<ActionDisruptive>(a);
    } else {
        p->m_actionsRuntimePos.push_back(std::dynamic_pointer_cast<ActionWithExecution>(a));
    }
}


RuleWithActions::~RuleWithActions() { }


bool RuleWithActions::evaluate(Transaction *transaction) const {
    /* Matched vars needs to be clear at every new rule execution */
    transaction->m_matched.clear();

    return true;
}


void RuleWithActions::executeActionsIndependentOfChainedRuleResult(Transaction *trans) const {

    for (actions::SetVar *a : getSetVarsActionsPtr()) {
        ms_dbg_a(trans, 4, "Running [independent] (non-disruptive) " \
            "action: " + *a->getName());

        a->execute(trans);
    }

    if (m_actionLogData) {
        m_actionLogData->execute(trans);
    }

    if (m_actionMsg) {
        m_actionMsg->execute(trans);
    }
}


void RuleWithActions::executeActionsAfterFullMatch(Transaction *trans) const {
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
    auto range = trans->m_rules->m_exceptions.m_action_pos_update_target_by_id.equal_range(m_ruleId);
    for (auto it = range.first; it != range.second; ++it) {
        ActionWithExecution *a = dynamic_cast<ActionWithExecution*>(it->second.get());
        if (dynamic_cast<ActionDisruptive *>(a)) {
            trans->messageGetLast()->setRule(this);
        }
        executeAction(trans, a, false);
        if (dynamic_cast<ActionDisruptive *>(a)) {
            disruptiveAlreadyExecuted = true;
        }
    }
    for (auto &a : getMatchActionsPtr()) {
        if (!dynamic_cast<ActionDisruptive *>(a)
                && !(disruptiveAlreadyExecuted
                && dynamic_cast<actions::Block *>(a))) {
            executeAction(trans, a, false);
        }
    }
    if (!disruptiveAlreadyExecuted && m_actionDisruptiveAction != nullptr) {
        trans->messageGetLast()->setRule(this);
        executeAction(trans,
            m_actionDisruptiveAction.get(), false);
    } else if  (!disruptiveAlreadyExecuted && hasBlockAction()
        && m_defaultActions.m_actionDisruptiveAction != nullptr) {
        trans->messageGetLast()->setRule(this);
        executeAction(trans,
            m_defaultActions.m_actionDisruptiveAction.get(), false);
    }
}


void RuleWithActions::executeAction(Transaction *trans,
    ActionWithExecution *a, bool defaultContext) {
    ms_dbg_a(trans, 9, "Running action: " + *a->getName());
    a->execute(trans);
}


void RuleWithActions::executeAction(Transaction *trans,
    ActionDisruptive *a, bool defaultContext) const {
    if (defaultContext && !hasBlockAction()) {
        ms_dbg_a(trans, 4, "Ignoring action: " + *a->getName() + \
            " (rule does not cotains block)");
        return;
    }

    if (trans->getRuleEngineState() == RulesSet::EnabledRuleEngine) {
        ms_dbg_a(trans, 4, "Running (disruptive)     action: " + \
            *a->getName() + ".");
        ActionWithExecution *ae = dynamic_cast<ActionWithExecution *>(a);
        ae->execute(trans);
        return;
    }

    ms_dbg_a(trans, 4, "Not running disruptive action: " \
        + *a->getName() + ". SecRuleEngine is not On.");
}


void RuleWithActions::executeTransformations(
    Transaction *trans,
    const std::string &in,
    TransformationsResults &results) const {
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
    auto range = trans->m_rules->m_exceptions.m_action_transformation_update_target_by_id.equal_range(m_ruleId);
    for (auto it = range.first; it != range.second; ++it) {
        Transformation *t = it->second.get();
        if (dynamic_cast<actions::transformations::None *>(t)) {
            none++;
        }
    }

    for (auto it = range.first; it != range.second; ++it) {
        Transformation *t = it->second.get();
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


bool RuleWithActions::containsMsg(const std::string& name, Transaction *t) const {
    return m_actionMsg && m_actionMsg->getEvaluatedRunTimeString(t) == name;
}


std::string RuleWithActions::getLogData(const Transaction *t) const { return m_actionLogData->getEvaluatedRunTimeString(t); }
std::string RuleWithActions::getMessage(const Transaction *t) const { return m_actionMsg->getEvaluatedRunTimeString(t); }


}  // namespace modsecurity
