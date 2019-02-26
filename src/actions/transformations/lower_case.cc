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

#include "src/actions/transformations/lower_case.h"

#include <algorithm>
#include <string>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "modsecurity/actions/action.h"

namespace modsecurity {
namespace actions {
namespace transformations {


void LowerCase::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    std::locale loc;
    out.resize(in.size());
    for (std::string::size_type i=0; i < in.size(); ++i) {
        out[i] = std::tolower(in[i], loc);
    }
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
