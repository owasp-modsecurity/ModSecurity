/* Synthetic v3 audit-writer/public-offset probe. No production code changes. */
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#ifndef WITH_YAJL
#error "YAJL-enabled ModSecurity is required"
#endif
namespace {
void discard(void *, const void *) { }
void require(int rc) { if (rc != 1) throw std::runtime_error("transaction API failed"); }
std::string bytes(const std::string &hex) {
    if (hex.size() % 2) throw std::runtime_error("odd hex length");
    std::string value;
    for (size_t i = 0; i < hex.size(); i += 2) {
        size_t used = 0;
        unsigned long c = std::stoul(hex.substr(i, 2), &used, 16);
        if (used != 2) throw std::runtime_error("invalid hex");
        value.push_back(static_cast<char>(c));
    }
    return value;
}
void audit(modsecurity::ModSecurity &engine, modsecurity::RulesSet &rules,
    const std::string &value, int parts, const char *id, int repeats) {
    modsecurity::Transaction tx(&engine, &rules, id, nullptr);
    require(tx.processConnection("192.0.2.31", 10001, "192.0.2.32", 8081));
    tx.setRequestHostName("writer.invalid");
    require(tx.processURI("/quote\"back\\?q=writer", "POST", "1.1"));
    require(tx.addRequestHeader("Content-Type", "application/octet-stream"));
    require(tx.addRequestHeader("Content-Length", std::to_string(value.size())));
    require(tx.addRequestHeader("X-Trace", "request:" + value));
    require(tx.addRequestHeader("X-Trace", "request:second"));
    require(tx.addRequestHeader("X-Name\"\\", "request:name"));
    require(tx.addRequestHeader("X-Uni\xc2\xa2", "request:unicode-name"));
    require(tx.processRequestHeaders());
    require(tx.appendRequestBody(reinterpret_cast<const unsigned char *>(value.data()), value.size()));
    require(tx.processRequestBody());
    require(tx.addResponseHeader("Content-Type", "text/plain"));
    require(tx.addResponseHeader("X-Trace", "response:" + value));
    require(tx.addResponseHeader("X-Trace", "response:second"));
    require(tx.addResponseHeader("X-Name\"\\", "response:name"));
    require(tx.addResponseHeader("X-Uni\xc2\xa2", "response:unicode-name"));
    require(tx.processResponseHeaders(202, "HTTP/1.1"));
    const std::string response = "response:" + value;
    require(tx.appendResponseBody(reinterpret_cast<const unsigned char *>(response.data()), response.size()));
    require(tx.processResponseBody());
    for (int i = 0; i < repeats; ++i) {
        const std::string record = tx.toJSON(parts);
        std::cout.write(record.data(), record.size());
    }
}
}
int main(int argc, char **argv) {
    try {
        if (argc < 2) throw std::runtime_error("mode missing");
        if (std::string(argv[1]) == "offset") {
            if (argc != 4) throw std::runtime_error("offset HEX MATCH");
            const std::string content = bytes(argv[2]);
            std::string json;
            const char *error = nullptr;
            int rc = modsecurity::ModSecurity::processContentOffset(content.data(),
                content.size(), argv[3], &json, &error);
            std::cout << rc << '\n';
            if (rc == 0) std::cout.write(json.data(), json.size());
            else std::cout << (error ? error : "missing error") << '\n';
            return 0;
        }
        if (argc != 5 || std::string(argv[1]) != "audit")
            throw std::runtime_error("audit HEX PART_MASK single|repeat|lifecycle");
        modsecurity::ModSecurity engine;
        engine.setConnectorInformation("extended JSON writer probe");
        engine.setServerLogCb(discard);
        modsecurity::RulesSet rules;
        if (rules.load("SecRuleEngine On\nSecRequestBodyAccess On\nSecResponseBodyAccess On\n"
            "SecResponseBodyMimeType text/plain\nSecAuditEngine Off\n", "extended-writer.conf") < 0)
            throw std::runtime_error(rules.getParserError());
        const std::string mode(argv[4]);
        if (mode != "single" && mode != "repeat" && mode != "lifecycle")
            throw std::runtime_error("unknown lifecycle mode");
        const int mask = std::stoi(argv[3]);
        audit(engine, rules, bytes(argv[2]), mask, "writer-first", mode == "repeat" ? 3 : 1);
        if (mode == "lifecycle") audit(engine, rules, "clean", mask, "writer-second", 1);
    } catch (const std::exception &error) {
        std::cerr << "extended probe failed: " << error.what() << '\n';
        return 2;
    }
}
