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
#include <vector>
#include <string>

#include "src/regex/regex_match.h"

namespace modsecurity {
namespace regex {
namespace backend {

class Backend {
public:
    virtual ~Backend() {}

    virtual bool ok(std::string *error = nullptr) const = 0;

    virtual std::list<RegexMatch> searchAll(const std::string& s) const = 0;
    virtual bool searchOneMatch(const std::string& s, std::vector<RegexMatchCapture>& captures) const = 0;
    virtual int search(const std::string &s, RegexMatch *m) const = 0;
    virtual int search(const std::string &s) const = 0;

    virtual const std::string& getPattern() const = 0;
};

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity

#endif  // SRC_REGEX_BACKEND_BACKEND_H_
