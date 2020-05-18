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
#include <utility>
#include <memory>

#include "modsecurity/actions/action.h"
#include "src/actions/action_with_run_time_string.h"

#ifndef SRC_ACTIONS_SET_ENV_H_
#define SRC_ACTIONS_SET_ENV_H_

class Transaction;

namespace modsecurity {
class Transaction;
namespace actions {


class SetENV : public ActionWithRunTimeString {
 public:
    explicit SetENV(std::unique_ptr<RunTimeString> runTimeString)
        : ActionWithRunTimeString(
            "setenv",
            RunTimeOnlyIfMatchKind,
            std::move(runTimeString)
        )
    { };

    explicit SetENV(const SetENV &action)
        : ActionWithRunTimeString(action)
    { };

    bool execute(RuleWithActions *rule, Transaction *transaction) override;

    virtual ActionWithRunTimeString *clone() override {
        return new SetENV(*this);
    }
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_SET_ENV_H_
