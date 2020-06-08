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

#include "modsecurity/actions/action.h"
#include "modsecurity/transaction.h"
#include "src/actions/action_with_execution.h"


#ifndef SRC_ACTIONS_CTL_RULE_REMOVE_TARGET_BY_TAG_H_
#define SRC_ACTIONS_CTL_RULE_REMOVE_TARGET_BY_TAG_H_


namespace modsecurity {
namespace actions {
namespace ctl {


class RuleRemoveTargetByTag : public ActionWithExecution {
 public:
    explicit RuleRemoveTargetByTag(const std::string &action)
        : Action(action)
    { }

    bool init(std::string *error) override;

    bool execute(Transaction *transaction) const noexcept override;

 private:
    std::string m_tag;
    std::string m_target;
};


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity


#endif  // SRC_ACTIONS_CTL_RULE_REMOVE_TARGET_BY_TAG_H_
