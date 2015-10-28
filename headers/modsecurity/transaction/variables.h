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


#ifdef __cplusplus
#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#endif


#include "modsecurity/transaction/variable.h"


#ifndef HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_
#define HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_

#ifndef __cplusplus
typedef struct Variable_t Variables;
#endif

#ifdef __cplusplus
namespace ModSecurity {
namespace transaction {

class Variables :
    public std::unordered_multimap<std::string, std::string> {
 public:
    Variables() {
        this->reserve(1000);
    }


    void storeVariable(std::string key, std::string value) {
        this->emplace(key, value);
    }


    bool storeOrUpdateVariable(const std::string &key,
        const std::string &value) {
        if (updateFirstVariable(key, value) == false) {
            storeVariable(key, value);
        }
        return true;
    }


    bool updateFirstVariable(const std::string &key, const std::string &value) {
        auto range = this->equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            it->second = value;
            return true;
        }
        return false;
    }


    void deleteVariable(const std::string& key) {
        this->erase(key);
    }


    std::list<Variable *>
        resolveVariable(const std::string& key,
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
        resolveVariable(const std::string& key) {
        std::list<Variable *> l;

        return resolveVariable(key, &l);
    }
};

}  // namespace transaction
}  // namespace ModSecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_
