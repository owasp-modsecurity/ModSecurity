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

#include "src/actions/transformations/base64_decode_ext.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "src/utils/base64.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void Base64DecodeExt::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    std::string ret = Utils::Base64::decode_forgiven(in.c_str());
    out.assign(ret.c_str(), ret.size());
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
