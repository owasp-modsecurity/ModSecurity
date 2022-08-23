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

#include "src/actions/maturity.h"

#include <iostream>
#include <string>
#include <charconv>

#include "modsecurity/actions/action.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"


namespace modsecurity {
namespace actions {


bool Maturity::init(std::string *error) {

    const auto conv_res = std::from_chars(m_parser_payload.data(), m_parser_payload.data() + m_parser_payload.size(), m_maturity);
    if (conv_res.ec == std::errc::invalid_argument) {
        // Conversion error
        error->assign("Maturity: The input \"" + m_parser_payload + "\" is " \
            "not a number.");
        return false;
    }
    return true;
}


bool Maturity::evaluate(RuleWithActions *rule, Transaction *transaction) {
    rule->m_maturity = m_maturity;
    return true;
}


}  // namespace actions
}  // namespace modsecurity
