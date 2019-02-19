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
#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <utility>
#include <memory>

#include "modsecurity/rules_set.h"
#include "src/operators/operator.h"
#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "src/actions/transformations/none.h"
#include "src/actions/tag.h"
#include "src/utils/string.h"
#include "modsecurity/rule_message.h"
#include "src/actions/msg.h"
#include "src/actions/log_data.h"
#include "src/actions/severity.h"
#include "src/actions/capture.h"
#include "src/actions/multi_match.h"
#include "src/actions/set_var.h"
#include "src/actions/block.h"
#include "src/variables/variable.h"


namespace modsecurity {

using operators::Operator;
using actions::Action;
using variables::Variable;
using actions::transformations::None;
using actions::transformations::Transformation;


RuleWithActions::RuleWithActions(
    Actions *actions,
    Transformations *transformations,
    std::unique_ptr<std::string> fileName,
    int lineNumber)
    : RuleBase(std::move(fileName), lineNumber),
    m_accuracy(0),
    m_actionsRuntimePos(),
    m_transformations(transformations != NULL ? *transformations : Transformations()),
    m_actionsSetVar(),
    m_actionsTag(),
    m_isChained(false),
    m_maturity(0),
    m_rev(""),
    m_ruleId(0),
    m_ver(""),
    m_disruptiveAction(nullptr),
    m_containsStaticBlockAction(false),
    m_containsCaptureAction(false),
    m_containsMultiMatchAction(false),
    m_severity(nullptr),
    m_logData(nullptr),
    m_msg(nullptr) {
    if (actions) {
        for (Action *a : *actions) {
            if (a->action_kind == Action::ConfigurationKind) {
                a->evaluate(this, NULL);
                delete a;
            } else if (a->action_kind == Action::RunTimeOnlyIfMatchKind) {
                if (dynamic_cast<actions::Capture *>(a)) {
                    m_containsCaptureAction = true;
                    delete a;
                } else if (dynamic_cast<actions::MultiMatch *>(a)) {
                    m_containsMultiMatchAction = true;
                    delete a;
                } else if (dynamic_cast<actions::Severity *>(a)) {
                    m_severity = dynamic_cast<actions::Severity *>(a);
                } else if (dynamic_cast<actions::LogData *>(a)) {
                    m_logData = dynamic_cast<actions::LogData*>(a);
                } else if (dynamic_cast<actions::Msg *>(a)) {
                    m_msg = dynamic_cast<actions::Msg*>(a);
                } else if (dynamic_cast<actions::SetVar *>(a)) {
                    m_actionsSetVar.push_back(
                        dynamic_cast<actions::SetVar *>(a));
                } else if (dynamic_cast<actions::Tag *>(a)) {
                    m_actionsTag.push_back(dynamic_cast<actions::Tag *>(a));
                } else if (dynamic_cast<actions::Block *>(a)) {
                    m_actionsRuntimePos.push_back(a);
                    m_containsStaticBlockAction = true;
                } else if (a->isDisruptive() == true) {
                    if (m_disruptiveAction != nullptr) {
                        delete m_disruptiveAction;
                        m_disruptiveAction = nullptr;
                    }
                    m_disruptiveAction = a;
                } else {
                    m_actionsRuntimePos.push_back(a);
                }
            } else {
                delete a;
                std::cout << "General failure, action: " << a->m_name;
                std::cout << " has an unknown type." << std::endl;
                throw;
            }
        }
        delete actions;
    }
}

RuleWithActions::~RuleWithActions() {
    if (m_severity) {
        delete m_severity;
        m_severity = nullptr;
    }
    if (m_logData) {
        delete m_logData;
        m_logData = nullptr;
    }
    if (m_msg) {
        delete m_msg;
        m_msg = nullptr;
    }
    while (m_transformations.empty() == false) {
        auto *a = m_transformations.back();
        m_transformations.pop_back();
        delete a;
    }
    while (m_actionsRuntimePos.empty() == false) {
        auto *a = m_actionsRuntimePos.back();
        m_actionsRuntimePos.pop_back();
        delete a;
    }
    while (m_actionsSetVar.empty() == false) {
        auto *a = m_actionsSetVar.back();
        m_actionsSetVar.pop_back();
        delete a;
    }
    while (m_actionsTag.empty() == false) {
        auto *a = m_actionsTag.back();
        m_actionsTag.pop_back();
        delete a;
    }
    if (m_disruptiveAction != nullptr) {
        delete m_disruptiveAction;
        m_disruptiveAction = nullptr;
    }
}

void RuleWithActions::executeActionsIndependentOfChainedRuleResult(Transaction *trans,
    bool *containsBlock, std::shared_ptr<RuleMessage> ruleMessage) {

    for (actions::SetVar *a : m_actionsSetVar) {
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
                *containsBlock = true;
        } else if (*a->m_name.get() == "setvar") {
            ms_dbg_a(trans, 4, "Running [independent] (non-disruptive) " \
                "action: " + *a->m_name.get());
            a->evaluate(this, trans, ruleMessage);
        }
    }

    if (m_severity) {
        m_severity->evaluate(this, trans, ruleMessage);
    }

    if (m_logData) {
        m_logData->evaluate(this, trans, ruleMessage);
    }

    if (m_msg) {
        m_msg->evaluate(this, trans, ruleMessage);
    }
}

inline void RuleWithActions::executeTransformation(
    actions::transformations::Transformation *a,
    std::shared_ptr<std::string> *value,
    Transaction *trans,
    TransformationResults *ret,
    std::string *path,
    int *nth) {

    std::string *oldValue = (*value).get();
    std::string newValue = a->evaluate(*oldValue, trans);

    if (newValue != *oldValue) {
        std::shared_ptr<std::string> u(new std::string(newValue));
        if (m_containsMultiMatchAction) {
            ret->push_back(std::make_pair(u, a->m_name));
            (*nth)++;
        }
        *value = u;
    }

    if (path->empty()) {
        path->append(*a->m_name.get());
    } else {
        path->append("," + *a->m_name.get());
    }

    ms_dbg_a(trans, 9, " T (" + \
        std::to_string(*nth) + ") " + \
        *a->m_name.get() + ": \"" + \
        utils::string::limitTo(80, newValue) +"\"");
}

void RuleWithActions::executeTransformations(
    Transaction *trans, const std::string &in, TransformationResults &ret) {
    int none = 0;
    int transformations = 0;
    std::string path("");
    std::shared_ptr<std::string> value =
        std::shared_ptr<std::string>(new std::string(in));

    if (m_containsMultiMatchAction == true) {
        /* keep the original value */
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(new std::string(*value)),
            std::shared_ptr<std::string>(new std::string(path))));
    }

    for (Action *a : m_transformations) {
        if (a->m_isNone) {
            none++;
        }
    }

    // Check for transformations on the SecDefaultAction
    // Notice that first we make sure that won't be a t:none
    // on the target rule.
    if (none == 0) {
        for (auto &a : trans->m_rules->m_defaultActions[getPhase()]) {
            if (a->action_kind \
                != actions::Action::RunTimeBeforeMatchAttemptKind) {
                continue;
            }

            // FIXME: here the object needs to be a transformation already.
            Transformation *t = dynamic_cast<Transformation *>(a.get());
            executeTransformation(t, &value, trans, &ret, &path,
                &transformations);
        }
    }

    for (Transformation *a : m_transformations) {
        if (none == 0) {
            Transformation *t = dynamic_cast<Transformation *>(a);
            executeTransformation(t, &value, trans, &ret, &path,
                &transformations);
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
            Transformation *t = dynamic_cast<Transformation *>(a);
            executeTransformation(t, &value, trans, &ret, &path,
                &transformations);
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

void RuleWithActions::executeActionsAfterFullMatch(Transaction *trans,
    bool containsBlock, std::shared_ptr<RuleMessage> ruleMessage) {
    bool disruptiveAlreadyExecuted = false;

    for (auto &a : trans->m_rules->m_defaultActions[getPhase()]) {
        if (a.get()->action_kind != actions::Action::RunTimeOnlyIfMatchKind) {
            continue;
        }
        if (!a.get()->isDisruptive()) {
            executeAction(trans, containsBlock, ruleMessage, a.get(), true);
        }
    }

    for (actions::Tag *a : this->m_actionsTag) {
        ms_dbg_a(trans, 4, "Running (non-disruptive) action: " \
            + *a->m_name.get());
        a->evaluate(this, trans, ruleMessage);
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pos_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        executeAction(trans, containsBlock, ruleMessage, a, false);
        disruptiveAlreadyExecuted = true;
    }
    for (Action *a : this->m_actionsRuntimePos) {
        if (!a->isDisruptive()
                && !(disruptiveAlreadyExecuted
                && dynamic_cast<actions::Block *>(a))) {
            executeAction(trans, containsBlock, ruleMessage, a, false);
        }
    }
    if (!disruptiveAlreadyExecuted && m_disruptiveAction != nullptr) {
        executeAction(trans, containsBlock, ruleMessage,
            m_disruptiveAction, false);
    }
}

bool RuleWithActions::containsTag(const std::string& name, Transaction *t) {
    for (auto &tag : m_actionsTag) {
        if (tag != NULL && tag->getName(t) == name) {
            return true;
        }
    }
    return false;
}


bool RuleWithActions::containsMsg(const std::string& name, Transaction *t) {
    return m_msg && m_msg->data(t) == name;
}

std::string RuleWithActions::logData(Transaction *t) { return m_logData->data(t); }
std::string RuleWithActions::msg(Transaction *t) { return m_msg->data(t); }
int RuleWithActions::severity() { return m_severity->m_severity; }


RuleWithOperator::RuleWithOperator(Operator *op,
    variables::Variables *_variables,
    std::vector<Action *> *actions,
    Transformations *transformations,
    std::unique_ptr<std::string> fileName,
    int lineNumber)
    : RuleWithActions(actions, transformations, std::move(fileName), lineNumber),
    m_chainedRuleChild(nullptr),
    m_chainedRuleParent(NULL),

    m_operator(op),
    m_variables(_variables),
    m_unconditional(false)  { /* */ }


RuleWithOperator::~RuleWithOperator() {
    if (m_operator != NULL) {
        delete m_operator;
    }

    while (m_variables != NULL && m_variables->empty() == false) {
        auto *a = m_variables->back();
        m_variables->pop_back();
        delete a;
    }

    if (m_variables != NULL) {
        delete m_variables;
    }
}


inline void RuleWithOperator::updateMatchedVars(Transaction *trans, const std::string &key,
    const std::string &value) {
    ms_dbg_a(trans, 9, "Matched vars updated.");
    trans->m_variableMatchedVar.set(value, trans->m_variableOffset);
    trans->m_variableMatchedVarName.set(key, trans->m_variableOffset);

    trans->m_variableMatchedVars.set(key, value, trans->m_variableOffset);
    trans->m_variableMatchedVarsNames.set(key, key, trans->m_variableOffset);
}


inline void RuleWithOperator::cleanMatchedVars(Transaction *trans) {
    ms_dbg_a(trans, 9, "Matched vars cleaned.");
    trans->m_variableMatchedVar.unset();
    trans->m_variableMatchedVars.unset();
    trans->m_variableMatchedVarName.unset();
    trans->m_variableMatchedVarsNames.unset();
}



bool RuleWithOperator::executeOperatorAt(Transaction *trans, std::string key,
    std::string value, std::shared_ptr<RuleMessage> ruleMessage) {
#if MSC_EXEC_CLOCK_ENABLED
    clock_t begin = clock();
    clock_t end;
    double elapsed_s = 0;
#endif
    bool ret;

    ms_dbg_a(trans, 9, "Target value: \"" + utils::string::limitTo(80,
        utils::string::toHexIfNeeded(value)) \
        + "\" (Variable: " + key + ")");

    ret = this->m_operator->evaluateInternal(trans, this, value, ruleMessage);
    if (ret == false) {
        return false;
    }

#if MSC_EXEC_CLOCK_ENABLED
    end = clock();
    elapsed_s = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    ms_dbg_a(trans, 5, "Operator completed in " + \
        std::to_string(elapsed_s) + " seconds");
#endif
    return ret;
}


void RuleWithOperator::getVariablesExceptions(Transaction *t,
    variables::Variables *exclusion, variables::Variables *addition) {
    for (auto &a : t->m_rules->m_exceptions.m_variable_update_target_by_tag) {
        if (containsTag(*a.first.get(), t) == false) {
            continue;
        }
        Variable *b = a.second.get();
        if (dynamic_cast<variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<variables::VariableModificatorExclusion*>(
                    b)->m_base.get());
        } else {
            addition->push_back(b);
        }
    }

    for (auto &a : t->m_rules->m_exceptions.m_variable_update_target_by_msg) {
        if (containsMsg(*a.first.get(), t) == false) {
            continue;
        }
        Variable *b = a.second.get();
        if (dynamic_cast<variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<variables::VariableModificatorExclusion*>(
                    b)->m_base.get());
        } else {
            addition->push_back(b);
        }
    }

    for (auto &a : t->m_rules->m_exceptions.m_variable_update_target_by_id) {
        if (m_ruleId != a.first) {
            continue;
        }
        Variable *b = a.second.get();
        if (dynamic_cast<variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<variables::VariableModificatorExclusion*>(
                    b)->m_base.get());
        } else {
            addition->push_back(b);
        }
    }
}


inline void RuleWithOperator::getFinalVars(variables::Variables *vars,
    variables::Variables *exclusion, Transaction *trans) {
    variables::Variables addition;
    getVariablesExceptions(trans, exclusion, &addition);

    for (int i = 0; i < m_variables->size(); i++) {
        Variable *variable = m_variables->at(i);
        std::vector<const VariableValue *> e;


        if (exclusion->contains(variable)) {
            continue;
        }
        if (std::find_if(trans->m_ruleRemoveTargetById.begin(),
                trans->m_ruleRemoveTargetById.end(),
                [&, variable, this](std::pair<int, std::string> &m) -> bool {
                    return m.first == m_ruleId
                        && m.second == *variable->m_fullName.get();
                }) != trans->m_ruleRemoveTargetById.end()) {
            continue;
        }
        if (std::find_if(trans->m_ruleRemoveTargetByTag.begin(),
                    trans->m_ruleRemoveTargetByTag.end(),
                    [&, variable, trans, this](
                        std::pair<std::string, std::string> &m) -> bool {
                        return containsTag(m.first, trans)
                            && m.second == *variable->m_fullName.get();
                    }) != trans->m_ruleRemoveTargetByTag.end()) {
            continue;
        }
        vars->push_back(variable);
    }

    for (int i = 0; i < addition.size(); i++) {
        Variable *variable = addition.at(i);
        vars->push_back(variable);
    }
}



void RuleWithActions::executeAction(Transaction *trans,
    bool containsBlock, std::shared_ptr<RuleMessage> ruleMessage,
    Action *a, bool defaultContext) {
    if (a->isDisruptive() == false && *a->m_name.get() != "block") {
        ms_dbg_a(trans, 9, "Running " \
            "action: " + *a->m_name.get());
        a->evaluate(this, trans, ruleMessage);
        return;
    }

    if (defaultContext && !containsBlock) {
        ms_dbg_a(trans, 4, "Ignoring action: " + *a->m_name.get() + \
            " (rule does not cotains block)");
        return;
    }

    if (trans->getRuleEngineState() == RulesSet::EnabledRuleEngine) {
        ms_dbg_a(trans, 4, "Running (disruptive)     action: " + *a->m_name.get() + \
            ".");
        a->evaluate(this, trans, ruleMessage);
        return;
    }

    ms_dbg_a(trans, 4, "Not running any disruptive action (or block): " \
        + *a->m_name.get() + ". SecRuleEngine is not On.");
}


bool RuleWithOperator::evaluate(Transaction *trans,
    std::shared_ptr<RuleMessage> ruleMessage) {
    bool globalRet = false;
    variables::Variables *variables = this->m_variables;
    bool recursiveGlobalRet;
    bool containsBlock = hasBlockAction();
    std::vector<std::unique_ptr<VariableValue>> finalVars;
    std::string eparam;
    variables::Variables vars;
    vars.reserve(4);
    variables::Variables exclusion;

    if (ruleMessage == NULL) {
        ruleMessage = std::shared_ptr<RuleMessage>(
            new RuleMessage(this, trans));
    }

    trans->m_matched.clear();

    if (isMarker() == true) {
        return true;
    }

    if (isUnconditional() == true) {
        ms_dbg_a(trans, 4, "(Rule: " + std::to_string(m_ruleId) \
            + ") Executing unconditional rule...");
        executeActionsIndependentOfChainedRuleResult(trans,
            &containsBlock, ruleMessage);
        goto end_exec;
    }

    for (auto &i : trans->m_ruleRemoveById) {
        if (m_ruleId != i) {
            continue;
        }
        ms_dbg_a(trans, 9, "Rule id: " + std::to_string(m_ruleId) +
            " was skipped due to a ruleRemoveById action...");
        return true;
    }
    for (auto &i : trans->m_ruleRemoveByIdRange) {
        if (!(i.first <= m_ruleId && i.second >= m_ruleId)) {
            continue;
        }
        ms_dbg_a(trans, 9, "Rule id: " + std::to_string(m_ruleId) +
            " was skipped due to a ruleRemoveById action...");
        return true;
    }

    if (m_operator->m_string) {
        eparam = m_operator->m_string->evaluate(trans);

        if (m_operator->m_string->containsMacro()) {
            eparam = "\"" + eparam + "\" Was: \"" \
                + m_operator->m_string->evaluate(NULL) + "\"";
        } else {
            eparam = "\"" + eparam + "\"";
        }
    ms_dbg_a(trans, 4, "(Rule: " + std::to_string(m_ruleId) \
        + ") Executing operator \"" + getOperatorName() \
        + "\" with param " \
        + eparam \
        + " against " \
        + variables + ".");
    } else {
        ms_dbg_a(trans, 4, "(Rule: " + std::to_string(m_ruleId) \
            + ") Executing operator \"" + getOperatorName() \
            + " against " \
            + variables + ".");
    }

    getFinalVars(&vars, &exclusion, trans);

    for (auto &var : vars) {
        std::vector<const VariableValue *> e;
        if (!var) {
            continue;
        }
        var->evaluate(trans, this, &e);
        for (const VariableValue *v : e) {
            const std::string &value = v->getValue();
            const std::string &key = v->getKeyWithCollection();

            if (exclusion.contains(v->getKey()) ||
                std::find_if(trans->m_ruleRemoveTargetById.begin(),
                    trans->m_ruleRemoveTargetById.end(),
                    [&, v, this](std::pair<int, std::string> &m) -> bool {
                        return m.first == m_ruleId && m.second == v->getKeyWithCollection();
                    }) != trans->m_ruleRemoveTargetById.end()
            ) {
                delete v;
                v = NULL;
                continue;
            }
            if (exclusion.contains(v->getKey()) ||
                std::find_if(trans->m_ruleRemoveTargetByTag.begin(),
                    trans->m_ruleRemoveTargetByTag.end(),
                    [&, v, trans, this](std::pair<std::string, std::string> &m) -> bool {
                        return containsTag(m.first, trans) && m.second == v->getKeyWithCollection();
                    }) != trans->m_ruleRemoveTargetByTag.end()
            ) {
                delete v;
                v = NULL;
                continue;
            }

            TransformationResults values;

            executeTransformations(trans, value, values);

            for (const auto &valueTemp : values) {
                bool ret;
                std::string valueAfterTrans = std::move(*valueTemp.first);

                ret = executeOperatorAt(trans, key, valueAfterTrans, ruleMessage);

                if (ret == true) {
                    ruleMessage->m_match = m_operator->resolveMatchMessage(trans,
                        key, value);
                    for (auto &i : v->getOrigin()) {
                        ruleMessage->m_reference.append(i->toText());
                    }

                    ruleMessage->m_reference.append(*valueTemp.second);
                    updateMatchedVars(trans, key, valueAfterTrans);
                    executeActionsIndependentOfChainedRuleResult(trans,
                        &containsBlock, ruleMessage);

                    bool isItToBeLogged = ruleMessage->m_saveMessage;
                    if (hasMultimatch() && isItToBeLogged) {
                        /* warn */
                        trans->m_rulesMessages.push_back(*ruleMessage);

                        /* error */
                        if (!ruleMessage->m_isDisruptive) {
                            trans->serverLog(ruleMessage);
                        }

                        RuleMessage *rm = new RuleMessage(this, trans);
                        rm->m_saveMessage = ruleMessage->m_saveMessage;
                        ruleMessage.reset(rm);
                    }

                    globalRet = true;
                }
            }
            delete v;
            v = NULL;
        }
        e.clear();
        e.reserve(4);
    }

    if (globalRet == false) {
        ms_dbg_a(trans, 4, "Rule returned 0.");
        cleanMatchedVars(trans);
        goto end_clean;
    }
    ms_dbg_a(trans, 4, "Rule returned 1.");

    if (this->isChained() == false) {
        goto end_exec;
    }

    /* FIXME: this check should happens on the parser. */
    if (this->m_chainedRuleChild == nullptr) {
        ms_dbg_a(trans, 4, "Rule is marked as chained but there " \
            "isn't a subsequent rule.");
        goto end_clean;
    }

    ms_dbg_a(trans, 4, "Executing chained rule.");
    recursiveGlobalRet = m_chainedRuleChild->evaluate(trans, ruleMessage);

    if (recursiveGlobalRet == true) {
        goto end_exec;
    }

end_clean:
    return false;

end_exec:
    executeActionsAfterFullMatch(trans, containsBlock, ruleMessage);

    /* last rule in the chain. */
    bool isItToBeLogged = (ruleMessage->m_saveMessage && (m_chainedRuleParent == nullptr));
    if (isItToBeLogged && !hasMultimatch()) {
        /* warn */
        trans->m_rulesMessages.push_back(*ruleMessage);

        /* error */
        if (!ruleMessage->m_isDisruptive) {
            trans->serverLog(ruleMessage);
        }
	}
    return true;
}


std::vector<actions::Action *> RuleWithActions::getActionsByName(const std::string& name,
    Transaction *trans) {
    std::vector<actions::Action *> ret;
    for (auto &z : m_actionsRuntimePos) {
        if (*z->m_name.get() == name) {
            ret.push_back(z);
        }
    }
    for (auto &z : m_transformations) {
        if (*z->m_name.get() == name) {
            ret.push_back(z);
        }
    }
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *z = dynamic_cast<actions::Action*>(b.second.get());
        if (*z->m_name.get() == name) {
            ret.push_back(z);
        }
    }
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pos_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *z = dynamic_cast<actions::Action*>(b.second.get());
        if (*z->m_name.get() == name) {
            ret.push_back(z);
        }
    }
    return ret;
}


std::string RuleWithOperator::getOperatorName() { return m_operator->m_op; }


}  // namespace modsecurity
