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
#include <memory>

#include "modsecurity/actions/action.h"

#ifndef SRC_ACTIONS_NO_AUDIT_LOG_H_
#define SRC_ACTIONS_NO_AUDIT_LOG_H_

#ifdef __cplusplus
class Transaction;

namespace modsecurity {
class Transaction;

namespace actions {


class NoAuditLog : public Action {
 public:
    explicit NoAuditLog(const std::string &action) 
        : Action(action, RunTimeOnlyIfMatchKind) { }

    bool evaluate(RuleWithActions *rule, Transaction *transaction,
        RuleMessage &rm) override;
};

}  // namespace actions
}  // namespace modsecurity
#endif

#endif  // SRC_ACTIONS_NO_AUDIT_LOG_H_
