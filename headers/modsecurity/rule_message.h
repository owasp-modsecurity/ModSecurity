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
#include "modsecurity/rule_with_operator.h"


#ifdef __cplusplus

namespace modsecurity {



class RuleMessage {
 public:
    enum LogMessageInfo {
        ErrorLogTailLogMessageInfo = 2,
        ClientLogMessageInfo = 4
    };


    RuleMessage(const RuleMessage *rule) :
        m_data(rule->m_data),
        m_isDisruptive(rule->m_isDisruptive),
        m_match(rule->m_match),
        m_message(rule->m_message),
        m_reference(rule->m_reference),
        m_rule(rule->m_rule),
        m_saveMessage(rule->m_saveMessage),
        m_transaction(rule->m_transaction)
    { }


    RuleMessage(Transaction *transaction) :
        m_transaction(transaction),
        m_rule(nullptr),
        m_data(""),
        m_isDisruptive(false),
        m_match(""),
        m_message(""),
        m_reference(""),
        m_saveMessage(true),
        m_severity(0)
    { }

    void clean() {
        m_data = "";
        m_match = "";
        m_isDisruptive = false;
        m_reference = "";
        m_severity = 0;
    }

    std::string log() {
        return RuleMessage::log(this, 0);
    }
    std::string log(int props) {
        return RuleMessage::log(this, props);
    }
    std::string log(int props, int responseCode) {
        return RuleMessage::log(this, props, responseCode);
    }
    std::string errorLog() {
        return RuleMessage::log(this,
		ClientLogMessageInfo | ErrorLogTailLogMessageInfo);
    }

    static std::string log(RuleMessage *rm, int props, int code);
    static std::string log(RuleMessage *rm, int props) {
        return RuleMessage::log(rm, props, -1);
    }
    static std::string log(RuleMessage *rm) {
        return RuleMessage::log(rm, 0);
    }

    static std::string _details(RuleMessage *rm);
    static std::string _errorLogTail(RuleMessage *rm);

    RuleWithActions *getRule() {
        return m_rule;
    }
    void setRule(RuleWithActions *rule) {
        m_rule = rule;
    }

    bool isSettle() {
        return m_rule != nullptr;
    }

    int getRuleId() {
        if (m_rule) {
            return m_rule->m_ruleId;
        }
        return 0;
    }

    int getPhase() {
        if (m_rule) {
            return m_rule->getPhase();
        }
        return 0;
    }

    std::string getFileName() {
        if (m_rule) {
            return *m_rule->getFileName().get();
        }
        return "";
    }

    int getLineNumber() {
        if (m_rule) {
            return m_rule->getLineNumber();
        }
        return 0;
    }

    std::string getRev() {
        if (m_rule) {
            return m_rule->m_rev;
        }
        return "";
    }

    std::string getVer() {
        if (m_rule) {
            return m_rule->m_rev;
        }
        return "";
    }

    int getMaturity() {
        if (m_rule) {
            return m_rule->m_maturity;
        }
        return 0;
    }

    int getAccuracy() {
        if (m_rule) {
            return m_rule->m_accuracy;
        }
        return 0;
    }

    std::string getClientIpAddress() {
        if (m_transaction) {
            return *m_transaction->m_clientIpAddress.get();
        }
        return "";
    }

    std::string getServerIpAddress() {
        if (m_transaction) {
            return *m_transaction->m_serverIpAddress.get();
        }
        return "";
    }

    std::string getRequestId() {
        if (m_transaction) {
            return *m_transaction->m_id.get();
        }
        return "";
    }

    std::string getUri() {
        if (m_transaction) {
            return *m_transaction->m_uri_no_query_string_decoded.get();
        }
        return "";
    }

    // Rule
    bool m_isDisruptive;
    bool m_saveMessage;
    int m_severity;
    std::list<std::string> m_tags;

    // Transaction
    std::string m_data;
    std::string m_match;

    std::string m_message;
    std::string m_reference;

 private:
    Transaction *m_transaction;
    RuleWithActions *m_rule;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_MESSAGE_H_
