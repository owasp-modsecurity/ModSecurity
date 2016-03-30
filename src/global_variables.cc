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


#include "modsecurity/transaction/global_variables.h"

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


GlobalVariables::GlobalVariables() {
    this->reserve(1000);
}

GlobalVariables::~GlobalVariables() {
    this->clear();
}

void GlobalVariables::store(std::string key, std::string compartment, std::string value) {
    this->emplace(new CollectionKey(key, compartment), value);
}


bool GlobalVariables::storeOrUpdateFirst(const std::string &key,
    std::string compartment, const std::string &value) {
    if (updateFirst(key, compartment, value) == false) {
        store(key, compartment, value);
    }
    return true;
}


bool GlobalVariables::updateFirst(const std::string &key, std::string compartment, const std::string &value) {
    auto range = this->equal_range(new CollectionKey(key, compartment));

    for (auto it = range.first; it != range.second; ++it) {
        it->second = value;
        return true;
    }
    return false;
}


void GlobalVariables::del(const std::string& key, std::string compartment) {
    this->erase(new CollectionKey(key, compartment));
}



void GlobalVariables::resolveSingleMatch(const std::string& var,
    std::string compartment, std::vector<const transaction::Variable *> *l) {
    auto range = this->equal_range(new CollectionKey(var, compartment));

    for (auto it = range.first; it != range.second; ++it) {
        l->push_back(new transaction::Variable(var, it->second));
    }
}

void GlobalVariables::resolveMultiMatches(const std::string& var,
    std::string compartment, std::vector<const transaction::Variable *> *l) {
    size_t keySize = var.size();
    l->reserve(15);

    auto range = this->equal_range(new CollectionKey(var, compartment));

    for (auto it = range.first; it != range.second; ++it) {
        l->insert(l->begin(), new transaction::Variable(var, it->second));
    }

    for (const auto& x : *this) {
        if (x.first->m_name.size() <= keySize + 1) {
            continue;
        }
        if (x.first->m_name.at(keySize) != ':') {
            continue;
        }
        if (x.first->m_name.compare(0, keySize, var) != 0) {
            continue;
        }
        l->insert(l->begin(), new transaction::Variable(x.first->m_name, x.second));
    }
}


void GlobalVariables::resolveRegularExpression(const std::string& var,
     std::string compartment, std::vector<const transaction::Variable *> *l) {
    /* Not ready */
}


std::string* GlobalVariables::resolveFirst(const std::string& var,
    std::string compartment) {
    auto range = equal_range(new CollectionKey(var, compartment));

    for (auto it = range.first; it != range.second; ++it) {
        return &it->second;
    }

    return NULL;
}


}  // namespace transaction
}  // namespace modsecurity
