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

#include "test/regression/regression_test.h"

#include <string.h>

#include <sstream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <memory>
#include <utility>

#include "src/utils/json_writer.h"

namespace modsecurity_test {
namespace {

std::string join_strings(const std::vector<std::string> &values) {
    std::stringstream stream;

    for (const auto &entry : values) {
        stream << entry;
    }

    return stream.str();
}

std::vector<std::string> json_array_to_vec_string(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonArray array;
    std::vector<std::string> values;

    if (modsecurity_test::json::get(value.get_array(), &array) == false) {
        return values;
    }

    for (auto entry_result : array) {
        modsecurity_test::json::JsonValue entry;

        if (modsecurity_test::json::get(std::move(entry_result), &entry)
                == false) {
            continue;
        }

        values.push_back(modsecurity_test::json::get_string(entry));
    }

    return values;
}

std::vector<std::pair<std::string, std::string>> json_object_to_map(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;
    std::vector<std::pair<std::string, std::string>> values;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return values;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        values.emplace_back(std::string(key),
            modsecurity_test::json::get_string(child));
    }

    return values;
}

void set_int_from_json(int &dest, std::string_view want_key,
    std::string_view key, modsecurity_test::json::JsonValue value) {
    if (key == want_key) {
        dest = static_cast<int>(modsecurity_test::json::get_integer(value));
    }
}

void set_opt_int_from_json(std::optional<int> &dest, std::string_view want_key,
    std::string_view key, modsecurity_test::json::JsonValue value) {
    if (key == want_key) {
        dest = static_cast<int>(modsecurity_test::json::get_integer(value));
    }
}

void set_string_from_json(std::string &dest, std::string_view want_key,
    std::string_view key, modsecurity_test::json::JsonValue value) {
    if (key == want_key) {
        dest = modsecurity_test::json::get_string(value);
    }
}

std::unique_ptr<RegressionTest> make_empty_regression_test() {
    auto test = std::make_unique<RegressionTest>();
    test->enabled = 0;
    test->version_min = 0;
    test->clientPort = 0;
    test->serverPort = 0;
    test->http_code = 200;
    return test;
}

void append_headers(modsecurity::utils::JsonWriter *writer,
    const std::vector<std::pair<std::string, std::string>> &headers) {
    writer->start_object();
    for (const auto &[name, value] : headers) {
        writer->key(name);
        writer->string(value);
    }
    writer->end_object();
}

void append_string_array(modsecurity::utils::JsonWriter *writer,
    std::vector<std::string> values) {
    if (values.empty()) {
        values.emplace_back("");
    }

    writer->start_array();
    for (const auto &value : values) {
        writer->string(value);
    }
    writer->end_array();
}

}  // namespace

std::string RegressionTest::print() {
    std::stringstream i;

#if 0
    i << KRED << "Test failed." << RESET << " From: " \
    i << this->filename << std::endl;
    i << "{" << std::endl;
    i << "  \"ret\": \"" << this->ret << "\"" << std::endl;
    i << "  \"type\": \"" << this->type << "\"" << std::endl;
    i << "  \"name\": \"" << this->name << "\"" << std::endl;
    i << "  \"input\": \"" << this->input << "\"" << std::endl;
    i << "  \"param\": \"" << this->param << "\"" << std::endl;
    i << "}" << std::endl;
    i << "Expecting: " << this->ret << " - operator returned: " << \
        this->obtained << std::endl;
#endif
    return i.str();
}

std::unique_ptr<RegressionTest> RegressionTest::from_json_document(
    modsecurity_test::json::JsonDocument *document) {
    modsecurity_test::json::JsonValue root;

    if (modsecurity_test::json::get(document->get_value(), &root) == false) {
        return make_empty_regression_test();
    }

    modsecurity_test::json::JsonType type;
    if (modsecurity_test::json::get(root.type(), &type) == false) {
        return make_empty_regression_test();
    }

    if (type == modsecurity_test::json::JsonType::Array) {
        modsecurity_test::json::JsonArray tests;
        if (modsecurity_test::json::get(root.get_array(), &tests) == false) {
            return make_empty_regression_test();
        }

        for (auto test_result : tests) {
            modsecurity_test::json::JsonValue test;
            if (modsecurity_test::json::get(std::move(test_result), &test)
                    == false) {
                continue;
            }

            return from_json_value(test);
        }

        return make_empty_regression_test();
    }

    return from_json_value(root);
}

std::unique_ptr<RegressionTest> RegressionTest::from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;
    auto test = make_empty_regression_test();

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return test;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        set_int_from_json(test->enabled, "enabled", key, child);
        set_int_from_json(test->version_min, "version_min", key, child);
        set_opt_int_from_json(test->version_max, "version_max", key, child);
        set_string_from_json(test->title, "title", key, child);
        set_string_from_json(test->url, "url", key, child);
        set_string_from_json(test->resource, "resource", key, child);
        set_opt_int_from_json(test->github_issue, "github_issue", key, child);

        if (key == "client") {
            test->update_client_from_json_value(child);
        } else if (key == "server") {
            test->update_server_from_json_value(child);
        } else if (key == "request") {
            test->update_request_from_json_value(child);
        } else if (key == "response") {
            test->update_response_from_json_value(child);
        } else if (key == "expected") {
            test->update_expected_from_json_value(child);
        } else if (key == "rules") {
            test->update_rules_from_json_value(child);
        }
    }

    test->name = test->title;
    return test;
}

void RegressionTest::update_client_from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        set_string_from_json(clientIp, "ip", key, child);
        set_int_from_json(clientPort, "port", key, child);
    }
}

void RegressionTest::update_server_from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        set_string_from_json(serverIp, "ip", key, child);
        set_int_from_json(serverPort, "port", key, child);
        set_string_from_json(hostname, "hostname", key, child);
    }
}

void RegressionTest::update_request_from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        set_string_from_json(uri, "uri", key, child);
        set_string_from_json(method, "method", key, child);
        if (key == "http_version") {
            httpVersion = modsecurity_test::json::get_raw_number(child);
        } else if (key == "headers") {
            request_headers = json_object_to_map(child);
        } else if (key == "body") {
            request_body_lines = json_array_to_vec_string(child);
            request_body = join_strings(request_body_lines);
        }
    }
}

void RegressionTest::update_response_from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        if (key == "headers") {
            response_headers = json_object_to_map(child);
        } else if (key == "body") {
            response_body_lines = json_array_to_vec_string(child);
            response_body = join_strings(response_body_lines);
        }
        set_string_from_json(response_protocol, "protocol", key, child);
    }
}

void RegressionTest::update_expected_from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(std::move(field_result), &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        set_string_from_json(audit_log, "audit_log", key, child);
        set_string_from_json(debug_log, "debug_log", key, child);
        set_string_from_json(error_log, "error_log", key, child);
        set_int_from_json(http_code, "http_code", key, child);
        set_string_from_json(redirect_url, "redirect_url", key, child);
        set_string_from_json(parser_error, "parser_error", key, child);
    }
}

void RegressionTest::update_rules_from_json_value(
    modsecurity_test::json::JsonValue value) {
    std::stringstream stream;

    rules_lines = json_array_to_vec_string(value);
    for (const auto &line : rules_lines) {
        stream << line << "\n";
    }

    rules = stream.str();
}


constexpr char ascii_tolower(char c) {
    return 'A' <= c && c <= 'Z' ? (c + ('a' - 'A')) : c;
}

bool iequals_ascii(std::string_view a, std::string_view b) {
    return a.size() == b.size() &&
        std::equal(a.begin(), a.end(), b.begin(), b.end(),
            [](char x, char y) {
                return ascii_tolower(x) == ascii_tolower(y);
            });
}

static bool has_chunked_header(const std::vector<std::pair<std::string, std::string>> &headers) {
    return std::any_of(std::begin(headers), std::end(headers),
        [](const auto &header) {
            const auto &[name, value]{header};
            return iequals_ascii(name, "Transfer-Encoding") && iequals_ascii(value, "chunked");
        });
}

static void update_content_length(std::vector<std::pair<std::string, std::string>> &headers, size_t length) {
    if (has_chunked_header(headers)) {
        return;
    }

    bool has_content_length = false;
    for (auto &[name, value] : headers) {
        if (iequals_ascii(name, "Content-Length")) {
            value = std::to_string(length);
            has_content_length = true;
        }
    }
    if (!has_content_length) {
        headers.emplace_back(std::pair{"Content-Length", std::to_string(length)});
    }
}

void RegressionTest::update_content_lengths() {
    update_content_length(request_headers, request_body.size());
    update_content_length(response_headers, response_body.size());
}

std::unique_ptr<RegressionTests> RegressionTests::from_json_document(
    modsecurity_test::json::JsonDocument *document) {
    modsecurity_test::json::JsonValue root;

    if (modsecurity_test::json::get(document->get_value(), &root) == false) {
        return std::make_unique<RegressionTests>();
    }

    return from_json_value(root);
}

std::unique_ptr<RegressionTests> RegressionTests::from_json_value(
    modsecurity_test::json::JsonValue value) {
    auto tests = std::make_unique<RegressionTests>();
    modsecurity_test::json::JsonType type;

    if (modsecurity_test::json::get(value.type(), &type) == false) {
        return tests;
    }

    if (type == modsecurity_test::json::JsonType::Array) {
        modsecurity_test::json::JsonArray array;

        if (modsecurity_test::json::get(value.get_array(), &array) == false) {
            return tests;
        }

        for (auto test_result : array) {
            modsecurity_test::json::JsonValue test_value;
            if (modsecurity_test::json::get(std::move(test_result), &test_value)
                    == false) {
                continue;
            }
            tests->tests.emplace_back(
                std::move(RegressionTest::from_json_value(test_value)));
        }
        return tests;
    }

    if (type == modsecurity_test::json::JsonType::Object) {
        tests->tests.emplace_back(std::move(RegressionTest::from_json_value(value)));
    }

    return tests;
}

void RegressionTests::update_content_lengths() {
    for (auto & test : tests) {
        test->update_content_lengths();
    }
}

std::string RegressionTests::toJSON() const {
    modsecurity::utils::JsonWriter writer(true, "  ");

    const auto addString = [&writer](std::string_view key,
        const std::string &value) {
        writer.key(key);
        writer.string(value);
    };
    const auto addStringIfNonEmpty = [&writer, &addString](
        std::string_view key, const std::string &value) {
        if (value.empty() == false) {
            addString(key, value);
        }
    };
    const auto addInteger = [&writer](std::string_view key, int value) {
        writer.key(key);
        writer.integer(value);
    };
    const auto addOptionalInteger = [&writer](std::string_view key,
        const std::optional<int> &value) {
        if (value.has_value()) {
            writer.key(key);
            writer.integer(value.value());
        }
    };

    writer.start_array();
    for (const auto &t : tests) {
        writer.start_object();
        addInteger("enabled", t->enabled);
        addInteger("version_min", t->version_min);
        addOptionalInteger("version_max", t->version_max);
        addString("title", t->title);
        addStringIfNonEmpty("url", t->url);
        addStringIfNonEmpty("resource", t->resource);
        addOptionalInteger("github_issue", t->github_issue);

        writer.key("client");
        writer.start_object();
        addString("ip", t->clientIp);
        addInteger("port", t->clientPort);
        writer.end_object();

        writer.key("server");
        writer.start_object();
        addString("ip", t->serverIp);
        addInteger("port", t->serverPort);
        writer.end_object();

        writer.key("request");
        writer.start_object();
        writer.key("headers");
        append_headers(&writer, t->request_headers);
        addString("uri", t->uri);
        addString("method", t->method);
        if (!t->httpVersion.empty()) {
            writer.key("http_version");
            writer.number(t->httpVersion);
        }

        writer.key("body");
        append_string_array(&writer, t->request_body_lines);
        writer.end_object();

        writer.key("response");
        writer.start_object();
        writer.key("headers");
        append_headers(&writer, t->response_headers);
        writer.key("body");
        append_string_array(&writer, t->response_body_lines);
        addStringIfNonEmpty("protocol", t->response_protocol);
        writer.end_object();

        writer.key("expected");
        writer.start_object();
        addStringIfNonEmpty("audit_log", t->audit_log);
        addStringIfNonEmpty("debug_log", t->debug_log);
        addStringIfNonEmpty("error_log", t->error_log);
        addInteger("http_code", t->http_code);
        addStringIfNonEmpty("redirect_url", t->redirect_url);
        addStringIfNonEmpty("parser_error", t->parser_error);
        writer.end_object();

        writer.key("rules");
        append_string_array(&writer, t->rules_lines);

        writer.end_object();
    }
    writer.end_array();

    return writer.to_string();
}

}  // namespace modsecurity_test
