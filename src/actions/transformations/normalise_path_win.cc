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


#include "src/actions/transformations/normalise_path_win.h"

#include <string.h>
#include <string>


#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"

#include "src/actions/transformations/normalise_path.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void NormalisePathWin::execute(const Transaction *t,
    const ModSecString &in,
    ModSecString &out) noexcept {
    int changed;

    out.assign(in);
    auto size = NormalisePath::normalize_path_inplace(
        reinterpret_cast<unsigned char *>(&out[0]), out.length(), 1, &changed);
    out.resize(size);
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
