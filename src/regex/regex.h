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

#include "src/regex/backend/backend.h"
#include "src/regex/backend/pcre.h"
#include "src/regex/backend/re2.h"
#include "src/regex/regex_match.h"
#include "src/regex/backend_fallback.h"

#ifndef SRC_REGEX_REGEX_H_
#define SRC_REGEX_REGEX_H_


namespace modsecurity {
namespace regex {

#ifdef WITH_PCRE
#   ifdef WITH_RE2
        using selectedBackend = BackendFallback<
            backend::Re2, backend::Pcre
        >;
#   else
        using selectedBackend = backend::Pcre;
#   endif
#else
#   error "PCRE is not available"
#endif

class Regex : public selectedBackend {
  public:
    explicit Regex(const std::string& pattern) :
        selectedBackend(pattern) { };
    ~Regex() { };
};

}  // namespace regex
}  // namespace modsecurity

#endif  // SRC_REGEX_REGEX_H_
