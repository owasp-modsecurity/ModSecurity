/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address support@trustwave.com.
 *
 */
#ifndef _MSC_GSB_H_
#define _MSC_GSB_H_

typedef struct gsb_db gsb_db;

#include <apr_file_io.h>
#include "modsecurity.h"

struct gsb_db {
    apr_file_t *db;
    const char *dbfn;
    apr_table_t *gsb_table;
};

int DSOLOCAL gsb_mal_init(directory_config *dcfg, const char *dbfn, char **error_msg);

//int DSOLOCAL gsb_mal_lookup(modsec_rec *msr, const char *target, char **error_msg);

#endif
