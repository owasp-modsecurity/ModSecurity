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

#include "src/actions/data/status.h"

#include <iostream>
#include <string>
#include <memory>
#include <charconv>

#include "modsecurity/transaction.h"


namespace modsecurity {
namespace actions {
namespace data {

bool Status::init(std::string *error) {
    const auto conv_res = std::from_chars(m_parser_payload.data(), m_parser_payload.data() + m_parser_payload.size(), m_status);
    if (conv_res.ec == std::errc::invalid_argument) {
        error->assign("Not a valid number: " + m_parser_payload);
        return false;
    }

    return true;
}


bool Status::evaluate(RuleWithActions *rule, Transaction *transaction,
    std::shared_ptr<RuleMessage> rm) {
    transaction->m_it.status = m_status;
    return true;
}


}  // namespace data
}  // namespace actions
}  // namespace modsecurity
