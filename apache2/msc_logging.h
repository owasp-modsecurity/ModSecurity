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
#define AUDITLOG_PART_LAST                  'I'
#define AUDITLOG_PART_ENDMARKER             'Z'

#include "modsecurity.h"

int DSOLOCAL is_valid_parts_specification(char *p);

char DSOLOCAL *construct_log_vcombinedus(modsec_rec *msr);

char DSOLOCAL *construct_log_vcombinedus_limited(modsec_rec *msr, int _limit, int *was_limited);

void DSOLOCAL sec_audit_logger(modsec_rec *msr);

#endif
