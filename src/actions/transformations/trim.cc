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

#include <functional>
#include <string>

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void Trim::ltrim(ModSecString *s) {
    s->erase(s->begin(), std::find_if(s->begin(), s->end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
}


void Trim::rtrim(ModSecString *s) {
    s->erase(std::find_if(s->rbegin(), s->rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s->end());
}


void Trim::trim(ModSecString *s) {
    rtrim(s);
    ltrim(s);
}


void Trim::execute(const Transaction *t,
    const ModSecString &in,
    ModSecString &out) noexcept {
    out = in;
    trim(&out);
};


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
