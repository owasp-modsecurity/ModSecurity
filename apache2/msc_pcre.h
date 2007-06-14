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

int DSOLOCAL msc_regexec_capture(msc_regex_t *regex, const char *s,
    unsigned int slen, int *ovector, int ovecsize, char **error_msg);

int DSOLOCAL msc_regexec(msc_regex_t *regex, const char *s, unsigned int slen,
    char **error_msg);

#endif
