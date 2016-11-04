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

#include "src/utils/regex.h"

#include <pcre.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

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
    : pattern(pattern_) {
    const char *errptr = NULL;
    int erroffset;

    if (pattern.empty() == true) {
        pattern.assign(".*");
    }

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


int regex_search(const std::string& s, SMatch *match,
    const Regex& regex) {
    int ovector[OVECCOUNT];
    int ret = pcre_exec(regex.m_pc, regex.m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;

    if (ret > 0) {
        match->match = std::string(s, ovector[ret-1],
            ovector[ret] - ovector[ret-1]);
        match->size_ = ret;
    }

    return ret;
}

int regex_search(const std::string& s, const Regex& regex) {
    int ovector[OVECCOUNT];
    return pcre_exec(regex.m_pc, regex.m_pce, s.c_str(),
        s.size(), 0, 0, ovector, OVECCOUNT) > 0;
}

}  // namespace Utils
}  // namespace modsecurity
