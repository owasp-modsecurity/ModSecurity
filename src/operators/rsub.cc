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

#include "operators/rsub.h"

#include <string>

#include "operators/operator.h"

namespace modsecurity {
namespace operators {

bool Rsub::evaluate(Transaction *transaction, const std::string &str) {
    /**
     * @todo Implement the operator Rsub.
     *       Reference: https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#rsub
     */
    return true;
}


Rsub::Rsub(std::string op, std::string param, bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace modsecurity
