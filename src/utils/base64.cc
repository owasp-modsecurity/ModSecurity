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

#include <string>
#include <fstream>
#include <iostream>

#include "utils/base64.h"
#include "mbedtls/base64.h"
#include <string.h>

namespace modsecurity {
namespace Utils {


std::string Base64::encode(std::string& data) {
    size_t encoded_len = 0;
    unsigned char *d = NULL;
    std::string ret;

    mbedtls_base64_encode(NULL, 0, &encoded_len,
        reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

    d = reinterpret_cast<unsigned char*>(malloc(sizeof(char) * encoded_len));
    if (d == NULL) {
        return data;
    }

    memset(d, '\0', encoded_len);

    mbedtls_base64_encode(d, encoded_len, &encoded_len,
        (unsigned char*) data.c_str(), data.length());

    ret.assign(reinterpret_cast<const char*>(d));
    free(d);


    return ret;
}


std::string Base64::decode(std::string& data) {
    size_t decoded_len = 0;
    unsigned char *d = NULL;
    std::string ret;

    mbedtls_base64_decode(NULL, 0, &decoded_len,
        reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

    d = reinterpret_cast<unsigned char*>(malloc(sizeof(char) * decoded_len));
    if (d == NULL) {
        return data;
    }

    memset(d, '\0', decoded_len);

    mbedtls_base64_decode(d, decoded_len, &decoded_len,
       reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

    ret.assign(reinterpret_cast<const char*>(d));
    free(d);

    return ret;
}


}  // namespace Utils
}  // namespace modsecurity
