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

using ModSecurity::Assay;

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

char ip[] = "200.249.12.31";

char rules_file[] = "basic_rules.conf";


#define NUM_REQUESTS 10000

int main(int argc, char *argv[]) {
    int i = 0;
    ModSecurity::ModSecurity *modsec;
    ModSecurity::Rules *rules;
    ModSecurity::ModSecurityIntervention it;

    modsec = new ModSecurity::ModSecurity();
    modsec->setConnectorInformation("ModSecurity-benchmark v0.0.1-alpha" \
            " (ModSecurity benchmark utility)");

    rules = new ModSecurity::Rules();
    if (rules->loadFromUri(rules_file) < 0) {
        std::cout << "Problems loading the rules..." << std::endl;
        std::cout << rules->parserError.str() << std::endl;
        return -1;
    }

    for (i = 0; i < NUM_REQUESTS; i++) {
        std::cout << "Proceeding with request " << i << std::endl;

        Assay *modsecAssay = new Assay(modsec, rules, NULL);
        modsecAssay->processConnection(ip, 12345, "127.0.0.1", 80);

        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }
        modsecAssay->processURI(request_uri, "GET", "1.1");
        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        modsecAssay->addRequestHeader("Host",
            "net.tutsplus.com");
        modsecAssay->addRequestHeader("User-Agent",
            "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) " \
            "Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\n\r");
        modsecAssay->addRequestHeader("Accept",
            "text/html,application/xhtml+xml,application/xml;" \
            "q=0.9,*/*;q=0.8");
        modsecAssay->addRequestHeader("Accept-Language",
            "en-us,en;q=0.5");
        modsecAssay->addRequestHeader("Accept-Encoding",
            "gzip,deflate");
        modsecAssay->addRequestHeader("Accept-Charset",
            "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
        modsecAssay->addRequestHeader("Keep-Alive",
            "300");
        modsecAssay->addRequestHeader("Connection",
            "keep-alive");
        modsecAssay->addRequestHeader("Cookie",
            "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120");
        modsecAssay->addRequestHeader("Pragma",
            "no-cache");
        modsecAssay->addRequestHeader("Cache-Control",
            "no-cache");
        modsecAssay->processRequestHeaders();

        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }


        modsecAssay->processRequestBody();

        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        modsecAssay->addResponseHeader("HTTP/1.1",
            "200 OK");
        modsecAssay->addResponseHeader("Content-Type",
            "text/xml; charset=utf-8");
        modsecAssay->addResponseHeader("Content-Length",
            "200");

        modsecAssay->processResponseHeaders();

        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }


        modsecAssay->appendResponseBody(response_body,
            strlen((const char*)response_body));
        modsecAssay->processResponseBody();

        if (modsecAssay->intervention(&it)) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        delete modsecAssay;
    }

    delete modsec;
}
