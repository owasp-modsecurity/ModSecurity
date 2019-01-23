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

#include "modsecurity/rule_message.h"

#include "modsecurity/rules_set.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/string.h"

namespace modsecurity {


inline void RuleMessage::_details(const RuleMessage *rm, std::string *msg) {
    *msg += " [file \"" + std::string(*rm->m_ruleFile.get()) + "\"]" \
        " [line \"" + std::to_string(rm->m_ruleLine) + "\"]" \
        " [id \"" + std::to_string(rm->m_ruleId) + "\"]" \
        " [rev \"" + rm->m_rev + "\"]" \
        " [msg \"" + rm->m_message + "\"]" \
        " [data \"" + utils::string::limitTo(200, rm->m_data) + "\"]" \
        " [severity \"" + std::to_string(rm->m_severity) + "\"]" \
        " [ver \"" + rm->m_ver + "\"]" \
        " [maturity \"" + std::to_string(rm->m_maturity) + "\"]" \
        " [accuracy \"" + std::to_string(rm->m_accuracy) + "\"]";

    for (auto &a : rm->m_tags) {
        *msg += " [tag \"" + a + "\"]";
    }

    *msg += " [hostname \"" + *rm->m_serverIpAddress.get() + "\"]" \
        " [uri \"" + *rm->m_uriNoQueryStringDecoded.get() + "\"]" \
        " [unique_id \"" + *rm->m_id.get() + "\"]" \
        " [ref \"" + utils::string::limitTo(200, rm->m_reference) + "\"]";
}


inline void RuleMessage::_errorLogTail(const RuleMessage *rm,
    std::string *msg) {
    *msg += " [hostname \"" + *rm->m_serverIpAddress.get() + "\"]" \
        " [uri \"" + utils::string::limitTo(200,
            *rm->m_uriNoQueryStringDecoded.get()) + "\"]" \
        " [unique_id \"" + *rm->m_id.get() + "\"]";
}


std::string RuleMessage::log(const RuleMessage *rm, int props, int code) {
    std::string msg("");
    msg.reserve(2048);

    if (props & ClientLogMessageInfo) {
        msg += "[client " + std::string(*rm->m_clientIpAddress.get()) + "] ";
    }

    if (rm->m_isDisruptive) {
        msg += "ModSecurity: Access denied with code ";
        if (code == -1) {
            msg += "%d";
        } else {
            msg += std::to_string(code);
        }
        msg += " (phase " + std::to_string(rm->m_rule->m_phase - 1) + "). ";
    } else {
        msg += "ModSecurity: Warning. ";
    }

    msg += (rm->m_match);
    _details(rm, &msg);

    if (props & ErrorLogTailLogMessageInfo) {
        _errorLogTail(rm, &msg);
    }

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


}  // namespace modsecurity
