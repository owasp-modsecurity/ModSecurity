/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


std::string RuleMessage::_details(const RuleMessage *rm) {
    std::string msg;

    msg.append(" [file \"" + rm->getFileName() + "\"]");
    msg.append(" [line \"" + std::to_string(rm->getLineNumber()) + "\"]");
    msg.append(" [id \"" + std::to_string(rm->getRuleId()) + "\"]");
    msg.append(" [rev \"" + rm->getRev() + "\"]");
    msg.append(" [msg \"" + rm->m_message + "\"]");
    msg.append(" [data \"" + utils::string::limitTo(200, rm->m_data) + "\"]");
    msg.append(" [severity \"" +
        std::to_string(rm->m_severity) + "\"]");

    msg.append(" [ver \"" + rm->getVer() + "\"]");
    msg.append(" [maturity \"" + std::to_string(rm->getMaturity()) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(rm->getAccuracy()) + "\"]");
    for (auto &a : rm->m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [hostname \"" + rm->getServerIpAddress() + "\"]");
    msg.append(" [uri \"" + utils::string::limitTo(200, rm->getUri()) + "\"]");
    msg.append(" [unique_id \"" + rm->getRequestId() + "\"]");
    msg.append(" [ref \"" + rm->m_reference + "\"]");

    return msg;
}


std::string RuleMessage::_errorLogTail(const RuleMessage *rm) {
    std::string msg;

    msg.append("[hostname \"" + rm->getServerIpAddress() + "\"]");
    msg.append(" [uri \"" + rm->getUri() + "\"]");
    msg.append(" [unique_id \"" + rm->getRequestId() + "\"]");

    return msg;
}


std::string RuleMessage::log(const RuleMessage *rm, int props, int code) {
    std::string msg("");
    msg.reserve(2048);

    if (props & ClientLogMessageInfo) {
        msg.append("[client " + rm->getClientIpAddress() + "] ");
    }

    if (rm->isDisruptive()
        && (rm->m_transaction->getRuleEngineState() == RulesSet::EnabledRuleEngine)) {
        msg.append("ModSecurity: Access denied with code ");
        if (code == -1) {
            msg.append("%d");
        } else {
            msg.append(std::to_string(code));
        }
        msg.append(" (phase ");
        msg.append(std::to_string(rm->getPhase() - 1) + "). ");
    } else {
        msg.append("ModSecurity: Warning. ");
    }

    msg.append(rm->m_match);
    msg.append(_details(rm));

    if (props & ErrorLogTailLogMessageInfo) {
        msg.append(" " + _errorLogTail(rm));
    }

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


}  // namespace modsecurity
