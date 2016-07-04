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

#include "actions/msg.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "src/macro_expansion.h"
#include "modsecurity/rule.h"

/*
 * Description: Assigns a custom message to the rule or chain in which it
 * appears. The message will be logged along with every alert.
 *
 * Action Group: Meta-data
 *
 * Example:
 * SecRule &REQUEST_HEADERS:Host "@eq 0" "log,id:60008,severity:2,msg:'Request Missing a Host Header'"
 *
 * Note  : The msg information appears in the error and/or audit log files
 *         and is not sent back to the client in response headers.
 *
 * Note 2: The msg action can appear multiple times in the SecRule, however
 *         just the last one will be take into consideration.
 *
 */


namespace modsecurity {
namespace actions {


bool Msg::evaluate(Rule *rule, Transaction *transaction) {
    std::string msg = MacroExpansion::expand(m_parser_payload, transaction);

#ifndef NO_LOGS
    transaction->debug(9, "Saving msg: " + msg);
#endif

    rule->m_log_message = data(transaction);

    return true;
}


std::string Msg::data(Transaction *transaction) {
    return MacroExpansion::expand(m_parser_payload, transaction);
}


}  // namespace actions
}  // namespace modsecurity
