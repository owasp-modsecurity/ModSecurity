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


#include "src/actions/transformations/url_encode.h"

#include <string>


#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"

#include "src/utils/string.h"


namespace modsecurity {
namespace actions {
namespace transformations {


std::string UrlEncode::url_enc(const char *input,
    unsigned int input_len, int *changed) {
    char *rval, *d;
    unsigned int i, len;
    int count = 0;

    *changed = 0;

    len = input_len * 3 + 1;
    d = rval = reinterpret_cast<char *>(malloc(len));
    if (rval == NULL) {
        return NULL;
    }

    /* ENH Only encode the characters that really need to be encoded. */

    for (i = 0; i < input_len; i++) {
        unsigned char c = input[i];

        if (c == ' ') {
            *d++ = '+';
            *changed = 1;
            count++;
        } else {
            if ( (c == 42) || ((c >= 48) && (c <= 57))
                || ((c >= 65) && (c <= 90))
                || ((c >= 97) && (c <= 122))) {
                *d++ = c;
                count++;
            } else {
                *d++ = '%';
                count++;
                utils::string::c2x(c, (unsigned char *)d);
                d += 2;
                count++;
                count++;
                *changed = 1;
            }
        }
    }

    *d = '\0';

    std::string ret("");
    ret.append(rval, count);
    free(rval);
    return ret;
}


void UrlEncode::execute(const Transaction *t,
    const ModSecString &in,
    ModSecString &out) noexcept {
    int changed;

    std::string ret = url_enc(in.c_str(), in.size(), &changed);
    out.assign(ret.c_str(), ret.size());
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
