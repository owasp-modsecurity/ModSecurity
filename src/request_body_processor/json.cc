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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/request_body_processor/json.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "src/request_body_processor/json_adapter.h"
#include "src/request_body_processor/json_instrumentation.h"


namespace modsecurity::RequestBodyProcessor {

static const double json_depth_limit_default = 10000.0;
static const char* json_depth_limit_exceeded_msg = ". Parsing depth limit exceeded";

namespace {

void assignJsonErrorMessage(std::string *err, JsonParseStatus parse_status,
    const std::string &detail) {
    if (err == nullptr) {
        return;
    }

    if (!detail.empty()) {
        err->assign(detail);
        return;
    }

    switch (parse_status) {
        case JsonParseStatus::ParseError:
            err->assign("Invalid JSON body.");
            break;
        case JsonParseStatus::TruncatedInput:
            err->assign("Incomplete JSON body.");
            break;
        case JsonParseStatus::Utf8Error:
            err->assign("Invalid UTF-8 in JSON body.");
            break;
        case JsonParseStatus::EngineAbort:
            err->assign("JSON traversal aborted by ModSecurity.");
            break;
        case JsonParseStatus::InternalError:
            err->assign("Internal JSON backend failure.");
            break;
        case JsonParseStatus::Ok:
            err->clear();
            break;
    }
}

JsonSinkStatus startContainer(
    std::deque<std::unique_ptr<JSONContainer>> *containers,
    std::unique_ptr<JSONContainer> container, int64_t *current_depth,
    double max_depth, bool *depth_limit_exceeded) {
    containers->push_back(std::move(container));
    (*current_depth)++;
    if (*current_depth > max_depth) {
        *depth_limit_exceeded = true;
        return JsonSinkStatus::DepthLimitExceeded;
    }
    return JsonSinkStatus::Continue;
}

JsonSinkStatus endContainer(
    std::deque<std::unique_ptr<JSONContainer>> *containers,
    int64_t *current_depth) {
    if (containers->empty()) {
        return JsonSinkStatus::InternalError;
    }

    containers->pop_back();

    if (!containers->empty()) {
        auto *array = dynamic_cast<JSONContainerArray *>(containers->back().get());
        if (array != nullptr) {
            array->m_elementCounter++;
        }
    }

    (*current_depth)--;
    if (*current_depth < 0) {
        *current_depth = 0;
        return JsonSinkStatus::InternalError;
    }

    return JsonSinkStatus::Continue;
}

JsonSinkStatus addArgumentAsSinkStatus(JSON *json,
    const std::string &argument_value) {
    return json->addArgument(argument_value) != 0 ? JsonSinkStatus::Continue
        : JsonSinkStatus::EngineAbort;
}

JsonSinkStatus addStringViewAsSinkStatus(JSON *json, std::string_view value) {
    return addArgumentAsSinkStatus(json, std::string(value.data(), value.size()));
}

}  // namespace

JSON::JSON(Transaction *transaction) : m_transaction(transaction),
    m_current_key(""),
    m_data(""),
    m_max_depth(json_depth_limit_default),
    m_current_depth(0),
    m_depth_limit_exceeded(false) {
}


JSON::~JSON() {
    clearContainers();
}


bool JSON::init() {
    clearContainers();
    m_current_key.clear();
    m_data.clear();
    m_current_depth = 0;
    m_depth_limit_exceeded = false;

    return true;
}


bool JSON::processChunk(const char *buf, unsigned int size,
    const std::string *err) {
    (void) err;
    if (buf != nullptr && size > 0) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
        const auto start_time = std::chrono::steady_clock::now();
        m_data.append(buf, size);
        recordJsonProcessChunkAppend(size, static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - start_time).count()));
#else
        m_data.append(buf, size);
#endif
    }

    return true;
}


bool JSON::complete(std::string *err) {
    if (m_data.empty()) {
        return true;
    }

    JSONAdapter adapter;
    if (JsonParseResult result = adapter.parse(m_data,
            static_cast<JsonEventSink *>(this)); !result.ok()) {
        if (result.sink_status == JsonSinkStatus::DepthLimitExceeded) {
            m_depth_limit_exceeded = true;
        }
        assignJsonErrorMessage(err, result.parse_status, result.detail);
        if (m_depth_limit_exceeded && err != nullptr) {
            err->append(json_depth_limit_exceeded_msg);
        }
        return false;
    }

    return true;
}


int JSON::addArgument(const std::string& value) {
    std::string data("");
    std::string path;

    for (size_t i =  0; i < m_containers.size(); i++) {
        const JSONContainerArray *a = dynamic_cast<JSONContainerArray *>(
            m_containers[i].get());
        path = path + m_containers[i]->m_name;
        if (a != nullptr) {
            path = path + ".array_" + std::to_string(a->m_elementCounter);
        } else {
            path = path + ".";
        }
    }

    if (m_containers.size() > 0) {
        JSONContainerArray *a = dynamic_cast<JSONContainerArray *>(
            m_containers.back().get());
        if (a) {
            a->m_elementCounter++;
        } else {
            data = getCurrentKey();
        }
    } else {
        data = getCurrentKey();
    }


    if (!m_transaction->addArgument("JSON", path + data, value, 0)) {
        // cancel parsing by returning false
        return 0;
    }

    return 1;
}


JsonSinkStatus JSON::on_key(std::string_view value) {
    m_current_key.assign(value.data(), value.size());
    return JsonSinkStatus::Continue;
}


JsonSinkStatus JSON::on_null() {
    return addArgumentAsSinkStatus(this, "");
}


JsonSinkStatus JSON::on_boolean(bool value) {
    return addArgumentAsSinkStatus(this, value ? "true" : "false");
}


JsonSinkStatus JSON::on_string(std::string_view value) {
    return addStringViewAsSinkStatus(this, value);
}


JsonSinkStatus JSON::on_number(std::string_view value) {
    return addStringViewAsSinkStatus(this, value);
}


JsonSinkStatus JSON::on_start_array() {
    return startContainer(&m_containers,
        std::make_unique<JSONContainerArray>(getCurrentKey()),
        &m_current_depth, m_max_depth, &m_depth_limit_exceeded);
}


JsonSinkStatus JSON::on_end_array() {
    return endContainer(&m_containers, &m_current_depth);
}


JsonSinkStatus JSON::on_start_object() {
    return startContainer(&m_containers,
        std::make_unique<JSONContainerMap>(getCurrentKey()),
        &m_current_depth, m_max_depth, &m_depth_limit_exceeded);
}


JsonSinkStatus JSON::on_end_object() {
    return endContainer(&m_containers, &m_current_depth);
}

void JSON::clearContainers() {
    m_containers.clear();
}

}  // namespace modsecurity::RequestBodyProcessor
