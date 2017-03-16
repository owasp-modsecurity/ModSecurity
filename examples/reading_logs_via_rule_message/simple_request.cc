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

#include <stdio.h>
#include <string.h>

#include <modsecurity/modsecurity.h>
#include <modsecurity/rules.h>

#include "examples/reading_logs_via_rule_message/reading_logs_via_rule_message.h"



char request_uri[] = "/TeSt.Pl?param1=TEsT&para2=TEST2";

char request_uri2[] = "/index.html?d=1";

char request_body[] = "";

char response_body[] = "" \
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\r" \
    "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" " \
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " \
    "xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\n\r" \
    "  <soap:Body>\n\r" \
    "  <EnlightenResponse xmlns=\"http://clearforest.com/\">\n\r" \
    "  <EnlightenResult>string</EnlightenResult>\n\r" \
    "  </EnlightenResponse>\n\r" \
    "  </soap:Body>\n\r" \
    "</soap:Envelope>\n\r";

char ip[] = "200.249.12.31";

std::unordered_multimap<std::string, std::string> requestHeaders;

int main(int argc, char **argv) {
    (*argv++);
    if (*argv == NULL) {
        (*argv--);
        std::cout << "Use " << *argv << " test-case-file.conf";
        std::cout << std::endl << std::endl;
        return -1;
    }

    requestHeaders.emplace("Host", "SITE2");
    requestHeaders.emplace("User-Agent", "wFetch");
    requestHeaders.emplace("test", "21321");


    std::string rules(*argv);
    ReadingLogsViaRuleMessage rlvrm(requestHeaders, request_uri, request_body,
        "", response_body, ip, rules);
    rlvrm.process();
    return 0;
}
