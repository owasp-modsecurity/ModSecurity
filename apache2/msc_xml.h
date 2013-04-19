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

#ifndef _MSC_XML_H_
#define _MSC_XML_H_

typedef struct xml_data xml_data;

#include "modsecurity.h"
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

/* Structures */

struct xml_data {
    xmlSAXHandler           *sax_handler;
    xmlParserCtxtPtr        parsing_ctx;
    xmlDocPtr               doc;

    unsigned int            well_formed;
};

/* Functions */

int DSOLOCAL xml_init(modsec_rec *msr, char **error_msg);

int DSOLOCAL xml_process_chunk(modsec_rec *msr, const char *buf,
    unsigned int size, char **error_msg);

int DSOLOCAL xml_complete(modsec_rec *msr, char **error_msg);

apr_status_t DSOLOCAL xml_cleanup(modsec_rec *msr);

#endif
