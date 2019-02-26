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
#include "src/actions/tag.h"
#include "modsecurity/rule_with_actions.h"


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


RuleWithActions *RuleMessage::getRule() const {
    return m_rule;
}


void RuleMessage::setRule(RuleWithActions *rule) {
    m_rule = rule;
}


bool RuleMessage::isSettle() const {
    return m_rule != nullptr;
}


int RuleMessage::getRuleId() const {
    if (m_rule) {
        return m_rule->getId();
    }
    return -1;
}


int RuleMessage::getPhase() const {
    if (m_rule) {
        return m_rule->getPhase();
    }
    return 0;
}


std::string RuleMessage::getFileName() const {
    if (m_rule) {
        return *m_rule->getFileName().get();
    }
    return "";
}


int RuleMessage::getLineNumber() const {
    if (m_rule) {
        return m_rule->getLineNumber();
    }
    return 0;
}


std::string RuleMessage::getRev() const {
    if (m_rule) {
        return m_rule->getRevision();
    }
    return "";
}


std::string RuleMessage::getVer() const {
    if (m_rule) {
        return m_rule->getVersion();
    }
    return "";
}


int RuleMessage::getMaturity() const {
    if (m_rule) {
        return m_rule->getMaturity();
    }
    return 0;
}


int RuleMessage::getAccuracy() const {
    if (m_rule) {
        return m_rule->getAccuracy();
    }
    return 0;
}


std::string RuleMessage::getClientIpAddress() const {
    if (m_transaction) {
        return *m_transaction->m_clientIpAddress.get();
    }
    return "";
}


std::string RuleMessage::getServerIpAddress() const {
    if (m_transaction) {
        return *m_transaction->m_serverIpAddress.get();
    }
    return "";
}


std::string RuleMessage::getRequestId() const {
    if (m_transaction) {
        return *m_transaction->m_id.get();
    }
    return "";
}


std::string RuleMessage::getUri() const {
    if (m_transaction) {
        return *m_transaction->m_uri_no_query_string_decoded.get();
    }
    return "";
}


bool RuleMessage::isDisruptive() const {
    if (m_rule) {
        return m_rule->hasDisruptiveAction();
    }
    return 0;
}


} // namespace modsecurity
