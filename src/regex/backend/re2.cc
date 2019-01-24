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

    res.set_dot_nl(true);

    return res;
}


Re2::Re2(const std::string& pattern_)
    : pattern(pattern_.empty() ? ".*" : pattern_),
      re(pattern, get_re2_options())
{
}

std::list<RegexMatch> Re2::searchAll(const std::string& s) const {
    std::list<RegexMatch> retList;

    re2::StringPiece subject(s);

    size_t offset = 0;
    while (offset <= s.size()) {
        int ngroups = re.NumberOfCapturingGroups() + 1;
        re2::StringPiece submatches[ngroups];

        if (!re.Match(subject, offset, s.size(), RE2::UNANCHORED,
            &submatches[0], ngroups)) {
            break;
        }

        for (int i = 0; i < ngroups; i++) {
            // N.B. StringPiece::as_string returns value, not reference
            auto match_string =  submatches[i].as_string();
            auto start = &submatches[i][0] - &subject[0];
            retList.push_front(RegexMatch(std::move(match_string), start));
        }

        offset = (&submatches[0][0] - &subject[0]) + submatches[0].length();
        if (submatches[0].size() == 0) {
            offset++;
        }
    }

    return retList;
}

int Re2::search(const std::string& s, RegexMatch *match) const {
    re2::StringPiece subject(s);
    re2::StringPiece submatches[1];
    if (re.Match(subject, 0, s.size(), RE2::UNANCHORED, &submatches[0], 1)) {
        // N.B. StringPiece::as_string returns value, not reference
        auto match_string = submatches[0].as_string();
        auto start = &submatches[0][0] - &subject[0];
        *match = RegexMatch(std::move(match_string), start);
        return 1;
    } else {
        return 0;
    }
}

int Re2::search(const std::string& s) const {
    re2::StringPiece subject(s);
    return re.Match(subject, 0, s.size(), RE2::UNANCHORED, NULL, 0);
}
#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

