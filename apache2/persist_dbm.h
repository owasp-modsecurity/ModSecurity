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

#ifndef _PERSIST_DBM_H_
#define _PERSIST_DBM_H_

#include "apr_general.h"
#include "modsecurity.h"

apr_table_t DSOLOCAL *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_value, int col_value_length);

int DSOLOCAL collection_store(modsec_rec *msr, apr_table_t *collection);

int DSOLOCAL collections_remove_stale(modsec_rec *msr, const char *col_name);

#endif
