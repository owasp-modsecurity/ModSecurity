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

#include <yajl/yajl_tree.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#ifndef TEST_REGRESSION_REGRESSION_TEST_H_
#define TEST_REGRESSION_REGRESSION_TEST_H_

namespace modsecurity_test {

class RegressionTest {
 public:
    static RegressionTest *from_yajl_node(const yajl_val &);

    std::string print();
    std::string filename;
    std::string name;
    std::string title;

    std::string rules;

    std::string url;
    int enabled;
    int version_min;
    int version_max;
    int github_issue;

    std::unordered_map<std::string, std::string> request_headers;
    std::unordered_map<std::string, std::string> response_headers;
    std::string request_body;
    std::string response_body;

    std::string audit_log;
    std::string debug_log;
    std::string error_log;

    std::string clientIp;
    std::string serverIp;
    int clientPort;
    int serverPort;

    std::string uri;

    static inline std::string yajl_array_to_str(const yajl_val &node);
    static inline std::vector<std::string> yajl_array_to_vec_str(
        const yajl_val &node);
    static inline std::unordered_map<std::string,
        std::string> yajl_array_to_map(const yajl_val &node);

    int http_code;
    std::string redirect_url;
};

}  // namespace modsecurity_test

#endif  // TEST_REGRESSION_REGRESSION_TEST_H_
