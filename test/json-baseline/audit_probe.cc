// Copyright 2026 ModSecurity contributors.
// Synthetic audit-writer integration probe; no request-parser assertions here.
#include <iostream>
#include <string>
#include <string_view>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "intervention_guard.h"
#include "probe_support.h"

#ifndef WITH_YAJL
#error "Compile this probe with YAJL support enabled"
#endif

namespace {
using json_test::ProbeError;

void discardServerLog(void *, const void *) {
    // Only JSON records belong on stdout, not synthetic server-log messages.
}

void requireSuccess(int result, const char *step) {
    if (result != 1) {
        throw ProbeError(std::string(step) + " did not succeed");
    }
}

std::string fixture(std::string_view name) {
    if (name == "ascii") {
        return R"(quote"slash\)";
    }
    if (name == "controls") {
        return std::string("\0\b\t\n\f\r\x1f", 7);
    }
    if (name == "unicode") {
        return "\xc2\xa2\xe2\x82\xac\xf0\x9f\x98\x80";
    }
    if (name == "mixed") {
        return fixture("ascii") + fixture("controls") + fixture("unicode");
    }
    if (name == "empty") {
        return "";
    }
    throw ProbeError("unknown fixture");
}

int parts(std::string_view selection) {
    using modsecurity::audit_log::AuditLog;
    if (selection == "all") {
        return AuditLog::AAuditLogPart | AuditLog::BAuditLogPart
            | AuditLog::CAuditLogPart | AuditLog::EAuditLogPart
            | AuditLog::FAuditLogPart | AuditLog::HAuditLogPart;
    }
    if (selection == "request") {
        return AuditLog::BAuditLogPart | AuditLog::CAuditLogPart;
    }
    if (selection == "none") {
        return 0;
    }
    throw ProbeError("unknown part selection");
}

std::string configuration(std::string_view mode) {
    return std::string("SecRuleEngine ")
        + (mode == "detect" ? "DetectionOnly" : "On")
        + "\nSecRequestBodyAccess On\nSecResponseBodyAccess On\n"
          "SecResponseBodyMimeType text/plain\nSecAuditEngine Off\n"
          "SecAction \"id:990100,phase:2,log,msg:'audit writer',tag:'fixture',"
        + (mode == "pass" ? "pass" : "deny,status:418") + "\"\n";
}

void requestBody(modsecurity::Transaction *tx, const std::string &payload,
    bool duplicate) {
    requireSuccess(tx->processConnection("192.0.2.10", 12345,
        "192.0.2.20", 8080), "processConnection");
    tx->setRequestHostName("json-baseline.invalid");
    requireSuccess(tx->processURI("/audit?source=fixture", "POST", "1.1"),
        "processURI");
    requireSuccess(tx->addRequestHeader("Content-Type",
        "application/octet-stream"), "addRequestHeader");
    requireSuccess(tx->addRequestHeader("Content-Length",
        std::to_string(payload.size())), "addRequestHeader");
    requireSuccess(tx->addRequestHeader("X-Fixture", payload),
        "addRequestHeader");
    requireSuccess(tx->addRequestHeader(R"(X-quote"slash\)", "name"),
        "addRequestHeader");
    if (duplicate) {
        requireSuccess(tx->addRequestHeader("X-Fixture", "second"),
            "addRequestHeader duplicate");
    }
    requireSuccess(tx->processRequestHeaders(), "processRequestHeaders");
    requireSuccess(tx->appendRequestBody(
        reinterpret_cast<const unsigned char *>(payload.data()),
        payload.size()), "appendRequestBody");
    requireSuccess(tx->processRequestBody(), "processRequestBody");
}

void responseBody(modsecurity::Transaction *tx, const std::string &payload,
    bool duplicate) {
    int responseCode = 201;
    {
        json_test::InterventionGuard intervention;
        tx->intervention(&intervention.value);
        if (intervention.value.disruptive) {
            responseCode = intervention.value.status;
        }
    }
    requireSuccess(tx->addResponseHeader("Content-Type", "text/plain"),
        "addResponseHeader");
    requireSuccess(tx->addResponseHeader("X-Fixture", payload),
        "addResponseHeader");
    requireSuccess(tx->addResponseHeader(R"(X-quote"slash\)", "name"),
        "addResponseHeader");
    if (duplicate) {
        requireSuccess(tx->addResponseHeader("X-Fixture", "second"),
            "addResponseHeader duplicate");
    }
    requireSuccess(tx->processResponseHeaders(responseCode, "HTTP/1.1"),
        "processResponseHeaders");
    requireSuccess(tx->appendResponseBody(
        reinterpret_cast<const unsigned char *>(payload.data()),
        payload.size()), "appendResponseBody");
    requireSuccess(tx->processResponseBody(), "processResponseBody");
}

void run(int argc, char **argv) {
    if (argc != 5) {
        throw ProbeError("usage: audit-probe FIXTURE "
            "all|request|none pass|deny|detect unique|duplicate");
    }
    const std::string payload = fixture(argv[1]);
    const std::string_view mode = argv[3];
    const std::string_view headers = argv[4];
    const bool duplicate = headers == "duplicate";
    if (mode != "pass" && mode != "deny" && mode != "detect") {
        throw ProbeError("unknown rule mode");
    }
    if (headers != "unique" && !duplicate) {
        throw ProbeError("unknown header mode");
    }
    modsecurity::ModSecurity engine;
    engine.setConnectorInformation("json-baseline audit probe");
    engine.setServerLogCb(discardServerLog);
    modsecurity::RulesSet rules;
    if (const std::string config = configuration(mode);
        rules.load(config.c_str(), "audit-probe.conf") < 0) {
        throw ProbeError(rules.getParserError());
    }
    modsecurity::Transaction tx(&engine, &rules, "audit-probe-id", nullptr);
    requestBody(&tx, payload, duplicate);
    responseBody(&tx, payload, duplicate);
    const std::string json = tx.toJSON(parts(argv[2]));
    std::cout.write(json.data(), json.size());
}
}  // namespace

int main(int argc, char **argv) {
    try {
        run(argc, argv);
    } catch (const std::exception &error) {
        std::cerr << "audit probe failed: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
