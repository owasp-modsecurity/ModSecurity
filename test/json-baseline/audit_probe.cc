/* Synthetic audit-writer integration probe; no request-parser assertions here. */
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#ifndef WITH_YAJL
#error "Compile this probe with YAJL support enabled"
#endif

namespace {

void discardServerLog(void *, const void *) { }

void requireSuccess(int result, const char *step) {
    if (result != 1) {
        throw std::runtime_error(std::string(step) + " did not succeed");
    }
}

std::string fixture(const std::string &name) {
    if (name == "ascii") {
        return "quote\"slash\\";
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
    throw std::runtime_error("unknown fixture");
}

int parts(const std::string &selection) {
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
    throw std::runtime_error("unknown part selection");
}

}  // namespace

int main(int argc, char **argv) {
    try {
        if (argc != 5) {
            throw std::runtime_error(
                "usage: audit-probe FIXTURE all|request|none pass|deny|detect unique|duplicate");
        }
        const std::string value = fixture(argv[1]);
        const std::string mode = argv[3];
        const bool duplicate = std::string(argv[4]) == "duplicate";
        if (mode != "pass" && mode != "deny" && mode != "detect") {
            throw std::runtime_error("unknown rule mode");
        }
        if (std::string(argv[4]) != "unique" && !duplicate) {
            throw std::runtime_error("unknown header mode");
        }

        modsecurity::ModSecurity engine;
        engine.setConnectorInformation("json-baseline audit probe");
        engine.setServerLogCb(discardServerLog);
        modsecurity::RulesSet rules;
        const std::string config = std::string("SecRuleEngine ")
            + (mode == "detect" ? "DetectionOnly" : "On")
            + "\nSecRequestBodyAccess On\nSecResponseBodyAccess On\n"
              "SecResponseBodyMimeType text/plain\nSecAuditEngine Off\n"
              "SecAction \"id:990100,phase:2,log,msg:'audit writer',tag:'fixture',"
            + (mode == "pass" ? "pass" : "deny,status:418") + "\"\n";
        if (rules.load(config.c_str(), "audit-probe.conf") < 0) {
            throw std::runtime_error(rules.getParserError());
        }
        modsecurity::Transaction transaction(&engine, &rules,
            "audit-probe-id", nullptr);
        requireSuccess(transaction.processConnection("192.0.2.10", 12345,
            "192.0.2.20", 8080), "processConnection");
        transaction.setRequestHostName("json-baseline.invalid");
        requireSuccess(transaction.processURI("/audit?source=fixture", "POST",
            "1.1"), "processURI");
        requireSuccess(transaction.addRequestHeader("Content-Type",
            "application/octet-stream"), "addRequestHeader");
        requireSuccess(transaction.addRequestHeader("Content-Length",
            std::to_string(value.size())), "addRequestHeader");
        requireSuccess(transaction.addRequestHeader("X-Fixture", value),
            "addRequestHeader");
        requireSuccess(transaction.addRequestHeader("X-quote\"slash\\", "name"),
            "addRequestHeader");
        if (duplicate) {
            requireSuccess(transaction.addRequestHeader("X-Fixture", "second"),
                "addRequestHeader duplicate");
        }
        requireSuccess(transaction.processRequestHeaders(),
            "processRequestHeaders");
        requireSuccess(transaction.appendRequestBody(
            reinterpret_cast<const unsigned char *>(value.data()),
            value.size()), "appendRequestBody");
        requireSuccess(transaction.processRequestBody(), "processRequestBody");

        modsecurity::ModSecurityIntervention intervention{};
        transaction.intervention(&intervention);
        const int responseCode = intervention.disruptive ? intervention.status : 201;
        modsecurity::intervention::free(&intervention);
        requireSuccess(transaction.addResponseHeader("Content-Type", "text/plain"),
            "addResponseHeader");
        requireSuccess(transaction.addResponseHeader("X-Fixture", value),
            "addResponseHeader");
        requireSuccess(transaction.addResponseHeader("X-quote\"slash\\", "name"),
            "addResponseHeader");
        if (duplicate) {
            requireSuccess(transaction.addResponseHeader("X-Fixture", "second"),
                "addResponseHeader duplicate");
        }
        requireSuccess(transaction.processResponseHeaders(responseCode, "HTTP/1.1"),
            "processResponseHeaders");
        requireSuccess(transaction.appendResponseBody(
            reinterpret_cast<const unsigned char *>(value.data()),
            value.size()), "appendResponseBody");
        requireSuccess(transaction.processResponseBody(), "processResponseBody");
        const std::string json = transaction.toJSON(parts(argv[2]));
        std::cout.write(json.data(), json.size());
    } catch (const std::exception &error) {
        std::cerr << "audit probe failed: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
