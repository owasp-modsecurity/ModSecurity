/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "html_entity_decode.h"

#include <cstring>

#include "src/utils/string.h"

#ifdef WIN32
#include "src/compat/msvc.h"
#endif

using namespace modsecurity::utils::string;

namespace modsecurity::actions::transformations {


namespace {

struct NamedEntity {
    const char *name;
    std::size_t len;
    unsigned char ch;
};

/* Named-entity table for t:htmlEntityDecode.
 *
 * Compared case-insensitively against the full extracted token; the length
 * must match exactly. Prior implementations used strncasecmp() with the name
 * length only, which caused prefix collisions (e.g. "&ltest;" decoded to "<"
 * and dropped "est"). See GHSA-cxqf-vgrr-xxrv.
 *
 * The set is limited to entities that map into the ASCII range, since those
 * are what attackers use to evade rules expecting ASCII payload bytes.
 */
constexpr NamedEntity named_entities[] = {
    {"amp",    3, '&'},
    {"apos",   4, '\''},
    {"ast",    3, '*'},
    {"bsol",   4, '\\'},
    {"caret",  5, '^'},
    {"colon",  5, ':'},
    {"comma",  5, ','},
    {"commat", 6, '@'},
    {"dollar", 6, '$'},
    {"equals", 6, '='},
    {"grave",  5, '`'},
    {"gt",     2, '>'},
    {"hyphen", 6, '-'},
    {"lbrace", 6, '{'},
    {"lbrack", 6, '['},
    {"lowbar", 6, '_'},
    {"lpar",   4, '('},
    {"lt",     2, '<'},
    {"nbsp",   4, NBSP},
    {"num",    3, '#'},
    {"percnt", 6, '%'},
    {"period", 6, '.'},
    {"plus",   4, '+'},
    {"quest",  5, '?'},
    {"quot",   4, '"'},
    {"rbrace", 6, '}'},
    {"rbrack", 6, ']'},
    {"rpar",   4, ')'},
    {"semi",   4, ';'},
    {"sol",    3, '/'},
    {"tilde",  5, '~'},
    {"verbar", 6, '|'},
};

bool lookup_named_entity(const unsigned char *name, std::size_t name_len,
                         unsigned char *out) {
    for (const auto &e : named_entities) {
        if (e.len == name_len &&
            strncasecmp(reinterpret_cast<const char *>(name), e.name,
                        name_len) == 0) {
            *out = e.ch;
            return true;
        }
    }
    return false;
}

}  // namespace


static inline bool inplace(std::string &value) {
    const auto input_len = value.length();
    auto d = reinterpret_cast<unsigned char*>(value.data());
    const unsigned char *input = d;
    const unsigned char *end = input + input_len;

    std::string::size_type i = 0;
    while (i < input_len) {
        std::string::size_type copy = 1;

        /* Require an ampersand and at least one character to
         * start looking into the entity.
         */
        if ((input[i] == '&') && (i + 1 < input_len)) {
            auto j = i + 1;

            if (input[j] == '#') {
                /* Numerical entity. */
                copy++;

                if (!(j + 1 < input_len)) {
                    goto HTML_ENT_OUT; /* Not enough bytes. */
                }
                j++;

                if ((input[j] == 'x') || (input[j] == 'X')) {
                    /* Hexadecimal entity. */
                    copy++;

                    if (!(j + 1 < input_len)) {
                        goto HTML_ENT_OUT; /* Not enough bytes. */
                    }
                    j++; /* j is the position of the first digit now. */

                    auto k = j;
                    while ((j < input_len) && (isxdigit(input[j]))) {
                        j++;
                    }
                    if (j > k) { /* Do we have at least one digit? */
                        /* Decode the entity. */
                        char *x = new char[(j - k) + 1];
                        std::copy(input + k, input + j, x);
                        x[j - k] = '\0';

                        *d++ = (unsigned char)strtol(x, nullptr, 16);
                        delete[] x;

                        /* Skip over the semicolon if it's there. */
                        if ((j < input_len) && (input[j] == ';')) {
                            i = j + 1;
                        } else {
                            i = j;
                        }
                        continue;
                    } else {
                        goto HTML_ENT_OUT;
                    }
                } else {
                    /* Decimal entity. */
                    auto k = j;

                    while ((j < input_len) && (isdigit(input[j]))) {
                        j++;
                    }
                    if (j > k) { /* Do we have at least one digit? */
                        /* Decode the entity. */
                        char *x = new char[j - k + 1];
                        std::copy(input + k, input + j, x);

                        x[j - k] = '\0';
                        *d++ = (unsigned char)strtol(x, nullptr, 10);
                        delete[] x;

                        /* Skip over the semicolon if it's there. */
                        if ((j < input_len) && (input[j] == ';')) {
                            i = j + 1;
                        } else {
                            i = j;
                        }
                        continue;
                    } else {
                        goto HTML_ENT_OUT;
                    }
                }
            } else {
                /* Text entity. */
                auto k = j;
                while ((j < input_len) && (isalnum(input[j]))) {
                    j++;
                }
                if (j > k) { /* Do we have at least one character? */
                    unsigned char decoded = 0;
                    if (!lookup_named_entity(&input[k], j - k, &decoded)) {
                        /* Unknown entity: copy the raw data over. */
                        copy = j - k + 1;
                        goto HTML_ENT_OUT;
                    }
                    *d++ = decoded;

                    /* Skip over the semicolon if it's there. */
                    if ((j < input_len) && (input[j] == ';')) {
                        i = j + 1;
                    } else {
                        i = j;
                    }

                    continue;
                }
            }
        }

HTML_ENT_OUT:

        for (auto z = 0; z < copy; z++) {
            *d++ = input[i++];
        }
    }

    *d = '\0';

    value.resize(d - input);
    return d != end;
}


bool HtmlEntityDecode::transform(std::string &value, const Transaction *trans) const {
    return inplace(value);
}


}  // namespace modsecurity::actions::transformations
