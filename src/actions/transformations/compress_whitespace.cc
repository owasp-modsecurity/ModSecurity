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

#include "compress_whitespace.h"


namespace modsecurity::actions::transformations {


CompressWhitespace::CompressWhitespace(const std::string &action) 
    : Transformation(action) {
    this->action_kind = 1;
}

bool CompressWhitespace::transform(std::string &value, const Transaction *trans) const {

    std::string a;
    int inWhiteSpace = 0;
    int i = 0;

    while (i < value.size()) {
        if (isspace(value[i])) {
            if (inWhiteSpace) {
                i++;
                continue;
            } else {
                inWhiteSpace = 1;
                a.append(" ", 1);
            }
        } else {
            inWhiteSpace = 0;
            a.append(&value.at(i), 1);
        }
        i++;
    }

    const auto changed = a != value;
    value = a;
    return changed;
}


}  // namespace modsecurity::actions::transformations
