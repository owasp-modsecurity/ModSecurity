/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Breach Security, Inc. (http://www.breach.com/)
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
#ifndef _MSC_PARSERS_H_
#define _MSC_PARSERS_H_

#include "modsecurity.h"

int DSOLOCAL parse_cookies_v0(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies);

int DSOLOCAL parse_cookies_v1(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies);

int DSOLOCAL parse_arguments(modsec_rec *msr, const char *s, apr_size_t inputlength,
    int argument_separator, const char *origin, apr_table_t *arguments, int *invalid_count);

void DSOLOCAL add_argument(modsec_rec *msr, apr_table_t *arguments, msc_arg *arg);

#endif
