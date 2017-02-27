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

#ifdef __cplusplus
#include <utility>
#endif

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"


#ifdef __cplusplus

namespace modsecurity {

class RuleMessageHighlightArea {
 public:
    RuleMessageHighlightArea()
        : m_startingAt(0),
        m_size(0) { }
    size_t m_startingAt;
    size_t m_size;
};


class RuleMessageHighlightOperator {
 public:
    RuleMessageHighlightOperator()
        : m_value("") { }
    RuleMessageHighlightArea m_area;
    std::string m_value;
};


class RuleMessageHighlight {
 public:
    std::list<RuleMessageHighlightArea> m_variable;
    std::list<std::pair<std::string, std::string>> m_value;
    std::list<RuleMessageHighlightOperator> m_op;
};


class RuleMessage {
 public:
    RuleMessage(Rule *rule, Transaction *trans);

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
    static RuleMessageHighlight computeHighlight(const RuleMessage *rm,
        const std::string buf);

    int m_accuracy;
    std::string m_buf;
    std::string m_clientIpAddress;
    std::string m_data;
    std::string m_disruptiveMessage;
    std::string m_id;
    bool m_isDisruptive;
    std::string m_match;
    int m_maturity;
    std::string m_message;
    bool m_noAuditLog;
    std::string m_opValue;
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
    std::string m_varValue;
    std::string m_ver;

    std::list<std::string> m_tags;
    RuleMessageHighlight m_highlight;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_MESSAGE_H_
