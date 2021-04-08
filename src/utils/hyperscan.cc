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
#include "hyperscan.h"

namespace modsecurity {
namespace Utils {

// Render the given literal as a hex-escaped pattern.
static
std::string makeHex(const char *pat, const size_t patLen) {
    std::string hexPattern;

    char hex[5];
    for (size_t i = 0; i < patLen; i++) {
        snprintf(hex, 5, "\\x%02x", (unsigned char)pat[i]);
        hexPattern += hex;
    }
    return hexPattern;
}

HyperscanPattern::HyperscanPattern(const char *pat, size_t patLen,
                                   unsigned int patId) :
                                   pattern(pat), len(patLen), id(patId) {}

HyperscanPm::~HyperscanPm() {
    if (db) {
        hs_free_database(db);
    }
    if (scratch) {
        hs_free_scratch(scratch);
    }
}

void HyperscanPm::addPattern(const char *pat, size_t patLen) {
    if (patLen == 0) {
        return;
    }

    HyperscanPattern p(pat, patLen, num_patterns++);
    patterns.emplace_back(p);
}

bool HyperscanPm::compile(std::string *error) {
    if (patterns.empty()) {
        return false;
    }

    if (hs_valid_platform() != HS_SUCCESS )
    {
        error->assign("This host does not support Hyperscan.");
        return false;
    }

    // Convert literal to its hex-escaped format.
    std::vector<std::string> hexPats;
    for (const auto &p : patterns) {
        hexPats.emplace_back(makeHex(p.pattern.c_str(), p.len));
    }

    // The Hyperscan compiler takes its patterns in a group of arrays.
    std::vector<const char *> pats;
    std::vector<unsigned> flags(num_patterns, HS_FLAG_CASELESS);
    std::vector<unsigned> ids;

    int i = 0;
    for (const auto &p : patterns) {
        pats.emplace_back(hexPats[i++].c_str());
        ids.emplace_back(p.id);
    }

    hs_compile_error_t *compile_error = NULL;
    hs_error_t hs_error = hs_compile_multi(&pats[0], &flags[0], &ids[0],
        num_patterns, HS_MODE_BLOCK, NULL, &db, &compile_error);

    if (compile_error != NULL) {
        std::string message(compile_error->message);
        std::string expression = std::to_string(compile_error->expression);
        error->assign("hs_compile_multi() failed: " + message +
                      "(expression: " + expression + ")");
        hs_free_compile_error(compile_error);
        return false;
    }

    if (hs_error != HS_SUCCESS) {
        error->assign("hs_compile_multi() failed: error " +
                      std::to_string(hs_error));
        return false;
    }

    // Allocate Hyperscan scratch space for this database.
    hs_error = hs_alloc_scratch(db, &scratch);

    if (hs_error != HS_SUCCESS) {
        error->assign("hs_alloc_scratch() failed: error " +
                      std::to_string(hs_error));
        return false;
    }

    size_t scratch_size = 0;
    hs_error = hs_scratch_size(scratch, &scratch_size);
    if (hs_error != HS_SUCCESS) {
        error->assign("hs_scratch_size() failed: error " +
                      std::to_string(hs_error));
        return false;
    }

    size_t db_size = 0;
    hs_error = hs_database_size(db, &db_size);
    if (hs_error != HS_SUCCESS) {
        error->assign("hs_database_size() failed: error " +
                      std::to_string(hs_error));
        return false;
    }

    return true;
}

// Context data used by Hyperscan match callback.
struct HyperscanCallbackContext {
    HyperscanPm *pm;
    unsigned int num_matches;
    unsigned int offset;
    const char **match;
};

// Match callback, called by hs_scan for every match.
static
int onMatch(unsigned int id, unsigned long long from, unsigned long long to,
            unsigned int flags, void *hs_ctx) {
    HyperscanCallbackContext *ctx = static_cast<HyperscanCallbackContext *>(hs_ctx);

    ctx->num_matches++;
    ctx->offset = (unsigned int)to - 1;
    *ctx->match = ctx->pm->getPatternById(id);
    return 1; // Terminate matching.
}

int HyperscanPm::search(const char *t, unsigned int tlen, const char **match) {
    HyperscanCallbackContext ctx;
    ctx.pm = this;
    ctx.num_matches = 0;
    ctx.offset = 0;
    ctx.match = match;

    hs_error_t error = hs_scan(db, t, tlen, 0, scratch, onMatch, &ctx);

    if (error != HS_SCAN_TERMINATED) {
        // TODO add debug output
        return -1;
    }

    return ctx.num_matches > 0 ? ctx.offset : -1;
}

const char *HyperscanPm::getPatternById(unsigned int patId) const {
    return patterns[patId].pattern.c_str();
}

}  // namespace Utils
}  // namespace modsecurity

#endif
