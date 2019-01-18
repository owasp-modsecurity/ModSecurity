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

#include "src/regex/regex.h"

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
namespace regex {


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


std::list<RegexMatch> Regex::searchAll(const std::string& s) const {
    const char *subject = s.c_str();
    const std::string tmpString = std::string(s.c_str(), s.size());
    int ovector[OVECCOUNT];
    int rc, i, offset = 0;
    std::list<RegexMatch> retList;

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
            retList.push_front(RegexMatch(match, start));

            if (len == 0) {
                rc = 0;
                break;
            }
        }
    } while (rc > 0);

    return retList;
}

bool Regex::searchOneMatch(const std::string& s, std::vector<RegexMatchCapture>& captures) const {
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
        RegexMatchCapture capture(i, start, len);
        captures.push_back(capture);
    }

    return (rc > 0);
}

int Regex::search(const std::string& s, RegexMatch *match) const {
    int ovector[OVECCOUNT];
    int ret = pcre_exec(m_pc, m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;

    if (ret > 0) {
        *match = RegexMatch(
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

}  // namespace regex
}  // namespace modsecurity
