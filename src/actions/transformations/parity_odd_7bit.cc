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

#include "src/actions/transformations/parity_odd_7bit.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cstring>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void ParityOdd7bit::execute(Transaction *t,
    ModSecString &in,
    ModSecString &out) {
    unsigned char *input;

    input = reinterpret_cast<unsigned char *>
        (malloc(sizeof(char) * in.length()+1));

    if (input == NULL) {
        return;
    }

    memcpy(input, in.c_str(), in.length()+1);

    inplace(input, in.length());

    out.assign(reinterpret_cast<char *>(input), in.length());
    free(input);
}


bool ParityOdd7bit::inplace(unsigned char *input, uint64_t input_len) {
    uint64_t i;

    i = 0;
    while (i < input_len) {
        unsigned int x = input[i];

        input[i] ^= input[i] >> 4;
        input[i] &= 0xf;

        if ((0x6996 >> input[i]) & 1) {
            input[i] = x & 0x7f;
        } else {
            input[i] = x | 0x80;
        }
        i++;
    }

    return true;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
