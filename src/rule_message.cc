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

std::string RuleMessage::disruptiveErrorLog(Transaction *trans,
    std::string msg2) {
    std::string msg;

    msg.append("[client " + std::string(trans->m_clientIpAddress) + "]");
    msg.append(" ModSecurity: ");
    msg.append(msg2);
    msg.append(m_match);
    msg.append(" [file \"" + std::string(m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(m_ruleId) + "\"]");
    msg.append(" [rev \"" + m_rev + "\"]");
    msg.append(" [msg \"" + m_message + "\"]");
    msg.append(" [data \"" + m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(m_severity) + "\"]");
    msg.append(" [ver \"" + m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(m_accuracy) + "\"]");
    for (auto &a : m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [hostname \"" + std::string(trans->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + trans->m_uri_no_query_string_decoded + "\"]");
    msg.append(" [unique_id \"" + trans->m_id + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


std::string RuleMessage::noClientErrorLog(Transaction *trans) {
    std::string msg;

    msg.append("ModSecurity: Warning. ");
    msg.append(m_match);
    msg.append(" [file \"" + std::string(m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(m_ruleId) + "\"]");
    msg.append(" [rev \"" + m_rev + "\"]");
    msg.append(" [msg \"" + m_message + "\"]");
    msg.append(" [data \"" + m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(m_severity) + "\"]");
    msg.append(" [ver \"" + m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(m_accuracy) + "\"]");
    for (auto &a : m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }

    return modsecurity::utils::string::toHexIfNeeded(msg);
}

std::string RuleMessage::errorLogTail(Transaction *trans) {
    std::string msg;

    msg.append("[hostname \"" + std::string(trans->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + trans->m_uri_no_query_string_decoded + "\"]");
    msg.append(" [unique_id \"" + trans->m_id + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}

std::string RuleMessage::errorLog(Transaction *trans) {
    std::string msg;

    msg.append("[client " + std::string(trans->m_clientIpAddress) + "] ");
    msg.append(noClientErrorLog(trans));
    msg.append(" " + errorLogTail(trans));

    return msg;
}

}  // namespace modsecurity
