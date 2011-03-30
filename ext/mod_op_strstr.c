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

#define ALPHABET_SIZE       256
#define MAX_PATTERN_SIZE    64

static void initBoyerMooreHorspool(const char *pattern, int patlength,
    int *bm_badcharacter_array);

static int BoyerMooreHorspool(const char *pattern, int patlength,
    const char *text, int textlen, int *bm_badcharacter_array);

/**
 * Operator parameter initialisation entry point.
 */
static int op_strstr_init(msre_rule *rule, char **error_msg) {
    /* Operator initialisation function will be called once per
     * statement where operator is used. It is meant to be used
     * to check the parameters to see whether they are present
     * and if they are in the correct format.
     */

    /* In this example we just look for a simple non-empty parameter. */
    if ((rule->op_param == NULL)||(strlen(rule->op_param) == 0)) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Missing parameter for operator 'strstr'.");
        return 0; /* ERROR */
    }

    /* If you need to transform the data in the parameter into something
     * else you should do that here. Simply create a new structure to hold
     * the transformed data and place the pointer to it into rule->op_param_data.
     * You will have access to this pointer later on.
     */
    rule->op_param_data = apr_pcalloc(rule->ruleset->mp, ALPHABET_SIZE * sizeof(int));
    initBoyerMooreHorspool(rule->op_param, strlen(rule->op_param), (int *)rule->op_param_data);

    /* OK */
    return 1;
}

/**
 * Operator execution entry point.
 */
static int op_strstr_exec(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    /* Here we need to inspect the contents of the supplied variable. */

    /* In a general case it is possible for the value
     * to be NULL. What you need to do in this case
     * depends on your operator. In this example we return
     * a "no match" response.
     */
    if (var->value == NULL) return 0; /* No match. */

    /* Another thing to note is that variables are not C strings,
     * meaning the NULL byte is not used to determine the end
     * of the string. Variable length var->value_len should be
     * used for this purpose.
     */

    if (BoyerMooreHorspool(rule->op_param, strlen(rule->op_param),
        var->value, var->value_len, (int *)rule->op_param_data) >= 0)
    {
        return 1; /* Match. */
    }
    
    return 0; /* No match. */
}

static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    void (*fn)(const char *name, void *fn_init, void *fn_exec);

    /* Look for the registration function
     * exported by ModSecurity.
     */
    fn = APR_RETRIEVE_OPTIONAL_FN(modsec_register_operator);
    if (fn) {
        /* Use it to register our new
         * transformation function under the
         * name "reverse".
         */
        fn("strstr", (void *)op_strstr_init, (void *)op_strstr_exec);
    } else {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, NULL,
            "mod_op_strstr: Unable to find modsec_register_operator.");
    }

    return OK;
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_LAST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA op_strstr_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    register_hooks         /* register hooks                      */
};

/*

This example uses an implementation Boyer-Moore-Horspool
matching algorithm as implemented in Streamline (http://ffpf.sourceforge.net).

Copyright (c) 2004-2006, Vrije Universiteit Amsterdam
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

Neither the name of the Vrije Universiteit nor the names of its contributors 
may be used to endorse or promote products derived from this software without 
specific prior written permission.

*/

static void precompute_badcharacter(const char *pattern, int patlength,
    int bm_badcharacter_array[])
{
    int i;
 
    for (i = 0; i < ALPHABET_SIZE; ++i) {
        bm_badcharacter_array[i] = patlength;
    }

    for (i = 0; i < patlength - 1; ++i){
        bm_badcharacter_array[(uint8_t)pattern[i]] = patlength - i - 1;
	}
}

static void initBoyerMooreHorspool(const char *pattern, int patlength,
    int *bm_badcharacter_array)
{
    precompute_badcharacter(pattern,
        (patlength < MAX_PATTERN_SIZE ? patlength : MAX_PATTERN_SIZE), bm_badcharacter_array);
}

static int BoyerMooreHorspool(const char *pattern, int patlength,
    const char *text, int textlen, int *bm_badcharacter_array)
{
    int j;
    char c;

    j = 0;
    while (j <= textlen - patlength) {
        c = text[j + patlength - 1];
        if (pattern[patlength - 1] == c && memcmp(pattern, text + j, patlength - 1) == 0) {
            return j;
        }
        j += bm_badcharacter_array[(uint8_t)c];
    }

    return -1;
}
