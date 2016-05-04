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
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#endif

#include "modsecurity/collection/collection.h"
#include "modsecurity/collection/variable.h"


#ifndef HEADERS_MODSECURITY_TRANSACTION_GLOBAL_COLLECTION_H_
#define HEADERS_MODSECURITY_TRANSACTION_GLOBAL_COLLECTION_H_

#ifndef __cplusplus
typedef struct GlobalCollection_t GlobalCollection;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace collection {

class CollectionKey {
 public:
    CollectionKey()
        : m_compartiment(""),
        m_name("") { }
    explicit CollectionKey(std::string name)
        : m_compartiment(""),
        m_name(name) { }
    CollectionKey(std::string name, std::string compartiment)
        : m_compartiment(compartiment),
        m_name(name) { }

    std::string m_name;
    std::string m_compartiment;
};


class collection_hash {
 public:
    size_t operator()(const CollectionKey *v) const {
        size_t h = 0;
        std::for_each(v->m_name.begin(), v->m_name.end(), [&](char c) {
            h += tolower(c);
        });
        std::for_each(v->m_compartiment.begin(),
            v->m_compartiment.end(), [&](char c) {
            h += tolower(c);
        });

        return h;
    }
};


class collection_equal {
 public:
    bool operator()(const CollectionKey *u, const CollectionKey *v) const {
        return u->m_name == v->m_name
            && u->m_compartiment == v->m_compartiment;
    }
};


class GlobalCollection :
    public std::unordered_multimap<CollectionKey *, std::string,
        collection_hash, collection_equal> {
 public:
    GlobalCollection();
    ~GlobalCollection();
    void store(std::string key, std::string compartment, std::string value);

    bool storeOrUpdateFirst(const std::string &key, std::string compartment,
        const std::string &value);

    bool updateFirst(const std::string &key, std::string compartment,
        const std::string &value);

    void del(const std::string& key, std::string compartment);

    std::string* resolveFirst(const std::string& var, std::string compartment);
    void resolveSingleMatch(const std::string& var, std::string compartment,
        std::vector<const Variable *> *l);
    void resolveMultiMatches(const std::string& var, std::string compartment,
        std::vector<const Variable *> *l);

    void resolveRegularExpression(const std::string& var,
        std::string compartment,
        std::vector<const Variable *> *l);
};

}  // namespace collection
}  // namespace modsecurity

#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_GLOBAL_COLLECTION_H_


