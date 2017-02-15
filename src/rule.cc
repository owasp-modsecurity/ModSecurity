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
#include "src/variables/variations/exclusion.h"
#include "src/utils/string.h"
#include "modsecurity/rules.h"
#include "modsecurity/rule_message.h"
#include "src/macro_expansion.h"
#include "src/actions/msg.h"
#include "src/actions/log_data.h"
#include "src/actions/severity.h"


using modsecurity::Variables::Variations::Exclusion;

namespace modsecurity {

using operators::Operator;
using actions::Action;
using Variables::Variable;
using actions::transformations::None;

Rule::~Rule() {
    if (op != NULL) {
        delete op;
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
    while (variables != NULL && variables->empty() == false) {
        auto *a = variables->back();
        variables->pop_back();
        delete a;
    }

    if (variables != NULL) {
        delete variables;
    }

    if (chainedRule != NULL) {
        delete chainedRule;
    }
}

Rule::Rule(std::string marker)
    : chained(false),
    chainedRule(NULL),
    variables(NULL),
    op(NULL),
    rule_id(0),
    phase(-1),
    m_accuracy(0),
    m_unconditional(false),
    m_secmarker(true),
    m_marker(marker),
    m_maturity(0),
    m_referenceCount(0),
    m_fileName(""),
    m_lineNumber(0) { }

Rule::Rule(Operator *_op,
        std::vector<Variable *> *_variables,
        std::vector<Action *> *actions,
        std::string fileName,
        int lineNumber): chained(false),
    chainedRule(NULL),
    variables(_variables),
    op(_op),
    rule_id(0),
    phase(-1),
    m_accuracy(0),
    m_unconditional(false),
    m_secmarker(false),
    m_marker(""),
    m_maturity(0),
    m_referenceCount(0),
    m_fileName(fileName),
    m_lineNumber(lineNumber) {
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
    if (phase == -1) {
        phase = modsecurity::Phases::RequestHeadersPhase;
    }

    if (op == NULL) {
        m_unconditional = true;
    }

    delete actions;
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
    trans->m_collections.storeOrUpdateFirst("MATCHED_VAR", value);
    trans->m_collections.storeOrUpdateFirst("MATCHED_VAR_NAME", key);
    trans->m_collections.store("MATCHED_VARS:" + key, value);
    trans->m_collections.store("MATCHED_VARS_NAMES:" + key, key);
}


void Rule::cleanMatchedVars(Transaction *trans) {
    trans->debug(4, "Matched vars cleaned.");
    trans->m_collections.storeOrUpdateFirst("MATCHED_VAR", "");
    trans->m_collections.del("MATCHED_VARS_NAME");
    trans->m_collections.del("MATCHED_VARS");
    trans->m_collections.del("MATCHED_VARS_NAMES");
}


void Rule::updateRulesVariable(Transaction *trans) {
    if (rule_id != 0) {
        trans->m_collections.storeOrUpdateFirst("RULE:id",
            std::to_string(rule_id));
    }
    if (m_rev.empty() == false) {
        trans->m_collections.storeOrUpdateFirst("RULE:rev", m_rev);
    }
    if (getActionsByName("msg").size() > 0) {
        actions::Msg *msg = dynamic_cast<actions::Msg*>(
            getActionsByName("msg")[0]);
        trans->m_collections.storeOrUpdateFirst("RULE:msg", msg->data(trans));
    }
    if (getActionsByName("logdata").size() > 0) {
        actions::LogData *data = dynamic_cast<actions::LogData*>(
            getActionsByName("logdata")[0]);
        trans->m_collections.storeOrUpdateFirst("RULE:logdata",
            data->data(trans));
    }
    if (getActionsByName("severity").size() > 0) {
        actions::Severity *data = dynamic_cast<actions::Severity*>(
            getActionsByName("severity")[0]);
        trans->m_collections.storeOrUpdateFirst("RULE:severity",
            std::to_string(data->m_severity));
    }
}


std::string Rule::resolveMatchMessage(std::string key, std::string value) {
    std::string ret = this->op->m_match_message;

    if (ret.empty() == true) {
        ret = "Matched \"Operator `" + this->op->m_op + "' with parameter `" +
            utils::string::limitTo(200, this->op->m_param) +
            "' against variable `" + key + "' (Value: `" +
            utils::string::limitTo(100, utils::string::toHexIfNeeded(value)) +
            "' ) \" at " + key;
    }

    return ret;
}


void Rule::executeActionsIndependentOfChainedRuleResult(Transaction *trans,
    bool *containsDisruptive, RuleMessage *ruleMessage) {
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
    std::string value) {
    clock_t begin = clock();
    clock_t end;
    double elapsed_s = 0;
    bool ret;

    trans->debug(9, "Target value: \"" + utils::string::limitTo(80,
            utils::string::toHexIfNeeded(value)) \
            + "\" (Variable: " + key + ")");

    ret = this->op->evaluateInternal(trans, this, value);
    if (ret == false) {
        return false;
    }

    end = clock();
    elapsed_s = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    trans->debug(4, "Operator completed in " + \
        std::to_string(elapsed_s) + " seconds");

    return ret;
}

// FIXME: this should be a list instead of a vector, keeping the but
// of v2 alive.
std::vector<std::string> Rule::executeSecDefaultActionTransofrmations(
    Transaction *trans, std::string &value, bool multiMatch) {
    int none = 0;
    int transformations = 0;
    std::vector<std::string> ret;

    if (multiMatch == true) {
        ret.push_back(value);
        ret.push_back(value);
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
        for (Action *a : trans->m_rules->defaultActions[this->phase]) {
            if (a->action_kind \
                == actions::Action::RunTimeBeforeMatchAttemptKind) {
                std::string oldValue = value;
                if (multiMatch) {
                    oldValue = ret.back();
                }
                std::string newValue = a->evaluate(oldValue, trans);
                if (multiMatch == true) {
                    if (newValue != oldValue) {
                        ret.push_back(newValue);
                    }
                } else {
                    value = newValue;
                }

                trans->debug(9, "(SecDefaultAction) T (" + \
                    std::to_string(transformations) + ") " + \
                    a->m_name + ": \"" + \
                    utils::string::limitTo(80, newValue) +"\"");

                transformations++;
            }
        }
    }

    for (Action *a : this->m_actionsRuntimePre) {
        if (none == 0) {
            std::string oldValue = value;
            if (multiMatch) {
                oldValue = ret.back();
            }

            std::string newValue = a->evaluate(oldValue, trans);
            if (multiMatch == true) {
                if (newValue != oldValue) {
                    ret.push_back(newValue);
                }
            } else {
                value = newValue;
            }

            trans->debug(9, " T (" + \
                std::to_string(transformations) + ") " + \
                a->m_name + ": \"" + \
                utils::string::limitTo(80, newValue) + "\"");
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
        for (const std::string &a : ret) {
            trans->debug(9, " - " + a);
        }
    } else {
        ret.push_back(value);
    }

    return ret;
}


std::vector<const collection::Variable *> Rule::getFinalVars(
    Transaction *trans) {
    std::list<std::string> exclusions;
    std::vector<Variable *> *variables = this->variables;
    std::vector<const collection::Variable *> finalVars;

    for (int i = 0; i < variables->size(); i++) {
        Variable *variable = variables->at(i);
        if (variable->m_isExclusion) {
            std::vector<const collection::Variable *> z;
            variable->evaluateInternal(trans, this, &z);
            for (auto &y : z) {
                exclusions.push_back(y->m_key);
                delete y;
            }
            exclusions.push_back(variable->m_name);
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
            if (std::find(exclusions.begin(), exclusions.end(),
                v->m_key) != exclusions.end()) {
#ifndef NO_LOGS
                trans->debug(9, "Variable: " + v->m_key +
                    " is part of the exclusion list, skipping...");
#endif
                continue;
            }

            for (auto &i : trans->m_ruleRemoveTargetByTag) {
                std::string tag = i.first;
                std::string args = i.second;
                if (containsTag(tag, trans) == false) {
                    continue;
                }
                if (args == v->m_key) {
                    trans->debug(9, "Variable: " + v->m_key +
                        " was excluded by ruleRemoteTargetByTag...");
                    ignoreVariable = true;
                    break;
                }
            }
            if (ignoreVariable) {
                continue;
            }

            for (auto &i : trans->m_ruleRemoveTargetById) {
                int id = i.first;
                std::string args = i.second;
                if (rule_id != id) {
                    continue;
                }
                if (args == v->m_key) {
                    trans->debug(9, "Variable: " + v->m_key +
                        " was excluded by ruleRemoteTargetById...");
                    ignoreVariable = true;
                    break;
                }
            }
            if (ignoreVariable) {
                continue;
            }

            finalVars.push_back(v);
        }
    }
    return finalVars;
}


void Rule::executeActionsAfterFullMatch(Transaction *trans,
    bool containsDisruptive, RuleMessage *ruleMessage) {

    for (Action *a : trans->m_rules->defaultActions[this->phase]) {
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
        } else {
            trans->debug(4, "_Not_ running (disruptive) action: "
                + a->m_name + ". SecRuleEngine is not On.");
            continue;
        }

        trans->debug(4, "Not running disruptive action: " + \
                a->m_name + ". SecRuleEngine is not On");
    }
}


bool Rule::evaluate(Transaction *trans) {
    bool globalRet = false;
    std::vector<Variable *> *variables = this->variables;
    bool recursiveGlobalRet;
    bool containsDisruptive = false;
    RuleMessage ruleMessage(this);
    std::vector<const collection::Variable *> finalVars;
    std::string eparam;

    trans->m_matched.clear();

    if (m_secmarker == true) {
        return true;
    }
    if (m_unconditional == true) {
        trans->debug(4, "(Rule: " + std::to_string(rule_id) \
            + ") Executing unconditional rule...");
        executeActionsIndependentOfChainedRuleResult(trans,
            &containsDisruptive, &ruleMessage);
        goto end_exec;
    }

    for (auto &i : trans->m_ruleRemoveById) {
        if (rule_id != i) {
            continue;
        }
        trans->debug(9, "Rule id: " + std::to_string(rule_id) +
            " was skipped due to an ruleRemoveById action...");
        return true;
    }

    eparam = MacroExpansion::expand(this->op->m_param, trans);

    if (this->op->m_param != eparam) {
        eparam = "\"" + eparam + "\" Was: \"" + this->op->m_param + "\"";
    } else {
        eparam = "\"" + eparam + "\"";
    }

    trans->debug(4, "(Rule: " + std::to_string(rule_id) \
        + ") Executing operator \"" + this->op->m_op \
        + "\" with param " \
        + eparam \
        + " against " \
        + Variable::to_s(variables) + ".");

    updateRulesVariable(trans);

    finalVars = getFinalVars(trans);

    for (const collection::Variable *v : finalVars) {
        std::string value = v->m_value;
        std::vector<std::string> values;
        bool multiMatch = getActionsByName("multimatch").size() > 0;

        values = executeSecDefaultActionTransofrmations(trans, value,
            multiMatch);

        for (const std::string &valueTemp : values) {
            bool ret;
            ret = executeOperatorAt(trans, v->m_key, valueTemp);
            if (ret == true) {
                ruleMessage.m_match = resolveMatchMessage(v->m_key, value);
                updateMatchedVars(trans, v->m_key, value);
                executeActionsIndependentOfChainedRuleResult(trans,
                    &containsDisruptive, &ruleMessage);
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

    if (this->chained == false) {
        goto end_exec;
    }

    if (this->chainedRule == NULL) {
        trans->debug(4, "Rule is marked as chained but there " \
            "isn't a subsequent rule.");
        goto end_clean;
    }

    trans->debug(4, "Executing chained rule.");
    recursiveGlobalRet = this->chainedRule->evaluate(trans);

    if (recursiveGlobalRet == true) {
        goto end_exec;
    }

end_clean:
    while (finalVars.empty() == false) {
        auto *a = finalVars.back();
        finalVars.pop_back();
        delete a;
    }

    return false;

end_exec:
    executeActionsAfterFullMatch(trans, containsDisruptive, &ruleMessage);
    for (const auto &u : ruleMessage.m_server_logs) {
        trans->serverLog(u);
    }

    while (finalVars.empty() == false) {
        auto *a = finalVars.back();
        finalVars.pop_back();
        delete a;
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
