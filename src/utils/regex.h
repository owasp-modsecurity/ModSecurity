/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <vector>

#ifndef SRC_UTILS_REGEX_H_
#define SRC_UTILS_REGEX_H_


namespace modsecurity {
namespace Utils {

#define OVECCOUNT 900

class SMatch {
 public:
    SMatch() :
	m_match(),
	m_offset(0) { }

    SMatch(const std::string &match, size_t offset) :
	m_match(match),
	m_offset(offset) { }

    const std::string& str() const { return m_match; }
    size_t offset() const { return m_offset; }

 private:
    std::string m_match;
    size_t m_offset;
};

struct SMatchCapture {
    SMatchCapture(size_t group, size_t offset, size_t length) :
	m_group(group),
	m_offset(offset),
	m_length(length) { }

    size_t m_group; // E.g. 0 = full match; 6 = capture group 6
    size_t m_offset; // offset of match within the analyzed string
    size_t m_length;
};

class Regex {
 public:
    explicit Regex(const std::string& pattern_);
    ~Regex();

    // m_pc and m_pce can't be easily copied
    Regex(const Regex&) = delete;
    Regex& operator=(const Regex&) = delete;

    std::list<SMatch> searchAll(const std::string& s) const;
    bool searchOneMatch(const std::string& s, std::vector<SMatchCapture>& captures) const;
    bool searchGlobal(const std::string& s, std::vector<SMatchCapture>& captures) const;
    int search(const std::string &s, SMatch *match) const;
    int search(const std::string &s) const;

    const std::string pattern;
 private:
    pcre *m_pc = NULL;
    pcre_extra *m_pce = NULL;
};


static inline int regex_search(const std::string& s, SMatch *match, const Regex& regex) {
    return regex.search(s, match);
}


static inline int regex_search(const std::string& s, const Regex& regex) {
    return regex.search(s);
}


}  // namespace Utils
}  // namespace modsecurity

#endif  // SRC_UTILS_REGEX_H_
