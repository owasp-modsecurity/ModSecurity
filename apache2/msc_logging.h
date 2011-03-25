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

#ifndef _MSC_LOGGING_H_
#define _MSC_LOGGING_H_

#define AUDITLOG_OFF                         0
#define AUDITLOG_ON                          1
#define AUDITLOG_RELEVANT                    2

#define AUDITLOG_SERIAL                      0
#define AUDITLOG_CONCURRENT                  1

#define AUDITLOG_PART_FIRST                 'A'
#define AUDITLOG_PART_HEADER                'A'
#define AUDITLOG_PART_REQUEST_HEADERS       'B'
#define AUDITLOG_PART_REQUEST_BODY          'C'
#define AUDITLOG_PART_RESPONSE_HEADERS      'D'
#define AUDITLOG_PART_RESPONSE_BODY         'E'
#define AUDITLOG_PART_A_RESPONSE_HEADERS    'F'
#define AUDITLOG_PART_A_RESPONSE_BODY       'G'
#define AUDITLOG_PART_TRAILER               'H'
#define AUDITLOG_PART_FAKE_REQUEST_BODY     'I'
#define AUDITLOG_PART_UPLOADS               'J'
#define AUDITLOG_PART_MATCHEDRULES          'K'
#define AUDITLOG_PART_LAST                  'K'
#define AUDITLOG_PART_ENDMARKER             'Z'

#include "modsecurity.h"

int DSOLOCAL is_valid_parts_specification(char *p);

char DSOLOCAL *construct_log_vcombinedus(modsec_rec *msr);

char DSOLOCAL *construct_log_vcombinedus_limited(modsec_rec *msr, int _limit, int *was_limited);

void DSOLOCAL sec_audit_logger(modsec_rec *msr);

#endif
