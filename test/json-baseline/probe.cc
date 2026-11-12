// Copyright 2026 ModSecurity contributors.
// Test-only byte-preserving observer for the unchanged libModSecurity API.
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifdef WITH_YAJL
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "src/request_body_processor/json.h"
#include "intervention_guard.h"
#include "probe_support.h"

namespace {
using json_test::hex;
using json_test::unhex;
using json_test::ProbeError;

std::string line() {
    std::string text;
    if (!std::getline(std::cin, text)) {
        throw ProbeError("missing protocol line");
    }
    return text;
}

void value(const char *name, const std::string &text) {
    std::cout << name << '\t' << hex(text) << '\n';
}

// resolve() exposes owning raw pointers. This guard owns the entire collection,
// even if resolution or later stream output throws before every value is read.
class ResolvedValues {
 public:
    ResolvedValues() = default;
    ResolvedValues(const ResolvedValues &) = delete;
    ResolvedValues &operator=(const ResolvedValues &) = delete;
    ~ResolvedValues() {
        for (const auto *entry : entries) {
            const std::unique_ptr<const modsecurity::VariableValue> owned(
                entry);
        }
    }
    std::vector<const modsecurity::VariableValue *> entries;
};

template <class Variable>
void dump(const char *name, Variable *variable) {
    ResolvedValues values;
    variable->resolve(&values.entries);
    for (const auto *entry : values.entries) {
        std::cout << name << '\t' << hex(entry->getKey()) << '\t'
                  << hex(entry->getValue()) << '\n';
    }
}

struct Request {
    std::string mode;
    std::string rules;
    std::string body;
    std::string uri;
    std::string contentType;
    std::string chunks;
    std::string secondBody;
};

Request readRequest() {
    Request request;
    request.mode = line();
    request.rules = unhex(line());
    request.body = unhex(line());
    request.uri = unhex(line());
    request.contentType = unhex(line());
    request.chunks = line();
    request.secondBody = request.mode == "sequence"
        ? unhex(line()) : R"({"ok":[1,2]})";
    return request;
}

bool jsonAvailable(modsecurity::ModSecurity *engine) {
    modsecurity::RulesSet rules;
    if (rules.load("SecRuleEngine On\nSecRequestBodyAccess On\n"
        "SecAction \"id:99,phase:1,pass,nolog,"
        "ctl:requestBodyProcessor=JSON\"\n") < 0) {
        throw ProbeError("JSON canary rules rejected");
    }
    modsecurity::Transaction tx(engine, &rules, nullptr);
    tx.processConnection("127.0.0.1", 1, "127.0.0.1", 80);
    tx.processURI("/", "POST", "1.1");
    tx.addRequestHeader("Content-Type", "application/json");
    tx.processRequestHeaders();
    const std::string sentinel = R"({"canary":"active"})";
    tx.appendRequestBody(
        reinterpret_cast<const unsigned char *>(sentinel.data()),
        sentinel.size());
    tx.processRequestBody();
    const auto result = tx.m_variableArgs.resolveFirst("json.canary");
    return result && *result == "active"
        && tx.m_variableReqbodyError.m_value == "0";
}

void prepareTransaction(modsecurity::Transaction *tx, const Request &request,
    const std::string &body) {
    tx->processConnection("127.0.0.1", 12345, "127.0.0.1", 80);
    tx->processURI(request.uri.c_str(), "POST", "1.1");
    tx->addRequestHeader("Host", "baseline.invalid");
    if (!request.contentType.empty()) {
        tx->addRequestHeader("Content-Type", request.contentType);
    }
    tx->addRequestHeader("Content-Length", std::to_string(body.size()));
    tx->processRequestHeaders();
}

void checkChunk(size_t size, size_t position, size_t length) {
    if (position > length || size > length - position) {
        throw ProbeError("oversized chunk");
    }
}

void processParser(modsecurity::Transaction *tx,
    const modsecurity::RulesSet &rules, const Request &request,
    const std::string &body) {
    modsecurity::RequestBodyProcessor::JSON parser(tx);
    if (rules.m_requestBodyJsonDepthLimit.m_set) {
        parser.setMaxDepth(rules.m_requestBodyJsonDepthLimit.m_value);
    }
    std::string error;
    bool accepted = true;
    size_t position = 0;
    size_t size = 0;
    std::istringstream chunks(request.chunks);
    while (chunks >> size) {
        checkChunk(size, position, body.size());
        accepted = parser.processChunk(body.data() + position, size, &error);
        std::cout << "CHUNK\t" << accepted << '\n';
        position += size;
        if (!accepted) {
            break;
        }
    }
    if (accepted && position < body.size()) {
        const bool step = parser.processChunk(body.data() + position,
            body.size() - position, &error);
        std::cout << "CHUNK\t" << step << '\n';
    }
    dump("PRE_ARGS", &tx->m_variableArgs);
    std::cout << "COMPLETE\t" << parser.complete(&error) << '\n';
    value("PARSER_ERROR", error);
}

void appendBody(modsecurity::Transaction *tx, const Request &request,
    const std::string &body) {
    size_t position = 0;
    size_t size = 0;
    std::istringstream chunks(request.chunks);
    while (chunks >> size) {
        checkChunk(size, position, body.size());
        std::cout << "APPEND\t" << tx->appendRequestBody(
            reinterpret_cast<const unsigned char *>(body.data() + position),
            size) << '\n';
        position += size;
    }
    if (position < body.size() || body.empty()) {
        std::cout << "APPEND\t" << tx->appendRequestBody(
            reinterpret_cast<const unsigned char *>(body.data() + position),
            body.size() - position) << '\n';
    }
}

void processBody(modsecurity::Transaction *tx, const Request &request,
    const std::string &body) {
    if (request.mode != "absent") {
        appendBody(tx, request, body);
    }
    {
        json_test::InterventionGuard intervention;
        tx->intervention(&intervention.value);
        std::cout << "APPEND_INTERVENTION\t"
                  << intervention.value.status << '\n';
    }
    std::cout << "PROCESS\t" << tx->processRequestBody() << '\n';
}

void dumpTransaction(modsecurity::Transaction *tx) {
    dump("NAMES", &tx->m_variableArgsNames);
    dump("ARGS", &tx->m_variableArgs);
    dump("GET", &tx->m_variableArgsGet);
    dump("POST", &tx->m_variableArgsPost);
    value("ERROR", tx->m_variableReqbodyError.m_value);
    value("PROCESSOR_ERROR", tx->m_variableReqbodyProcessorError.m_value);
    value("ERROR_MSG", tx->m_variableReqbodyErrorMsg.m_value);
    value("PROCESSOR_ERROR_MSG",
        tx->m_variableReqbodyProcessorErrorMsg.m_value);
    value("PROCESSOR", tx->m_variableReqbodyProcessor.m_value);
    value("INBOUND", tx->m_variableInboundDataError.m_value);
    value("REQUEST_BODY", tx->m_variableRequestBody.m_value);
    const auto phase = tx->m_collections.m_tx_collection->resolveFirst(
        "phase2");
    value("PHASE2", phase ? *phase : "");
    const auto matched = tx->m_collections.m_tx_collection->resolveFirst(
        "value_seen");
    value("VALUE_SEEN", matched ? *matched : "");
    json_test::InterventionGuard intervention;
    const bool interrupted = tx->intervention(&intervention.value);
    std::cout << "INTERVENTION\t" << interrupted << '\t'
              << intervention.value.status << '\n';
}

int run(const Request &request) {
    modsecurity::ModSecurity engine;
    std::cout << "VERSION\t" << hex(engine.whoAmI()) << '\n';
    if (!jsonAvailable(&engine)) {
        std::cerr << "BLOCKED: active library JSON canary failed\n";
        return 77;
    }
    std::cout << "YAJL_ACTIVE\t1\n";
    modsecurity::RulesSet rules;
    const int loaded = rules.load(request.rules.c_str());
    std::cout << "CONFIG\t" << (loaded >= 0) << '\n';
    if (loaded < 0) {
        value("CONFIG_ERROR", rules.getParserError());
        return 0;
    }
    const bool secondBody = request.mode == "isolation"
        || request.mode == "sequence";
    const int count = secondBody || request.mode == "repeat" ? 2 : 1;
    for (int index = 0; index < count; ++index) {
        const auto &body = secondBody && index == 1
            ? request.secondBody : request.body;
        modsecurity::Transaction tx(&engine, &rules, nullptr);
        prepareTransaction(&tx, request, body);
        std::cout << "TRANSACTION\t" << index << '\n';
        if (request.mode == "parser") {
            processParser(&tx, rules, request, body);
        } else {
            processBody(&tx, request, body);
        }
        dumpTransaction(&tx);
    }
    return 0;
}
}  // namespace
#endif

int main() {
#ifndef WITH_YAJL
    std::cerr << "BLOCKED: JSON support was not enabled\n";
    return 77;
#else
    try {
        return run(readRequest());
    } catch (const std::exception &error) {
        std::cerr << "HARNESS ERROR: " << error.what() << '\n';
        return 2;
    }
#endif
}
