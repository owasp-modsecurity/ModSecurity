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

#include <vector>
#include <string>
#include <list>
#include <utility>
#include <memory>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/utils/string.h"
#include "src/utils/regex.h"

#ifndef SRC_VARIABLES_VARIABLE_H_
#define SRC_VARIABLES_VARIABLE_H_

namespace modsecurity {

class Transaction;
namespace Variables {

class Variable {
 public:
    /**
     *
     */
    enum VariableType {
      /**
       *
       */
      SingleMatch,
      /**
       *
       */
      MultipleMatches,
      /**
       *
       */
      RegularExpression
    };

    /**
     *
     */
    enum VariableKind {
      /**
       *
       */
      DirectVariable,
      /**
       *
       */
      CollectionVarible,
    };

    explicit Variable(std::string _name);
    Variable(std::string name, VariableKind kind);
    virtual ~Variable() { }


    virtual std::vector<const collection::Variable *>
        *evaluate(Transaction *transaction);


    virtual void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l);


    virtual void evaluateInternal(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l);

    static std::string to_s(std::vector<Variable *> *variables);

    std::string m_name;
    std::string m_collectionName;

    VariableType m_type;
    VariableKind m_kind;
    bool m_isExclusion;
    bool m_isCount;
};


class VariableModificatorExclusion : public Variable {
 public:
    explicit VariableModificatorExclusion(std::unique_ptr<Variable> var)
        : Variable(var->m_name),
        m_var(std::move(var)) {
            m_isExclusion = true;
        }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        m_var->evaluate(transaction, rule, l);
    }

    std::unique_ptr<Variable> m_var;
};


class VariableModificatorCount : public Variable {
 public:
    explicit VariableModificatorCount(std::unique_ptr<Variable> var)
        : Variable(var->m_name),
        m_var(std::move(var)) {
            m_isCount = true;
        }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        std::vector<const collection::Variable *> reslIn;
        std::string *res = NULL;
        collection::Variable *val = NULL;
        int count = 0;

        m_var->evaluate(transaction, rule, &reslIn);
        for (const collection::Variable *a : reslIn) {
            count++;
            if (a->m_dynamic) {
                delete a;
                a = NULL;
            }
        }
        reslIn.clear();

        res = new std::string(std::to_string(count));

        val = new collection::Variable(&m_name, res);
        val->m_dynamic_value = true;
        val->m_dynamic = true;

        l->push_back(val);
        return;
    }

    std::unique_ptr<Variable> m_var;
};

}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_VARIABLE_H_
