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

#include "common/modsecurity_test.h"
#include "regression/regression_test.h"
#include "common/modsecurity_test_results.h"
#include "regression/custom_debug_log.h"

using modsecurity_test::CustomDebugLog;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;
using modsecurity_test::RegressionTest;

std::string default_test_path = "test-cases/regression";

void print_help() {
    std::cout << "Use ./regression-tests /path/to/file" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


void actions(ModSecurityTestResults<RegressionTest> *r,
    ModSecurity::ModSecurityIntervention *it) {
    if (it != NULL) {
        if (it->pause != 0) {
            // FIXME:
        }
        if (it->status != 0) {
            r->status = it->status;
        }
        if (it->url != NULL) {
            r->location = it->url;
        }
    }
}


void perform_unit_test(std::vector<RegressionTest *> *tests,
    ModSecurityTestResults<RegressionTest> *res) {
    ModSecurity::ModSecurity *modsec;
    ModSecurity::Rules *modsec_rules;
    ModSecurity::Assay *modsec_assay;

    CustomDebugLog *debug_log = new CustomDebugLog();

    for (RegressionTest *t : *tests) {
        ModSecurityTestResults<RegressionTest> r;
        r.status = 200;

        modsec = new ModSecurity::ModSecurity();
        modsec_rules = new ModSecurity::Rules(debug_log);

        if (modsec_rules->load((char *)(t->rules.c_str())) == false) {
            std::cerr << "Problems parsing the rules, aborting!" << std::endl;
            return;
        }
        modsec_assay = new ModSecurity::Assay(modsec, modsec_rules);

        if (t->ip.empty() == false) {
            // FIXME: no cast please.
            modsec_assay->processConnection((char *)(t->ip.c_str()));
            actions(&r, modsec_assay->intervention());
            if (r.status != 200) {
                goto end;
            }
        }
        if (t->uri.empty() == false) {
            modsec_assay->processURI((char *)(t->uri.c_str()));
            actions(&r, modsec_assay->intervention());
            if (r.status != 200) {
                goto end;
            }
        }

        for (std::pair<std::string, std::string> headers :
            t->request_headers) {
            modsec_assay->addRequestHeader((char *)headers.first.c_str(),
                (char *)headers.second.c_str());
        }

        modsec_assay->processRequestHeaders();
        actions(&r, modsec_assay->intervention());
        if (r.status != 200) {
            goto end;
        }

        modsec_assay->appendRequestBody(
            (unsigned char *)t->request_body.c_str(),
            t->request_body.size());
        modsec_assay->processRequestBody();
        actions(&r, modsec_assay->intervention());
        if (r.status != 200) {
            goto end;
        }

        for (std::pair<std::string, std::string> headers :
            t->response_headers) {
            modsec_assay->addResponseHeader((char *)headers.first.c_str(),
            (char *)headers.second.c_str());
        }

        modsec_assay->processResponseHeaders();
        actions(&r, modsec_assay->intervention());
        if (r.status != 200) {
            goto end;
        }

        modsec_assay->appendResponseBody(
            (unsigned char *)t->response_body.c_str(),
            t->response_body.size());
        modsec_assay->processResponseBody();
        actions(&r, modsec_assay->intervention());
        if (r.status != 200) {
            goto end;
        }

        modsec_assay->processLogging();

end:
        CustomDebugLog *d = reinterpret_cast<CustomDebugLog *>
            (modsec_rules->debug_log);

        std::cout << "- " << t->name << " ...";

        if (!d->contains(t->debug_log)) {
            std::cout << "Debug log was not matching the expected results.";
            std::cout << std::endl;
        } else if (r.status != t->http_code) {
            std::cout << "HTTP code mismatch. expecting: " + \
                std::to_string(t->http_code) + \
                " got: " + std::to_string(r.status) + "\n";
        } else {
            std::cout << "passed!" << std::endl;
            goto after_debug_log;
        }

        std::cout << "Debug log:" << std::endl;
        std::cout << "" << d->log_messages() << "" <<std::endl;
after_debug_log:

        res->log_raw_debug_log = d->log_messages();

        delete modsec_assay;
        delete modsec_rules;
        delete modsec;

        res->insert(res->end(), r.begin(), r.end());
    }
}


int main(int argc, char **argv) {
    ModSecurityTest<RegressionTest> test;
    ModSecurityTestResults<RegressionTest> results;

    test.cmd_options(argc, argv);
    std::cout << test.header();

    test.load_tests();

    std::ofstream test_log;
    test_log.open("regression_test_log.txt");

    for (std::pair<std::string, std::vector<RegressionTest *> *> a : test) {
        std::vector<RegressionTest *> *tests = a.second;
        ModSecurityTestResults<RegressionTest> res;

        perform_unit_test(tests, &res);

        test_log << res.log_raw_debug_log;
    }

    test_log.close();

    return 0;
}



