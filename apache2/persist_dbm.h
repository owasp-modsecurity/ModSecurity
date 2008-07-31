/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2008 Breach Security, Inc. (http://www.breach.com/)
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
#ifndef _PERSIST_DBM_H_
#define _PERSIST_DBM_H_

#include "apr_general.h"
#include "modsecurity.h"

apr_table_t DSOLOCAL *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_value, int col_value_length);

int DSOLOCAL collection_store(modsec_rec *msr, apr_table_t *collection);

int DSOLOCAL collections_remove_stale(modsec_rec *msr, const char *col_name);

#endif
