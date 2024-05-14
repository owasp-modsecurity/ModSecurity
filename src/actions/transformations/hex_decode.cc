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

#include "hex_decode.h"

#include "src/utils/string.h"


namespace modsecurity::actions::transformations {


bool HexDecode::transform(std::string &value, const Transaction *trans) const {
    std::string ret;
    unsigned char *input;
    int size = 0;

    input = reinterpret_cast<unsigned char *>
        (malloc(sizeof(char) * value.length()+1));

    if (input == NULL) {
        return "";
    }

    memcpy(input, value.c_str(), value.length()+1);

    size = inplace(input, value.length());

    ret.assign(reinterpret_cast<char *>(input), size);
    free(input);

    const auto changed = ret != value;
    value = ret;
    return changed;
}


int HexDecode::inplace(unsigned char *data, int len) {
    unsigned char *d = data;
    int count = 0;

    if ((data == NULL) || (len == 0)) {
        return 0;
    }

    for (int i = 0;i <= len - 2;i += 2) {
        *d++ = utils::string::x2c(&data[i]);
        count++;
    }
    *d = '\0';

    return count;
}


}  // namespace modsecurity::actions::transformations
