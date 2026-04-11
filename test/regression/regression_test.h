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

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <memory>

#include "test/common/json.h"

#ifndef TEST_REGRESSION_REGRESSION_TEST_H_
#define TEST_REGRESSION_REGRESSION_TEST_H_

namespace modsecurity_test {


class RegressionTest {
 public:
    static std::unique_ptr<RegressionTest> from_json_document(
        modsecurity_test::json::JsonDocument *document);
    static std::unique_ptr<RegressionTest> from_json_value(
        modsecurity_test::json::JsonValue value);

    static std::string print();
    std::string filename;
    std::string name;
    std::string title;

    std::string rules;

    std::string url;
    int enabled;
    int version_min;
    std::optional<int> version_max;
    std::optional<int> github_issue;

    std::vector<std::pair<std::string, std::string>> request_headers;
    std::vector<std::pair<std::string, std::string>> response_headers;
    std::string request_body;
    std::string response_body;
    std::string response_protocol;

    std::string audit_log;
    std::string debug_log;
    std::string error_log;
    std::string parser_error;

    std::string clientIp;
    std::string serverIp;
    int clientPort;
    int serverPort;
    std::string hostname;

    std::string method;
    std::string httpVersion;
    std::string uri;
    std::string resource;

    int http_code;
    std::string redirect_url;

    // fields for formatting JSON

    std::vector<std::string> request_body_lines;
    std::vector<std::string> response_body_lines;
    std::vector<std::string> rules_lines;
    void update_content_lengths();

private:
   void update_client_from_json_value(modsecurity_test::json::JsonValue value);
   void update_server_from_json_value(modsecurity_test::json::JsonValue value);
   void update_request_from_json_value(modsecurity_test::json::JsonValue value);
   void update_response_from_json_value(modsecurity_test::json::JsonValue value);
   void update_expected_from_json_value(modsecurity_test::json::JsonValue value);
   void update_rules_from_json_value(modsecurity_test::json::JsonValue value);
};

class RegressionTests {
 public:
    static std::unique_ptr<RegressionTests> from_json_document(
        modsecurity_test::json::JsonDocument *document);
    static std::unique_ptr<RegressionTests> from_json_value(
        modsecurity_test::json::JsonValue value);
    void update_content_lengths();
    std::string toJSON() const;

    std::string filename;
    std::string name;

    std::vector<std::unique_ptr<RegressionTest>> tests;
};

class RegressionTestResult {
 public:
    RegressionTestResult() :
      passed(false),
      skipped(false),
      disabled(false),
      test(NULL) { }

    bool passed;
    bool skipped;
    bool disabled;
    RegressionTest *test;
    std::stringstream reason;
};


}  // namespace modsecurity_test

#endif  // TEST_REGRESSION_REGRESSION_TEST_H_
