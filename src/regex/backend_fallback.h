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
#ifndef SRC_REGEX_BACKEND_FALLBACK_H_
#define SRC_REGEX_BACKEND_FALLBACK_H_

#include <memory>

#include "src/regex/backend/backend.h"

namespace modsecurity {
namespace regex {

template<typename T>
static backend::Backend* compile_regex_fallback(const std::string& pattern) {
    return new T(pattern);
}

template<typename T, typename T2, typename... Args>
static backend::Backend* compile_regex_fallback(const std::string& pattern) {
    T *regex = new T{pattern};
    if (regex->ok()) {
        return regex;
    } else {
        delete regex;
        return compile_regex_fallback<T2, Args...>(pattern);
    }
}

template<typename... Args>
class BackendFallback : public backend::Backend {
public:
    BackendFallback(const std::string& pattern)
        : backend(compile_regex_fallback<Args...>(pattern))
    {}

    virtual bool ok() const override {
        return backend->ok();
    }

    std::vector<RegexMatch> searchAll(const std::string& s, bool overlapping = false) const override {
        return backend->searchAll(s, overlapping);
    }

    bool search(const std::string &s, RegexMatch *m = nullptr, ssize_t max_groups = -1) const override {
        return backend->search(s, m, max_groups);
    }

    const std::string& getPattern() const override {
        return backend->getPattern();
    }
private:
    std::unique_ptr<backend::Backend> backend;
};

}  // namespace regex
}  // namespace modsecurity

#endif // SRC_REGEX_BACKEND_FALLBACK_H_
