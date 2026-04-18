/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#ifndef SRC_UTILS_SHA1_H_
#define SRC_UTILS_SHA1_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <exception>
#include <string>
#include <string_view>

#include "src/utils/string.h"
#include "mbedtls/md.h"

namespace modsecurity::Utils {

class DigestCalculationException : public std::exception {
 public:
    explicit DigestCalculationException(const char *message)
        : m_message(message) { }

    const char *what() const noexcept override {
        return m_message.c_str();
    }

 private:
    std::string m_message;
};


template<mbedtls_md_type_t DigestType, int DigestSize>
class DigestImpl {
 public:
    static std::string digest(const std::string& input) {
        try {
            const auto digestBytes = calculateDigest(input);
            return std::string(digestBytes.begin(), digestBytes.end());
        } catch (const DigestCalculationException&) {
            assert(false);
            return std::string(DigestSize, '\0');
        }
    }

    static void digestReplace(std::string& value) {
        try {
            const auto digestBytes = calculateDigest(value);
            value.assign(digestBytes.begin(), digestBytes.end());
        } catch (const DigestCalculationException&) {
            assert(false);
            value.assign(DigestSize, '\0');
        }
    }

    static std::string hexdigest(const std::string& input) {
        try {
            const auto digestBytes = calculateDigest(input);
            const auto *digestByteData =
                static_cast<const std::byte *>(static_cast<const void *>(digestBytes.data()));
            return utils::string::string_to_hex(
                digestByteData, digestBytes.size());
        } catch (const DigestCalculationException&) {
            assert(false);
            const std::array<unsigned char, DigestSize> digestBytes = {};
            const auto *digestByteData =
                static_cast<const std::byte *>(static_cast<const void *>(digestBytes.data()));
            return utils::string::string_to_hex(
                digestByteData, digestBytes.size());
        }
    }

 private:
    static std::array<unsigned char, DigestSize> calculateDigest(
        std::string_view input) {
        std::array<unsigned char, DigestSize> digestBytes = {};

        const mbedtls_md_info_t *mdInfo = mbedtls_md_info_from_type(DigestType);
        if (mdInfo == nullptr) {
            throw DigestCalculationException(
                "mbedtls_md_info_from_type() returned nullptr");
        }

        const auto *inputBytes =
            static_cast<const unsigned char *>(static_cast<const void *>(input.data()));

        if (const int ret = mbedtls_md(
                mdInfo,
                inputBytes,
                input.size(),
                digestBytes.data()); ret != 0) {
            throw DigestCalculationException("mbedtls_md() failed");
        }

        return digestBytes;
    }
};


class Sha1 : public DigestImpl<MBEDTLS_MD_SHA1, 20> {
};

}  // namespace modsecurity::Utils

#endif  // SRC_UTILS_SHA1_H_
