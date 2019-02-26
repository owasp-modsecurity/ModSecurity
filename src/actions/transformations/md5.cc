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

#include "src/actions/transformations/md5.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "src/utils/md5.h"

namespace modsecurity {
namespace actions {
namespace transformations {


void Md5::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    std::string ret = Utils::Md5::digest(std::string(in.c_str(), in.size()));

    out.assign(ret.c_str(), ret.size());
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
