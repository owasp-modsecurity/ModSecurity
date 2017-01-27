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

#ifndef SRC_VARIABLES_REQUEST_HEADERS_H_
#define SRC_VARIABLES_REQUEST_HEADERS_H_

#include "src/variables/variable.h"

namespace modsecurity {

class Transaction;
namespace Variables {

class RequestHeaders_DictElement : public Variable {
 public:
    RequestHeaders_DictElement(std::string dictElement)
        : Variable("REQUEST_HEADERS" + std::string(":") +
            std::string(dictElement)),
        m_dictElement(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        transaction->m_variableRequestHeaders.resolve(m_dictElement, l);
    }

    std::string m_dictElement;
};


class RequestHeaders_NoDictElement : public Variable {
 public:
    RequestHeaders_NoDictElement()
        : Variable("REQUEST_HEADERS") { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        transaction->m_variableRequestHeaders.resolve(l);
    }
};


class RequestHeaders_DictElementRegexp : public Variable {
 public:
    RequestHeaders_DictElementRegexp(std::string dictElement)
        : Variable("REQUEST_HEADERS"),
        m_r(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableRequestHeaders.resolveRegularExpression(
            &m_r, l);
    }

    Utils::Regex m_r;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_REQUEST_HEADERS_H_

