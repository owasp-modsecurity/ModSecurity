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
#include <vector>
#include <algorithm>
#endif


#include "modsecurity/collection/variable.h"
#include "modsecurity/collection/collection.h"

#ifndef HEADERS_MODSECURITY_COLLECTION_BACKEND_IN_MEMORY_PER_PROCESS_H_
#define HEADERS_MODSECURITY_COLLECTION_BACKEND_IN_MEMORY_PER_PROCESS_H_

#ifdef __cplusplus
namespace modsecurity {
namespace collection {
namespace backend {

/*
 * FIXME:
 *
 * This was an example grabbed from:
 * http://stackoverflow.com/questions/8627698/case-insensitive-stl-containers-e-g-stdunordered-set
 *
 * We have to have a better hash function, maybe based on the std::hash.
 *
 */
struct MyEqual {
    bool operator()(const std::string& Left, const std::string& Right) const {
        /*
        return Left.size() == Right.size()
             && std::equal(Left.begin(), Left.end(), Right.begin(),
            [](char a, char b) {
            return tolower(a) == tolower(b);
        });
        */
        return Left == Right;
    }
};

struct MyHash{
    size_t operator()(const std::string& Keyval) const {
        // You might need a better hash function than this
        size_t h = 0;
        std::for_each(Keyval.begin(), Keyval.end(), [&](char c) {
            h += c;
        });
        return h;
    }
};

class InMemoryPerProcess :
    public std::unordered_multimap<std::string, std::string,
        /*std::hash<std::string>*/MyHash, MyEqual>,
    public Collection {
 public:
    InMemoryPerProcess();
    ~InMemoryPerProcess();
    void store(std::string key, std::string value);

    bool storeOrUpdateFirst(const std::string &key,
        const std::string &value);

    bool updateFirst(const std::string &key, const std::string &value);

    void del(const std::string& key);

    std::string* resolveFirst(const std::string& var);

    void resolveSingleMatch(const std::string& var,
        std::vector<const Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        std::vector<const Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        std::vector<const Variable *> *l);
};

}  // namespace backend
}  // namespace collection
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_COLLECTION_BACKEND_IN_MEMORY_PER_PROCESS_H_
