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

#include "src/actions/transformations/remove_nulls.h"

#include <string.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void RemoveNulls::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    int64_t i;
    out = in;

    i = 0;
    while (i < out.size()) {
        if (out.at(i) == '\0') {
            out.erase(i, 1);
        } else {
            i++;
        }
    }
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
