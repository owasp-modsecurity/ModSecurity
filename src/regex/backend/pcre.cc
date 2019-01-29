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


#include "src/regex/backend/pcre.h"
#include "src/regex/regex_match.h"


namespace modsecurity {
namespace regex {
namespace backend {

#ifdef WITH_PCRE

#if PCRE_HAVE_JIT
#define pcre_study_opt PCRE_STUDY_JIT_COMPILE
#else
#define pcre_study_opt 0
#endif


Pcre::Pcre(const std::string& pattern_)
    : pattern(pattern_.empty() ? ".*" : pattern_) {
    const char *errptr = NULL;
    int erroffset;

    m_pc = pcre_compile(pattern.c_str(), PCRE_DOTALL|PCRE_MULTILINE,
        &errptr, &erroffset, NULL);

    m_pce = pcre_study(m_pc, pcre_study_opt, &errptr);

    pcre_fullinfo(m_pc, m_pce, PCRE_INFO_CAPTURECOUNT, &m_capture_count);
}


Pcre::~Pcre() {
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

static bool do_match(
        pcre *pc,
        pcre_extra *pce,
        int pcre_capture_count,
        const char *s,
        size_t n,
        RegexMatch *m,
        ssize_t max_groups,
        size_t offset)
{
    if (m == nullptr) {
        max_groups = 0;
    }

    // "+1" is required for full match (aka group 0)
    int ovecsize = (pcre_capture_count+1) * 3;
    int ovector[ovecsize];
    int ret = pcre_exec(pc, pce, s, n, offset, 0, ovector, ovecsize);

    if (ret > 0) {
        if (max_groups < 0) {
            max_groups = ret;
        }

        if (max_groups > 0) {
            size_t ngroups = std::min<size_t>(max_groups, ret);
            RegexMatch::MatchGroupContainer groups;
            groups.reserve(ngroups);
            for (size_t i = 0; i < ngroups; i++) {
                size_t start = ovector[2*i];
                size_t end = ovector[2*i+1];
                std::string group(s + start, end - start);

                groups.push_back(MatchGroup{start, std::move(group)});
            }
            *m = RegexMatch(std::move(groups));
        }
        return true;
    }
    return false;

}

std::vector<RegexMatch> Pcre::searchAll(const std::string& s, bool overlapping) const {
    std::vector<RegexMatch> res;
    size_t offset = 0;

    while (1) {
        RegexMatch m;
        bool match = do_match(m_pc, m_pce, m_capture_count, s.data(), s.size(), &m, -1, offset);
        if (!match) break;

        if (overlapping) {
            // start just after the beginning of the last match
            offset = m.group(0).offset + 1;
        } else {
            // start just at the end of the last match
            offset = m.group(0).offset + m.group(0).string.size();
            if (offset == m.group(0).offset) {
                // empty match - advance by one to not match empty string repeatedly
                offset++;
            }
        }
        res.push_back(std::move(m));
    }
    return res;
}

bool Pcre::search(const std::string &s, RegexMatch *m, ssize_t max_groups) const {
    return do_match(m_pc, m_pce, m_capture_count, s.data(), s.size(), m, max_groups, 0);
}

#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

