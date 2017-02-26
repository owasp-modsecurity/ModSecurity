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

#include "src/actions/log_data.h"

#include <iostream>
#include <string>
#include <memory>

#include "modsecurity/actions/action.h"
#include "modsecurity/transaction.h"
#include "src/macro_expansion.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"


namespace modsecurity {
namespace actions {


bool LogData::evaluate(Rule *rule, Transaction *transaction,
    std::shared_ptr<RuleMessage> rm) {
    rm->m_data = data(transaction);

    transaction->m_collections.storeOrUpdateFirst("RULE:logdata", rm->m_data);
    return true;
}

std::string LogData::data(Transaction *transaction) {
    return MacroExpansion::expand(m_parser_payload, transaction);
}


}  // namespace actions
}  // namespace modsecurity
