/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "actions/transformations/url_decode.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/assay.h"
#include "actions/transformations/transformation.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {
namespace transformations {


int UrlDecode::urldecode_nonstrict_inplace(unsigned char *input,
    uint64_t input_len, int *invalid_count, int *changed) {
    unsigned char *d = (unsigned char *)input;
    uint64_t i, count;

    *changed = 0;

    if (input == NULL) {
        return -1;
    }

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            /* Character is a percent sign. */

            /* Are there enough bytes available? */
            if (i + 2 < input_len) {
                char c1 = input[i + 1];
                char c2 = input[i + 2];
                if (VALID_HEX(c1) && VALID_HEX(c2)) {
                    uint64_t uni = x2c(&input[i + 1]);

                    *d++ = (wchar_t)uni;
                    count++;
                    i += 3;
                    *changed = 1;
                } else {
                    /* Not a valid encoding, skip this % */
                    *d++ = input[i++];
                    count++;
                    (*invalid_count)++;
                }
            } else {
                /* Not enough bytes available, copy the raw bytes. */
                *d++ = input[i++];
                count++;
                (*invalid_count)++;
            }
        } else {
            /* Character is not a percent sign. */
            if (input[i] == '+') {
                *d++ = ' ';
                *changed = 1;
            } else {
                *d++ = input[i];
            }
            count++;
            i++;
        }
    }

    *d = '\0';

    return count;
}


UrlDecode::UrlDecode(std::string action)
    : Transformation(action) {
    this->action_kind = 1;
}

std::string UrlDecode::evaluate(std::string value,
    Assay *assay) {
    unsigned char *val = NULL;
    int invalid_count;
    int changed;

    if (UrlDecodeInstantCache::getInstance().count(value) > 0) {
        return UrlDecodeInstantCache::getInstance().at(value);
    }

    val = (unsigned char *) malloc(sizeof(char) * value.size() + 1);
    memcpy(val, value.c_str(), value.size() + 1);
    val[value.size()] = '\0';

    int size = urldecode_nonstrict_inplace(val, value.size(),
        &invalid_count, &changed);
    std::string out;

    out.append((const char *)val, size);

    free(val);

    UrlDecodeInstantCache::getInstance().cache(value, out);

    return out;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
