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

#include <string>
#include <memory>

#include "modsecurity/rule_message.h"

#ifndef EXAMPLES_READING_LOGS_VIA_RULE_MESSAGE_READING_LOGS_VIA_RULE_MESSAGE_H_
#define EXAMPLES_READING_LOGS_VIA_RULE_MESSAGE_READING_LOGS_VIA_RULE_MESSAGE_H_


class ReadingLogsViaRuleMessage {
 public:
    ReadingLogsViaRuleMessage(char *request_header,
        char *request_uri,
        char *request_body,
        char *response_headers,
        char *response_body,
        char *ip,
        std::string rules) :
            m_request_header(request_header),
            m_request_uri(request_uri),
            m_request_body(request_body),
            m_response_headers(response_headers),
            m_response_body(response_body),
            m_ip(ip),
            m_rules(rules)
        { }

    int process() {
        modsecurity::ModSecurity *modsec;
        modsecurity::Rules *rules;
        modsecurity::ModSecurityIntervention it;

        modsec = new modsecurity::ModSecurity();
        modsec->setConnectorInformation("ModSecurity-test v0.0.1-alpha" \
            " (ModSecurity test)");
        modsec->setServerLogCb(logCb, modsecurity::RuleMessageLogProperty
            | modsecurity::IncludeFullHighlightLogProperty);

        rules = new modsecurity::Rules();
        if (rules->loadFromUri(m_rules.c_str()) < 0) {
            std::cout << "Problems loading the rules..." << std::endl;
            std::cout << rules->m_parserError.str() << std::endl;
            return -1;
        }

        modsecurity::Transaction *modsecTransaction = \
            new modsecurity::Transaction(modsec, rules, NULL);
        modsecTransaction->processConnection(m_ip, 12345, "127.0.0.1", 80);
        modsecTransaction->processURI(m_request_uri, "GET", "1.1");

        modsecTransaction->addRequestHeader("Host",
            "net.tutsplus.com");
        modsecTransaction->processRequestHeaders();
        modsecTransaction->processRequestBody();

        modsecTransaction->addResponseHeader("Content-Type", "text/xml; charset=utf-8");
        modsecTransaction->addResponseHeader("Content-Length", "123");
        modsecTransaction->processResponseHeaders(200, "HTTP 1.2");
        modsecTransaction->appendResponseBody(
            (const unsigned char*)m_response_body,
            strlen((const char*)m_response_body));
        modsecTransaction->processResponseBody();
        modsecTransaction->processLogging();

        delete modsecTransaction;
        delete rules;
        delete modsec;
        return 0;
end:
        return -1;
    }


    static std::string highlightToText(
        const modsecurity::RuleMessageHighlight &h) {
        std::cout << " * ModSecurity variable to be highlighted" << std::endl;

        for (const auto &i : h.m_variable) {
            std::cout << "  - From: " << std::to_string(i.m_startingAt);
            std::cout << " to: " << std::to_string(i.m_startingAt + i.m_size);
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << " * Variable's values ";
        std::cout << "(may include transformations)" << std::endl;
        for (const auto &i : h.m_value) {
            std::cout << "  - " << i.first << ": " << i.second << std::endl;
        }
        std::cout << std::endl;

        std::cout << " * Operators match to be highlight inside ";
        std::cout << "the variables (after transformations)" << std::endl;

        for (const auto &i : h.m_op) {
            std::cout << "  - From: " << i.m_area.m_startingAt;
            std::cout << " to: " << std::to_string(i.m_area.m_startingAt \
                + i.m_area.m_size);
            std::cout << " [Value: " << i.m_value << "]" << std::endl;
        }
        std::cout << std::endl;
        return "";
    }

    static void logCb(void *data, const void *ruleMessagev) {
        if (ruleMessagev == NULL) {
            std::cout << "I've got a call but the message was null ;(";
            std::cout << std::endl;
            return;
        }

        const modsecurity::RuleMessage *ruleMessage = \
            reinterpret_cast<const modsecurity::RuleMessage *>(ruleMessagev);

        std::cout << "Rule Id: " << std::to_string(ruleMessage->m_ruleId);
        std::cout << " phase: " << std::to_string(ruleMessage->m_phase);
        std::cout << std::endl;
        if (ruleMessage->m_isDisruptive) {
            std::cout << " * Disruptive action: ";
            std::cout << modsecurity::RuleMessage::log(ruleMessage);
            std::cout << std::endl;
            std::cout << " ** %d is meant to be informed by the webserver.";
            std::cout << std::endl;
        } else {
            std::cout << " * Match, but no disruptive action: ";
            std::cout << modsecurity::RuleMessage::log(ruleMessage);
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Verbose details on the match highlight" << std::endl;
        std::cout << "  Highlight reference string: ";
        std::cout << ruleMessage->m_reference << std::endl;
        std::cout << std::endl;
        std::cout << "Details:" << std::endl;
        modsecurity::RuleMessageHighlight h =
            modsecurity::RuleMessage::computeHighlight(ruleMessage,
                ruleMessage->m_buf);
        highlightToText(h);
        std::cout << std::endl;
    }

 protected:
    char *m_request_header;
    char *m_request_uri;
    char *m_request_body;
    char *m_response_headers;
    char *m_response_body;
    char *m_ip;
    std::string m_rules;
};

#endif  // EXAMPLES_READING_LOGS_VIA_RULE_MESSAGE_READING_LOGS_VIA_RULE_MESSAGE_H_
