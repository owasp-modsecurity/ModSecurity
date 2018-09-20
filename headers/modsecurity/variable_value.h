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
#include <utility>
#endif

#include "modsecurity/variable_origin.h"

#ifndef HEADERS_MODSECURITY_VARIABLE_VALUE_H_
#define HEADERS_MODSECURITY_VARIABLE_VALUE_H_

#ifndef __cplusplus
typedef struct Variable_t VariableValue;
#endif

#ifdef __cplusplus
namespace modsecurity {

class Collection;
class VariableValue {
 public:
    explicit VariableValue(const std::string *key)
        : m_key(""),
        m_value("") {
            m_key.assign(*key);
            m_keyWithCollection = std::make_shared<std::string>(*key);
        }

    VariableValue(const std::string *key, const std::string *value)
        : m_key(""),
        m_value("") {
            m_key.assign(*key);
            m_value.assign(*value);
            m_keyWithCollection = std::make_shared<std::string>(*key);
        }

    VariableValue()
        : m_key(""),
        m_value("") {
            m_keyWithCollection = std::make_shared<std::string>(m_key);
        }

    VariableValue(const std::string *a, const std::string *b,
        const std::string *c)
        : m_key(*a + ":" + *b),
        m_value(*c) {
            m_keyWithCollection = std::make_shared<std::string>(*a + ":" + *b);
        }

    explicit VariableValue(std::shared_ptr<std::string> fullName)
        : m_key(""),
        m_value("") {
            m_keyWithCollection = fullName;
            m_key.assign(*fullName.get());
        }

    VariableValue(std::shared_ptr<std::string> fullName,
        const std::string *value)
        : m_key(""),
        m_value("") {
            m_value.assign(*value);
            m_keyWithCollection = fullName;
            m_key.assign(*fullName.get());
        }


    explicit VariableValue(const VariableValue *o) :
        m_key(""),
        m_value("") {
        m_key.assign(o->m_key);
        m_value.assign(o->m_value);
        m_col.assign(o->m_col);
        m_keyWithCollection = o->m_keyWithCollection;

        for (auto &i : o->m_orign) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            origin->m_offset = i->m_offset;
            origin->m_length = i->m_length;
            m_orign.push_back(std::move(origin));
        }
    }

    std::string m_key;
    std::string m_value;
    std::string m_col;
    std::shared_ptr<std::string> m_keyWithCollection;
    std::list<std::unique_ptr<VariableOrigin>> m_orign;
};

}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_VARIABLE_VALUE_H_
