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

#include <yajl/yajl_tree.h>

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <utility>

#include "test/common/colors.h"

#ifndef TEST_REGRESSION_REGRESSION_TEST_H_
#define TEST_REGRESSION_REGRESSION_TEST_H_

namespace modsecurity_test {


class RegressionTest {
 public:
    static RegressionTest *from_yajl_node(const yajl_val &);

    static std::string print();
    std::string filename;
    std::string name;
    std::string title;

    std::string rules;

    std::string url;
    int enabled;
    int version_min;
    int version_max;
    int github_issue;

    std::vector<std::pair<std::string, std::string>> request_headers;
    std::vector<std::pair<std::string, std::string>> response_headers;
    std::string request_body;
    std::string response_body;
    std::string response_protocol;

    std::string audit_log;
    std::string debug_log;
    std::string error_log;
    std::string parser_error;
    std::string parser_warn;

    std::string clientIp;
    std::string serverIp;
    int clientPort;
    int serverPort;

    std::string method;
    std::string httpVersion;
    std::string uri;
    std::string resource;

    static inline std::string yajl_array_to_str(const yajl_val &node);
    static inline std::vector<std::string> yajl_array_to_vec_str(
        const yajl_val &node);
    static inline std::vector<std::pair<std::string, std::string>>
        yajl_array_to_map(const yajl_val &node);

    int http_code;
    std::string redirect_url;
};


class RegressionTestResult {
 public:
    enum TestStatus {
        PENDING = 2,
        SKIPPED = 4,
        DISABLED = 8,
        PASSED = 16,
        FAILED = 32
    };

    explicit RegressionTestResult(RegressionTest *t) :
      m_status(PENDING),
      m_test(t)
    {
        if (t->enabled == 0) {
            m_status = DISABLED;
        }
    }


    std::string getTestResources() const {
        return m_test->resource;
    }

    std::string getName() const {
        return m_test->name;
    }

    std::string getRules() const {
        return m_test->rules;
    }

    std::string getExpectedParserError() const {
        return m_test->parser_error;
    }

    std::string getFileName() const {
        size_t offset = m_test->filename.find_last_of("/\\");
        std::string filename("");
        if (offset != std::string::npos) {
            filename = std::string(m_test->filename, offset + 1,
                m_test->filename.length() - offset - 1);
        } else {
            filename = m_test->filename;
        }

        return filename;
    }

    void failed(const std::string &reason) {
        m_status = FAILED;
        this->reason << reason;
    }

    void passed() {
        m_status = PASSED;
    }

    void skipped(const std::string &reason) {
        m_status = SKIPPED;
        this->reason << reason;
    }

    void print(bool automake = false) const {
        std::string statusAutoMake;
        std::string statusConsole;
        std::string statusColor;

        if (m_status == RegressionTestResult::TestStatus::PENDING) {
            return;
        }

        if (m_status == RegressionTestResult::TestStatus::SKIPPED) {
            statusAutoMake = "SKIP";
            statusConsole = "skipped";
            statusColor = KCYN;
        }

        if (m_status == RegressionTestResult::TestStatus::DISABLED) {
            statusAutoMake = "SKIP";
            statusConsole = "disabled";
            statusColor = KCYN;
        }

        if (m_status == RegressionTestResult::TestStatus::PASSED) {
            statusAutoMake = "PASS";
            statusConsole = "passed!";
            statusColor = KGRN;
        }

        if (m_status == RegressionTestResult::TestStatus::FAILED) {
            statusAutoMake = "FAILED";
            statusConsole = "failed!";
            statusColor = KRED;
        }

        if (automake) {
            std::cout << ":test-result: " << statusAutoMake \
                << " " << getFileName();
                //<< ":" << getName();
            std::cout << std::endl;
            return;
        }

        std::cout << statusColor << statusConsole << RESET << std::endl;
        std::cout << reason.str() << std::endl;
    }



    std::stringstream reason;
    TestStatus m_status;
 private:
    RegressionTest *m_test;
};


}  // namespace modsecurity_test

#endif  // TEST_REGRESSION_REGRESSION_TEST_H_
