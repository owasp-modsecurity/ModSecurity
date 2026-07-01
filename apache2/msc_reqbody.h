/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2022 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef _MSC_REQBODY_H_
#define _MSC_REQBODY_H_

#include "modsecurity.h"

void DSOLOCAL modsecurity_request_body_do_enable_partial_processing(modsec_rec *msr);
apr_ssize_t DSOLOCAL modsecurity_request_body_may_enable_partial_processing_for_no_files_length(modsec_rec *msr,
    apr_ssize_t adding_length, const char *reqbody_processor);

#endif
