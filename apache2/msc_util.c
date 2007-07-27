/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2007 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include "msc_util.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

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
 */
char *url_encode(apr_pool_t *mp, char *input, unsigned int input_len) {
    char *rval, *d;
    unsigned int i, len;

    len = input_len * 3 + 1;
    d = rval = apr_palloc(mp, len);
    if (rval == NULL) return NULL;

    /* ENH Only encode the characters that really need to be encoded. */

    for(i = 0; i < input_len; i++) {
        unsigned char c = input[i];

        if (c == ' ') {
            *d++ = '+';
        } else
        if ( (c == 42) || ((c >= 48)&&(c <= 57)) || ((c >= 65)&&(c <= 90))
            || ((c >= 97)&&(c <= 122))
        ) {
            *d++ = c;
        } else {
            *d++ = '%';
            c2x(c, (unsigned char *)d);
            d += 2;
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

#define VALID_HEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))

/**
 *
 */
int urldecode_uni_nonstrict_inplace_ex(unsigned char *input, long int input_len) {
    unsigned char *d = input;
    long int i, count;

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
                    } else {
                        /* Invalid data. */
                        int j;

                        for(j = 0; (j < 6)&&(i < input_len); j++) {
                            *d++ = input[i++];
                            count++;
                        }
                    }
                } else {
                    /* Not enough bytes available (4 data bytes were needed). */
                    while(i < input_len) {
                        *d++ = input[i++];
                        count++;
                    }
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

                    /* ENH Use VALID_HEX? */
                    if ( (((c1 >= '0')&&(c1 <= '9')) || ((c1 >= 'a')&&(c1 <= 'f')) ||
                          ((c1 >= 'A')&&(c1 <= 'F')))
                        && (((c2 >= '0')&&(c2 <= '9')) || ((c2 >= 'a')&&(c2 <= 'f')) ||
                            ((c2 >= 'A')&&(c2 <= 'F'))) )
                    {
                        *d++ = x2c(&input[i + 1]);
                        count++;
                        i += 3;
                    } else {
                        /* Not a valid encoding, copy the raw input bytes. */
                        *d++ = '%';
                        *d++ = c1;
                        *d++ = c2;
                        count += 3;
                        i += 3;
                    }
                } else {
                    /* Not enough bytes available. */
                
                    *d++ = '%';
                    count++;
                    i++;

                    if (i + 1 < input_len) {
                        *d++ = input[i];
                        count++;
                        i++;
                    }
                }
            }
        }
        else {
            /* Character is not a percent sign. */
            if (input[i] == '+') {
                *d++ = ' ';
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
 */
int urldecode_nonstrict_inplace_ex(unsigned char *input, long int input_len, int *invalid_count) {
    unsigned char *d = (unsigned char *)input;
    long int i, count;

    if (input == NULL) return -1;

    i = count = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            /* Character is a percent sign. */

            /* Are there enough bytes available? */
            if (i + 2 < input_len) {
                char c1 = input[i + 1];
                char c2 = input[i + 2];

                /* ENH Use VALID_HEX? */
                if ( (((c1 >= '0')&&(c1 <= '9')) || ((c1 >= 'a')&&(c1 <= 'f')) || ((c1 >= 'A')&&(c1 <= 'F')))
                    && (((c2 >= '0')&&(c2 <= '9')) || ((c2 >= 'a')&&(c2 <= 'f')) || ((c2 >= 'A')&&(c2 <= 'F'))) )
                {
                    /* Valid encoding - decode it. */
                    *d++ = x2c(&input[i + 1]);
                    count++;
                    i += 3;
                } else {
                    /* Invalid encoding, just copy the raw bytes. */
                    *d++ = '%';
                    *d++ = c1;
                    *d++ = c2;
                    count += 3;
                    i += 3;
                    (*invalid_count)++; /* parens quiet compiler warning */
                }
            } else {
                /* Not enough bytes available, copy the raw bytes. */
                (*invalid_count)++; /* parens quiet compiler warning */

                *d++ = '%';
                count++;
                i++;

                if (i + 1 < input_len) {
                    *d++ = input[i];
                    count++;
                    i++;
                }
            }
        } else {
            /* Character is not a percent sign. */
            if (input[i] == '+') {
                *d++ = ' ';
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

#define ISODIGIT(X) ((X >= '0')&&(X <= '7'))

int ansi_c_sequences_decode_inplace(unsigned char *input, int input_len) {
    unsigned char *d = input;
    int i, count;

    i = count = 0;
    while(i < input_len) {
        if ((input[i] == '\\')&&(i + 1 < input_len)) {
            int c = -1;
        
            /* ENH Should we handle \c as well?
             *     See http://www.opengroup.org/onlinepubs/009695399/utilities/printf.html
             */

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
                if (isdigit(input[i + 1])) { /* Octal. */
                    char buf[10];
                    int j = 0, l = 3;

                    /* Up to 4 digits if the first digit is a zero. */
                    if (input[i + 1] == '0') l = 4;

                    while((i + 1 + j < input_len)&&(j <= l)) {
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

int normalise_path_inplace(unsigned char *input, int input_len, int win) {
    unsigned char *d = input;
    int i, count;

    i = count = 0;
    while ((i < input_len)&&(count < input_len)) {
        char c = input[i];

        /* Convert backslash to forward slash on Windows only. */
        if ((win)&&(c == '\\')) c = '/';

        if (c == '/') {
            /* Is there a directory back-reference? Yes, we
             * require at least 5 prior bytes here. That's on
             * purpose.
             */
            if ((count >= 5)&&(*(d - 1) == '.')&&(*(d - 2) == '.')&&(*(d - 3) == '/')) {
                unsigned char *cd = d - 4;
                int ccount = count - 4;

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
            } else
            /* Or are there just multiple occurences of forward slash? */
            if ((count >= 1)&&(*(d - 1) == '/')) {
                /* Ignore the last one byte. */
                d--;
                count--;
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
