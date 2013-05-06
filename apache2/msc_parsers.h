/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef _MSC_PARSERS_H_
#define _MSC_PARSERS_H_

#include "modsecurity.h"

int DSOLOCAL parse_cookies_v0(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies,
    const char *delim);

int DSOLOCAL parse_cookies_v1(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies);

int DSOLOCAL parse_arguments(modsec_rec *msr, const char *s, apr_size_t inputlength,
    int argument_separator, const char *origin, apr_table_t *arguments, int *invalid_count);

void DSOLOCAL add_argument(modsec_rec *msr, apr_table_t *arguments, msc_arg *arg);

#endif
