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

#include "actions/skip_after.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {

SkipAfter::SkipAfter(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind),
    m_marker(action) {
}


bool SkipAfter::evaluate(Rule *rule, Transaction *transaction) {
#ifndef NO_LOGS
    transaction->debug(5, "Setting skipAfter for: " + m_marker);
#endif
    transaction->m_marker = m_marker;
    return true;
}

}  // namespace actions
}  // namespace modsecurity
