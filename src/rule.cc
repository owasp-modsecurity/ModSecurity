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

#include "src/operators/operator.h"
#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "src/actions/transformations/none.h"
#include "src/actions/tag.h"
#include "src/utils/string.h"
#include "modsecurity/rules.h"
#include "modsecurity/rule_message.h"
#include "src/actions/msg.h"
#include "src/actions/log_data.h"
#include "src/actions/severity.h"
#include "src/actions/capture.h"
#include "src/actions/multi_match.h"
#include "src/actions/set_var.h"
#include "src/actions/disruptive/block.h"
#include "src/variables/variable.h"


namespace modsecurity {

using operators::Operator;
using actions::Action;
using Variables::Variable;
using actions::transformations::None;


Rule::Rule(std::string marker)
    : m_accuracy(0),
    m_actionsRuntimePos(),
    m_actionsRuntimePre(),
    m_actionsSetVar(),
    m_actionsTag(),
    m_chained(false),
    m_chainedRule(NULL),
    m_fileName(""),
    m_lineNumber(0),
    m_marker(marker),
    m_maturity(0),
    m_op(NULL),
    m_phase(-1),
    m_rev(""),
    m_ruleId(0),
    m_secMarker(true),
    m_variables(NULL),
    m_ver(""),
    m_unconditional(false),
    m_referenceCount(1),
    m_containsStaticDisruptiveAction(false),
    m_containsStaticBlockAction(false),
    m_containsCaptureAction(false),
    m_containsMultiMatchAction(false),
    m_severity(nullptr),
    m_logData(nullptr),
    m_msg(nullptr) { }


Rule::Rule(Operator *_op,
    Variables::Variables *_variables,
    std::vector<Action *> *actions,
    std::string fileName,
    int lineNumber)
    : m_accuracy(0),
    m_actionsRuntimePos(),
    m_actionsRuntimePre(),
    m_actionsSetVar(),
    m_actionsTag(),
    m_chained(false),
    m_chainedRule(NULL),
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_marker(""),
    m_maturity(0),
    m_op(_op),
    m_phase(-1),
    m_rev(""),
    m_ruleId(0),
    m_secMarker(false),
    m_variables(_variables),
    m_ver(""),
    m_unconditional(false),
    m_referenceCount(1),
    m_containsStaticDisruptiveAction(false),
    m_containsStaticBlockAction(false),
    m_containsCaptureAction(false),
    m_containsMultiMatchAction(false),
    m_severity(nullptr),
    m_logData(nullptr),
    m_msg(nullptr) {
    /* */
    organizeActions(actions);

    /**
     * If phase is not entered, we assume phase 2. For historical reasons.
     *
     */
    if (m_phase == -1) {
        m_phase = modsecurity::Phases::RequestHeadersPhase;
    }

    m_unconditional = (m_op == NULL);

    delete actions;
}


Rule::~Rule() {
    if (m_op != NULL) {
        delete m_op;
    }

    cleanUpActions();

    while (m_variables != NULL && m_variables->empty() == false) {
        auto *a = m_variables->back();
        m_variables->pop_back();
        delete a;
    }

    if (m_variables != NULL) {
        delete m_variables;
    }

    if (m_chainedRule != NULL) {
        delete m_chainedRule;
    }
}


void Rule::organizeActions(std::vector<Action *> *actions) {
    if (!actions) {
        return;
    }
    for (Action *a : *actions) {
        if (a->action_kind == Action::ConfigurationKind) {
            a->evaluate(this, NULL);
            delete a;
        } else if (a->action_kind == Action::RunTimeBeforeMatchAttemptKind) {
            m_actionsRuntimePre.push_back(a);
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
            } else if (dynamic_cast<actions::disruptive::Block *>(a)) {
                m_actionsRuntimePos.push_back(a);
                m_containsStaticBlockAction = true;
            } else if (a->isDisruptive() == true) {
                m_actionsRuntimePos.push_back(a);
                m_containsStaticDisruptiveAction = true;
            } else {
                m_actionsRuntimePos.push_back(a);
            }
        } else {
            std::cout << "General failure, action: " << a->m_name;
            std::cout << " has an unknown type." << std::endl;
            delete a;
        }
    }
}


void Rule::cleanUpActions() {
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
    while (m_actionsRuntimePre.empty() == false) {
        auto *a = m_actionsRuntimePre.back();
        m_actionsRuntimePre.pop_back();
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
}


void Rule::updateMatchedVars(Transaction *trans, std::string key,
    std::string value) {
#ifndef NO_LOGS
    trans->debug(9, "Matched vars updated.");
#endif
    trans->m_variableMatchedVar.set(value, trans->m_variableOffset);
    trans->m_variableMatchedVarName.set(key, trans->m_variableOffset);

    trans->m_variableMatchedVars.set(key, value, trans->m_variableOffset);
    trans->m_variableMatchedVarsNames.set(key, key, trans->m_variableOffset);
}


void Rule::cleanMatchedVars(Transaction *trans) {
#ifndef NO_LOGS
    trans->debug(9, "Matched vars cleaned.");
#endif
    trans->m_variableMatchedVar.unset();
    trans->m_variableMatchedVars.unset();
    trans->m_variableMatchedVarName.unset();
    trans->m_variableMatchedVarsNames.unset();
}


void Rule::updateRulesVariable(Transaction *trans,
    std::shared_ptr<RuleMessage> rm) {
    if (m_ruleId != 0) {
        trans->m_variableRule.set("id", std::to_string(m_ruleId), 0);
    }
    if (m_rev.empty() == false) {
        trans->m_variableRule.set("rev", m_rev, 0);
    }
    if (m_severity) {
        trans->m_variableRule.set("severity",
            std::to_string(m_severity->m_severity), 0);
    }
    if (m_logData) {
        trans->m_variableRule.set("logdata", m_logData->data(trans), 0);
    }
    if (m_msg) {
        trans->m_variableRule.set("msg", m_msg->data(trans), 0);
    }
}


void Rule::executeActionsIndependentOfChainedRuleResult(Transaction *trans,
    bool *containsBlock, std::shared_ptr<RuleMessage> ruleMessage) {

    for (actions::SetVar *a : m_actionsSetVar) {
#ifndef NO_LOGS
        trans->debug(4, "Running [independent] (non-disruptive) " \
            "action: " + a->m_name);
#endif
        a->evaluate(this, trans);
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        if (a->isDisruptive() == true && a->m_name == "block") {
#ifndef NO_LOGS
            trans->debug(9, "Rule contains a `block' action");
                *containsBlock = true;
#endif
        } else if (a->m_name == "setvar") {
#ifndef NO_LOGS
            trans->debug(4, "Running [independent] (non-disruptive) " \
                "action: " + a->m_name);
#endif
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


bool Rule::executeOperatorAt(Transaction *trans, std::string key,
    std::string value, std::shared_ptr<RuleMessage> ruleMessage) {
#if MSC_EXEC_CLOCK_ENABLED
    clock_t begin = clock();
    clock_t end;
    double elapsed_s = 0;
#endif
    bool ret;

#ifndef NO_LOGS
    if (trans && trans->m_rules && trans->m_rules->m_debugLog
        && trans->m_rules->m_debugLog->getDebugLogLevel() >= 9) {
        trans->debug(9, "Target value: \"" + utils::string::limitTo(80,
            utils::string::toHexIfNeeded(value)) \
            + "\" (Variable: " + key + ")");
    }
#endif

    ret = this->m_op->evaluateInternal(trans, this, value, ruleMessage);
    if (ret == false) {
        return false;
    }

#if MSC_EXEC_CLOCK_ENABLED
    end = clock();
    elapsed_s = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

#ifndef NO_LOGS
    trans->debug(5, "Operator completed in " + \
        std::to_string(elapsed_s) + " seconds");
#endif
#endif
    return ret;
}


inline void Rule::executeTransformation(actions::Action *a,
    std::shared_ptr<std::string> *value,
    Transaction *trans,
    std::list<std::pair<std::shared_ptr<std::string>,
        std::shared_ptr<std::string>>> *ret,
    std::string *path,
    int *nth) {

    std::string *oldValue = (*value).get();
    std::string newValue = a->evaluate(*oldValue, trans);

    if (newValue != *oldValue) {
        std::shared_ptr<std::string> u(new std::string(newValue));
        if (m_containsMultiMatchAction) {
            std::shared_ptr<std::string> t(new std::string(a->m_name));
            ret->push_back(std::make_pair(u, t));
        }
        *value = u;
    }

    if (path->empty()) {
        path->append(a->m_name);
    } else {
        path->append("," + a->m_name);
    }

#ifndef NO_LOGS
    trans->debug(9, " T (" + \
        std::to_string(*nth) + ") " + \
        a->m_name + ": \"" + \
        utils::string::limitTo(80, newValue) +"\"");
#endif
    (*nth)++;
}


std::list<std::pair<std::shared_ptr<std::string>,
    std::shared_ptr<std::string>>>
    Rule::executeDefaultTransformations(
    Transaction *trans, const std::string &in) {
    int none = 0;
    int transformations = 0;
    std::string path("");
    std::list<std::pair<std::shared_ptr<std::string>,
        std::shared_ptr<std::string>>> ret;
    std::shared_ptr<std::string> value =
        std::shared_ptr<std::string>(new std::string(in));

    if (m_containsMultiMatchAction == true) {
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(value),
            std::shared_ptr<std::string>(new std::string(path))));
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(value),
            std::shared_ptr<std::string>(new std::string(path))));
    }

    for (Action *a : this->m_actionsRuntimePre) {
        if (a->m_isNone) {
            none++;
        }
    }

    // Check for transformations on the SecDefaultAction
    // Notice that first we make sure that won't be a t:none
    // on the target rule.
    if (none == 0) {
        for (Action *a : trans->m_rules->m_defaultActions[this->m_phase]) {
            if (a->action_kind \
                != actions::Action::RunTimeBeforeMatchAttemptKind) {
                continue;
            }

            executeTransformation(a, &value, trans, &ret, &path,
                &transformations);
        }
    }

    for (Action *a : this->m_actionsRuntimePre) {
        if (none == 0) {
            executeTransformation(a, &value, trans, &ret, &path,
                &transformations);
        }
        if (a->m_isNone) {
            none--;
        }
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        if (a->m_isNone) {
            none++;
        }
    }

    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        if (none == 0) {
            executeTransformation(a, &value, trans, &ret, &path,
                &transformations);
        }
        if (a->m_isNone) {
            none--;
        }
    }

    if (m_containsMultiMatchAction == true) {
        // v2 checks the last entry twice. Don't know why.
        ret.push_back(ret.back());

#ifndef NO_LOGS
        trans->debug(9, "multiMatch is enabled. " \
            + std::to_string(ret.size()) + \
            " values to be tested.");
#endif
    } else {
        ret.push_back(std::make_pair(
            value,
            std::shared_ptr<std::string>(new std::string(path))));
    }

    return ret;
}


void Rule::getVariablesExceptions(Transaction *t,
    Variables::Variables *exclusion, Variables::Variables *addition) {
    for (auto &a : t->m_rules->m_exceptions.m_variable_update_target_by_tag) {
        if (containsTag(*a.first.get(), t) == false) {
            continue;
        }
        Variable *b = a.second.get();
        if (dynamic_cast<Variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<Variables::VariableModificatorExclusion*>(
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
        if (dynamic_cast<Variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<Variables::VariableModificatorExclusion*>(
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
        if (dynamic_cast<Variables::VariableModificatorExclusion*>(b)) {
            exclusion->push_back(
                dynamic_cast<Variables::VariableModificatorExclusion*>(
                    b)->m_base.get());
        } else {
            addition->push_back(b);
        }
    }
}


inline void Rule::getFinalVars(Variables::Variables *vars,
    Variables::Variables *exclusion, Transaction *trans) {
    Variables::Variables addition;

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



void Rule::executeAction(Transaction *trans,
    bool containsBlock, std::shared_ptr<RuleMessage> ruleMessage,
    Action *a, bool defaultContext) {
    if (a->isDisruptive() == false) {
#ifndef NO_LOGS
        trans->debug(9, "Running " \
            "action: " + a->m_name);
#endif
        a->evaluate(this, trans, ruleMessage);
        return;
    }

    if (defaultContext && !containsBlock) {
#ifndef NO_LOGS
        trans->debug(4, "Ignoring action: " + a->m_name + \
            " (rule does not cotains block)");
#endif
        return;
    }

    if (trans->getRuleEngineState() == Rules::EnabledRuleEngine) {
#ifndef NO_LOGS
        trans->debug(4, "Running (disruptive)     action: " + a->m_name + \
            ".");
#endif
        a->evaluate(this, trans, ruleMessage);
        return;
    }

#ifndef NO_LOGS
    trans->debug(4, "Not running disruptive action: " \
        + a->m_name + ". SecRuleEngine is not On.");
#endif
}



void Rule::executeActionsAfterFullMatch(Transaction *trans,
    bool containsBlock, std::shared_ptr<RuleMessage> ruleMessage) {

    for (Action *a : trans->m_rules->m_defaultActions[this->m_phase]) {
        if (a->action_kind != actions::Action::RunTimeOnlyIfMatchKind) {
            continue;
        }
        executeAction(trans, containsBlock, ruleMessage, a, true);
    }

    for (actions::Tag *a : this->m_actionsTag) {
#ifndef NO_LOGS
        trans->debug(4, "Running (non-disruptive) action: " \
            + a->m_name);
#endif
        a->evaluate(this, trans, ruleMessage);
    }

    for (Action *a : this->m_actionsRuntimePos) {
        executeAction(trans, containsBlock, ruleMessage, a, false);
    }
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pos_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *a = dynamic_cast<actions::Action*>(b.second.get());
        executeAction(trans, containsBlock, ruleMessage, a, false);
    }
}


bool Rule::evaluate(Transaction *trans,
    std::shared_ptr<RuleMessage> ruleMessage) {
    bool globalRet = false;
    Variables::Variables *variables = this->m_variables;
    bool recursiveGlobalRet;
    bool containsBlock = m_containsStaticBlockAction;
    std::vector<std::unique_ptr<VariableValue>> finalVars;
    std::string eparam;
    Variables::Variables vars;
    vars.reserve(4);
    Variables::Variables exclusion;

    if (ruleMessage == NULL) {
        ruleMessage = std::shared_ptr<RuleMessage>(
            new RuleMessage(this, trans));
    }

    trans->m_matched.clear();

    if (m_secMarker == true) {
        return true;
    }

    if (m_unconditional == true) {
#ifndef NO_LOGS
        trans->debug(4, "(Rule: " + std::to_string(m_ruleId) \
            + ") Executing unconditional rule...");
#endif
        executeActionsIndependentOfChainedRuleResult(trans,
            &containsBlock, ruleMessage);
        goto end_exec;
    }

    for (auto &i : trans->m_ruleRemoveById) {
        if (m_ruleId != i) {
            continue;
        }
#ifndef NO_LOGS
        trans->debug(9, "Rule id: " + std::to_string(m_ruleId) +
            " was skipped due to a ruleRemoveById action...");
#endif
        return true;
    }

    if (m_op->m_string) {
        eparam = m_op->m_string->evaluate(trans);

        if (m_op->m_string->containsMacro()) {
            eparam = "\"" + eparam + "\" Was: \"" \
                + m_op->m_string->evaluate(NULL) + "\"";
        } else {
            eparam = "\"" + eparam + "\"";
        }
#ifndef NO_LOGS
    trans->debug(4, "(Rule: " + std::to_string(m_ruleId) \
        + ") Executing operator \"" + this->m_op->m_op \
        + "\" with param " \
        + eparam \
        + " against " \
        + variables + ".");
#endif
    } else {
#ifndef NO_LOGS
    trans->debug(4, "(Rule: " + std::to_string(m_ruleId) \
        + ") Executing operator \"" + this->m_op->m_op \
        + " against " \
        + variables + ".");
#endif
    }

    updateRulesVariable(trans, ruleMessage);

    getFinalVars(&vars, &exclusion, trans);

    for (auto &var : vars) {
        std::vector<const VariableValue *> e;
        var->evaluate(trans, this, &e);
        for (const VariableValue *v : e) {
            const std::string &value = v->m_value;
            const std::string &key = v->m_key;

            if (exclusion.contains(v->m_key) ||
                std::find_if(trans->m_ruleRemoveTargetById.begin(),
                    trans->m_ruleRemoveTargetById.end(),
                    [&, v, this](std::pair<int, std::string> &m) -> bool {
                        return m.first == m_ruleId && m.second == v->m_key;
                    }) != trans->m_ruleRemoveTargetById.end()
            ) {
                delete v;
                v = NULL;
                continue;
            }
            if (exclusion.contains(v->m_key) ||
                std::find_if(trans->m_ruleRemoveTargetByTag.begin(),
                    trans->m_ruleRemoveTargetByTag.end(),
                    [&, v, trans, this](std::pair<std::string, std::string> &m) -> bool {
                        return containsTag(m.first, trans) && m.second == v->m_key;
                    }) != trans->m_ruleRemoveTargetByTag.end()
            ) {
                delete v;
                v = NULL;
                continue;
            }

            std::list<std::pair<std::shared_ptr<std::string>,
                std::shared_ptr<std::string>>> values;

            values = executeDefaultTransformations(trans, value);

            for (const auto &valueTemp : values) {
                bool ret;
                std::string valueAfterTrans = std::move(*valueTemp.first);

                ret = executeOperatorAt(trans, key, valueAfterTrans, ruleMessage);

                if (ret == true) {
                    ruleMessage->m_match = m_op->resolveMatchMessage(trans,
                        key, value);
                    for (auto &i : v->m_orign) {
                        ruleMessage->m_reference.append(i->toText());
                    }
                    ruleMessage->m_reference.append(*valueTemp.second);
                    updateMatchedVars(trans, key, value);
                    executeActionsIndependentOfChainedRuleResult(trans,
                        &containsBlock, ruleMessage);
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
#ifndef NO_LOGS
        trans->debug(4, "Rule returned 0.");
#endif
        cleanMatchedVars(trans);
        goto end_clean;
    }

#ifndef NO_LOGS
    trans->debug(4, "Rule returned 1.");
#endif

    if (this->m_chained == false) {
        goto end_exec;
    }

    if (this->m_chainedRule == NULL) {
#ifndef NO_LOGS
        trans->debug(4, "Rule is marked as chained but there " \
            "isn't a subsequent rule.");
#endif
        goto end_clean;
    }

#ifndef NO_LOGS
    trans->debug(4, "Executing chained rule.");
#endif
    recursiveGlobalRet = this->m_chainedRule->evaluate(trans, ruleMessage);

    if (recursiveGlobalRet == true) {
        goto end_exec;
    }

end_clean:
    return false;

end_exec:
    executeActionsAfterFullMatch(trans, containsBlock, ruleMessage);
    if (m_ruleId != 0 && ruleMessage->m_saveMessage != false) {
        trans->serverLog(ruleMessage);
        trans->m_rulesMessages.push_back(*ruleMessage);
    }

    return true;
}


std::vector<actions::Action *> Rule::getActionsByName(const std::string& name,
    Transaction *trans) {
    std::vector<actions::Action *> ret;
    for (auto &z : m_actionsRuntimePos) {
        if (z->m_name == name) {
            ret.push_back(z);
        }
    }
    for (auto &z : m_actionsRuntimePre) {
        if (z->m_name == name) {
            ret.push_back(z);
        }
    }
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pre_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *z = dynamic_cast<actions::Action*>(b.second.get());
        if (z->m_name == name) {
            ret.push_back(z);
        }
    }
    for (auto &b :
        trans->m_rules->m_exceptions.m_action_pos_update_target_by_id) {
        if (m_ruleId != b.first) {
            continue;
        }
        actions::Action *z = dynamic_cast<actions::Action*>(b.second.get());
        if (z->m_name == name) {
            ret.push_back(z);
        }
    }
    return ret;
}


bool Rule::containsTag(const std::string& name, Transaction *t) {
    for (auto &tag : m_actionsTag) {
        if (tag != NULL && tag->getName(t) == name) {
            return true;
        }
    }
    return false;
}


bool Rule::containsMsg(const std::string& name, Transaction *t) {
    return m_msg && m_msg->data(t) == name;
}


}  // namespace modsecurity
