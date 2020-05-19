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

#include <utility>
#include <memory>
#include <string>

#include "modsecurity/actions/action.h"
#include "src/run_time_string.h"

#ifndef SRC_ACTIONS_ACTION_WITH_RUN_TIME_STRING_H_
#define SRC_ACTIONS_ACTION_WITH_RUN_TIME_STRING_H_


namespace modsecurity {
namespace actions {


class ActionWithRunTimeString : public virtual Action {
 public:
    explicit ActionWithRunTimeString(std::unique_ptr<RunTimeString> string = nullptr)
        : m_string(std::move(string))
    { }

    ActionWithRunTimeString(const ActionWithRunTimeString &a)
        : m_string(a.m_string?std::unique_ptr<RunTimeString>(new RunTimeString(*a.m_string.get())):nullptr)
    { }

    ActionWithRunTimeString& operator=(const ActionWithRunTimeString& a) {
        m_string = std::unique_ptr<RunTimeString>(new RunTimeString(*a.m_string.get()));
        return *this;
    }

    virtual void populate(RuleWithActions *rule) {
        if (m_string) {
            m_string->populate(rule);
        }
    }

    std::string getEvaluatedRunTimeString(Transaction *transaction) const noexcept {
        return (m_string == nullptr)?"":m_string->evaluate(transaction);
    }

    bool hasRunTimeString() const noexcept {
        return m_string != nullptr;
    }

    virtual ActionWithRunTimeString* clone() = 0;

 private:
    std::unique_ptr<RunTimeString> m_string;
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_ACTION_WITH_RUN_TIME_STRING_H_
