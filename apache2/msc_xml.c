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

#include "msc_xml.h"

static xmlParserInputBufferPtr
xml_unload_external_entity(const char *URI, xmlCharEncoding enc)    {
    return NULL;
}


/**
 * Initialise XML parser.
 */
int xml_init(modsec_rec *msr, char **error_msg) {
    xmlParserInputBufferCreateFilenameFunc entity;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    msr->xml = apr_pcalloc(msr->mp, sizeof(xml_data));
    if (msr->xml == NULL) return -1;

    if(msr->txcfg->xml_external_entity == 0)    {
        entity = xmlParserInputBufferCreateFilenameDefault(xml_unload_external_entity);
    }

    return 1;
}

#if 0
static void xml_receive_sax_error(void *data, const char *msg, ...) {
    modsec_rec *msr = (modsec_rec *)data;
    char message[256];

    if (msr == NULL) return;

    apr_snprintf(message, sizeof(message), "%s (line %d offset %d)",
        log_escape_nq(msr->mp, msr->xml->parsing_ctx->lastError.message),
        msr->xml->parsing_ctx->lastError.line,
        msr->xml->parsing_ctx->lastError.int2);

    msr_log(msr, 5, "XML: Parsing error: %s", message);
}
#endif

/**
 * Feed one chunk of data to the XML parser.
 */
int xml_process_chunk(modsec_rec *msr, const char *buf, unsigned int size, char **error_msg) {
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* We want to initialise our parsing context here, to
     * enable us to pass it the first chunk of data so that
     * it can attempt to auto-detect the encoding.
     */
    if (msr->xml->parsing_ctx == NULL) {

        /* First invocation. */

        msr_log(msr, 4, "XML: Initialising parser.");

        /* NOTE When Sax interface is used libxml will not
         *      create the document object, but we need it.

        msr->xml->sax_handler = (xmlSAXHandler *)apr_pcalloc(msr->mp, sizeof(xmlSAXHandler));
        if (msr->xml->sax_handler == NULL) return -1;
        msr->xml->sax_handler->error = xml_receive_sax_error;
        msr->xml->sax_handler->warning = xml_receive_sax_error;
        msr->xml->parsing_ctx = xmlCreatePushParserCtxt(msr->xml->sax_handler, msr,
            buf, size, "body.xml");

        */

        msr->xml->parsing_ctx = xmlCreatePushParserCtxt(NULL, NULL, buf, size, "body.xml");
        if (msr->xml->parsing_ctx == NULL) {
            *error_msg = apr_psprintf(msr->mp, "XML: Failed to create parsing context.");
            return -1;
        }
    } else {

        /* Not a first invocation. */

        xmlParseChunk(msr->xml->parsing_ctx, buf, size, 0);
        if (msr->xml->parsing_ctx->wellFormed != 1) {
            *error_msg = apr_psprintf(msr->mp, "XML: Failed parsing document.");
            return -1;
        }
    }

    return 1;
}

/**
 * Finalise XML parsing.
 */
int xml_complete(modsec_rec *msr, char **error_msg) {
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Only if we have a context, meaning we've done some work. */
    if (msr->xml->parsing_ctx != NULL) {
        /* This is how we signalise the end of parsing to libxml. */
        xmlParseChunk(msr->xml->parsing_ctx, NULL, 0, 1);

        /* Preserve the results for our reference. */
        msr->xml->well_formed = msr->xml->parsing_ctx->wellFormed;
        msr->xml->doc = msr->xml->parsing_ctx->myDoc;

        /* Clean up everything else. */
        xmlFreeParserCtxt(msr->xml->parsing_ctx);
        msr->xml->parsing_ctx = NULL;
        msr_log(msr, 4, "XML: Parsing complete (well_formed %u).", msr->xml->well_formed);

        if (msr->xml->well_formed != 1) {
            *error_msg = apr_psprintf(msr->mp, "XML: Failed parsing document.");
            return -1;
        }
    }

    return 1;
}

/**
 * Frees the resources used for XML parsing.
 */
apr_status_t xml_cleanup(modsec_rec *msr) {
    if (msr->xml->doc != NULL) {
        xmlFreeDoc(msr->xml->doc);
        msr->xml->doc = NULL;
    }

    return 1;
}
