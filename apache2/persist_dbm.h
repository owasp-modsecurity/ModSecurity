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

#ifndef _PERSIST_DBM_H_
#define _PERSIST_DBM_H_

#include "apr_general.h"
#include "modsecurity.h"

/* The DB Option Definition */
#define DB_OPT_AGMDB    1
#define DB_OPT_ORIGIN   0

/*
* The maximum number of entries that database can support.
* The database will refuse the new entry injection if the total entry number is larger than MAXIMUM_AGMDB_ENTRY_NUM
* TODO: add as a configuration parameter 
*/  
#define MAXIMUM_AGMDB_ENTRY_NUM 200000

struct agmdb_handle_entry{
    const char* col_name;
    void* handle;
    struct agmdb_handle_entry *next;
};

apr_table_t DSOLOCAL *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_value, int col_value_length);

int DSOLOCAL collection_store(modsec_rec *msr, apr_table_t *collection);

int DSOLOCAL collections_remove_stale(modsec_rec *msr, const char *col_name);

#endif
