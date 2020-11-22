/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifdef __cplusplus
#include <vector>
#include <utility>

#include "modsecurity/string_view.hpp"
#endif

#include "modsecurity/variable_value.h"


#ifndef HEADERS_MODSECURITY_ANCHORED_SET_VARIABLE_MATCH_VARS_NAMES_H_
#define HEADERS_MODSECURITY_ANCHORED_SET_VARIABLE_MATCH_VARS_NAMES_H_

#ifdef __cplusplus

namespace modsecurity {
class Transaction;
namespace Utils {
class Regex;
}
namespace variables {
class KeyExclusions;
}


class AnchoredVariableMatchVarsNames {
 public:
    explicit AnchoredVariableMatchVarsNames(Transaction *t)
        : m_name("MATCHED_VARS_NAMES"),
        m_transaction(t)
    { }

    AnchoredVariableMatchVarsNames(const AnchoredVariableMatchVarsNames &a) = delete;
    AnchoredVariableMatchVarsNames &operator= (const AnchoredVariableMatchVarsNames &a) = delete;

    void set(std::shared_ptr<const VariableValue> v) noexcept {
        m_vvs.push_back(v);
    }

    void unset() noexcept {
        m_vvs.clear();
    }

    void resolve(VariableValues *l,
        const variables::KeyExclusions &ke) const noexcept;

    void resolve(const std::string &key,
        VariableValues *l) const noexcept;


    void resolveRegularExpression(const Utils::Regex *r,
        VariableValues *l,
        const variables::KeyExclusions &ke) const noexcept;

    std::unique_ptr<std::string> resolveFirst(const std::string &key) const noexcept;

 private:
    std::vector<std::shared_ptr<const VariableValue>> m_vvs;
    const std::string m_name;
    const Transaction *m_transaction;
};


}  // namespace modsecurity

#endif


#endif  // HEADERS_MODSECURITY_ANCHORED_SET_VARIABLE_MATCH_VARS_NAMES_H_

