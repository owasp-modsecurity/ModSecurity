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

#include "src/actions/skip.h"

#include <iostream>
#include <string>
#include <charconv>

#include "modsecurity/rules_set.h"
#include "modsecurity/actions/action.h"
#include "modsecurity/transaction.h"

namespace modsecurity {
namespace actions {


bool Skip::init(std::string *error) {
    const auto conv_res = std::from_chars(m_parser_payload.data(), m_parser_payload.data() + m_parser_payload.size(), m_skip_next);
    if (conv_res.ec == std::errc::invalid_argument) {
        error->assign("Skip: The input \"" + m_parser_payload + "\" is " \
            "not a number.");
        return false;
    }
    return true;
}


bool Skip::evaluate(RuleWithActions *rule, Transaction *transaction) {
    ms_dbg_a(transaction, 5, "Skipping the next " + \
        std::to_string(m_skip_next) + " rules.");

    transaction->m_skip_next = m_skip_next;

    return true;
}


}  // namespace actions
}  // namespace modsecurity
