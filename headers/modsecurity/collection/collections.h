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

#include "modsecurity/collection/collection.h"
#include "modsecurity/collection/variable.h"

#ifndef HEADERS_MODSECURITY_COLLECTION_COLLECTIONS_H_
#define HEADERS_MODSECURITY_COLLECTION_COLLECTIONS_H_

#ifndef __cplusplus
typedef struct Collections_t Collections;
#endif

#ifdef __cplusplus

namespace modsecurity {
namespace collection {

class Collections :
    public std::unordered_map<std::string, Collection *> {
 public:
    Collections(Collection *global, Collection *ip, Collection *session,
        Collection *user, Collection *resource);
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

    void resolveSingleMatch(const std::string& var,
        std::vector<const Variable *> *l);
    void resolveSingleMatch(const std::string& var,
        const std::string& collection,
        std::vector<const Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        std::vector<const Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        const std::string& collection,
        std::vector<const Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        std::vector<const Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        const std::string& collection,
        std::vector<const Variable *> *l);

    /**
     * This is a special collection to host the transaction variables.
     *
     * It exists independent of initialization and it is only valid during a transaction.
     *
     * Notice that it is not the TX collection.
     */
    Collection *m_transient;

    std::string m_global_collection_key;
    std::string m_ip_collection_key;
    std::string m_session_collection_key;
    std::string m_user_collection_key;
    std::string m_resource_collection_key;

    Collection *m_global_collection;
    Collection *m_ip_collection;
    Collection *m_session_collection;
    Collection *m_user_collection;
    Collection *m_resource_collection;
};

}  // namespace collection
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_COLLECTION_COLLECTIONS_H_


