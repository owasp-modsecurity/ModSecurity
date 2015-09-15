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

#include "src/rule.h"

#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <utility>

#include "operators/operator.h"
#include "actions/action.h"
#include "modsecurity/modsecurity.h"
#include "actions/transformations/none.h"
#include "variables/variations/exclusion.h"
#include "src/utils.h"

using ModSecurity::Variables::Variations::Exclusion;

namespace ModSecurity {

using operators::Operator;
using actions::Action;
using Variables::Variable;
using actions::transformations::None;

Rule::~Rule() {
    if (op != NULL) {
        delete op;
    }
    while (actions_conf.empty() == false) {
        auto *a = actions_conf.back();
        actions_conf.pop_back();
        delete a;
    }
    while (actions_runtime_pre.empty() == false) {
        auto *a = actions_runtime_pre.back();
        actions_runtime_pre.pop_back();
        delete a;
    }
    while (actions_runtime_pos.empty() == false) {
        auto *a = actions_runtime_pos.back();
        actions_runtime_pos.pop_back();
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
}

Rule::Rule(std::string marker)
    : chained(false),
    chainedRule(NULL),
    variables(NULL),
    op(NULL),
    rule_id(0),
    phase(-1),
    m_unconditional(false),
    m_secmarker(true),
    m_marker(marker),
    m_referenceCount(0) { };

Rule::Rule(Operator *_op,
        std::vector<Variable *> *_variables,
        std::vector<Action *> *actions)
    : chained(false),
    chainedRule(NULL),
    variables(_variables),
    op(_op),
    rule_id(0),
    phase(-1),
    m_unconditional(false),
    m_secmarker(false),
    m_marker(""),
    m_referenceCount(0) {
    for (Action *a : *actions) {
        if (a->action_kind == Action::ConfigurationKind) {
            actions_conf.push_back(a);
            a->evaluate(this, NULL);
        } else if (a->action_kind == Action::RunTimeBeforeMatchAttemptKind) {
            actions_runtime_pre.push_back(a);
        } else if (a->action_kind == Action::RunTimeOnlyIfMatchKind) {
            actions_runtime_pos.push_back(a);
        } else {
            std::cout << "General failure, action: " << a->name;
            std::cout << " has an unknown type." << std::endl;
            delete a;
        }
    }
    /**
     * If phase is not entered, we assume phase 2. For historical reasons.
     *
     */
    if (phase == -1) {
        phase = ModSecurity::Phases::RequestHeadersPhase;
    }

    if (op == NULL) {
        m_unconditional = true;
    }

    delete actions;
}


bool Rule::evaluateActions(Assay *assay) {
    int none = 0;
    int transformations = 0;
    for (Action *a : this->actions_runtime_pre) {
        None *z = dynamic_cast<None *>(a);
        if (z != NULL) {
            none++;
        }
    }

    assay->debug(4, "Running unconditional rule.");

    if (none == 0) {
        /*
        for (Action *a : assay->m_rules->defaultActions[this->phase]) {
            if (a->action_kind == actions::Action::RunTimeBeforeMatchAttemptKind) {
                value = a->evaluate(value, assay);
                assay->debug(9, "(SecDefaultAction) T (" + \
                    std::to_string(transformations) + ") " + \
                    a->name + ": \"" + value +"\"");
                    transformations++;
            }
        }
        */
    }

    for (Action *a : this->actions_runtime_pre) {
        None *z = dynamic_cast<None *>(a);
        /*
        if (none == 0) {
            value = a->evaluate(value, assay);
            assay->debug(9, " T (" + \
                std::to_string(transformations) + ") " + \
                a->name + ": \"" + value +"\"");
                transformations++;
        }
        */
        if (z != NULL) {
            none--;
        }
    }

    for (Action *a : assay->m_rules->defaultActions[this->phase]) {
        if (a->action_kind == actions::Action::RunTimeOnlyIfMatchKind) {
            assay->debug(4, "(SecDefaultAction) Running action: " + a->action);
            a->evaluate(this, assay);
        }
    }

    for (Action *a :
        this->actions_runtime_pos) {
        assay->debug(4, "Running action: " + a->action);
        a->evaluate(this, assay);
    }

    return true;
}


bool Rule::evaluate(Assay *assay) {
    bool ret = false;
    std::vector<Variable *> *variables = this->variables;

    if (m_secmarker == true) {
        return true;
    }
    if (m_unconditional == true) {
        return evaluateActions(assay);
    }

    assay->debug(4, "Executing operator \"" + this->op->op \
        + "\" with param \"" + this->op->param +  "\" against " \
        + Variable::to_s(variables) + ".");

    clock_t begin = clock();

    std::list<std::string> exclusions;
    for (int i = 0; i < variables->size(); i++) {
        Variable *variable = variables->at(i);
        Exclusion *exl = dynamic_cast<Exclusion *>(variable);

        if (exl != NULL) {
            std::list<std::pair<std::string, std::string>> z =
                variable->evaluate(assay);
            for (auto &y : z) {
                exclusions.push_back(y.first);
            }
            exclusions.push_back(variable->name);
        }
    }

    for (int i = 0; i < variables->size(); i++) {
        int transformations = 0;
        Variable *variable = variables->at(i);
        Exclusion *exl = dynamic_cast<Exclusion *>(variable);
        if (exl != NULL) {
            continue;
        }

        std::list<std::pair<std::string, std::string>> e =
            variable->evaluate(assay);

        for (auto &v : e) {
            if (std::find(exclusions.begin(), exclusions.end(),
                v.first) != exclusions.end()) {
                assay->debug(9, "Variable: " + v.first + " is part of the" +
                    " exclusion list, skipping...");
                continue;
            }
            std::string value = v.second;
            int none = 0;
            for (Action *a : this->actions_runtime_pre) {
                None *z = dynamic_cast<None *>(a);
                if (z != NULL) {
                    none++;
                }
            }

            // Check for transformations on the SecDefaultAction
            // Notice that first we make sure that won't be a t:none
            // on the target rule.
            if (none == 0) {
                for (Action *a : assay->m_rules->defaultActions[this->phase]) {
                    if (a->action_kind == actions::Action::RunTimeBeforeMatchAttemptKind) {
                        value = a->evaluate(value, assay);
                        assay->debug(9, "(SecDefaultAction) T (" + \
                            std::to_string(transformations) + ") " + \
                            a->name + ": \"" + value +"\"");
                        transformations++;
                    }
                }
            }

            for (Action *a : this->actions_runtime_pre) {
                None *z = dynamic_cast<None *>(a);
                if (none == 0) {
                    value = a->evaluate(value, assay);
                    assay->debug(9, " T (" + \
                            std::to_string(transformations) + ") " + \
                            a->name + ": \"" + value +"\"");
                    transformations++;
                }
                if (z != NULL) {
                    none--;
                }
            }

            assay->debug(9, "Target value: \"" + limitTo(80, toHexIfNeeded(value)) + \
                "\" (Variable: " + v.first + ")");

            ret = this->op->evaluate(assay, value);

            clock_t end = clock();
            double elapsed_secs = static_cast<double>(end - begin) \
                / CLOCKS_PER_SEC;

            assay->debug(4, "Operator completed in " + \
                std::to_string(elapsed_secs) + " seconds");

            if (ret) {
                bool chainResult = false;
                assay->debug(4, "Rule returned 1.");

                if (this->chained && this->chainedRule == NULL) {
                    assay->debug(4, "Rule is marked as chained but there " \
                        "isn't a subsequent rule.");
                }
                if (this->chained && this->chainedRule != NULL) {
                    assay->debug(4, "Executing chained rule.");
                    if (assay->update_variable_first("MATCHED_VAR",
                        value) == false) {
                        assay->store_variable("MATCHED_VAR", value);
                    }
                    if (assay->update_variable_first("MATCHED_VAR_NAME",
                        v.first) == false) {
                        assay->store_variable("MATCHED_VAR_NAME", v.first);
                    }
                    assay->store_variable("MATCHED_VARS:" + v.first, value);
                    assay->store_variable("MATCHED_VARS_NAMES:" + v.first,
                        v.first);
                    chainResult = this->chainedRule->evaluate(assay);
                    assay->update_variable_first("MATCHED_VAR", "");
                    assay->delete_variable("MATCHED_VARS:" + v.first);
                    assay->delete_variable("MATCHED_VARS_NAMES:" + v.first);
                    assay->delete_variable("MATCHED_VARS_NAMES:" + v.first);
                }
                if (this->chained && chainResult == true || !this->chained) {
                    for (Action *a : assay->m_rules->defaultActions[this->phase]) {
                        if (a->action_kind == actions::Action::RunTimeOnlyIfMatchKind) {
                            assay->debug(4, "(SecDefaultAction) Running action: " + a->action);
                            a->evaluate(this, assay);
                        }
                    }
                    for (Action *a :
                        this->actions_runtime_pos) {
                        assay->debug(4, "Running action: " + a->action);
                        a->evaluate(this, assay);
                    }
                }

            } else {
                assay->debug(4, "Rule returned 0.");
            }
        }
    }
    return ret;
}

}  // namespace ModSecurity
