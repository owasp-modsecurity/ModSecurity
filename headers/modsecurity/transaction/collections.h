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
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <iomanip>
#include <set>
#include <cstdio>
#include <string>
#include <list>
#endif

#include "modsecurity/transaction/variables.h"
#include "modsecurity/transaction/variable.h"

#ifndef HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_
#define HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_

#ifndef __cplusplus
typedef struct Variable_t Variable;
#endif

#ifdef __cplusplus
namespace ModSecurity {
namespace transaction {

class Collections :
    public std::unordered_map<std::string, transaction::Variables *> {
 public:
    Collections();
    ~Collections();

    void store(std::string key, std::string value);
    void storeOrUpdateFirst(const std::string& collectionName,
        const std::string& variableName,
        const std::string& targetValue);
    bool storeOrUpdateFirst(const std::string &key, const std::string &value);
    bool updateFirst(const std::string &key, const std::string &value);
    void del(const std::string& key);
    std::string* resolveFirst(const std::string& var);
    std::string* resolveFirst(const std::string& collectionName,
        const std::string& var);

    void resolve(const std::string& var,
        std::list<transaction::Variable *> *l);
    std::list<transaction::Variable *> *resolve(const std::string& var);


    /**
     * This is a special collection to host the transaction variables.
     *
     * It exists independent of initialization and it is only valid during a transaction.
     *
     * Notice that it is not the TX collection.
     */
    transaction::Variables m_transient;
};

}  // namespace transaction
}  // namespace ModSecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_


