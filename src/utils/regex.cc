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

#include "utils/regex.h"

#include <pcrecpp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include <fstream>
#include <iostream>

#include "utils/geo_lookup.h"

namespace ModSecurity {
namespace Utils {


Regex::Regex(const std::string& pattern_)
    : pattern(pattern_) {
    if (pattern.empty() == true) {
        pattern.assign(".*");
    }
}

int regex_search(const std::string& s, SMatch *match,
    const Regex& regex) {
    std::string m;
    pcrecpp::RE re(regex.pattern,
        pcrecpp::RE_Options(PCRE_DOTALL|PCRE_MULTILINE));

    /** FIXME: Should be not necessary to call PartialMatch twice here. */
    match->size_ = re.PartialMatch(s);
    re.PartialMatch(s, &m);
    match->match = m;

    return match->size_;
}

int regex_search(const std::string& s, Regex regex) {
    pcrecpp::RE re(regex.pattern);
    return re.PartialMatch(s);
}

}  // namespace Utils
}  // namespace ModSecurity
