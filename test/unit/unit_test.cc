/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "test/common/colors.h"
#include "src/regex/regex.h"
#include "src/utils/string.h"


namespace modsecurity_test {


std::string string_to_hex(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

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
    modsecurity::regex::Regex re("\\\\x([a-z0-9A-Z]{2})");
    modsecurity::regex::Regex re2("\\\\u([a-z0-9A-Z]{4})");
    modsecurity::regex::RegexMatch match;

    while (re.search(*str, &match)) {
        unsigned int p;
        std::string toBeReplaced = match.str();
        toBeReplaced.erase(0, 2);
        sscanf(toBeReplaced.c_str(), "%3x", &p);
        replaceAll(str, match.str(), p);
    }

    while (re2.search(*str, &match)) {
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

static void print_array(std::stringstream &i,
    const std::vector<std::string> array) {
    i << "[";
    bool first = true;
    for (const auto &s : array) {
        if (first) {
            first = false;
        } else {
            i << ", ";
        }
        i << "\"" << modsecurity::utils::string::toHexIfNeeded(s) << "\"";
    }
    i << "]";
}

std::string UnitTest::print() {
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
    if (this->re_groups.size() != 0) {
        i << "  \"re_groups\": ";
        print_array(i, this->re_groups);
        i << std::endl;

    }
    i << "}" << std::endl;
    if (this->ret != this->obtained) {
        i << "Expecting: \"" << this->ret << "\" - returned: \"";
        i << this->obtained << "\"" << std::endl;
    }
    if (this->output != this->obtainedOutput) {
        i << "Expecting: \"";
        i << modsecurity::utils::string::toHexIfNeeded(this->output);
        i << "\" - returned: \"";
        i << modsecurity::utils::string::toHexIfNeeded(this->obtainedOutput);
        i << "\"";
        i << std::endl;
    }
    if (this->re_groups.size() && this->re_groups != this->obtained_re_groups) {
        i << "Expecting:\n  ";
        print_array(i, this->re_groups);
        i << "\nObtained:\n  ";
        print_array(i, this->obtained_re_groups);
        i << std::endl;
    }

    return i.str();
}


UnitTest *UnitTest::from_yajl_node(const yajl_val &node) {
    size_t num_tests = node->u.object.len;
    UnitTest *u = new UnitTest();

    for (int i = 0; i < num_tests; i++) {
        const char *key = node->u.object.keys[ i ];
        yajl_val val = node->u.object.values[ i ];

        u->skipped = false;
        if (strcmp(key, "param") == 0) {
           u->param = YAJL_GET_STRING(val);
        } else if (strcmp(key, "input") == 0) {
           u->input = YAJL_GET_STRING(val);
           json2bin(&u->input);
        } else if (strcmp(key, "resource") == 0) {
           u->resource = YAJL_GET_STRING(val);
        } else if (strcmp(key, "name") == 0) {
           u->name = YAJL_GET_STRING(val);
        } else if (strcmp(key, "type") == 0) {
           u->type = YAJL_GET_STRING(val);
        } else if (strcmp(key, "ret") == 0) {
           u->ret = YAJL_GET_INTEGER(val);
        } else if (strcmp(key, "output") == 0) {
           u->output = std::string(YAJL_GET_STRING(val));
           json2bin(&u->output);
           /*
            * Converting \\u0000 to \0 due to the following gcc bug:
            * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53690
            *
            */
        } else if (strcmp(key, "re_groups") == 0) {
            auto arr = YAJL_GET_ARRAY(val);
            if (arr == NULL) {
                continue;
            }
            for (int i = 0; i < arr->len; i++) {
                const char *s = YAJL_GET_STRING(arr->values[i]);
                if (s == NULL) continue;
                u->re_groups.push_back(s);
            }
        }
    }

    return u;
}

}  // namespace modsecurity_test
