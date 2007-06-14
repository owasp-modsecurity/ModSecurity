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
#ifndef _PERSIST_DBM_H_
#define _PERSIST_DBM_H_

#include "apr_general.h"
#include "modsecurity.h"

apr_table_t DSOLOCAL *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_value, int col_value_length);

int DSOLOCAL collection_store(modsec_rec *msr, apr_table_t *collection);

int DSOLOCAL collections_remove_stale(modsec_rec *msr, const char *col_name);

#endif
