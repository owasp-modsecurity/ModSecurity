/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#ifndef HEADERS_MODSECURITY_INTERVENTION_H_
#define HEADERS_MODSECURITY_INTERVENTION_H_

#ifdef __cplusplus
namespace ModSecurity {
#endif

typedef struct ModSecurityIntervention_t {
    int status;
    int pause;
    const char *url;
    const char *log;
    int disruptive;
} ModSecurityIntervention;

#ifdef __cplusplus
}  // namespace ModSecurity
#endif

#endif  // HEADERS_MODSECURITY_INTERVENTION_H_
