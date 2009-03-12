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
#ifndef ACMP_H_
#define ACMP_H_

#include <apr.h>
#include <apr_pools.h>

#define ACMP_FLAG_BYTE               0
#define ACMP_FLAG_CASE_SENSITIVE     1
#define ACMP_FLAG_CASE_INSENSITIVE   0
#ifdef ACMP_USE_UTF8
#define ACMP_FLAG_UTF8               0x100
#endif

/**
 * Opaque struct with parser data
 */
typedef struct ACMP ACMP;

/**
 * Used to separate state from the trie for acmp_process_quick function
 */
typedef struct {
    ACMP *parser;
    void *ptr;
} ACMPT;

/**
 * Callback function. Arguments are:
 * ACMP * - acmp parser that initiated callback
 * void * - custom data you supplied when adding callback
 * apr_size_t - position in bytes where pattern was found
 * apr_size_t - position in chars where pattern was found, for multibyte strings
 */
typedef void (*acmp_callback_t)(ACMP *, void *, apr_size_t, apr_size_t);

/**
 * flags - OR-ed values of ACMP_FLAG constants
 * pool  - apr_pool to use as parent pool, can be set to NULL
 */
ACMP *acmp_create(int flags, apr_pool_t *pool);

/**
 * Destroys previously created parser
 */
void acmp_destroy(ACMP *parser);

/**
 * Creates parser with same options and same patterns
 * parser - ACMP parser to duplicate
 * pool - parent pool to use, if left as NULL original parser's parent pool is used
 */
ACMP *acmp_duplicate(ACMP *parser, apr_pool_t *pool);

/**
 * Adds pattern to parser. Cannot be done after starting the search.
 * parser - ACMP parser
 * pattern - string with pattern to match
 * callback - Optional, pointer to an acmp_callback_t function
 * data - pointer to data that will be passed to callback function, only used if callback
 *   is supplied
 * len - Length of pattern in characters, if zero string length is used.
 */
apr_status_t acmp_add_pattern(ACMP *parser, const char *pattern,
    acmp_callback_t callback, void *data, apr_size_t len);

/**
 * Called to process incoming data stream. You must call acmp_done after sending
 *   last data packet
 *
 * data - ptr to incoming data
 * len  - size of data in bytes
 */
apr_status_t acmp_process(ACMP *parser, const char *data, apr_size_t len);

/**
 * Returns number of matches on all patterns combined
 */
apr_size_t acmp_match_count_total(ACMP *parser);

/**
 * Returns number of matches for given pattern
 */
apr_size_t acmp_match_count(ACMP *parser, const char *pattern);

/**
 * Resets the state of parser so you can start using it with new set of data,
 * or add new patterns.
 */
void acmp_reset(ACMP *parser);

/**
 * Creates an ACMPT struct that will use parser's tree, without duplicating its data
 */
ACMPT *acmp_duplicate_quick(ACMP *parser, apr_pool_t *pool);

/**
 * Process the data using ACMPT to keep state, and ACMPT's parser to keep the tree
 */
apr_status_t acmp_process_quick(ACMPT *acmpt, const char **match, const char *data, apr_size_t len);

/**
 * Prepares parser for searching
 */
apr_status_t acmp_prepare(ACMP *parser);


#endif /*ACMP_H_*/
