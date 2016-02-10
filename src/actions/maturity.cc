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

#include "actions/maturity.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {

Maturity::Maturity(std::string action)
    : Action(action, ConfigurationKind),
    m_maturity_str(action) {
    if (m_maturity_str.at(0) == '\'') {
        m_maturity_str.erase(0, 1);
        m_maturity_str.pop_back();
    }
    m_maturity = std::stoi(m_maturity_str);
}


bool Maturity::evaluate(Rule *rule, Transaction *transaction) {
    rule->m_maturity = m_maturity;
    return true;
}

}  // namespace actions
}  // namespace modsecurity
