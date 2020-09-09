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

#include "src/utils/regex.h"

#include <pcre.h>
#include <string>
#include <list>

#include <fstream>
#include <iostream>

#include "src/utils/geo_lookup.h"

#if PCRE_HAVE_JIT
#define pcre_study_opt PCRE_STUDY_JIT_COMPILE
#else
#define pcre_study_opt 0
#endif

namespace modsecurity {
namespace Utils {

// Helper function to tell us if the current config indicates CRLF is a valid newline sequence
bool crlfIsNewline() {
    int d = 0;
    pcre_config(PCRE_CONFIG_NEWLINE, &d);

    unsigned int option_bits = (d == 13)? PCRE_NEWLINE_CR :
        (d == 10)? PCRE_NEWLINE_LF :
        (d == (13<<8 | 10))? PCRE_NEWLINE_CRLF :
        (d == -2)? PCRE_NEWLINE_ANYCRLF :
        (d == -1)? PCRE_NEWLINE_ANY : 0;

    bool crlf_is_newline =
        option_bits == PCRE_NEWLINE_ANY ||
        option_bits == PCRE_NEWLINE_CRLF ||
        option_bits == PCRE_NEWLINE_ANYCRLF;

    return crlf_is_newline;
}

Regex::Regex(const std::string& pattern_)
    : pattern(pattern_.empty() ? ".*" : pattern_) {
    const char *errptr = NULL;
    int erroffset;

    m_pc = pcre_compile(pattern.c_str(), PCRE_DOTALL|PCRE_MULTILINE,
        &errptr, &erroffset, NULL);

    m_pce = pcre_study(m_pc, pcre_study_opt, &errptr);
}


Regex::~Regex() {
    if (m_pc != NULL) {
        pcre_free(m_pc);
        m_pc = NULL;
    }
    if (m_pce != NULL) {
#if PCRE_HAVE_JIT
        pcre_free_study(m_pce);
#else
        pcre_free(m_pce);
#endif
        m_pce = NULL;
    }
}


std::list<SMatch> Regex::searchAll(const std::string& s) const {
    const char *subject = s.c_str();
    const std::string tmpString = std::string(s.c_str(), s.size());
    int ovector[OVECCOUNT];
    int rc, i, offset = 0;
    std::list<SMatch> retList;

    do {
        rc = pcre_exec(m_pc, m_pce, subject,
            s.size(), offset, 0, ovector, OVECCOUNT);

        for (i = 0; i < rc; i++) {
            size_t start = ovector[2*i];
            size_t end = ovector[2*i+1];
            size_t len = end - start;
            if (end > s.size()) {
                rc = 0;
                break;
            }
            std::string match = std::string(tmpString, start, len);
            offset = start + len;
            retList.push_front(SMatch(match, start));

            if (len == 0) {
                rc = 0;
                break;
            }
        }
    } while (rc > 0);

    return retList;
}

bool Regex::searchOneMatch(const std::string& s, std::vector<SMatchCapture>& captures) const {
    const char *subject = s.c_str();
    int ovector[OVECCOUNT];

    int rc = pcre_exec(m_pc, m_pce, subject, s.size(), 0, 0, ovector, OVECCOUNT);

    for (int i = 0; i < rc; i++) {
        size_t start = ovector[2*i];
        size_t end = ovector[2*i+1];
        size_t len = end - start;
        if (end > s.size()) {
            continue;
        }
        SMatchCapture capture(i, start, len);
        captures.push_back(capture);
    }

    return (rc > 0);
}

bool Regex::searchGlobal(const std::string& s, std::vector<SMatchCapture>& captures) const {
    const char *subject = s.c_str();

    bool prev_match_zero_length = false;
    int startOffset = 0;

    while (startOffset <= s.length()) {
        int ovector[OVECCOUNT];
        int pcre_options = 0;
        if (prev_match_zero_length) {
            pcre_options = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
        }
        int rc = pcre_exec(m_pc, m_pce, subject, s.length(), startOffset, pcre_options, ovector, OVECCOUNT);

        if (rc > 0) {
            size_t firstGroupForThisFullMatch = captures.size();
            for (int i = 0; i < rc; i++) {
                size_t start = ovector[2*i];
                size_t end = ovector[2*i+1];
                size_t len = end - start;
                if (end > s.length()) {
                    continue;
                }
                SMatchCapture capture(firstGroupForThisFullMatch + i, start, len);
                captures.push_back(capture);

                if (i == 0) {
                    if (len > 0) {
                        // normal case; next call to pcre_exec should start after the end of the last full match string
                        startOffset = end;
                        prev_match_zero_length = false;
                    } else {
                        // zero-length match; modify next match attempt to avoid infinite loop
                        prev_match_zero_length = true;
                    }
                }
            }
        } else {
            if (prev_match_zero_length) {
                // The n-1 search found a zero-length match, so we did a subsequent search
                // with the special flags. That subsequent exec did not find a match, so now advance
                // by one character (unless CRLF, then advance by two)
                startOffset++;
                if (crlfIsNewline() && (startOffset < s.length()) && (s[startOffset-1] == '\r')
                    && (s[startOffset] == '\n')) {
                    startOffset++;
                }
                prev_match_zero_length = false;
            } else {
                // normal case; no match on most recent scan (with options=0).  We are done.
                break;
            }
        }
    }

    return (captures.size() > 0);
}

int Regex::search(const std::string& s, SMatch *match) const {
    int ovector[OVECCOUNT];
    int ret = pcre_exec(m_pc, m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;

    if (ret > 0) {
        *match = SMatch(
            std::string(s, ovector[ret-1], ovector[ret] - ovector[ret-1]),
            0);
    }

    return ret;
}

int Regex::search(const std::string& s) const {
    int ovector[OVECCOUNT];
    return pcre_exec(m_pc, m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;
}

}  // namespace Utils
}  // namespace modsecurity
