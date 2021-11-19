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

#ifndef _MSC_JSON_H_
#define _MSC_JSON_H_
#ifdef WITH_YAJL

typedef struct json_data json_data;


#include "modsecurity.h"

#include <yajl/yajl_parse.h>
//#ifdef WITH_YAJL
//#else


#include "modsecurity.h"

/* Structures */
struct json_data {
    /* yajl configuration and parser state */
    yajl_handle handle;
    yajl_status status;

    /* error reporting and JSON array flag */
    unsigned char *yajl_error;

    /* prefix is used to create data hierarchy (i.e., 'parent.child.value') */
    unsigned char *prefix;
    unsigned char *current_key;
    long int current_depth;
    int depth_limit_exceeded;
};

/* Functions */

int DSOLOCAL json_init(modsec_rec *msr, char **error_msg);

int DSOLOCAL json_process(modsec_rec *msr, const char *buf,
    unsigned int size, char **error_msg);

int DSOLOCAL json_complete(modsec_rec *msr, char **error_msg);

apr_status_t DSOLOCAL json_cleanup(modsec_rec *msr);

int DSOLOCAL json_process_chunk(modsec_rec *msr, const char *buf,
		unsigned int size, char **error_msg);

#endif
#endif
