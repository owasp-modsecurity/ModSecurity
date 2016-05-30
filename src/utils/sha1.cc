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


#include "src/utils/sha1.h"
#include "src/utils/mbedtls/sha1.h"

namespace modsecurity {
namespace Utils {


std::string Sha1::hexdigest(std::string& input) {
    unsigned char digest[20];

    mbedtls_sha1(reinterpret_cast<const unsigned char *>(input.c_str()),
        input.size(), digest);

    char buf[41];
    for (int i=0; i<20; i++) {
        sprintf(buf+i*2, "%02x", digest[i]);
    }
    buf[40] = 0;

    return std::string(buf, 40);
}


std::string Sha1::digest(std::string& input) {
    unsigned char output[20];
    std::string ret;

    mbedtls_sha1(reinterpret_cast<const unsigned char *>(input.c_str()),
        input.size(), output);

    ret.assign(reinterpret_cast<const char *>(output), 20);

    return ret;
}


}  // namespace Utils
}  // namespace modsecurity

