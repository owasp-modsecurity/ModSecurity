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
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"

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

    static std::string to_s(std::vector<Variable *> *variables);


    virtual std::vector<const collection::Variable *>
        *evaluate(Transaction *transaction);


    virtual void evaluateInternal(Transaction *transaction,
        std::vector<const collection::Variable *> *l);

    virtual void evaluateInternal(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        evaluateInternal(transaction, l);
    }


    virtual void evaluate(Transaction *transaction,
        std::vector<const collection::Variable *> *l);

    virtual void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        evaluate(transaction, l);
    }


    std::string m_name;
    std::string m_collectionName;

    VariableType m_type;
    VariableKind m_kind;
    bool m_isExclusion;
    bool m_isCount;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_VARIABLE_H_
