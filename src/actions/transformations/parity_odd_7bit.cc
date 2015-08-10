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

#include "actions/transformations/parity_odd_7bit.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/assay.h"
#include "actions/transformations/transformation.h"


namespace ModSecurity {
namespace actions {
namespace transformations {

ParityOdd7bit::ParityOdd7bit(std::string action)
    : Transformation(action) {
    this->action_kind = 1;
}

std::string ParityOdd7bit::evaluate(std::string value,
    Assay *assay) {
    /**
     * @todo Implement the transformation ParityOdd7bit
     */
    if (assay) {
        assay->debug(4, "Transformation ParityOdd7bit is not " \
            "implemented yet.");
    }
    return value;
}

}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
