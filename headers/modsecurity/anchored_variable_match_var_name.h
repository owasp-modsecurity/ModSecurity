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
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>
#include <cstring>

#include "modsecurity/string_view.hpp"
#endif

#include "modsecurity/variable_value.h"


#ifndef HEADERS_MODSECURITY_ANCHORED_VARIABLE_NAME_H_
#define HEADERS_MODSECURITY_ANCHORED_VARIABLE_NAME_H_

#ifdef __cplusplus


namespace modsecurity {
class Transaction;

class AnchoredVariableMatchVarName {
 public:
    AnchoredVariableMatchVarName()
        : m_name("MATCHED_VAR_NAME")
    { }

    AnchoredVariableMatchVarName(const AnchoredVariableMatchVarName &a) = delete;
    AnchoredVariableMatchVarName &operator= (const AnchoredVariableMatchVarName &a) = delete;

    void set(std::shared_ptr<const VariableValue> v) noexcept {
        m_vv = v;
    }

    void unset() noexcept {
        m_vv = nullptr;
    }

    void evaluate(VariableValues *l) const noexcept {
        if (!m_vv) {
            return;
        }
        const VariableValue *var = new VariableValue(
            std::unique_ptr<std::string>(new std::string(m_vv->getName())),
            &m_name
            );
        l->push_back(std::unique_ptr<const VariableValue>(var));
    }

    std::unique_ptr<std::string> resolveFirst() const noexcept {
        if (m_vv) {
            return std::unique_ptr<std::string>(new std::string(m_vv->getName()));
        }
        return std::unique_ptr<std::string>(new std::string(""));
    }

 private:
    std::shared_ptr<const VariableValue> m_vv;
    const std::string m_name;
};


}  // namespace modsecurity

#endif


#endif  // HEADERS_MODSECURITY_ANCHORED_VARIABLE_NAME_H_
