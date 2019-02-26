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

#include "src/actions/transformations/hex_encode.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <iterator>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void HexEncode::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    std::stringstream result;
    for (std::size_t i=0; i < in.length(); i++) {
        int ii = reinterpret_cast<char>(in[i]);
        result << std::setw(2) << std::setfill('0') << std::hex << ii;
    }

    out.assign(result.str().c_str());
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
