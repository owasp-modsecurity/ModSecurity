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

#include "actions/rule_id.h"

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "src/rule.h"

namespace ModSecurity {
namespace actions {

RuleId::RuleId(std::string action)
    : Action(action) {
    this->action_kind = ConfigurationKind;
    std::string a = action;
    a.erase(0, 3);
    if (a.at(0) == '\'') {
        a.erase(0, 1);
        a.pop_back();
    }
    this->rule_id = std::stod(a);
    }


bool RuleId::evaluate(Rule *rule) {
    rule->rule_id = this->rule_id;
    return true;
}

}  // namespace actions
}  // namespace ModSecurity
