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

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "src/request_body_processor/json_instrumentation.h"
#include "simdjson.h"

namespace modsecurity::RequestBodyProcessor {
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

std::size_t effectiveTechnicalMaxDepth(
    const JsonBackendParseOptions &options) {
    return options.technical_max_depth > 0
        ? static_cast<std::size_t>(options.technical_max_depth) : 1;
}

std::string_view trimTrailingJsonWhitespace(std::string_view token) {
    while (!token.empty()) {
        if (const char tail = token.back();
            tail != ' ' && tail != '\t' && tail != '\n' && tail != '\r') {
            break;
        }
        token.remove_suffix(1);
    }
    return token;
}

/*
 * The ondemand parser is reused per thread because simdjson benefits from
 * keeping its internal buffers warm across parses. thread_local storage keeps
 * the parser isolated to the calling thread, so no parser state is shared
 * across transactions running on different threads. The parse and full
 * document traversal both complete inside parseDocumentWithSimdjson(), so no
 * parser-backed state escapes this function. We intentionally do not add an
 * automatic release/recreate heuristic here: the vendored simdjson API
 * explicitly supports parser reuse, and retained capacity after unusually
 * large inputs remains a conscious tradeoff rather than an accidental leak.
 */
simdjson::ondemand::parser &getReusableSimdjsonParser() {
    thread_local std::unique_ptr<simdjson::ondemand::parser> parser;
    if (parser == nullptr) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
        const auto parser_start = std::chrono::steady_clock::now();
        parser.reset(new simdjson::ondemand::parser());
        recordSimdjsonParserConstruction(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - parser_start).count()));
#else
        parser.reset(new simdjson::ondemand::parser());
#endif
    }
    return *parser;
}

std::size_t clampRequestedMaxDepth(std::size_t input_size,
    const JsonBackendParseOptions &options) {
    const std::size_t requested_depth = effectiveTechnicalMaxDepth(options);
    const std::size_t max_possible_depth = (input_size / 2) + 1;
    return std::min(requested_depth, std::max<std::size_t>(1,
        max_possible_depth));
}

simdjson::error_code prepareParser(simdjson::ondemand::parser *parser,
    std::size_t input_size, const JsonBackendParseOptions &options) {
    if (parser == nullptr) {
        return simdjson::MEMALLOC;
    }

    const JsonBackendParseOptions default_options;
    std::size_t required_max_depth = parser->max_depth();
    if (options.technical_max_depth != default_options.technical_max_depth) {
        required_max_depth = clampRequestedMaxDepth(input_size, options);
    }

    if (parser->capacity() >= input_size
        && parser->max_depth() == required_max_depth) {
        return simdjson::SUCCESS;
    }

    // simdjson reuses parser buffers across parses. allocate() can grow the
    // per-thread parser to satisfy a larger document or different max-depth,
    // but it does not proactively shrink retained capacity for later, smaller
    // inputs. In simdjson 4.6.1 the max-depth parameter is only enforced by
    // simdjson's development checks, so we keep passing it here for that
    // internal guardrail while our own walker enforces technical_max_depth at
    // runtime using current_depth().
    return parser->allocate(input_size, required_max_depth);
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
    JsonBackendWalker(JsonEventSink *sink,
        const JsonBackendParseOptions &options)
        : m_sink(sink),
          m_technical_max_depth(effectiveTechnicalMaxDepth(options)) { }

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

                if (JsonSinkStatus sink_status = m_sink->on_string(decoded);
                    sink_status != JsonSinkStatus::Continue) {
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

                if (JsonSinkStatus sink_status = m_sink->on_number(
                        trimTrailingJsonWhitespace(raw_number));
                    sink_status != JsonSinkStatus::Continue) {
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

                if (JsonSinkStatus sink_status = m_sink->on_boolean(
                        boolean_value);
                    sink_status != JsonSinkStatus::Continue) {
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

                if (JsonSinkStatus sink_status = m_sink->on_null();
                    sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a root null");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::unknown:
                return makeResult(JsonParseStatus::ParseError,
                    "Invalid JSON token encountered in simdjson backend.");
            case simdjson::ondemand::json_type::object:
            case simdjson::ondemand::json_type::array:
                return makeResult(JsonParseStatus::InternalError,
                    "Unexpected root scalar container encountered in simdjson backend.");
        }

        return makeResult(JsonParseStatus::InternalError,
            "Unsupported root scalar type encountered in simdjson backend.");
    }

    JsonParseResult walkValue(simdjson::ondemand::value value) {
        simdjson::ondemand::json_type type;

        if (JsonParseResult result = getResult(value.type(), &type);
            !result.ok()) {
            return result;
        }

        switch (type) {
            case simdjson::ondemand::json_type::object:
                if (auto result = enforceTechnicalDepth(value); !result.ok()) {
                    return result;
                }
                return walkObject(value);
            case simdjson::ondemand::json_type::array:
                if (auto result = enforceTechnicalDepth(value); !result.ok()) {
                    return result;
                }
                return walkArray(value);
            case simdjson::ondemand::json_type::string:
                return walkString(value);
            case simdjson::ondemand::json_type::number:
                return walkNumber(value);
            case simdjson::ondemand::json_type::boolean:
                return walkBoolean(value);
            case simdjson::ondemand::json_type::null: {
                if (JsonSinkStatus sink_status = m_sink->on_null();
                    sink_status != JsonSinkStatus::Continue) {
                    return stopTraversal(sink_status, "handling a null value");
                }
                return makeResult(JsonParseStatus::Ok);
            }
            case simdjson::ondemand::json_type::unknown:
                return makeResult(JsonParseStatus::ParseError,
                    "Invalid JSON token encountered in simdjson backend.");
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
        if (JsonParseResult result = getResult(value.get_string(), &decoded);
            !result.ok()) {
            return result;
        }

        if (JsonSinkStatus sink_status = m_sink->on_string(decoded);
            sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a string");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkNumber(simdjson::ondemand::value value) {
        std::string_view raw_number = trimTrailingJsonWhitespace(
            value.raw_json_token());
        if (JsonSinkStatus sink_status = m_sink->on_number(raw_number);
            sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a number");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult walkBoolean(simdjson::ondemand::value value) {
        bool boolean_value = false;
        if (JsonParseResult result = getResult(value.get_bool(),
                &boolean_value); !result.ok()) {
            return result;
        }

        if (JsonSinkStatus sink_status = m_sink->on_boolean(boolean_value);
            sink_status != JsonSinkStatus::Continue) {
            return stopTraversal(sink_status, "handling a boolean");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonParseResult enforceTechnicalDepth(simdjson::ondemand::value value) {
        const int32_t current_depth = value.current_depth();
        if (current_depth <= 0) {
            return makeResult(JsonParseStatus::InternalError,
                "Invalid current depth reported by simdjson backend.");
        }

        if (static_cast<std::size_t>(current_depth) > m_technical_max_depth) {
            return makeResult(JsonParseStatus::ParseError,
                "JSON nesting depth exceeds backend technical max depth.");
        }

        return makeResult(JsonParseStatus::Ok);
    }

    JsonEventSink *m_sink;
    std::size_t m_technical_max_depth;
};

struct PreparedSimdjsonInput {
    simdjson::padded_string_view view{};
    simdjson::padded_string owned_copy{};
};

PreparedSimdjsonInput prepareMutableSimdjsonInput(std::string *input) {
    PreparedSimdjsonInput prepared;

    // The production request-body path owns a mutable std::string, so we can
    // pad that buffer in place and keep the logical JSON length in the
    // returned padded_string_view. This removes the extra padded_string copy
    // while still satisfying simdjson's padding requirement explicitly.
    prepared.view = simdjson::pad(*input);
    return prepared;
}

PreparedSimdjsonInput prepareConstSimdjsonInput(const std::string &input) {
    PreparedSimdjsonInput prepared;
    prepared.view = simdjson::padded_string_view(input);

    // The const path must not guess about std::string capacity. We only parse
    // directly when simdjson itself confirms that the existing allocation
    // and/or trailing whitespace provide sufficient padding.
    if (prepared.view.has_sufficient_padding()) {
        return prepared;
    }

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto padded_start = std::chrono::steady_clock::now();
    prepared.owned_copy = simdjson::padded_string(input);
    recordSimdjsonPaddedCopy(input.size(), static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - padded_start).count()));
#else
    prepared.owned_copy = simdjson::padded_string(input);
#endif
    prepared.view = prepared.owned_copy;
    return prepared;
}

JsonParseResult parsePreparedDocumentWithSimdjson(
    simdjson::padded_string_view input, JsonEventSink *sink,
    const JsonBackendParseOptions &options) {
    simdjson::ondemand::parser &parser = getReusableSimdjsonParser();
    // This only prepares parser capacity and max-depth bookkeeping. Buffer
    // lifetime and padding must already have been handled by the caller.
    if (auto error = prepareParser(&parser, input.length(), options); error) {
        return fromSimdjsonError(error);
    }

    simdjson::ondemand::document document;
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto iterate_start = std::chrono::steady_clock::now();
    if (auto error = parser.iterate(input).get(document); error) {
        recordSimdjsonIterate(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - iterate_start).count()));
        return fromSimdjsonError(error);
    }
    recordSimdjsonIterate(static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - iterate_start).count()));
#else
    if (auto error = parser.iterate(input).get(document); error) {
        return fromSimdjsonError(error);
    }
#endif

    JsonBackendWalker walker(sink, options);
    return walker.walk(&document);
}

}  // namespace

JsonParseResult parseDocumentWithSimdjson(std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) {
    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

    PreparedSimdjsonInput prepared = prepareMutableSimdjsonInput(&input);
    return parsePreparedDocumentWithSimdjson(prepared.view, sink, options);
}

JsonParseResult parseDocumentWithSimdjson(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) {
    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

    PreparedSimdjsonInput prepared = prepareConstSimdjsonInput(input);
    return parsePreparedDocumentWithSimdjson(prepared.view, sink, options);
}

}  // namespace modsecurity::RequestBodyProcessor
