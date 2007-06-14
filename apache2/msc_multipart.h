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
#ifndef _MSC_MULTIPART_H_
#define _MSC_MULTIPART_H_

#define MULTIPART_BUF_SIZE              4096

#define MULTIPART_FORMDATA              1
#define MULTIPART_FILE                  2

typedef struct multipart_part multipart_part;
typedef struct multipart_data multipart_data;

#include "apr_general.h"
#include "apr_tables.h"
#include "modsecurity.h"

typedef struct value_part_t value_part_t;
struct value_part_t {
    char *data;
    long int length;
};

struct multipart_part {
    /* part type, can be MULTIPART_FORMDATA or MULTIPART_FILE */
    int                      type;
    /* the name */
    char                    *name;

    /* variables only, variable value */
    char                    *value;
    apr_array_header_t      *value_parts;
    
    /* files only, the content type (where available) */
    char                    *content_type;

    /* files only, the name of the temporary file holding data */
    char                    *tmp_file_name;
    int                      tmp_file_fd;
    unsigned                 tmp_file_size;
    /* files only, filename as supplied by the browser */
    char                    *filename;
    
    char                    *last_header_name;
    apr_table_t             *headers;

    unsigned int             offset;
    unsigned int             length;
};

struct multipart_data {
    /* this array keeps parts */
    apr_array_header_t      *parts;

    /* mime boundary used to detect when
     * parts end and new begin
     */
    char                    *boundary;

    /* internal buffer and other variables
     * used while parsing
     */
    char                     buf[MULTIPART_BUF_SIZE + 2];
    int                      buf_contains_line;
    char                    *bufptr;
    int                      bufleft;

    unsigned int             buf_offset;

    /* pointer that keeps track of a part while
     * it is being built
     */
    multipart_part          *mpp;


    /* part parsing state; 0 means we are reading
     * headers, 1 means we are collecting data
     */
    int                     mpp_state;

    /* because of the way this parsing algorithm
     * works we hold back the last two bytes of
     * each data chunk so that we can discard it
     * later if the next data chunk proves to be
     * a boundary; the first byte is an indicator
     * 0 - no content, 1 - two data bytes available
     */
    char                    reserve[4];

    int                     seen_data;    
    int                     is_complete;
};


/* Functions */

int DSOLOCAL multipart_init(modsec_rec *msr, char **error_msg);

int DSOLOCAL multipart_complete(modsec_rec *msr, char **error_msg);

int DSOLOCAL multipart_process_chunk(modsec_rec *msr, const char *buf,
    unsigned int size, char **error_msg);

apr_status_t DSOLOCAL multipart_cleanup(modsec_rec *msr);

int DSOLOCAL multipart_get_arguments(modsec_rec *msr, char *origin, apr_table_t *arguments);

char DSOLOCAL *multipart_reconstruct_urlencoded_body_sanitise(modsec_rec *msr);

#endif
