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
#define GEO_COUNTRY_LAST          256
#define GEO_SEGMENT_RECORD_LENGTH 3
#define GEO_STATE_BEGIN_REV0      16700000
#define GEO_STATE_BEGIN_REV1      16000000
#define GEO_COUNTRY_BEGIN         16776960


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

#endif
