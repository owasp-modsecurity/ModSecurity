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

/*
 * Enforce using PCRE for regular expressions instead of C++ regex library.
 *
 * Since regex library proven to be unusable for gcc 4.8 and earlier versions,
 * this define is required to build workable version of executable for CentOS 7,
 * RHRL 7, Ubuntu 14.04 and SUSE Linux 12.
 */
#define PCRE_ONLY	1

#include "unit/unit_test.h"

#include <string.h>
#ifdef PCRE_ONLY
#include <pcre.h>
#endif

#include <sstream>
#include <string>
#include <iostream>
#include <iterator>
#ifndef PCRE_ONLY
#include <regex>
#endif
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

#ifdef PCRE_ONLY
void replaceAll(std::string *s, char *search,
    const char replace) {
    for (size_t pos = 0; ; pos += 0) {
        pos = s->find(search, pos);
        if (pos == std::string::npos) {
            break;
        }
        s->erase(pos, strlen(search));
        s->insert(pos, &replace, 1);
    }
}
#else
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
#endif


void json2bin(std::string *str) {
#ifdef PCRE_ONLY
    pcre *re;
    const char *emsg;
    int ov[5], eoffs, rc;
    char u[7], *cptr;

    re = pcre_compile("\\\\x([a-z0-9A-Z]{2})", 0, &emsg, &eoffs, NULL);
    if (!re)
	return;

    while ((rc = pcre_exec(re, NULL, str->c_str(), str->length(), 0, 0, ov, 5)) >= 0) {
	unsigned int p;

	memset(u, 0, sizeof(u));
	cptr = (char *) str->c_str();  cptr += ov[0];
	for (p = 0; p < (ov[1]-ov[0]); p++) { u[p] = *(cptr+p); }

	sscanf((char *)(u+2), "%x", &p);
	replaceAll(str, (char *)u, p);
    }
    pcre_free(re);

    re = pcre_compile("\\\\u([a-z0-9A-Z]{4})", 0, &emsg, &eoffs, NULL);
    if (!re)
	return;

    while ((rc = pcre_exec(re, NULL, str->c_str(), str->length(), 0, 0, ov, 5)) >= 0) {
	unsigned int p;

	memset(u, 0, sizeof(u));
	cptr = (char *) str->c_str();  cptr += ov[0];
	for (p = 0; p < (ov[1]-ov[0]); p++) { u[p] = *(cptr+p); }

	sscanf((char *)(u+2), "%4x", &p);
	replaceAll(str, (char *)u, p);
    }
    pcre_free(re);

#else
    std::regex re("\\\\x([a-z0-9A-Z]{2})");
    std::regex re2("\\\\u([a-z0-9A-Z]{4})");
    std::smatch match;

    while (std::regex_search(*str, match, re) && match.size() > 1) {
        unsigned int p;
        std::string toBeReplaced = match.str();
        toBeReplaced.erase(0, 2);
        sscanf(toBeReplaced.c_str(), "%x", &p);
        replaceAll(str, match.str(), p);
    }

    while (std::regex_search(*str, match, re2) && match.size() > 1) {
        unsigned int p;
        std::string toBeReplaced = match.str();
        toBeReplaced.erase(0, 2);
        sscanf(toBeReplaced.c_str(), "%4x", &p);
        replaceAll(str, match.str(), p);
    }
#endif

    /*
    replaceAll(str, "\\0", '\0');
    replaceAll(str, "\\b", '\b');
    replaceAll(str, "\\t", '\t');
    replaceAll(str, "\\n", '\n');
    replaceAll(str, "\\r", '\r');
    */
//    replaceAll(str, "\\f", '\f');
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
           json2bin(&u->input);
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
        }
    }

    return u;
}

}  // namespace modsecurity_test
