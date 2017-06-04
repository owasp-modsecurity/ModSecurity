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
#include "reading_logs_via_rule_message.h"


std::unordered_multimap<std::string, std::string> requestHeaders;

int main(int argc, char **argv) {
    char a = '\0';
    *argv++;
    if (*argv == NULL) {
        *argv--;
        std::cout << "Use " << *argv << " test-case-file.conf";
        std::cout << std::endl << std::endl;
        return -1;
    }

    std::string rules(*argv);
    ReadingLogsViaRuleMessage rlvrm(requestHeaders, request_uri, request_body,
        &a, response_body, ip, rules);
    rlvrm.process();

    pthread_exit(NULL);
    return 0;
}
