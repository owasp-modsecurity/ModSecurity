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

#include "src/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <wordexp.h>


#include <stdint.h>
#include <inttypes.h>


#include <random>
#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>

#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "modsecurity/modsecurity.h"

namespace modsecurity {

std::string phase_name(int x) {
    switch (x) {
        case ModSecurity::Phases::ConnectionPhase:
            return "Connection Phase";
            break;
        case ModSecurity::Phases::UriPhase:
            return "URI Phase";
            break;
        case ModSecurity::Phases::RequestHeadersPhase:
            return "Request Headers";
            break;
        case ModSecurity::Phases::RequestBodyPhase:
            return "Request Headers";
            break;
        case ModSecurity::Phases::ResponseHeadersPhase:
            return "Response Headers";
            break;
        case ModSecurity::Phases::ResponseBodyPhase:
            return "Reponse Body";
            break;
        case ModSecurity::Phases::LoggingPhase:
            return "Logging";
            break;
    }
    return "Phase '" + std::to_string(x) + "' is not known.";
}


std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);  // Turn the string into a stream.
    std::string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


double random_number(const double from, const double to) {
    std::mt19937 mt(time(0));
    return std::bind(
        std::uniform_real_distribution<>{from, to},
        std::default_random_engine{ mt() })();
}


std::string dash_if_empty(const std::string& str) {
    if (&str == NULL || str.empty()) {
        return "-";
    }

    return str;
}


double generate_assay_unique_id() {
    return random_number(0, 100);
}


std::string ascTime(time_t *t) {
    std::string ts = std::ctime(t);
    ts.pop_back();
    return ts;
}


void chomp(std::string *str) {
    std::string::size_type pos = str->find_last_not_of("\n\t");
    if (pos != std::string::npos) {
        str->erase(pos+1, str->length()-pos-1);
    }
}


std::string tolower(std::string str) {
    std::locale loc;
    std::string value;

    for (std::string::size_type i=0; i < str.length(); ++i) {
        value.assign(value + std::tolower(str[i], loc));
    }

    return value;
}

std::string toupper(std::string str) {
    std::locale loc;
    std::string value;

    for (std::string::size_type i=0; i < str.length(); ++i) {
        value.assign(value + std::toupper(str[i], loc));
    }

    return value;
}

const char SAFE[256] = {
    /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
    /* 0 */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* 1 */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* 2 */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* 3 */ 1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 0, 0,  0, 0, 0, 0,

    /* 4 */ 0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
    /* 5 */ 1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 0, 0,
    /* 6 */ 0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
    /* 7 */ 1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 0, 0,

    /* 8 */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* 9 */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* A */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* B */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,

    /* C */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* D */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* E */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    /* F */ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0
};


const char HEX2DEC[256] = {
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 1 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 2 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 3 */  0,  1,  2,  3,   4,  5,  6,  7,   8,  9, -1, -1,  -1, -1, -1, -1,

    /* 4 */ -1, 10, 11, 12,  13, 14, 15, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 5 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 6 */ -1, 10, 11, 12,  13, 14, 15, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 7 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,

    /* 8 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* 9 */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* A */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* B */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,

    /* C */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* D */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* E */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
    /* F */ -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1
};


std::string uri_decode(const std::string & sSrc) {
    // Note from RFC1630: "Sequences which start with a percent
    // sign but are not followed by two hexadecimal characters
    // (0-9, A-F) are reserved for future extension"

    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const int SRC_LEN = sSrc.length();
    const unsigned char * const SRC_END = pSrc + SRC_LEN;
    // last decodable '%'
    const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

    char * const pStart = new char[SRC_LEN];
    char * pEnd = pStart;

    while (pSrc < SRC_LAST_DEC) {
        if (*pSrc == '%') {
            char dec1, dec2;
            if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
            }
        }
        *pEnd++ = *pSrc++;
    }

    // the last 2- chars
    while (pSrc < SRC_END) {
        *pEnd++ = *pSrc++;
    }

    std::string sResult(pStart, pEnd);
    delete [] pStart;
    return sResult;
}


void createDir(std::string dir, int mode) {
#if defined _MSC_VER
    _mkdir(dir.data());
#elif defined __GNUC__
    mkdir(dir.data(), mode);
#endif
}


double cpu_seconds(void) {
        struct timespec t;

        if (!clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t))
                return static_cast<double>(t.tv_sec)
                     + static_cast<double>(t.tv_nsec / 1000000000.0);
        else
                return static_cast<double>(clock()) /
                    static_cast<double>(CLOCKS_PER_SEC);
}


int js_decode_nonstrict_inplace(unsigned char *input, int64_t input_len) {
    unsigned char *d = (unsigned char *)input;
    int64_t i, count;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '\\') {
            /* Character is an escape. */

            if ((i + 5 < input_len) && (input[i + 1] == 'u')
                && (VALID_HEX(input[i + 2])) && (VALID_HEX(input[i + 3]))
                && (VALID_HEX(input[i + 4])) && (VALID_HEX(input[i + 5]))) {
                /* \uHHHH */

                /* Use only the lower byte. */
                *d = x2c(&input[i + 4]);

                /* Full width ASCII (ff01 - ff5e) needs 0x20 added */
                if ((*d > 0x00) && (*d < 0x5f)
                    && ((input[i + 2] == 'f') || (input[i + 2] == 'F'))
                    && ((input[i + 3] == 'f') || (input[i + 3] == 'F'))) {
                    (*d) += 0x20;
                }

                d++;
                count++;
                i += 6;
            } else if ((i + 3 < input_len) && (input[i + 1] == 'x')
                    && VALID_HEX(input[i + 2]) && VALID_HEX(input[i + 3])) {
                /* \xHH */
                *d++ = x2c(&input[i + 2]);
                count++;
                i += 4;
            } else if ((i + 1 < input_len) && ISODIGIT(input[i + 1])) {
                /* \OOO (only one byte, \000 - \377) */
                char buf[4];
                int j = 0;

                while ((i + 1 + j < input_len) && (j < 3)) {
                    buf[j] = input[i + 1 + j];
                    j++;
                    if (!ISODIGIT(input[i + 1 + j])) break;
                }
                buf[j] = '\0';

                if (j > 0) {
                    /* Do not use 3 characters if we will be > 1 byte */
                    if ((j == 3) && (buf[0] > '3')) {
                        j = 2;
                        buf[j] = '\0';
                    }
                    *d++ = (unsigned char)strtol(buf, NULL, 8);
                    i += 1 + j;
                    count++;
                }
            } else if (i + 1 < input_len) {
                /* \C */
                unsigned char c = input[i + 1];
                switch (input[i + 1]) {
                    case 'a' :
                        c = '\a';
                        break;
                    case 'b' :
                        c = '\b';
                        break;
                    case 'f' :
                        c = '\f';
                        break;
                    case 'n' :
                        c = '\n';
                        break;
                    case 'r' :
                        c = '\r';
                        break;
                    case 't' :
                        c = '\t';
                        break;
                    case 'v' :
                        c = '\v';
                        break;
                        /* The remaining (\?,\\,\',\") are just a removal
                         * of the escape char which is default.
                         */
                }

                *d++ = c;
                i += 2;
                count++;
            } else {
                /* Not enough bytes */
                while (i < input_len) {
                    *d++ = input[i++];
                    count++;
                }
            }
        } else {
            *d++ = input[i++];
            count++;
        }
    }

    *d = '\0';

    return count;
}


/**
 * Decode a string that contains CSS-escaped characters.
 *
 * References:
 *     http://www.w3.org/TR/REC-CSS2/syndata.html#q4
 *     http://www.unicode.org/roadmaps/
 */
int css_decode_inplace(unsigned char *input, int64_t input_len) {
    unsigned char *d = (unsigned char *)input;
    int64_t i, j, count;

    if (input == NULL) {
        return -1;
    }

    i = count = 0;
    while (i < input_len) {
        /* Is the character a backslash? */
        if (input[i] == '\\') {
            /* Is there at least one more byte? */
            if (i + 1 < input_len) {
                i++; /* We are not going to need the backslash. */

                /* Check for 1-6 hex characters following the backslash */
                j = 0;
                while ((j < 6)
                        && (i + j < input_len)
                        && (VALID_HEX(input[i + j]))) {
                    j++;
                }

                if (j > 0) {
                    /* We have at least one valid hexadecimal character. */
                    int fullcheck = 0;

                    /* For now just use the last two bytes. */
                    switch (j) {
                        /* Number of hex characters */
                        case 1:
                            *d++ = xsingle2c(&input[i]);
                            break;

                        case 2:
                        case 3:
                            /* Use the last two from the end. */
                            *d++ = x2c(&input[i + j - 2]);
                            break;

                        case 4:
                            /* Use the last two from the end, but request
                             * a full width check.
                             */
                            *d = x2c(&input[i + j - 2]);
                            fullcheck = 1;
                            break;

                        case 5:
                            /* Use the last two from the end, but request
                             * a full width check if the number is greater
                             * or equal to 0xFFFF.
                             */
                            *d = x2c(&input[i + j - 2]);
                            /* Do full check if first byte is 0 */
                            if (input[i] == '0') {
                                fullcheck = 1;
                            } else {
                                d++;
                            }
                            break;

                        case 6:
                            /* Use the last two from the end, but request
                             * a full width check if the number is greater
                             * or equal to 0xFFFF.
                             */
                            *d = x2c(&input[i + j - 2]);

                            /* Do full check if first/second bytes are 0 */
                            if ((input[i] == '0')
                                    && (input[i + 1] == '0')) {
                                fullcheck = 1;
                            } else {
                                d++;
                            }
                            break;
                    }

                    /* Full width ASCII (0xff01 - 0xff5e) needs 0x20 added */
                    if (fullcheck) {
                        if ((*d > 0x00) && (*d < 0x5f)
                                && ((input[i + j - 3] == 'f') ||
                                    (input[i + j - 3] == 'F'))
                                && ((input[i + j - 4] == 'f') ||
                                    (input[i + j - 4] == 'F'))) {
                            (*d) += 0x20;
                        }

                        d++;
                    }

                    /* We must ignore a single whitespace after a hex escape */
                    if ((i + j < input_len) && isspace(input[i + j])) {
                        j++;
                    }

                    /* Move over. */
                    count++;
                    i += j;
                } else if (input[i] == '\n') {
                /* No hexadecimal digits after backslash */
                    /* A newline character following backslash is ignored. */
                    i++;
                } else {
                /* The character after backslash is not a hexadecimal digit,
                 * nor a newline. */
                    /* Use one character after backslash as is. */
                    *d++ = input[i++];
                    count++;
                }
            } else {
            /* No characters after backslash. */
                /* Do not include backslash in output
                 *(continuation to nothing) */
                i++;
            }
        } else {
        /* Character is not a backslash. */
            /* Copy one normal character to output. */
            *d++ = input[i++];
            count++;
        }
    }

    /* Terminate output string. */
    *d = '\0';

    return count;
}


/**
 *
 * IMP1 Assumes NUL-terminated
 */
int html_entities_decode_inplace(unsigned char *input, int input_len) {
    unsigned char *d = input;
    int i, count;

    if ((input == NULL) || (input_len <= 0)) return 0;

    i = count = 0;
    while ((i < input_len) && (count < input_len)) {
        int z, copy = 1;

        /* Require an ampersand and at least one character to
         * start looking into the entity.
         */
        if ((input[i] == '&') && (i + 1 < input_len)) {
            int k, j = i + 1;

            if (input[j] == '#') {
                /* Numerical entity. */
                copy++;

                if (!(j + 1 < input_len)) {
                    /* Not enough bytes. */
                    goto HTML_ENT_OUT;
                }
                j++;

                if ((input[j] == 'x') || (input[j] == 'X')) {
                    /* Hexadecimal entity. */
                    copy++;

                    if (!(j + 1 < input_len)) {
                        /* Not enough bytes. */
                        goto HTML_ENT_OUT;
                    }
                    j++; /* j is the position of the first digit now. */

                    k = j;
                    while ((j < input_len) && (isxdigit(input[j]))) j++;
                    if (j > k) { /* Do we have at least one digit? */
                        char *x;
                        /* Decode the entity. */
                        /* char *x = apr_pstrmemdup(mp,
                         * (const char *)&input[k], j - k); */
                        x = reinterpret_cast<char *>(malloc(sizeof(char) *
                            (j - k)));
                        memcpy(x, (const char *)&input[k], j - k);
                        *d++ = (unsigned char)strtol(x, NULL, 16);
                        count++;
                        free(x);
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
                    k = j;
                    while ((j < input_len) && (isdigit(input[j]))) j++;
                    if (j > k) { /* Do we have at least one digit? */
                        /* Decode the entity. */
                        char *x = NULL;
                        /* char *x = apr_pstrmemdup(mp,
                         * (const char *)&input[k], j - k); */
                        x = reinterpret_cast<char *>(malloc(sizeof(char) *
                            (j - k)));
                        memcpy(x, (const char *)&input[k], j - k);
                        *d++ = (unsigned char)strtol(x, NULL, 10);
                        count++;
                        free(x);
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

                k = j;
                while ((j < input_len) && (isalnum(input[j]))) j++;
                if (j > k) { /* Do we have at least one digit? */
                    /* char *x = apr_pstrmemdup(mp,
                     *  (const char *)&input[k], j - k); */
                    char *x = NULL;
                    x = reinterpret_cast<char *>(malloc(sizeof(char) *
                        (j - k)));
                    memcpy(x, (const char *)&input[k], j - k);

                    /* Decode the entity. */
                    /* ENH What about others? */
                    if (strcasecmp(x, "quot") == 0) {
                        *d++ = '"';
                    } else if (strcasecmp(x, "amp") == 0) {
                        *d++ = '&';
                    } else if (strcasecmp(x, "lt") == 0) {
                        *d++ = '<';
                    } else if (strcasecmp(x, "gt") == 0) {
                        *d++ = '>';
                    } else if (strcasecmp(x, "nbsp") == 0) {
                        *d++ = NBSP;
                    } else {
                        /* We do no want to convert this entity,
                        * copy the raw data over. */
                        copy = j - k + 1;
                        free(x);
                        goto HTML_ENT_OUT;
                    }

                    count++;

                    /* Skip over the semicolon if it's there. */
                    if ((j < input_len) && (input[j] == ';')) {
                        i = j + 1;
                    } else {
                        i = j;
                    }

                    free(x);
                    continue;
                }
            }
        }
HTML_ENT_OUT:
        for (z = 0; ((z < copy) && (count < input_len)); z++) {
            *d++ = input[i++];
            count++;
        }
    }

    *d = '\0';

    return count;
}


/**
 *
 * IMP1 Assumes NUL-terminated
 */
int normalize_path_inplace(unsigned char *input, int input_len,
    int win, int *changed) {
    unsigned char *src;
    unsigned char *dst;
    unsigned char *end;
    int ldst = 0;
    int hitroot = 0;
    int done = 0;
    int relative;
    int trailing;

    *changed = 0;

    /* Need at least one byte to normalize */
    if (input_len <= 0) return 0;

    /*
     * ENH: Deal with UNC and drive letters?
     */

    src = dst = input;
    end = input + (input_len - 1);
    ldst = 1;

    relative = ((*input == '/') || (win && (*input == '\\'))) ? 0 : 1;
    trailing = ((*end == '/') || (win && (*end == '\\'))) ? 1 : 0;


    while (!done && (src <= end) && (dst <= end)) {
        /* Convert backslash to forward slash on Windows only. */
        if (win) {
            if (*src == '\\') {
                *src = '/';
                *changed = 1;
            }
            if ((src < end) && (*(src + 1) == '\\')) {
                *(src + 1) = '/';
                *changed = 1;
            }
        }

        /* Always normalize at the end of the input. */
        if (src == end) {
            done = 1;
        } else if (*(src + 1) != '/') {
        /* Skip normalization if this is NOT the
         *end of the path segment. */
            goto copy; /* Skip normalization. */
        }

        /*** Normalize the path segment. ***/

        /* Could it be an empty path segment? */
        if ((src != end) && *src == '/') {
            /* Ignore */
            *changed = 1;
            goto copy; /* Copy will take care of this. */
        } else if (*src == '.') {
        /* Could it be a back or self reference? */
            /* Back-reference? */
            if ((dst > input) && (*(dst - 1) == '.')) {
                /* If a relative path and either our normalization has
                 * already hit the rootdir, or this is a backref with no
                 * previous path segment, then mark that the rootdir was hit
                 * and just copy the backref as no normilization is possible.
                 */
                if (relative && (hitroot || ((dst - 2) <= input))) {
                    hitroot = 1;

                    goto copy; /* Skip normalization. */
                }

                /* Remove backreference and the previous path segment. */
                dst -= 3;
                while ((dst > input) && (*dst != '/')) {
                    dst--;
                }

                /* But do not allow going above rootdir. */
                if (dst <= input) {
                    hitroot = 1;
                    dst = input;

                    /* Need to leave the root slash if this
                     * is not a relative path and the end was reached
                     * on a backreference.
                     */
                    if (!relative && (src == end)) {
                        dst++;
                    }
                }

                if (done) goto length; /* Skip the copy. */
                src++;

                *changed = 1;
            } else if (dst == input) {
                /* Relative Self-reference? */
                *changed = 1;

                /* Ignore. */

                if (done) goto length; /* Skip the copy. */
                src++;
            } else if (*(dst - 1) == '/') {
            /* Self-reference? */
                *changed = 1;

                /* Ignore. */

                if (done) goto length; /* Skip the copy. */
                dst--;
                src++;
            }
        } else if (dst > input) {
        /* Found a regular path segment. */
            hitroot = 0;
        }

copy:
        /*** Copy the byte if required. ***/

        /* Skip to the last forward slash when multiple are used. */
        if (*src == '/') {
            unsigned char *oldsrc = src;

            while ((src < end)
                && ((*(src + 1) == '/') || (win && (*(src + 1) == '\\'))) ) {
                src++;
            }
            if (oldsrc != src) *changed = 1;

            /* Do not copy the forward slash to the root
             * if it is not a relative path.  Instead
             * move over the slash to the next segment.
             */
            if (relative && (dst == input)) {
                src++;
                goto length; /* Skip the copy */
            }
        }

        *(dst++) = *(src++);

length:
        ldst = (dst - input);
    }
    /* Make sure that there is not a trailing slash in the
     * normalized form if there was not one in the original form.
     */
    if (!trailing && (dst > input) && *(dst - 1) == '/') {
        ldst--;
        dst--;
    }

    /* Always NUL terminate */
    *dst = '\0';

    return ldst;
}


/**
 * Converts a single hexadecimal digit into a decimal value.
 */
unsigned char xsingle2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));

    return digit;
}


unsigned char x2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}


unsigned char *c2x(unsigned what, unsigned char *where) {
    static const char c2x_table[] = "0123456789abcdef";

    what = what & 0xff;
    *where++ = c2x_table[what >> 4];
    *where++ = c2x_table[what & 0x0f];

    return where;
}


std::string string_to_hex(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


/**
 *
 * IMP1 Assumes NUL-terminated
 */
int urldecode_uni_nonstrict_inplace_ex(Assay *assay, unsigned char *input,
    int64_t input_len, int *changed) {
    unsigned char *d = input;
    int64_t i, count, fact, j, xv;
    int Code, hmap = -1;

    *changed = 0;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            if ((i + 1 < input_len) &&
                ((input[i + 1] == 'u') || (input[i + 1] == 'U'))) {
            /* Character is a percent sign. */
                /* IIS-specific %u encoding. */
                if (i + 5 < input_len) {
                    /* We have at least 4 data bytes. */
                    if ((VALID_HEX(input[i + 2])) &&
                        (VALID_HEX(input[i + 3])) &&
                        (VALID_HEX(input[i + 4])) &&
                        (VALID_HEX(input[i + 5]))) {
                        Code = 0;
                        fact = 1;

                        if (assay
                            && assay->m_rules->unicode_map_table != NULL
                            && assay->m_rules->unicode_codepage > 0)  {
                            for (j = 5; j >= 2; j--) {
                                if (isxdigit((input[i+j]))) {
                                    if (input[i+j] >= 97) {
                                        xv = (input[i+j] - 97) + 10;
                                    } else if (input[i+j] >= 65)  {
                                        xv = (input[i+j] - 65) + 10;
                                    } else {
                                        xv = (input[i+j]) - 48;
                                    }
                                    Code += (xv * fact);
                                    fact *= 16;
                                }
                            }

                            if (Code >= 0 && Code <= 65535)  {
                                hmap = assay->m_rules->unicode_map_table[Code];
                            }
                        }

                        if (hmap != -1)  {
                            *d = hmap;
                        } else {
                            /* We first make use of the lower byte here,
                             * ignoring the higher byte. */
                            *d = x2c(&input[i + 4]);

                            /* Full width ASCII (ff01 - ff5e)
                             * needs 0x20 added */
                            if ((*d > 0x00) && (*d < 0x5f)
                                    && ((input[i + 2] == 'f')
                                    || (input[i + 2] == 'F'))
                                    && ((input[i + 3] == 'f')
                                    || (input[i + 3] == 'F'))) {
                                (*d) += 0x20;
                            }
                        }
                        d++;
                        count++;
                        i += 6;
                        *changed = 1;
                    } else {
                        /* Invalid data, skip %u. */
                        *d++ = input[i++];
                        *d++ = input[i++];
                        count += 2;
                    }
                } else {
                    /* Not enough bytes (4 data bytes), skip %u. */
                    *d++ = input[i++];
                    *d++ = input[i++];
                    count += 2;
                }
            } else {
                /* Standard URL encoding. */
                /* Are there enough bytes available? */
                if (i + 2 < input_len) {
                    /* Yes. */

                    /* Decode a %xx combo only if it is valid.
                     */
                    char c1 = input[i + 1];
                    char c2 = input[i + 2];

                    if (VALID_HEX(c1) && VALID_HEX(c2)) {
                        *d++ = x2c(&input[i + 1]);
                        count++;
                        i += 3;
                        *changed = 1;
                    } else {
                        /* Not a valid encoding, skip this % */
                        *d++ = input[i++];
                        count++;
                    }
                } else {
                    /* Not enough bytes available, skip this % */
                    *d++ = input[i++];
                    count++;
                }
            }
        } else {
            /* Character is not a percent sign. */
            if (input[i] == '+') {
                *d++ = ' ';
                *changed = 1;
            } else {
                *d++ = input[i];
            }

            count++;
            i++;
        }
    }

    *d = '\0';

    return count;
}

std::string limitTo(int amount, const std::string &str) {
    std::string ret;

    if (str.length() > amount) {
        ret.assign(str, 0, amount);
        ret = ret + " (" + std::to_string(str.length() - amount) + " " \
            "characters omitted)";
        return ret;
    }

    return str;
}

std::string toHexIfNeeded(const std::string &str) {
    std::stringstream res;

    for (int i = 0; i < str.size(); i++) {
        int c = str.at(i);
        if (c < 32 || c > 126) {
            res << "\\x" << std::setw(2) << std::setfill('0') << std::hex << c;
        } else {
            res << str.at(i);
        }
    }

    return res.str();
}


std::vector<std::string> expandEnv(const std::string& var, int flags) {
    std::vector<std::string> vars;

    wordexp_t p;
    if (wordexp(var.c_str(), &p, flags) == false) {
        if (p.we_wordc) {
            for (char** exp = p.we_wordv; *exp; ++exp) {
                vars.push_back(exp[0]);
            }
        }
        wordfree(&p);
    }
    return vars;
}


std::string get_path(const std::string& file) {
    size_t found;

    found = file.find_last_of("/\\");
    if (found > 0) {
        return file.substr(0, found);
    }

    return std::string("");
}


std::string find_resource(const std::string& resource,
    const std::string& config) {
    std::ifstream *iss = NULL;

    // Trying absolute or relative to the current dir.
    iss = new std::ifstream(resource, std::ios::in);
    if (iss->is_open()) {
        iss->close();
        delete iss;
        return resource;
    }
    delete iss;

    // Trying the same path of the configuration file.
    std::string f = get_path(config) + "/" + resource;
    iss = new std::ifstream(f, std::ios::in);
    if (iss->is_open()) {
        iss->close();
        delete iss;
        return f;
    }
    delete iss;

    return std::string("");
}

}  // namespace ModSecurity

