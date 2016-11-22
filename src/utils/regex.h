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

#ifndef SRC_UTILS_REGEX_H_
#define SRC_UTILS_REGEX_H_


namespace modsecurity {
namespace Utils {

#define OVECCOUNT 30

class SMatch {
 public:
    SMatch() : size_(0) { }
    size_t size() const { return size_; }
    std::string str() const { return match; }
    int size_;
    std::string match;
};


class Regex {
 public:
    explicit Regex(const std::string& pattern_);
    ~Regex();
    std::string pattern;
    pcre *m_pc = NULL;
    pcre_extra *m_pce = NULL;
    int m_ovector[OVECCOUNT];

    std::list<SMatch> searchAll(const std::string& s);
};


int regex_search(const std::string& s, SMatch *m,
    const Regex& regex);

int regex_search(const std::string& s, const Regex& r);



}  // namespace Utils
}  // namespace modsecurity

#endif  // SRC_UTILS_REGEX_H_
