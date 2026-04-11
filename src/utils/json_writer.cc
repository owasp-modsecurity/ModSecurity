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

#include "src/utils/json_writer.h"

#include <cstdio>
#include <utility>

namespace modsecurity {
namespace utils {

JsonWriter::JsonWriter(bool pretty, std::string indent)
    : m_output(),
      m_stack(),
      m_pretty(pretty),
      m_indent(std::move(indent)) { }

void JsonWriter::start_object() {
    begin_container(ContainerType::Object, '{');
}

void JsonWriter::end_object() {
    end_container(ContainerType::Object, '}');
}

void JsonWriter::start_array() {
    begin_container(ContainerType::Array, '[');
}

void JsonWriter::end_array() {
    end_container(ContainerType::Array, ']');
}

void JsonWriter::key(std::string_view value) {
    Frame &frame = m_stack.back();

    if (!frame.first) {
        m_output.push_back(',');
    }
    if (m_pretty) {
        newline_and_indent(m_stack.size());
    }

    write_escaped_string(value);
    m_output.append(m_pretty ? ": " : ":");

    frame.first = false;
    frame.expecting_key = false;
}

void JsonWriter::string(std::string_view value) {
    before_value();
    write_escaped_string(value);
}

void JsonWriter::number(std::string_view raw) {
    before_value();
    m_output.append(raw.data(), raw.size());
}

void JsonWriter::integer(int64_t value) {
    before_value();
    m_output.append(std::to_string(value));
}

void JsonWriter::boolean(bool value) {
    before_value();
    m_output.append(value ? "true" : "false");
}

void JsonWriter::null() {
    before_value();
    m_output.append("null");
}

const std::string& JsonWriter::str() const {
    return m_output;
}

const std::string& JsonWriter::to_string() const {
    return m_output;
}

void JsonWriter::before_value() {
    if (m_stack.empty()) {
        return;
    }

    Frame &frame = m_stack.back();
    if (frame.type == ContainerType::Array) {
        if (!frame.first) {
            m_output.push_back(',');
        }
        if (m_pretty) {
            newline_and_indent(m_stack.size());
        }
        frame.first = false;
        return;
    }

    frame.expecting_key = true;
}

void JsonWriter::after_container_end() {
    if (m_stack.empty()) {
        return;
    }

    Frame &frame = m_stack.back();
    if (frame.type == ContainerType::Object) {
        frame.expecting_key = true;
    }
}

void JsonWriter::begin_container(ContainerType type, char token) {
    before_value();
    m_output.push_back(token);
    m_stack.push_back(Frame{
        type,
        true,
        type == ContainerType::Object
    });
}

void JsonWriter::end_container(ContainerType expected, char token) {
    Frame frame = m_stack.back();
    m_stack.pop_back();

    if (frame.type == expected && !frame.first && m_pretty) {
        newline_and_indent(m_stack.size());
    }

    m_output.push_back(token);
    after_container_end();
}

void JsonWriter::newline_and_indent(std::size_t depth) {
    m_output.push_back('\n');
    for (std::size_t i = 0; i < depth; ++i) {
        m_output.append(m_indent);
    }
}

void JsonWriter::write_escaped_string(std::string_view value) {
    static const char *kHex = "0123456789abcdef";
    char unicode_escape[] = {'\\', 'u', '0', '0', '0', '0', '\0'};

    m_output.push_back('"');
    for (const unsigned char c : value) {
        switch (c) {
            case '"':
                m_output.append("\\\"");
                break;
            case '\\':
                m_output.append("\\\\");
                break;
            case '\b':
                m_output.append("\\b");
                break;
            case '\f':
                m_output.append("\\f");
                break;
            case '\n':
                m_output.append("\\n");
                break;
            case '\r':
                m_output.append("\\r");
                break;
            case '\t':
                m_output.append("\\t");
                break;
            default:
                if (c < 0x20) {
                    unicode_escape[4] = kHex[(c >> 4) & 0x0f];
                    unicode_escape[5] = kHex[c & 0x0f];
                    m_output.append(unicode_escape, 6);
                } else {
                    m_output.push_back(static_cast<char>(c));
                }
                break;
        }
    }
    m_output.push_back('"');
}

}  // namespace utils
}  // namespace modsecurity
