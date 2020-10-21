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

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifndef SRC_VARIABLES_RESOURCE_H_
#define SRC_VARIABLES_RESOURCE_H_

#include "src/variables/variable.h"
#include "src/run_time_string.h"
#include "src/variables/variable_with_runtime_string.h"

namespace modsecurity {

class Transaction;
namespace variables {


class Resource_DictElement : public Variable {
 public:
    explicit Resource_DictElement(const std::string &dictElement)
        : Variable("RESOURCE:" + dictElement),
        m_dictElement("RESOURCE:" + dictElement) { }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {
        t->m_collections.m_resource_collection->resolveMultiMatches(
            m_name, t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value, l, m_keyExclusion);
    }

    std::string m_dictElement;
};


class Resource_NoDictElement : public Variable {
 public:
    Resource_NoDictElement()
        : Variable("RESOURCE") { }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {
        t->m_collections.m_resource_collection->resolveMultiMatches(m_name,
            t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value, l, m_keyExclusion);
    }
};


class Resource_DictElementRegexp : public VariableRegex {
 public:
    explicit Resource_DictElementRegexp(const std::string &dictElement)
        : VariableRegex("RESOURCE:", dictElement),
        m_dictElement(dictElement) { }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {
        t->m_collections.m_resource_collection->resolveRegularExpression(
            m_dictElement, t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value, l, m_keyExclusion);
    }

    std::string m_dictElement;
};


class Resource_DynamicElement : public VariableWithRunTimeString {
 public:
    explicit Resource_DynamicElement(std::unique_ptr<RunTimeString> dictElement)
        : VariableWithRunTimeString(
            "RESOURCE:dynamic",
            std::move(dictElement)
        )
    { }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {
        std::string string = m_string->evaluate(t);
        t->m_collections.m_resource_collection->resolveMultiMatches(
            string,
            t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value, l, m_keyExclusion);
    }

    static void del(Transaction *t, const std::string &k) {
        t->m_collections.m_resource_collection->del(k,
            t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value);
    }

    static void storeOrUpdateFirst(Transaction *t, const std::string &var,
        const std::string &value) {
        t->m_collections.m_resource_collection->storeOrUpdateFirst(
            var, t->m_collections.m_resource_collection_key,
            t->m_rules->m_secWebAppId.m_value, value);
    }
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_RESOURCE_H_
