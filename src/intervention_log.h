/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2026 OWASP ModSecurity Project
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing, please contact OWASP directly using
 * the email address modsecurity@owasp.org.
 *
 */

#ifndef SRC_INTERVENTION_LOG_H_
#define SRC_INTERVENTION_LOG_H_

#include <string.h>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/transaction.h"

namespace modsecurity {

namespace intervention {

class LogPayloadAccess {
 public:
    static bool isEnabled(const ModSecurity *modsecurity) {
        return (modsecurity->m_logProperties
            & ModSecurity::InterventionLogPayloadDisabledMask) == 0;
    }
};


static inline void setLogPayload(Transaction *transaction,
    const RuleMessage &message) {
    freeLog(&transaction->m_it);
    if (!LogPayloadAccess::isEnabled(transaction->m_ms)) {
        return;
    }

    transaction->m_it.log = strdup(
        message.log(RuleMessage::LogMessageInfo::ClientLogMessageInfo).c_str());
}


static inline void setLogPayload(Transaction *transaction,
    const char *message) {
    freeLog(&transaction->m_it);
    if (!LogPayloadAccess::isEnabled(transaction->m_ms)) {
        return;
    }

    transaction->m_it.log = strdup(message);
}

}  // namespace intervention
}  // namespace modsecurity

#endif  // SRC_INTERVENTION_LOG_H_
