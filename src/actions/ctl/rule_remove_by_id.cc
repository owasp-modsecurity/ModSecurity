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

#include "actions/ctl/rule_remove_by_id.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"

namespace modsecurity {
namespace actions {
namespace ctl {


bool RuleRemoveById::init(std::string *error) {
    std::string what(m_parser_payload, 15, m_parser_payload.size() - 15);

    try {
        m_id = std::stoi(what);
    } catch(...) {
        error->assign("Not able to convert '" + what +
            "' into a number");
        return false;
    }

    return true;
}

bool RuleRemoveById::evaluate(Rule *rule, Transaction *transaction) {
    transaction->m_ruleRemoveById.push_back(m_id);
    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
