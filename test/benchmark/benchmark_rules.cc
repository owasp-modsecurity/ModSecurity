/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2025 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <benchmark/benchmark.h>

#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

using modsecurity::ModSecurity;
using modsecurity::RulesSet;
using modsecurity::Transaction;

// Static test data - realistic HTTP headers
static const std::vector<std::pair<std::string, std::string>> TEST_HEADERS = {
    {"Host", "www.example.com"},
    {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36"},
    {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
    {"Accept-Language", "en-US,en;q=0.5"},
    {"Accept-Encoding", "gzip, deflate, br"},
    {"Connection", "keep-alive"},
    {"Upgrade-Insecure-Requests", "1"},
    {"Cache-Control", "max-age=0"},
    {"Cookie", "sessionid=abc123; token=xyz789"},
    {"Content-Type", "application/x-www-form-urlencoded"},
    {"Content-Length", "42"},
    {"X-Forwarded-For", "192.168.1.100"},
    {"X-Real-IP", "203.0.113.45"},
    {"Referer", "https://www.example.com/previous-page"},
    {"Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"}
};

static const char* TEST_URI = "/api/users?id=123&name=test&action=update";
static const char* TEST_CLIENT_IP = "203.0.113.45";
static const char* TEST_SERVER_IP = "192.0.2.1";

// Helper class to manage ModSecurity lifecycle
class ModSecFixture {
public:
    ModSecurity* modsec;
    RulesSet* rules;

    ModSecFixture() {
        modsec = new ModSecurity();
        modsec->setConnectorInformation("ModSecurity-benchmark v1.0");
        rules = new RulesSet();
    }

    ~ModSecFixture() {
        delete rules;
        delete modsec;
    }

    bool loadRules(const std::string& ruleConfig) {
        // Create a temporary rules object for this configuration
        RulesSet* tempRules = new RulesSet();
        int result = tempRules->load(ruleConfig.c_str());

        if (result < 0) {
            delete tempRules;
            return false;
        }

        // Replace old rules
        delete rules;
        rules = tempRules;
        return true;
    }

    Transaction* createTransaction() {
        return new Transaction(modsec, rules, nullptr);
    }
};

// Helper to run a complete transaction
static void runTransaction(Transaction* trans) {
    modsecurity::ModSecurityIntervention intervention;
    modsecurity::intervention::clean(&intervention);

    // Connection phase
    trans->processConnection(TEST_CLIENT_IP, 54321, TEST_SERVER_IP, 443);
    if (trans->intervention(&intervention)) {
        modsecurity::intervention::free(&intervention);
        return;
    }

    // URI phase
    trans->processURI(TEST_URI, "POST", "HTTP/1.1");
    if (trans->intervention(&intervention)) {
        modsecurity::intervention::free(&intervention);
        return;
    }

    // Request headers phase
    for (const auto& header : TEST_HEADERS) {
        trans->addRequestHeader(header.first, header.second);
    }
    trans->processRequestHeaders();
    if (trans->intervention(&intervention)) {
        modsecurity::intervention::free(&intervention);
        return;
    }

    // Request body phase (no body for this benchmark)
    trans->processRequestBody();

    modsecurity::intervention::free(&intervention);
}

// Benchmark: Rule targeting a single header (REQUEST_HEADERS:User-Agent)
static void BM_SingleHeader(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains Mozilla" \
            "id:1001,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_SingleHeader);

// Benchmark: Rule targeting all request headers (REQUEST_HEADERS)
static void BM_AllHeaders(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS "@contains Mozilla" \
            "id:1002,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_AllHeaders);

// Benchmark: Rule targeting headers with regex key matching
static void BM_HeadersRegexKey(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:/^X-/ "@contains 192" \
            "id:1003,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_HeadersRegexKey);

// Benchmark: Rule targeting REQUEST_HEADERS_NAMES
static void BM_HeaderNames(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS_NAMES "@contains User-Agent" \
            "id:1004,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_HeaderNames);

// Benchmark: Multiple rules targeting different headers
static void BM_MultipleHeaderRules(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains Mozilla" \
            "id:1005,phase:1,pass,nolog,t:none"
        SecRule REQUEST_HEADERS:Host "@contains example" \
            "id:1006,phase:1,pass,nolog,t:none"
        SecRule REQUEST_HEADERS:Cookie "@contains session" \
            "id:1007,phase:1,pass,nolog,t:none"
        SecRule REQUEST_HEADERS:Accept "@rx ^text/html" \
            "id:1008,phase:1,pass,nolog,t:none"
        SecRule REQUEST_HEADERS:Content-Type "@contains application" \
            "id:1009,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_MultipleHeaderRules);

// Benchmark: Rule with @rx operator on single header
static void BM_RegexSingleHeader(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@rx Mozilla/[0-9]+\.[0-9]+" \
            "id:1010,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_RegexSingleHeader);

// Benchmark: Rule with @rx operator on all headers
static void BM_RegexAllHeaders(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS "@rx Mozilla/[0-9]+\.[0-9]+" \
            "id:1011,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_RegexAllHeaders);

// Benchmark: Rule with transformation on single header
static void BM_TransformationSingleHeader(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains mozilla" \
            "id:1012,phase:1,pass,nolog,t:lowercase"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_TransformationSingleHeader);

// Benchmark: Rule with transformation on all headers
static void BM_TransformationAllHeaders(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS "@contains mozilla" \
            "id:1013,phase:1,pass,nolog,t:lowercase"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_TransformationAllHeaders);

// Benchmark: Rule with multiple transformations
static void BM_MultipleTransformations(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains mozilla" \
            "id:1014,phase:1,pass,nolog,t:lowercase,t:compressWhitespace,t:trim"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_MultipleTransformations);

// Benchmark: No rules (baseline overhead)
static void BM_NoRules(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_NoRules);

// Benchmark: Rule targeting ARGS (query parameters)
static void BM_Args(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule ARGS "@contains test" \
            "id:1015,phase:2,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_Args);

// Benchmark: Rule targeting specific ARG
static void BM_SingleArg(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule ARGS:name "@contains test" \
            "id:1016,phase:2,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_SingleArg);

BENCHMARK_MAIN();
