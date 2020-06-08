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


#include "src/actions/ctl/request_body_access.h"

#include <string>

#include "modsecurity/rules_set_properties.h"
#include "modsecurity/transaction.h"


namespace modsecurity {
namespace actions {
namespace ctl {


bool RequestBodyAccess::init(std::string *error) {
    std::string what(m_parserPayload, 18, m_parserPayload.size() - 18);

    if (what == "true") {
        m_requestBodyAccess = true;
    } else if (what == "false") {
        m_requestBodyAccess = false;
    } else {
        error->assign("Internal error. Expected: true or false, got: " \
            + m_parserPayload);
        return false;
    }

    return true;
}


bool RequestBodyAccess::execute(Transaction *transaction) const noexcept {
    if (m_requestBodyAccess) {
        transaction->m_requestBodyAccess =
            RulesSetProperties::TrueConfigBoolean;
    } else {
        transaction->m_requestBodyAccess =
            RulesSetProperties::FalseConfigBoolean;
    }

    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
