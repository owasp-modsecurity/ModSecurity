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

#include "stdio.h"
#include "stdlib.h"

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"


char main_rule_uri[] = "basic_rules.conf";

int main (int argc, char **argv)
{
    ModSecurity *modsec = NULL;
    Assay *assay = NULL;
    Rules *rules = NULL;

    modsec = msc_init();

    msc_set_connector_info(modsec, "ModSecurity-test v0.0.1-alpha (Simple " \
        "example on how to use ModSecurity API");

    rules = msc_create_rules_set();
    msc_rules_add_file(rules, main_rule_uri);

    assay = msc_new_assay(modsec, rules);

    msc_process_connection(assay, "127.0.0.1", 12345, "127.0.0.1", 80);
    msc_process_uri(assay,
        "http://www.modsecurity.org/test?key1=value1&key2=value2&key3=value3",
        "GET", "1.1");
    msc_process_request_headers(assay);
    msc_process_request_body(assay);
    msc_process_response_headers(assay);
    msc_process_response_body(assay);

    msc_rules_cleanup(rules);
    msc_cleanup(modsec);

    return 0;
}


