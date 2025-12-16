#ifndef SRC_UTILS_SHA256_H_
#define SRC_UTILS_SHA256_H_

#include "src/utils/sha1.h"   // bringt DigestImpl + detail::ensure_psa_init()
#include <psa/crypto.h>

namespace modsecurity::Utils {

inline int modsec_psa_sha256(const unsigned char *input,
                             size_t ilen,
                             unsigned char output[32])
{
    if (!detail::ensure_psa_init()) {
        return -1;
    }

    size_t out_len = 0;
    psa_status_t status = psa_hash_compute(
        PSA_ALG_SHA_256,
        input,
        ilen,
        output,
        32,
        &out_len
    );

    return (status == PSA_SUCCESS && out_len == 32) ? 0 : -1;
}

class Sha256 : public DigestImpl<&modsec_psa_sha256, 32> {};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_SHA256_H_