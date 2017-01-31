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

#ifndef SRC_VARIABLES_FILES_SIZES_H_
#define SRC_VARIABLES_FILES_SIZES_H_

#include "src/variables/variable.h"

namespace modsecurity {

class Transaction;
namespace Variables {

class FilesSizes_DictElement : public Variable {
 public:
    explicit FilesSizes_DictElement(std::string dictElement)
        : Variable("FILES_SIZES" + std::string(":") +
            std::string(dictElement)),
        m_dictElement(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableFilesSizes.resolve(m_dictElement, l);
    }

    std::string m_dictElement;
};


class FilesSizes_NoDictElement : public Variable {
 public:
    FilesSizes_NoDictElement()
        : Variable("FILES_SIZES") { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableFilesSizes.resolve(l);
    }
};


class FilesSizes_DictElementRegexp : public Variable {
 public:
    explicit FilesSizes_DictElementRegexp(std::string dictElement)
        : Variable("FILES_SIZES"),
        m_r(dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_variableFilesSizes.resolveRegularExpression(
            &m_r, l);
    }

    Utils::Regex m_r;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_FILES_SIZES_H_

