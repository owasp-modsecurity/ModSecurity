
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
#include <vector>

#include "modsecurity/rules_set.h"
#include "modsecurity/rule.h"
#include "src/engine/lua.h"
#include "src/operators/operator.h"
#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "src/actions/transformations/none.h"
#include "src/actions/tag.h"
#include "src/utils/string.h"
#include "modsecurity/rule_message.h"
#include "src/actions/msg.h"
#include "src/actions/log_data.h"
#include "src/actions/severity.h"
#include "src/variables/variable.h"
#include "src/rule_with_actions.h"


#ifndef SRC_RULE_SCRIPT_H_
#define SRC_RULE_SCRIPT_H_


namespace modsecurity {

using actions::Action;

/** @ingroup ModSecurity_CPP_API */
class RuleScript : public RuleWithActions {
 public:
    RuleScript(const std::string &name,
        std::vector<Action *> *actions,
        Transformations *t,
        std::unique_ptr<std::string> fileName,
        int lineNumber)
            : RuleWithActions(actions, t, std::move(fileName), lineNumber),
        m_name(name),
        m_lua(std::unique_ptr<engine::Lua>(new engine::Lua())) { }

    RuleScript(const RuleScript &rs)
        : RuleWithActions(rs),
        m_name(rs.m_name),
        m_lua(rs.m_lua) { }

    RuleScript &operator=(const RuleScript& r) {
        RuleWithActions::operator = (r);
        m_name = r.m_name;
        m_lua = r.m_lua;
        return *this;
    }

    bool init(std::string *err);
    bool evaluate(Transaction *trans) override;

    std::string m_name;
    std::shared_ptr<engine::Lua> m_lua;
};

}  // namespace modsecurity

#endif  // SRC_RULE_SCRIPT_H_

