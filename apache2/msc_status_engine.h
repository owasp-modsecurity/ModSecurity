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

#ifndef MSC_STATUS_ENGINE_H
#define MSC_STATUS_ENGINE_H

#include "apr.h"
#include "apr_pools.h"
#include "apr_version.h"
#include "apr_optional.h"
#include "msc_pcre.h"

#ifndef WIN32
#define STATUS_ENGINE_DNS_IN_BETWEEN_DOTS 32
#else
#define STATUS_ENGINE_DNS_IN_BETWEEN_DOTS 30
#endif

#define STATUS_ENGINE_DNS_SUFFIX "status.modsecurity.org"

#define MAX_MACHINE_NAME_SIZE 100

#define MAC_ADDRESS_SIZE 20

static const char msc_status_engine_basis_32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

int msc_status_engine_call(void);

int DSOLOCAL msc_status_engine_unique_id (unsigned char *digest);

int DSOLOCAL msc_beacon_string (char *beacon_string, int beacon_string_max_len);

#endif
