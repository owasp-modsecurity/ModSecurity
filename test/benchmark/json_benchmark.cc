/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2024 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "config.h"

#include <array>
#include <sys/resource.h>

#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "src/request_body_processor/json_instrumentation.h"

#ifndef MSC_JSON_BENCHMARK_RULES_DIR
#error "MSC_JSON_BENCHMARK_RULES_DIR must be defined by the build system."
#endif

namespace {

using Clock = std::chrono::steady_clock;

constexpr std::size_t kDefaultTargetBytes = 1048576;
constexpr std::size_t kDefaultDepth = 512;
constexpr unsigned long long kDefaultIterations = 100;
constexpr const char *kRulesFileName = "json_benchmark_rules.conf";
constexpr const char *kClientIp = "198.51.100.10";  // RFC 5737 documentation range
constexpr const char *kServerIp = "198.51.100.20";  // RFC 5737 documentation range

struct Options {
    std::string scenario;
    unsigned long long iterations{kDefaultIterations};
    std::size_t target_bytes{kDefaultTargetBytes};
    std::size_t depth{kDefaultDepth};
    bool include_invalid{false};
    bool output_json{false};
};

struct Metrics {
    std::uint64_t append_request_body_ns{0};
    std::uint64_t process_request_body_ns{0};
    std::uint64_t total_transaction_ns{0};
    unsigned long long parse_success_count{0};
    unsigned long long parse_error_count{0};
};

class JsonBenchmarkError : public std::runtime_error {
 public:
    explicit JsonBenchmarkError(const std::string &message)
        : std::runtime_error(message) { }
};

const char *const usage_message =
    "Usage: json_benchmark --scenario NAME [--iterations N] "
    "[--target-bytes N] [--depth N] [--include-invalid] [--output json]";

std::string benchmarkBackend() {
#if defined(MSC_JSON_BACKEND_SIMDJSON)
    return "simdjson";
#elif defined(MSC_JSON_BACKEND_JSONCONS)
    return "jsoncons";
#else
#error "A JSON backend must be selected at build time."
#endif
}

std::string rulesFilePath() {
    return std::string(MSC_JSON_BENCHMARK_RULES_DIR) + "/" + kRulesFileName;
}

std::uint64_t elapsedNanos(Clock::time_point start_time) {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            Clock::now() - start_time).count());
}

unsigned long long parseUnsignedLongLong(const char *value,
    const char *flag_name, bool allow_zero) {
    errno = 0;
    char *end = nullptr;
    const unsigned long long parsed = std::strtoull(value, &end, 10);
    if (errno != 0 || end == value || *end != '\0'
            || (!allow_zero && parsed == 0)) {
        throw JsonBenchmarkError(std::string("invalid numeric value for ")
            + flag_name + ": " + value);
    }
    return parsed;
}

std::size_t parseSize(const char *value, const char *flag_name) {
    const unsigned long long parsed =
        parseUnsignedLongLong(value, flag_name, true);
    if (parsed > std::numeric_limits<std::size_t>::max()) {
        throw JsonBenchmarkError(std::string("value too large for ")
            + flag_name + ": " + value);
    }
    return static_cast<std::size_t>(parsed);
}

unsigned long long parseIterations(const char *value) {
    return parseUnsignedLongLong(value, "--iterations", false);
}

const char *requireOptionValue(int argc, const char *argv[], int *index,
    const char *option_name) {
    if (*index + 1 >= argc) {
        throw JsonBenchmarkError(std::string("missing value for ")
            + option_name);
    }
    *index += 1;
    return argv[*index];
}

Options parseOptions(int argc, const char *argv[]) {
    Options options;

    int i = 1;
    while (i < argc) {
        const std::string current(argv[i]);
        if (current == "-h" || current == "-?" || current == "--help") {
            std::cout << usage_message << std::endl;
            std::exit(0);
        } else if (current == "--scenario") {
            options.scenario.assign(
                requireOptionValue(argc, argv, &i, "--scenario"));
        } else if (current == "--iterations") {
            options.iterations = parseIterations(
                requireOptionValue(argc, argv, &i, "--iterations"));
        } else if (current == "--target-bytes") {
            options.target_bytes = parseSize(
                requireOptionValue(argc, argv, &i, "--target-bytes"),
                "--target-bytes");
        } else if (current == "--depth") {
            options.depth = parseSize(
                requireOptionValue(argc, argv, &i, "--depth"), "--depth");
        } else if (current == "--include-invalid") {
            options.include_invalid = true;
        } else if (current == "--output") {
            if (const std::string output_format(
                    requireOptionValue(argc, argv, &i, "--output"));
                output_format != "json") {
                throw JsonBenchmarkError("unsupported output format: "
                    + output_format);
            }
            options.output_json = true;
        } else {
            throw JsonBenchmarkError("unknown option: " + current);
        }
        i++;
    }

    if (options.scenario.empty()) {
        throw JsonBenchmarkError("missing required --scenario");
    }

    if (const bool is_invalid_scenario = options.scenario == "truncated"
            || options.scenario == "malformed";
        is_invalid_scenario && !options.include_invalid) {
        throw JsonBenchmarkError(
            "invalid JSON scenarios require --include-invalid");
    }

    return options;
}

std::string makeLargeObject(std::size_t target_bytes) {
    std::string body("{");
    std::size_t index = 0;

    while (body.size() + 32 < target_bytes || index == 0) {
        if (index > 0) {
            body.push_back(',');
        }
        body += "\"key";
        body += std::to_string(index);
        body += "\":\"value";
        body += std::to_string(index);
        body += "\"";
        index++;
    }

    body.push_back('}');
    return body;
}

std::string makeUtf8Object(std::size_t target_bytes) {
    static const std::array<const char *, 5> utf8_values{{
        u8"Gr\u00fc\u00dfe",
        u8"\u3053\u3093\u306b\u3061\u306f",
        u8"\u043f\u0440\u0438\u0432\u0435\u0442",
        u8"\u0645\u0631\u062d\u0628\u0627",
        u8"\U0001F30D"
    }};

    std::string body("{");
    std::size_t index = 0;

    while (body.size() + 48 < target_bytes || index == 0) {
        if (index > 0) {
            body.push_back(',');
        }
        body += "\"utf8_";
        body += std::to_string(index);
        body += "\":\"";
        body += utf8_values[index % utf8_values.size()];
        body += "\"";
        index++;
    }

    body.push_back('}');
    return body;
}

std::string makeNumbersArray(std::size_t target_bytes) {
    static const std::array<const char *, 8> numeric_tokens{{
        "0",
        "-0",
        "1.0",
        "1e3",
        "-1.25e-4",
        "123456789012345678901234567890",
        "6.02214076e23",
        "3.141592653589793238462643383279"
    }};

    std::string body("[");
    std::size_t index = 0;

    while (body.size() + 40 < target_bytes || index == 0) {
        if (index > 0) {
            body.push_back(',');
        }
        body += numeric_tokens[index % numeric_tokens.size()];
        index++;
    }

    body.push_back(']');
    return body;
}

std::string makeDeepNesting(std::size_t depth) {
    std::string body;

    for (std::size_t i = 0; i < depth; i++) {
        if ((i % 2) == 0) {
            body += "{\"k\":";
        } else {
            body.push_back('[');
        }
    }

    body += "\"leaf\"";

    for (std::size_t i = depth; i > 0; i--) {
        if (((i - 1) % 2) == 0) {
            body.push_back('}');
        } else {
            body.push_back(']');
        }
    }

    return body;
}

std::string buildScenarioBody(const Options &options) {
    if (options.scenario == "large-object") {
        return makeLargeObject(options.target_bytes);
    }
    if (options.scenario == "deep-nesting") {
        return makeDeepNesting(options.depth);
    }
    if (options.scenario == "numbers") {
        return makeNumbersArray(options.target_bytes);
    }
    if (options.scenario == "utf8") {
        return makeUtf8Object(options.target_bytes);
    }
    if (options.scenario == "truncated") {
        std::string body = makeLargeObject(options.target_bytes);
        body.pop_back();
        return body;
    }
    if (options.scenario == "malformed") {
        std::string body = makeLargeObject(options.target_bytes);
        body.insert(body.size() - 1, ",]");
        return body;
    }

    throw JsonBenchmarkError("unsupported scenario: " + options.scenario);
}

bool isResolvedZero(const std::unique_ptr<std::string> &value) {
    return value && *value == "0";
}

Metrics runBenchmark(modsecurity::ModSecurity *modsec,
    modsecurity::RulesSet *rules, const std::string &body,
    const Options &options) {
    Metrics metrics;

    for (unsigned long long iteration = 0; iteration < options.iterations;
        iteration++) {
        const auto total_start = Clock::now();
        modsecurity::Transaction transaction(modsec, rules, nullptr);
        transaction.processConnection(kClientIp, 12345, kServerIp, 80);
        transaction.processURI("/json-benchmark", "POST", "1.1");
        transaction.addRequestHeader("Host", "localhost");
        transaction.addRequestHeader("User-Agent",
            "ModSecurity-json-benchmark/1.0");
        transaction.addRequestHeader("Content-Type", "application/json");
        const std::string content_length = std::to_string(body.size());
        transaction.addRequestHeader("Content-Length", content_length);
        transaction.processRequestHeaders();

        const auto append_start = Clock::now();
        const int append_ok = transaction.appendRequestBody(
            reinterpret_cast<const unsigned char *>(body.data()), body.size());
        metrics.append_request_body_ns += elapsedNanos(append_start);
        if (append_ok == 0) {
            throw JsonBenchmarkError(
                "appendRequestBody reported partial body processing");
        }

        const auto process_start = Clock::now();
        if (!transaction.processRequestBody()) {
            throw JsonBenchmarkError("processRequestBody returned false");
        }
        metrics.process_request_body_ns += elapsedNanos(process_start);
        metrics.total_transaction_ns += elapsedNanos(total_start);

        const std::unique_ptr<std::string> reqbody_error =
            transaction.m_variableReqbodyError.resolveFirst();
        const std::unique_ptr<std::string> processor_error =
            transaction.m_variableReqbodyProcessorError.resolveFirst();

        if (!reqbody_error || !processor_error) {
            throw JsonBenchmarkError(
                "unable to resolve JSON parse outcome variables");
        }

        const bool parse_success = isResolvedZero(reqbody_error)
            && isResolvedZero(processor_error);
        if (const bool parse_error = !isResolvedZero(reqbody_error)
                || !isResolvedZero(processor_error);
            parse_success == parse_error) {
            throw JsonBenchmarkError(
                "ambiguous JSON parse outcome observed in benchmark");
        }

        if (parse_success) {
            metrics.parse_success_count++;
        } else {
            metrics.parse_error_count++;
        }
    }

    return metrics;
}

long currentMaxRssKb() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        return -1;
    }
    return usage.ru_maxrss;
}

void printJsonStringField(const char *name, const std::string &value,
    bool &first) {
    if (!first) {
        std::cout << ",";
    }
    std::cout << "\"" << name << "\":\"" << value << "\"";
    first = false;
}

template <typename T>
void printJsonNumericField(const char *name, T value, bool &first) {
    if (!first) {
        std::cout << ",";
    }
    std::cout << "\"" << name << "\":" << value;
    first = false;
}

void printHumanField(const char *name, const std::string &value) {
    std::cout << name << ": " << value << "\n";
}

template <typename T>
void printHumanField(const char *name, T value) {
    std::cout << name << ": " << value << "\n";
}

void printJson(const Options &options, const std::string &body,
    const Metrics &metrics) {
    bool first = true;

    std::cout << "{";
    printJsonStringField("backend", benchmarkBackend(), first);
    printJsonStringField("scenario", options.scenario, first);
    printJsonNumericField("iterations", options.iterations, first);
    printJsonNumericField("body_bytes", body.size(), first);
    printJsonNumericField("append_request_body_ns",
        metrics.append_request_body_ns, first);
    printJsonNumericField("process_request_body_ns",
        metrics.process_request_body_ns, first);
    printJsonNumericField("total_transaction_ns",
        metrics.total_transaction_ns, first);
    printJsonNumericField("parse_success_count",
        metrics.parse_success_count, first);
    printJsonNumericField("parse_error_count",
        metrics.parse_error_count, first);
    printJsonNumericField("ru_maxrss_kb", currentMaxRssKb(), first);

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const modsecurity::RequestBodyProcessor::JsonInstrumentationMetrics
        instrumentation =
            modsecurity::RequestBodyProcessor::jsonInstrumentationSnapshot();

    printJsonNumericField("request_body_snapshot_count",
        instrumentation.request_body_snapshot_count, first);
    printJsonNumericField("request_body_snapshot_bytes",
        instrumentation.request_body_snapshot_bytes, first);
    printJsonNumericField("request_body_snapshot_ns",
        instrumentation.request_body_snapshot_ns, first);
    printJsonNumericField("json_process_chunk_calls",
        instrumentation.json_process_chunk_calls, first);
    printJsonNumericField("json_process_chunk_appended_bytes",
        instrumentation.json_process_chunk_appended_bytes, first);
    printJsonNumericField("json_process_chunk_ns",
        instrumentation.json_process_chunk_ns, first);
    printJsonNumericField("simdjson_parser_constructions",
        instrumentation.simdjson_parser_constructions, first);
    printJsonNumericField("simdjson_parser_construction_ns",
        instrumentation.simdjson_parser_construction_ns, first);
    printJsonNumericField("simdjson_padded_copy_bytes",
        instrumentation.simdjson_padded_copy_bytes, first);
    printJsonNumericField("simdjson_padded_copy_ns",
        instrumentation.simdjson_padded_copy_ns, first);
    printJsonNumericField("simdjson_iterate_ns",
        instrumentation.simdjson_iterate_ns, first);
    printJsonNumericField("jsoncons_cursor_constructions",
        instrumentation.jsoncons_cursor_constructions, first);
    printJsonNumericField("jsoncons_cursor_init_ns",
        instrumentation.jsoncons_cursor_init_ns, first);
    printJsonNumericField("jsoncons_token_cursor_constructions",
        instrumentation.jsoncons_token_cursor_constructions, first);
    printJsonNumericField("jsoncons_token_cursor_init_ns",
        instrumentation.jsoncons_token_cursor_init_ns, first);
    printJsonNumericField("jsoncons_event_loop_ns",
        instrumentation.jsoncons_event_loop_ns, first);
    printJsonNumericField("jsoncons_token_sync_steps",
        instrumentation.jsoncons_token_sync_steps, first);
    printJsonNumericField("jsoncons_token_exact_advance_steps",
        instrumentation.jsoncons_token_exact_advance_steps, first);
#endif

    std::cout << "}" << std::endl;
}

void printHumanReadable(const Options &options, const std::string &body,
    const Metrics &metrics) {
    printHumanField("backend", benchmarkBackend());
    printHumanField("scenario", options.scenario);
    printHumanField("iterations", options.iterations);
    printHumanField("body_bytes", body.size());
    printHumanField("append_request_body_ns",
        metrics.append_request_body_ns);
    printHumanField("process_request_body_ns",
        metrics.process_request_body_ns);
    printHumanField("total_transaction_ns",
        metrics.total_transaction_ns);
    printHumanField("parse_success_count",
        metrics.parse_success_count);
    printHumanField("parse_error_count",
        metrics.parse_error_count);
    printHumanField("ru_maxrss_kb", currentMaxRssKb());
}

}  // namespace

int main(int argc, const char *argv[]) {
    try {
        const Options options = parseOptions(argc, argv);
        const std::string body = buildScenarioBody(options);

        modsecurity::ModSecurity modsec;
        modsec.setConnectorInformation(
            "ModSecurity-json-benchmark v0.0.1-alpha");

        modsecurity::RulesSet rules;
        if (const std::string rules_path = rulesFilePath();
            rules.loadFromUri(rules_path.c_str()) < 0) {
            std::cerr << "failed to load benchmark rules from "
                << rules_path << std::endl;
            std::cerr << rules.m_parserError.str() << std::endl;
            return 1;
        }

        modsecurity::RequestBodyProcessor::jsonInstrumentationReset();
        const Metrics metrics = runBenchmark(&modsec, &rules, body, options);

        if (options.output_json) {
            printJson(options, body, metrics);
        } else {
            printHumanReadable(options, body, metrics);
        }
        return 0;
    } catch (const std::exception &error) {
        std::cerr << error.what() << std::endl;
        std::cerr << usage_message << std::endl;
        return 64;
    }
}
