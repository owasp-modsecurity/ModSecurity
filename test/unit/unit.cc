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
#include "operators/operator.h"

#include "common/modsecurity_test.h"
#include "common/modsecurity_test_results.h"
#include "common/colors.h"
#include "unit/unit_test.h"


using modsecurity_test::UnitTest;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;

std::string default_test_path = "test-cases/secrules-language-tests/operators";

void print_help() {
#ifdef HAS_GETOPT
    std::cout << "Use ./unit  [--no-color]  -t /path/to/test/files ";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "  -h\t\tThis help message" << std::endl;
    std::cout << "  -v\t\tVerbose" << std::endl;
    std::cout << "  -c\t\tNo color" << std::endl;
    std::cout << "  -t\t\tPath to test cases" << std::endl;
#else
    std::cout << "Use ./unit /path/to/file" << std::endl;
#endif
    std::cout << std::endl;
    std::cout << std::endl;
}


void perform_unit_test(UnitTest *t, ModSecurityTestResults<UnitTest>* res) {
    const char *error = NULL;
    ModSecurity::operators::Operator *op =
        ModSecurity::operators::Operator::instantiate("\"@" + t->name + \
            " " + t->param + "\"");
    op->init(&error);

    int ret = op->evaluate(NULL, t->input);
    if (ret != t->ret) {
        t->obtained = ret;
        res->push_back(t);
    }
}


int main(int argc, char **argv) {
    int total = 0;
    ModSecurityTest<UnitTest> test;
    ModSecurityTestResults<UnitTest> results;

    test.cmd_options(argc, argv);
    std::cout << test.header();

    test.load_tests();

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *tests = a.second;

        total += tests->size();
        for (UnitTest *t : *tests) {
            ModSecurityTestResults<UnitTest> r;

            std::cout << "  " << a.first << "...\t";

            perform_unit_test(t, &r);

            if (r.size() == 0) {
                std::cout << KGRN << r.size() << " tests failed.";
            } else {
                std::cout << KRED << r.size() << " tests failed.";
            }
            std::cout << RESET << std::endl;

            results.insert(results.end(), r.begin(), r.end());
        }
    }
    std::cout << "Total >> "  << total << std::endl;

    for (UnitTest *t : results) {
        std::cout << t->print() << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Ran a total of: " << total << " unit tests - ";
    if (results.size() == 0) {
        std::cout << KGRN << "All tests passed" << RESET << std::endl;
    } else {
        std::cout << KRED << results.size() << " failed." << RESET << std::endl;
    }
}


