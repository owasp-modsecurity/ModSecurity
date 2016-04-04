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

#include "modsecurity/transaction/global_variables.h"
#include "modsecurity/transaction/variables.h"
#include "modsecurity/transaction/variable.h"
#include "modsecurity/transaction/collection.h"

#ifndef HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_
#define HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_

#ifndef __cplusplus
typedef struct Variable_t Variable;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace transaction {

class Collections :
    public std::unordered_map<std::string, Collection *> {
 public:
    Collections(GlobalVariables *global, GlobalVariables *ip);
    ~Collections();

    void init(const std::string& name, const std::string& key);
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

    void resolveSingleMatch(const std::string& var,
        std::vector<const transaction::Variable *> *l);
    void resolveSingleMatch(const std::string& var,
        const std::string& collection,
        std::vector<const transaction::Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        std::vector<const transaction::Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        const std::string& collection,
        std::vector<const transaction::Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        std::vector<const transaction::Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        const std::string& collection,
        std::vector<const transaction::Variable *> *l);

    /**
     * This is a special collection to host the transaction variables.
     *
     * It exists independent of initialization and it is only valid during a transaction.
     *
     * Notice that it is not the TX collection.
     */
    transaction::Variables m_transient;

    std::string m_global_collection_key;
    std::string m_ip_collection_key;

    transaction::GlobalVariables *m_global_collection;
    transaction::GlobalVariables *m_ip_collection;
};

}  // namespace transaction
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_COLLECTIONS_H_


