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


#include "src/actions/ctl/audit_log_parts.h"

#include <string>
#include <utility>

#include "modsecurity/transaction.h"
#include "modsecurity/audit_log.h"
/**
 * FIXME: rules_set.h inclusion is here due to ms_dbg_a.
 *        It should be removed.
 */
#include "modsecurity/rules_set.h"


namespace modsecurity {
namespace actions {
namespace ctl {


bool AuditLogParts::init(std::string *error) {
    std::string what(m_parserPayload, 14, 1);
    std::string parts_str(m_parserPayload, 15, m_parserPayload.length()-15);

    if ((what != "-") && (what != "+")) {
        error->assign("ctl:auditLogParts modificators expects add or " \
            "remove (+/-) in front of the modificator. Got: " + what);
        return false;
    }

    int flags = AuditLog::addParts(0, parts_str);

    if (what == "+") {
        m_partsToModify = flags;
    } else {
        m_partsToModify = -1 * flags;
    }

    return true;
}


bool AuditLogParts::execute(Transaction *transaction) noexcept {
    ms_dbg_a(transaction, 7, "AuditLog parts before modification: " +
        std::to_string(transaction->m_auditLogParts) + ".");

    if (m_partsToModify < 0) {
        transaction->m_auditLogParts = \
            transaction->m_auditLogParts & ~(m_partsToModify * -1);
    } else {
        transaction->m_auditLogParts = \
            transaction->m_auditLogParts | m_partsToModify;
    }

    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
