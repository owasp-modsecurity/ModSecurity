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

#include "operators/gsblookup.h"

#include <string>

#include "operators/operator.h"

namespace modsecurity {
namespace operators {

bool GsbLookup::evaluate(Assay *assay) {
    /**
     * @todo Implement the operator GeoLookup.
     *       Reference: https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#gsblookup
     */
    return true;
}


GsbLookup::GsbLookup(std::string op, std::string param,
    bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace ModSecurity
