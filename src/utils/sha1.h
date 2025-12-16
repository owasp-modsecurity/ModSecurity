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
#include <mutex>      // NEW: std::once_flag, std::call_once

#include "src/utils/string.h"

// PSA statt mbedtls/sha1.h
#include <psa/crypto.h>

namespace modsecurity::Utils {

using DigestOp = int (*)(const unsigned char *, size_t, unsigned char []);

// Gemeinsamer, thread-sicherer PSA-Init für alle Digests
namespace detail {
inline bool ensure_psa_init() {
    static std::once_flag once;
    static psa_status_t init_status = PSA_ERROR_GENERIC_ERROR;

    std::call_once(once, []() {
        init_status = psa_crypto_init();
    });

    return init_status == PSA_SUCCESS;
}
}  // namespace detail


template<DigestOp digestOp, int DigestSize>
class DigestImpl {
 public:
    static std::string digest(const std::string& input) {
        return digestHelper(input, [](std::string_view digest) {
            return std::string(digest);
        });
    }

    static void digestReplace(std::string& value) {
        digestHelper(value, [&value](std::string_view digest) mutable {
            value.assign(digest.data(), digest.size());
        });
    }

    static std::string hexdigest(const std::string &input) {
        return digestHelper(input, [](std::string_view digest) {
            return utils::string::string_to_hex(digest);
        });
    }

 private:
    template<typename ConvertOp>
    static auto digestHelper(const std::string &input, ConvertOp convertOp)
        -> decltype(convertOp(std::string_view{})) {

        unsigned char digest[DigestSize];

        const int ret = (*digestOp)(
            reinterpret_cast<const unsigned char *>(input.data()),
            input.size(),
            digest
        );

        // NEW: kein assert-only; in Release sonst potentiell UB.
        if (ret != 0) {
            return convertOp(std::string_view{}); // leerer Digest signalisiert Fehler
        }

        return convertOp(std::string_view(
            reinterpret_cast<const char*>(digest), DigestSize
        ));
    }
};


// PSA-Wrapper mit alter Signatur
inline int modsec_psa_sha1(const unsigned char *input,
                           size_t ilen,
                           unsigned char output[20])
{
    if (!detail::ensure_psa_init()) {
        return -1;
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

    return (status == PSA_SUCCESS && out_len == 20) ? 0 : -1;
}

class Sha1 : public DigestImpl<&modsec_psa_sha1, 20> {};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_SHA1_H_