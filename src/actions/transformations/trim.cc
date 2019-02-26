/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/actions/transformations/trim.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "modsecurity/actions/action.h"

namespace modsecurity {
namespace actions {
namespace transformations {


void Trim::ltrim(ModSecStackString *s) {
    s->erase(s->begin(), std::find_if(s->begin(), s->end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
}


void Trim::rtrim(ModSecStackString *s) {
    s->erase(std::find_if(s->rbegin(), s->rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s->end());
}


void Trim::trim(ModSecStackString *s) {
    rtrim(s);
    ltrim(s);
}


void Trim::execute(Transaction *t,
    ModSecStackString &in,
    ModSecStackString &out) {
    out = in;
    trim(&out);
};


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
