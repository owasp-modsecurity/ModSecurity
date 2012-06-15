/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef _MSC_CRYPT_H_
#define _MSC_CRYPT_H_

#include "modsecurity.h"
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#define HMAC_PAD_SIZE 65
#define HASH_ONLY 0
#define FULL_LINK 1

#ifndef INT32_MAX
#define INT32_MAX      (2147483647)
#endif

struct glinear {
  uint32_t seed;
  uint32_t mod;
  uint32_t mul;
  uint32_t add;
};

char DSOLOCAL *hmac(modsec_rec *msr,const unsigned char *key, int key_len,
                     char *msg, int msglen);
unsigned char DSOLOCAL *do_hash_link(modsec_rec *msr, char *link,
                      int type);
unsigned char DSOLOCAL *getkey(apr_pool_t *mp);

int DSOLOCAL init_response_body_html_parser(modsec_rec *msr);
int DSOLOCAL encrypt_response_body_links(modsec_rec *msr);
int DSOLOCAL inject_encrypted_response_body(modsec_rec *msr, int elts);
int DSOLOCAL do_encryption_method(modsec_rec *msr, char *link, int type);
int DSOLOCAL modify_response_header(modsec_rec *msr);
char DSOLOCAL *normalize_path(modsec_rec *msr, char *input);
#endif
