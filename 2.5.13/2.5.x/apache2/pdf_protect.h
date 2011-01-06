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
#ifndef _PDF_PROTECT_H_
#define _PDF_PROTECT_H_

#define PDF_PROTECT_METHOD_TOKEN_REDIRECTION    1
#define PDF_PROTECT_METHOD_FORCED_DOWNLOAD      2

apr_status_t DSOLOCAL pdfp_output_filter(ap_filter_t *f, apr_bucket_brigade *bb_in);

int DSOLOCAL pdfp_check(modsec_rec *msr);

#endif
