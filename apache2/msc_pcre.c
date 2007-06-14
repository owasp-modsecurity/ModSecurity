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
#include "msc_pcre.h"
#include "apr_strings.h"

/**
 * Releases the resources used by a single regular expression pattern.
 */
apr_status_t msc_pcre_cleanup(msc_regex_t *regex) {
    if (regex != NULL) {
        if (regex->pe != NULL) {
            free(regex->pe);
            regex->pe = NULL;
        }
        if (regex->re != NULL) {    
            free(regex->re);
            regex->re = NULL;
        }
    }

    return APR_SUCCESS;
}

/**
 * Compiles the provided regular expression pattern. The last two
 * parameters are optional, but if they are provided and an error
 * occurs they will contain the error message and the offset in
 * the pattern where the offending part of the pattern begins.
 */  
void *msc_pregcomp(apr_pool_t *pool, const char *pattern, int options,
    const char **_errptr, int *_erroffset)
{
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;

    regex = apr_pcalloc(pool, sizeof(msc_regex_t));
    if (regex == NULL) return NULL;
    regex->pattern = pattern;

    if ((_errptr == NULL)||(_erroffset == NULL)) {
        regex->re = pcre_compile(pattern, options, &errptr, &erroffset, NULL);
    } else {
        regex->re = pcre_compile(pattern, options, _errptr, _erroffset, NULL);
    }
    if (regex->re == NULL) return NULL;

    #ifdef WITH_PCRE_STUDY
    regex->pe = pcre_study(regex->re, 0, &errptr);
    #endif

    apr_pool_cleanup_register(pool, (void *)regex,
        (apr_status_t (*)(void *))msc_pcre_cleanup, apr_pool_cleanup_null);

    return regex;
}

/**
 * Executes regular expression, capturing subexpressions in the given
 * vector. Returns PCRE_ERROR_NOMATCH when there is no match, error code < -1
 * on errors, and a value > 0 when there is a match.
 */                              
int msc_regexec_capture(msc_regex_t *regex, const char *s, unsigned int slen,
    int *ovector, int ovecsize, char **error_msg)
{
    if (error_msg == NULL) return -1000; /* To differentiate from PCRE as it already uses -1. */
    *error_msg = NULL;

    return pcre_exec(regex->re, regex->pe, s, slen, 0, 0, ovector, ovecsize);
}

/**
 * Executes regular expression but ignores any of the subexpression
 * captures. See above for the return codes.
 */
int msc_regexec(msc_regex_t *regex, const char *s, unsigned int slen,
    char **error_msg)
{
    if (error_msg == NULL) return -1000; /* To differentiate from PCRE as it already uses -1. */
    *error_msg = NULL;

    return msc_regexec_capture(regex, s, slen, NULL, 0, error_msg);
}
