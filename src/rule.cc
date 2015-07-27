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

#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <utility>

#include "operators/operator.h"
#include "actions/action.h"
#include "../headers/modsecurity/modsecurity.h"


namespace ModSecurity {

using operators::Operator;
using actions::Action;
using Variables::Variable;

Rule::~Rule() {
    delete op;
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
    while (variables->empty() == false) {
        auto *a = variables->back();
        variables->pop_back();
        delete a;
    }

    delete variables;
}


Rule::Rule(Operator *_op,
        std::vector<Variable *> *_variables,
        std::vector<Action *> *actions)
    : chained(false),
    chainedRule(NULL),
    variables(_variables),
    op(_op),
    rule_id(0),
    phase(-1),
    m_referenceCount(0) {
    for (Action *a : *actions) {
        if (a->action_kind == Action::ConfigurationKind) {
            actions_conf.push_back(a);
            a->evaluate(this);
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

    delete actions;
}

bool Rule::evaluate(Assay *assay) {
    bool ret = false;
    std::vector<Variable *> *variables = this->variables;

    assay->debug(4, "Executing operator \"" + this->op->op \
        + "\" with param \"" + this->op->param +  "\" against " \
        + Variable::to_s(variables) + ".");

    clock_t begin = clock();

    for (int i = 0; i < variables->size(); i++) {
        int transformations = 0;
        Variable *variable = variables->at(i);

        std::list<std::pair<std::string, std::string>> e =
            variable->evaluate(assay);

        for (auto &v : e) {
            std::string value = v.second;
            for (Action *a :
                this->actions_runtime_pre) {
                value = a->evaluate(value, assay);
                assay->debug(9, " T (" + \
                        std::to_string(transformations) + ") " + \
                        a->name + ": \"" + value +"\"");
                transformations++;
            }

            assay->debug(9, "Target value: \"" + value + "\" (Variable: " + \
                v.first + ")");

            ret = this->op->evaluate(assay, value);

            clock_t end = clock();
            double elapsed_secs = static_cast<double>(end - begin) \
                / CLOCKS_PER_SEC;

            assay->debug(4, "Operator completed in " + \
                std::to_string(elapsed_secs) + " seconds");

            if (ret) {
                assay->debug(4, "Rule returned 1.");

                for (Action *a :
                    this->actions_runtime_pos) {
                    assay->debug(4, "Running action: " + a->action);
                    a->evaluate(assay);
                }
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
                    this->chainedRule->evaluate(assay);
                    assay->update_variable_first("MATCHED_VAR", "");
                    assay->delete_variable("MATCHED_VARS:" + v.first);
                    assay->delete_variable("MATCHED_VARS_NAMES:" + v.first);
                    assay->delete_variable("MATCHED_VARS_NAMES:" + v.first);
                }
            } else {
                assay->debug(4, "Rule returned 0.");
            }
        }
    }
    return ret;
}

}  // namespace ModSecurity
