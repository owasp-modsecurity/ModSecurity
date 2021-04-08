/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <vector>

#ifdef WITH_HS
#include <hs.h>

namespace modsecurity {
namespace Utils {

struct HyperscanPattern {
    HyperscanPattern(const char *pat, size_t patLen, unsigned int patId);

    std::string pattern;
    size_t len;
    unsigned int id; /* actual pattern id */
};

class HyperscanPm {
 public:
    ~HyperscanPm();

    void addPattern(const char *pat, size_t patLen);

    bool compile(std::string *error);

    int search(const char *t, unsigned int tlen, const char **match);

    const char *getPatternById(unsigned int patId) const;

 private:
    hs_database_t *db = nullptr;
    // Scratch space for Hyperscan.
    hs_scratch_t *scratch = nullptr;
    unsigned int num_patterns = 0; // number of elements
    std::vector<HyperscanPattern> patterns;
};

}  // namespace Utils
}  // namespace modsecurity

#endif // WITH_HS
