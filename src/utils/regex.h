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

    // to_string is convenience method for returning string for the match.
    // You must supply the same string that was used to obtain the match,
    // as offset would be invalid otherwise.
    std::string to_string(const std::string &matched_string) const {
        return matched_string.substr(m_offset, m_length);
    }
};

class Regex {
 public:
    typedef std::vector<SMatchCapture> match_type;

    explicit Regex(const std::string& pattern_);
    ~Regex();

    // m_pc and m_pce can't be easily copied
    Regex(const Regex&) = delete;
    Regex& operator=(const Regex&) = delete;

    bool searchOneMatch(const std::string& s, match_type& captures) const;
    std::vector<match_type> searchAllMatches(const std::string &s) const;
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
