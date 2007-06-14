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
#include <ctype.h>

#include "apr_md5.h"
#include "apr_sha1.h"
#include "apr_base64.h"

#include "re.h"
#include "msc_util.h"

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

    i = j = count = 0;
    while(i < input_len) {
        if (isspace(input[i])||(input[i] == NBSP)) {
            changed = 1;
            count++;
        } else {
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

    *rval = (char *)input;
    *rval_len = j;

    return changed;
}

/* urlDecode */

static int msre_fn_urlDecode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;
    int invalid_count;

    length = urldecode_nonstrict_inplace_ex(input, input_len, &invalid_count);
    *rval = (char *)input;
    *rval_len = length;
    
    return (*rval_len == input_len ? 0 : 1);
}

/* urlDecodeUni */

static int msre_fn_urlDecodeUni_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    long int length;

    length = urldecode_uni_nonstrict_inplace_ex(input, input_len);
    *rval = (char *)input;
    *rval_len = length;
    
    return (*rval_len == input_len ? 0 : 1);
}

/* urlEncode */

static int msre_fn_urlEncode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval = url_encode(mptmp, (char *)input, input_len);
    *rval_len = strlen(*rval);
    
    return (*rval_len == input_len ? 0 : 1);
}

/* base64Encode */

static int msre_fn_base64Encode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = apr_base64_encode_len(input_len); /* returns len with NULL byte included */
    *rval = apr_palloc(mptmp, *rval_len);
    apr_base64_encode(*rval, (const char *)input, input_len);
    (*rval_len)--;

    return 1;
}

/* base64Decode */

static int msre_fn_base64Decode_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = apr_base64_decode_len((const char *)input); /* returns len with NULL byte included */
    *rval = apr_palloc(mptmp, *rval_len);
    apr_base64_decode(*rval, (const char *)input);
    (*rval_len)--;

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

/* normalisePath */

static int msre_fn_normalisePath_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = normalise_path_inplace(input, input_len, 0);
    *rval = (char *)input;

    return (*rval_len == input_len ? 0 : 1);
}

/* normalisePathWin */

static int msre_fn_normalisePathWin_execute(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    *rval_len = normalise_path_inplace(input, input_len, 1);
    *rval = (char *)input;

    return (*rval_len == input_len ? 0 : 1);
}

/* ------------------------------------------------------------------------------ */

/**
 * Registers one transformation function with the engine.
 */
void msre_engine_tfn_register(msre_engine *engine, const char *name,
    FN_TFN_EXECUTE(execute))
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

    /* escapeSeqDecode */
    msre_engine_tfn_register(engine,
        "escapeSeqDecode",
        msre_fn_escapeSeqDecode_execute
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
        msre_fn_normalisePath_execute
    );

    /* normalisePathWin */
    msre_engine_tfn_register(engine,
        "normalisePathWin",
        msre_fn_normalisePathWin_execute
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

    /* urlEncode */
    msre_engine_tfn_register(engine,
        "urlEncode",
        msre_fn_urlEncode_execute
    );
}
