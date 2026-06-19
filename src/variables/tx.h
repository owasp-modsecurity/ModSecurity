/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <memory>

#ifndef SRC_VARIABLES_TX_H_
#define SRC_VARIABLES_TX_H_

#include "src/variables/variable.h"
#include "src/run_time_string.h"
#include "src/collection/backend/in_memory-per_process.h"

namespace modsecurity {

class Transaction;
namespace variables {


class Tx_DictElement : public Variable {
 public:
    explicit Tx_DictElement(const std::string &dictElement)
        : Variable("TX:" + dictElement),
        m_dictElement("TX:" + dictElement) { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        t->m_collections.m_tx_collection->resolveMultiMatches(
            m_name, l, m_keyExclusion);
    }

    std::string m_dictElement;
};


class Tx_NoDictElement : public Variable {
 public:
    Tx_NoDictElement()
        : Variable("TX") { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        t->m_collections.m_tx_collection->resolveMultiMatches("", l,
            m_keyExclusion);
    }
};


class Tx_DictElementRegexp : public VariableRegex {
 public:
    explicit Tx_DictElementRegexp(const std::string &dictElement)
        : VariableRegex("TX", dictElement) { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        // TX is always backed by InMemoryPerProcess. Reuse the regex compiled
        // once in VariableRegex::m_r via the backend's concrete fast path,
        // instead of recompiling the pattern on every transaction. This is kept
        // off the Collection base class to avoid an ABI-breaking vtable change;
        // any other backend falls back to the string-based resolution.
        auto *collection = t->m_collections.m_tx_collection;
        if (auto *inMemory =
                dynamic_cast<collection::backend::InMemoryPerProcess *>(
                    collection)) {
            inMemory->resolveRegularExpression(&m_r, l, m_keyExclusion);
        } else {
            collection->resolveRegularExpression(m_r.pattern, l,
                m_keyExclusion);
        }
    }
};


class Tx_DynamicElement : public Variable {
 public:
    explicit Tx_DynamicElement(std::unique_ptr<RunTimeString> dictElement)
        : Variable("TX:dynamic"),
        m_string(std::move(dictElement)) { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        std::string string = m_string->evaluate(t);
        t->m_collections.m_tx_collection->resolveMultiMatches(string, l,
            m_keyExclusion);
    }

    static void del(Transaction *t, const std::string &k) {
        t->m_collections.m_tx_collection->del(k);
    }

    static void storeOrUpdateFirst(Transaction *t, const std::string &var,
        const std::string &value) {
        t->m_collections.m_tx_collection->storeOrUpdateFirst(var, value);
    }

    std::unique_ptr<RunTimeString> m_string;
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_TX_H_
