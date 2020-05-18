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
#include <string>
#include <vector>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_XML_H_
#define SRC_VARIABLES_XML_H_

#include "src/variables/variable.h"
#include "src/variables/xml.h"
#include "src/variables/rule_variable.h"

namespace modsecurity {

class Transaction;
namespace variables {

/* Invocation without an XPath expression makes sense
 * with functions that manipulate the document tree.
 */
class XML_WithoutNSPath : public RuleVariable, public Variable {
 public:
    XML_WithoutNSPath()
        : RuleVariable(),
        Variable("XML"),
        m_plain("[XML document tree]"),
        m_var(&m_name, &m_plain)
    { };

    XML_WithoutNSPath(const XML_WithoutNSPath &r)
        : RuleVariable(r),
        Variable(r),
        m_plain(r.m_plain),
        m_var(r.m_var)
    { };

    void evaluate(Transaction *transaction,
        std::vector<const VariableValue *> *l) override {
        l->push_back(new VariableValue(&m_var));
    }

    virtual variables::Variable *clone() override {
        return new XML_WithoutNSPath(*this);
    };

    std::string m_plain;
    VariableValue m_var;
};

class XML_WithNSPath : public RuleVariable, public VariableDictElement {
 public:
    explicit XML_WithNSPath(const std::string &nsPath)
        : RuleVariable(),
        VariableDictElement("XML", nsPath)
    { };

    XML_WithNSPath(const XML_WithNSPath &r)
        : RuleVariable(),
        VariableDictElement(r)
    { };

    void evaluate(Transaction *transaction,
        std::vector<const VariableValue *> *l) override;

    virtual Variable *clone() override {
        return new XML_WithNSPath(*this);
    };
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_XML_H_
