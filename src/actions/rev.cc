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

#include "actions/rev.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/assay.h"
#include "src/utils.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {

Rev::Rev(std::string action)
    : Action(action, ConfigurationKind),
    m_rev(action) {
    m_rev.erase(0, 1);
    m_rev.pop_back();
}


bool Rev::evaluate(Rule *rule, Assay *assay) {
    rule->rev = m_rev;
    return true;
}

}  // namespace actions
}  // namespace modsecurity
