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

#include <iostream>
#include <fstream>
#include <string>

#ifndef SRC_UTILS_REGEX_H_
#define SRC_UTILS_REGEX_H_


namespace ModSecurity {
namespace Utils {


class Regex {
 public:
    explicit Regex(const std::string& pattern_);
    std::string pattern;
};


class SMatch {
 public:
    SMatch() : size_(0) { }
    size_t size() { return size_; }
    int size_;
};


int regex_search(const std::string& s, SMatch *m,
    const Regex& regex);

int regex_search(const std::string& s, Regex r);



}  // namespace Utils
}  // namespace ModSecurity

#endif  // SRC_UTILS_REGEX_H_
