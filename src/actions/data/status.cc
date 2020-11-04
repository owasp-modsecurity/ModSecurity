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


#include "src/actions/data/status.h"

#include <string>

#include "modsecurity/transaction.h"


namespace modsecurity {
namespace actions {
namespace data {


bool Status::init(std::string *error) {
    try {
        m_status = std::stoi(m_parserPayload);
    } catch (...) {
        error->assign("Not a valid number: " + m_parserPayload);
        return false;
    }

    return true;
}


bool Status::execute(Transaction *transaction) noexcept {
    transaction->m_it.status = m_status;
    return true;
}


}  // namespace data
}  // namespace actions
}  // namespace modsecurity
