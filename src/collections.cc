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


#include "modsecurity/transaction/collections.h"

#ifdef __cplusplus
#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#endif

#include "modsecurity/transaction/variable.h"
#include "src/utils.h"

namespace modsecurity {
namespace transaction {


Collections::Collections() {
    /* Create collection TX */
    this->emplace("TX", new Collection("TX", ""));
}


Collections::~Collections() {
    for (const auto &thing : *this) {
        delete thing.second;
    }
    this->clear();
}


void Collections::init(const std::string& name, const std::string& key) {
    this->emplace(name, new Collection(name, key));
}


void Collections::storeOrUpdateFirst(const std::string& collectionName,
    const std::string& variableName,
    const std::string& targetValue) {
    try {
        transaction::Variables *collection;
        collection = this->at(collectionName);
        collection->storeOrUpdateFirst(collectionName + ":"
            + variableName, targetValue);
    } catch (...) {
#if 0
        debug(9, "don't know any collection named: "
            + collectionName + ". it was created?");
#endif
    }
}


void Collections::store(std::string key, std::string value) {
    m_transient.store(key, value);
}


bool Collections::storeOrUpdateFirst(const std::string &key,
    const std::string &value) {
    return m_transient.storeOrUpdateFirst(key, value);
}


bool Collections::updateFirst(const std::string &key,
    const std::string &value) {
    return m_transient.updateFirst(key, value);
}


void Collections::del(const std::string& key) {
    return m_transient.del(key);
}


std::string* Collections::resolveFirst(const std::string& var) {
    std::string *transientVar = m_transient.resolveFirst(var);

    if (transientVar != NULL) {
        return transientVar;
    }

    for (auto &a : *this) {
        auto range = a.second->equal_range(var);
        for (auto it = range.first; it != range.second; ++it) {
            return &it->second;
        }
    }

    return NULL;
}


std::string* Collections::resolveFirst(const std::string& collectionName,
        const std::string& var) {
        for (auto &a : *this) {
            if (tolower(a.first) == tolower(collectionName)) {
                transaction::Variables *t = a.second;
                auto range = t->equal_range(toupper(collectionName)
                    + ":" + var);
                for (auto it = range.first; it != range.second; ++it) {
                    return &it->second;
                }
            }
        }

        return NULL;
}


void Collections::resolveSingleMatch(const std::string& var,
    std::vector<const transaction::Variable *> *l) {

    m_transient.resolveSingleMatch(var, l);
}


void Collections::resolveSingleMatch(const std::string& var,
    const std::string& collection,
    std::vector<const transaction::Variable *> *l) {

    try {
        this->at(collection)->resolveSingleMatch(var, l);
    } catch (...) { }
}

void Collections::resolveMultiMatches(const std::string& var,
    std::vector<const transaction::Variable *> *l) {

    m_transient.resolveMultiMatches(var, l);
}


void Collections::resolveMultiMatches(const std::string& var,
    const std::string& collection,
    std::vector<const transaction::Variable *> *l) {
    try {
        this->at(collection)->resolveMultiMatches(var, l);
    } catch (...) { }
}

void Collections::resolveRegularExpression(const std::string& var,
    std::vector<const transaction::Variable *> *l) {
    m_transient.resolveRegularExpression(var, l);
}


void Collections::resolveRegularExpression(const std::string& var,
    const std::string& collection,
    std::vector<const transaction::Variable *> *l) {

    try {
        this->at(collection)->resolveRegularExpression(var, l);
    } catch (...) { }
}

}  // namespace transaction
}  // namespace modsecurity
