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

#ifdef WITH_RE2
#include <re2/re2.h>
#endif

#include <string>
#include <list>

#include "src/regex/backend/backend.h"
#include "src/regex/regex_match.h"

#ifndef SRC_REGEX_BACKEND_RE2_H_
#define SRC_REGEX_BACKEND_RE2_H_

namespace modsecurity {
namespace regex {
namespace backend {

#ifdef WITH_RE2

class Re2 : public Backend {
 public:
    explicit Re2(const std::string& pattern_);

    // RE2 class is not copyable, so neither is this
    Re2(const Re2&) = delete;
    Re2& operator=(const Re2&) = delete;

    virtual bool ok(std::string *error = nullptr) const override {
        if (re.ok()) {
            return true;
        }
        if (error != nullptr) {
            *error = re.error();
        }
        return false;
    }

    std::vector<RegexMatch> searchAll(const std::string& s, bool overlapping = false) const override;
    bool search(const std::string &s, RegexMatch *m = nullptr, ssize_t max_groups = -1) const override;

    virtual const std::string& getPattern() const override {
        return re.pattern();
    };
 private:
    const RE2 re;
};

#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

#endif  // SRC_REGEX_BACKEND_PCRE_H_
