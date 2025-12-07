/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc.
 *
 * Licensed under the Apache License, Version 2.0
 */

#ifndef SRC_UTILS_MD5_H_
#define SRC_UTILS_MD5_H_

#include "src/utils/sha1.h"   // bringt DigestImpl und psa/crypto.h rein
#include <string>

namespace modsecurity::Utils {

// Wrapper mit gleicher Signatur wie mbedtls_md5,
// intern aber PSA-API.
inline int modsec_psa_md5(const unsigned char *input,
                          size_t ilen,
                          unsigned char output[16])
{
    // sha1.h macht bereits ein lazy psa_crypto_init() in modsec_psa_sha1,
    // aber falls MD5 vor SHA1 benutzt wird, sorgen wir hier auch nochmal vor.
    static bool psa_initialized = false;

    if (!psa_initialized) {
        psa_status_t init_status = psa_crypto_init();
        if (init_status != PSA_SUCCESS) {
            return -1;
        }
        psa_initialized = true;
    }

    size_t out_len = 0;
    psa_status_t status = psa_hash_compute(
        PSA_ALG_MD5,
        input,
        ilen,
        output,
        16,
        &out_len
    );

    if (status != PSA_SUCCESS || out_len != 16) {
        return -1;
    }

    return 0;
}

// Statt &mbedtls_md5 benutzen wir jetzt &modsec_psa_md5.
class Md5 : public DigestImpl<&modsec_psa_md5, 16> {
};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_MD5_H_
