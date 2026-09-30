/*
 * Template for creating custom ModSecurity benchmarks
 *
 * Copy this file and modify it for your specific benchmarking needs.
 *
 * Build with:
 *   ./build_with_benchmark.sh
 *
 * Or manually:
 *   g++ -std=c++17 -O3 -I../../headers benchmark_template.cc \
 *       -L../../src/.libs -lmodsecurity -lbenchmark -lpthread \
 *       -o my_benchmark
 */

#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "modsec_fixture.h"

using modsecurity::ModSecurity;
using modsecurity::RulesSet;
using modsecurity::Transaction;

// ============================================================================
// CUSTOMIZE YOUR TEST DATA HERE
// ============================================================================

// Example: Custom headers for your specific use case
static const std::vector<std::pair<std::string, std::string>> MY_HEADERS = {
    {"Host", "api.myapp.com"},
    {"User-Agent", "MyApp/1.0"},
    {"Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"},
    {"Content-Type", "application/json"},
    {"X-Request-ID", "req-12345"},
    {"X-API-Key", "your-api-key-here"},
    // Add more headers as needed
};

// Example: Custom URI with query parameters
static const char* MY_URI = "/api/v1/users?limit=10&offset=0&sort=name";

// Example: Request body for POST/PUT tests
static const char* MY_REQUEST_BODY = R"({
    "username": "testuser",
    "email": "test@example.com",
    "age": 25,
    "tags": ["tag1", "tag2"]
})";

// ============================================================================
// EXAMPLE 1: Benchmark testing a specific header
// ============================================================================

static void BM_MyCustomHeader(benchmark::State& state) {
    ModSecFixture fixture;

    // Define your rule to test
    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off

        # Test if X-API-Key header contains specific pattern
        SecRule REQUEST_HEADERS:X-API-Key "@rx ^[a-zA-Z0-9-]+$" \
            "id:9001,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();

        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        // Setup transaction
        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");

        // Add your custom headers
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }

        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_MyCustomHeader);

// ============================================================================
// EXAMPLE 2: Benchmark comparing different operators
// ============================================================================

static void BM_OperatorContains(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:Authorization "@contains Bearer" \
            "id:9002,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_OperatorContains);

static void BM_OperatorRx(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:Authorization "@rx ^Bearer\s+[A-Za-z0-9\-_]+$" \
            "id:9003,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_OperatorRx);

// ============================================================================
// EXAMPLE 3: Parameterized benchmark testing variable header counts
// ============================================================================

static void BM_HeaderCount(benchmark::State& state) {
    ModSecFixture fixture;
    int num_headers = state.range(0);

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS "@contains test" \
            "id:9004,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");

        // Add N headers dynamically
        for (int i = 0; i < num_headers; i++) {
            std::string name = "X-Custom-" + std::to_string(i);
            std::string value = "test-value-" + std::to_string(i);
            trans->addRequestHeader(name, value);
        }

        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }

    // Report additional context
    state.SetLabel(std::to_string(num_headers) + " headers");
}
// Test with 1, 5, 10, 25, 50, 100 headers
BENCHMARK(BM_HeaderCount)->Arg(1)->Arg(5)->Arg(10)->Arg(25)->Arg(50)->Arg(100);

// ============================================================================
// EXAMPLE 4: Benchmark with request body processing
// ============================================================================

static void BM_JSONBody(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess On
        SecResponseBodyAccess Off

        # Enable JSON body processor
        SecRule REQUEST_HEADERS:Content-Type "^application/json" \
            "id:9005,phase:1,pass,nolog,ctl:requestBodyProcessor=JSON"

        # Check JSON body content
        SecRule ARGS:username "@contains test" \
            "id:9006,phase:2,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "POST", "HTTP/1.1");

        trans->addRequestHeader("Host", "api.myapp.com");
        trans->addRequestHeader("Content-Type", "application/json");
        trans->addRequestHeader("Content-Length", std::to_string(strlen(MY_REQUEST_BODY)));

        trans->processRequestHeaders();

        // Process JSON body
        trans->appendRequestBody(
            reinterpret_cast<const unsigned char*>(MY_REQUEST_BODY),
            strlen(MY_REQUEST_BODY)
        );
        trans->processRequestBody();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_JSONBody);

// ============================================================================
// EXAMPLE 5: Benchmark testing multiple rules in sequence
// ============================================================================

static void BM_MultipleRules(benchmark::State& state) {
    ModSecFixture fixture;
    int num_rules = state.range(0);

    // Generate multiple similar rules
    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
    )";

    for (int i = 0; i < num_rules; i++) {
        rules += "SecRule REQUEST_HEADERS:User-Agent \"@contains Mozilla\" ";
        rules += "\"id:" + std::to_string(9100 + i) + ",phase:1,pass,nolog\"\n";
    }

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }

    state.SetLabel(std::to_string(num_rules) + " rules");
}
BENCHMARK(BM_MultipleRules)->Range(1, 100);  // Test 1, 8, 64, 100 rules

// ============================================================================
// EXAMPLE 6: Benchmark comparing transformation performance
// ============================================================================

static void BM_NoTransformation(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains mozilla" \
            "id:9200,phase:1,pass,nolog,t:none"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_NoTransformation);

static void BM_WithLowercase(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@contains mozilla" \
            "id:9201,phase:1,pass,nolog,t:lowercase"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        modsecurity::ModSecurityIntervention intervention;
        modsecurity::intervention::clean(&intervention);

        trans->processConnection("203.0.113.45", 54321, "192.0.2.1", 443);
        trans->processURI(MY_URI, "GET", "HTTP/1.1");
        for (const auto& header : MY_HEADERS) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
        modsecurity::intervention::free(&intervention);
    }
}
BENCHMARK(BM_WithLowercase);

// ============================================================================
// MAIN - Required for Google Benchmark
// ============================================================================

BENCHMARK_MAIN();
