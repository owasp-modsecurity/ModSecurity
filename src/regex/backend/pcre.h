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

#ifdef WITH_PCRE
#include <pcre.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "src/regex/backend/backend.h"
#include "src/regex/regex_match.h"

#ifndef SRC_REGEX_BACKEND_PCRE_H_
#define SRC_REGEX_BACKEND_PCRE_H_

namespace modsecurity {
namespace regex {
namespace backend {

#ifdef WITH_PCRE

#define OVECCOUNT 30


class Pcre : public Backend {
 public:
    explicit Pcre(const std::string& pattern_);
    ~Pcre();

    // m_pc and m_pce can't be easily copied
    Pcre(const Pcre&) = delete;
    Pcre& operator=(const Pcre&) = delete;

    virtual bool ok() const override {
        return m_pc != NULL;
    }

    std::list<RegexMatch> searchAll(const std::string& s) const override;
    int search(const std::string &s, RegexMatch *m) const override;
    int search(const std::string &s) const override;

    virtual const std::string& getPattern() const override {
        return pattern;
    };
 private:
    const std::string pattern;

    pcre *m_pc = NULL;
    pcre_extra *m_pce = NULL;
};

#endif

}  // namespace backend
}  // namespace regex
}  // namespace modsecurity


#endif  // SRC_REGEX_BACKEND_PCRE_H_
