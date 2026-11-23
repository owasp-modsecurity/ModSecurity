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
#include <string_view>
#include <memory>
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
        m_isStatic(!m_string->containsMacro()),
        m_staticName(m_isStatic ? m_string->evaluate(nullptr) : "") { }

    std::string getName(Transaction *transaction);
    bool hasName(std::string_view name, Transaction *transaction);

    bool evaluate(RuleWithActions *rule, Transaction *transaction, RuleMessage &ruleMessage) override;

 protected:
    std::unique_ptr<RunTimeString> m_string;

 private:
    // A tag without macro has the same name in every transaction: keep it,
    // instead of building a new string each time a rule checks its tags.
    bool m_isStatic;
    std::string m_staticName;
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_TAG_H_
