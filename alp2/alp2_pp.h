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

#ifndef _ALP2_LL_H_
#define _ALP2_LL_H_

#include <apr_general.h>
#include <apr_md5.h>
#include <apr_strings.h>
#include <apr_tables.h>
#include <apr_md5.h>

/* -- Constants -- */

#define ALP2_MAX_LINE_SIZE 16384

#define ALP2_ERROR_INCORRECT_STATE  -1001
#define ALP2_ERROR_FATAL            -1002

#define ALP2_DONE       0
#define ALP2_NEED_DATA  1

#define ALP2_EVENT_ENTRY_START          1
#define ALP2_EVENT_ENTRY_END            2
#define ALP2_EVENT_PART_START           3
#define ALP2_EVENT_PART_END             4
#define ALP2_EVENT_PART_DATA            5


/* -- Data structures -- */

typedef struct alp2_pp_part_t alp2_pp_part_t;

struct alp2_pp_part_t {
    int id; // XXX int here but unsigned int other places???
    
    /* Relative to the beginning of the entry, not
     * including the boundary lines. Just content.
     */
    size_t offset;
    
    size_t size;
};

typedef struct alp2_pp_entry_t alp2_pp_entry_t;

struct alp2_pp_entry_t {
    apr_pool_t *mp;
    apr_array_header_t *parts;
    
    /* Entry offset and size include 
     * the delimiting boundaries. 
     */
    size_t offset;
    size_t size;

    const char *boundary;

    apr_md5_ctx_t *md5_context;
    uint8_t md5_digest[APR_MD5_DIGESTSIZE];
};

typedef struct alp2_pp_t alp2_pp_t;

struct alp2_pp_t {
    void *user_data;
    int (*callback)(alp2_pp_t *alp, int event_type);

    /* The memory pool used during the parsing of
     * individual audit log entries. Cleared between
     * entries.
     */
    apr_pool_t *mp;

    unsigned int errored;
    unsigned int done;
    
    const char *boundary;
    char *last_processed_part;
    char *current_line;
    
    /* The number of bytes processed since
     * the beginning or the last reset.
     */
    size_t current_offset;
    
    const char *input_buf;
    size_t input_len;
    size_t input_pos;

    char *line_buf;
    size_t line_pos;
    size_t line_size;
    unsigned int line_has_start;
    size_t line_offset;
    
    alp2_pp_part_t *current_part;
    alp2_pp_entry_t *current_entry;
};


/* Functions. */

int alp2_pp_init(alp2_pp_t *alp_pp, void *user_data,
    int (*callback)(alp2_pp_t *alp, int event_type), apr_pool_t *mp);

int alp2_pp_process(alp2_pp_t *alp_pp, const char *data, size_t len);

void alp2_pp_terminate(alp2_pp_t *alp_pp);

char *alp2_pp_line_chomp(alp2_pp_t *alp_pp);

#endif
