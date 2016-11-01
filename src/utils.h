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
    int urldecode_nonstrict_inplace(unsigned char *input,
    uint64_t input_len, int *invalid_count, int *changed);
    double random_number(const double from, const double to);
    double generate_transaction_unique_id();
    void createDir(std::string dir, int mode);
    std::string uri_decode(const std::string & sSrc);
    double cpu_seconds(void);
    int js_decode_nonstrict_inplace(unsigned char *input, int64_t input_len);
    unsigned char x2c(unsigned char *what);
    unsigned char *c2x(unsigned what, unsigned char *where);
    int css_decode_inplace(unsigned char *input, int64_t input_len);
    unsigned char xsingle2c(unsigned char *what);
    int html_entities_decode_inplace(unsigned char *input, int input_len);
    int normalize_path_inplace(unsigned char *input, int input_len,
        int win, int *changed);
    int urldecode_uni_nonstrict_inplace_ex(Transaction *transaction,
        unsigned char *input, int64_t input_len, int *changed);
    std::string phase_name(int x);
    std::vector<std::string> expandEnv(const std::string& var, int flags);
    std::string find_resource(const std::string& file,
        const std::string& param);
}  // namespace modsecurity


#endif  // SRC_UTILS_H_
