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


#include "src/actions/skip.h"

#include <string>

#include "modsecurity/transaction.h"
/**
 * FIXME: rules_set.h inclusion is here due to ms_dbg_a.
 *        It should be removed.
 */
#include "modsecurity/rules_set.h"


namespace modsecurity {
namespace actions {


bool Skip::init(std::string *error) {
    try {
        m_skip_next = std::stoi(m_parserPayload);
    }  catch (...) {
        error->assign("Skip: The input \"" + m_parserPayload + "\" is " \
            "not a number.");
        return false;
    }
    return true;
}


bool Skip::execute(Transaction *transaction) const noexcept {
    ms_dbg_a(transaction, 5, "Skipping the next " + \
        std::to_string(m_skip_next) + " rules.");

    transaction->m_skip_next = m_skip_next;

    return true;
}


}  // namespace actions
}  // namespace modsecurity
