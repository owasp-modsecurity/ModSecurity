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

bool Regex::searchOneMatch(const std::string& s, match_type& captures) const {
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

std::vector<Regex::match_type> Regex::searchAllMatches(const std::string& s) const {
    int ovector[OVECCOUNT];
    int offset = 0;
    std::vector<Regex::match_type> matches;

    while (int rc = pcre_exec(m_pc, m_pce, s.data(), s.size(), offset, 0, ovector, OVECCOUNT) > 0) {
        Regex::match_type match;

        for (int i = 0; i < rc; i++) {
            int start = ovector[2*i];
            int end = ovector[2*i+1];

            // see man pcreapi for details when offsets are set to -1
            if (start >= 0 && end >= 0) {
                int len = end - start;
                match.emplace_back(i, start, len);
            }
        }
        matches.push_back(std::move(match));

        // offsets for full match (group 0)
        int start = ovector[0];
        int end = ovector[1];
        offset = end;
        if (start == end) {
            // skip zero-length match (otherwise, the loop won't terminate)
            offset++;
        }
    }
    return matches;
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
