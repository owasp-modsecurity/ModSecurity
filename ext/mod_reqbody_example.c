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

/* This is just an example on how to make an extension to allow custom
 * request body parsing.  This just describes how to do this externally
 * and you should look more at on e of the internal parsers to see
 * the full potential.
 *
 * This module defines "EXAMPLE" and can be enabled with a rule like this:
 *
 *   SecAction "phase:1,pass,nolog,ctl:requestBodyProcessor=EXAMPLE"
 *
 * See these for real parsers:
 *   msc_reqbody.c
 *   msc_multipart.c
 *   msc_xml.c 
 */

#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "apr_optional.h"

#include "modsecurity.h"

typedef struct example_ctx {
    unsigned long    length;
} example_ctx;

/**
 * This function will be invoked to initialize the processor.  This is
 * probably only needed for streaming parsers that must create a context.
 */
static int example_init(modsec_rec *msr, char **error_msg)
{
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, NULL,
        "mod_reqbody_example: init()");

    msr->reqbody_processor_ctx = apr_pcalloc(msr->mp, sizeof(example_ctx));
    if (msr->reqbody_processor_ctx == NULL) {
        /* Set error message and return -1 if unsuccessful */
        *error_msg = apr_pstrdup(msr->mp, "failed to create example requbody processor context");
        return -1;
    }

    /* Return 1 on success */
    return 1;
}

/**
 * This function will be invoked whenever the ModSecurity has data to
 * be processed.  You probably at least need to increment the no_files
 * length, but otherwise this is only useful for a streaming parser.
 */
static int example_process(modsec_rec *msr,
                           const char *buf, unsigned int size, char **error_msg)
{
    example_ctx *ctx = (example_ctx *)msr->reqbody_processor_ctx;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, NULL,
        "mod_reqbody_example: process()");

    /* Need to increment the no_files length if this is not an uploaded file.
     * Failing to do this will mess up some other limit checks.
     */
    msr->msc_reqbody_no_files_length += size;

    /* Check for an existing context and do something interesting
     * with the chunk of data we have been given.
     */
    if (ctx != NULL) {
        ctx->length += size;
    }

    /* Return 1 on success */
    return 1;
}

/**
 * This function is called to signal the parser that the request body is
 * complete. Here you should do any final parsing.  For non-streaming parsers
 * you can parse the data in msr->msc_reqbody_buffer of length
 * msr->msc_reqbody_length.  See modsecurity_request_body_end_urlencoded() in
 * msc_reqbody.c for an example of this.
 */
static int example_complete(modsec_rec *msr, char **error_msg)
{
    example_ctx *ctx = (example_ctx *)msr->reqbody_processor_ctx;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, NULL,
        "mod_reqbody_example: complete()");

    ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, NULL,
        "mod_reqbody_example: request body length=%lu", ctx->length);

    /* Return 1 on success */
    return 1;
}

static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    
    void (*fn)(const char *name,
               void *fn_init, void *fn_process, void *fn_complete);

    /* Look for the registration function exported by ModSecurity. */
    fn = APR_RETRIEVE_OPTIONAL_FN(modsec_register_reqbody_processor);
    if (fn) {
        /* Use it to register our new request body parser functions under
         * the name "EXAMPLE".
         */
        fn("EXAMPLE",
           (void *)example_init,
           (void *)example_process,
           (void *)example_complete);
    }
    else {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, NULL,
            "mod_reqbody_example: Unable to find modsec_register_reqbody_processor.");
    }

    return OK;
}

/* This is a function to register another function to be called during
 * the Apache configuration process.
 */
static void register_hooks(apr_pool_t *p) {
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_LAST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA reqbody_example_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    register_hooks         /* register hooks                      */
};

