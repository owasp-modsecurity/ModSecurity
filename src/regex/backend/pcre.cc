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
#include <list>


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
    if (m_pc == NULL) {
        m_error = "pcre_compile error at offset " + std::to_string(erroffset) + ": " + std::string(errptr);
        return;
    }

    m_pce = pcre_study(m_pc, pcre_study_opt, &errptr);
    if (m_pce == NULL) {
        m_error = "pcre_study error: " + std::string(errptr);
        pcre_free(m_pc);
        m_pc = nullptr;
        return;
    }
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


std::list<RegexMatch> Pcre::searchAll(const std::string& s) const {
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
        }

        offset = ovector[1]; // end
        if (offset == ovector[0]) { // start == end (size == 0)
            offset++;
        }
    } while (rc > 0);

    return retList;
}


bool Pcre::searchOneMatch(const std::string& s, std::vector<RegexMatchCapture>& captures) const {
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


int Pcre::search(const std::string& s, RegexMatch *match) const {
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


int Pcre::search(const std::string& s) const {
    int ovector[OVECCOUNT];
    return pcre_exec(m_pc, m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;
}

#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

