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

#ifndef HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_
#define HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_

#ifndef __cplusplus
typedef struct Variable_t Variables;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace collection {

class Collection {
 public:
    virtual void store(std::string key, std::string value) = 0;

    virtual bool storeOrUpdateFirst(const std::string &key,
        const std::string &value) = 0;

    virtual bool updateFirst(const std::string &key, const std::string &value) = 0;

    virtual void del(const std::string& key) = 0;

    virtual std::string* resolveFirst(const std::string& var) = 0;

    virtual void resolveSingleMatch(const std::string& var,
        std::vector<const Variable *> *l) = 0;
    virtual void resolveMultiMatches(const std::string& var,
        std::vector<const Variable *> *l) = 0;
    virtual void resolveRegularExpression(const std::string& var,
        std::vector<const Variable *> *l) = 0;
};

}  // namespace collection
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_
