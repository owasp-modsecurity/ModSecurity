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
#include "src/request_body_processor/json_backend_common.h"

#include <cctype>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "src/request_body_processor/json_instrumentation.h"
#include <jsoncons/json_cursor.hpp>
#include <jsoncons/json_error.hpp>
#include <jsoncons/json_options.hpp>
#include <jsoncons/semantic_tag.hpp>

namespace modsecurity::RequestBodyProcessor {
namespace {
using json_backend_common::finishSinkCall;
using json_backend_common::makeResult;

bool isUtf8RelatedError(const std::error_code &error) {
    switch (static_cast<jsoncons::json_errc>(error.value())) {
        case jsoncons::json_errc::illegal_character_in_string:
        case jsoncons::json_errc::illegal_control_character:
        case jsoncons::json_errc::illegal_escaped_character:
        case jsoncons::json_errc::expected_codepoint_surrogate_pair:
        case jsoncons::json_errc::invalid_hex_escape_sequence:
        case jsoncons::json_errc::invalid_unicode_escape_sequence:
        case jsoncons::json_errc::expected_continuation_byte:
        case jsoncons::json_errc::over_long_utf8_sequence:
        case jsoncons::json_errc::illegal_codepoint:
        case jsoncons::json_errc::illegal_surrogate_value:
        case jsoncons::json_errc::unpaired_high_surrogate:
        case jsoncons::json_errc::illegal_unicode_character:
            return true;
        default:
            return false;
    }
}

JsonParseResult fromJsonconsError(const std::error_code &error,
    const jsoncons::ser_context &context) {
    std::string detail = error.message() + " at line "
        + std::to_string(context.line()) + ", column "
        + std::to_string(context.column()) + ".";

    switch (static_cast<jsoncons::json_errc>(error.value())) {
        case jsoncons::json_errc::unexpected_eof:
            return makeResult(JsonParseStatus::TruncatedInput,
                JsonSinkStatus::Continue, detail);
        case jsoncons::json_errc::max_nesting_depth_exceeded:
            return makeResult(JsonParseStatus::ParseError,
                JsonSinkStatus::Continue, detail);
        case jsoncons::json_errc::source_error:
            return makeResult(JsonParseStatus::InternalError,
                JsonSinkStatus::Continue, detail);
        default:
            if (isUtf8RelatedError(error)) {
                return makeResult(JsonParseStatus::Utf8Error,
                    JsonSinkStatus::Continue, detail);
            }
            return makeResult(JsonParseStatus::ParseError,
                JsonSinkStatus::Continue, detail);
    }
}

bool isDigit(char value) {
    return std::isdigit(static_cast<unsigned char>(value)) != 0;
}

bool isValidJsonNumber(std::string_view token) {
    std::size_t index = 0;

    if (token.empty()) {
        return false;
    }

    if (token[index] == '-') {
        index++;
        if (index == token.size()) {
            return false;
        }
    }

    if (token[index] == '0') {
        index++;
    } else {
        if (!isDigit(token[index]) || token[index] == '0') {
            return false;
        }
        while (index < token.size() && isDigit(token[index])) {
            index++;
        }
    }

    if (index < token.size() && token[index] == '.') {
        index++;
        if (index == token.size() || !isDigit(token[index])) {
            return false;
        }
        while (index < token.size() && isDigit(token[index])) {
            index++;
        }
    }

    if (index < token.size() && (token[index] == 'e' || token[index] == 'E')) {
        index++;
        if (index < token.size() && (token[index] == '+' || token[index] == '-')) {
            index++;
        }
        if (index == token.size() || !isDigit(token[index])) {
            return false;
        }
        while (index < token.size() && isDigit(token[index])) {
            index++;
        }
    }

    return index == token.size();
}

bool tokenMatchesNumericEvent(jsoncons::staj_event_type event_type,
    std::string_view token) {
    if (!isValidJsonNumber(token)) {
        return false;
    }

    if (event_type == jsoncons::staj_event_type::int64_value
        || event_type == jsoncons::staj_event_type::uint64_value) {
        return token.find_first_of(".eE") == std::string_view::npos;
    }

    return true;
}

bool isNumericEventType(jsoncons::staj_event_type event_type) {
    switch (event_type) {
        case jsoncons::staj_event_type::int64_value:
        case jsoncons::staj_event_type::uint64_value:
        case jsoncons::staj_event_type::double_value:
        case jsoncons::staj_event_type::half_value:
            return true;
        default:
            return false;
    }
}

bool isNumericStringEvent(const jsoncons::staj_event &event) {
    return event.event_type() == jsoncons::staj_event_type::string_value
        && (event.tag() == jsoncons::semantic_tag::bigint
            || event.tag() == jsoncons::semantic_tag::bigdec);
}

class RawJsonTokenCursor {
 public:
    explicit RawJsonTokenCursor(std::string_view input)
        : m_input(input) { }

    bool consume(const jsoncons::staj_event &event, std::string_view *raw_token,
        std::string *detail) {
        skipInsignificant();

        if (isNumericEventType(event.event_type()) || isNumericStringEvent(event)) {
            return consumeNumber(raw_token, detail);
        }

        switch (event.event_type()) {
            case jsoncons::staj_event_type::begin_object:
                return consumeChar('{', raw_token, detail);
            case jsoncons::staj_event_type::end_object:
                return consumeChar('}', raw_token, detail);
            case jsoncons::staj_event_type::begin_array:
                return consumeChar('[', raw_token, detail);
            case jsoncons::staj_event_type::end_array:
                return consumeChar(']', raw_token, detail);
            case jsoncons::staj_event_type::key:
            case jsoncons::staj_event_type::string_value:
                return consumeString(raw_token, detail);
            case jsoncons::staj_event_type::null_value:
                return consumeLiteral("null", raw_token, detail);
            case jsoncons::staj_event_type::bool_value: {
                std::error_code error;
                const bool value = event.get<bool>(error);
                if (error) {
                    if (detail != nullptr) {
                        *detail = std::string("Unable to decode boolean event while synchronizing raw token cursor: ")
                            + error.message();
                    }
                    return false;
                }
                return consumeLiteral(value ? "true" : "false", raw_token, detail);
            }
            case jsoncons::staj_event_type::byte_string_value:
                if (detail != nullptr) {
                    *detail = "Unsupported byte-string event encountered in jsoncons backend.";
                }
                return false;
        }

        if (detail != nullptr) {
            *detail = "Unsupported STAJ event encountered while synchronizing raw JSON tokens.";
        }
        return false;
    }

    bool consumeNextNumberToken(std::string_view *raw_token,
        std::string *detail) {
        std::size_t probe_offset = m_offset;
        if (!skipToNextNumberToken(&probe_offset, detail)) {
            return false;
        }
        if (!consumeNumberAt(&probe_offset, raw_token, detail)) {
            return false;
        }
        m_offset = probe_offset;
        return true;
    }

    bool advanceExactNumber(std::string_view exact_number, std::string *detail) {
        if (!isValidJsonNumber(exact_number)) {
            if (detail != nullptr) {
                *detail = "Unable to advance raw JSON number cursor using a non-numeric token.";
            }
            return false;
        }

        std::size_t probe_offset = m_offset;
        if (!skipToNextNumberToken(&probe_offset, detail)) {
            return false;
        }
        if (probe_offset + exact_number.size() > m_input.size()
            || m_input.compare(probe_offset, exact_number.size(), exact_number)
                != 0) {
            if (detail != nullptr) {
                *detail = "Exact raw JSON number token did not match jsoncons numeric lexeme.";
            }
            return false;
        }

        const std::size_t next_offset = probe_offset + exact_number.size();
        if (next_offset < m_input.size()
            && !isNumberBoundary(m_input[next_offset])) {
            if (detail != nullptr) {
                *detail = "Exact raw JSON number token was followed by additional numeric characters.";
            }
            return false;
        }

        m_offset = next_offset;
        return true;
    }

 private:
    static bool isWhitespace(char value) {
        return std::isspace(static_cast<unsigned char>(value)) != 0;
    }

    static bool isHexDigit(char value) {
        return std::isxdigit(static_cast<unsigned char>(value)) != 0;
    }

    static bool isNumberBoundary(char value) {
        return isWhitespace(value) || value == ',' || value == ']' || value == '}';
    }

    void skipInsignificant() {
        skipInsignificantAt(&m_offset);
    }

    void skipInsignificantAt(std::size_t *offset) const {
        while (*offset < m_input.size()) {
            if (char current = m_input[*offset];
                isWhitespace(current) || current == ',' || current == ':') {
                (*offset)++;
                continue;
            }
            break;
        }
    }

    bool consumeChar(char expected, std::string_view *raw_token,
        std::string *detail) {
        return consumeCharAt(&m_offset, expected, raw_token, detail);
    }

    bool consumeCharAt(std::size_t *offset, char expected,
        std::string_view *raw_token, std::string *detail) const {
        if (*offset >= m_input.size() || m_input[*offset] != expected) {
            if (detail != nullptr) {
                *detail = std::string("Expected raw JSON token '") + expected
                    + "' while synchronizing jsoncons events.";
            }
            return false;
        }

        *raw_token = std::string_view(m_input.data() + *offset, 1);
        (*offset)++;
        return true;
    }

    bool consumeLiteral(const char *literal, std::string_view *raw_token,
        std::string *detail) {
        return consumeLiteralAt(&m_offset, literal, raw_token, detail);
    }

    bool consumeLiteralAt(std::size_t *offset, const char *literal,
        std::string_view *raw_token, std::string *detail) const {
        const std::size_t length = std::char_traits<char>::length(literal);
        if (*offset + length > m_input.size()
            || m_input.compare(*offset, length, literal) != 0) {
            if (detail != nullptr) {
                *detail = std::string("Expected raw JSON literal '") + literal
                    + "' while synchronizing jsoncons events.";
            }
            return false;
        }

        *raw_token = std::string_view(m_input.data() + *offset, length);
        *offset += length;
        return true;
    }

    bool consumeString(std::string_view *raw_token, std::string *detail) {
        return consumeStringAt(&m_offset, raw_token, detail);
    }

    bool consumeStringAt(std::size_t *offset, std::string_view *raw_token,
        std::string *detail) const {
        const std::size_t start = *offset;

        if (*offset >= m_input.size() || m_input[*offset] != '"') {
            if (detail != nullptr) {
                *detail = "Expected raw JSON string token while synchronizing jsoncons events.";
            }
            return false;
        }

        (*offset)++;
        while (*offset < m_input.size()) {
            char current = m_input[*offset];
            (*offset)++;
            if (current == '\\') {
                if (*offset >= m_input.size()) {
                    if (detail != nullptr) {
                        *detail = "Truncated escape sequence while synchronizing raw JSON string token.";
                    }
                    return false;
                }

                char escaped = m_input[*offset];
                (*offset)++;
                if (escaped == 'u') {
                    for (int i = 0; i < 4; i++) {
                        if (*offset >= m_input.size()
                            || !isHexDigit(m_input[*offset])) {
                            if (detail != nullptr) {
                                *detail = "Invalid Unicode escape while synchronizing raw JSON string token.";
                            }
                            return false;
                        }
                        (*offset)++;
                    }
                }
                continue;
            }

            if (current == '"') {
                *raw_token = std::string_view(m_input.data() + start,
                    *offset - start);
                return true;
            }

            if (static_cast<unsigned char>(current) < 0x20) {
                if (detail != nullptr) {
                    *detail = "Unexpected control character while synchronizing raw JSON string token.";
                }
                return false;
            }
        }

        if (detail != nullptr) {
            *detail = "Unterminated string token while synchronizing jsoncons events.";
        }
        return false;
    }

    bool consumeNumber(std::string_view *raw_token, std::string *detail) {
        return consumeNumberAt(&m_offset, raw_token, detail);
    }

    bool consumeNumberAt(std::size_t *offset, std::string_view *raw_token,
        std::string *detail) const {
        const std::size_t start = *offset;

        if (*offset < m_input.size() && m_input[*offset] == '-') {
            (*offset)++;
        }

        if (*offset >= m_input.size()) {
            if (detail != nullptr) {
                *detail = "Unexpected end of input while synchronizing raw JSON number token.";
            }
            return false;
        }

        if (m_input[*offset] == '0') {
            (*offset)++;
        } else {
            if (!isDigit(m_input[*offset]) || m_input[*offset] == '0') {
                if (detail != nullptr) {
                    *detail = "Invalid integer component while synchronizing raw JSON number token.";
                }
                return false;
            }
            while (*offset < m_input.size() && isDigit(m_input[*offset])) {
                (*offset)++;
            }
        }

        if (*offset < m_input.size() && m_input[*offset] == '.') {
            (*offset)++;
            if (*offset >= m_input.size() || !isDigit(m_input[*offset])) {
                if (detail != nullptr) {
                    *detail = "Invalid fraction component while synchronizing raw JSON number token.";
                }
                return false;
            }
            while (*offset < m_input.size() && isDigit(m_input[*offset])) {
                (*offset)++;
            }
        }

        if (*offset < m_input.size()
            && (m_input[*offset] == 'e' || m_input[*offset] == 'E')) {
            (*offset)++;
            if (*offset < m_input.size()
                && (m_input[*offset] == '+' || m_input[*offset] == '-')) {
                (*offset)++;
            }
            if (*offset >= m_input.size() || !isDigit(m_input[*offset])) {
                if (detail != nullptr) {
                    *detail = "Invalid exponent component while synchronizing raw JSON number token.";
                }
                return false;
            }
            while (*offset < m_input.size() && isDigit(m_input[*offset])) {
                (*offset)++;
            }
        }

        *raw_token = std::string_view(m_input.data() + start, *offset - start);
        return true;
    }

    bool skipTokenAt(std::size_t *offset, std::string *detail) const {
        std::string_view ignored;
        if (*offset >= m_input.size()) {
            if (detail != nullptr) {
                *detail = "Unexpected end of input while searching for a raw JSON number token.";
            }
            return false;
        }

        switch (m_input[*offset]) {
            case '{':
                return consumeCharAt(offset, '{', &ignored, detail);
            case '}':
                return consumeCharAt(offset, '}', &ignored, detail);
            case '[':
                return consumeCharAt(offset, '[', &ignored, detail);
            case ']':
                return consumeCharAt(offset, ']', &ignored, detail);
            case '"':
                return consumeStringAt(offset, &ignored, detail);
            case 't':
                return consumeLiteralAt(offset, "true", &ignored, detail);
            case 'f':
                return consumeLiteralAt(offset, "false", &ignored, detail);
            case 'n':
                return consumeLiteralAt(offset, "null", &ignored, detail);
            default:
                if (detail != nullptr) {
                    *detail = "Unable to locate the next raw JSON number token while synchronizing jsoncons events.";
                }
                return false;
        }
    }

    bool skipToNextNumberToken(std::size_t *offset, std::string *detail) const {
        while (true) {
            skipInsignificantAt(offset);
            if (*offset >= m_input.size()) {
                if (detail != nullptr) {
                    *detail = "Unexpected end of input while searching for a raw JSON number token.";
                }
                return false;
            }
            if (m_input[*offset] == '-' || isDigit(m_input[*offset])) {
                return true;
            }
            if (!skipTokenAt(offset, detail)) {
                return false;
            }
        }
    }

    std::string_view m_input;
    std::size_t m_offset{0};
};

std::string_view rawNumberFromContext(std::string_view input,
    jsoncons::staj_event_type event_type, const jsoncons::ser_context &context,
    const jsoncons::staj_event &event, std::string_view scanned_token) {
    const std::size_t begin = context.begin_position();
    if (const std::size_t end = context.end_position();
        begin < end && end <= input.size()) {
        std::string_view candidate(input.data() + begin, end - begin);
        if (tokenMatchesNumericEvent(event_type, candidate)) {
            return candidate;
        }
    }

    if (tokenMatchesNumericEvent(event_type, scanned_token)) {
        return scanned_token;
    }

    if (isNumericStringEvent(event)) {
        std::error_code error;
        jsoncons::string_view decoded = event.get<jsoncons::string_view>(error);
        if (error) {
            return std::string_view();
        }
        if (isValidJsonNumber(std::string_view(decoded.data(), decoded.size()))) {
            return std::string_view(decoded.data(), decoded.size());
        }
        return std::string_view();
    }

    return std::string_view();
}

JsonParseResult emitNumberFromRawToken(std::string_view input, JsonEventSink *sink,
    RawJsonTokenCursor *token_cursor, jsoncons::staj_event_type event_type,
    const jsoncons::ser_context &context, const jsoncons::staj_event &event) {
    std::string_view raw_token;
    std::string sync_detail;
    if (!token_cursor->consumeNextNumberToken(&raw_token, &sync_detail)) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::Continue, sync_detail);
    }
    recordJsonconsTokenSyncStep();
    std::string_view raw_number = rawNumberFromContext(input,
        event_type, context, event, raw_token);
    if (raw_number.empty()) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::Continue,
            "Unable to materialize numeric JSON token from jsoncons backend.");
    }
    return finishSinkCall(sink->on_number(raw_number), "handling a number");
}

JsonParseResult decodeStringEventValue(const jsoncons::staj_event &event,
    const jsoncons::ser_context &context, jsoncons::string_view *decoded) {
    std::error_code error;
    *decoded = event.get<jsoncons::string_view>(error);
    if (error) {
        return fromJsonconsError(error, context);
    }
    return makeResult(JsonParseStatus::Ok);
}

JsonParseResult emitEvent(std::string_view input, JsonEventSink *sink,
    RawJsonTokenCursor *token_cursor, const jsoncons::staj_event &event,
    const jsoncons::ser_context &context) {
    std::error_code error;

    switch (event.event_type()) {
        case jsoncons::staj_event_type::begin_object:
            return finishSinkCall(sink->on_start_object(), "starting an object");
        case jsoncons::staj_event_type::end_object:
            return finishSinkCall(sink->on_end_object(), "ending an object");
        case jsoncons::staj_event_type::begin_array:
            return finishSinkCall(sink->on_start_array(), "starting an array");
        case jsoncons::staj_event_type::end_array:
            return finishSinkCall(sink->on_end_array(), "ending an array");
        case jsoncons::staj_event_type::key: {
            jsoncons::string_view decoded;
            if (JsonParseResult result = decodeStringEventValue(event, context,
                    &decoded); !result.ok()) {
                return result;
            }
            return finishSinkCall(sink->on_key(std::string_view(decoded.data(),
                decoded.size())), "processing an object key");
        }
        case jsoncons::staj_event_type::string_value: {
            jsoncons::string_view decoded;
            if (JsonParseResult result = decodeStringEventValue(event, context,
                    &decoded); !result.ok()) {
                return result;
            }
            if (isNumericStringEvent(event)) {
                std::string sync_detail;
                if (const std::string_view decoded_number(decoded.data(),
                        decoded.size()); isValidJsonNumber(decoded_number)
                    && token_cursor->advanceExactNumber(decoded_number,
                        &sync_detail)) {
                    recordJsonconsTokenExactAdvanceStep();
                    return finishSinkCall(sink->on_number(decoded_number),
                        "handling a number");
                }
                return emitNumberFromRawToken(input, sink, token_cursor,
                    jsoncons::staj_event_type::double_value, context, event);
            }
            return finishSinkCall(sink->on_string(std::string_view(decoded.data(),
                decoded.size())), "handling a string");
        }
        case jsoncons::staj_event_type::null_value:
            return finishSinkCall(sink->on_null(), "handling a null value");
        case jsoncons::staj_event_type::bool_value:
            {
            bool boolean_value = event.get<bool>(error);
            if (error) {
                return fromJsonconsError(error, context);
            }
            return finishSinkCall(sink->on_boolean(boolean_value),
                "handling a boolean");
            }
        case jsoncons::staj_event_type::int64_value:
        case jsoncons::staj_event_type::uint64_value:
        case jsoncons::staj_event_type::double_value:
        case jsoncons::staj_event_type::half_value:
            return emitNumberFromRawToken(input, sink, token_cursor,
                event.event_type(), context, event);
        case jsoncons::staj_event_type::byte_string_value:
            return makeResult(JsonParseStatus::InternalError,
                JsonSinkStatus::Continue,
                "Unsupported byte-string event encountered in jsoncons backend.");
        default:
            return makeResult(JsonParseStatus::InternalError,
                JsonSinkStatus::Continue,
                "Unsupported JSON token type encountered in jsoncons backend.");
    }
}

}  // namespace

JsonParseResult parseDocumentWithJsoncons(const std::string &input,
    JsonEventSink *sink, const JsonBackendParseOptions &options) {
    if (sink == nullptr) {
        return makeResult(JsonParseStatus::InternalError,
            JsonSinkStatus::InternalError, "JSON event sink is null.");
    }

    jsoncons::json_options cursor_options;
    cursor_options.max_nesting_depth(options.technical_max_depth);
    cursor_options.lossless_number(true);
    cursor_options.lossless_bignum(true);

    std::error_code error;
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto cursor_start = std::chrono::steady_clock::now();
    jsoncons::json_string_cursor cursor(input, cursor_options, error);
    recordJsonconsCursorInit(static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - cursor_start).count()));
#else
    jsoncons::json_string_cursor cursor(input, cursor_options, error);
#endif
    if (error) {
        return fromJsonconsError(error, cursor.context());
    }

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto token_cursor_start = std::chrono::steady_clock::now();
    RawJsonTokenCursor token_cursor(input);
    recordJsonconsTokenCursorInit(static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - token_cursor_start).count()));
    const auto event_loop_start = std::chrono::steady_clock::now();
    const auto record_event_loop = [&event_loop_start]() {
        recordJsonconsEventLoop(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - event_loop_start).count()));
    };
    const auto finish_with_event_loop = [&record_event_loop](
        JsonParseResult result) {
        record_event_loop();
        return result;
    };
#else
    RawJsonTokenCursor token_cursor(input);
#endif

    while (!cursor.done()) {
        if (JsonParseResult result = emitEvent(input, sink, &token_cursor,
                cursor.current(), cursor.context()); !result.ok()) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
            return finish_with_event_loop(result);
#endif
            return result;
        }

        cursor.next(error);
        if (error) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
            return finish_with_event_loop(
                fromJsonconsError(error, cursor.context()));
#endif
            return fromJsonconsError(error, cursor.context());
        }
    }

    cursor.check_done(error);
    if (error) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
        return finish_with_event_loop(fromJsonconsError(error,
            cursor.context()));
#endif
        return fromJsonconsError(error, cursor.context());
    }

#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    record_event_loop();
#endif
    return makeResult(JsonParseStatus::Ok);
}

}  // namespace modsecurity::RequestBodyProcessor
