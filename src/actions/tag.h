/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <optional>
#include <utility>

#include "modsecurity/actions/action.h"
#include "src/run_time_string.h"

#ifndef SRC_ACTIONS_TAG_H_
#define SRC_ACTIONS_TAG_H_

class Transaction;

namespace modsecurity {
class Transaction;
namespace actions {


class Tag : public Action {
 public:
    explicit Tag(std::unique_ptr<RunTimeString> z)
        : Action("tag"),
        m_string(std::move(z)),
        m_staticName(m_string->containsMacro()
            ? std::nullopt
            : std::optional<std::string>(m_string->evaluate())) { }

    std::string getName(Transaction *transaction);
    bool hasName(const std::string &name, Transaction *transaction);

    bool evaluate(RuleWithActions *rule, Transaction *transaction, RuleMessage &ruleMessage) override;

 protected:
    std::unique_ptr<RunTimeString> m_string;

 private:
    // Without macros, the name is the same in every transaction and is
    // evaluated once. Empty if the tag contains macros.
    const std::optional<std::string> m_staticName;
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_TAG_H_
