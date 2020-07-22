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


#include "src/actions/transformations/hex_decode.h"

#include <string>

#include "modsecurity/transaction.h"

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"

#include "src/utils/string.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void HexDecode::execute(const Transaction *t,
    const ModSecString &in,
    ModSecString &out) noexcept {
    out.assign(in);
    auto size = inplace(reinterpret_cast<unsigned char *>(
        &out[0]), out.length());
    out.resize(size);
}


int HexDecode::inplace(unsigned char *data, int len) {
    unsigned char *d = data;
    int i, count = 0;

    if ((data == NULL) || (len == 0)) {
        return 0;
    }

    for (i = 0; i <= len - 2; i += 2) {
        *d++ = utils::string::x2c(&data[i]);
        count++;
    }
    *d = '\0';

    return count;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
