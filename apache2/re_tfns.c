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

#include <ctype.h>

#include "apr_md5.h"
#include "apr_sha1.h"
#include "apr_base64.h"

#include "msc_unicode.h"
#include "re.h"
#include "msc_util.h"

/* cmdline */

/**
* \brief cmdline transformation function
*
* \param mptmp Pointer to resource pool
* \param input Pointer to input data
* \param input_len Input data length
* \param rval Pointer to decoded buffer
* \param rval_len Decoded buffer length
*
* \retval 0 On failure
* \retval 1 On Success
*/
static int msre_fn_cmdline_execute(apr_pool_t *mptmp, unsigned char *input,
        long int input_len, char **rval, long int *rval_len)
{
    int space = 0;
    unsigned char *s = input;

    if (rval == NULL) return -1;

    *rval = (char *)input;
    /* Check characters */
    for ( ; *input; input++ ) {
        switch(*input) {
            /* remove some characters */
            case '"':
            case '\'':
            case '\\':
            case '^':
                continue;
                /* replace some characters to space (only one) */
            case ' ':
            case ',':
            case ';':
            case '\t':
            case '\r':
            case '\n':
                if (!space) {
                    *s++ = ' ';
                    space++;
                }
                break;
            case '/':
            case '(':
                /* remove space before / or ( */
                if (space) s--;
                space = 0;
                *s++ = *input;
                break;
                /* copy normal characters */
            default :
                *s++ = tolower(*input);
                space = 0;
        }
    }

    *s = 0;
    *rval_len = strlen(*rval);
    return 1;
}

/* lowercase */

static int msre_fn_lowercase_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;
    int changed = 0;

    if (rval == NULL) return -1;
    *rval = NULL;

    i = 0;
    while(i < input_len) {
        int x = input[i];
        input[i] = tolower(x);
        if (x != input[i]) changed = 1;
        i++;
    }

    *rval = (char *)input;
    *rval_len = input_len;

    return changed;
}

/* trimLeft */

static int msre_fn_trimLeft_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;

    *rval = (char *)input;
    for (i = 0; i < input_len; i++) {
        if (isspace(**rval) == 0) {
            break;
        }
        (*rval)++;
    }

    *rval_len = input_len - i;

    return (*rval_len == input_len ? 0 : 1);
}

/* trimRight */

static int msre_fn_trimRight_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;

    *rval = (char *)input;
    for (i = input_len - 1; i >= 0; i--) {
        if (isspace((*rval)[i]) == 0) {
            break;
        }
        (*rval)[i] = '\0';
    }

    *rval_len = i + 1;

    return (*rval_len == input_len ? 0 : 1);
}

/* trim */

static int msre_fn_trim_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    int rc = 0;

    rc = msre_fn_trimLeft_execute(mptmp, input, input_len, rval, rval_len);
    if (rc == 1) {
        rc = msre_fn_trimRight_execute(mptmp, (unsigned char *)*rval, *rval_len, rval, rval_len);
    }
    else {
        rc = msre_fn_trimRight_execute(mptmp, input, input_len, rval, rval_len);
    }

    return (*rval_len == input_len ? 0 : 1);
}

/* removeNulls */

static int msre_fn_removeNulls_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i, j;
    int changed = 0;

    i = j = 0;
    while(i < input_len) {
        if (input[i] == '\0') {
            changed = 1;
        } else {
            input[j] = input[i];
            j++;
        }
        i++;
    }

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* replaceNulls */

static int msre_fn_replaceNulls_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;
    int changed = 0;

    if (rval == NULL) return -1;
    *rval = NULL;

    i = 0;
    while(i < input_len) {
        if (input[i] == '\0') {
            changed = 1;
            input[i] = ' ';
        }
        i++;
    }

    *rval = (char *)input;
    *rval_len = input_len;

    return changed;
}

/* compressWhitespace */

static int msre_fn_compressWhitespace_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i, j, count;
    int changed = 0;
    int inwhitespace = 0;

    i = j = count = 0;
    while(i < input_len) {
        if (isspace(input[i])||(input[i] == NBSP)) {
            if (inwhitespace) changed = 1;
            inwhitespace = 1;
            count++;
        } else {
            inwhitespace = 0;
            if (count) {
                input[j] = ' ';
                count = 0;
                j++;
            }
            input[j] = input[i];
            j++;
        }
        i++;
    }

    if (count) {
        input[j] = ' ';
        j++;
    }

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* cssDecode */

static int msre_fn_cssDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;

    length = css_decode_inplace(input, input_len);
    *rval = (char *)input;
    *rval_len = length;

    return (*rval_len == input_len ? 0 : 1);
}

/* removeWhitespace */

static int msre_fn_removeWhitespace_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i, j;
    int changed = 0;

    i = j = 0;
    while(i < input_len) {
        if (isspace(input[i])||(input[i] == NBSP)) {
            /* do nothing */
            changed = 1;
        } else {
            input[j] = input[i];
            j++;
        }
        i++;
    }

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* removeCommentsChar */

static int msre_fn_removeCommentsChar_execute(apr_pool_t *mptmp, unsigned char *input,
        long int input_len, char **rval, long int *rval_len)
{
    long int i, j;
    int changed = 0;

    i = j = 0;
    while(i < input_len) {
        if ((input[i] == '/')&&(i + 1 < input_len)&&(input[i + 1] == '*')) {
            changed = 1;
            i += 2;
        } else if ((input[i] == '*')&&(i + 1 < input_len)&&(input[i + 1] == '/')) {
            changed = 1;
            i += 2;
        } else if ((input[i] == '<')&&(i + 1 < input_len)&&(input[i + 1] == '!')&&
                    (i + 2 < input_len)&&(input[i+2] == '-')&&(i + 3 < input_len)&&
                    (input[i + 3] == '-')) {
            changed = 1;
            i += 4;
        } else if ((input[i] == '-')&&(i + 1 < input_len)&&(input[i + 1] == '-')&&
                    (i + 2 < input_len)&&(input[i+2] == '>'))   {
            changed = 1;
            i += 3;
        } else if ((input[i] == '-')&&(i + 1 < input_len)&&(input[i + 1] == '-')) {
            changed = 1;
            i += 2;
        } else if (input[i] == '#') {
            changed = 1;
            i++;
        } else {
            input[j] = input[i];
            i++;
            j++;
        }
    }
    input[j] = '\0';

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* removeComments */

static int msre_fn_removeComments_execute(apr_pool_t *mptmp, unsigned char *input,
        long int input_len, char **rval, long int *rval_len)
{
    long int i, j, incomment;
    int changed = 0;

    i = j = incomment = 0;
    while(i < input_len) {
        if (incomment == 0) {
            if ((input[i] == '/')&&(i + 1 < input_len)&&(input[i + 1] == '*')) {
                changed = 1;
                incomment = 1;
                i += 2;
            } else if ((input[i] == '<')&&(i + 1 < input_len)&&(input[i + 1] == '!')&&
                    (i + 2 < input_len)&&(input[i+2] == '-')&&(i + 3 < input_len)&&
                    (input[i + 3] == '-') && (incomment == 0)) {
                incomment = 1;
                changed = 1;
                i += 4;
            } else if ((input[i] == '-')&&(i + 1 < input_len)&&(input[i + 1] == '-')
                        && (incomment == 0)) {
                changed = 1;
                input[i] = ' ';
                break;
            } else if (input[i] == '#' && (incomment == 0)) {
                changed = 1;
                input[i] = ' ';
               break;
            } else {
                input[j] = input[i];
                i++;
                j++;
            }
        } else {
            if ((input[i] == '*')&&(i + 1 < input_len)&&(input[i + 1] == '/')) {
                incomment = 0;
                i += 2;
                input[j] = input[i];
                i++;
                j++;
            } else if ((input[i] == '-')&&(i + 1 < input_len)&&(input[i + 1] == '-')&&
                    (i + 2 < input_len)&&(input[i+2] == '>'))   {
                incomment = 0;
                i += 3;
                input[j] = input[i];
                i++;
                j++;
            } else {
                i++;
            }
        }
    }

    if (incomment) {
        input[j++] = ' ';
    }

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* replaceComments */

static int msre_fn_replaceComments_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i, j, incomment;
    int changed = 0;

    i = j = incomment = 0;
    while(i < input_len) {
        if (incomment == 0) {
            if ((input[i] == '/')&&(i + 1 < input_len)&&(input[i + 1] == '*')) {
                changed = 1;
                incomment = 1;
                i += 2;
            } else {
                input[j] = input[i];
                i++;
                j++;
            }
        } else {
            if ((input[i] == '*')&&(i + 1 < input_len)&&(input[i + 1] == '/')) {
                incomment = 0;
                i += 2;
                input[j] = ' ';
                j++;
            } else {
                i++;
            }
        }
    }

    if (incomment) {
        input[j++] = ' ';
    }

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* jsDecode */

static int msre_fn_jsDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;

    length = js_decode_nonstrict_inplace(input, input_len);
    *rval = (char *)input;
    *rval_len = length;

    return (*rval_len == input_len ? 0 : 1);
}

/* urlDecode */

static int msre_fn_urlDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;
    int invalid_count;
    int changed;

    length = urldecode_nonstrict_inplace_ex(input, input_len, &invalid_count, &changed);
    *rval = (char *)input;
    *rval_len = length;

    return changed;
}

/* urlDecodeUni */

static int msre_fn_urlDecodeUni_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;
    int changed;

    length = urldecode_uni_nonstrict_inplace_ex(input, input_len, &changed);
    *rval = (char *)input;
    *rval_len = length;

    return changed;
}

static int msre_fn_utf8Unicode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    int changed = 0;

    *rval = (char *)utf8_unicode_inplace_ex(mptmp, input, input_len, &changed);
    *rval_len = strlen(*rval);

    return changed;
}


/* urlEncode */

static int msre_fn_urlEncode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    int changed;

    *rval = url_encode(mptmp, (char *)input, input_len, &changed);
    *rval_len = strlen(*rval);

    return changed;
}

/* base64Encode */

static int msre_fn_base64Encode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = apr_base64_encode_len(input_len); /* returns len with NULL byte included */
    *rval = apr_palloc(mptmp, *rval_len);
    apr_base64_encode(*rval, (const char *)input, input_len);
    (*rval_len)--;

    return *rval_len ? 1 : 0;
}

/* base64Decode */

static int msre_fn_base64Decode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = apr_base64_decode_len((const char *)input); /* returns len with NULL byte included */
    *rval = apr_palloc(mptmp, *rval_len);
    *rval_len = apr_base64_decode(*rval, (const char *)input);

    return *rval_len ? 1 : 0;
}

/* length */

static int msre_fn_length_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval = apr_psprintf(mptmp, "%ld", input_len);
    *rval_len = strlen(*rval);

    return 1;
}

/* md5 */

static int msre_fn_md5_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    unsigned char digest[APR_MD5_DIGESTSIZE];

    apr_md5(digest, input, input_len);

    *rval_len = APR_MD5_DIGESTSIZE;
    *rval = apr_pstrmemdup(mptmp, (const char *)digest, APR_MD5_DIGESTSIZE);

    return 1;
}

/* sha1 */

static int msre_fn_sha1_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    unsigned char digest[APR_SHA1_DIGESTSIZE];
    apr_sha1_ctx_t context;

    apr_sha1_init(&context);
    apr_sha1_update(&context, (const char *)input, input_len);
    apr_sha1_final(digest, &context);

    *rval_len = APR_SHA1_DIGESTSIZE;
    *rval = apr_pstrmemdup(mptmp, (const char *)digest, APR_SHA1_DIGESTSIZE);

    return 1;
}

/**
* \brief SqlHexDecode transformation function. Transform xNN data.
*
* \param mptmp Pointer to resource pool
* \param input Pointer to input data
* \param input_len Input data length
* \param rval Pointer to decoded buffer
* \param rval_len Decoded buffer length
*
* \retval 0 On failure
* \retval 1 On Success
*/
static int msre_fn_sqlHexDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = sql_hex2bytes_inplace(input, input_len);
    *rval = (char *)input;

    return 1;
}

/* hexDecode */

static int msre_fn_hexDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = hex2bytes_inplace(input, input_len);
    *rval = (char *)input;

    return 1;
}

/* hexEncode */

static int msre_fn_hexEncode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval = bytes2hex(mptmp, input, input_len);
    *rval_len = strlen(*rval);

    return 1;
}

/* htmlEntityDecode */

static int msre_fn_htmlEntityDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = html_entities_decode_inplace(mptmp, input, input_len);
    *rval = (char *)input;

    return (*rval_len == input_len ? 0 : 1);
}

/* escapeSeqDecode */

static int msre_fn_escapeSeqDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = ansi_c_sequences_decode_inplace(input, input_len);
    *rval = (char *)input;

    return (*rval_len == input_len ? 0 : 1);
}

/* normalizePath */

static int msre_fn_normalizePath_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    int changed;

    *rval_len = normalize_path_inplace(input, input_len, 0, &changed);
    *rval = (char *)input;

    return changed;
}

/* normalizePathWin */

static int msre_fn_normalizePathWin_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    int changed;

    *rval_len = normalize_path_inplace(input, input_len, 1, &changed);
    *rval = (char *)input;

    return changed;
}

/* parityEven7bit */

static int msre_fn_parityEven7bit_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;
    int changed = 0;

    if (rval == NULL) return -1;
    *rval = NULL;

    i = 0;
    while(i < input_len) {
        unsigned int x = input[i];

        input[i] ^= input[i] >> 4;
        input[i] &= 0xf;

        if ((0x6996 >> input[i]) & 1) {
            input[i] = x | 0x80;
        }
        else {
            input[i] = x & 0x7f;
        }

        if (x != input[i]) changed = 1;
        i++;
    }

    *rval = (char *)input;
    *rval_len = input_len;

    return changed;
}

/* parityZero7bit */

static int msre_fn_parityZero7bit_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;
    int changed = 0;

    if (rval == NULL) return -1;
    *rval = NULL;

    i = 0;
    while(i < input_len) {
        unsigned char c = input[i];
        input[i] &= 0x7f;
        if (c != input[i]) changed = 1;
        i++;
    }

    *rval = (char *)input;
    *rval_len = input_len;

    return changed;
}

/* parityOdd7bit */

static int msre_fn_parityOdd7bit_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int i;
    int changed = 0;

    if (rval == NULL) return -1;
    *rval = NULL;

    i = 0;
    while(i < input_len) {
        unsigned int x = input[i];

        input[i] ^= input[i] >> 4;
        input[i] &= 0xf;

        if ((0x6996 >> input[i]) & 1) {
            input[i] = x & 0x7f;
        }
        else {
            input[i] = x | 0x80;
        }

        if (x != input[i]) changed = 1;
        i++;
    }

    *rval = (char *)input;
    *rval_len = input_len;

    return changed;
}

/**
* \brief Base64 transformation function based on RFC2045
*
* \param mptmp Pointer to resource pool
* \param input Pointer to input data
* \param input_len Input data length
* \param rval Pointer to decoded buffer
* \param rval_len Decoded buffer length
*
* \retval 0 On failure
* \retval 1 On Success
*/
static int msre_fn_base64DecodeExt_execute(apr_pool_t *mptmp, unsigned char *input, long int input_len, char **rval, long int *rval_len)
{
    *rval_len = input_len;
    *rval = apr_palloc(mptmp, *rval_len);
    *rval_len = decode_base64_ext(*rval, (const unsigned char *)input, input_len);

    return *rval_len ? 1 : 0;
}


/* ------------------------------------------------------------------------------ */

/**
 * Registers one transformation function with the engine.
 */
void msre_engine_tfn_register(msre_engine *engine, const char *name,
    fn_tfn_execute_t execute)
{
    msre_tfn_metadata *metadata = (msre_tfn_metadata *)apr_pcalloc(engine->mp,
        sizeof(msre_tfn_metadata));
    if (metadata == NULL) return;

    metadata->name = name;
    metadata->execute = execute;

    apr_table_setn(engine->tfns, name, (void *)metadata);
}

/**
 * Returns transformation function metadata given a name.
 */
msre_tfn_metadata *msre_engine_tfn_resolve(msre_engine *engine, const char *name) {
    return (msre_tfn_metadata *)apr_table_get(engine->tfns, name);
}

/**
 * Register the default transformation functions.
 */
void msre_engine_register_default_tfns(msre_engine *engine) {

    /* none */
    msre_engine_tfn_register(engine,
        "none",
        NULL
    );

    /* base64Decode */
    msre_engine_tfn_register(engine,
        "base64Decode",
        msre_fn_base64Decode_execute
    );

    /* base64Encode */
    msre_engine_tfn_register(engine,
        "base64Encode",
        msre_fn_base64Encode_execute
    );

    /* compressWhitespace */
    msre_engine_tfn_register(engine,
        "compressWhitespace",
        msre_fn_compressWhitespace_execute
    );

    /* cssDecode */
    msre_engine_tfn_register(engine,
        "cssDecode",
        msre_fn_cssDecode_execute
    );

    /* escapeSeqDecode */
    msre_engine_tfn_register(engine,
        "escapeSeqDecode",
        msre_fn_escapeSeqDecode_execute
    );

    /* sqlHexDecode */
    msre_engine_tfn_register(engine,
        "sqlHexDecode",
        msre_fn_sqlHexDecode_execute
    );

    /* hexDecode */
    msre_engine_tfn_register(engine,
        "hexDecode",
        msre_fn_hexDecode_execute
    );

    /* hexEncode */
    msre_engine_tfn_register(engine,
        "hexEncode",
        msre_fn_hexEncode_execute
    );

    /* htmlEntityDecode */
    msre_engine_tfn_register(engine,
        "htmlEntityDecode",
        msre_fn_htmlEntityDecode_execute
    );

    /* jsDecode */
    msre_engine_tfn_register(engine,
        "jsDecode",
        msre_fn_jsDecode_execute
    );

    /* length */
    msre_engine_tfn_register(engine,
        "length",
        msre_fn_length_execute
    );

    /* lowercase */
    msre_engine_tfn_register(engine,
        "lowercase",
        msre_fn_lowercase_execute
    );

    /* md5 */
    msre_engine_tfn_register(engine,
        "md5",
        msre_fn_md5_execute
    );

    /* normalisePath */
    msre_engine_tfn_register(engine,
        "normalisePath",
        msre_fn_normalizePath_execute
    );
    
    /* normalizePath */
    msre_engine_tfn_register(engine,
        "normalizePath",
        msre_fn_normalizePath_execute
    );

    /* normalisePathWin */
    msre_engine_tfn_register(engine,
        "normalisePathWin",
        msre_fn_normalizePathWin_execute
    );
    
    /* normalizePathWin */
    msre_engine_tfn_register(engine,
        "normalizePathWin",
        msre_fn_normalizePathWin_execute
    );

    /* parityEven7bit */
    msre_engine_tfn_register(engine,
        "parityEven7bit",
        msre_fn_parityEven7bit_execute
    );

    /* parityZero7bit */
    msre_engine_tfn_register(engine,
        "parityZero7bit",
        msre_fn_parityZero7bit_execute
    );

    /* parityOdd7bit */
    msre_engine_tfn_register(engine,
        "parityOdd7bit",
        msre_fn_parityOdd7bit_execute
    );

    /* removeWhitespace */
    msre_engine_tfn_register(engine,
        "removeWhitespace",
        msre_fn_removeWhitespace_execute
    );

    /* removeNulls */
    msre_engine_tfn_register(engine,
        "removeNulls",
        msre_fn_removeNulls_execute
    );

    /* replaceNulls */
    msre_engine_tfn_register(engine,
        "replaceNulls",
        msre_fn_replaceNulls_execute
    );

    /* removeComments */
    msre_engine_tfn_register(engine,
        "removeComments",
        msre_fn_removeComments_execute
    );

    /* removeCommentsChar */
    msre_engine_tfn_register(engine,
        "removeCommentsChar",
        msre_fn_removeCommentsChar_execute
    );

    /* replaceComments */
    msre_engine_tfn_register(engine,
        "replaceComments",
        msre_fn_replaceComments_execute
    );

    /* sha1 */
    msre_engine_tfn_register(engine,
        "sha1",
        msre_fn_sha1_execute
    );

    /* trim */
    msre_engine_tfn_register(engine,
        "trim",
        msre_fn_trim_execute
    );

    /* trimLeft */
    msre_engine_tfn_register(engine,
        "trimLeft",
        msre_fn_trimLeft_execute
    );

    msre_engine_tfn_register(engine,
        "cmdline",
        msre_fn_cmdline_execute
    );

    /* trimRight */
    msre_engine_tfn_register(engine,
        "trimRight",
        msre_fn_trimRight_execute
    );

    /* urlDecode */
    msre_engine_tfn_register(engine,
        "urlDecode",
        msre_fn_urlDecode_execute
    );

    /* urlDecodeUni */
    msre_engine_tfn_register(engine,
        "urlDecodeUni",
        msre_fn_urlDecodeUni_execute
    );

    /* Utf8Unicode */
    msre_engine_tfn_register(engine,
        "Utf8toUnicode",
        msre_fn_utf8Unicode_execute
    );

    /* urlEncode */
    msre_engine_tfn_register(engine,
        "urlEncode",
        msre_fn_urlEncode_execute
    );

    /* base64DecodeExt */
    msre_engine_tfn_register(engine,
        "base64DecodeExt",
        msre_fn_base64DecodeExt_execute
    );

}
