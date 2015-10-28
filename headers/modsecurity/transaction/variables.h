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
    std::unordered_map<std::string, transaction::Variables *> *m_collections;

    Variables();
    void store(std::string key, std::string value);

    bool storeOrUpdateFirst(const std::string &key,
        const std::string &value);

    bool updateFirst(const std::string &key, const std::string &value);

    void del(const std::string& key);

    std::list<Variable *>
        resolveInt(const std::string& key,
        std::list<Variable *> *l);

    std::list<Variable *>
        resolveInt(const std::string& key);


    std::string* resolveFirst(const std::string& var);


    std::string* resolveFirst(const std::string& collectionName,
        const std::string& var);

    void setCollections(std::unordered_map<std::string,
        transaction::Variables *> *c);

    std::list<transaction::Variable *> *
        resolve(const std::string& var);

    void resolve(const std::string& var,
        std::list<transaction::Variable *> *l);
};

}  // namespace transaction
}  // namespace ModSecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_
