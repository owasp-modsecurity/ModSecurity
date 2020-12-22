/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <unistd.h>

#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

#include "modsecurity/rules_set.h"
#include "modsecurity/modsecurity.h"
#include "test/common/modsecurity_test.h"
#include "test/common/colors.h"
#include "test/regression/regression_test.h"
#include "test/common/modsecurity_test_results.h"
#include "test/regression/custom_debug_log.h"
#include "src/utils/regex.h"

using modsecurity_test::CustomDebugLog;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;
using modsecurity_test::RegressionTest;
using modsecurity_test::RegressionTestResult;

using modsecurity::Utils::regex_search;
using modsecurity::Utils::SMatch;
using modsecurity::Utils::Regex;

std::string default_test_path = "test-cases/regression";
std::list<std::string> resources;

void print_help()
{
    std::cout << "Use ./regression-tests /path/to/file" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


bool contains(const std::string &s, const std::string &pattern)
{
    bool ret;
    modsecurity::Utils::Regex re(pattern);
    ret = modsecurity::Utils::regex_search(s, re);
    return ret;
}


void clearAuditLog(const std::string &filename)
{
    if (!filename.empty()) {
        std::ifstream file;
        file.open(filename.c_str(), std::ifstream::out | std::ifstream::trunc);
        if (!file.is_open() || file.fail()) {
            std::cout << std::endl << "Failed to clear previous contents of audit log: " \
                << filename << std::endl;
        }
        file.close();
    }
}


std::string getAuditLogContent(const std::string &filename)
{
    std::stringstream buffer;
    if (!filename.empty()) {
        try {
           std::ifstream t(filename);
           buffer << t.rdbuf();
        } catch (...) {
            std::cout << "Failed to read file:" << filename << std::endl;
        }
    }
    return buffer.str();
}


void actions(ModSecurityTestResults<RegressionTest> *r,
    modsecurity::Transaction *a,
    std::stringstream *serverLog)
{
    modsecurity::ModSecurityIntervention it;
    memset(&it, '\0', sizeof(modsecurity::ModSecurityIntervention));
    it.status = 200;
    if (a->intervention(&it) == true) {
        if (it.pause != 0) {
            // FIXME:
        }
        if (it.status != 0) {
            r->status = it.status;
        }
        if (it.url != NULL) {
            r->location.append(it.url);
	    free(it.url);
	    it.url = NULL;
        }
        if (it.log != NULL) {
            *serverLog << it.log;
            free(it.log);
            it.log = NULL;
        }
    }
}


void logCb(void *data, const void *msgv) {
    const char *msg = reinterpret_cast<const char*>(msgv);
    std::stringstream *ss = (std::stringstream *) data;
    *ss << msg << std::endl;
}

void printTestHeader(bool automake,
    int count,
    RegressionTestResult *r) {

    if (automake) {
        return;
    }

    std::cout << std::setw(3) << std::right << std::to_string(count) << " ";
    std::cout << std::setw(50) << std::left << r->getFileName();
    std::cout << std::setw(70) << std::left << r->getName();
}


void testCleanUp() {
    #ifdef WITH_LMDB
    // some tests (e.g. issue-1831.json)  don't like it when data persists between runs
    unlink("./modsec-shared-collections");
    unlink("./modsec-shared-collections-lock");
    #endif
}


modsecurity::ModSecurity *setupModSec() {
    auto modsec = new modsecurity::ModSecurity();
    modsec->setConnectorInformation("ModSecurity-regression v0.0.1-alpha" \
        " (ModSecurity regression test utility)");
    modsec->setServerLogCb(logCb);
    return modsec;
}


modsecurity::RulesSet *setupModSecRules(RegressionTestResult *r, std::string &warning) {
    CustomDebugLog *debug_log = new CustomDebugLog();
    auto rules = new modsecurity::RulesSet(debug_log);
    rules->load("SecDebugLogLevel 9");


    if (rules->load(r->getRules().c_str(), r->getFileName()) >= 0 &&
        r->getExpectedParserError().empty()) {
        warning.assign(rules->getParserWarnings());
        return rules;
    }
    warning.assign(rules->getParserWarnings());


    if (!r->getExpectedParserError().empty()) {
        Regex re(r->getExpectedParserError());
        SMatch match;
        auto s = rules->getParserError();
        if (regex_search(s, &match, re)) {
            r->passed();
            return nullptr;
        }
    }

    /*
     * Not expecting any error, thus return the error to
     * the user.
     */
    std::stringstream reason;
    reason << KRED << "parse failed." << RESET << std::endl;
    reason << KWHT << "Expected: " << RESET << r->getExpectedParserError() << std::endl;
    reason << KWHT << "Produced: " << RESET << rules->getParserError() << std::endl;
    r->failed(reason.str());
    return nullptr;
}


bool isResourceAvailable(RegressionTestResult *r) {
    bool found = true;
    auto res = r->getTestResources();
    if (res.empty() == false) {
        found = (std::find(resources.begin(), resources.end(), res)
            != resources.end());
    }

    return found;
}


void processRequest(
    RegressionTest *t,
    modsecurity::ModSecurity *modsec,
    modsecurity::RulesSet *rules,
    std::string &error_log,
    std::string &audit_log,
    std::string &debug_log,
    int *status_code) {

    ModSecurityTestResults<RegressionTest> r;
    r.status = 200;
    std::stringstream serverLog;
    modsecurity::Transaction *modsec_transaction = NULL;

    modsec_transaction = new modsecurity::Transaction(modsec, rules,
        &serverLog);

    std::string auditLogFile(modsec_transaction->m_rules->m_auditLog->m_path1);

    clearAuditLog(auditLogFile);

    /* connection */
    modsec_transaction->processConnection(t->clientIp.c_str(),
        t->clientPort, t->serverIp.c_str(), t->serverPort);
    actions(&r, modsec_transaction, &serverLog);

    /* uri */
    modsec_transaction->processURI(t->uri.c_str(), t->method.c_str(),
        t->httpVersion.c_str());
    actions(&r, modsec_transaction, &serverLog);

    /* request headers */
    for (std::pair<std::string, std::string> headers :
        t->request_headers) {
        modsec_transaction->addRequestHeader(headers.first.c_str(),
            headers.second.c_str());
    }
    modsec_transaction->processRequestHeaders();
    actions(&r, modsec_transaction, &serverLog);

    /* request body */
    modsec_transaction->appendRequestBody(
        (unsigned char *)t->request_body.c_str(),
        t->request_body.size());
    modsec_transaction->processRequestBody();
    actions(&r, modsec_transaction, &serverLog);

    /* response headers */
    for (std::pair<std::string, std::string> headers :
        t->response_headers) {
        modsec_transaction->addResponseHeader(headers.first.c_str(),
            headers.second.c_str());
    }
    modsec_transaction->processResponseHeaders(r.status,
        t->response_protocol);
    actions(&r, modsec_transaction, &serverLog);

    /* response body */
    modsec_transaction->appendResponseBody(
        (unsigned char *)t->response_body.c_str(),
        t->response_body.size());
    modsec_transaction->processResponseBody();
    actions(&r, modsec_transaction, &serverLog);

    /* logging */
    modsec_transaction->processLogging();

    /* collect all logging */
    audit_log.assign(getAuditLogContent(auditLogFile));
    clearAuditLog(auditLogFile);

    error_log.assign(serverLog.str());
    CustomDebugLog *d = reinterpret_cast<CustomDebugLog *>(rules->m_debugLog);
    debug_log.assign(d->log_messages());

    *status_code = r.status;
}


void processLogs(RegressionTest *t,
    RegressionTestResult *testRes,
    const std::string &serverLog,
    const std::string &audit_log,
    const std::string &debug_log,
    const std::string &parser_warning,
    int status_code) {


    if (!contains(debug_log, t->debug_log)) {
        std::stringstream reason;
        reason << "Debug log was not matching the " \
            << "expected results." << std::endl;
        reason << KWHT << "Expecting: " << RESET \
            << t->debug_log + "";
        testRes->failed(reason.str());
    } else if (status_code != t->http_code) {
        std::stringstream reason;
        reason << "HTTP code mismatch. expecting: " + \
            std::to_string(t->http_code) + \
            " got: " + std::to_string(status_code) + "\n";
        testRes->failed(reason.str());
    } else if (!contains(serverLog, t->error_log)) {
        std::stringstream reason;
        reason << "Error log was not matching the " \
            << "expected results." << std::endl;
        reason << KWHT << "Expecting: " << RESET \
            << t->error_log + "";
        testRes->failed(reason.str());
    } else if (!t->audit_log.empty()
         && !contains(audit_log, t->audit_log)) {
        std::stringstream reason;
        reason << "Audit log was not matching the " \
            << "expected results." << std::endl;
        reason << KWHT << "Expecting: " << RESET \
            << t->audit_log + "";
        testRes->failed(reason.str());
    } else if (!contains(parser_warning, t->parser_warn)) {
        std::stringstream reason;
        reason << "Parser warning was not matching the " \
            << "expected results." << std::endl;
        reason << KWHT << "Expecting: " << RESET \
            << t->parser_warn + "";
        testRes->failed(reason.str());
    } else {
        testRes->passed();
        return;
    }

    if (testRes->m_status != RegressionTestResult::TestStatus::PASSED) {
        testRes->reason << std::endl;
        testRes->reason << KWHT << "Debug log:" << RESET << std::endl;
        testRes->reason << debug_log << std::endl;
        testRes->reason << KWHT << "Error log:" << RESET << std::endl;
        testRes->reason << serverLog << std::endl;
        testRes->reason << KWHT << "Audit log:" << RESET << std::endl;
        testRes->reason << audit_log << std::endl;
        testRes->reason << KWHT << "Parser warning:" << RESET << std::endl;
        testRes->reason << parser_warning << std::endl;
    }
}


RegressionTestResult *perform_regression_test(
        RegressionTest *t,
        bool automake)
    {
    modsecurity::ModSecurity *modsec = setupModSec();
    modsecurity::RulesSet *modsec_rules = nullptr;
    RegressionTestResult *testRes = new RegressionTestResult(t);

    std::string error_log;
    std::string audit_log;
    std::string debug_log;
    std::string parser_warning;
    int status_code = 200;

    if (t->enabled == 0) {
        goto ret;
    }
    testCleanUp();

    if (!isResourceAvailable(testRes)) {
        std::stringstream reason;
        reason << KCYN << "ModSecurity was not ";
        reason << KCYN << "compiled with support ";
        reason << KCYN << "to: " << t->resource << std::endl;
        reason << RESET << std::endl;
        testRes->skipped(reason.str());
        goto ret;
    }

    modsec_rules = setupModSecRules(testRes, parser_warning);
    if (modsec_rules == nullptr) {
        goto ret;
    }

    processRequest(t,
        modsec,
        modsec_rules,
        error_log,
        audit_log,
        debug_log,
        &status_code);

    processLogs(t,
      testRes,
      error_log,
      audit_log,
      debug_log,
      parser_warning,
      status_code);

ret:
    if (modsec_rules != nullptr)
    {
        delete modsec_rules;
    }
    delete modsec;

    return testRes;
}


int main(int argc, char **argv) {
    ModSecurityTest<RegressionTest> test;

    std::string ver(MODSECURITY_VERSION);
    std::string envvar("MODSECURITY=ModSecurity " + ver + " regression tests");

    putenv(strdup(envvar.c_str()));
#ifndef NO_LOGS
    int test_number = 0;
#endif

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
#ifdef WITH_LUA
    resources.push_back("lua");
#endif

#ifdef NO_LOGS
    std::cout << "Test utility cannot work without logging support." \
        << std::endl;
#else
    test.cmd_options(argc, argv);
    if (!test.m_automake_output && !test.m_count_all) {
        std::cout << test.header();
    }

    test.load_tests();

    if (!test.m_automake_output && !test.m_count_all) {
        std::cout << std::setw(4) << std::right << "# ";
        std::cout << std::setw(50) << std::left << "File Name";
        std::cout << std::setw(70) << std::left << "Test Name";
        std::cout << std::setw(10) << std::left << "Passed?";
        std::cout << std::endl;
        std::cout << std::setw(4) << std::right << "--- ";
        std::cout << std::setw(50) << std::left << "---------";
        std::cout << std::setw(70) << std::left << "---------";
        std::cout << std::setw(10) << std::left << "-------";
        std::cout << std::endl;
    }

    if (test.m_count_all) {
        std::list<std::string> keyList;
        for (std::pair<std::string, std::vector<RegressionTest *> *> a : test) {
            keyList.push_back(a.first);
        }
        std::cout << std::to_string(keyList.size()) << std::endl;
        exit(0);
    }

    ModSecurityTestResults<RegressionTestResult> res;

    for (auto testFiles : test) {
        auto &testsCases = *testFiles.second;
        for (RegressionTest *t : testsCases) {
            test_number++;
            if ((test.m_test_number != 0) && test_number != test.m_test_number) {
                continue;
            }

            auto result = perform_regression_test(t, test.m_automake_output);
            printTestHeader(test.m_automake_output, res.size(), result);
            result->print(test.m_automake_output);
            res.push_back(result);
        }
    }

    std::cout << std::endl;

    int passed = 0;
    int failed = 0;
    int disabled = 0;
    int skipped = 0;

    for (RegressionTestResult *r : res) {
        if (r->m_status == RegressionTestResult::TestStatus::SKIPPED) {
            skipped++;
        }
        if (r->m_status == RegressionTestResult::TestStatus::DISABLED) {
            disabled++;
        }
        if (r->m_status == RegressionTestResult::TestStatus::PASSED) {
            passed++;
        }

        if (r->m_status == RegressionTestResult::TestStatus::FAILED) {
            if (!test.m_automake_output) {
                std::cout << KRED << "Test failed." << RESET << KWHT \
                    << " From: " \
                    << RESET << r->getFileName() << "." << std::endl;
                std::cout << KWHT << "Test name: " << RESET \
                    << r->getFileName() \
                    << "." << std::endl;
                std::cout << KWHT << "Reason: " << RESET << std::endl;
                std::cout << r->reason.str() << std::endl;
            }
            failed++;
        }
        delete r;
    }

    if (!test.m_automake_output) {
        std::cout << "Ran a total of: " << std::to_string(failed + passed) \
            << " regression tests - ";
        if (failed == 0) {
            std::cout << KGRN << "All tests passed. " << RESET;
        } else {
            std::cout << KRED << failed << " failed. " << RESET;
        }

        std::cout << KCYN << std::to_string(skipped) << " ";
		std::cout << "skipped test(s). " << std::to_string(disabled) << " ";
        std::cout << "disabled test(s)." << RESET << std::endl;
    }

    for (std::pair<std::string, std::vector<RegressionTest *> *> a : test) {
        std::vector<RegressionTest *> *vec = a.second;
        for (int i = 0; i < vec->size(); i++) {
            delete vec->at(i);
        }
        delete vec;
    }

#endif
    return 0;
}
