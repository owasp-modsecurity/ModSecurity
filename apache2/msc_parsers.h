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
#ifndef _MSC_PARSERS_H_
#define _MSC_PARSERS_H_

#include "modsecurity.h"

int DSOLOCAL parse_cookies_v0(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies);

int DSOLOCAL parse_cookies_v1(modsec_rec *msr, char *_cookie_header, apr_table_t *cookies);

int DSOLOCAL parse_arguments(modsec_rec *msr, const char *s, apr_size_t inputlength, 
    int argument_separator, const char *origin, apr_table_t *arguments, int *invalid_count);

#endif
