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


#include "src/actions/disruptive/redirect.h"

#include <string>

#include "modsecurity/transaction.h"
/**
 * FIXME: rules_set.h inclusion is here due to ms_dbg_a.
 *        It should be removed.
 */
#include "modsecurity/rules_set.h"
#include "modsecurity/rule_message.h"


namespace modsecurity {
namespace actions {
namespace disruptive {


bool Redirect::execute(Transaction *transaction) noexcept {
    std::string m_urlExpanded(getEvaluatedRunTimeString(transaction));
    /* if it was changed before, lets keep it. */
    if (transaction->m_it.status == 200
        || (!(transaction->m_it.status <= 307
            && transaction->m_it.status >= 301))) {
        transaction->m_it.status = m_status;
    }

    intervention::freeUrl(&transaction->m_it);
    transaction->m_it.url = strdup(m_urlExpanded.c_str());
    transaction->m_it.disruptive = true;
    intervention::freeLog(&transaction->m_it);

    transaction->m_it.log = strdup(
        transaction->messageGetLast()->log(
            RuleMessage::LogMessageInfo::ClientLogMessageInfo)
                .c_str());

    return true;
}


}  // namespace disruptive
}  // namespace actions
}  // namespace modsecurity
