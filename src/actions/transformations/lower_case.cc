/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include "lower_case.h"

#include <locale>


namespace modsecurity::actions::transformations {


LowerCase::LowerCase(const std::string &a)
    : Transformation(a) {
}

bool LowerCase::transform(std::string &val, const Transaction *trans) const {
    std::locale loc;
    std::string value(val);

    for (std::string::size_type i=0; i < value.length(); ++i) {
        value[i] = std::tolower(value[i], loc);
    }

    const auto changed = val != value;
    val = value;
    return changed;
}


}  // namespace modsecurity::actions::transformations
