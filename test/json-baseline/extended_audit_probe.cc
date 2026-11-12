// Copyright 2026 ModSecurity contributors.
// Synthetic v3 audit-writer/public-offset probe. No production code changes.
#include <iostream>
#include <string>
#include <string_view>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "probe_support.h"

#ifndef WITH_YAJL
#error "YAJL-enabled ModSecurity is required"
#endif

namespace {
using json_test::ProbeError;

void discard(void *, const void *) {
    // Ignore synthetic logs so stdout contains only framed JSON evidence.
}

void require(int result) {
    if (result != 1) {
        throw ProbeError("transaction API failed");
    }
}

void audit(modsecurity::ModSecurity *engine, modsecurity::RulesSet *rules,
    const std::string &value, int parts, const char *id, int repeats) {
    modsecurity::Transaction tx(engine, rules, id, nullptr);
    require(tx.processConnection("192.0.2.31", 10001, "192.0.2.32", 8081));
    tx.setRequestHostName("writer.invalid");
    require(tx.processURI(R"(/quote"back\?q=writer)", "POST", "1.1"));
    require(tx.addRequestHeader("Content-Type", "application/octet-stream"));
    require(tx.addRequestHeader("Content-Length",
        std::to_string(value.size())));
    require(tx.addRequestHeader("X-Trace", "request:" + value));
    require(tx.addRequestHeader("X-Trace", "request:second"));
    require(tx.addRequestHeader(R"(X-Name"\)", "request:name"));
    require(tx.addRequestHeader("X-Uni\xc2\xa2", "request:unicode-name"));
    require(tx.processRequestHeaders());
    require(tx.appendRequestBody(
        reinterpret_cast<const unsigned char *>(value.data()), value.size()));
    require(tx.processRequestBody());
    require(tx.addResponseHeader("Content-Type", "text/plain"));
    require(tx.addResponseHeader("X-Trace", "response:" + value));
    require(tx.addResponseHeader("X-Trace", "response:second"));
    require(tx.addResponseHeader(R"(X-Name"\)", "response:name"));
    require(tx.addResponseHeader("X-Uni\xc2\xa2", "response:unicode-name"));
    require(tx.processResponseHeaders(202, "HTTP/1.1"));
    const std::string response = "response:" + value;
    require(tx.appendResponseBody(
        reinterpret_cast<const unsigned char *>(response.data()),
        response.size()));
    require(tx.processResponseBody());
    for (int i = 0; i < repeats; ++i) {
        const std::string record = tx.toJSON(parts);
        std::cout.write(record.data(), record.size());
    }
}

void offset(int argc, char **argv) {
    if (argc != 4) {
        throw ProbeError("offset HEX MATCH");
    }
    const std::string content = json_test::unhex(argv[2]);
    std::string json;
    const char *error = nullptr;
    const int result = modsecurity::ModSecurity::processContentOffset(
        content.data(), content.size(), argv[3], &json, &error);
    std::cout << result << '\n';
    if (result == 0) {
        std::cout.write(json.data(), json.size());
    } else {
        std::cout << (error ? error : "missing error") << '\n';
    }
}

void run(int argc, char **argv) {
    if (argc < 2) {
        throw ProbeError("mode missing");
    }
    if (std::string_view(argv[1]) == "offset") {
        offset(argc, argv);
        return;
    }
    if (argc != 5 || std::string_view(argv[1]) != "audit") {
        throw ProbeError("audit HEX PART_MASK single|repeat|lifecycle");
    }
    modsecurity::ModSecurity engine;
    engine.setConnectorInformation("extended JSON writer probe");
    engine.setServerLogCb(discard);
    modsecurity::RulesSet rules;
    if (rules.load("SecRuleEngine On\nSecRequestBodyAccess On\n"
        "SecResponseBodyAccess On\nSecResponseBodyMimeType text/plain\n"
        "SecAuditEngine Off\n", "extended-writer.conf") < 0) {
        throw ProbeError(rules.getParserError());
    }
    const std::string_view mode(argv[4]);
    if (mode != "single" && mode != "repeat" && mode != "lifecycle") {
        throw ProbeError("unknown lifecycle mode");
    }
    const int mask = std::stoi(argv[3]);
    audit(&engine, &rules, json_test::unhex(argv[2]), mask, "writer-first",
        mode == "repeat" ? 3 : 1);
    if (mode == "lifecycle") {
        audit(&engine, &rules, "clean", mask, "writer-second", 1);
    }
}
}  // namespace

int main(int argc, char **argv) {
    try {
        run(argc, argv);
    } catch (const std::exception &error) {
        std::cerr << "extended probe failed: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
