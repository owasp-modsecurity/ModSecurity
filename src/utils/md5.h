/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc.
 *
 * Licensed under the Apache License, Version 2.0
 */

#ifndef SRC_UTILS_MD5_H_
#define SRC_UTILS_MD5_H_

#include "src/utils/sha1.h"   // nutzt DigestImpl + detail::ensure_psa_init()
#include <string>

#include <psa/crypto.h>       // optional (weil sha1.h es schon inkludiert), aber ok

namespace modsecurity::Utils {

// PSA-Wrapper mit alter Signatur
inline int modsec_psa_md5(const unsigned char *input,
                          size_t ilen,
                          unsigned char output[16])
{
    if (!detail::ensure_psa_init()) {
        return -1;
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

    return (status == PSA_SUCCESS && out_len == 16) ? 0 : -1;
}

class Md5 : public DigestImpl<&modsec_psa_md5, 16> {};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_MD5_H_