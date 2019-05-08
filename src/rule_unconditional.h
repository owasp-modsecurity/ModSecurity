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

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <utility>
#endif

#ifndef SRC_RULE_UNCONDITIONAL_H_
#define SRC_RULE_UNCONDITIONAL_H_

#include "modsecurity/modsecurity.h"
#include "modsecurity/variable_value.h"
#include "modsecurity/rule.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/actions/action.h"
#include "src/rule_with_actions.h"

#ifdef __cplusplus

namespace modsecurity {


class RuleUnconditional : public RuleWithActions {
 public:
    RuleUnconditional(
        std::vector<actions::Action *> *actions,
        Transformations *transformations,
        std::unique_ptr<std::string> fileName,
        int lineNumber)
        : RuleWithActions(actions, transformations, std::move(fileName), lineNumber) { }

    RuleUnconditional(const RuleUnconditional &r)
        : RuleWithActions(r)
        { }

    virtual bool evaluate(Transaction *transaction) const override;

 private:
};


}  // namespace modsecurity

#endif

#endif  // SRC_RULE_UNCONDITIONAL_H_
