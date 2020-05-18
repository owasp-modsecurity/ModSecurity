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

#ifndef SRC_ACTIONS_INIT_COL_H_
#define SRC_ACTIONS_INIT_COL_H_


namespace modsecurity {
class Transaction;
namespace actions {


class InitCol : public ActionWithRunTimeString {
 public:
    InitCol(
        const std::string &action,
        std::unique_ptr<RunTimeString> runTimeString
    ) : ActionWithRunTimeString(
            action,
            std::move(runTimeString)
        )
    { };

    InitCol(const InitCol &action)
        : ActionWithRunTimeString(action),
        m_collection_key(action.m_collection_key)
    { };

    bool init(std::string *error) override;

    bool execute(RuleWithActions *rule, Transaction *transaction) override;

    virtual ActionWithRunTimeString *clone() override {
        return new InitCol(*this);
    }

 private:
    std::string m_collection_key;
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_INIT_COL_H_
