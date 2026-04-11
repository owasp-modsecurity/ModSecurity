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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/request_body_processor/json_backend.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <utility>

#include "src/request_body_processor/json_instrumentation.h"
#include "simdjson.h"

namespace modsecurity {
namespace RequestBodyProcessor {
namespace {

JsonParseResult makeResult(JsonParseStatus parse_status,
    JsonSinkStatus sink_status = JsonSinkStatus::Continue,
    std::string detail = "") {
    return JsonParseResult{parse_status, sink_status, std::move(detail)};
}

JsonParseResult makeResult(JsonParseStatus parse_status, std::string detail) {
    return makeResult(parse_status, JsonSinkStatus::Continue, std::move(detail));
}

JsonParseResult stopTraversal(JsonSinkStatus sink_status,
    std::string_view location) {
    return makeResult(JsonParseStatus::Ok, sink_status,
        std::string("JSON traversal stopped while ") + std::string(location)
        + ".");
}

JsonParseResult fromSimdjsonError(simdjson::error_code error) {
    switch (error) {
        case simdjson::UTF8_ERROR:
            return makeResult(JsonParseStatus::Utf8Error,
                std::string("Invalid UTF-8 in JSON body: ")
                + simdjson::error_message(error));
        case simdjson::EMPTY:
        case simdjson::UNCLOSED_STRING:
        case simdjson::INCOMPLETE_ARRAY_OR_OBJECT:
        case simdjson::INSUFFICIENT_PADDING:
            return makeResult(JsonParseStatus::TruncatedInput,
                std::string("Incomplete JSON body: ")
                + simdjson::error_message(error));
        case simdjson::DEPTH_ERROR:
        case simdjson::TAPE_ERROR:
        case simdjson::STRING_ERROR:
        case simdjson::T_ATOM_ERROR:
        case simdjson::F_ATOM_ERROR:
        case simdjson::N_ATOM_ERROR:
        case simdjson::NUMBER_ERROR:
        case simdjson::BIGINT_ERROR:
        case simdjson::UNESCAPED_CHARS:
        case simdjson::TRAILING_CONTENT:
            return makeResult(JsonParseStatus::ParseError,
                std::string("Invalid JSON body: ")
                + simdjson::error_message(error));
        case simdjson::CAPACITY:
        case simdjson::OUT_OF_CAPACITY:
        case simdjson::MEMALLOC:
            return makeResult(JsonParseStatus::InternalError,
                std::string("JSON parser backend failure: ")
                + simdjson::error_message(error));
        default:
            return makeResult(JsonParseStatus::InternalError,
                std::string("JSON backend failed: ")
                + simdjson::error_message(error));
    }
}

template <typename ResultType, typename TargetType>
JsonParseResult getResult(ResultType &&result, TargetType *target) {
    if (auto error = std::forward<ResultType>(result).get(*target); error) {
        return fromSimdjsonError(error);
    }

    return makeResult(JsonParseStatus::Ok);
}

class JsonBackendWalker {
 public:
    explicit JsonBackendWalker(JsonEventSink *sink) : m_sink(sink) { }

    JsonParseResult walk(simdjson::ondemand::document *document) {
        bool is_scalar = false;
        JsonParseResult result = getResult(document->is_scalar(), &is_scalar);
        if (!result.ok()) {
            return result;
        }

        if (is_scalar) {
            return walkDocumentScalar(document);
        }

        simdjson::ondemand::value root_value;
        result = getResult(document->get_value(), &root_value);
        if (!result.ok()) {
            return result;
        }

        return walkValue(root_value);
    }

 private:
    JsonParseResult walkDocumentScalar(simdjson::ondemand::document *document) {
        simdjson::ondemand::json_type type;
        JsonParseResult result = getResult(document->type(), &type);
        if (!result.ok()) {
            return result;
        }

        switch (type) {
            case simdjson::ondemand::json_type::string: {
                std::string_view decoded;
                result = getResult(document->get_string(), &decoded);
                if (!result.ok()) {
                    return result;
                }

                JsonSinkStatus sink_status = m_sink->on_string(decoded);
                if (sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a root string");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::number: {
                std::string_view raw_number;
                result = getResult(document->raw_json_token(), &raw_number);
                if (!result.ok()) {
                    return result;
                }

                JsonSinkStatus sink_status = m_sink->on_number(raw_number);
                if (sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a root number");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::boolean: {
                bool boolean_value = false;
                result = getResult(document->get_bool(), &boolean_value);
                if (!result.ok()) {
                    return result;
                }

                JsonSinkStatus sink_status = m_sink->on_boolean(boolean_value);
                if (sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a root boolean");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::null: {
                bool is_null = false;
                result = getResult(document->is_null(), &is_null);
                if (!result.ok()) {
                    return result;
                }
                if (!is_null) {
                    return makeResult(JsonParseStatus::InternalError,
                        "Root scalar classified as null but failed validation.");
                }

                JsonSinkStatus sink_status = m_sink->on_null();
                if (sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a root null");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::object:
            case simdjson::ondemand::json_type::array:
            case simdjson::ondemand::json_type::unknown:
                return makeResult(JsonParseStatus::InternalError,
                    "Unexpected root scalar type encountered in simdjson backend.");
        }

        return makeResult(JsonParseStatus::InternalError,
            "Unsupported root scalar type encountered in simdjson backend.");
    }

    JsonParseResult walkValue(simdjson::ondemand::value value) {
        simdjson::ondemand::json_type type;

        JsonParseResult result = getResult(value.type(), &type);
        if (!result.ok()) {
            return result;
        }

        switch (type) {
            case simdjson::ondemand::json_type::object:
                return walkObject(value);
            case simdjson::ondemand::json_type::array:
                return walkArray(value);
            case simdjson::ondemand::json_type::string:
                return walkString(value);
            case simdjson::ondemand::json_type::number:
                return walkNumber(value);
            case simdjson::ondemand::json_type::boolean:
                return walkBoolean(value);
            case simdjson::ondemand::json_type::null: {
                JsonSinkStatus sink_status = m_sink->on_null();
                if (sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a null value");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::unknown:
                return makeResult(JsonParseStatus::InternalError,
                    "Unknown JSON token type encountered.");
        }

        return makeResult(JsonParseStatus::InternalError,
            "Unsupported JSON token type encountered.");
    }

    JsonParseResult walkObject(simdjson::ondemand::value value) {
        simdjson::ondemand::object object;
        JsonParseResult result = getResult(value.get_object(), &object);
        if (!result.ok()) {
            return result;
        }

        JsonSinkStatus sink_status = m_sink->on_start_object();
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "starting an object");
        }

        for (auto field_result : object) {
            simdjson::ondemand::field field;
            std::string_view key;
            simdjson::ondemand::value child;

            result = getResult(std::move(field_result), &field);
            if (!result.ok()) {
                return result;
            }

            result = getResult(field.unescaped_key(), &key);
            if (!result.ok()) {
                return result;
            }

            sink_status = m_sink->on_key(key);
            if (sink_status != JsonSinkStatus::Continue) {
                return stopTraversal(sink_status, "processing an object key");
            }

            child = field.value();

            result = walkValue(child);
            if (!result.ok()) {
                return result;
            }
        }

        sink_status = m_sink->on_end_object();
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "ending an object");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkArray(simdjson::ondemand::value value) {
        simdjson::ondemand::array array;
        JsonParseResult result = getResult(value.get_array(), &array);
        if (!result.ok()) {
            return result;
        }

        JsonSinkStatus sink_status = m_sink->on_start_array();
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "starting an array");
        }

        for (auto element_result : array) {
            simdjson::ondemand::value element;

            result = getResult(std::move(element_result), &element);
            if (!result.ok()) {
                return result;
            }

            result = walkValue(element);
            if (!result.ok()) {
                return result;
            }
        }

        sink_status = m_sink->on_end_array();
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "ending an array");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkString(simdjson::ondemand::value value) {
        std::string_view decoded;
        JsonParseResult result = getResult(value.get_string(), &decoded);
        if (!result.ok()) {
            return result;
        }

        JsonSinkStatus sink_status = m_sink->on_string(decoded);
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a string");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkNumber(simdjson::ondemand::value value) {
        std::string_view raw_number = value.raw_json_token();
        JsonSinkStatus sink_status = m_sink->on_number(raw_number);

        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a number");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkBoolean(simdjson::ondemand::value value) {
        bool boolean_value = false;
        JsonParseResult result = getResult(value.get_bool(), &boolean_value);
        if (!result.ok()) {
            return result;
        }

        JsonSinkStatus sink_status = m_sink->on_boolean(boolean_value);
        if (sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a boolean");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonEventSink *m_sink;
};

}  // namespace

JsonParseResult parseDocumentWithSimdjson(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) {
    (void) options;

    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto parser_start = std::chrono::steady_clock::now();
    simdjson::ondemand::parser parser;
    recordSimdjsonParserConstruction(static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - parser_start).count()));
    const auto padded_start = std::chrono::steady_clock::now();
    simdjson::padded_string padded(input);
    recordSimdjsonPaddedCopy(input.size(), static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - padded_start).count()));
#else
    simdjson::ondemand::parser parser;
    simdjson::padded_string padded(input);
#endif
    simdjson::ondemand::document document;

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto iterate_start = std::chrono::steady_clock::now();
    if (auto error = parser.iterate(padded).get(document); error) {
        recordSimdjsonIterate(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - iterate_start).count()));
        return fromSimdjsonError(error);
    }
    recordSimdjsonIterate(static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - iterate_start).count()));
#else
    if (auto error = parser.iterate(padded).get(document); error) {
        return fromSimdjsonError(error);
    }
#endif

    JsonBackendWalker walker(sink);
    return walker.walk(&document);
}

}  // namespace RequestBodyProcessor
}  // namespace modsecurity
