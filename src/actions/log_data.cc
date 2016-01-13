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

#include "actions/log_data.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "src/macro_expansion.h"

namespace modsecurity {
namespace actions {

LogData::LogData(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind),
    m_data(action) {
    m_data.erase(0, 1);
    m_data.pop_back();
}


bool LogData::evaluate(Rule *rule, Transaction *transaction) {
    std::string msg = MacroExpansion::expand(m_data, transaction);
#ifndef NO_LOGS
    transaction->debug(9, "Saving msg: " + msg);
#endif
    transaction->rulesMessages.push_back(msg);
    transaction->serverLog(msg);
    return true;
}

}  // namespace actions
}  // namespace modsecurity
