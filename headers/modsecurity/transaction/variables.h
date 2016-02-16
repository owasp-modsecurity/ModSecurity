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
#endif


#include "modsecurity/transaction/variable.h"

#ifndef HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_
#define HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_

#ifndef __cplusplus
typedef struct Variable_t Variables;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace transaction {

class Variables :
    public std::unordered_multimap<std::string, std::string> {
 public:
    Variables();
    ~Variables();
    void store(std::string key, std::string value);

    bool storeOrUpdateFirst(const std::string &key,
        const std::string &value);

    bool updateFirst(const std::string &key, const std::string &value);

    void del(const std::string& key);

    std::string* resolveFirst(const std::string& var);


    void resolveSingleMatch(const std::string& var,
        std::vector<const transaction::Variable *> *l);
    void resolveMultiMatches(const std::string& var,
        std::vector<const transaction::Variable *> *l);
    void resolveRegularExpression(const std::string& var,
        std::vector<const transaction::Variable *> *l);
};

}  // namespace transaction
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_VARIABLES_H_
