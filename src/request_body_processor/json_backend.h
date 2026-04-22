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

#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_H_

#include <string>
#include <string_view>

namespace modsecurity::RequestBodyProcessor {

enum class JsonParseStatus {
    Ok,
    ParseError,
    TruncatedInput,
    Utf8Error,
    EngineAbort,
    InternalError
};

enum class JsonSinkStatus {
    Continue,
    EngineAbort,
    DepthLimitExceeded,
    InternalError
};

struct JsonParseResult {
    JsonParseStatus parse_status{JsonParseStatus::Ok};
    JsonSinkStatus sink_status{JsonSinkStatus::Continue};
    std::string detail;

    bool ok() const {
        return parse_status == JsonParseStatus::Ok
            && sink_status == JsonSinkStatus::Continue;
    }
};

struct JsonBackendParseOptions {
    int technical_max_depth{1048576};
};

class JsonEventSink {
 public:
    virtual ~JsonEventSink() = default;

    virtual JsonSinkStatus on_start_object() = 0;
    virtual JsonSinkStatus on_end_object() = 0;
    virtual JsonSinkStatus on_start_array() = 0;
    virtual JsonSinkStatus on_end_array() = 0;
    virtual JsonSinkStatus on_key(std::string_view value) = 0;
    virtual JsonSinkStatus on_string(std::string_view value) = 0;
    virtual JsonSinkStatus on_number(std::string_view raw_number) = 0;
    virtual JsonSinkStatus on_boolean(bool value) = 0;
    virtual JsonSinkStatus on_null() = 0;
};

JsonParseResult parseDocumentWithSimdjson(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options);

JsonParseResult parseDocumentWithSimdjson(std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options);

JsonParseResult parseDocumentWithJsoncons(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options);

}  // namespace modsecurity::RequestBodyProcessor

#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_BACKEND_H_
