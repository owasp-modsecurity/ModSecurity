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

#include "src/request_body_processor/json_adapter.h"

#include <utility>

#include "src/config.h"

namespace modsecurity::RequestBodyProcessor {
namespace {

JsonParseResult makeResult(JsonParseStatus parse_status,
    JsonSinkStatus sink_status = JsonSinkStatus::Continue,
    std::string detail = "") {
    return JsonParseResult{parse_status, sink_status, std::move(detail)};
}

JsonParseResult normalizeResult(JsonParseResult result) {
    if (result.parse_status != JsonParseStatus::Ok) {
        return result;
    }

    switch (result.sink_status) {
        case JsonSinkStatus::Continue:
            return result;
        case JsonSinkStatus::EngineAbort:
            result.parse_status = JsonParseStatus::EngineAbort;
            return result;
        case JsonSinkStatus::DepthLimitExceeded:
            result.parse_status = JsonParseStatus::ParseError;
            return result;
        case JsonSinkStatus::InternalError:
            result.parse_status = JsonParseStatus::InternalError;
            return result;
    }

    result.parse_status = JsonParseStatus::InternalError;
    result.sink_status = JsonSinkStatus::InternalError;
    result.detail.assign("Unknown JSON sink status.");
    return result;
}

}  // namespace

JsonParseResult JSONAdapter::parse(std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) const {
    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

    if (input.empty()) {
        return makeResult(JsonParseStatus::Ok);
    }

#if defined(MSC_JSON_BACKEND_SIMDJSON)
    return normalizeResult(parseDocumentWithSimdjson(input, sink, options));
#elif defined(MSC_JSON_BACKEND_JSONCONS)
    return normalizeResult(parseDocumentWithJsoncons(input, sink, options));
#else
    return makeResult(JsonParseStatus::InternalError,
        JsonSinkStatus::InternalError,
        "ModSecurity was built without a selected JSON backend.");
#endif
}

JsonParseResult JSONAdapter::parse(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) const {
    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

    if (input.empty()) {
        return makeResult(JsonParseStatus::Ok);
    }

#if defined(MSC_JSON_BACKEND_SIMDJSON)
    return normalizeResult(parseDocumentWithSimdjson(input, sink, options));
#elif defined(MSC_JSON_BACKEND_JSONCONS)
    return normalizeResult(parseDocumentWithJsoncons(input, sink, options));
#else
    return makeResult(JsonParseStatus::InternalError,
        JsonSinkStatus::InternalError,
        "ModSecurity was built without a selected JSON backend.");
#endif
}

}  // namespace modsecurity::RequestBodyProcessor
