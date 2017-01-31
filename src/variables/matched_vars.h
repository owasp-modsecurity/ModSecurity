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

#ifndef SRC_VARIABLES_MATCHED_VARS_H_
#define SRC_VARIABLES_MATCHED_VARS_H_

#include "src/variables/variable.h"

namespace modsecurity {

class Transaction;
namespace Variables {

class MatchedVars_DictElement : public Variable {
 public:
    explicit MatchedVars_DictElement(std::string dictElement)
        : Variable("MATCHED_VARS" + std::string(":") +
            std::string(dictElement)),
        m_dictElement(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableMatchedVars.resolve(m_dictElement, l);
    }

    std::string m_dictElement;
};


class MatchedVars_NoDictElement : public Variable {
 public:
    MatchedVars_NoDictElement()
        : Variable("MATCHED_VARS") { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableMatchedVars.resolve(l);
    }
};


class MatchedVars_DictElementRegexp : public Variable {
 public:
    explicit MatchedVars_DictElementRegexp(std::string dictElement)
        : Variable("MATCHED_VARS"),
        m_r(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableMatchedVars.resolveRegularExpression(
            &m_r, l);
    }

    Utils::Regex m_r;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_MATCHED_VARS_H_

