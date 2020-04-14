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


    explicit RuleMessage(const RuleMessage &ruleMessage) :
        m_severity(ruleMessage.m_severity),
        m_tags(),
        m_data(ruleMessage.m_data),
        m_match(ruleMessage.m_match),
        m_message(ruleMessage.m_message),
        m_reference(ruleMessage.m_reference),
        m_transaction(ruleMessage.m_transaction),
        m_rule(ruleMessage.m_rule)
    { }


    explicit RuleMessage(Transaction *transaction) :
        m_severity(0),
        m_tags(),
        m_data(""),
        m_match(""),
        m_message(""),
        m_reference(""),
        m_transaction(transaction),
        m_rule(nullptr)
    { }

    RuleMessage &operator=(const RuleMessage& ruleMessage) {
        m_severity = ruleMessage.m_severity;
        m_tags = ruleMessage.m_tags;
        m_data = ruleMessage.m_data;
        m_match = ruleMessage.m_match;
        m_message = ruleMessage.m_message;
        m_reference = ruleMessage.m_reference;
        m_transaction = ruleMessage.m_transaction;
        m_rule = ruleMessage.m_rule;
        return *this;
    }

    void clean() {
        m_data = "";
        m_match = "";
        m_reference = "";
        m_severity = 0;
    }

    std::string log() {
        return log(this, 0);
    }
    std::string log(int props) {
        return log(this, props);
    }
    std::string log(int props, int responseCode) {
        return log(this, props, responseCode);
    }
    std::string errorLog() {
        return log(this,
		ClientLogMessageInfo | ErrorLogTailLogMessageInfo);
    }

    static std::string log(const RuleMessage *rm, int props, int code);
    static std::string log(const RuleMessage *rm, int props) {
        return log(rm, props, -1);
    }
    static std::string log(const RuleMessage *rm) {
        return log(rm, 0);
    }

    static std::string _details(const RuleMessage *rm);
    static std::string _errorLogTail(const RuleMessage *rm);

    RuleWithActions *getRule() const {
        return m_rule;
    }

    void setRule(RuleWithActions *rule) {
        m_rule = rule;
    }

    bool isSettle() const {
        return m_rule != nullptr;
    }

    int getRuleId() const {
        if (m_rule) {
            return m_rule->m_ruleId;
        }
        return -1;
    }

    int getPhase() const {
        if (m_rule) {
            return m_rule->getPhase();
        }
        return 0;
    }

    std::string getFileName() const {
        if (m_rule) {
            return *m_rule->getFileName().get();
        }
        return "";
    }

    int getLineNumber() const {
        if (m_rule) {
            return m_rule->getLineNumber();
        }
        return 0;
    }

    std::string getRev() const {
        if (m_rule) {
            return m_rule->getRevision();
        }
        return "";
    }

    std::string getVer() const {
        if (m_rule) {
            return m_rule->getRevision();
        }
        return "";
    }

    int getMaturity() const {
        if (m_rule) {
            return m_rule->getMaturity();
        }
        return 0;
    }

    int getAccuracy() const {
        if (m_rule) {
            return m_rule->getAccuracy();
        }
        return 0;
    }

    std::string getClientIpAddress() const {
        if (m_transaction) {
            return *m_transaction->m_clientIpAddress.get();
        }
        return "";
    }

    std::string getServerIpAddress() const {
        if (m_transaction) {
            return *m_transaction->m_serverIpAddress.get();
        }
        return "";
    }

    std::string getRequestId() const {
        if (m_transaction) {
            return *m_transaction->m_id.get();
        }
        return "";
    }

    std::string getUri() const {
        if (m_transaction) {
            return *m_transaction->m_uri_no_query_string_decoded.get();
        }
        return "";
    }

    bool isDisruptive() const {
        if (m_rule) {
            return m_rule->hasDisruptiveAction();
        }
        return 0;
    }

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
