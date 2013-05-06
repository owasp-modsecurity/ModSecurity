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

#ifndef _MSC_GSB_H_
#define _MSC_GSB_H_

typedef struct gsb_db gsb_db;

#include <apr_file_io.h>
#include "modsecurity.h"
#include "apr_hash.h"

struct gsb_db {
    apr_file_t *db;
    const char *dbfn;
    apr_hash_t *gsb_table;
};

int DSOLOCAL gsb_db_init(directory_config *dcfg, const char *dbfn, char **error_msg);

#endif
