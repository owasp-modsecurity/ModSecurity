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

namespace modsecurity {

//FIXME remove the Transaction from the constructor
AnchoredVariable::AnchoredVariable(Transaction *t,
    const std::string& name)
    : m_var{&name}
{
}

void AnchoredVariable::unset() {
    m_var = std::move(VariableValue{&m_var.getKey()});
}

//FIXME will remove this later when changing the
//  interface but *obj.evaluate() is a pain
const std::string& AnchoredVariable::getValue() const{
    return m_var.getValue();
}
std::string& AnchoredVariable::getValue(){
    return m_var.getValue();
}

void AnchoredVariable::set(const std::string &a, size_t offset,
    size_t offsetLen) {
    m_var.getValue().assign(a.c_str(), a.size());

    m_var.addOrigin(VariableOrigin{offsetLen, offset});
}


void AnchoredVariable::set(const std::string &a, size_t offset) {
  set(a, offset, a.size());
}


void AnchoredVariable::append(const std::string &a, size_t offset,
    bool spaceSeparator) {
    append(a, offset, spaceSeparator, a.size());
}


void AnchoredVariable::append(const std::string &a, size_t offset,
    bool spaceSeparator, int size) {
    if (spaceSeparator && !m_var.getValue().empty()) {
        m_var.getValue().append(" " + a);
    } else {
        m_var.getValue().append(a);
    }
    m_var.addOrigin(VariableOrigin{size, offset});
}

void AnchoredVariable::evaluate(VariableValueList *l) {
    l->emplace_back(&m_var);
}


std::string * AnchoredVariable::evaluate() {
    return &m_var.getValue();
}


std::unique_ptr<std::string> AnchoredVariable::resolveFirst() {
    const std::string& value = m_var.getValue();
    std::unique_ptr<std::string> a(new std::string{value});
    return a;
}


}  // namespace modsecurity
