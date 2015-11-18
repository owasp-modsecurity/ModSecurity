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


#ifndef HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_
#define HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_


#ifndef __cplusplus
typedef struct Collection_t Collection;
#endif


#ifdef __cplusplus
#include <string>

namespace ModSecurity {

class Collection : public transaction::Variables {
 public:
     Collection(std::string name, std::string key)
       : m_name(name),
       m_persisted(false),
       m_timeout(0),
       m_updateCounter(0),
       m_updateRate(0),
       m_key(key) { }

     std::string m_name;
     clock_t m_createTime;
     bool m_persisted; /* IS_NEW from the old documentation */
     std::string m_key;
     clock_t m_lastUpdateTime;
     double m_timeout;
     double m_updateCounter;
     double m_updateRate;
};

}  // namespace ModSecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_COLLECTION_H_


