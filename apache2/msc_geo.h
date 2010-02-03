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
#ifndef _MSC_GEO_H_
#define _MSC_GEO_H_

#define GEO_STRUCT_INFO_MAX_SIZE  20
#define GEO_DB_INFO_MAX_SIZE      100
#define GEO_COUNTRY_OFFSET        0xffff00
#define GEO_MAX_RECORD_LEN        4
#define GEO_COUNTRY_UNKNOWN       "Unknown"
#define GEO_CITY_UNKNOWN          "Unknown"
#define GEO_CITY_RECORD_LEN       50
#define GEO_COUNTRY_DATABASE      1
#define GEO_CITY_DATABASE_0       6
#define GEO_CITY_DATABASE_1       2
#define GEO_COUNTRY_LAST          250


typedef struct geo_rec geo_rec;
typedef struct geo_db geo_db;

#include <apr_file_io.h>
#include "modsecurity.h"

/* Structures */

struct geo_rec {
    const char *country_code;
    const char *country_code3;
    const char *country_name;
    const char *country_continent;
    const char *region;
    const char *city;
    const char *postal_code;
    float       latitude;
    float       longitude;
    int         dma_code;
    int         area_code;
};

struct geo_db {
    apr_file_t *db;
    const char *dbfn;
    int dbtype;
    unsigned int ctry_offset;
};

/* Functions */

int DSOLOCAL geo_init(directory_config *dcfg, const char *dbfn, char **error_msg);

int DSOLOCAL geo_lookup(modsec_rec *msr, geo_rec *rec, const char *target, char **error_msg);

apr_status_t DSOLOCAL geo_cleanup(modsec_rec *msr);

#endif
