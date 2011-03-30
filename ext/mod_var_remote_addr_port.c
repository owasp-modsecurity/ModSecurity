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

#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "apr_optional.h"

#include "modsecurity.h"

/* -- Generic generators/validators from re_variables.c -- */

/**
 * Generates a variable from a string and a length.
 */
static int var_simple_generate_ex(msre_var *var, apr_table_t *vartab, apr_pool_t *mptmp,
    const char *value, int value_len)
{
    msre_var *rvar = NULL;

    if (value == NULL) return 0;

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = value;
    rvar->value_len = value_len;
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/**
 * Generates a variable from a NULL-terminated string.
 */
static int var_simple_generate(msre_var *var, apr_table_t *vartab, apr_pool_t *mptmp,
    const char *value)
{
    if (value == NULL) return 0;
    return var_simple_generate_ex(var, vartab, mptmp, value, strlen(value));
}


/* -- Module specific code -- */

/**
 * Create a silly variable with value = a.b.c.d:port
 */
static int var_remote_addr_port_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = apr_psprintf(mptmp, "%s:%d", msr->remote_addr, msr->remote_port);

    return var_simple_generate(var, vartab, mptmp, value);
}

static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    void (*register_fn)(const char *name, unsigned int type,
                        unsigned int argc_min, unsigned int argc_max,
                        void *fn_validate, void *fn_generate,
                        unsigned int is_cacheable, unsigned int availability);

    /* Look for the registration function
     * exported by ModSecurity.
     */
    register_fn = APR_RETRIEVE_OPTIONAL_FN(modsec_register_variable);
    if (register_fn) {
        /* Use it to register our new
         * variable under the
         * name "REMOTE_ADDR_PORT".
         */
        register_fn(
            "REMOTE_ADDR_PORT",
            VAR_SIMPLE,
            0, 0,
            NULL,
            var_remote_addr_port_generate,
            VAR_DONT_CACHE,
            PHASE_REQUEST_HEADERS
        );
    }  else {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, NULL,
            "mod_var_remote_addr_port: Unable to find modsec_register_variable.");
    }

    return OK;
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_LAST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA var_remote_addr_port_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    register_hooks         /* register hooks                      */
};

