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

#include "msc_json.h"

#ifdef WITH_YAJL

int json_add_argument(modsec_rec *msr, const char *value, unsigned length)
{
    msc_arg *arg = (msc_arg *) NULL;

    /**
     * If we do not have a prefix, we cannot create a variable name
     * to reference this argument; for now we simply ignore these
     */
    if (!msr->json->current_key) {
        msr_log(msr, 3, "Cannot add scalar value without an associated key");
        return 1;
    }

    arg = (msc_arg *) apr_pcalloc(msr->mp, sizeof(msc_arg));

    /**
     * Argument name is 'prefix + current_key'
     */
    if (msr->json->prefix) {
        arg->name = apr_psprintf(msr->mp, "%s.%s", msr->json->prefix,
            msr->json->current_key);
    }
    else {
        arg->name = apr_psprintf(msr->mp, "%s", msr->json->current_key);
    }
    arg->name_len = strlen(arg->name);

    /**
     * Argument value is copied from the provided string
     */
    arg->value = apr_pstrmemdup(msr->mp, value, length);
    arg->value_len = length;
    arg->origin = "JSON";

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Adding JSON argument '%s' with value '%s'",
            arg->name, arg->value);
    }

    apr_table_addn(msr->arguments,
        log_escape_nq_ex(msr->mp, arg->name, arg->name_len), (void *) arg);

    return 1;
}

/**
 * yajl callback functions
 * For more information on the function signatures and order, check
 * http://lloyd.github.com/yajl/yajl-1.0.12/structyajl__callbacks.html
 */

/**
 * Callback for hash key values; we use those to define the variable names
 * under ARGS. Whenever we reach a new key, we update the current key value.
 */
static int yajl_map_key(void *ctx, const unsigned char *key, size_t length)
{
    modsec_rec *msr = (modsec_rec *) ctx;
    unsigned char *safe_key = (unsigned char *) NULL;

    /**
     * yajl does not provide us with null-terminated strings, but
     * rather expects us to copy the data from the key up to the
     * length informed; we create a standalone null-termined copy
     * in safe_key
     */
    safe_key = apr_pstrndup(msr->mp, key, length);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "New JSON hash key '%s'", safe_key);
    }

    /**
     * TODO: How do we free the previously string value stored here?
     */
    msr->json->current_key = safe_key;

    return 1;
}

/**
 * Callback for null values
 *
 * TODO: Is there a way to define true null parameter values instead of
 * empty values?
 */
static int yajl_null(void *ctx)
{
    modsec_rec *msr = (modsec_rec *) ctx;

    return json_add_argument(msr, "", 0);
}

/**
 * Callback for boolean values
 */
static int yajl_boolean(void *ctx, int value)
{
    modsec_rec *msr = (modsec_rec *) ctx;

    if (value) {
        return json_add_argument(msr, "true", strlen("true"));
    }
    else {
        return json_add_argument(msr, "false", strlen("false"));
    }
}

/**
 * Callback for string values
 */
static int yajl_string(void *ctx, const unsigned char *value, size_t length)
{
    modsec_rec *msr = (modsec_rec *) ctx;

    return json_add_argument(msr, value, length);
}

/**
 * Callback for numbers; YAJL can use separate callbacks for integers/longs and
 * float/double values, but since we are not interested in using the numeric
 * values here, we use a generic handler which uses numeric strings
 */
static int yajl_number(void *ctx, const char *value, size_t length)
{
    modsec_rec *msr = (modsec_rec *) ctx;

    return json_add_argument(msr, value, length);
}

/**
 * Callback for a new hash, which indicates a new subtree, labeled as the current
 * argument name, is being created
 */
static int yajl_start_map(void *ctx)
{
    modsec_rec *msr = (modsec_rec *) ctx;

    /**
     * If we do not have a current_key, this is a top-level hash, so we do not
     * need to do anything
     */
    if (!msr->json->current_key) return 1;

    /**
     * Check if we are already inside a hash context, and append or create the
     * current key name accordingly
     */
    if (msr->json->prefix) {
        msr->json->prefix = apr_psprintf(msr->mp, "%s.%s", msr->json->prefix,
            msr->json->current_key);
    }
    else {
        msr->json->prefix = apr_pstrdup(msr->mp, msr->json->current_key);
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "New JSON hash context (prefix '%s')", msr->json->prefix);
    }

    return 1;
}

/**
 * Callback for end hash, meaning the current subtree is being closed, and that
 * we should go back to the parent variable label
 */
static int yajl_end_map(void *ctx)
{
    modsec_rec *msr = (modsec_rec *) ctx;
    unsigned char *separator = (unsigned char *) NULL;

    /**
     * If we have no prefix, then this is the end of a top-level hash and
     * we don't do anything
     */
    if (msr->json->prefix == NULL) return 1;

    /**
     * Current prefix might or not include a separator character; top-level
     * hash keys do not have separators in the variable name
     */
    separator = strrchr(msr->json->prefix, '.');

    if (separator) {
        msr->json->prefix = apr_pstrmemdup(msr->mp, msr->json->prefix,
            separator - msr->json->prefix);
        msr->json->current_key = apr_psprintf(msr->mp, "%s", separator + 1);
    }
    else {
        /**
         * TODO: Check if it is safe to do this kind of pointer tricks
         */
        msr->json->current_key = msr->json->prefix;
        msr->json->prefix = (unsigned char *) NULL;
    }

    return 1;
}

/**
 * Initialise JSON parser.
 */
int json_init(modsec_rec *msr, char **error_msg) {
    /**
     * yajl configuration and callbacks
     */
    static yajl_callbacks callbacks = {
        yajl_null,
        yajl_boolean,
        NULL /* yajl_integer  */,
        NULL /* yajl_double */,
        yajl_number,
        yajl_string,
        yajl_start_map,
        yajl_map_key,
        yajl_end_map,
        NULL /* yajl_start_array */,
        NULL /* yajl_end_array  */
    };

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    msr_log(msr, 4, "JSON parser initialization");
    msr->json = apr_pcalloc(msr->mp, sizeof(json_data));
    if (msr->json == NULL) return -1;

    /**
     * Prefix and current key are initially empty
     */
    msr->json->prefix = (unsigned char *) NULL;
    msr->json->current_key = (unsigned char *) NULL;

    /**
     * yajl initialization
     *
     * yajl_parser_config definition:
     * http://lloyd.github.io/yajl/yajl-2.0.1/yajl__parse_8h.html#aec816c5518264d2ac41c05469a0f986c
     *
     * TODO: make UTF8 validation optional, as it depends on Content-Encoding
     */
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "yajl JSON parsing callback initialization");
    }
    msr->json->handle = yajl_alloc(&callbacks, NULL, msr);
    yajl_config(msr->json->handle, yajl_allow_partial_values, 0);

    return 1;
}

/**
 * Feed one chunk of data to the JSON parser.
 */
int json_process_chunk(modsec_rec *msr, const char *buf, unsigned int size, char **error_msg) {
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Feed our parser and catch any errors */
    msr->json->status = yajl_parse(msr->json->handle, buf, size);
    if (msr->json->status != yajl_status_ok) {
        /* We need to free the yajl error message later, how to do this? */
        *error_msg = yajl_get_error(msr->json->handle, 0, buf, size);
        return -1;
    }

    return 1;
}

/**
 * Finalise JSON parsing.
 */
int json_complete(modsec_rec *msr, char **error_msg) {
    char *json_data = (char *) NULL;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Wrap up the parsing process */
    msr->json->status = yajl_complete_parse(msr->json->handle);
    if (msr->json->status != yajl_status_ok) {
        /* We need to free the yajl error message later, how to do this? */
        *error_msg = yajl_get_error(msr->json->handle, 0, NULL, 0);
        return -1;
    }

    return 1;
}

/**
 * Frees the resources used for XML parsing.
 */
apr_status_t json_cleanup(modsec_rec *msr) {
    msr_log(msr, 4, "JSON: Cleaning up JSON results");

    return 1;
}

#endif
