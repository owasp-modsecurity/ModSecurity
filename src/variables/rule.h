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

#include <string>
#include <vector>
#include <list>

#ifndef SRC_VARIABLES_RULE_H_
#define SRC_VARIABLES_RULE_H_

#include "variables/variable.h"

namespace modsecurity {

class Transaction;
namespace Variables {

class Rule : public Variable {
 public:
    explicit Rule(std::string _name)
        : Variable(_name) { };

    void evaluateInternal(Transaction *transaction,
        modsecurity::Rule *rule,
        std::vector<const collection::Variable *> *l) override;
};

}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_RULE_H_
