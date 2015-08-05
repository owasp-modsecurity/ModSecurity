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

#define VALID_HEX(X) (((X >= '0') && (X <= '9')) || \
    ((X >= 'a') && (X <= 'f')) || ((X >= 'A') && (X <= 'F')))
#define ISODIGIT(X) ((X >= '0') && (X <= '7'))
#define NBSP 160

namespace ModSecurity {


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
    std::random_device rd;

    return std::bind(
        std::uniform_real_distribution<>{from, to},
        std::default_random_engine{ rd() })();
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
 * Converts a single hexadecimal digit into a decimal value.
 */
static unsigned char xsingle2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));

    return digit;
}


static unsigned char x2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}


}  // namespace ModSecurity

