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

#include "src/actions/action_type_rule_metadata.h"


#ifndef SRC_ACTIONS_VER_H_
#define SRC_ACTIONS_VER_H_


namespace modsecurity {
namespace actions {


class Ver : public ActionTypeRuleMetaData {
 public:
    explicit Ver(const std::string &action)
        : Action(action),
        m_version("")
    { };

    void configure(RuleWithActions *rule) override {
        rule->setVersion(m_version);
    }

 private:
    std::string m_version;
};


}  // namespace actions
}  // namespace modsecurity

#endif  // SRC_ACTIONS_VER_H_
