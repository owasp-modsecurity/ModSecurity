/**
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

#include "src/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <random>
#include <memory>
#include <functional>

#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "modsecurity/modsecurity.h"

namespace ModSecurity {

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);  // Turn the string into a stream.
    std::string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


double random_number(const double from, const double to) {
    std::random_device rd;

    return std::bind(
        std::uniform_real_distribution<>{from, to},
        std::default_random_engine{ rd() })();
}


std::string dash_if_empty(const std::string& str) {
    if (str.empty()) {
        return "-";
    }

    return str;
}


double generate_assay_unique_id() {
    return random_number(0, 100);
}


std::string ascTime(time_t *t) {
    std::string ts = std::ctime(t);
    ts.pop_back();
    return ts;
}


void createDir(std::string dir, int mode) {
#if defined _MSC_VER
    _mkdir(dir.data());
#elif defined __GNUC__
    mkdir(dir.data(), mode);
#endif
}

}  // namespace ModSecurity

