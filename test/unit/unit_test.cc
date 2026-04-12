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

#include "test/unit/unit_test.h"

#include <string.h>

#include <sstream>
#include <string>
#include <iostream>
#include <iterator>
#include <memory>

#include "test/common/colors.h"
#include "src/utils/regex.h"
#include "src/utils/string.h"


namespace modsecurity_test {
namespace {

std::unique_ptr<UnitTest> make_empty_unit_test() {
    auto test = std::make_unique<UnitTest>();
    test->ret = 0;
    test->skipped = false;
    return test;
}

}  // namespace


void replaceAll(std::string *s, const std::string &search,
    const char replace) {
    for (size_t pos = 0; ; pos += 0) {
        pos = s->find(search, pos);
        if (pos == std::string::npos) {
            break;
        }
        s->erase(pos, search.length());
        s->insert(pos, &replace, 1);
    }
}


void json2bin(std::string *str) {
    modsecurity::Utils::Regex re("\\\\x([a-z0-9A-Z]{2})");
    modsecurity::Utils::Regex re2("\\\\u([a-z0-9A-Z]{4})");
    modsecurity::Utils::SMatch match;

    while (modsecurity::Utils::regex_search(*str, &match, re)) {
        unsigned int p;
        std::string toBeReplaced = match.str();
        toBeReplaced.erase(0, 2);
        sscanf(toBeReplaced.c_str(), "%3x", &p);
        replaceAll(str, match.str(), p);
    }

    while (modsecurity::Utils::regex_search(*str, &match, re2)) {
        unsigned int p;
        std::string toBeReplaced = match.str();
        toBeReplaced.erase(0, 2);
        sscanf(toBeReplaced.c_str(), "%4x", &p);
        replaceAll(str, match.str(), p);
    }

    /*
    replaceAll(str, "\\0", '\0');
    replaceAll(str, "\\b", '\b');
    replaceAll(str, "\\t", '\t');
    replaceAll(str, "\\n", '\n');
    replaceAll(str, "\\r", '\r');
    */
//    replaceAll(str, "\\f", '\f');
}


std::string UnitTest::print() const {
    std::stringstream i;

    i << KRED << "Test failed." << RESET;
    i << " From: " << this->filename << std::endl;
    i << "{" << std::endl;
    i << "  \"ret\": \"" << this->ret << "\"" << std::endl;
    i << "  \"type\": \"" << this->type << "\"" << std::endl;
    i << "  \"name\": \"" << this->name << "\"" << std::endl;
    i << "  \"input\": \"" << this->input << "\"" << std::endl;
    i << "  \"param\": \"" << this->param << "\"" << std::endl;
    i << "  \"output\": \"" << this->output << "\"" << std::endl;
    i << "}" << std::endl;
    if (this->ret != this->result.ret) {
        i << "Expecting: \"" << this->ret << "\" - returned: \"";
        i << this->result.ret << "\"" << std::endl;
    }
    if (this->output != this->result.output) {
        i << "Expecting: \"";
        i << modsecurity::utils::string::toHexIfNeeded(this->output);
        i << "\" - returned: \"";
        i << modsecurity::utils::string::toHexIfNeeded(this->result.output);
        i << "\"";
        i << std::endl;
    }

    return i.str();
}


std::unique_ptr<UnitTest> UnitTest::from_json_document(
    const modsecurity_test::json::JsonDocument *document) {
    modsecurity_test::json::JsonValue root;

    if (modsecurity_test::json::get(document->get_value(), &root) == false) {
        return make_empty_unit_test();
    }

    modsecurity_test::json::JsonType type;
    if (modsecurity_test::json::get(root.type(), &type) == false) {
        return make_empty_unit_test();
    }

    if (type == modsecurity_test::json::JsonType::Array) {
        modsecurity_test::json::JsonArray tests;
        if (modsecurity_test::json::get(root.get_array(), &tests) == false) {
            return make_empty_unit_test();
        }

        for (auto test_result : tests) {
            modsecurity_test::json::JsonValue test_value;
            if (modsecurity_test::json::get(std::move(test_result),
                    &test_value) == false) {
                continue;
            }

            return from_json_value(test_value);
        }

        return make_empty_unit_test();
    }

    return from_json_value(root);
}

std::unique_ptr<UnitTest> UnitTest::from_json_value(
    modsecurity_test::json::JsonValue value) {
    modsecurity_test::json::JsonObject object;
    auto u = make_empty_unit_test();

    if (modsecurity_test::json::get(value.get_object(), &object) == false) {
        return u;
    }

    for (auto field_result : object) {
        modsecurity_test::json::JsonField field;
        std::string_view key;
        modsecurity_test::json::JsonValue child;

        if (modsecurity_test::json::get(field_result, &field)
                == false) {
            continue;
        }
        if (modsecurity_test::json::get(field.unescaped_key(), &key) == false) {
            continue;
        }
        child = field.value();

        if (key == "param") {
            u->param = modsecurity_test::json::get_string(child);
        } else if (key == "input") {
            u->input = modsecurity_test::json::get_string(child);
            json2bin(&u->input);
        } else if (key == "resource") {
            u->resource = modsecurity_test::json::get_string(child);
        } else if (key == "name") {
            u->name = modsecurity_test::json::get_string(child);
        } else if (key == "type") {
            u->type = modsecurity_test::json::get_string(child);
        } else if (key == "ret") {
            u->ret = static_cast<int>(modsecurity_test::json::get_integer(child));
        } else if (key == "output") {
            u->output = modsecurity_test::json::get_string(child);
            json2bin(&u->output);
            /*
             * Converting \\u0000 to \0 due to the following gcc bug:
             * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53690
             *
             */
        }
    }

    return u;
}

}  // namespace modsecurity_test
