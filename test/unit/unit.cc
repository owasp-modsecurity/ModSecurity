/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <string.h>
#include <thread>

#include "modsecurity/rules_set.h"
#include "modsecurity/modsecurity.h"
#include "src/operators/operator.h"
#include "src/actions/transformations/transformation.h"
#include "modsecurity/transaction.h"
#include "modsecurity/actions/action.h"


#include "test/common/modsecurity_test.h"
#include "test/common/modsecurity_test_results.h"
#include "test/common/colors.h"
#include "test/unit/unit_test.h"
#include "src/utils/string.h"




using modsecurity_test::UnitTest;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;
using modsecurity::actions::transformations::Transformation;
using modsecurity::operators::Operator;

std::string default_test_path = "test-cases/secrules-language-tests/operators";
static std::list<std::string> resources;

void print_help() {
    std::cout << "Use ./unit /path/to/file" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


void perform_unit_test(ModSecurityTest<UnitTest> *test, UnitTest *t,
    ModSecurityTestResults<UnitTest>* res) {
    std::string error;
    bool found = true;

    if (test->m_automake_output) {
        std::cout << ":test-result: ";
    }

    if (t->resource.empty() == false) {
        found = (std::find(resources.begin(), resources.end(), t->resource)
            != resources.end());
    }

    if (!found) {
        t->skipped = true;
        res->push_back(t);
        if (test->m_automake_output) {
            std::cout << "SKIP ";
        }
    }

    if (t->type == "op") {
        Operator *op = Operator::instantiate(t->name, t->param);
        op->init(t->filename, &error);
        int ret = op->evaluate(NULL, NULL, t->input, NULL);
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
            << modsecurity::utils::string::toHexIfNeeded(t->input)
            << std::endl;
    }
}

struct thread_data {
    bool *done;
    bool *success;
    std::string *err;
};

void spawn_rule_loading_thread(thread_data d) {
    modsecurity::RulesSet *r = new modsecurity::RulesSet();
    auto rules = R"(SecAction "id:900000,phase:1,pass,nolog,setvar:tx.paranoia_level=1")";
    *(d.success) = r->load(rules, "") == 1;
    if (!*d.success) {
        *d.err = r->getParserError();
    }
    delete r;
    *(d.done) = true;
}

std::vector<thread_data> spawn_threads(
    unsigned number_of_threads,
    std::chrono::milliseconds delay) {

    auto threads = std::vector<thread_data>();

    for (int i = 0; i < number_of_threads; i++) {
        threads.push_back({new bool(false), new bool(false), new std::string("")});
        std::thread (spawn_rule_loading_thread, threads.back()).detach();

        if (delay > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(delay);
        }
    }

    return threads;
}

bool threads_finished(std::vector<thread_data> threads) {
    for (auto t: threads) {
        if (!*(t.done)) {
            return false;
        }
    }
    return true;
}

bool threads_succeeded(std::vector<thread_data> threads) {
    for (auto t: threads) {
        if (!*(t.success)) {
            return false;
        }
    }
    return true;
}

void assert_threads(std::string test_name, std::vector<thread_data> threads) {
    std::string result = threads_succeeded(threads) ? "succeeded" : "failed";
    std::cout << "Test: '" << test_name << "' " << result << ".\n";
    if (!threads_succeeded(threads)) {
        for (int i = 0; i < threads.size(); i++) {
            std::cout << "thread [" << i << "] returned: '" << *(threads[i].err) << "'\n";
        }
    }
}

void clean_up(std::vector<thread_data> threads) {
    for (auto t: threads) {
        delete t.done;
        delete t.success;
        delete t.err;
    }
}

bool wait_for_threads(
    std::string test_name,
    std::vector<thread_data> threads,
    std::chrono::seconds timeout) {
    
    auto start = std::chrono::system_clock::now();
    while (true) {
        if (threads_finished(threads)) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (std::chrono::system_clock::now() - start > timeout) {
            std::cout << "Test: '" << test_name << "' reached timeout. Failed.\n";
            return false;
        }
    }
    return true;
}

void test_1_overlaping_thread() {
    std::string test_name = "Test 1 thread";

    auto threads = spawn_threads(1, std::chrono::milliseconds(0));

    auto finished = wait_for_threads(test_name, threads, std::chrono::seconds(1));

    if (finished) {
        assert_threads(test_name, threads);
    }
    clean_up(threads);

}

void test_3_overlaping_threads() {
    std::string test_name = "Test 3 overlapping load rule threads";

    auto threads = spawn_threads(3, std::chrono::milliseconds(0));

    auto finished = wait_for_threads(test_name, threads, std::chrono::seconds(1));

    if (finished) {
        assert_threads(test_name, threads);
    }
    clean_up(threads);
}

void test_3_non_overlaping_threads() {
    std::string test_name = "Test 3 non overlapping load rule threads (delay between)";

    auto threads = spawn_threads(3, std::chrono::milliseconds(100));

    auto finished = wait_for_threads(test_name, threads, std::chrono::seconds(1));

    if (finished) {
        assert_threads(test_name, threads);
    }
    clean_up(threads);
}

int main(int argc, char **argv) {
    int total = 0;
    ModSecurityTest<UnitTest> test;
    ModSecurityTestResults<UnitTest> results;

#if defined(WITH_GEOIP) or defined(WITH_MAXMIND)
    resources.push_back("geoip-or-maxmind");
#endif

#if defined(WITH_MAXMIND)
    resources.push_back("maxmind");
#endif

#if defined(WITH_GEOIP)
    resources.push_back("geoip");
#endif

#ifdef WITH_CURL
    resources.push_back("curl");
#endif
#ifdef WITH_SSDEEP
    resources.push_back("ssdeep");
#endif


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
                int skp = 0;
                if (r.size() == 0) {
                    std::cout << KGRN << "0 tests failed.";
                } else {
                    for (auto &i : r) {
                        if (i->skipped == true) {
                            skp++;
                        }
                    }
                    std::cout << KRED << r.size()-skp << " tests failed.";
                }
                std::cout << RESET;
                if (skp > 0) {
                    std::cout << " " << std::to_string(skp) << " ";
                    std::cout << "skipped.";
                }
                std::cout << std::endl;
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
            int skp = 0;
            for (auto &i : results) {
                if (i->skipped == true) {
                    skp++;
                }
            }
            std::cout << KRED << results.size()-skp << " failed.";
            std::cout << RESET << std::endl;
            if (skp > 0) {
                std::cout << " " << std::to_string(skp) << " ";
                std::cout << "skipped.";
            }
        }
    }

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *vec = a.second;
        for (int i = 0; i < vec->size(); i++) {
            delete vec->at(i);
        }
        delete vec;
    }

    std::cout << "\n\nExecuting thread tests.\n";
    test_1_overlaping_thread();
    test_3_overlaping_threads();
    test_3_non_overlaping_threads();
}


