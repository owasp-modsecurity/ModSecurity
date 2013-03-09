/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <limits.h>

#include "http_core.h"
#include "http_request.h"

#include "modsecurity.h"
#include "apache2.h"
#include "http_main.h"
#include "http_connection.h"

#include "apr_optional.h"
#include "mod_log_config.h"

#include "msc_logging.h"
#include "msc_util.h"

#include "ap_mpm.h"
#include "scoreboard.h"

#include "apr_version.h"

#include "apr_lib.h"
#include "ap_config.h"
#include "http_config.h"


static apr_status_t regex_cleanup(void *preg)
{
    ap_regfree((ap_regex_t *) preg);
    return APR_SUCCESS;
}

AP_DECLARE(ap_regex_t *) ap_pregcomp(apr_pool_t *p, const char *pattern,
                                     int cflags)
{
    ap_regex_t *preg = apr_palloc(p, sizeof *preg);

    if (ap_regcomp(preg, pattern, cflags)) {
        return NULL;
    }

    apr_pool_cleanup_register(p, (void *) preg, regex_cleanup,
                              apr_pool_cleanup_null);

    return preg;
}

AP_DECLARE(void) ap_regfree(ap_regex_t *preg)
{
(pcre_free)(preg->re_pcre);
}

AP_DECLARE(int) ap_regcomp(ap_regex_t *preg, const char *pattern, int cflags)
{
const char *errorptr;
int erroffset;
int options = 0;
int nsub = 0;

if ((cflags & AP_REG_ICASE) != 0) options |= PCRE_CASELESS;
if ((cflags & AP_REG_NEWLINE) != 0) options |= PCRE_MULTILINE;

preg->re_pcre = pcre_compile(pattern, options, &errorptr, &erroffset, NULL);
preg->re_erroffset = erroffset;

if (preg->re_pcre == NULL) return AP_REG_INVARG;

pcre_fullinfo((const pcre *)preg->re_pcre, NULL, PCRE_INFO_CAPTURECOUNT, &nsub);
preg->re_nsub = nsub;
return 0;
}

#ifndef POSIX_MALLOC_THRESHOLD
#define POSIX_MALLOC_THRESHOLD (10)
#endif

AP_DECLARE(int) ap_regexec(const ap_regex_t *preg, const char *string,
                           apr_size_t nmatch, ap_regmatch_t pmatch[],
                           int eflags)
{
int rc;
int options = 0;
int *ovector = NULL;
int small_ovector[POSIX_MALLOC_THRESHOLD * 3];
int allocated_ovector = 0;

if ((eflags & AP_REG_NOTBOL) != 0) options |= PCRE_NOTBOL;
if ((eflags & AP_REG_NOTEOL) != 0) options |= PCRE_NOTEOL;

((ap_regex_t *)preg)->re_erroffset = (apr_size_t)(-1);  /* Only has meaning after compile */

if (nmatch > 0)
  {
  if (nmatch <= POSIX_MALLOC_THRESHOLD)
    {
    ovector = &(small_ovector[0]);
    }
  else
    {
    ovector = (int *)malloc(sizeof(int) * nmatch * 3);
    if (ovector == NULL) return AP_REG_ESPACE;
    allocated_ovector = 1;
    }
  }

rc = pcre_exec((const pcre *)preg->re_pcre, NULL, string, (int)strlen(string),
  0, options, ovector, nmatch * 3);

if (rc == 0) rc = nmatch;    /* All captured slots were filled in */

if (rc >= 0)
  {
  apr_size_t i;
  for (i = 0; i < (apr_size_t)rc; i++)
    {
    pmatch[i].rm_so = ovector[i*2];
    pmatch[i].rm_eo = ovector[i*2+1];
    }
  if (allocated_ovector) free(ovector);
  for (; i < nmatch; i++) pmatch[i].rm_so = pmatch[i].rm_eo = -1;
  return 0;
  }

else
  {
  if (allocated_ovector) free(ovector);
  switch(rc)
    {
    case PCRE_ERROR_NOMATCH: return AP_REG_NOMATCH;
    case PCRE_ERROR_NULL: return AP_REG_INVARG;
    case PCRE_ERROR_BADOPTION: return AP_REG_INVARG;
    case PCRE_ERROR_BADMAGIC: return AP_REG_INVARG;
    case PCRE_ERROR_UNKNOWN_NODE: return AP_REG_ASSERT;
    case PCRE_ERROR_NOMEMORY: return AP_REG_ESPACE;
#ifdef PCRE_ERROR_MATCHLIMIT
    case PCRE_ERROR_MATCHLIMIT: return AP_REG_ESPACE;
#endif
#ifdef PCRE_ERROR_BADUTF8
    case PCRE_ERROR_BADUTF8: return AP_REG_INVARG;
#endif
#ifdef PCRE_ERROR_BADUTF8_OFFSET
    case PCRE_ERROR_BADUTF8_OFFSET: return AP_REG_INVARG;
#endif
    default: return AP_REG_ASSERT;
    }
  }
}

