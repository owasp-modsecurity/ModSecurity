/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2022 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "msc_pcre.h"
#include "apr_strings.h"

/**
 * Releases the resources used by a single regular expression pattern.
 */
static apr_status_t msc_pcre_cleanup(msc_regex_t *regex) {
    if (regex != NULL) {
#ifdef WITH_PCRE2
        if (regex->match_context != NULL) {
            pcre2_match_context_free(regex->match_context);
            regex->match_context = NULL;
        }
        if (regex->re != NULL) {
            pcre2_code_free(regex->re);
            regex->re = NULL;
        }
#else
        if (regex->pe != NULL) {
            pcre_free(regex->pe);
            regex->pe = NULL;
        }
        if (regex->re != NULL) {
            pcre_free(regex->re);
            regex->re = NULL;
        }
#endif
    }

    return APR_SUCCESS;
}

/**
 * Compiles the provided regular expression pattern. The _err*
 * parameters are optional, but if they are provided and an error
 * occurs they will contain the error message and the offset in
 * the pattern where the offending part of the pattern begins. The
 * match_limit* parameters are optional and if >0, then will set
 * match limits.
 */
void *msc_pregcomp_ex(apr_pool_t *pool, const char *pattern, int options,
                      const char **_errptr, int *_erroffset,
                      int match_limit, int match_limit_recursion)
#ifdef WITH_PCRE2
{
    msc_regex_t *regex = NULL;
    PCRE2_SPTR pcre2_pattern;
    uint32_t pcre2_options;
    int error_number = 0;
    PCRE2_SIZE error_offset = 0;
    pcre2_match_context *match_context = NULL;

    regex = apr_pcalloc(pool, sizeof(msc_regex_t));
    if (regex == NULL) return NULL;
    regex->pattern = pattern;

    pcre2_pattern = (PCRE2_SPTR)pattern;
    pcre2_options = (uint32_t)options;

    regex->re = pcre2_compile(pcre2_pattern, PCRE2_ZERO_TERMINATED,
        pcre2_options, &error_number, &error_offset, NULL);
    if (regex->re == NULL) {
        if (_erroffset != NULL) {
            *_erroffset = (int)error_offset;
        }
        return NULL;
    }

#ifdef WITH_PCRE_JIT
    regex->jit_compile_rc = pcre2_jit_compile(regex->re, PCRE2_JIT_COMPLETE);
#endif

    /* Setup the pcre2 match context */
    regex->match_context = NULL;
    match_context = pcre2_match_context_create(NULL);
    if (match_context == NULL) {
        return NULL;
    }

    /* Prefer the match limit passed as an arg; else use compilation default */
    {
        uint32_t final_match_limit = 0;
        if (match_limit > 0) {
            final_match_limit = match_limit;
            pcre2_set_match_limit(match_context, final_match_limit);
        }
#ifdef MODSEC_PCRE_MATCH_LIMIT
        else {
            final_match_limit = MODSEC_PCRE_MATCH_LIMIT;
            pcre2_set_match_limit(match_context, final_match_limit);
        }
#endif /* MODSEC_PCRE_MATCH_LIMIT */
    }

    /* Prefer the depth limit passed as an arg; else use compilation default */
    {
        uint32_t final_match_limit_recursion = 0;
        if (match_limit_recursion > 0) {
            final_match_limit_recursion = match_limit_recursion;
            pcre2_set_depth_limit(match_context, final_match_limit_recursion);
        }
#ifdef MODSEC_PCRE_MATCH_LIMIT_RECURSION
        else {
            final_match_limit_recursion = MODSEC_PCRE_MATCH_LIMIT_RECURSION;
            pcre2_set_depth_limit(match_context, final_match_limit_recursion);
        }
#endif /* MODSEC_PCRE_MATCH_LIMIT_RECURSION */
    }

    regex->match_context = match_context;

    apr_pool_cleanup_register(pool, (void *)regex,
        (apr_status_t (*)(void *))msc_pcre_cleanup, apr_pool_cleanup_null);

    return regex;
}
#else /* not WITH_PCRE2 */
{
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;
    pcre_extra *pe = NULL;

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
        #ifdef WITH_PCRE_JIT
                pe = pcre_study(regex->re, PCRE_STUDY_JIT_COMPILE, &errptr);
        #else
                pe = pcre_study(regex->re, 0, &errptr);
        #endif
    #endif

    /* Setup the pcre_extra record if pcre_study did not already do it */
    if (pe == NULL) {
        pe = (pcre_extra*)pcre_malloc(sizeof(pcre_extra));
        if (pe == NULL) {
            return NULL;
        }
        memset(pe, 0, sizeof(pcre_extra));
    }

#ifdef PCRE_EXTRA_MATCH_LIMIT
    /* If match limit is available, then use it */

    /* Use ModSecurity runtime defaults */
    if (match_limit > 0) {
        pe->match_limit = match_limit;
        pe->flags |= PCRE_EXTRA_MATCH_LIMIT;
    }
#ifdef MODSEC_PCRE_MATCH_LIMIT
    /* Default to ModSecurity compiled defaults */
    else {
        pe->match_limit = MODSEC_PCRE_MATCH_LIMIT;
        pe->flags |= PCRE_EXTRA_MATCH_LIMIT;
    }
#endif /* MODSEC_PCRE_MATCH_LIMIT */
#else
#pragma message ( "This PCRE version does not support match limits!  Upgrade to at least PCRE v6.5." )
#endif /* PCRE_EXTRA_MATCH_LIMIT */

#ifdef PCRE_EXTRA_MATCH_LIMIT_RECURSION
    /* If match limit recursion is available, then use it */

    /* Use ModSecurity runtime defaults */
    if (match_limit_recursion > 0) {
        pe->match_limit_recursion = match_limit_recursion;
        pe->flags |= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
    }
#ifdef MODSEC_PCRE_MATCH_LIMIT_RECURSION
    /* Default to ModSecurity compiled defaults */
    else {
        pe->match_limit_recursion = MODSEC_PCRE_MATCH_LIMIT_RECURSION;
        pe->flags |= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
    }
#endif /* MODSEC_PCRE_MATCH_LIMIT_RECURSION */
#else
#pragma message ( "This PCRE version does not support match recursion limits!  Upgrade to at least PCRE v6.5." )
#endif /* PCRE_EXTRA_MATCH_LIMIT_RECURSION */

    regex->pe = pe;

    apr_pool_cleanup_register(pool, (void *)regex,
        (apr_status_t (*)(void *))msc_pcre_cleanup, apr_pool_cleanup_null);

    return regex;
}
#endif /* WITH_PCRE2 */

/**
 * Compiles the provided regular expression pattern.  Calls msc_pregcomp_ex()
 * with default limits.
 */
void *msc_pregcomp(apr_pool_t *pool, const char *pattern, int options,
                   const char **_errptr, int *_erroffset)
{
    return msc_pregcomp_ex(pool, pattern, options, _errptr, _erroffset, 0, 0);
}

/**
 * Executes regular expression with extended options (or match context)
 * Returns PCRE_ERROR_NOMATCH (or PCRE2_ERROR_NOMATCH),
 * error code < -1 on errors, and a value > 0 when there is a match.
 */
int msc_regexec_ex(msc_regex_t *regex, const char *s, unsigned int slen,
    int startoffset, int options, int *ovector, int ovecsize, char **error_msg)
{
    if (error_msg == NULL) return -1000; /* To differentiate from PCRE as it already uses -1. */
    *error_msg = NULL;

#ifdef WITH_PCRE2
    {
        PCRE2_SPTR pcre2_s;
        int pcre2_ret;
        pcre2_match_data *match_data;
        PCRE2_SIZE *pcre2_ovector = NULL;

        pcre2_s = (PCRE2_SPTR)s;
        match_data = pcre2_match_data_create_from_pattern(regex->re, NULL);

#ifdef WITH_PCRE_JIT
        if (regex->jit_compile_rc == 0) {
            pcre2_ret = pcre2_jit_match(regex->re, pcre2_s, slen,
                (PCRE2_SIZE)(startoffset), (uint32_t)options, match_data, regex->match_context);
        }
        if (regex->jit_compile_rc != 0 || pcre2_ret == PCRE2_ERROR_JIT_STACKLIMIT) {
            pcre2_ret = pcre2_match(regex->re, pcre2_s, slen,
                (PCRE2_SIZE)(startoffset), (PCRE2_NO_JIT | (uint32_t)options), match_data, regex->match_context);
        }
#else
        pcre2_ret = pcre2_match(regex->re, pcre2_s, slen,
            (PCRE2_SIZE)(startoffset), (uint32_t)options, match_data, regex->match_context);
#endif
        if (match_data != NULL) {
            if (ovector != NULL) {
                pcre2_ovector = pcre2_get_ovector_pointer(match_data);
                if (pcre2_ovector != NULL) {
                    for (int i = 0; ((i < pcre2_ret) && ((i*2) <= ovecsize)); i++) {
                        if ((i*2) < ovecsize) {
                            ovector[2*i] = pcre2_ovector[2*i];
                            ovector[2*i+1] = pcre2_ovector[2*i+1];
                        }
                    }
                }
            }
            pcre2_match_data_free(match_data);
        }
        if ((pcre2_ret*2) > ovecsize) {
            return 0;
        } else {
            return pcre2_ret;
        }
    }
#else
    return pcre_exec(regex->re, regex->pe, s, slen, startoffset, options, ovector, ovecsize);
#endif
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

    return msc_regexec_ex(regex, s, slen, 0, 0, ovector, ovecsize, error_msg);
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

    return msc_regexec_ex(regex, s, slen, 0, 0, NULL, 0, error_msg);
}

/**
 * Gets info on a compiled regex.
 */
int msc_fullinfo(msc_regex_t *regex, int what, void *where)
{
#ifdef WITH_PCRE2
    return pcre2_pattern_info(regex->re, (uint32_t)what, where);
#else
    return pcre_fullinfo(regex->re, regex->pe, what, where);
#endif
}

