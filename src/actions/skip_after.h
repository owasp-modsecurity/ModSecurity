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


#ifndef SRC_ACTIONS_SKIP_AFTER_H_
#define SRC_ACTIONS_SKIP_AFTER_H_


namespace modsecurity {
namespace actions {


class SkipAfter : public Action {
 public:
    explicit SkipAfter(const std::string &action)
        : Action(action),
        m_skipName(std::make_shared<std::string>(m_parserPayload))
    { }

    bool execute(Transaction *transaction) noexcept override;

 private:
    // FIXME: This should be a regular pointer instead of a shared pointer.
    std::shared_ptr<std::string> m_skipName;
};


}  // namespace actions
}  // namespace modsecurity


#endif  // SRC_ACTIONS_SKIP_AFTER_H_
