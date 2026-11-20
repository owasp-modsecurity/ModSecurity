/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 * Copyright (c) 2026 OWASP Foundation (http://www.owasp.org/)
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
#include <memory>
#include <string>
#include <vector>

#include "modsecurity/anchored_set_variable.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/regex.h"
#include "src/variables/variable.h"

namespace modsecurity {


AnchoredSetVariable::AnchoredSetVariable(Transaction *t,
    const std::string &name)
    : m_transaction(t),
    m_name(name) {
        reserve(10);
    }


AnchoredSetVariable::~AnchoredSetVariable() {
    unset();
}


void AnchoredSetVariable::unset() {
    for (const auto& x : *this) {
        VariableValue *var = x.second;
        delete var;
    }
    clear();
}


void AnchoredSetVariable::set(const std::string &key,
    const std::string &value, size_t offset, size_t len) {
    VariableValue *var = new VariableValue(&m_name, &key, &value);
    var->addOrigin(len, offset);
    emplace(key, var);
}


void AnchoredSetVariable::set(const std::string &key,
    const std::string &value, size_t offset) {
    VariableValue *var = new VariableValue(&m_name, &key, &value);
    var->addOrigin(value.size(), offset);
    emplace(key, var);
}



namespace {
// Puts `found` in front of `l`, the last first (as inserting each at the
// front did), without a step that can throw once the room is there: the
// copies stay owned by `found` until then (no leak on bad_alloc).
void moveToFront(std::vector<const VariableValue *> *l,
    std::vector<std::unique_ptr<VariableValue>> *found) {
    l->reserve(l->size() + found->size());
    l->insert(l->begin(), found->size(), nullptr);
    for (size_t i = 0; i < found->size(); i++) {
        (*l)[i] = (*found)[found->size() - 1 - i].release();
    }
}
}  // namespace

void AnchoredSetVariable::resolve(
    std::vector<const VariableValue *> *l) const {
    // Each value went to the front of `l` (O(n^2) over n arguments);
    // collected first, then put in front at once in the same order.
    std::vector<std::unique_ptr<VariableValue>> found;
    found.reserve(size());
    for (const auto& x : *this) {
        found.push_back(std::make_unique<VariableValue>(x.second));
    }
    moveToFront(l, &found);
}


void AnchoredSetVariable::resolve(
    std::vector<const VariableValue *> *l) {
    static_cast<const AnchoredSetVariable&>(*this).resolve(l);
}


void AnchoredSetVariable::resolve(
    std::vector<const VariableValue *> *l,
    const variables::KeyExclusions &ke) const {
    std::vector<std::unique_ptr<VariableValue>> found;
    found.reserve(size());
    for (const auto& x : *this) {
        if (!ke.toOmit(x.first)) {
            found.push_back(std::make_unique<VariableValue>(x.second));
        } else {
            ms_dbg_a(m_transaction, 7, "Excluding key: " + x.first
                + " from target value.");
        }
    }
    moveToFront(l, &found);
}


void AnchoredSetVariable::resolve(
    std::vector<const VariableValue *> *l,
    variables::KeyExclusions &ke) {  // cppcheck-suppress constParameterReference
    static_cast<const AnchoredSetVariable&>(*this).resolve(l, ke);
}


void AnchoredSetVariable::resolve(const std::string &key,
    std::vector<const VariableValue *> *l) {
    auto range = this->equal_range(key);
    for (auto it = range.first; it != range.second; ++it) {
        l->push_back(new VariableValue(it->second));
    }
}


std::unique_ptr<std::string> AnchoredSetVariable::resolveFirst(
    const std::string &key) {

    if (auto search = this->find(key); search != this->end()) {
        return std::make_unique<std::string>(search->second->getValue());
    }

    return nullptr;
}

// cppcheck-suppress constParameterPointer  -  keep the old signatures for ABI compatibility
void AnchoredSetVariable::resolveRegularExpression(Utils::Regex *r,
    std::vector<const VariableValue *> *l) const {
    std::vector<std::unique_ptr<VariableValue>> found;
    for (const auto& x : *this) {
        int ret = Utils::regex_search(x.first, *r);
        if (ret <= 0) {
            continue;
        }
        found.push_back(std::make_unique<VariableValue>(x.second));
    }
    moveToFront(l, &found);
}


void AnchoredSetVariable::resolveRegularExpression(Utils::Regex *r,
    std::vector<const VariableValue *> *l) {
    static_cast<const AnchoredSetVariable&>(*this).resolveRegularExpression(r, l);
}

// cppcheck-suppress constParameterPointer  -  keep the old signatures for ABI compatibility
void AnchoredSetVariable::resolveRegularExpression(Utils::Regex *r,
    std::vector<const VariableValue *> *l,
    const variables::KeyExclusions &ke) const {
    std::vector<std::unique_ptr<VariableValue>> found;
    for (const auto& x : *this) {
        int ret = Utils::regex_search(x.first, *r);
        if (ret <= 0) {
            continue;
        }
        if (!ke.toOmit(x.first)) {
            found.push_back(std::make_unique<VariableValue>(x.second));
        } else {
            ms_dbg_a(m_transaction, 7, "Excluding key: " + x.first
                + " from target value.");
        }
    }
    moveToFront(l, &found);
}


void AnchoredSetVariable::resolveRegularExpression(Utils::Regex *r,
    std::vector<const VariableValue *> *l,
    variables::KeyExclusions &ke) {  // cppcheck-suppress constParameterReference
    static_cast<const AnchoredSetVariable&>(*this).resolveRegularExpression(r, l, ke);
}


}  // namespace modsecurity
