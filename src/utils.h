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

#include "modsecurity/modsecurity.h"

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define VALID_HEX(X) (((X >= '0') && (X <= '9')) || \
    ((X >= 'a') && (X <= 'f')) || ((X >= 'A') && (X <= 'F')))
#define ISODIGIT(X) ((X >= '0') && (X <= '7'))
#define NBSP 160


namespace modsecurity {
    std::vector<std::string> split(std::string str, char delimiter);
    int urldecode_nonstrict_inplace(unsigned char *input,
    uint64_t input_len, int *invalid_count, int *changed);
    double random_number(const double from, const double to);
    double generate_transaction_unique_id();
    std::string ascTime(time_t *t);
    void createDir(std::string dir, int mode);
    std::string dash_if_empty(const std::string& str);
    void chomp(std::string *str);
    std::string uri_decode(const std::string & sSrc);
    std::string tolower(std::string str);
    std::string toupper(std::string str);
    double cpu_seconds(void);
    int js_decode_nonstrict_inplace(unsigned char *input, int64_t input_len);
    unsigned char x2c(unsigned char *what);
    unsigned char *c2x(unsigned what, unsigned char *where);
    int css_decode_inplace(unsigned char *input, int64_t input_len);
    unsigned char xsingle2c(unsigned char *what);
    int html_entities_decode_inplace(unsigned char *input, int input_len);
    int normalize_path_inplace(unsigned char *input, int input_len,
        int win, int *changed);
    std::string string_to_hex(const std::string& input);
    int urldecode_uni_nonstrict_inplace_ex(Transaction *transaction,
        unsigned char *input, int64_t input_len, int *changed);
    std::string phase_name(int x);
    std::string limitTo(int amount, const std::string &str);
    std::string toHexIfNeeded(const std::string &str);
    std::vector<std::string> expandEnv(const std::string& var, int flags);
    std::string find_resource(const std::string& file,
        const std::string& param);
}  // namespace modsecurity


#endif  // SRC_UTILS_H_
