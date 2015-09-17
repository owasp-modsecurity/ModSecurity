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

#include "actions/transformations/utf8_to_unicode.h"

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

Utf8Unicode::Utf8Unicode(std::string action)
    : Transformation(action) {
    this->action_kind = 1;
}

std::string Utf8Unicode::evaluate(std::string value,
    Assay *assay) {
    /**
     * @todo Implement the transformation Utf8Unicode
     */
    if (assay) {
#ifndef NO_LOGS
        assay->debug(4, "Transformation Utf8Unicode is not implemented yet.");
#endif
    }
    return value;
}

}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
