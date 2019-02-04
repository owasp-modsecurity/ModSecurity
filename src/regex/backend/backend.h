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
#ifndef SRC_REGEX_BACKEND_BACKEND_H_
#define SRC_REGEX_BACKEND_BACKEND_H_

#include <list>
#include <string>

#include "src/regex/regex_match.h"

namespace modsecurity {
namespace regex {
namespace backend {

class Backend {
public:
    virtual ~Backend() {}

    virtual bool ok(std::string *error = nullptr) const = 0;

    virtual std::vector<RegexMatch> searchAll(const std::string& s, bool overlapping = false) const = 0;
    virtual bool search(const std::string &s, RegexMatch *m = nullptr, ssize_t max_groups = -1) const = 0;

    virtual const std::string& getPattern() const = 0;
};

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

#endif  // SRC_REGEX_BACKEND_BACKEND_H_
