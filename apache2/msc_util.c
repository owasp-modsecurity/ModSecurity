/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2008 Breach Security, Inc. (http://www.breach.com/)
 *
 * This product is released under the terms of the General Public Licence,
 * version 2 (GPLv2). Please refer to the file LICENSE (included with this
 * distribution) which contains the complete text of the licence.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the exception in
 * file MODSECURITY_LICENSING_EXCEPTION in the directory of this software
 * distribution.
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Breach Security, Inc.
 * directly using the email address support@breach.com.
 *
 */
#include "msc_release.h"
#include "msc_util.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <apr_lib.h>

/* NOTE: Be careful as these can ONLY be used on static values for X.
 * (i.e. VALID_HEX(c++) will NOT work)
 */
#define VALID_HEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))
#define ISODIGIT(X) ((X >= '0')&&(X <= '7'))


/**
 *
 */
int parse_boolean(const char *input) {
    if (input == NULL) return -1;
    if (strcasecmp(input, "on") == 0) return 1;
    if (strcasecmp(input, "true") == 0) return 1;
    if (strcasecmp(input, "1") == 0) return 1;
    if (strcasecmp(input, "off") == 0) return 0;
    if (strcasecmp(input, "false") == 0) return 0;
    if (strcasecmp(input, "0") == 0) return 0;

    return -1;
}

/**
 * Parses a string that contains a name-value pair in the form "name=value".
 * IMP1 It does not check for whitespace between tokens.
 */
int parse_name_eq_value(apr_pool_t *mp, const char *input, char **name, char **value) {
    char *p = NULL;

    if ((name == NULL)||(value == NULL)) return -1;
    if (input == NULL) return 0;

    *name = NULL;
    *value = NULL;
    p = (char *)input;

    while((*p != '=')&&(*p != '\0')) p++;
    if (*p == '\0') {
        *name = (char *)input;
        return 1;
    }

    *name = apr_pstrmemdup(mp, input, p - input);
    if (*name == NULL) return -1;
    p++;

    *value = apr_pstrdup(mp, p);
    if (*value == NULL) return -1;

    return 1;
}

/**
 *
 * IMP1 Assumes NUL-terminated
 */
char *url_encode(apr_pool_t *mp, char *input, unsigned int input_len, int *changed) {
    char *rval, *d;
    unsigned int i, len;

    *changed = 0;

    len = input_len * 3 + 1;
    d = rval = apr_palloc(mp, len);
    if (rval == NULL) return NULL;

    /* ENH Only encode the characters that really need to be encoded. */

    for(i = 0; i < input_len; i++) {
        unsigned char c = input[i];

        if (c == ' ') {
            *d++ = '+';
            *changed = 1;
        } else
        if ( (c == 42) || ((c >= 48)&&(c <= 57)) || ((c >= 65)&&(c <= 90))
            || ((c >= 97)&&(c <= 122))
        ) {
            *d++ = c;
        } else {
            *d++ = '%';
            c2x(c, (unsigned char *)d);
            d += 2;
            *changed = 1;
        }
    }

    *d = '\0';

    return rval;
}

/**
 * Appends an URL-encoded version of the source string to the
 * destination string, but makes sure that no more than "maxlen"
 * bytes are added.
 */
char *strnurlencat(char *destination, char *source, unsigned int maxlen) {
    char *s = source;
    char *d = destination;

    /* ENH Only encode the characters that really need to be encoded. */

    /* Advance to the end of destination string. */
    while(*d != '\0') d++;

    /* Loop while there's bytes in the source string or
     * until we reach the output limit.
     */
    while((*s != '\0')&&(maxlen > 0)) {
        unsigned char c = *s;

        if (c == ' ') {
            *d++ = '+';
            maxlen--;
        } else
        if ( (c == 42) || ((c >= 48)&&(c <= 57)) || ((c >= 65)&&(c <= 90))
            || ((c >= 97)&&(c <= 122))
        ) {
            *d++ = c;
            maxlen--;
        } else {
            if (maxlen >= 3) {
                *d++ = '%';
                c2x(c, (unsigned char *)d);
                d += 2;
                maxlen -= 3;
            } else {
                /* If there's not enough room for the encoded
                 * byte we ignore it.
                 */
                maxlen = 0;
            }
        }

        s++;
    }

    *d++ = '\0';

    return destination;
}

/**
 *
 */
char *file_basename(apr_pool_t *mp, const char *filename) {
    char *d, *p;

    if (filename == NULL) return NULL;
    d = apr_pstrdup(mp, filename);
    if (d == NULL) return NULL;

    p = strrchr(d, '/');
    if (p != NULL) d = p + 1;
    p = strrchr(d, '\\');
    if (p != NULL) d = p + 1;

    return d;
}

/**
 *
 */
#ifdef WIN32
char *file_dirname(apr_pool_t *p, const char *filename) {
    char *b, *c, *d;

    if (filename == NULL) return NULL;
    b = apr_pstrdup(p, filename);
    if (b == NULL) return NULL;

    c = strrchr(b, '/');
    if (c != NULL) {
        d = strrchr(c, '\\');
        if (d != NULL) *d = '\0';
        else *c = '\0';
    } else {
        d = strrchr(b, '\\');
        if (d != NULL) *d = '\0';
    }

    return b;
}
#else
char *file_dirname(apr_pool_t *p, const char *filename) {
    char *b, *c;

    if (filename == NULL) return NULL;
    b = apr_pstrdup(p, filename);
    if (b == NULL) return NULL;

    c = strrchr(b, '/');
    if (c != NULL) *c = '\0';

    return b;
}
#endif


/**
 *
 */
int hex2bytes_inplace(unsigned char *data, int len) {
    unsigned char *d = data;
    int i, count = 0;

    if ((data == NULL)||(len == 0)) return 0;

    for(i = 0; i <= len - 2; i += 2) {
        *d++ = x2c(&data[i]);
        count++;
    }
    *d = '\0';

    return count;
}

/**
 * Converts a series of bytes into its hexadecimal
 * representation.
 */
char *bytes2hex(apr_pool_t *pool, unsigned char *data, int len) {
    static unsigned char b2hex[] = "0123456789abcdef";
    char *hex = NULL;
    int i, j;

    hex = apr_palloc(pool, (len * 2) + 1);
    if (hex == NULL) return NULL;

    j = 0;
    for(i = 0; i < len; i++) {
        hex[j++] = b2hex[data[i] >> 4];
        hex[j++] = b2hex[data[i] & 0x0f];
    }
    hex[j] = 0;

    return hex;
}

/**
 *
 */
int is_token_char(unsigned char c) {
    /* ENH Is the performance important at all? We could use a table instead. */

    /* CTLs not allowed */
    if ((c <= 32)||(c >= 127)) return 0;

    switch(c) {
        case '(' :
        case ')' :
        case '<' :
        case '>' :
        case '@' :
        case ',' :
        case ';' :
        case ':' :
        case '\\' :
        case '"' :
        case '/' :
        case '[' :
        case ']' :
        case '?' :
        case '=' :
            return 0;
    }

    return 1;
}

/**
 *
 */
int remove_lf_crlf_inplace(char *text) {
    char *p = text;
    int count = 0;

    if (text == NULL) return -1;

    while(*p != '\0') {
        count++;
        p++;
    }

    if (count > 0) {
        if (*(p - 1) == '\n') {
            *(p - 1) = '\0';
            if (count > 1) {
                if (*(p - 2) == '\r') {
                    *(p - 2) = '\0';
                }
            }
        }
    }

    return 1;
}

/**
 * Converts a byte given as its hexadecimal representation
 * into a proper byte. Handles uppercase and lowercase letters
 * but does not check for overflows.
 */
unsigned char x2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}

/**
 * Converts a single hexadecimal digit into a decimal value.
 */
unsigned char xsingle2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));

    return digit;
}

/**
 *
 */
char *guess_tmp_dir(apr_pool_t *p) {
    char *filename = NULL;

    /* ENH Use apr_temp_dir_get instead. */

    #ifdef WIN32
    filename = apr_pcalloc(p, 256);
    if (filename == NULL) return "";
    if (GetTempPath(255, filename) != 0) return filename;
    #endif

    filename = getenv("TMPDIR");
    if (filename != NULL) return filename;

    filename = getenv("TEMP");
    if (filename != NULL) return filename;

    filename = getenv("TMP");
    if (filename != NULL) return filename;

    #if defined NETWARE
    return("sys:/tmp/");
    #elif defined WIN32
    return("");
    #else
    return("/tmp/");
    #endif
}

/**
 *
 */
char *current_logtime(apr_pool_t *mp) {
    apr_time_exp_t t;
    char tstr[100];
    apr_size_t len;

    apr_time_exp_lt(&t, apr_time_now());

    apr_strftime(tstr, &len, 80, "%d/%b/%Y:%H:%M:%S ", &t);
    apr_snprintf(tstr + strlen(tstr), 80 - strlen(tstr), "%c%.2d%.2d",
        t.tm_gmtoff < 0 ? '-' : '+',
        t.tm_gmtoff / (60 * 60), t.tm_gmtoff % (60 * 60));
    return apr_pstrdup(mp, tstr);
}

/**
 *
 */
char *current_filetime(apr_pool_t *mp) {
    apr_time_exp_t t;
    char tstr[100];
    apr_size_t len;

    apr_time_exp_lt(&t, apr_time_now());

    apr_strftime(tstr, &len, 80, "%Y%m%d-%H%M%S", &t);
    return apr_pstrdup(mp, tstr);
}

/**
 *
 */
int msc_mkstemp(char *template) {
    /* ENH Use apr_file_mktemp instead. */

    #if !(defined(WIN32)||defined(NETWARE))
    return mkstemp(template);
    #else
    if (mktemp(template) == NULL) return -1;
    return open(template, O_WRONLY | O_APPEND | O_CREAT | O_BINARY, CREATEMODE_UNISTD);
    #endif
}

/**
 * Converts the input string to lowercase (in-place).
 */
char *strtolower_inplace(unsigned char *str) {
    unsigned char *c = str;

    if (str == NULL) return NULL;

    while(*c != 0) {
        *c = tolower(*c);
        c++;
    }

    return (char *)str;
}

/**
 * Converts a single byte into its hexadecimal representation.
 * Will overwrite two bytes at the destination.
 */
unsigned char *c2x(unsigned what, unsigned char *where) {
    static const char c2x_table[] = "0123456789abcdef";

    what = what & 0xff;
    *where++ = c2x_table[what >> 4];
    *where++ = c2x_table[what & 0x0f];

    return where;
}

char *log_escape(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 1, 0);
}

char *log_escape_nq(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 0, 0);
}

char *log_escape_ex(apr_pool_t *mp, const char *text, unsigned long int text_length) {
    return _log_escape(mp, (const unsigned char *)text, text_length, 1, 0);
}

char *log_escape_nq_ex(apr_pool_t *mp, const char *text, unsigned long int text_length) {
    return _log_escape(mp, (const unsigned char *)text, text_length, 0, 0);
}

char *log_escape_header_name(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 0, 1);
}

char *log_escape_raw(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length) {
    unsigned char *ret = apr_palloc(mp, text_length * 4 + 1);
    unsigned long int i, j;

    for (i = 0, j = 0; i < text_length; i++, j += 4) {
        ret[j] = '\\';
        ret[j+1] = 'x';
        c2x(text[i], ret+j+2);
    }
    ret[text_length * 4] = '\0';

    return (char *)ret;
}

/**
 * Transform text to ASCII printable or hex escaped
 */
char *log_escape_hex(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length) {
    unsigned char *ret = apr_palloc(mp, text_length * 4 + 1);
    unsigned long int i, j;

    for (i = 0, j = 0; i < text_length; i++) {
        if (  (text[i] == '"')
            ||(text[i] == '\\')
            ||(text[i] <= 0x1f)
            ||(text[i] >= 0x7f))
        {
            ret[j] = '\\';
            ret[j+1] = 'x';
            c2x(text[i], ret+j+2);
            j += 4;
        }
        else {
            ret[j] = text[i];
            j ++;
        }
    }
    ret[j] = '\0';

    return (char *)ret;
}

/**
 * Transform input into a form safe for logging.
 */
char *_log_escape(apr_pool_t *mp, const unsigned char *input, unsigned long int input_len,
    int escape_quotes, int escape_colon)
{
    unsigned char *d = NULL;
    char *ret = NULL;
    unsigned long int i;

    if (input == NULL) return NULL;

    ret = apr_palloc(mp, input_len * 4 + 1);
    if (ret == NULL) return NULL;
    d = (unsigned char *)ret;

    i = 0;
    while(i < input_len) {
        switch(input[i]) {
            case ':' :
                if (escape_colon) {
                    *d++ = '\\';
                    *d++ = ':';
                } else {
                    *d++ = input[i];
                }
                break;
            case '"' :
                if (escape_quotes) {
                    *d++ = '\\';
                    *d++ = '"';
                } else {
                    *d++ = input[i];
                }
                break;
            case '\b' :
                *d++ = '\\';
                *d++ = 'b';
                break;
            case '\n' :
                *d++ = '\\';
                *d++ = 'n';
                break;
            case '\r' :
                *d++ = '\\';
                *d++ = 'r';
                break;
            case '\t' :
                *d++ = '\\';
                *d++ = 't';
                break;
            case '\v' :
                *d++ = '\\';
                *d++ = 'v';
                break;
            case '\\' :
                *d++ = '\\';
                *d++ = '\\';
                break;
            default :
                if ((input[i] <= 0x1f)||(input[i] >= 0x7f)) {
                    *d++ = '\\';
                    *d++ = 'x';
                    c2x(input[i], d);
                    d += 2;
                } else {
                    *d++ = input[i];
                }
                break;
        }

        i++;
    }

    *d = 0;

    return ret;
}

/**
 * JavaScript decoding.
 * IMP1 Assumes NUL-terminated
 */

int js_decode_nonstrict_inplace(unsigned char *input, long int input_len) {
    unsigned char *d = (unsigned char *)input;
    long int i, count;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '\\') {
            /* Character is an escape. */

            if (   (i + 5 < input_len) && (input[i + 1] == 'u')
                && (VALID_HEX(input[i + 2])) && (VALID_HEX(input[i + 3]))
                && (VALID_HEX(input[i + 4])) && (VALID_HEX(input[i + 5])) )
            {
                /* \uHHHH */

                /* Use only the lower byte. */
                *d = x2c(&input[i + 4]);

                /* Full width ASCII (ff01 - ff5e) needs 0x20 added */
                if (   (*d > 0x00) && (*d < 0x5f)
                    && ((input[i + 2] == 'f') || (input[i + 2] == 'F'))
                    && ((input[i + 3] == 'f') || (input[i + 3] == 'F')))
                {
                    (*d) += 0x20;
                }

                d++;
                count++;
                i += 6;
            }
            else if (   (i + 3 < input_len) && (input[i + 1] == 'x')
                     && VALID_HEX(input[i + 2]) && VALID_HEX(input[i + 3])) {
                /* \xHH */
                *d++ = x2c(&input[i + 2]);
                count++;
                i += 4;
            }
            else if ((i + 1 < input_len) && ISODIGIT(input[i + 1])) {
                /* \OOO (only one byte, \000 - \377) */
                char buf[4];
                int j = 0;

                while((i + 1 + j < input_len)&&(j < 3)) {
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
                    *d++ = strtol(buf, NULL, 8);
                    i += 1 + j;
                    count++;
                }
            }
            else if (i + 1 < input_len) {
                /* \C */
                unsigned char c = input[i + 1];
                switch(input[i + 1]) {
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
            }
            else {
                /* Not enough bytes */
                while(i < input_len) {
                    *d++ = input[i++];
                    count++;
                }
            }
        }
        else {
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
int urldecode_uni_nonstrict_inplace_ex(unsigned char *input, long int input_len, int *changed) {
    unsigned char *d = input;
    long int i, count;

    *changed = 0;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            /* Character is a percent sign. */

            if ((i + 1 < input_len)&&( (input[i + 1] == 'u')||(input[i + 1] == 'U') )) {
                /* IIS-specific %u encoding. */
                if (i + 5 < input_len) {
                    /* We have at least 4 data bytes. */
                    if (  (VALID_HEX(input[i + 2]))&&(VALID_HEX(input[i + 3]))
                        &&(VALID_HEX(input[i + 4]))&&(VALID_HEX(input[i + 5])) )
                    {
                        /* We first make use of the lower byte here, ignoring the higher byte. */
                        *d = x2c(&input[i + 4]);

                        /* Full width ASCII (ff01 - ff5e) needs 0x20 added */
                        if (   (*d > 0x00) && (*d < 0x5f)
                            && ((input[i + 2] == 'f') || (input[i + 2] == 'F'))
                            && ((input[i + 3] == 'f') || (input[i + 3] == 'F')))
                        {
                            (*d) += 0x20;
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
            }
            else {
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
        }
        else {
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

/**
 *
 * IMP1 Assumes NUL-terminated
 */
int urldecode_nonstrict_inplace_ex(unsigned char *input, long int input_len, int *invalid_count, int *changed) {
    unsigned char *d = (unsigned char *)input;
    long int i, count;

    *changed = 0;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            /* Character is a percent sign. */

            /* Are there enough bytes available? */
            if (i + 2 < input_len) {
                char c1 = input[i + 1];
                char c2 = input[i + 2];

                if (VALID_HEX(c1) && VALID_HEX(c2)) {
                    /* Valid encoding - decode it. */
                    *d++ = x2c(&input[i + 1]);
                    count++;
                    i += 3;
                    *changed = 1;
                } else {
                    /* Not a valid encoding, skip this % */
                    *d++ = input[i++];
                    count ++;
                    (*invalid_count)++;
                }
            } else {
                /* Not enough bytes available, copy the raw bytes. */
                *d++ = input[i++];
                count ++;
                (*invalid_count)++;
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

/**
 *
 * IMP1 Assumes NUL-terminated
 */
int html_entities_decode_inplace(apr_pool_t *mp, unsigned char *input, int input_len) {
    unsigned char *d = input;
    int i, count;

    if ((input == NULL)||(input_len <= 0)) return 0;

    i = count = 0;
    while((i < input_len)&&(count < input_len)) {
        int z, copy = 1;

        /* Require an ampersand and at least one character to
         * start looking into the entity.
         */
        if ((input[i] == '&')&&(i + 1 < input_len)) {
            int k, j = i + 1;

            if (input[j] == '#') {
                /* Numerical entity. */
                copy++;

                if (!(j + 1 < input_len)) goto HTML_ENT_OUT; /* Not enough bytes. */
                j++;

                if ((input[j] == 'x')||(input[j] == 'X')) {
                    /* Hexadecimal entity. */
                    copy++;

                    if (!(j + 1 < input_len)) goto HTML_ENT_OUT; /* Not enough bytes. */
                    j++; /* j is the position of the first digit now. */

                    k = j;
                    while((j < input_len)&&(isxdigit(input[j]))) j++;
                    if (j > k) { /* Do we have at least one digit? */
                        /* Decode the entity. */
                        char *x = apr_pstrmemdup(mp, (const char *)&input[k], j - k);
                        *d++ = (unsigned char)strtol(x, NULL, 16);
                        count++;

                        /* Skip over the semicolon if it's there. */
                        if ((j < input_len)&&(input[j] == ';')) i = j + 1;
                        else i = j;

                        continue;
                    } else {
                        goto HTML_ENT_OUT;
                    }
                } else {
                    /* Decimal entity. */
                    k = j;
                    while((j < input_len)&&(isdigit(input[j]))) j++;
                    if (j > k) { /* Do we have at least one digit? */
                        /* Decode the entity. */
                        char *x = apr_pstrmemdup(mp, (const char *)&input[k], j - k);
                        *d++ = (unsigned char)strtol(x, NULL, 10);
                        count++;

                        /* Skip over the semicolon if it's there. */
                        if ((j < input_len)&&(input[j] == ';')) i = j + 1;
                        else i = j;

                        continue;
                    } else {
                        goto HTML_ENT_OUT;
                    }
                }
            } else {
                /* Text entity. */

                k = j;
                while((j < input_len)&&(isalnum(input[j]))) j++;
                if (j > k) { /* Do we have at least one digit? */
                    char *x = apr_pstrmemdup(mp, (const char *)&input[k], j - k);

                    /* Decode the entity. */
                    /* ENH What about others? */
                    if (strcasecmp(x, "quot") == 0) *d++ = '"';
                    else
                    if (strcasecmp(x, "amp") == 0) *d++ = '&';
                    else
                    if (strcasecmp(x, "lt") == 0) *d++ = '<';
                    else
                    if (strcasecmp(x, "gt") == 0) *d++ = '>';
                    else
                    if (strcasecmp(x, "nbsp") == 0) *d++ = NBSP;
                    else {
                        /* We do no want to convert this entity, copy the raw data over. */
                        copy = j - k + 1;
                        goto HTML_ENT_OUT;
                    }

                    count++;

                    /* Skip over the semicolon if it's there. */
                    if ((j < input_len)&&(input[j] == ';')) i = j + 1;
                    else i = j;

                    continue;
                }
            }
        }

        HTML_ENT_OUT:

        for(z = 0; ((z < copy) && (count < input_len)); z++) {
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
int ansi_c_sequences_decode_inplace(unsigned char *input, int input_len) {
    unsigned char *d = input;
    int i, count;

    i = count = 0;
    while(i < input_len) {
        if ((input[i] == '\\')&&(i + 1 < input_len)) {
            int c = -1;

            switch(input[i + 1]) {
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
                case '\\' :
                    c = '\\';
                    break;
                case '?' :
                    c = '?';
                    break;
                case '\'' :
                    c = '\'';
                    break;
                case '"' :
                    c = '"';
                    break;
            }

            if (c != -1) i += 2;

            /* Hexadecimal or octal? */
            if (c == -1) {
                if ((input[i + 1] == 'x')||(input[i + 1] == 'X')) {
                    /* Hexadecimal. */
                    if ((i + 3 < input_len)&&(isxdigit(input[i + 2]))&&(isxdigit(input[i + 3]))) {
                        /* Two digits. */
                        c = x2c(&input[i + 2]);
                        i += 4;
                    } else {
                        /* Invalid encoding, do nothing. */
                    }
                }
                else
                if (ISODIGIT(input[i + 1])) { /* Octal. */
                    char buf[4];
                    int j = 0;

                    while((i + 1 + j < input_len)&&(j < 3)) {
                        buf[j] = input[i + 1 + j];
                        j++;
                        if (!ISODIGIT(input[i + 1 + j])) break;
                    }
                    buf[j] = '\0';

                    if (j > 0) {
                        c = strtol(buf, NULL, 8);
                        i += 1 + j;
                    }
                }
            }

            if (c == -1) {
                /* Didn't recognise encoding, copy raw bytes. */
                *d++ = input[i + 1];
                count++;
                i += 2;
            } else {
                /* Converted the encoding. */
                *d++ = c;
                count++;
            }
        } else {
            /* Input character not a backslash, copy it. */
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
int normalise_path_inplace(unsigned char *input, int input_len, int win, int *changed) {
    unsigned char *d = input;
    int i, count;

    *changed = 0;

    i = count = 0;
    while ((i < input_len)&&(count < input_len)) {
        char c = input[i];

        /* Convert backslash to forward slash on Windows only. */
        if ((win)&&(c == '\\')) {
            c = '/';
            *changed = 1;
        }

        if (c == '/') {
            /* Is there a directory back-reference? Yes, we
             * require at least 5 prior bytes here. That's on
             * purpose.
             */
            if ((count >= 5)&&(*(d - 1) == '.')&&(*(d - 2) == '.')&&(*(d - 3) == '/')) {
                unsigned char *cd = d - 4;
                int ccount = count - 4;

                *changed = 1;

                /* Go back until we reach the beginning or a forward slash. */
                while ((ccount > 0)&&(*cd != '/')) {
                    ccount--;
                    cd--;
                }

                if (*cd == '/') {
                    d = cd;
                    count = ccount;
                }
            } else
            /* Is there a directory self-reference? */
            if ((count >= 2)&&(*(d - 1) == '.')&&(*(d - 2) == '/')) {
                /* Ignore the last two bytes. */
                d -= 2;
                count -= 2;
                *changed = 1;
            } else
            /* Or are there just multiple occurences of forward slash? */
            if ((count >= 1)&&(*(d - 1) == '/')) {
                /* Ignore the last one byte. */
                d--;
                count--;
                *changed = 1;
            }
        }

        /* Copy the byte over. */
        *d++ = c;
        count++;
        i++;
    }

    *d = '\0';

    return count;
}

char *modsec_build(apr_pool_t *mp) {
    return apr_psprintf(mp, "%02i%02i%02i%1i%02i",
                            atoi(MODSEC_VERSION_MAJOR),
                            atoi(MODSEC_VERSION_MINOR),
                            atoi(MODSEC_VERSION_MAINT),
                            get_modsec_build_type(NULL),
                            atoi(MODSEC_VERSION_RELEASE));
}

int is_empty_string(const char *string) {
    unsigned int i;

    if (string == NULL) return 1;

    for(i = 0; string[i] != '\0'; i++) {
        if (!isspace(string[i])) {
            return 0;
        }
    }

    return 1;
}

char *resolve_relative_path(apr_pool_t *pool, const char *parent_filename, const char *filename) {
    if (filename == NULL) return NULL;
    // TODO Support paths on operating systems other than Unix.
    if (filename[0] == '/') return (char *)filename;

    return apr_pstrcat(pool, apr_pstrndup(pool, parent_filename,
        strlen(parent_filename) - strlen(apr_filepath_name_get(parent_filename))),
        filename, NULL);
}

/**
 * Decode a string that contains CSS-escaped characters.
 * 
 * References:
 *     http://www.w3.org/TR/REC-CSS2/syndata.html#q4
 *     http://www.unicode.org/roadmaps/
 */
int css_decode_inplace(unsigned char *input, long int input_len) {
    unsigned char *d = (unsigned char *)input;
    long int i, j, count;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {

        /* Is the character a backslash? */
        if (input[i] == '\\') {

            /* Is there at least one more byte? */
            if (i + 1 < input_len) {
                i++; /* We are not going to need the backslash. */

                /* Check for 1-6 hex characters following the backslash */
                j = 0;
                while (    (j < 6)
                        && (i + j < input_len)
                        && (VALID_HEX(input[i + j])))
                {
                    j++;
                }

                if (j > 0) { /* We have at least one valid hexadecimal character. */
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
                            }
                            else {
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
                            if (    (input[i] == '0')
                                 && (input[i + 1] == '0')
                            ) {
                                fullcheck = 1;
                            }
                            else {
                                d++;
                            }
                            break;
                    }

                    /* Full width ASCII (0xff01 - 0xff5e) needs 0x20 added */
                    if (fullcheck) {
                        if (   (*d > 0x00) && (*d < 0x5f)
                            && ((input[i + j - 3] == 'f') ||
                                (input[i + j - 3] == 'F'))
                            && ((input[i + j - 4] == 'f') ||
                                (input[i + j - 4] == 'F')))
                        {
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
                }

                /* No hexadecimal digits after backslash */
                else if (input[i] == '\n') {
                    /* A newline character following backslash is ignored. */
                    i++;
                }

                /* The character after backslash is not a hexadecimal digit, nor a newline. */
                else {
                    /* Use one character after backslash as is. */
                    *d++ = input[i++];
                    count++;
                }
            }
            
            /* No characters after backslash. */
            else {
                /* Do not include backslash in output (continuation to nothing) */
                i++; 
            }
        }

        /* Character is not a backslash. */
        else {
            /* Copy one normal character to output. */
            *d++ = input[i++];
            count++;
        }
    }

    /* Terminate output string. */
    *d = '\0';

    return count;
}
