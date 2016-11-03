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

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#ifndef SRC_UTILS_MSC_STRING_H_
#define SRC_UTILS_MSC_STRING_H_

#define VALID_HEX(X) (((X >= '0') && (X <= '9')) || \
    ((X >= 'a') && (X <= 'f')) || ((X >= 'A') && (X <= 'F')))
#define ISODIGIT(X) ((X >= '0') && (X <= '7'))
#define NBSP 160


namespace modsecurity {
namespace utils {

const char HEX2DEC[256] = {
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 1 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 2 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 3 */  0,  1,  2,  3,   4,  5,  6,  7,   8,  9, -1, -1,  -1, -1, -1, -1,

    /* 4 */ -1, 10, 11, 12,  13, 14, 15, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 5 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 6 */ -1, 10, 11, 12,  13, 14, 15, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 7 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,

    /* 8 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 9 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* A */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* B */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,

    /* C */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* D */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* E */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* F */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1
};


class String {
 public:
    static std::string ascTime(time_t *t);
    static std::string dash_if_empty(const char *str);
    static std::string dash_if_empty(const std::string *str);
    static std::string limitTo(int amount, const std::string &str);
    static std::string removeBracketsIfNeeded(std::string a);
    static std::string string_to_hex(const std::string& input);
    static std::string toHexIfNeeded(const std::string &str);
    static std::string tolower(std::string str);
    static std::string toupper(std::string str);
    static std::vector<std::string> split(std::string str, char delimiter);
    static void chomp(std::string *str);
};

unsigned char x2c(unsigned char *what);
unsigned char xsingle2c(unsigned char *what);
unsigned char *c2x(unsigned what, unsigned char *where);

}  // namespace utils
}  // namespace modsecurity

#endif  // SRC_UTILS_MSC_STRING_H_
