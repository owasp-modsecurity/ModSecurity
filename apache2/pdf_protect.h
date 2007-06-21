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

#ifndef _PDF_PROTECT_H_
#define _PDF_PROTECT_H_

#define PDF_PROTECT_METHOD_TOKEN_REDIRECTION    1
#define PDF_PROTECT_METHOD_FORCED_DOWNLOAD      2

apr_status_t DSOLOCAL pdfp_output_filter(ap_filter_t *f, apr_bucket_brigade *bb_in);

int DSOLOCAL pdfp_check(modsec_rec *msr);

#endif
