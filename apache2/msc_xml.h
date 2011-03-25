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
