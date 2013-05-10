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

#ifndef _MSC_UNICODE_H_
#define _MSC_UNICODE_H_

typedef struct unicode_map unicode_map;

#include <apr_file_io.h>
#include "modsecurity.h"
#include "apr_hash.h"

struct unicode_map {
    apr_file_t *map;
    const char *mapfn;
};

int DSOLOCAL unicode_map_init(directory_config *dcfg, const char *mapfn, char **error_msg);

#endif
