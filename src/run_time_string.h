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


#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <utility>

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/variables/variable.h"
#include "src/variables/rule_variable.h"


#ifndef SRC_RUN_TIME_STRING_H_
#define SRC_RUN_TIME_STRING_H_

namespace modsecurity {

class RunTimeElementHolder {
 public:
    RunTimeElementHolder()
        : m_string(""),
        m_variable(nullptr)
    { };


    RunTimeElementHolder(const RunTimeElementHolder &other)
        : m_string(other.m_string),
        m_variable(other.m_variable) {
            variables::RuleVariable *rv = dynamic_cast<variables::RuleVariable *>(m_variable.get());
            if (rv != nullptr) {
                auto nrv = rv->clone();
                rv = dynamic_cast<variables::RuleVariable *>(nrv);
                rv->populate(nullptr);
                m_variable = std::unique_ptr<variables::Variable>(nrv);
            }
        };

 /* protected: */
    std::string m_string;
    std::shared_ptr<variables::Variable> m_variable;
};

class RunTimeString {
 public:
    RunTimeString()
        : m_containsMacro(false),
        m_elements()
    { };


    RunTimeString(const RunTimeString &other)
        : m_containsMacro(other.m_containsMacro),
        m_elements()
    {
        for (auto &m : other.m_elements) {
            m_elements.push_back(std::unique_ptr<RunTimeElementHolder>(new RunTimeElementHolder(*m.get())));
        }
    };


    void appendText(const std::string &text);
    void appendVar(std::unique_ptr<modsecurity::variables::Variable> var);


    std::string evaluate(Transaction *t);

    inline std::string evaluate() {
        return evaluate(NULL);
    }


    inline bool containsMacro() const { return m_containsMacro; }


    void populate(RuleWithActions *rule) {
        for (auto &a : m_elements) {
            modsecurity::variables::RuleVariable *vrule = dynamic_cast<variables::RuleVariable *>(a->m_variable.get());
            if (vrule != nullptr) {
                vrule->populate(rule);
            }
        }
    }

 private:
    bool m_containsMacro;
    std::list<std::unique_ptr<RunTimeElementHolder>> m_elements;
};


}  // namespace modsecurity



#endif  // SRC_RUN_TIME_STRING_H_
