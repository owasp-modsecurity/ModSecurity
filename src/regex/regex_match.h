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


#ifndef SRC_REGEX_REGEX_MATCH_H_
#define SRC_REGEX_REGEX_MATCH_H_

#include <vector>
#include <string>

namespace modsecurity {
namespace regex {

struct MatchGroup {
    size_t offset;
    std::string string;
};

class RegexMatch {
public:
    using MatchGroupContainer = std::vector<MatchGroup>;

    RegexMatch() {}

    RegexMatch(MatchGroupContainer groups)
        : m_groups(std::move(groups))
    {}

    size_t num_groups() const {
        return m_groups.size();
    }

    const MatchGroup& group(size_t i) const {
        return m_groups[i];
    }

private:
    MatchGroupContainer m_groups;
};

}  // namespace regex
}  // namespace modsecurity

#endif  // SRC_REGEX_REGEX_MATCH_H_
