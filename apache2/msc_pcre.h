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

#ifndef _MSC_PCRE_H_
#define _MSC_PCRE_H_

typedef struct msc_regex_t msc_regex_t;

#include "pcre.h"

#ifndef PCRE_ERROR_MATCHLIMIT
/* Define for compile, but not valid in this version of PCRE. */
#define PCRE_ERROR_MATCHLIMIT (-8)
#endif /* PCRE_ERROR_MATCHLIMIT */

#ifndef PCRE_ERROR_RECURSIONLIMIT
/* Define for compile, but not valid in this version of PCRE. */
#define PCRE_ERROR_RECURSIONLIMIT (-21)
#endif /* PCRE_ERROR_RECURSIONLIMIT */

#include "apr_general.h"
#include "modsecurity.h"

struct msc_regex_t {
    void            *re;
    void            *pe;
    const char      *pattern;
};

void DSOLOCAL *msc_pregcomp_ex(apr_pool_t *pool, const char *pattern, int options,
                               const char **_errptr, int *_erroffset,
                               int match_limit, int match_limit_recursion);

void DSOLOCAL *msc_pregcomp(apr_pool_t *pool, const char *pattern, int options,
                            const char **_errptr, int *_erroffset);

int DSOLOCAL msc_regexec_ex(msc_regex_t *regex, const char *s,
                            unsigned int slen, int startoffset, int options,
                            int *ovector, int ovecsize, char **error_msg);

int DSOLOCAL msc_regexec_capture(msc_regex_t *regex, const char *s,
                                 unsigned int slen, int *ovector,
                                 int ovecsize, char **error_msg);

int DSOLOCAL msc_regexec(msc_regex_t *regex, const char *s,
                         unsigned int slen, char **error_msg);

int DSOLOCAL msc_fullinfo(msc_regex_t *regex, int what, void *where);

#endif
