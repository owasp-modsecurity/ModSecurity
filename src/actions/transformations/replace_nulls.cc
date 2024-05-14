/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2023 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "replace_nulls.h"


namespace modsecurity::actions::transformations {

ReplaceNulls::ReplaceNulls(const std::string &action) 
    : Transformation(action) {
    this->action_kind = 1;
}

bool ReplaceNulls::transform(std::string &val, const Transaction *trans) const {
    int64_t i;
    std::string value(val);

    i = 0;
    while (i < value.size()) {
        if (value.at(i) == '\0') {
            value[i] = ' ';
        } else {
            i++;
        }
    }

    const auto changed = val != value;
    val = value;
    return changed;
}

}  // namespace modsecurity::actions::transformations
