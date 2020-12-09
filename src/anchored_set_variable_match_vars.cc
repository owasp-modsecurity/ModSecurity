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

#include <string>
#include <vector>

#include "modsecurity/anchored_set_variable_match_vars.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/regex.h"
#include "src/variables/variable.h"


namespace modsecurity {


void AnchoredVariableMatchVars::resolve(VariableValues *l,
        const variables::KeyExclusions &ke) const noexcept
{
    for (const auto& x : m_vvs) {
        if (ke.toOmit(x->getName())) {
            ms_dbg_a(m_transaction, 7, "Excluding key: " + x->getName()
                + " from target value.");
            continue;
        }
        const VariableValue *var = new VariableValue(
            &m_name,
            std::unique_ptr<std::string>(new std::string(x->getName())),
            std::unique_ptr<std::string>(new std::string(x->getValue()))
        );
        l->insert(l->begin(), std::unique_ptr<const VariableValue>(var));
    }
}


void AnchoredVariableMatchVars::resolve(const std::string &key,
        VariableValues *l) const noexcept
{
    for (const auto& x : m_vvs) {
        if (key != x->getName()) {
            continue;
        }
        const VariableValue *var = new VariableValue(
            &m_name,
            std::unique_ptr<std::string>(new std::string(x->getName())),
            std::unique_ptr<std::string>(new std::string(x->getValue()))
        );
        l->insert(l->begin(), std::unique_ptr<const VariableValue>(var));
    }
}


void AnchoredVariableMatchVars::resolveRegularExpression(const Utils::Regex *r,
        VariableValues *l,
        const variables::KeyExclusions &ke) const noexcept
{
    for (const auto& x : m_vvs) {
        int ret = Utils::regex_search(x->getName(), *r);
        if (ret <= 0) {
            continue;
        }
        if (ke.toOmit(x->getName())) {
            ms_dbg_a(m_transaction, 7, "Excluding key: " + x->getName()
                + " from target value.");
            continue;
        }
        const VariableValue *var = new VariableValue(
            &m_name,
            std::unique_ptr<std::string>(new std::string(x->getName())),
            std::unique_ptr<std::string>(new std::string(x->getValue()))
        );
        l->insert(l->begin(), std::unique_ptr<const VariableValue>(var));
    }
}


std::unique_ptr<std::string> AnchoredVariableMatchVars::resolveFirst(const std::string &key) const noexcept
{
    for (const auto& x : m_vvs) {
        if (key != x->getName()) {
            continue;
        }
        return std::unique_ptr<std::string>(new std::string(x->getValue()));
    }
    return std::unique_ptr<std::string>();
}


}  // namespace modsecurity
