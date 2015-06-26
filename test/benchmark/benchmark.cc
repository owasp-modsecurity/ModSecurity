/**
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


#define NUM_REQUESTS 1000

int main(int argc, char *argv[]) {
    int i = 0;
    ModSecurity::ModSecurity *modsec;
    ModSecurity::Rules *rules;

    modsec = new ModSecurity::ModSecurity();
    rules = new ModSecurity::Rules();
    rules->loadFromUri(rules_file);

    for (i = 0; i < NUM_REQUESTS; i++) {
        std::cout << "Proceding with request " << i << std::endl;

        Assay *modsecAssay = new Assay(modsec, rules);
        modsecAssay->processConnection(ip);

        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }
        modsecAssay->processURI(request_uri);
        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        modsecAssay->addRequestHeader((char *) "Host",
            (char *) "net.tutsplus.com");
        modsecAssay->addRequestHeader((char *) "User-Agent", (char *)
            "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) " \
            "Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\n\r");
        modsecAssay->addRequestHeader((char *) "Accept",
            (char *) "text/html,application/xhtml+xml,application/xml;" \
            "q=0.9,*/*;q=0.8");
        modsecAssay->addRequestHeader((char *) "Accept-Language",
            (char *) "en-us,en;q=0.5");
        modsecAssay->addRequestHeader((char *) "Accept-Encoding",
            (char *) "gzip,deflate");
        modsecAssay->addRequestHeader((char *) "Accept-Charset",
            (char *) "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
        modsecAssay->addRequestHeader((char *) "Keep-Alive",
            (char *) "300");
        modsecAssay->addRequestHeader((char *) "Connection",
            (char *) "keep-alive");
        modsecAssay->addRequestHeader((char *) "Cookie",
            (char *) "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120");
        modsecAssay->addRequestHeader((char *) "Pragma",
            (char *) "no-cache");
        modsecAssay->addRequestHeader((char *) "Cache-Control",
            (char *) "no-cache");
        modsecAssay->processRequestHeaders();

        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }


        modsecAssay->processRequestBody();

        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        modsecAssay->addResponseHeader((char *) "HTTP/1.1",
            (char *) "200 OK");
        modsecAssay->addResponseHeader((char *) "Content-Type",
            (char *) "text/xml; charset=utf-8");
        modsecAssay->addResponseHeader((char *) "Content-Length",
            (char *) "200");

        modsecAssay->processResponseHeaders();

        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }


        modsecAssay->appendResponseBody(response_body,
            strlen((const char*)response_body));
        modsecAssay->processResponseBody();

        if (modsecAssay->intervention()) {
            std::cout << "There is an intervention" << std::endl;
            continue;
        }

        delete modsecAssay;
    }

    delete modsec;
}
