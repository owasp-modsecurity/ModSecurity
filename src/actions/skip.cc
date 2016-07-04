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

#include "actions/skip.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {


bool Skip::init(std::string *error) {
    try {
        m_skip_next = std::stoi(m_parser_payload);
    }  catch (...) {
        error->assign("Skip: The input \"" + m_parser_payload + "\" is " \
            "not a number.");
        return false;
    }
    return true;
}


bool Skip::evaluate(Rule *rule, Transaction *transaction) {
#ifndef NO_LOGS
    transaction->debug(5, "Skipping the next " + std::to_string(m_skip_next) \
        + " rules.");
#endif
    transaction->m_skip_next = m_skip_next;

    return true;
}


}  // namespace actions
}  // namespace modsecurity
