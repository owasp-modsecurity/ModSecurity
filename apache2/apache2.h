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
#ifndef _APACHE2_H_
#define _APACHE2_H_

#include "http_core.h"
#include "http_request.h"
#include "httpd.h"
#include "ap_release.h"

#include <apr_general.h>
#include <apr_optional.h>


#if (!defined(NO_MODSEC_API))
/* Optional functions. */

APR_DECLARE_OPTIONAL_FN(void, modsec_register_tfn, (const char *name, void *fn));
APR_DECLARE_OPTIONAL_FN(void, modsec_register_operator, (const char *name, void *fn_init, void *fn_exec));
APR_DECLARE_OPTIONAL_FN(void, modsec_register_variable,
    (const char *name, unsigned int type,
     unsigned int argc_min, unsigned int argc_max,
     void *fn_validate, void *fn_generate,
     unsigned int is_cacheable, unsigned int availability));
#endif

/* ap_get_server_version() is gone in 2.3.0.
 * It was replaced by two calls in 2.2.4 and higher:
 *   ap_get_server_banner()
 *   ap_get_server_description()
 */
#if (AP_SERVER_MAJORVERSION_NUMBER > 2) \
    || ((AP_SERVER_MAJORVERSION_NUMBER == 2)&& (AP_SERVER_MINORVERSION_NUMBER > 2)) \
    || ((AP_SERVER_MAJORVERSION_NUMBER == 2) && (AP_SERVER_MINORVERSION_NUMBER == 2) && (AP_SERVER_PATCHLEVEL_NUMBER >= 4))
#define apache_get_server_version() ap_get_server_banner()
#else
#define apache_get_server_version() ap_get_server_version()
#endif


/* Configuration functions. */

void DSOLOCAL *create_directory_config(apr_pool_t *mp, char *path);

void DSOLOCAL *merge_directory_configs(apr_pool_t *mp, void *_parent, void *_child);

void DSOLOCAL init_directory_config(directory_config *dcfg);


/* IO functions. */

apr_status_t DSOLOCAL input_filter(ap_filter_t *f, apr_bucket_brigade *bb_out,
    ap_input_mode_t mode, apr_read_type_e block, apr_off_t nbytes);

apr_status_t DSOLOCAL output_filter(ap_filter_t *f, apr_bucket_brigade *bb_in);

apr_status_t DSOLOCAL read_request_body(modsec_rec *msr, char **error_msg);


/* Utility functions */

int DSOLOCAL perform_interception(modsec_rec *msr);

apr_status_t DSOLOCAL send_error_bucket(modsec_rec *msr, ap_filter_t *f, int status);

int DSOLOCAL apache2_exec(modsec_rec *msr, const char *command, const char **argv, char **output);

void DSOLOCAL record_time_checkpoint(modsec_rec *msr, int checkpoint_no);

char DSOLOCAL *get_apr_error(apr_pool_t *p, apr_status_t rc);

char DSOLOCAL *get_env_var(request_rec *r, char *name);

void DSOLOCAL internal_log(request_rec *r, directory_config *dcfg, modsec_rec *msr,
    int level, const char *text, va_list ap);

void DSOLOCAL msr_log(modsec_rec *msr, int level, const char *text, ...) PRINTF_ATTRIBUTE(3,4);

char DSOLOCAL *format_error_log_message(apr_pool_t *mp, error_message *em);

const DSOLOCAL char *get_response_protocol(request_rec *r);

#endif

