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

#ifndef TEST_COMMON_JSON_H_
#define TEST_COMMON_JSON_H_

#include <jsoncons/json.hpp>

#include <fstream>
#include <iterator>
#include <string>
#include <string_view>

namespace modsecurity_test {
namespace json {

enum class JsonType {
    Object,
    Array,
    String,
    Number,
    Boolean,
    Null,
    Unknown
};

class JsonValue;

class JsonField {
 public:
    JsonField() = default;
    JsonField(std::string_view key, const jsoncons::json *value)
        : m_key(key),
          m_value(value) { }

    bool valid() const {
        return m_value != nullptr;
    }

    std::string_view unescaped_key() const {
        return m_key;
    }

    JsonValue value() const;

 private:
    std::string_view m_key;
    const jsoncons::json *m_value{nullptr};
};

class JsonArray {
 public:
    class iterator {
     public:
        explicit iterator(jsoncons::json::const_array_iterator iterator)
            : m_iterator(iterator) { }

        JsonValue operator*() const;

        iterator &operator++() {
            ++m_iterator;
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return m_iterator != other.m_iterator;
        }

     private:
        jsoncons::json::const_array_iterator m_iterator;
    };

    JsonArray() = default;
    explicit JsonArray(const jsoncons::json *value) : m_value(value) { }

    bool valid() const {
        return m_value != nullptr && m_value->is_array();
    }

    iterator begin() const {
        return iterator(m_value->array_range().begin());
    }

    iterator end() const {
        return iterator(m_value->array_range().end());
    }

 private:
    const jsoncons::json *m_value{nullptr};
};

class JsonObject {
 public:
    class iterator {
     public:
        explicit iterator(jsoncons::json::const_object_iterator iterator)
            : m_iterator(iterator) { }

        JsonField operator*() const {
            const auto &member = *m_iterator;
            return JsonField(std::string_view(member.key().data(),
                member.key().size()), &member.value());
        }

        iterator &operator++() {
            ++m_iterator;
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return m_iterator != other.m_iterator;
        }

     private:
        jsoncons::json::const_object_iterator m_iterator;
    };

    JsonObject() = default;
    explicit JsonObject(const jsoncons::json *value) : m_value(value) { }

    bool valid() const {
        return m_value != nullptr && m_value->is_object();
    }

    iterator begin() const {
        return iterator(m_value->object_range().begin());
    }

    iterator end() const {
        return iterator(m_value->object_range().end());
    }

 private:
    const jsoncons::json *m_value{nullptr};
};

class JsonValue {
 public:
    JsonValue() = default;
    explicit JsonValue(const jsoncons::json *value) : m_value(value) { }

    bool valid() const {
        return m_value != nullptr;
    }

    JsonObject get_object() const {
        return JsonObject(valid() && m_value->is_object() ? m_value : nullptr);
    }

    JsonArray get_array() const {
        return JsonArray(valid() && m_value->is_array() ? m_value : nullptr);
    }

    JsonType type() const {
        if (!valid()) {
            return JsonType::Unknown;
        }
        if (m_value->is_object()) {
            return JsonType::Object;
        }
        if (m_value->is_array()) {
            return JsonType::Array;
        }
        if (m_value->is_string()) {
            return JsonType::String;
        }
        if (m_value->is_bool()) {
            return JsonType::Boolean;
        }
        if (m_value->is_null()) {
            return JsonType::Null;
        }
        return JsonType::Number;
    }

    const jsoncons::json *raw() const {
        return m_value;
    }

 private:
    const jsoncons::json *m_value{nullptr};
};

inline JsonValue JsonField::value() const {
    return JsonValue(m_value);
}

inline JsonValue JsonArray::iterator::operator*() const {
    return JsonValue(&(*m_iterator));
}

class JsonDocument {
 public:
    JsonValue get_value() const {
        return JsonValue(&m_root);
    }

    JsonArray get_array() const {
        return JsonArray(&m_root);
    }

    bool parse(const std::string &input, std::string *error = nullptr) {
        try {
            m_root = jsoncons::json::parse(input);
            return true;
        } catch (const std::exception &exception) {
            if (error != nullptr) {
                error->assign(exception.what());
            }
            return false;
        }
    }

 private:
    jsoncons::json m_root;
};

inline bool get(JsonArray value, JsonArray *target,
    std::string *error = nullptr) {
    (void) error;
    if (!value.valid()) {
        return false;
    }
    *target = value;
    return true;
}

inline bool get(JsonField value, JsonField *target,
    std::string *error = nullptr) {
    (void) error;
    if (!value.valid()) {
        return false;
    }
    *target = value;
    return true;
}

inline bool get(JsonObject value, JsonObject *target,
    std::string *error = nullptr) {
    (void) error;
    if (!value.valid()) {
        return false;
    }
    *target = value;
    return true;
}

inline bool get(JsonType value, JsonType *target,
    std::string *error = nullptr) {
    (void) error;
    *target = value;
    return true;
}

inline bool get(JsonValue value, JsonValue *target,
    std::string *error = nullptr) {
    (void) error;
    if (!value.valid()) {
        return false;
    }
    *target = value;
    return true;
}

inline bool get(std::string_view value, std::string_view *target,
    std::string *error = nullptr) {
    (void) error;
    *target = value;
    return true;
}

inline bool load_document(const std::string &file, JsonDocument *document,
    std::string *error) {
    std::ifstream input(file.c_str());
    std::string buffer;

    if (input.is_open() == false) {
        if (error != nullptr) {
            error->assign("Unable to open JSON file.");
        }
        return false;
    }

    buffer.assign((std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>());
    return document->parse(buffer, error);
}

inline std::string get_string(JsonValue value) {
    if (!value.valid()) {
        return "";
    }

    try {
        return value.raw()->as<std::string>();
    } catch (const std::exception &) {
        return "";
    }
}

inline std::string get_raw_number(JsonValue value) {
    if (!value.valid() || value.raw()->is_number() == false) {
        return "";
    }

    try {
        return value.raw()->as<std::string>();
    } catch (const std::exception &) {
        return "";
    }
}

inline int64_t get_integer(JsonValue value) {
    if (!value.valid()) {
        return 0;
    }

    try {
        return value.raw()->as<int64_t>();
    } catch (const std::exception &) {
        return 0;
    }
}

}  // namespace json
}  // namespace modsecurity_test

#endif  // TEST_COMMON_JSON_H_
