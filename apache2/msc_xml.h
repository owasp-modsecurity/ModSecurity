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
