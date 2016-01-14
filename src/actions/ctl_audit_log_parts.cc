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

#include "actions/ctl_audit_log_parts.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"

namespace modsecurity {
namespace actions {

CtlAuditLogParts::CtlAuditLogParts(std::string action)
        : Action(action, RunTimeOnlyIfMatchKind),
        mPartsAction(0) {
    std::string what(action, 18, 1);
    mParts = std::string(action, 19, action.length()-19);
    if (what == "+") {
        mPartsAction = 0;
    } else {
        mPartsAction = 1;
    }
}

bool CtlAuditLogParts::evaluate(Rule *rule, Transaction *transaction) {
    transaction->m_auditLogModifier.push_back(
        std::make_pair(mPartsAction, mParts));
    return true;
}

}  // namespace actions
}  // namespace modsecurity
