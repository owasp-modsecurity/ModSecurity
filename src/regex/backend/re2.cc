/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2019
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

#include "src/regex/backend/re2.h"
#include "src/regex/regex_match.h"

namespace modsecurity {
namespace regex {
namespace backend {

#ifdef WITH_RE2

static RE2::Options get_re2_options() {
    RE2::Options res;

    // Re2 is usually used with fallback to libpcre,
    // so disable unnecessary stderr noise
    res.set_log_errors(false);

    res.set_dot_nl(true);

    return res;
}


Re2::Re2(const std::string& pattern)
    : re(pattern.empty() ? ".*" : pattern, get_re2_options())
{
}

static bool do_match(
        const RE2 &re,
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
    size_t ngroups = re.NumberOfCapturingGroups() + 1;
    if (max_groups >= 0 && max_groups < ngroups) {
        ngroups = max_groups;
    }
    re2::StringPiece submatches[ngroups];

    if (re.Match(re2::StringPiece(s, n), offset, n, RE2::UNANCHORED,
        &submatches[0], ngroups)) {
        if (ngroups != 0) {
            RegexMatch::MatchGroupContainer groups;
            groups.reserve(ngroups);
            for (size_t i = 0; i < ngroups; i++) {
                size_t start = submatches[i].data() - s;
                std::string group = submatches[i].as_string();
                groups.push_back(MatchGroup{start, std::move(group)});
            }
            *m = RegexMatch(std::move(groups));
        }
        return true;
    }
    return false;
}

std::vector<RegexMatch> Re2::searchAll(const std::string& s, bool overlapping) const {
    std::vector<RegexMatch> res;
    size_t offset = 0;

    while (1) {
        RegexMatch m;
        bool match = do_match(re, s.data(), s.size(), &m, -1, offset);
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

bool Re2::search(const std::string &s, RegexMatch *m, ssize_t max_groups) const {
    return do_match(re, s.data(), s.size(), m, max_groups, 0);
}

#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

