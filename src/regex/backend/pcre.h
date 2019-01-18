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

#include <pcre.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "src/regex/regex_match.h"

#ifndef SRC_REGEX_BACKEND_PCRE_H_
#define SRC_REGEX_BACKEND_PCRE_H_

namespace modsecurity {
namespace regex {
namespace backend {


#define OVECCOUNT 30


class Pcre {
 public:
    explicit Pcre(const std::string& pattern_);
    ~Pcre();

    // m_pc and m_pce can't be easily copied
    Pcre(const Pcre&) = delete;
    Pcre& operator=(const Pcre&) = delete;

    std::list<RegexMatch> searchAll(const std::string& s) const;
    int search(const std::string &s, RegexMatch *m) const;
    int search(const std::string &s) const;

    const std::string pattern;
 private:
    pcre *m_pc = NULL;
    pcre_extra *m_pce = NULL;
};


}  // namespace backend
}  // namespace regex
}  // namespace modsecurity


#endif  // SRC_REGEX_BACKEND_PCRE_H_
