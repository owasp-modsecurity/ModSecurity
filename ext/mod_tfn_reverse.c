/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address support@trustwave.com.
 *
 */

#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "apr_optional.h"

#include "modsecurity.h"

/**
 * This function will be invoked by
 * ModSecurity to transform input.
 */
static int reverse(apr_pool_t *mptmp, unsigned char *input,
    long int input_len, char **rval, long int *rval_len)
{
    /* Transformation functions can choose to do their
     * thing in-place, overwriting the existing content. This
     * is normally possible only if the transformed content
     * is of equal length or shorter.
     *
     * If you need to expand the content use the temporary
     * memory pool mptmp to allocate the space.
     */

    /* Reverse the string in place, but only if it's long enough. */
    if (input_len > 1) {
        long int i = 0;
        long int j = input_len - 1;
        while(i < j) {
            char c = input[i];
            input[i] = input[j];
            input[j] = c;
            i++;
            j--;
        }
    }

    /* Tell ModSecurity about the content
     * we have generated. In this case we
     * merely point back to the input buffer.
     */
    *rval = (char *)input;
    *rval_len = input_len;

    /* Must return 1 if the content was
     * changed, or 0 otherwise.
     */
    return 1; 
}

static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    void (*fn)(const char *name, void *fn);

    /* Look for the registration function
     * exported by ModSecurity.
     */
    fn = APR_RETRIEVE_OPTIONAL_FN(modsec_register_tfn);
    if (fn) {
        /* Use it to register our new
         * transformation function under the
         * name "reverse".
         */
        fn("reverse", (void *)reverse);
    } else {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, NULL,
            "mod_tfn_reverse: Unable to find modsec_register_tfn.");
    }

    return OK;
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_LAST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA tfn_reverse_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    register_hooks         /* register hooks                      */
};

