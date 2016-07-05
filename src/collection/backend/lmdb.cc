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


#include "src/collection/backend/lmdb.h"

#include "lmdb.h"

#include <string>

#include "modsecurity/collection/variable.h"
#include "src/utils.h"

namespace modsecurity {
namespace collection {
namespace backend {


LMDB::LMDB() {
}

LMDB::~LMDB() {
}

void LMDB::store(std::string key, std::string value) {

}


bool LMDB::storeOrUpdateFirst(const std::string &key,
    const std::string &value) {
    return true;
}


bool LMDB::updateFirst(const std::string &key,
    const std::string &value) {

}


void LMDB::del(const std::string& key) {
}


void LMDB::resolveSingleMatch(const std::string& var,
    std::vector<const Variable *> *l) {

}


void LMDB::resolveMultiMatches(const std::string& var,
    std::vector<const Variable *> *l) {

}


void LMDB::resolveRegularExpression(const std::string& var,
    std::vector<const Variable *> *l) {
}


std::string* LMDB::resolveFirst(const std::string& var) {
}


}  // namespace backend
}  // namespace collection
}  // namespace modsecurity
