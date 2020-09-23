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

class RunTimeString {
 public:
    using Variable = variables::Variable;
    using RuleVariable = variables::RuleVariable;

    RunTimeString()
        : m_containsMacro(false),
        m_elements()
    { };


    RunTimeString(const RunTimeString &other)
        : m_containsMacro(other.m_containsMacro),
        m_elements()
    {
        for (auto &m : other.m_elements) {
            m_elements.emplace_back(new ElementHolder(*m.get()));
        }
    };

    RunTimeString& operator=(RunTimeString other)
    {
        m_containsMacro = other.m_containsMacro;
        for (auto &m : other.m_elements) {
            m_elements.emplace_back(new ElementHolder(*m.get()));
        }
        return *this;
    }


    void append(const std::string &text);
    void append(std::unique_ptr<Variable> var);


    std::string evaluate(const Transaction *t = nullptr) const noexcept;


    inline bool containsMacro() const noexcept {
        return m_containsMacro;
    }


    void populate(const RuleWithActions *rule) noexcept {
        for (auto &a : m_elements) {
            a->populate(rule);
        }
    }


    class ElementHolder {
     public:
        ElementHolder()
            : m_string(""),
            m_variable(nullptr)
        { };
    
        explicit ElementHolder(std::unique_ptr<Variable> variable)
            : m_string(""),
            m_variable(std::move(variable))
        { };
    
        explicit ElementHolder(const std::string &str)
            : m_string(str),
            m_variable(nullptr)
        { };

        ElementHolder(const ElementHolder &other)
            : m_string(other.m_string),
            m_variable(nullptr) {
                RuleVariable *rv = dynamic_cast<RuleVariable *>(other.m_variable.get());
                if (rv != nullptr) {
                    auto nrv = rv->clone();
                    rv = dynamic_cast<RuleVariable *>(nrv);
                    rv->populate(nullptr);
                    m_variable = std::unique_ptr<Variable>(nrv);
                } else {
                    m_variable = other.m_variable;
                }

            };


        void appendValueTo(const Transaction *transaction, std::string &v) const noexcept {
            if (m_variable && transaction) {
                VariableValues l;

                m_variable->evaluate(transaction, &l);

                if (!l.empty()) {
                    v.append(l[0]->getValue());
                }
                return;
            }

            v.append(m_string);
        }
    

        void populate(const RuleWithActions *rule) noexcept {
            if (!m_variable) {
                return;
            }

            RuleVariable *vrule = dynamic_cast<RuleVariable *>(m_variable.get());
            if (!vrule) {
                return;
            }
            vrule->populate(rule);
        }

     private:
        const std::string m_string;
        /*
         *
         * FIXME: In the current state m_variable should be a unique_ptr. There
         *        is no copy for variables, thus having a shared pointer here.
         *        As an optimization we can have it as a shared_ptr to reduce the
         *        memory footprint in anchored variables.
         *
         */
        std::shared_ptr<Variable> m_variable;
    };
    
 private:
    bool m_containsMacro:1;
    std::vector<std::unique_ptr<ElementHolder>> m_elements;
};


}  // namespace modsecurity



#endif  // SRC_RUN_TIME_STRING_H_
