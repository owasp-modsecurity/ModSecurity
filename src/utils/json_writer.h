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

#ifndef SRC_UTILS_JSON_WRITER_H_
#define SRC_UTILS_JSON_WRITER_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace modsecurity::utils {

class JsonWriter {
 public:
    explicit JsonWriter(bool pretty = false, std::string indent = "  ");

    void start_object();
    void end_object();
    void start_array();
    void end_array();

    void key(std::string_view value);
    void string(std::string_view value);
    void number(std::string_view raw);
    void integer(int64_t value);
    void boolean(bool value);
    void null();

    const std::string& str() const;
    const std::string& to_string() const;

 private:
    enum class ContainerType {
        Object,
        Array
    };

    struct Frame {
        ContainerType type;
        bool first;
        bool expecting_key;
    };

    void before_value();
    void after_container_end();
    void begin_container(ContainerType type, char token);
    void end_container(ContainerType expected, char token);
    void newline_and_indent(std::size_t depth);
    void write_escaped_string(std::string_view value);

    std::string m_output;
    std::vector<Frame> m_stack;
    bool m_pretty;
    std::string m_indent;
};

}  // namespace modsecurity::utils

#endif  // SRC_UTILS_JSON_WRITER_H_
