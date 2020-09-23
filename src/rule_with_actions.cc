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
    m_chainedRuleChild(nullptr),
    m_chainedRuleParent(nullptr),
    m_XmlNSs(),
    m_accuracy(ACCURACY_NOT_SET),
    m_maturity(MATURITY_NOT_SET),
    m_severity(SEVERITY_NOT_SET),
    m_containsCapture(false),
    m_isChained(false),
    m_revision(""),
    m_version(""),
    m_actionMsg(nullptr),
    m_actionLogData(nullptr),
    m_defaultActions()
{
    // FIXME: split confs on parser.
    std::vector<std::shared_ptr<ActionTypeRuleMetaData>> confs;
    std::vector<std::shared_ptr<Action>> newActions;
    if (actions) {
        for (auto &a : *actions) {
            if (std::dynamic_pointer_cast<ActionTypeRuleMetaData>(a)) {
                confs.push_back(std::dynamic_pointer_cast<ActionTypeRuleMetaData>(a));
                continue;
            } else if (std::dynamic_pointer_cast<ActionDisruptive>(a)) {
                setDisruptiveAction(std::dynamic_pointer_cast<ActionDisruptive>(a));
                continue;
            }
            newActions.push_back(a);
        }
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
    for (auto &c : confs) {
        c->configure(this);
    }

    for (auto &a : newActions) {
        if (std::dynamic_pointer_cast<actions::SetVar>(a)) {
            addSetVar(std::dynamic_pointer_cast<actions::SetVar>(a));
            continue;
        }
        if (std::dynamic_pointer_cast<actions::Tag>(a)) {
            addTag(std::dynamic_pointer_cast<actions::Tag>(a));
            continue;
        }
        if (std::dynamic_pointer_cast<actions::XmlNS>(a)) {
            m_XmlNSs.push_back(std::dynamic_pointer_cast<actions::XmlNS>(a));
            continue;
        }
        if (std::dynamic_pointer_cast<actions::LogData>(a)) {
            m_actionLogData = std::dynamic_pointer_cast<actions::LogData>(a);
            continue;
        }
        if (std::dynamic_pointer_cast<actions::Msg>(a)) {
            m_actionMsg = std::dynamic_pointer_cast<actions::Msg>(a);
            continue;
        }

        addGenericMatchAction(std::dynamic_pointer_cast<ActionWithExecution>(a));
    }
    populate(this);
}


RuleWithActions::RuleWithActions(const RuleWithActions &r)
    : Rule(r),
    RuleWithActionsProperties(r),
    m_ruleId(r.m_ruleId),
    m_chainedRuleChild(r.m_chainedRuleChild),
    m_chainedRuleParent(r.m_chainedRuleParent),
    m_XmlNSs(/*r.m_XmlNSs*/),
    m_accuracy(r.m_accuracy),
    m_maturity(r.m_maturity),
    m_severity(r.m_severity),
    m_containsCapture(r.m_containsCapture),
    m_isChained(r.m_isChained),
    m_revision(r.m_revision),
    m_version(r.m_version),
    m_actionMsg(nullptr /*r.m_actionMsg*/),
    m_actionLogData(nullptr /* r.m_actionLogData */),
    m_defaultActions(r.m_defaultActions) {
        copyActionsWithRunTimeStrings(r);
        m_defaultActions.populate(this);
        populate(this);
}


RuleWithActions &RuleWithActions::operator=(const RuleWithActions& r) {
    Rule::operator = (r);
    RuleWithActionsProperties::operator = (r);
    m_ruleId = r.m_ruleId;
    m_chainedRuleChild = r.m_chainedRuleChild;
    m_chainedRuleParent = r.m_chainedRuleParent;
    /*m_XmlNSs = r.m_XmlNSs;*/
    m_accuracy = r.m_accuracy;
    m_maturity = r.m_maturity;
    m_severity = r.m_severity;
    m_containsCapture = r.m_containsCapture;
    m_isChained = r.m_isChained;
    m_revision = r.m_revision;
    m_version = r.m_version;
    /*m_actionMsg = r.m_actionMsg;*/
    /*m_actionLogData = r.m_actionLogData;*/
    m_defaultActions = r.m_defaultActions;
    copyActionsWithRunTimeStrings(r);
    m_defaultActions.populate(this);
    populate(this);
    return *this;
}


void inline RuleWithActions::copyActionsWithRunTimeStrings(const RuleWithActions &r) {
    if (r.m_actionLogData) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(r.m_actionLogData.get());
        if (!arts) {
            /* Humpf?  */
            m_actionLogData = r.m_actionLogData;
        } else {
            std::shared_ptr<actions::LogData> z(dynamic_cast<actions::LogData *>(arts->clone()));
            actions::ActionWithRunTimeString *aa = dynamic_cast<actions::ActionWithRunTimeString *>(z.get());
            aa->populate(nullptr);
            m_actionLogData = z;
        }
    }
    if (r.m_actionMsg) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(r.m_actionMsg.get());
        if (!arts) {
            /* Humpf?  */
            m_actionMsg = r.m_actionMsg;
        } else {
            std::shared_ptr<actions::Msg> z(dynamic_cast<actions::Msg *>(arts->clone()));
            actions::ActionWithRunTimeString *aa = dynamic_cast<actions::ActionWithRunTimeString *>(z.get());
            aa->populate(nullptr);
            m_actionMsg = z;
        }
    }
    for (auto &i : r.m_XmlNSs) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (!arts) {
            /* Humpf?  */
            m_XmlNSs.push_back(i);
        } else {
            std::shared_ptr<actions::XmlNS> z(dynamic_cast<actions::XmlNS *>(arts->clone()));
            actions::ActionWithRunTimeString *aa = dynamic_cast<actions::ActionWithRunTimeString *>(z.get());
            aa->populate(nullptr);
            m_XmlNSs.push_back(z);
        }
    }
}



void RuleWithActions::addDefaultAction(std::shared_ptr<actions::Action> a) {
    actions::ActionAllowedAsSecDefaultAction *d = dynamic_cast<actions::ActionAllowedAsSecDefaultAction *>(a.get());
    if (d == nullptr) {
        throw std::runtime_error("Action is being used as DefaultAction but not allowed.");
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
    if (std::dynamic_pointer_cast<actions::SetVar>(a)) {
        m_defaultActions.addSetVar(std::dynamic_pointer_cast<actions::SetVar>(a));
        return;
    }
    if (std::dynamic_pointer_cast<actions::Tag>(a)) {
        m_defaultActions.addTag(std::dynamic_pointer_cast<actions::Tag>(a));
        return;
    }
    if (std::dynamic_pointer_cast<actions::Block>(a)) {
        m_defaultActions.setBlock(true);
        return;
    }
    if (std::dynamic_pointer_cast<ActionDisruptive>(a)) {
        m_defaultActions.setDisruptiveAction(std::dynamic_pointer_cast<ActionDisruptive>(a));
        return;
    }
    std::shared_ptr<ActionWithExecution> bp = std::dynamic_pointer_cast<ActionWithExecution>(a);
    if (!bp) {
        return;
    }
    m_defaultActions.addGenericMatchAction(bp);
}


bool RuleWithActions::evaluate(Transaction *transaction) const {
    /* Matched vars needs to be clear at every new rule execution */
    transaction->m_matched.clear();

    return true;
}


void RuleWithActions::executeActionsIndependentOfChainedRuleResult(Transaction *trans) const {
    /* setVar */
    auto f = [](Transaction *t, const std::shared_ptr<actions::SetVar> &var) {
        ms_dbg_a(t, 4, "Running [independent] (non-disruptive) action: " + *var->getName());
        var->execute(t);
    };
    for (auto &a : m_defaultActions.getSetVars()) {  f(trans, a);  }
    for (auto &a : getSetVars()) {  f(trans, a);  }


    if (m_actionLogData) {
        m_actionLogData->execute(trans);
    }

    if (m_actionMsg) {
        m_actionMsg->execute(trans);
    }
}


void RuleWithActions::executeActionsAfterFullMatch(Transaction *trans) const {
    bool disruptiveAlreadyExecuted = false;

    /* tags */
    auto f = [](Transaction *t, const std::shared_ptr<actions::Tag> &tag) {
        ms_dbg_a(t, 4, "Running (non-disruptive) action: " + tag->getTagName(t));
        tag->execute(t);
    };
    for (auto &a : m_defaultActions.getTags()) {  f(trans, a);  }
    for (auto &a : getTags()) { f(trans, a);  }


    /**
     * FIXME: SecRuleUpdateActionBy should be runtime
     */
    auto range = trans->m_rules->m_exceptions.m_action_pos_update_target_by_id.equal_range(m_ruleId);
    for (auto it = range.first; it != range.second; ++it) {
        ActionWithExecution *a = dynamic_cast<ActionWithExecution*>(it->second.get());
        if (dynamic_cast<ActionDisruptive *>(a)) {
            trans->messageGetLast()->setRule(this);
            disruptiveAlreadyExecuted = true;
        }
        ms_dbg_a(trans, 9, "Running action placed by updateTargetById: " + *a->getName());
        a->execute(trans);
    }

    /* generic actions */
    auto fg = [](Transaction *t, const std::shared_ptr<ActionWithExecution> &a) {
        ms_dbg_a(t, 9, "Running action: " + *a->getName());
        a->execute(t);
    };
    for (auto &a : m_defaultActions.getGenericMatchActions()) {  fg(trans, a);  }
    for (auto &a : getGenericMatchActions()) {  fg(trans, a);  }

    /* disruptive actions */
    if (disruptiveAlreadyExecuted) {
        return;
    }
    auto fd = [](Transaction *t, const std::shared_ptr<ActionDisruptive> &a) {
        if (t->getRuleEngineState() == RulesSet::EnabledRuleEngine) {
            ms_dbg_a(t, 4, "Running (disruptive)     action: " + *a->getName() + ".");
            const ActionWithExecution *ae = dynamic_cast<const ActionWithExecution *>(a.get());
            ae->execute(t);
            return;
        }

        ms_dbg_a(t, 4, "Not running disruptive action: " \
            + *a->getName() + ". SecRuleEngine is not On.");
    };
    if (hasDisruptiveAction()) {
        trans->messageGetLast()->setRule(this);
        fd(trans, getDisruptiveAction());
    } else if ((hasBlock() || m_defaultActions.hasBlock()) && m_defaultActions.hasDisruptiveAction()) {
        trans->messageGetLast()->setRule(this);
        fd(trans, m_defaultActions.getDisruptiveAction());
    }
}


void RuleWithActions::executeTransformations(
    Transaction *trans,
    const std::string &in,
    TransformationsResults &results) const {
    int none = 0;

    ModSecString ssin;
    ssin.assign(in.c_str(), in.size());
    results.push_back(TransformationResult(&ssin));

    //FIXME: none should be pre-computed.
    for (auto &action : m_defaultActions.getTransformations()) {
        if (dynamic_cast<actions::transformations::None *>(action.get())) {
            none++;
        }
    }
    for (auto &action : getTransformations()) {
        if (dynamic_cast<actions::transformations::None *>(action.get())) {
            none++;
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

    for (auto &t : m_defaultActions.getTransformations()) {
        if (none == 0) {
            executeTransformation(trans, &results, t.get());
        }
        if (dynamic_cast<actions::transformations::None *>(t.get())) {
            none--;
        }
    }

    for (auto &t : getTransformations()) {
        if (none == 0) {
            executeTransformation(trans, &results, t.get());
        }
        if (dynamic_cast<actions::transformations::None *>(t.get())) {
            none--;
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


bool RuleWithActions::containsTag(const std::string& name, const Transaction *t) const noexcept {
    for (auto &tag : m_defaultActions.getTags()) {
        if (tag != nullptr && tag->getTagName(t) == name) {
            return true;
        }
    }
    for (auto &tag : getTags()) {
        if (tag != nullptr && tag->getTagName(t) == name) {
            return true;
        }
    }
    return false;
}


bool RuleWithActions::containsMsg(const std::string& name, const Transaction *t) const noexcept {
    return m_actionMsg && m_actionMsg->getEvaluatedRunTimeString(t) == name;
}


std::string RuleWithActions::getLogData(const Transaction *t) const noexcept {
    return m_actionLogData->getEvaluatedRunTimeString(t);
}


std::string RuleWithActions::getMessage(const Transaction *t) const noexcept {
    return m_actionMsg->getEvaluatedRunTimeString(t);
}

}  // namespace modsecurity
