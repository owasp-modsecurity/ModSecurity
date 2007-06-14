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
#ifndef _APACHE2_H_
#define _APACHE2_H_

#include "http_core.h"
#include "http_request.h"

#include <apr_general.h>
#include <apr_optional.h>


/* Optional functions. */

APR_DECLARE_OPTIONAL_FN(void, modsec_register_tfn, (const char *name, void *fn));
APR_DECLARE_OPTIONAL_FN(void, modsec_register_operator, (const char *name, void *fn_init, void *fn_exec));


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

int DSOLOCAL apache2_exec(modsec_rec *msr, const char *command, const char **argv, char **output);

void DSOLOCAL record_time_checkpoint(modsec_rec *msr, int checkpoint_no);

char DSOLOCAL *get_apr_error(apr_pool_t *p, apr_status_t rc);

char DSOLOCAL *get_env_var(request_rec *r, char *name);

void DSOLOCAL internal_log(request_rec *r, directory_config *dcfg, modsec_rec *msr,
    int level, const char *text, va_list ap);

void DSOLOCAL msr_log(modsec_rec *msr, int level, const char *text, ...);

char DSOLOCAL *format_error_log_message(apr_pool_t *mp, error_message *em);

const DSOLOCAL char *get_response_protocol(request_rec *r);

#endif

