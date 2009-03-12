/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2009 Breach Security, Inc. (http://www.breach.com/)
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
#ifndef _MSC_PCRE_H_
#define _MSC_PCRE_H_

typedef struct msc_regex_t msc_regex_t;

#include "pcre.h"
#include "apr_general.h"
#include "modsecurity.h"

struct msc_regex_t {
    void            *re;
    void            *pe;
    const char      *pattern;
};

apr_status_t DSOLOCAL msc_pcre_cleanup(msc_regex_t *regex);

void DSOLOCAL *msc_pregcomp(apr_pool_t *pool, const char *pattern, int options,
    const char **_errptr, int *_erroffset);

int DSOLOCAL msc_regexec_ex(msc_regex_t *regex, const char *s, unsigned int slen,
    int startoffset, int options, int *ovector, int ovecsize, char **error_msg);

int DSOLOCAL msc_regexec_capture(msc_regex_t *regex, const char *s,
    unsigned int slen, int *ovector, int ovecsize, char **error_msg);

int DSOLOCAL msc_regexec(msc_regex_t *regex, const char *s, unsigned int slen,
    char **error_msg);

int DSOLOCAL msc_fullinfo(msc_regex_t *regex, int what, void *where);

#endif
