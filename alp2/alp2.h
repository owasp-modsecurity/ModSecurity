/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Breach Security, Inc. (http://www.breach.com/)
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

#ifndef _ALP2_H_
#define _ALP2_H_

#include <apr_time.h>
#include <apr_uri.h>

#include "alp2_pp.h"
#include "pcre.h"

/* -- Data structures -- */

typedef struct alp2_msg_t alp2_msg_t;

struct alp2_msg_t {
    const char *engine_message;
    const char *target;
    const char *id;
    const char *rev;
    const char *msg;
    const char *data;
    const char *file;
    unsigned long file_line;
    size_t offset;
    int severity;
    int warning;
    apr_array_header_t *tags;
};

typedef struct auditlog2_t auditlog2_t;

struct auditlog2_t {
    apr_pool_t *mp;

    /* Transaction data */

    const char *id;

    apr_time_t timestamp;
    unsigned int duration;

    const char *src_ip;
    unsigned int src_port;
    const char *dst_ip;
    unsigned int dst_port;

    /* Request */

    unsigned int request_line_valid;
    const char *request_line;
    const char *request_method;
    const char *request_uri;
    apr_uri_t *parsed_uri;
    const char *request_protocol;
    apr_table_t *request_headers;    

    /* Determine the hostname: The hostname from the URI is
     * used where present, otherwise the value of the Host
     * request header is used.
     *
     * If neither of these two is available we will use the
     * combination of the destination IP and port as hostname.
     *
     * The resulting hostname may have the port attached.
     */
    const char *hostname;

    /* Response */

    const char *response_protocol;
    unsigned int response_status;
    const char *response_message;
    apr_table_t *response_headers;
    const char *response_tfn;

    /* Other */

    apr_table_t *trailer_headers;

    unsigned int was_intercepted;
    unsigned int intercept_phase; /* -1 if interception did not happen */

    const char *producer;
    const char *server;
    const char *handler;

    const char *application_id;
    const char *session_id;
    const char *user_id;

    apr_array_header_t *messages;

    alp2_pp_entry_t *pp_entry;
};

typedef struct alp2_t alp2_t;

struct alp2_t {
    apr_pool_t *mp;

    void *user_data;
    int (*user_callback)(alp2_t *alp);

    alp2_pp_t *pp;

    unsigned int previous_part_id;
    unsigned int part_line_counter;
    unsigned int part_data_done;
    unsigned int seen_part_h;

    unsigned int done;
    unsigned int parse_error;
    apr_array_header_t *errors;

    /* Regular expression patterns. */
    // TODO All these need reviewing
    pcre *part_a_pattern;
    pcre *request_line_pattern;
    pcre *header_pattern;
    pcre *response_line_pattern;
    
    pcre *trailer_action_pattern;
    pcre *trailer_stopwatch_pattern;
    pcre *trailer_webappinfo_pattern;

    auditlog2_t *auditlog;
};

/* Higher-level (user) parser. */

/* NOTE Parser will create a subpool for its own use, but each
 *      entry will be created in a separate subpool directly
 *      under the main pool. This allows the created audit log
 *      entries to survive the death of the parser.
 */


/* -- Functions -- */

int alp2_create(alp2_t **_alp, apr_pool_t *mp,
                void *user_data, int (*user_callback)(alp2_t *alp));

int alp2_process(alp2_t *alp, const char *data, size_t len);

void alp2_destroy(alp2_t *alp);

auditlog2_t *alp2_auditlog_create(apr_pool_t *mp);

void alp2_auditlog_destroy(auditlog2_t *al);

#endif
