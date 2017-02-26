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

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
#include <list>
#include <cstring>
#endif

#ifndef HEADERS_MODSECURITY_RULE_MESSAGE_H_
#define HEADERS_MODSECURITY_RULE_MESSAGE_H_

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"


#ifdef __cplusplus

namespace modsecurity {

class RuleMessage {
 public:
    explicit RuleMessage(Rule *rule, Transaction *trans) :
        m_accuracy(rule->m_accuracy),
        m_clientIpAddress(trans->m_clientIpAddress),
        m_data(""),
        m_disruptiveMessage(""),
        m_id(trans->m_id),
        m_isDisruptive(false),
        m_match(""),
        m_maturity(rule->m_maturity),
        m_message(""),
        m_noAuditLog(false),
        m_phase(rule->m_phase - 1),
        m_reference(""),
        m_rev(rule->m_rev),
        m_rule(rule),
        m_ruleFile(rule->m_fileName),
        m_ruleId(rule->m_ruleId),
        m_ruleLine(rule->m_lineNumber),
        m_saveMessage(false),
        m_serverIpAddress(trans->m_serverIpAddress),
        m_severity(0),
        m_uriNoQueryStringDecoded(trans->m_uri_no_query_string_decoded),
        m_ver(rule->m_ver)
    { }

    std::string errorLog() {
        return RuleMessage::errorLog(this);
    }
    std::string disruptiveErrorLog() {
        return RuleMessage::disruptiveErrorLog(this);
    }
    std::string noClientErrorLog() {
        return RuleMessage::noClientErrorLog(this);
    }
    std::string errorLogTail() {
        return RuleMessage::errorLogTail(this);
    }
    std::string log() {
        return RuleMessage::log(this);
    }
    static std::string disruptiveErrorLog(const RuleMessage *rm);
    static std::string noClientErrorLog(const RuleMessage *rm);
    static std::string errorLogTail(const RuleMessage *rm);
    static std::string errorLog(const RuleMessage *rm);
    static std::string log(const RuleMessage *rm);

    int m_accuracy;
    std::string m_clientIpAddress;
    std::string m_data;
    std::string m_disruptiveMessage;
    std::string m_id;
    bool m_isDisruptive;
    std::string m_match;
    int m_maturity;
    std::string m_message;
    bool m_noAuditLog;
    int m_phase;
    std::string m_reference;
    std::string m_rev;
    Rule *m_rule;
    std::string m_ruleFile;
    int m_ruleId;
    int m_ruleLine;
    bool m_saveMessage;
    std::string m_serverIpAddress;
    int m_severity;
    std::string m_uriNoQueryStringDecoded;
    std::string m_ver;

    std::list<std::string> m_tags;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_MESSAGE_H_
