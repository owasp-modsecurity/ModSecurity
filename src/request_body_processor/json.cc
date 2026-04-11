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

#include "src/request_body_processor/json.h"

#include <string>

#include "src/request_body_processor/json_adapter.h"


namespace modsecurity {
namespace RequestBodyProcessor {

static const double json_depth_limit_default = 10000.0;
static const char* json_depth_limit_exceeded_msg = ". Parsing depth limit exceeded";

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
        m_data.append(buf, size);
    }

    return true;
}


bool JSON::complete(std::string *err) {
    if (m_data.empty()) {
        return true;
    }

    JSONAdapter adapter;
    JsonParseResult result = adapter.parse(m_data,
        static_cast<JsonEventSink *>(this));

    if (!result.ok()) {
        if (result.sink_status == JsonSinkStatus::DepthLimitExceeded) {
            m_depth_limit_exceeded = true;
        }
        if (err != nullptr) {
            switch (result.parse_status) {
                case JsonParseStatus::ParseError:
                    if (result.detail.empty()) {
                        err->assign("Invalid JSON body.");
                    } else {
                        err->assign(result.detail);
                    }
                    break;
                case JsonParseStatus::TruncatedInput:
                    if (result.detail.empty()) {
                        err->assign("Incomplete JSON body.");
                    } else {
                        err->assign(result.detail);
                    }
                    break;
                case JsonParseStatus::Utf8Error:
                    if (result.detail.empty()) {
                        err->assign("Invalid UTF-8 in JSON body.");
                    } else {
                        err->assign(result.detail);
                    }
                    break;
                case JsonParseStatus::EngineAbort:
                    if (result.detail.empty()) {
                        err->assign("JSON traversal aborted by ModSecurity.");
                    } else {
                        err->assign(result.detail);
                    }
                    break;
                case JsonParseStatus::InternalError:
                    if (result.detail.empty()) {
                        err->assign("Internal JSON backend failure.");
                    } else {
                        err->assign(result.detail);
                    }
                    break;
                case JsonParseStatus::Ok:
                    err->clear();
                    break;
            }
        }
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
            m_containers[i]);
        path = path + m_containers[i]->m_name;
        if (a != NULL) {
            path = path + ".array_" + std::to_string(a->m_elementCounter);
        } else {
            path = path + ".";
        }
    }

    if (m_containers.size() > 0) {
        JSONContainerArray *a = dynamic_cast<JSONContainerArray *>(
            m_containers.back());
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
    return addArgument("") != 0 ? JsonSinkStatus::Continue
        : JsonSinkStatus::EngineAbort;
}


JsonSinkStatus JSON::on_boolean(bool value) {
    if (value) {
        return addArgument("true") != 0 ? JsonSinkStatus::Continue
            : JsonSinkStatus::EngineAbort;
    }
    return addArgument("false") != 0 ? JsonSinkStatus::Continue
        : JsonSinkStatus::EngineAbort;
}


JsonSinkStatus JSON::on_string(std::string_view value) {
    return addArgument(std::string(value.data(), value.size())) != 0
        ? JsonSinkStatus::Continue : JsonSinkStatus::EngineAbort;
}


JsonSinkStatus JSON::on_number(std::string_view value) {
    return addArgument(std::string(value.data(), value.size())) != 0
        ? JsonSinkStatus::Continue : JsonSinkStatus::EngineAbort;
}


JsonSinkStatus JSON::on_start_array() {
    std::string name = getCurrentKey();
    m_containers.push_back(
        reinterpret_cast<JSONContainer *>(new JSONContainerArray(name)));
    m_current_depth++;
    if (m_current_depth > m_max_depth) {
        m_depth_limit_exceeded = true;
        return JsonSinkStatus::DepthLimitExceeded;
    }
    return JsonSinkStatus::Continue;
}


JsonSinkStatus JSON::on_end_array() {
    if (m_containers.empty()) {
        return JsonSinkStatus::InternalError;
    }

    JSONContainer *a = m_containers.back();
    m_containers.pop_back();
    delete a;
    if (m_containers.size() > 0) {
        JSONContainerArray *ja = dynamic_cast<JSONContainerArray *>(
            m_containers.back());
        if (ja) {
            ja->m_elementCounter++;
        }
    }
    m_current_depth--;
    if (m_current_depth < 0) {
        m_current_depth = 0;
        return JsonSinkStatus::InternalError;
    }

    return JsonSinkStatus::Continue;
}


JsonSinkStatus JSON::on_start_object() {
    std::string name(getCurrentKey());
    m_containers.push_back(
        reinterpret_cast<JSONContainer *>(new JSONContainerMap(name)));
    m_current_depth++;
    if (m_current_depth > m_max_depth) {
        m_depth_limit_exceeded = true;
        return JsonSinkStatus::DepthLimitExceeded;
    }
    return JsonSinkStatus::Continue;
}


JsonSinkStatus JSON::on_end_object() {
    if (m_containers.empty()) {
        return JsonSinkStatus::InternalError;
    }

    JSONContainer *a = m_containers.back();
    m_containers.pop_back();
    delete a;

    if (m_containers.size() > 0) {
        JSONContainerArray *ja = dynamic_cast<JSONContainerArray *>(
            m_containers.back());
        if (ja) {
            ja->m_elementCounter++;
        }
    }

    m_current_depth--;
    if (m_current_depth < 0) {
        m_current_depth = 0;
        return JsonSinkStatus::InternalError;
    }
    return JsonSinkStatus::Continue;
}

void JSON::clearContainers() {
    while (m_containers.size() > 0) {
        JSONContainer *a = m_containers.back();
        m_containers.pop_back();
        delete a;
    }
}

}  // namespace RequestBodyProcessor
}  // namespace modsecurity
