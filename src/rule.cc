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
#include "src/macro_expansion.h"
#include "src/actions/msg.h"
#include "src/actions/log_data.h"
#include "src/actions/severity.h"
#include "src/variables/variable.h"


namespace modsecurity {

using operators::Operator;
using actions::Action;
using Variables::Variable;
using actions::transformations::None;


Rule::Rule(std::string marker)
    : m_accuracy(0),
    m_actionsConf(),
    m_actionsRuntimePos(),
    m_actionsRuntimePre(),
    m_chained(false),
    m_chainedRule(NULL),
    m_fileName(""),
    m_lineNumber(0),
    m_logData(""),
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
    m_referenceCount(1) { }


Rule::Rule(Operator *_op,
        std::vector<Variable *> *_variables,
        std::vector<Action *> *actions,
        std::string fileName,
        int lineNumber)
    : m_accuracy(0),
    m_actionsConf(),
    m_actionsRuntimePos(),
    m_actionsRuntimePre(),
    m_chained(false),
    m_chainedRule(NULL),
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_logData(""),
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
    m_referenceCount(1) {
    if (actions != NULL) {
        for (Action *a : *actions) {
            if (a->action_kind == Action::ConfigurationKind) {
                m_actionsConf.push_back(a);
                a->evaluate(this, NULL);
            } else if (a->action_kind
                == Action::RunTimeBeforeMatchAttemptKind) {
                m_actionsRuntimePre.push_back(a);
            } else if (a->action_kind == Action::RunTimeOnlyIfMatchKind) {
                m_actionsRuntimePos.push_back(a);
            } else {
                std::cout << "General failure, action: " << a->m_name;
                std::cout << " has an unknown type." << std::endl;
                delete a;
            }
        }
    }
    /**
     * If phase is not entered, we assume phase 2. For historical reasons.
     *
     */
    if (m_phase == -1) {
        m_phase = modsecurity::Phases::RequestHeadersPhase;
    }

    if (m_op == NULL) {
        m_unconditional = true;
    }

    delete actions;
}


Rule::~Rule() {
    if (m_op != NULL) {
        delete m_op;
    }
    while (m_actionsConf.empty() == false) {
        auto *a = m_actionsConf.back();
        m_actionsConf.pop_back();
        delete a;
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


std::vector<std::string> Rule::getActionNames() {
    std::vector<std::string> a;
    for (auto &z : this->m_actionsRuntimePos) {
        a.push_back(z->m_name);
    }
    for (auto &z : this->m_actionsRuntimePre) {
        a.push_back(z->m_name);
    }
    for (auto &z : this->m_actionsConf) {
        a.push_back(z->m_name);
    }

    return a;
}


bool Rule::evaluateActions(Transaction *trans) {
    return true;
}


void Rule::updateMatchedVars(Transaction *trans, std::string key,
    std::string value) {
    trans->debug(4, "Matched vars updated.");
    trans->m_variableMatchedVar.set(value, trans->m_variableOffset);
    trans->m_variableMatchedVarName.set(key, trans->m_variableOffset);

    trans->m_variableMatchedVars.set(key, value, trans->m_variableOffset);
    trans->m_variableMatchedVarsNames.set(key, key, trans->m_variableOffset);
}


void Rule::cleanMatchedVars(Transaction *trans) {
    trans->debug(4, "Matched vars cleaned.");
    trans->m_variableMatchedVar.unset();
    trans->m_variableMatchedVars.unset();
    trans->m_variableMatchedVarName.unset();
    trans->m_variableMatchedVarsNames.unset();
}


void Rule::updateRulesVariable(Transaction *trans) {
    if (m_ruleId != 0) {
        trans->m_variableRule.set("id",
            std::to_string(m_ruleId), 0);
    }
    if (m_rev.empty() == false) {
        trans->m_variableRule.set("rev",
            m_rev, 0);
    }
    if (getActionsByName("msg").size() > 0) {
        actions::Msg *msg = dynamic_cast<actions::Msg*>(
            getActionsByName("msg")[0]);
        trans->m_variableRule.set("msg",
           msg->data(trans), 0);
    }
    if (getActionsByName("logdata").size() > 0) {
        actions::LogData *data = dynamic_cast<actions::LogData*>(
            getActionsByName("logdata")[0]);
        trans->m_variableRule.set("logdata",
            data->data(trans), 0);
    }
    if (getActionsByName("severity").size() > 0) {
        actions::Severity *data = dynamic_cast<actions::Severity*>(
            getActionsByName("severity")[0]);
        trans->m_variableRule.set("severity",
            std::to_string(data->m_severity), 0);
    }
}


std::string Rule::resolveMatchMessage(std::string key, std::string value) {
    std::string ret = this->m_op->m_match_message;

    if (ret.empty() == true) {
        ret = "Matched \"Operator `" + this->m_op->m_op + "' with parameter `" +
            utils::string::limitTo(200, this->m_op->m_param) +
            "' against variable `" + key + "' (Value: `" +
            utils::string::limitTo(100, utils::string::toHexIfNeeded(value)) +
            "' )";
    }

    return ret;
}


void Rule::executeActionsIndependentOfChainedRuleResult(Transaction *trans,
    bool *containsDisruptive, std::shared_ptr<RuleMessage> ruleMessage) {
    for (Action *a : this->m_actionsRuntimePos) {
        if (a->isDisruptive() == true) {
            if (a->m_name == "pass") {
                trans->debug(4, "Rule contains a `pass' action");
            } else {
                *containsDisruptive = true;
            }
        } else {
            if (a->m_name == "setvar"
                || a->m_name == "msg"
                || a->m_name == "log") {
                trans->debug(4, "Running [I] (_non_ disruptive) " \
                    "action: " + a->m_name);
                    a->evaluate(this, trans, ruleMessage);
            }
        }
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

    trans->debug(9, "Target value: \"" + utils::string::limitTo(80,
            utils::string::toHexIfNeeded(value)) \
            + "\" (Variable: " + key + ")");

    ret = this->m_op->evaluateInternal(trans, this, value, ruleMessage);
    if (ret == false) {
        return false;
    }

#if MSC_EXEC_CLOCK_ENABLED
    end = clock();
    elapsed_s = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    trans->debug(4, "Operator completed in " + \
        std::to_string(elapsed_s) + " seconds");
#endif
    return ret;
}


std::list<std::pair<std::shared_ptr<std::string>,
    std::shared_ptr<std::string>>>
    Rule::executeDefaultTransformations(

    Transaction *trans, const std::string &in, bool multiMatch) {
    int none = 0;
    int transformations = 0;

    std::list<std::pair<std::shared_ptr<std::string>,
        std::shared_ptr<std::string>>> ret;


    std::shared_ptr<std::string> value =
        std::shared_ptr<std::string>(new std::string(in));
    std::shared_ptr<std::string> newValue;

    std::shared_ptr<std::string> transStr =
        std::shared_ptr<std::string>(new std::string());

    if (multiMatch == true) {
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(value),
            std::shared_ptr<std::string>(transStr)));
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(value),
            std::shared_ptr<std::string>(transStr)));
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
                == actions::Action::RunTimeBeforeMatchAttemptKind) {
                newValue = std::unique_ptr<std::string>(
                    new std::string(a->evaluate(*value, trans)));

                if (multiMatch == true) {
                    if (*newValue != *value) {
                        ret.push_back(std::make_pair(
                            newValue,
                                          transStr));
                    }
                }
                value = std::shared_ptr<std::string>(newValue);
                if (transStr->empty()) {
                    transStr->append(a->m_name);
                } else {
                    transStr->append("," + a->m_name);
                }
                trans->debug(9, "(SecDefaultAction) T (" + \
                    std::to_string(transformations) + ") " + \
                    a->m_name + ": \"" + \
                    utils::string::limitTo(80, *value) +"\"");

                transformations++;
            }
        }
    }

    for (Action *a : this->m_actionsRuntimePre) {
        if (none == 0) {
            newValue = std::shared_ptr<std::string>(
                new std::string(a->evaluate(*value, trans)));

            if (multiMatch == true) {
                if (*value != *newValue) {
                    ret.push_back(std::make_pair(
                        newValue,
                                      transStr));
                    value = newValue;
                }
            }

            value = newValue;
            trans->debug(9, " T (" + \
                std::to_string(transformations) + ") " + \
                a->m_name + ": \"" + \
                utils::string::limitTo(80, *value) + "\"");
            if (transStr->empty()) {
                transStr->append(a->m_name);
            } else {
                transStr->append("," + a->m_name);
            }
            transformations++;
        }
        if (a->m_isNone) {
            none--;
        }
    }
    if (multiMatch == true) {
        // v2 checks the last entry twice. Don't know why.
        ret.push_back(ret.back());

        trans->debug(9, "multiMatch is enabled. " \
            + std::to_string(ret.size()) + \
            " values to be tested.");
    } else {
        ret.push_back(std::make_pair(
            std::shared_ptr<std::string>(value),
            std::shared_ptr<std::string>(transStr)));
    }

    return ret;
}


std::vector<std::unique_ptr<collection::Variable>> Rule::getFinalVars(
    Transaction *trans) {
    std::list<const std::string*> exclusions;
    std::vector<Variable *> *variables = m_variables;
    std::vector<std::unique_ptr<collection::Variable>> finalVars;

    for (int i = 0; i < variables->size(); i++) {
        Variable *variable = variables->at(i);
        if (variable->m_isExclusion) {
            std::vector<const collection::Variable *> z;
            variable->evaluateInternal(trans, this, &z);
            for (auto &y : z) {
                exclusions.push_back(y->m_key);
            }
            exclusions.push_back(&variable->m_name);
        }
    }

    for (int i = 0; i < variables->size(); i++) {
        Variable *variable = variables->at(i);
        std::vector<const collection::Variable *> e;
        bool ignoreVariable = false;

        if (variable->m_isExclusion) {
            continue;
        }

        variable->evaluateInternal(trans, this, &e);
        for (const collection::Variable *v : e) {
            const std::string *key = v->m_key;
            if (std::find_if(exclusions.begin(), exclusions.end(),
                [key](const std::string *m) -> bool { return *key == *m; })
                != exclusions.end()) {
#ifndef NO_LOGS
                trans->debug(9, "Variable: " + *key +
                    " is part of the exclusion list, skipping...");
#endif
                if (v->m_dynamic) {
                    delete v;
                    v = NULL;
                }
                continue;
            }

            for (auto &i : trans->m_ruleRemoveTargetByTag) {
                std::string tag = i.first;
                std::string args = i.second;
                if (containsTag(tag, trans) == false) {
                    continue;
                }
                if (args == *key) {
                    trans->debug(9, "Variable: " + *key +
                        " was excluded by ruleRemoteTargetByTag...");
                    ignoreVariable = true;
                    break;
                }
            }
            if (ignoreVariable) {
                if (v->m_dynamic) {
                    delete v;
                    v = NULL;
                }
                continue;
            }

            for (auto &i : trans->m_ruleRemoveTargetById) {
                int id = i.first;
                std::string args = i.second;
                if (m_ruleId != id) {
                    continue;
                }
                if (args == *key) {
                    trans->debug(9, "Variable: " + *key +
                        " was excluded by ruleRemoteTargetById...");
                    ignoreVariable = true;
                    break;
                }
            }
            if (ignoreVariable) {
                if (v->m_dynamic) {
                    delete v;
                    v = NULL;
                }
                continue;
            }

            std::unique_ptr<collection::Variable> var(new collection::Variable(
                new std::string(*v->m_key),
                new std::string(*v->m_value)));
            var->m_dynamic_value = true;
            var->m_dynamic_key = true;
            for (auto &i : v->m_orign) {
                std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
                origin->m_offset = i->m_offset;
                origin->m_length = i->m_length;
                var->m_orign.push_back(std::move(origin));
            }

            if (v->m_dynamic) {
                delete v;
                v = NULL;
            }
            finalVars.push_back(std::move(var));
        }
    }
    return finalVars;
}


void Rule::executeActionsAfterFullMatch(Transaction *trans,
    bool containsDisruptive, std::shared_ptr<RuleMessage> ruleMessage) {

    for (Action *a : trans->m_rules->m_defaultActions[this->m_phase]) {
        if (a->action_kind != actions::Action::RunTimeOnlyIfMatchKind) {
            continue;
        }

        if (a->isDisruptive() == false) {
            trans->debug(4, "(SecDefaultAction) Running " \
                "action: " + a->m_name);
                a->evaluate(this, trans, ruleMessage);
            continue;
        }

        if (containsDisruptive) {
            trans->debug(4, "(SecDefaultAction) _ignoring_ " \
                "action: " + a->m_name + \
                " (rule contains a disruptive action)");
            continue;
        }

        if (trans->m_rules->m_secRuleEngine == Rules::EnabledRuleEngine) {
            trans->debug(4, "(SecDefaultAction) " \
                "Running action: " + a->m_name + \
                " (rule _does not_ contains a " \
                "disruptive action)");
            a->evaluate(this, trans, ruleMessage);
            continue;
        }

        trans->debug(4, "(SecDefaultAction) _Not_ running action: " \
                + a->m_name + ". Rule _does not_contains a disruptive action,"\
                + " but SecRuleEngine is not On.");
    }

    for (Action *a : this->m_actionsRuntimePos) {
        if (a->isDisruptive() == false) {
            if (a->m_name != "setvar" && a->m_name != "log"
                && a->m_name != "msg") {
                trans->debug(4, "Running [I] (_non_ disruptive) " \
                    "action: " + a->m_name);
                    a->evaluate(this, trans, ruleMessage);
            }
            continue;
        }
        if (trans->m_rules->m_secRuleEngine == Rules::EnabledRuleEngine) {
            trans->debug(4, "Running (disruptive) action: " + a->m_name);
            a->evaluate(this, trans, ruleMessage);
            continue;
        }

        trans->debug(4, "Not running disruptive action: " + \
                a->m_name + ". SecRuleEngine is not On");
    }
}


bool Rule::evaluate(Transaction *trans,
    std::shared_ptr<RuleMessage> ruleMessage) {
    bool globalRet = false;
    std::vector<Variable *> *variables = this->m_variables;
    bool recursiveGlobalRet;
    bool containsDisruptive = false;
    std::vector<std::unique_ptr<collection::Variable>> finalVars;
    std::string eparam;

    if (ruleMessage == NULL) {
        ruleMessage = std::shared_ptr<RuleMessage>(
            new RuleMessage(this, trans));
    }

    trans->m_matched.clear();

    if (m_secMarker == true) {
        return true;
    }
    if (m_unconditional == true) {
        trans->debug(4, "(Rule: " + std::to_string(m_ruleId) \
            + ") Executing unconditional rule...");
        executeActionsIndependentOfChainedRuleResult(trans,
            &containsDisruptive, ruleMessage);
        goto end_exec;
    }

    for (auto &i : trans->m_ruleRemoveById) {
        if (m_ruleId != i) {
            continue;
        }
        trans->debug(9, "Rule id: " + std::to_string(m_ruleId) +
            " was skipped due to an ruleRemoveById action...");
        return true;
    }

    eparam = MacroExpansion::expand(this->m_op->m_param, trans);

    if (this->m_op->m_param != eparam) {
        eparam = "\"" + eparam + "\" Was: \"" + this->m_op->m_param + "\"";
    } else {
        eparam = "\"" + eparam + "\"";
    }

    trans->debug(4, "(Rule: " + std::to_string(m_ruleId) \
        + ") Executing operator \"" + this->m_op->m_op \
        + "\" with param " \
        + eparam \
        + " against " \
        + Variable::to_s(variables) + ".");

    updateRulesVariable(trans);

    finalVars = getFinalVars(trans);

    for (auto &v : finalVars) {
        const std::string value = *(v->m_value);
        const std::string key = *(v->m_key);

        std::list<std::pair<std::shared_ptr<std::string>,
            std::shared_ptr<std::string>>> values;

        bool multiMatch = getActionsByName("multimatch").size() > 0;

        values = executeDefaultTransformations(trans, value,
            multiMatch);
        for (const auto &valueTemp : values) {
            bool ret;
            std::string valueAfterTrans = std::move(*valueTemp.first);

            ret = executeOperatorAt(trans, key, valueAfterTrans, ruleMessage);

            if (ret == true) {
                ruleMessage->m_match = resolveMatchMessage(key, value);
                for (auto &i : v->m_orign) {
                    ruleMessage->m_reference.append(i->toText());
                }
                ruleMessage->m_reference.append(*valueTemp.second);
                updateMatchedVars(trans, key, value);
                executeActionsIndependentOfChainedRuleResult(trans,
                    &containsDisruptive, ruleMessage);
                globalRet = true;
            }
        }
    }

    if (globalRet == false) {
        trans->debug(4, "Rule returned 0.");
        cleanMatchedVars(trans);
        goto end_clean;
    }

    trans->debug(4, "Rule returned 1.");

    if (this->m_chained == false) {
        goto end_exec;
    }

    if (this->m_chainedRule == NULL) {
        trans->debug(4, "Rule is marked as chained but there " \
            "isn't a subsequent rule.");
        goto end_clean;
    }

    trans->debug(4, "Executing chained rule.");
    recursiveGlobalRet = this->m_chainedRule->evaluate(trans, ruleMessage);

    if (recursiveGlobalRet == true) {
        goto end_exec;
    }

end_clean:
    return false;

end_exec:
    executeActionsAfterFullMatch(trans, containsDisruptive, ruleMessage);
    if (this->m_chained == false) {
        trans->serverLog(ruleMessage);
        trans->m_rulesMessages.push_back(*ruleMessage);
    }

    return true;
}


std::vector<actions::Action *> Rule::getActionsByName(const std::string& name) {
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
    for (auto &z : m_actionsConf) {
        if (z->m_name == name) {
            ret.push_back(z);
        }
    }
    return ret;
}


bool Rule::containsTag(const std::string& name, Transaction *t) {
    for (auto &z : this->m_actionsRuntimePos) {
        actions::Tag *tag = dynamic_cast<actions::Tag *> (z);
        if (tag != NULL && tag->getName(t) == name) {
            return true;
        }
    }
    return false;
}


}  // namespace modsecurity
