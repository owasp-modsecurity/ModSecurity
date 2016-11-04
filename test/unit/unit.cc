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
#include "src/operators/operator.h"
#include "src/actions/transformations/transformation.h"

#include "test/common/modsecurity_test.h"
#include "test/common/modsecurity_test_results.h"
#include "test/common/colors.h"
#include "test/unit/unit_test.h"
#include "src/utils/string.h"


using modsecurity::utils::String;
using modsecurity_test::UnitTest;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;
using modsecurity::actions::transformations::Transformation;
using modsecurity::operators::Operator;

std::string default_test_path = "test-cases/secrules-language-tests/operators";

void print_help() {
    std::cout << "Use ./unit /path/to/file" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


void perform_unit_test(ModSecurityTest<UnitTest> *test, UnitTest *t,
    ModSecurityTestResults<UnitTest>* res) {
    std::string error;

    if (test->m_automake_output) {
        std::cout << ":test-result: ";
    }

    if (t->type == "op") {
        Operator *op = Operator::instantiate("\"@" + t->name + \
                " " + t->param + "\"");
        op->init(t->filename, &error);
        int ret = op->evaluate(NULL, t->input);
        t->obtained = ret;
        if (ret != t->ret) {
            res->push_back(t);
            if (test->m_automake_output) {
                std::cout << "FAIL ";
            }
        } else if (test->m_automake_output) {
            std::cout << "PASS ";
        }
        delete op;
    } else if (t->type == "tfn") {
        Transformation *tfn = Transformation::instantiate("t:" + t->name);
        std::string ret = tfn->evaluate(t->input, NULL);
        t->obtained = 1;
        t->obtainedOutput = ret;
        if (ret != t->output) {
            res->push_back(t);
            if (test->m_automake_output) {
                std::cout << "FAIL ";
            }
        } else if (test->m_automake_output) {
            std::cout << "PASS ";
        }
        delete tfn;
    } else {
        std::cerr << "Failed. Test type is unknown: << " << t->type;
        std::cerr << std::endl;
    }

    if (test->m_automake_output) {
        std::cout << t->name << " "
            << String::toHexIfNeeded(t->input) << std::endl;
    }
}


int main(int argc, char **argv) {
    int total = 0;
    ModSecurityTest<UnitTest> test;
    ModSecurityTestResults<UnitTest> results;

    test.cmd_options(argc, argv);
    if (!test.m_automake_output) {
        std::cout << test.header();
    }

    test.load_tests();
    if (test.target == default_test_path) {
        test.load_tests("test-cases/secrules-language-tests/transformations");
    }

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *tests = a.second;

        total += tests->size();
        for (UnitTest *t : *tests) {
            ModSecurityTestResults<UnitTest> r;

            if (!test.m_automake_output) {
                std::cout << "  " << a.first << "...\t";
            }
            perform_unit_test(&test, t, &r);

            if (!test.m_automake_output) {
                if (r.size() == 0) {
                    std::cout << KGRN << r.size() << " tests failed.";
                } else {
                    std::cout << KRED << r.size() << " tests failed.";
                }
                std::cout << RESET << std::endl;
            }

            results.insert(results.end(), r.begin(), r.end());
        }
    }

    if (!test.m_automake_output) {
        std::cout << "Total >> "  << total << std::endl;
    }

    for (UnitTest *t : results) {
        std::cout << t->print() << std::endl;
    }

    if (!test.m_automake_output) {
        std::cout << std::endl;

        std::cout << "Ran a total of: " << total << " unit tests - ";
        if (results.size() == 0) {
            std::cout << KGRN << "All tests passed" << RESET << std::endl;
        } else {
            std::cout << KRED << results.size() << " failed.";
            std::cout << RESET << std::endl;
        }
    }

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *vec = a.second;
        for (int i = 0; i < vec->size(); i++) {
            delete vec->at(i);
        }
        delete vec;
    }
}


