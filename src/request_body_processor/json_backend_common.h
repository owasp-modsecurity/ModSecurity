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

#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_COMMON_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_COMMON_H_

#include <string>
#include <string_view>
#include <utility>

#include "src/request_body_processor/json_backend.h"

namespace modsecurity::RequestBodyProcessor::json_backend_common {

inline JsonParseResult makeResult(JsonParseStatus parse_status,
    JsonSinkStatus sink_status = JsonSinkStatus::Continue,
    std::string detail = "") {
    return JsonParseResult{parse_status, sink_status, std::move(detail)};
}

inline JsonParseResult makeResult(JsonParseStatus parse_status,
    std::string detail) {
    return makeResult(parse_status, JsonSinkStatus::Continue, std::move(detail));
}

inline JsonParseResult stopTraversal(JsonSinkStatus sink_status,
    std::string_view location) {
    return makeResult(JsonParseStatus::Ok, sink_status,
        std::string("JSON traversal stopped while ") + std::string(location)
        + ".");
}

inline JsonParseResult finishSinkCall(JsonSinkStatus sink_status,
    std::string_view location) {
    if (sink_status != JsonSinkStatus::Continue) {
        return stopTraversal(sink_status, location);
    }
    return makeResult(JsonParseStatus::Ok);
}

}  // namespace modsecurity::RequestBodyProcessor::json_backend_common

#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_COMMON_H_
