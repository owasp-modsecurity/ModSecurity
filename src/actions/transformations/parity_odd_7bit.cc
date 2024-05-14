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

#include "parity_odd_7bit.h"

#include <cstring>


namespace modsecurity::actions::transformations {


bool ParityOdd7bit::transform(std::string &value, const Transaction *trans) const {
    if (value.empty()) return false;

    unsigned char *input;

    input = reinterpret_cast<unsigned char *>
        (malloc(sizeof(char) * value.length()+1));

    if (input == NULL) {
        return "";
    }

    memcpy(input, value.c_str(), value.length()+1);

    const auto ret = inplace(input, value.length());

    value.assign(reinterpret_cast<char *>(input), value.length());
    free(input);

    return ret;
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


}  // namespace modsecurity::actions::transformations
