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

#include "modsecurity/rules.h"

#include "modsecurity/rule_message.h"

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/string.h"

namespace modsecurity {

std::string RuleMessage::disruptiveErrorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("[client " + std::string(rm->m_clientIpAddress) + "]");
    msg.append(" ModSecurity: ");
    msg.append(rm->m_disruptiveMessage);
    msg.append(rm->m_match);
    msg.append(" [file \"" + std::string(rm->m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(rm->m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(rm->m_ruleId) + "\"]");
    msg.append(" [rev \"" + rm->m_rev + "\"]");
    msg.append(" [msg \"" + rm->m_message + "\"]");
    msg.append(" [data \"" + rm->m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(rm->m_severity) + "\"]");
    msg.append(" [ver \"" + rm->m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(rm->m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(rm->m_accuracy) + "\"]");
    for (auto &a : rm->m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [hostname \"" + std::string(rm->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + rm->m_uriNoQueryStringDecoded + "\"]");
    msg.append(" [unique_id \"" + rm->m_id + "\"]");
    msg.append(" [ref \"" + rm->m_reference + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


std::string RuleMessage::noClientErrorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("ModSecurity: Warning. ");
    msg.append(rm->m_match);
    msg.append(" [file \"" + std::string(rm->m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(rm->m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(rm->m_ruleId) + "\"]");
    msg.append(" [rev \"" + rm->m_rev + "\"]");
    msg.append(" [msg \"" + rm->m_message + "\"]");
    msg.append(" [data \"" + rm->m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(rm->m_severity) + "\"]");
    msg.append(" [ver \"" + rm->m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(rm->m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(rm->m_accuracy) + "\"]");
    for (auto &a : rm->m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [ref \"" + rm->m_reference + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}

std::string RuleMessage::errorLogTail(const RuleMessage *rm) {
    std::string msg;

    msg.append("[hostname \"" + std::string(rm->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + rm->m_uriNoQueryStringDecoded + "\"]");
    msg.append(" [unique_id \"" + rm->m_id + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}

std::string RuleMessage::errorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("[client " + std::string(rm->m_clientIpAddress) + "] ");
    msg.append(noClientErrorLog(rm));
    msg.append(" " + errorLogTail(rm));

    return msg;
}

std::string RuleMessage::log(const RuleMessage *rm) {
    std::string msg;

    if (rm->m_isDisruptive) {
        msg.append(disruptiveErrorLog(rm));
    } else {
        msg.append(errorLog(rm));
    }

    return msg;
}

}  // namespace modsecurity
