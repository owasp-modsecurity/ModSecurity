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

#include <string.h>

#include <ctime>
#include <iostream>
#include <string>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"

using modsecurity::Transaction;

char request_header[] =  "" \
    "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\n\r" \
    "Host: net.tutsplus.com\n\r" \
    "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5)" \
    " Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\n\r" \
    "Accept: text/html,application/xhtml+xml,application/xml; " \
    "q=0.9,*/*;q=0.8\n\r" \
    "Accept-Language: en-us,en;q=0.5\n\r" \
    "Accept-Encoding: gzip,deflate\n\r" \
    "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\n\r" \
    "Keep-Alive: 300\n\r" \
    "Connection: keep-alive\n\r" \
    "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\n\r" \
    "Pragma: no-cache\n\r" \
    "Cache-Control: no-cache\n\r";

char request_uri[] = "GET /test.pl?param1=test&para2=test2";

char request_body[] = "";

char response_headers[] = "" \
    "HTTP/1.1 200 OK\n\r" \
    "Content-Type: text/xml; charset=utf-8\n\r" \
    "Content-Length: length\n\r";

unsigned char response_body[] = "" \
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

char ip[] = "192.0.2.31";

char rules_file[] = "basic_rules.conf";


#define NUM_REQUESTS 10000


int main(int argc, char *argv[]) {
    int i = 0;
    modsecurity::ModSecurity *modsec;
    modsecurity::Rules *rules;
    modsecurity::ModSecurityIntervention it;

    modsec = new modsecurity::ModSecurity();
    modsec->setConnectorInformation("ModSecurity-benchmark v0.0.1-alpha" \
            " (ModSecurity benchmark utility)");

    rules = new modsecurity::Rules();
    if (rules->loadFromUri(rules_file) < 0) {
        std::cout << "Problems loading the rules..." << std::endl;
        std::cout << rules->m_parserError.str() << std::endl;
        return -1;
    }

    for (i = 0; i < NUM_REQUESTS; i++) {
        std::cout << "Proceeding with request " << i << std::endl;

        Transaction *modsecTransaction = new Transaction(modsec, rules, NULL);
        modsecTransaction->processConnection(ip, 12345, "127.0.0.1", 80);

        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }
        modsecTransaction->processURI(request_uri, "GET", "1.1");
        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }

        modsecTransaction->addRequestHeader("Host",
            "net.tutsplus.com");
        modsecTransaction->addRequestHeader("User-Agent",
            "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) " \
            "Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)");
        modsecTransaction->addRequestHeader("Accept",
            "text/html,application/xhtml+xml,application/xml;" \
            "q=0.9,*/*;q=0.8");
        modsecTransaction->addRequestHeader("Accept-Language",
            "en-us,en;q=0.5");
        modsecTransaction->addRequestHeader("Accept-Encoding",
            "gzip,deflate");
        modsecTransaction->addRequestHeader("Accept-Charset",
            "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
        modsecTransaction->addRequestHeader("Keep-Alive",
            "300");
        modsecTransaction->addRequestHeader("Connection",
            "keep-alive");
        modsecTransaction->addRequestHeader("Cookie",
            "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120");
        modsecTransaction->addRequestHeader("Pragma",
            "no-cache");
        modsecTransaction->addRequestHeader("Cache-Control",
            "no-cache");
        modsecTransaction->processRequestHeaders();

        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }


        modsecTransaction->processRequestBody();

        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }

        modsecTransaction->addResponseHeader("HTTP/1.1",
            "200 OK");
        modsecTransaction->addResponseHeader("Content-Type",
            "text/xml; charset=utf-8");
        modsecTransaction->addResponseHeader("Content-Length",
            "200");

        modsecTransaction->processResponseHeaders(200, "HTTP 1.2");

        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }


        modsecTransaction->appendResponseBody(response_body,
            strlen((const char*)response_body));
        modsecTransaction->processResponseBody();

        if (modsecTransaction->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            goto next_request;
        }

next_request:
        modsecTransaction->processLogging();
        delete modsecTransaction;
    }

    delete rules;
    delete modsec;
}
