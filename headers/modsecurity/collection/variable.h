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
#include <memory>
#include <list>
#endif

#include "modsecurity/variable_origin.h"

#ifndef HEADERS_MODSECURITY_COLLECTION_VARIABLE_H_
#define HEADERS_MODSECURITY_COLLECTION_VARIABLE_H_

#ifndef __cplusplus
typedef struct Variable_t Variable;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace collection {

class Variable {
 public:
    explicit Variable(const std::string *key) :
        m_dynamic(true) {
            m_key.reset(new std::string(*key));
        }
    Variable(const std::string *key, const std::string *value) :
        m_dynamic(true) {
            m_key.reset(new std::string(*key));
            m_value.reset(new std::string(*value));

        }
    explicit Variable(const std::shared_ptr<std::string> key) :
        m_dynamic(true) {
            m_key = key;
        }
    Variable(std::shared_ptr<std::string> key, std::shared_ptr<std::string> value) :
        m_dynamic(true) {
            m_key = key;
            m_value = value;

        }
    ~Variable() {
    }

    std::shared_ptr<std::string> m_key;
    std::shared_ptr<std::string> m_value;
    std::list<std::unique_ptr<VariableOrigin>> m_orign;
    bool m_dynamic;
};

}  // namespace collection
}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_COLLECTION_VARIABLE_H_
