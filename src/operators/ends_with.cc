/**
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

#include "operators/ends_with.h"

#include <string>

#include "operators/operator.h"

namespace ModSecurity {
namespace operators {

bool EndsWith::evaluate(Assay *assay,
    std::string input) {
    // Check that the param is not longer than the input
    if (this->param.length() > input.length()) {
        return 0;
    }

    // Check that the input != the param
    if (this->param == input) {
        return 1;
    }

    // Start at the end of the input minus the size of the input
    std::string endString = input.substr(input.length() - \
        (this->param.length()), (this->param.length()));

    // FIXME: We can make this smalle
    if (endString == this->param) {
        return 1;
    }

    return 0;
}


EndsWith::EndsWith(std::string op, std::string param,
    bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace ModSecurity
