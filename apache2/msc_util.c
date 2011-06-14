/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#include "modsecurity.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "msc_release.h"
#include "msc_util.h"

#include <apr_lib.h>
#include "modsecurity_config.h"

/**
 * NOTE: Be careful as these can ONLY be used on static values for X.
 * (i.e. VALID_HEX(c++) will NOT work)
 */
#define VALID_HEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))
#define ISODIGIT(X) ((X >= '0')&&(X <= '7'))

#if (defined(WIN32) || defined(NETWARE))
/** Windows does not define all the octal modes */
#define S_IXOTH 00001
#define S_IWOTH 00002
#define S_IROTH 00004
#define S_IXGRP 00010
#define S_IWGRP 00020
#define S_IRGRP 00040
#define S_IXUSR 00100
#define S_IWUSR 00200
#define S_IRUSR 00400
#define S_ISVTX 01000
#define S_ISGID 02000
#define S_ISUID 04000
#endif /* defined(WIN32 || NETWARE) */

/* Base64 tables used in decodeBase64Ext */
static const char b64_pad = '=';

static const short b64_reverse_t[256] = {
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
  -2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
  -2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
};

static unsigned char *c2x(unsigned what, unsigned char *where);
static unsigned char x2c(unsigned char *what);
static unsigned char xsingle2c(unsigned char *what);

/* \brief Remove escape char
 *
 * \param mptmp Pointer to the pool
 * \param input Pointer to input string
 * \param input_len Input data length
 *
 * \retval string On Success
 */
char *remove_escape(apr_pool_t *mptmp, const char *input, int input_len)  {
    char *parm = apr_palloc(mptmp, input_len);
    char *ret = parm;
    int len = input_len;

    for(; *input !='\0' && len >=0; input++, len--)    {
        if(*input != '\\')    {
            *parm++ = *input;
        }
    }

    *parm = '\0';
    return ret;
}

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

/* \brief Decode Base64 data with special chars
 *
 * \param plain_text Pointer to plain text data
 * \param input Pointer to input data
 * \param input_len Input data length
 *
 * \retval 0 On failure
 * \retval string length On Success
 */
int decode_base64_ext(char *plain_text, const char *input, int input_len)
{
    const char *encoded = input;
    int i = 0, j = 0, k = 0;
    int ch = 0;

    while ((ch = *encoded++) != '\0' && input_len-- > 0) {
        if (ch == b64_pad) {
            if (*encoded != '=' && (i % 4) == 1) {
                return 0;
            }
            continue;
        }

        ch = b64_reverse_t[ch];
        if (ch < 0 || ch == -1) {
            continue;
        } else if (ch == -2) {
            return 0;
        }
        switch(i % 4) {
            case 0:
                plain_text[j] = ch << 2;
                break;
            case 1:
                plain_text[j++] |= ch >> 4;
                plain_text[j] = (ch & 0x0f) << 4;
                break;
            case 2:
                plain_text[j++] |= ch >>2;
                plain_text[j] = (ch & 0x03) << 6;
                break;
            case 3:
                plain_text[j++] |= ch;
                break;
        }
        i++;
    }

    k = j;
    if (ch == b64_pad) {
        switch(i % 4) {
            case 1:
                return 0;
            case 2:
                k++;
            case 3:
                plain_text[k] = 0;
        }
    }

    plain_text[j] = '\0';

    return j;
}

/* \brief Convert const char to int
 *
 * \param c number string
 *
 * \retval n The converted number
 */
int convert_to_int(const char c)
{
    int n;
    if ((c>='0') && (c<='9'))
        n = c - '0';
    else if ((c>='A') && (c<='F'))
        n = c - 'A' + 10;
    else if ((c>='a') && (c<='f'))
        n = c - 'a' + 10;
    else
        n = 0;
    return n;
}

/* \brief Set a match to tx.N
 *
 * \param msr Pointer to modsec resource
 * \param capture If ON match will be saved
 * \param match Pointer to captured string
 *\parm tx_n The tx number to save the data
 *
 * \retval 0 On Sucess|Fail
 */
int set_match_to_tx(modsec_rec *msr, int capture, const char *match, int tx_n)  {

    if (capture) {
        msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

        if (s == NULL) return -1;

        s->name = apr_psprintf(msr->mp,"%d", tx_n);
        s->name_len = strlen(s->name);
        s->value = apr_pstrdup(msr->mp, match);
        if (s->value == NULL) return -1;
        s->value_len = strlen(s->value);
        apr_table_setn(msr->tx_vars, s->name, (void *)s);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Added phrase match to TX.%d: %s",
                    tx_n, log_escape_nq_ex(msr->mp, s->value, s->value_len));
        }

        /*
           for(i = 0; i <= 9; i++) {
           char buf[2];
           apr_snprintf(buf, sizeof(buf), "%d", i);
           apr_table_unset(msr->tx_vars, buf);
           }
         */
    }

    return 0;
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
    static const unsigned char b2hex[] = "0123456789abcdef";
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
static unsigned char x2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}

/**
 * Converts a single hexadecimal digit into a decimal value.
 */
static unsigned char xsingle2c(unsigned char *what) {
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
int msc_mkstemp_ex(char *template, int mode) {
    int fd = -1;

    /* ENH Use apr_file_mktemp instead. */

#if !(defined(WIN32)||defined(NETWARE))
    fd = mkstemp(template);
#ifdef HAVE_FCHMOD
    if ((fd != -1) && (mode != 0)) {
        if (fchmod(fd, mode) == -1) {
            return -1;
        }
    }
#endif /* HAVE_FCHMOD */
#else
    if (mktemp(template) == NULL) return -1;
    fd = open(template, O_WRONLY | O_APPEND | O_CREAT | O_BINARY, mode);
#endif /* !(defined(WIN32)||defined(NETWARE)) */

    return fd;
}

/**
 *
 */
int msc_mkstemp(char *template) {
    return msc_mkstemp_ex(template, CREATEMODE_UNISTD);
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
static unsigned char *c2x(unsigned what, unsigned char *where) {
    static const char c2x_table[] = "0123456789abcdef";

    what = what & 0xff;
    *where++ = c2x_table[what >> 4];
    *where++ = c2x_table[what & 0x0f];

    return where;
}

static char *_log_escape(apr_pool_t *p, const unsigned char *input,
        unsigned long int input_length, int escape_quotes, int escape_colon, int escape_re);

char *log_escape_re(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 1, 1, 1);
}

char *log_escape(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 1, 0, 0);
}

char *log_escape_nq(apr_pool_t *mp, const char *text) {
    return _log_escape(mp, (const unsigned char *)text, text ? strlen(text) : 0, 0, 0, 0);
}

char *log_escape_ex(apr_pool_t *mp, const char *text, unsigned long int text_length) {
    return _log_escape(mp, (const unsigned char *)text, text_length, 1, 0, 0);
}

char *log_escape_nq_ex(apr_pool_t *mp, const char *text, unsigned long int text_length) {
    return _log_escape(mp, (const unsigned char *)text, text_length, 0, 0, 0);
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

char *log_escape_nul(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length) {
    unsigned char *ret = apr_palloc(mp, text_length * 4 + 1);
    unsigned long int i, j;

    for (i = 0, j = 0; i < text_length; i++) {
        if (text[i] == '\0') {
            ret[j] = '\\';
            ret[j+1] = 'x';
            c2x(text[i], ret+j+2);
            j += 4;
        }
        else {
            ret[j] = text[i];
            j++;
        }
    }
    ret[j] = '\0';

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
static char *_log_escape(apr_pool_t *mp, const unsigned char *input, unsigned long int input_len,
        int escape_quotes, int escape_colon, int escape_re)
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
            case '+' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '+';
                } else {
                    *d++ = input[i];
                }
                break;
            case '.' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '.';
                } else {
                    *d++ = input[i];
                }
                break;
            case ']' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = ']';
                } else {
                    *d++ = input[i];
                }
                break;
            case '[' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '[';
                } else {
                    *d++ = input[i];
                }
                break;
            case '(' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '(';
                } else {
                    *d++ = input[i];
                }
                break;
            case ')' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = ')';
                } else {
                    *d++ = input[i];
                }
                break;
            case '?' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '?';
                } else {
                    *d++ = input[i];
                }
                break;
            case '/' :
                if (escape_re) {
                    *d++ = '\\';
                    *d++ = '/';
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
                    *d++ = (unsigned char)strtol(buf, NULL, 8);
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
    long int i, count, fact, j, xv;
    int Code, hmap = -1;

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

                        Code = 0;
                        fact = 1;

                        if (unicode_map_table != NULL && unicode_codepage > 0)  {

                            for(j=5; j>=2; j--)   {
                                if (isxdigit((input[i+j])))  {
                                    if ((input[i+j])>=97)    {
                                        xv = ( (input[i+j]) - 97) + 10;
                                    } else if ( (input[i+j]) >= 65)  {
                                        xv = ((input[i+j]) - 65) + 10;
                                    } else {
                                        xv = (input[i+j]) - 48;
                                    }
                                    Code += (xv * fact);
                                    fact *= 16;
                                }
                            }

                            if(Code >= 0 && Code <= 65535)  {
                                hmap = unicode_map_table[Code];
                            }
                        }

                        if(hmap != -1)  {
                            *d = hmap;
                        } else {
                            /* We first make use of the lower byte here, ignoring the higher byte. */
                            *d = x2c(&input[i + 4]);

                            /* Full width ASCII (ff01 - ff5e) needs 0x20 added */
                            if (   (*d > 0x00) && (*d < 0x5f)
                                    && ((input[i + 2] == 'f') || (input[i + 2] == 'F'))
                                    && ((input[i + 3] == 'f') || (input[i + 3] == 'F')))
                            {
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
int normalize_path_inplace(unsigned char *input, int input_len, int win, int *changed) {
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
        }

        /* Skip normalization if this is NOT the end of the path segment. */
        else if (*(src + 1) != '/') {
            goto copy; /* Skip normalization. */
        }

        /*** Normalize the path segment. ***/

        /* Could it be an empty path segment? */
        if ((src != end) && *src == '/') {
            /* Ignore */
            *changed = 1;
            goto copy; /* Copy will take care of this. */
        }

        /* Could it be a back or self reference? */
        else if (*src == '.') {

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
            }

            /* Relative Self-reference? */
            else if (dst == input) {
                *changed = 1;

                /* Ignore. */

                if (done) goto length; /* Skip the copy. */
                src++;
            }

            /* Self-reference? */
            else if (*(dst - 1) == '/') {
                *changed = 1;

                /* Ignore. */

                if (done) goto length; /* Skip the copy. */
                dst--;
                src++;
            }
        }

        /* Found a regular path segment. */
        else if (dst > input) {
            hitroot = 0;
        }

copy:
        /*** Copy the byte if required. ***/

        /* Skip to the last forward slash when multiple are used. */
        if (*src == '/') {
            unsigned char *oldsrc = src;

            while (   (src < end)
                    && ((*(src + 1) == '/') || (win && (*(src + 1) == '\\'))) )
            {
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

/**
 * Translate UNIX octal umask/mode to APR apr_fileperms_t
 */
apr_fileperms_t mode2fileperms(int mode) {
    apr_fileperms_t perms = 0;

    if (mode & S_IXOTH) perms |= APR_WEXECUTE;
    if (mode & S_IWOTH) perms |= APR_WWRITE;
    if (mode & S_IROTH) perms |= APR_WREAD;
    if (mode & S_IXGRP) perms |= APR_GEXECUTE;
    if (mode & S_IWGRP) perms |= APR_GWRITE;
    if (mode & S_IRGRP) perms |= APR_GREAD;
    if (mode & S_IXUSR) perms |= APR_UEXECUTE;
    if (mode & S_IWUSR) perms |= APR_UWRITE;
    if (mode & S_IRUSR) perms |= APR_UREAD;
    if (mode & S_ISVTX) perms |= APR_WSTICKY;
    if (mode & S_ISGID) perms |= APR_GSETID;
    if (mode & S_ISUID) perms |= APR_USETID;

    return perms;
}

/**
 * Generate a single variable.
 */
char *construct_single_var(modsec_rec *msr, char *name) {
    char *varname = NULL;
    char *param = NULL;
    msre_var *var = NULL;
    msre_var *vx = NULL;
    char *my_error_msg = NULL;

    /* Extract variable name and its parameter from the script. */
    varname = apr_pstrdup(msr->mp, name);
    param = strchr(varname, '.');
    if (param != NULL) {
        *param = '\0';
        param++;
    }

    /* Resolve variable. */
    var = msre_create_var_ex(msr->mp, msr->modsecurity->msre,
            varname, param, msr, &my_error_msg);
    if (var == NULL) return NULL;

    /* Generate variable. */
    vx = generate_single_var(msr, var, NULL, NULL, msr->msc_rule_mptmp);
    if (vx == NULL) return NULL;

    return (char *)vx->value;
}
