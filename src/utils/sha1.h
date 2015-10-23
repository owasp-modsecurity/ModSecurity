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

/** TODO: Reimplement this on the same terms and/or check if we use it. */
/*
    sha1.h - header of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Grabsch <vog@notjusthosting.com>
*/

#ifndef SRC_UTILS_SHA1_H_
#define SRC_UTILS_SHA1_H_

#include <algorithm>
#include <iostream>
#include <string>

namespace ModSecurity {
namespace Utils {

class SHA1 {
 public:
    SHA1();
    void update(std::string *s);
    void update(std::istream *is);
    std::string final();
    std::string final_bin() {
        return final_bin(true);
    }
    std::string final_bin(bool reset);

 private:
    /* number of 32bit integers per SHA1 digest */
    static const unsigned int DIGEST_INTS = 5;
    /* number of 32bit integers per SHA1 block */
    static const unsigned int BLOCK_INTS = 16;
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    uint32_t digest[DIGEST_INTS];
    std::string buffer;
    uint64_t transforms;

    void reset();
    void transform(uint32_t block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string &buffer,
        uint32_t block[BLOCK_BYTES]);

    void read(std::istream *is, std::string *s, int max);
};

}  // namespace Utils
}  // namespace ModSecurity

#endif  // SRC_UTILS_SHA1_H_
