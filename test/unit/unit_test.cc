/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "unit/unit_test.h"

#include <string.h>

#include <sstream>
#include <string>

#include "common/colors.h"
#include "src/utils.h"

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
    i << "}" << std::endl;
    if (this->ret != this->obtained) {
        i << "Expecting: \"" << this->ret << "\" - returned: \"";
        i << this->obtained << "\"" << std::endl;
    }
    if (this->output != this->obtainedOutput) {
        i << "Expecting: \"" << modsecurity::toHexIfNeeded(this->output);
        i << "\" - returned: \"";
        i << modsecurity::toHexIfNeeded(this->obtainedOutput) << "\"";
        i << std::endl;
    }

    return i.str();
}


UnitTest *UnitTest::from_yajl_node(yajl_val &node) {
    size_t num_tests = node->u.object.len;
    UnitTest *u = new UnitTest();

    for (int i = 0; i < num_tests; i++) {
        const char *key = node->u.object.keys[ i ];
        yajl_val val = node->u.object.values[ i ];


        if (strcmp(key, "param") == 0) {
           u->param = YAJL_GET_STRING(val);
        } else if (strcmp(key, "input") == 0) {
           u->input = YAJL_GET_STRING(val);
           /*
            * Converting \\u0000 to \0 due to the following gcc bug:
            * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53690
            *
            */
           replaceAll(&(u->input), "\\0", '\0');
           replaceAll(&(u->input), "\\xe4", '\xe4');
           replaceAll(&(u->input), "\\x03", '\x03');
           replaceAll(&(u->input), "\\xbf", '\xbf');
           replaceAll(&(u->input), "\\xc9", '\xc9');
           replaceAll(&(u->input), "\\x3b", '\x3b');
           replaceAll(&(u->input), "\\xFF", '\xff');
           replaceAll(&(u->input), "\\u0000", '\0');
           replaceAll(&(u->input), "\\u0001", '\u0001');
           replaceAll(&(u->input), "\\u0002", '\u0002');
           replaceAll(&(u->input), "\\u0003", '\u0003');
           replaceAll(&(u->input), "\\u0004", '\u0004');
           replaceAll(&(u->input), "\\u0005", '\u0005');
           replaceAll(&(u->input), "\\u0006", '\u0006');
           replaceAll(&(u->input), "\\u0007", '\u0007');
           replaceAll(&(u->input), "\\b", '\b');
        } else if (strcmp(key, "name") == 0) {
           u->name = YAJL_GET_STRING(val);
        } else if (strcmp(key, "type") == 0) {
           u->type = YAJL_GET_STRING(val);
        } else if (strcmp(key, "ret") == 0) {
           u->ret = YAJL_GET_INTEGER(val);
        } else if (strcmp(key, "output") == 0) {
           u->output = std::string(YAJL_GET_STRING(val));
           /*
            * Converting \\u0000 to \0 due to the following gcc bug:
            * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53690
            *
            */
           // FIXME: Really? replace the hex chars in a smart fashion.
           replaceAll(&(u->output), "\\x00", '\0');
           replaceAll(&(u->output), "\\u0000", '\0');
           replaceAll(&(u->output), "\\xe4", '\xe4');
           replaceAll(&(u->output), "\\x03", '\x03');
           replaceAll(&(u->output), "\\x04", '\x04');
           replaceAll(&(u->output), "\\x07", '\x07');
           replaceAll(&(u->output), "\\x09", '\x09');
           replaceAll(&(u->output), "\\xbf", '\xbf');
           replaceAll(&(u->output), "\\xc9", '\xc9');
           replaceAll(&(u->output), "\\x3b", '\x3b');
           replaceAll(&(u->output), "\\xFF", '\xff');
           replaceAll(&(u->output), "\\0", '\0');
           replaceAll(&(u->output), "\\x90", '\x90');
           replaceAll(&(u->output), "\\x14", '\x14');
           replaceAll(&(u->output), "\\x82", '\x82');
           replaceAll(&(u->output), "\\x9a", '\x9a');
           replaceAll(&(u->output), "\\xbc", '\xbc');
           replaceAll(&(u->output), "\\xfe", '\xfe');
           replaceAll(&(u->output), "\\x89", '\x89');
           replaceAll(&(u->output), "\\xa0", '\xa0');
           replaceAll(&(u->output), "\\xa2", '\xa2');
           replaceAll(&(u->output), "\\xa6", '\xa6');
           replaceAll(&(u->output), "\\xab", '\xab');
           replaceAll(&(u->output), "\\xd4", '\xd4');
           replaceAll(&(u->output), "\\x1d", '\x1d');
           replaceAll(&(u->output), "\\x8c", '\x8c');
           replaceAll(&(u->output), "\\xd9", '\xd9');
           replaceAll(&(u->output), "\\x8f", '\x8f');
           replaceAll(&(u->output), "\\xb2", '\xb2');
           replaceAll(&(u->output), "\\xe9", '\xe9');
           replaceAll(&(u->output), "\\x9e", '\x9e');
           replaceAll(&(u->output), "\\x80", '\x80');
           replaceAll(&(u->output), "\\x98", '\x98');
           replaceAll(&(u->output), "\\xec", '\xec');
           replaceAll(&(u->output), "\\xf8", '\xf8');
           replaceAll(&(u->output), "\\xc1", '\xc1');
           replaceAll(&(u->output), "\\xc3", '\xc3');
           replaceAll(&(u->output), "\\x83", '\x83');
           replaceAll(&(u->output), "\\xaa", '\xaa');
           replaceAll(&(u->output), "\\xa5", '\xa5');
           replaceAll(&(u->output), "\\xb5", '\xb5');
           replaceAll(&(u->output), "\\xd1", '\xd1');
           replaceAll(&(u->output), "\\xde", '\xde');
           replaceAll(&(u->output), "\\xea", '\xea');
           replaceAll(&(u->output), "\\xe6", '\xe6');
           replaceAll(&(u->output), "\\xe7", '\xe7');
           replaceAll(&(u->output), "\\xd3", '\xd3');
           replaceAll(&(u->output), "\\xb4", '\xb4');
           replaceAll(&(u->output), "\\xdf", '\xdf');
           replaceAll(&(u->output), "\\xaf", '\xaf');
           replaceAll(&(u->output), "\\x01", '\x01');
           replaceAll(&(u->output), "\\x16", '\x16');
           replaceAll(&(u->output), "\\x0b", '\x0b');
           replaceAll(&(u->output), "\\x1f", '\x1f');
           replaceAll(&(u->output), "\\x83", '\x83');
           replaceAll(&(u->output), "\\xd2", '\xd2');
        }
    }

    return u;
}

}  // namespace modsecurity_test
