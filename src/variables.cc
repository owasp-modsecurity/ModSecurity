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

namespace ModSecurity {
namespace transaction {


Variables::Variables() :
    m_collections(NULL) {
    this->reserve(1000);
}

void Variables::setCollections(std::unordered_map<std::string,
    transaction::Variables *> *c) {
    m_collections = c;
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


std::list<Variable *>
        Variables::resolveInt(const std::string& key,
        std::list<Variable *> *l) {
        auto range = this->equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            l->push_back(new transaction::Variable(key, it->second));
        }

        if (key.find(":") == std::string::npos && l->size() == 0) {
            size_t keySize = key.size() + 1;
            for (auto& x : *this) {
                if (x.first.size() <= keySize) {
                    continue;
                }
                if (x.first.at(keySize - 1) != ':') {
                    continue;
                }
                if (x.first.compare(0, keySize, key + ":") != 0) {
                    continue;
                }
                //  auto range = this->equal_range(x.first);

                //  for (auto it = range.first; it != range.second; ++it) {
                l->push_back(new transaction::Variable(x.first, x.second));
                //  }
            }
        }

        return *l;
    }


std::list<Variable *>
        Variables::resolveInt(const std::string& key) {
        std::list<Variable *> l;

        return resolveInt(key, &l);
    }


std::string* Variables::resolveFirst(const std::string& var) {
        auto range = equal_range(var);

        for (auto it = range.first; it != range.second; ++it) {
            return &it->second;
        }

        if (m_collections == NULL) {
            return NULL;
        }

        for (auto &a : *m_collections) {
            auto range = a.second->equal_range(var);
            for (auto it = range.first; it != range.second; ++it) {
                return &it->second;
            }
        }

        return NULL;
    }


std::string* Variables::resolveFirst(const std::string& collectionName,
        const std::string& var) {
        if (m_collections == NULL) {
            return NULL;
        }

        for (auto &a : *m_collections) {
            if (tolower(a.first) == tolower(collectionName)) {
                auto range = a.second->equal_range(toupper(collectionName)
                    + ":" + var);
                for (auto it = range.first; it != range.second; ++it) {
                    return &it->second;
                }
            }
        }

        return NULL;
    }


void Variables::resolve(const std::string& var,
    std::list<transaction::Variable *> *l) {

    resolveInt(var, l);

    if (m_collections == NULL) {
        return;
    }

    /* It may be a collection */
    for (auto &a : *m_collections) {
        a.second->resolveInt(var, l);
    }
}

std::list<transaction::Variable *> *
    Variables::resolve(const std::string& var) {
    std::list<transaction::Variable *> *l =
        new std::list<transaction::Variable *>();

    resolve(var, l);

    return l;
}


}  // namespace transaction
}  // namespace ModSecurity
