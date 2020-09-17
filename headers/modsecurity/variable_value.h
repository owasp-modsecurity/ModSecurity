/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <vector>
#endif

#include "modsecurity/variable_origin.h"
#ifdef __cplusplus
#include "modsecurity/string_view.hpp"
#endif

#ifndef HEADERS_MODSECURITY_VARIABLE_VALUE_H_
#define HEADERS_MODSECURITY_VARIABLE_VALUE_H_

#ifndef __cplusplus
typedef struct Variable_t VariableValue;
#endif

#ifdef __cplusplus
namespace modsecurity {

class VariableValue;
using VariableValues = std::vector<std::shared_ptr<const VariableValue>>;
using Origins = std::vector<VariableOrigin>;

class Collection;
class VariableValue {
 public:
    /**
    *
    *
    * Use cases for VariableValue creation:
    *
    * AnchoredSet - Use case A (eg. ARGS).         - Collection + Key
    * Anchored    - Use case B (eg. REQUEST_URI).  - Key
    * Custom      - Use case C (eg. WEBAPP_ID).    - Key
    * CustomSet
    *     Fixed   - Use case D (eg. TX).           - Collection + Key
    *     Dynamic - Use case E (eg. ENV).          - Collection + Key
    *
    *
    *     | Key  | Collec. | Key + Collec. | Value | Full Name
    *   A |  &   |   &     |   *           |   &   |   *
    *   B |  x   |   &     |   *           |   &   |   &
    *   C |  x   |   &     |   *           |   &   |   &
    *   D |  &   |   &     |   *           |   &   |   *
    *   E |  &   |   &     |   *           |   *   |   *
    *
    *
    *
    *  Currently big picture of getName and getKey:
    *
    *  getName()
    *    - Lua Engine     - RuleWithOperator                     - Transaction
    *                       - UpdateMatchedVar                       - logging (audit)
    *                       - ExecuteOperatorAt
    *                       - ResolveMatchMsg
    *                       - RulesExceptions (key and value)
    *
    *
    *  getKey()
    *    - Transaction       - Variable
    *      - LogGen            - Contains
    *                            - Regexp
    *
    *
    * Performance strategy: Delay the name resolution till is really necessary.
    *
    *
    */


    /**
     *
     *  Use case C + VariableModificatorCount
     *
     *
     **/
    explicit VariableValue(const std::string *collection,
        const std::string *value = nullptr)
        : m_origin(),
        m_value(new std::string(value != nullptr?*value:"")), // FIXME: no need for to copy here.
        m_valueHolder(nullptr),
        m_key(nullptr),
        m_keyHolder(nullptr),
        m_collection(new std::string(*collection)) // FIXME: no need for to copy here.
    { };


    /* Use case D.1. - ARGS */
    VariableValue(const std::string *collection,
        std::unique_ptr<std::string> key,
        std::unique_ptr<std::string> value)
        : m_origin(),
        m_value(nullptr),
        m_valueHolder(std::move(value)),
        m_key(nullptr),
        m_keyHolder(std::move(key)),
        m_collection(collection)
    {
        m_value = m_valueHolder.get();
        m_key = m_keyHolder.get();
    };


    /* Use case D.2. - RULE */
    VariableValue(const std::string *collection,
        const std::string *key,
        std::unique_ptr<std::string> value)
        : m_origin(),
        m_value(nullptr),
        m_valueHolder(std::move(value)),
        m_key(key),
        m_keyHolder(nullptr),
        m_collection(collection)
    {
        m_value = m_valueHolder.get();
    };


    /* Use case D.3. - TX */
    VariableValue(const std::string *collection,
        const std::string *key,
        const std::string *value)
        : m_origin(),
        m_value(value),
        m_valueHolder(nullptr),
        m_key(key),
        m_keyHolder(nullptr),
        m_collection(collection)
    { };


    // FIXME: It maybe the case for VariableValue to use string_view for everything.
    /* Use case D.4. - MATCHED_VARS */
    VariableValue(const std::string *collection,
        const std::string *key,
        const bpstd::string_view *value)
        : m_origin(),
        m_value(),
        m_valueHolder(std::unique_ptr<std::string>(new std::string(value->c_str()))),
        m_key(key),
        m_keyHolder(nullptr),
        m_collection(collection)
    {
        m_value = m_valueHolder.get();
    };


    /* Use case E.1. - Env */
    VariableValue(std::unique_ptr<std::string> value,
        std::unique_ptr<std::string> key,
        std::shared_ptr<std::string> collection)
        : m_origin(),
        m_value(nullptr),
        m_valueHolder(std::move(value)),
        m_key(nullptr),
        m_keyHolder(std::move(key)),
        m_collection(collection.get())
    {
        m_value = m_valueHolder.get();
        m_key = m_keyHolder.get();
    };


    /* Use case E.2. - DURATION */
    VariableValue(std::unique_ptr<std::string> value,
        const std::string *collection)
        : m_origin(),
        m_value(nullptr),
        m_valueHolder(std::move(value)),
        m_key(nullptr),
        m_keyHolder(nullptr),
        m_collection(collection)
    {
        m_value = m_valueHolder.get();
    };


    VariableValue(const VariableValue &o) = delete;
    VariableValue operator=(const VariableValue &o) = delete;


    const std::string& getValue() const noexcept {
        return *m_value;
    }


    const std::string& getKey() const noexcept {
        return *m_key;
    }


    inline const std::string getName() const noexcept {
        if (m_key == nullptr || m_key->empty()) {
            return *m_collection;
        }
        return *m_collection + ":" + *m_key;
    }


    void setValue(const std::string &value) {
        m_value = &value;
    }


    void addOrigin(VariableOrigin origin) {
        m_origin.push_back(std::move(origin));
    }


    const Origins& getOrigin() const {
        return m_origin;
    }

 private:
    Origins m_origin;

    const std::string *m_value;
    std::unique_ptr<std::string> m_valueHolder;

    const std::string *m_key;
    std::unique_ptr<std::string> m_keyHolder;

    const std::string *m_collection;
};

}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_VARIABLE_VALUE_H_
