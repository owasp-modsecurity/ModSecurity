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


#include "modsecurity/transaction/variables.h"

#ifdef __cplusplus
#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#endif

#include "modsecurity/transaction/variable.h"
#include "src/utils.h"

namespace modsecurity {
namespace transaction {


Variables::Variables() {
    this->reserve(1000);
}

Variables::~Variables() {
    this->clear();
}

void Variables::store(std::string key, std::string value) {
    this->emplace(key, value);
}


bool Variables::storeOrUpdateFirst(const std::string &key,
    const std::string &value) {
    if (updateFirst(key, value) == false) {
        store(key, value);
    }
    return true;
}


bool Variables::updateFirst(const std::string &key, const std::string &value) {
    auto range = this->equal_range(key);

    for (auto it = range.first; it != range.second; ++it) {
        it->second = value;
        return true;
    }
    return false;
}


void Variables::del(const std::string& key) {
    this->erase(key);
}


void Variables::resolveSingleMatch(const std::string& var,
    std::vector<const transaction::Variable *> *l) {
    auto range = this->equal_range(var);

    for (auto it = range.first; it != range.second; ++it) {
        l->push_back(new transaction::Variable(var, it->second));
    }
}


void Variables::resolveMultiMatches(const std::string& var,
    std::vector<const transaction::Variable *> *l) {
    size_t keySize = var.size();
    l->reserve(15);

    auto range = this->equal_range(var);

    for (auto it = range.first; it != range.second; ++it) {
        l->insert(l->begin(), new transaction::Variable(var, it->second));
    }

    for (const auto& x : *this) {
        if (x.first.size() <= keySize + 1) {
            continue;
        }
        if (x.first.at(keySize) != ':') {
            continue;
        }
        if (x.first.compare(0, keySize, var) != 0) {
            continue;
        }
        l->insert(l->begin(), new transaction::Variable(x.first, x.second));
    }
}


void Variables::resolveRegularExpression(const std::string& var,
    std::vector<const transaction::Variable *> *l) {
    /* Not ready */
}


std::string* Variables::resolveFirst(const std::string& var) {
    auto range = equal_range(var);

    for (auto it = range.first; it != range.second; ++it) {
        return &it->second;
    }

    return NULL;
}


}  // namespace transaction
}  // namespace modsecurity
