/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc.
 *
 * Licensed under the Apache License, Version 2.0
 */

#ifndef SRC_UTILS_SHA1_H_
#define SRC_UTILS_SHA1_H_

#include <string>
#include <string_view>
#include <cassert>

#include "src/utils/string.h"

// NEU: PSA statt mbedtls/sha1.h
#include <psa/crypto.h>

namespace modsecurity::Utils {

using DigestOp = int (*)(const unsigned char *, size_t, unsigned char []);


template<DigestOp digestOp, int DigestSize>
class DigestImpl {
 public:
    static std::string digest(const std::string& input) {
        return digestHelper(input, [](const auto digest) {
            return std::string(digest);
        });
    }

    static void digestReplace(std::string& value) {
        digestHelper(value, [&value](const auto digest) mutable {
            value = digest;
        });
    }

    static std::string hexdigest(const std::string &input) {
        return digestHelper(input, [](const auto digest) {
            return utils::string::string_to_hex(digest);
        });
    }

 private:
    template<typename ConvertOp>
    static auto digestHelper(const std::string &input,
                             ConvertOp convertOp) -> auto {
        char digest[DigestSize];

        const auto ret = (*digestOp)(
            reinterpret_cast<const unsigned char *>(input.c_str()),
            input.size(),
            reinterpret_cast<unsigned char *>(digest)
        );
        assert(ret == 0);

        return convertOp(std::string_view(digest, DigestSize));
    }
};

// NEU: Wrapper, der die PSA-API in die alte Signatur presst.
inline int modsec_psa_sha1(const unsigned char *input,
                           size_t ilen,
                           unsigned char output[20])
{
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
        PSA_ALG_SHA_1,
        input,
        ilen,
        output,
        20,
        &out_len
    );

    if (status != PSA_SUCCESS || out_len != 20) {
        return -1;
    }

    return 0;
}

// Statt &mbedtls_sha1 nehmen wir jetzt unseren PSA-Wrapper
class Sha1 : public DigestImpl<&modsec_psa_sha1, 20> {
};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_SHA1_H_
