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

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../headers/modsecurity/anchored_variable.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/regex.h"
#include "modsecurity/string_view.hpp"

namespace modsecurity {


AnchoredVariable::AnchoredVariable(Transaction *t,
    const std::string &name)
    : m_transaction(t),
    m_offset(0),
    m_name(name),
    m_value(""),
    m_var(std::make_shared<VariableValue>(&name)) {
}

void AnchoredVariable::unset() {
    m_value.clear();
}


void AnchoredVariable::set(const std::string &a, size_t offset,
    size_t offsetLen) {

    m_offset = offset;
    m_value.assign(a.c_str(), a.size());
    VariableOrigin origin;
    origin.m_offset = offset;
    origin.m_length = offsetLen;
    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::set(const std::string &a, size_t offset) {
    m_offset = offset;
    m_value.assign(a.c_str(), a.size());
    VariableOrigin origin;
    origin.m_offset = offset;
    origin.m_length = m_value.size();
    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::set(const char *a, size_t offset) {
    VariableOrigin origin;

    m_offset = offset;
    m_value.assign(a, strlen(a));
    origin.m_offset = offset;
    origin.m_length = m_value.size();
    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::set(const bpstd::string_view &a, size_t offset) {
    VariableOrigin origin;

    m_offset = offset;
    m_value.assign(a.c_str(), a.size());
    origin.m_offset = offset;
    origin.m_length = m_value.size();

    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::append(const std::string &a, size_t offset,
    bool spaceSeparator) {
    if (spaceSeparator && !m_value.empty()) {
        m_value.append(" " + a);
    } else {
        m_value.append(a);
    }
    m_offset = offset;
    VariableOrigin origin;
    origin.m_offset = offset;
    origin.m_length = a.size();
    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::append(const std::string &a, size_t offset,
    bool spaceSeparator, int size) {
    if (spaceSeparator && !m_value.empty()) {
        m_value.append(" " + a);
    } else {
        m_value.append(a);
    }
    m_offset = offset;
    VariableOrigin origin;
    origin.m_offset = offset;
    origin.m_length = size;
    m_var->addOrigin(std::move(origin));
}


void AnchoredVariable::evaluate(VariableValues *l) const noexcept {
    if (m_name.empty()) {
        return;
    }

    m_var->setValue(m_value);
    l->push_back(m_var);
}


const std::string *AnchoredVariable::evaluate() const noexcept {
    return &m_value;
}


std::unique_ptr<std::string> AnchoredVariable::resolveFirst() const noexcept {
    if (m_value.empty()) {
        return nullptr;
    }
    std::unique_ptr<std::string> a(new std::string());
    a->append(m_value);
    return a;
}


}  // namespace modsecurity
