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
    explicit RuleMessage(Rule *rule) :
        m_ruleFile(rule->m_fileName),
        m_ruleLine(rule->m_lineNumber),
        m_ruleId(rule->rule_id),
        m_rev(rule->m_rev),
        m_accuracy(rule->m_accuracy),
        m_message(std::string("")),
        m_data(std::string("")),
        m_severity(0),
        m_ver(rule->m_ver),
        m_maturity(rule->m_maturity),
        m_rule(rule),
        m_saveMessage(false),
        m_noAuditLog(false),
        m_match(std::string(""))
    { }

    std::string errorLog(Transaction *trans);
    std::string disruptiveErrorLog(Transaction *trans, std::string log2);
    std::string noClientErrorLog(Transaction *trans);
    std::string errorLogTail(Transaction *trans);

    std::string m_match;
    std::string m_ruleFile;
    int m_ruleLine;
    int m_ruleId;
    std::string m_message;
    std::string m_data;
    int m_severity;
    std::string m_ver;
    std::string m_rev;
    int m_maturity;
    int m_accuracy;

    std::list<std::string> m_tags;
    std::list<std::string> m_server_logs;

    bool m_noAuditLog;
    Rule *m_rule;
    bool m_saveMessage;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_MESSAGE_H_
