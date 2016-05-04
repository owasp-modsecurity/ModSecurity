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


#include "modsecurity/collection/collections.h"

#ifdef __cplusplus
#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#endif

#include "modsecurity/collection/variable.h"
#include "modsecurity/collection/collection.h"
#include "src/collection/backend/in_memory-per_process.h"
#include "src/utils.h"

namespace modsecurity {
namespace collection {


Collections::Collections(Collection *global,
    Collection *ip)
    : m_global_collection_key(""),
    m_ip_collection_key(""),
    m_global_collection(global),
    m_ip_collection(ip),
    m_transient(new backend::InMemoryPerProcess()) {
    /* Create collection TX */
    this->emplace("TX", new backend::InMemoryPerProcess());
}


Collections::~Collections() {
    for (const auto &thing : *this) {
        delete thing.second;
    }
    this->clear();
}

void Collections::storeOrUpdateFirst(const std::string& collectionName,
    const std::string& variableName,
    const std::string& targetValue) {
    if (tolower(collectionName) == "ip"
        && !m_ip_collection_key.empty()) {
        m_ip_collection->storeOrUpdateFirst(collectionName + ":"
            + variableName, m_ip_collection_key, targetValue);
        return;
    }

    if (tolower(collectionName) == "global"
        && !m_global_collection_key.empty()) {
        m_global_collection->storeOrUpdateFirst(collectionName + ":"
            + variableName, m_global_collection_key, targetValue);
        return;
    }

    try {
        Collection *collection;
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
    m_transient->store(key, value);
}


bool Collections::storeOrUpdateFirst(const std::string &key,
    const std::string &value) {
    return m_transient->storeOrUpdateFirst(key, value);
}


bool Collections::updateFirst(const std::string &key,
    const std::string &value) {
    return m_transient->updateFirst(key, value);
}


void Collections::del(const std::string& key) {
    return m_transient->del(key);
}


std::string* Collections::resolveFirst(const std::string& var) {
    std::string *transientVar = m_transient->resolveFirst(var);

    if (transientVar != NULL) {
        return transientVar;
    }

    for (auto &a : *this) {
        std::string *res = a.second->resolveFirst(toupper(a.first) + ":" + var);
        if (res != NULL) {
            return res;
        }
    }

    return NULL;
}


std::string* Collections::resolveFirst(const std::string& collectionName,
        const std::string& var) {
        if (tolower(collectionName) == "ip"
            && !m_ip_collection_key.empty()) {
            return m_ip_collection->resolveFirst(toupper(collectionName)
                    + ":" + var, m_ip_collection_key);
        }

        if (tolower(collectionName) == "global"
            && !m_global_collection_key.empty()) {
            return m_global_collection->resolveFirst(toupper(collectionName)
                    + ":" + var, m_global_collection_key);
        }

        for (auto &a : *this) {
            if (tolower(a.first) == tolower(collectionName)) {
                std::string *res = a.second->resolveFirst(toupper(a.first)
                    + ":" + var);
                if (res != NULL) {
                    return res;
                }
            }
        }

        return NULL;
}


void Collections::resolveSingleMatch(const std::string& var,
    std::vector<const Variable *> *l) {

    m_transient->resolveSingleMatch(var, l);
}


void Collections::resolveSingleMatch(const std::string& var,
    const std::string& collection,
    std::vector<const Variable *> *l) {

    if (tolower(collection) == "ip"
        && !m_ip_collection_key.empty()) {
        m_ip_collection->resolveSingleMatch(var, m_ip_collection_key, l);
        return;
    }

    if (tolower(collection) == "global"
        && !m_global_collection_key.empty()) {
        m_global_collection->resolveSingleMatch(var,
            m_global_collection_key, l);
        return;
    }

    try {
        this->at(collection)->resolveSingleMatch(var, l);
    } catch (...) { }
}

void Collections::resolveMultiMatches(const std::string& var,
    std::vector<const Variable *> *l) {

    m_transient->resolveMultiMatches(var, l);
}


void Collections::resolveMultiMatches(const std::string& var,
    const std::string& collection,
    std::vector<const Variable *> *l) {
    if (tolower(collection) == "ip"
        && !m_ip_collection_key.empty()) {
        m_ip_collection->resolveMultiMatches(var, m_ip_collection_key, l);
        return;
    }

    if (tolower(collection) == "global"
        && !m_global_collection_key.empty()) {
        m_global_collection->resolveMultiMatches(var,
            m_global_collection_key, l);
        return;
    }

    try {
        this->at(collection)->resolveMultiMatches(var, l);
    } catch (...) { }
}

void Collections::resolveRegularExpression(const std::string& var,
    std::vector<const Variable *> *l) {
    m_transient->resolveRegularExpression(var, l);
}


void Collections::resolveRegularExpression(const std::string& var,
    const std::string& collection,
    std::vector<const Variable *> *l) {
    if (tolower(collection) == "ip"
        && !m_ip_collection_key.empty()) {
        m_ip_collection->resolveRegularExpression(toupper(collection)
            + ":" + var, m_ip_collection_key, l);
        return;
    }

    if (tolower(collection) == "global"
        && !m_global_collection_key.empty()) {
        m_global_collection->resolveRegularExpression(toupper(collection)
            + ":" + var, m_global_collection_key, l);
        return;
    }

    try {
        this->at(collection)->resolveRegularExpression(var, l);
    } catch (...) { }
}

}  // namespace collection
}  // namespace modsecurity
