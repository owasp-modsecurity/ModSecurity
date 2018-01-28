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

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_TX_H_
#define SRC_VARIABLES_TX_H_

#include "src/variables/variable.h"
#include "src/run_time_string.h"

namespace modsecurity {

class Transaction;
namespace Variables {


class Tx_DictElement : public Variable {
 public:
    explicit Tx_DictElement(std::string dictElement)
        : Variable("TX:" + dictElement),
        m_dictElement("TX:" + dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveMultiMatches(m_dictElement, "TX", l);
    }

    std::string m_dictElement;
};


class Tx_NoDictElement : public Variable {
 public:
    Tx_NoDictElement()
        : Variable("TX") { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveMultiMatches(m_name, "TX", l);
    }
};


class Tx_DictElementRegexp : public Variable {
 public:
    explicit Tx_DictElementRegexp(std::string dictElement)
        : Variable("TX"),
        m_r(dictElement),
        m_dictElement("TX:" + dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveRegularExpression(m_dictElement,
            "TX", l);
    }

    Utils::Regex m_r;
    std::string m_dictElement;
};


class Tx_DynamicElement : public Variable {
 public:
    explicit Tx_DynamicElement(std::unique_ptr<RunTimeString> dictElement)
        : Variable("TX:dynamic"),
        m_string(std::move(dictElement)) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        std::string string = m_string->evaluate(transaction);
        transaction->m_collections.resolveMultiMatches("TX:" + string, "TX", l);
    }

    std::unique_ptr<RunTimeString> m_string;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_TX_H_
