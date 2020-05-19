/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include <memory>
#include <string>
#include <utility>

#include "modsecurity/actions/action.h"
#include "modsecurity/transaction.h"
#include "src/actions/action_with_run_time_string.h"
#include "src/variables/variable_with_runtime_string.h"
#include "src/rule_with_operator.h"


#ifndef SRC_ACTIONS_SET_VAR_H_
#define SRC_ACTIONS_SET_VAR_H_


namespace modsecurity {
namespace actions {


enum SetVarOperation {
    /* Set variable to something */
    setOperation,
    /* read variable, sum predicate and set */
    sumAndSetOperation,
    /* read variable, substract predicate and set */
    substractAndSetOperation,
    /* set variable to 1 */
    setToOneOperation,
    /* unset operation */
    unsetOperation,
};


class SetVar : public ActionWithRunTimeString {
 public:
    SetVar(SetVarOperation operation,
        std::unique_ptr<modsecurity::variables::Variable> variable,
        std::unique_ptr<RunTimeString> predicate)
        : ActionWithRunTimeString(std::move(predicate)),
        m_operation(operation),
        m_variable(std::move(variable)),
        Action("setvar")
    { }


    SetVar(SetVarOperation operation,
        std::unique_ptr<modsecurity::variables::Variable> variable)
        : ActionWithRunTimeString(),
        Action("setvar"),
        m_operation(operation),
        m_variable(std::move(variable))
    { }


    SetVar(const SetVar &var)
        : ActionWithRunTimeString(var),
        Action(var),
        m_operation(var.m_operation),
        m_variable(var.m_variable) {
        variables::RuleVariable *rv = dynamic_cast<variables::RuleVariable *>(
            m_variable.get());
        if (rv != nullptr) {
            auto nrv = rv->clone();
            rv = dynamic_cast<variables::RuleVariable *>(nrv);
            rv->populate(nullptr);
            m_variable = std::unique_ptr<variables::Variable>(nrv);
        }
    }


    bool execute(Transaction *transaction) noexcept override;
    bool init(std::string *error) override;

    void populate(RuleWithActions *rule) override {
        ActionWithRunTimeString::populate(rule);
        variables::RuleVariable *rulev =
            dynamic_cast<variables::RuleVariable *>(
                m_variable.get());

        if (rulev != nullptr) {
            rulev->populate(rule);
        }
        variables::VariableWithRunTimeString *rulev2 =
            dynamic_cast<variables::VariableWithRunTimeString *>(
                m_variable.get());

        if (rulev2 != nullptr) {
            rulev2->populate(rule);
        }
    }

    ActionWithRunTimeString *clone() override {
        return new SetVar(*this);
    }

 private:
    SetVarOperation m_operation;
    std::shared_ptr<modsecurity::variables::Variable> m_variable;
};


}  // namespace actions
}  // namespace modsecurity


#endif  // SRC_ACTIONS_SET_VAR_H_
