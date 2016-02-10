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

#include "actions/ver.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {

Ver::Ver(std::string action)
    : Action(action, ConfigurationKind),
    m_ver(action) {
    if (m_ver.at(0) == '\'') {
        m_ver.erase(0, 1);
        m_ver.pop_back();
    }
}


bool Ver::evaluate(Rule *rule, Transaction *transaction) {
    rule->m_ver = m_ver;
    return true;
}

}  // namespace actions
}  // namespace modsecurity
