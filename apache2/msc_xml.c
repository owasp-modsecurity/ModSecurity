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

#include "msc_xml.h"

static void msc_xml_on_start_elementns(
    void *ctx,
    const xmlChar *localname,
    const xmlChar *prefix,
    const xmlChar *URI,
    int nb_namespaces,
    const xmlChar **namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar **attributes
) {

    // get the length of XML tag (localname)
    size_t taglen = strlen((const char *)localname);
    modsec_rec * msr = (modsec_rec *)ctx;
    msc_xml_parser_state * xml_parser_state = msr->xml->xml_parser_state;

    // pathlen contains the concatenated strings of tags with '.'
    // eg xml.root.level1.leaf
    xml_parser_state->pathlen += (taglen + 1);
    char *newpath = apr_pstrcat(msr->mp, xml_parser_state->currpath, ".", (char *)localname, NULL);
    xml_parser_state->currpath = newpath;
    xml_parser_state->currpathbufflen += taglen + 1; // +1 for the '.' character here too

    int *new_stack_item = (int *)apr_array_push(xml_parser_state->has_child_stack);
    *new_stack_item = 0;
    xml_parser_state->depth++;
    // set the current value to null
    // this is necessary because if there is any text between the tags (new line, etc)
    // it will be added to the current value
    xml_parser_state->currval = NULL;
    xml_parser_state->currvalbufflen = 0;

    // if there is an item before the current one we set that has a child
    if (xml_parser_state->depth > 1) {
        int *parent_stack_item = &((int *)xml_parser_state->has_child_stack->elts)[xml_parser_state->has_child_stack->nelts - 2];
        *parent_stack_item = 1;
    }

}

static void msc_xml_on_end_elementns(
    void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI
) {

    size_t taglen = strlen((const char *)localname);
    modsec_rec * msr = (modsec_rec *)ctx;
    msc_xml_parser_state * xml_parser_state = msr->xml->xml_parser_state;

    // if the node is a leaf we add it as argument
    // get the top item from the stack which tells this info
    int * top_stack_item = apr_array_pop(xml_parser_state->has_child_stack);
    if (*top_stack_item == 0) {

        if (apr_table_elts(msr->arguments)->nelts >= msr->txcfg->arguments_limit) {
            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Skipping request argument, over limit (XML): name \"%s\", value \"%s\"",
                        log_escape_ex(msr->mp, xml_parser_state->currpath, xml_parser_state->currpathbufflen),
                        log_escape_ex(msr->mp,
                                      (xml_parser_state->currval == NULL ? apr_pstrndup(msr->mp, "", 0) : xml_parser_state->currval),
                                      xml_parser_state->currvalbufflen
                                     )
                        );
            }
            msr->msc_reqbody_error = 1;
            msr->xml->xml_error = apr_psprintf(msr->mp, "More than %ld ARGS (GET + XML)", msr->txcfg->arguments_limit);
            xmlStopParser((xmlParserCtxtPtr)msr->xml->parsing_ctx_arg);
        }
        else {

            msc_arg * arg = (msc_arg *) apr_pcalloc(msr->mp, sizeof(msc_arg));

            arg->name = xml_parser_state->currpath;
            arg->name_len = xml_parser_state->currpathbufflen;
            arg->value = (xml_parser_state->currval == NULL) ? apr_pstrndup(msr->mp, "", 0) : xml_parser_state->currval;
            arg->value_len = xml_parser_state->currvalbufflen;
            arg->value_origin_len = arg->value_len;
            arg->origin = "XML";

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Adding XML argument '%s' with value '%s'",
                    arg->name, arg->value);
            }

            apr_table_addn(msr->arguments,
                log_escape_nq_ex(msr->mp, arg->name, arg->name_len), (void *) arg);
        } // end else
    } // end top_stack_item == 0

    // decrease the length of current path length - +1 because of the '\0'
    xml_parser_state->pathlen -= (taglen + 1);

    // -1 is needed because we don't need the last '.'
    char * newpath = apr_pstrndup(msr->mp, xml_parser_state->currpath, xml_parser_state->pathlen - 1);
    xml_parser_state->currpath = newpath;
    xml_parser_state->currpathbufflen = xml_parser_state->pathlen - 1;

    xml_parser_state->depth--;
    xml_parser_state->currval = NULL;
    xml_parser_state->currvalbufflen = 0;
}

static void msc_xml_on_characters(void *ctx, const xmlChar *ch, int len) {

    modsec_rec * msr = (modsec_rec *)ctx;
    msc_xml_parser_state * xml_parser_state = msr->xml->xml_parser_state;

    // libxml2 SAX parser will call this function multiple times
    // during the parsing of a single node, if the value has multibyte
    // characters, so we need to concatenate the values
    xml_parser_state->currval = apr_pstrcat(msr->mp,
                                            ((xml_parser_state->currval != NULL) ? xml_parser_state->currval : ""),
                                            apr_pstrndup(msr->mp, (const char *)ch, len),
                                            NULL);
    xml_parser_state->currvalbufflen += len;
    // check if the memory allocation was successful
    if (xml_parser_state->currval == NULL) {
        msr->xml->xml_error = apr_psprintf(msr->mp, "Failed to allocate memory for XML value.");
        xmlStopParser((xmlParserCtxtPtr)msr->xml->parsing_ctx_arg);
    }

}


static xmlParserInputBufferPtr
xml_unload_external_entity(const char *URI, xmlCharEncoding enc)    {
    return NULL;
}


/**
 * Initialise XML parser.
 */
int xml_init(modsec_rec *msr, char **error_msg) {
    assert(msr != NULL);
    assert(error_msg != NULL);
    xmlParserInputBufferCreateFilenameFunc entity;

    *error_msg = NULL;

    msr->xml = apr_pcalloc(msr->mp, sizeof(xml_data));
    if (msr->xml == NULL) return -1;

    if(msr->txcfg->xml_external_entity == 0)    {
        entity = xmlParserInputBufferCreateFilenameDefault(xml_unload_external_entity);
    }

    if (msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_OFF) {

        msr->xml->sax_handler = (xmlSAXHandler *)apr_pcalloc(msr->mp, sizeof(xmlSAXHandler));
        memset(msr->xml->sax_handler, 0, sizeof(xmlSAXHandler));
        if (msr->xml->sax_handler == NULL) {
            *error_msg = apr_psprintf(msr->mp, "XML: Failed to create SAX handler.");
            return -1;
        }

        msr->xml->sax_handler->initialized = XML_SAX2_MAGIC;
        msr->xml->sax_handler->startElementNs = msc_xml_on_start_elementns;
        msr->xml->sax_handler->endElementNs = msc_xml_on_end_elementns;
        msr->xml->sax_handler->characters = msc_xml_on_characters;

        // set the parser state struct
        msr->xml->xml_parser_state                  = apr_pcalloc(msr->mp, sizeof(msc_xml_parser_state));
        msr->xml->xml_parser_state->depth           = 0;
        msr->xml->xml_parser_state->pathlen         = 4; // "xml\0"
        msr->xml->xml_parser_state->currpath        = apr_pstrdup(msr->mp, "xml");
        msr->xml->xml_parser_state->currpathbufflen = 3; // "xml"
        msr->xml->xml_parser_state->currval         = NULL;
        msr->xml->xml_parser_state->currvalbufflen  = 0;
        // initialize the stack with item of 10
        // this will store the information about nodes
        // 10 is just an initial value, it can be automatically incremented
        msr->xml->xml_parser_state->has_child_stack = apr_array_make(msr->mp, 10, sizeof(int));
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
    assert(msr != NULL);
    assert(error_msg != NULL);
    *error_msg = NULL;

    /* We want to initialise our parsing context here, to
     * enable us to pass it the first chunk of data so that
     * it can attempt to auto-detect the encoding.
     */
    if (msr->xml->parsing_ctx == NULL && msr->xml->parsing_ctx_arg == NULL) {

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

        if (msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_ONLYARGS) {
            msr->xml->parsing_ctx = xmlCreatePushParserCtxt(NULL, NULL, buf, size, "body.xml");
            if (msr->xml->parsing_ctx == NULL) {
                *error_msg = apr_psprintf(msr->mp, "XML: Failed to create parsing context.");
                return -1;
            }
        }
        if (msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_OFF) {
            msr->xml->parsing_ctx_arg = xmlCreatePushParserCtxt(
                msr->xml->sax_handler,
                msr,
                buf,
                size,
                NULL);
            if (msr->xml->parsing_ctx_arg == NULL) {
                *error_msg = apr_psprintf(msr->mp, "XML: Failed to create parsing context for ARGS.");
                return -1;
            }
        }
    } else {

        /* Not a first invocation. */
        msr_log(msr, 4, "XML: Continue parsing.");
        if (msr->xml->parsing_ctx != NULL &&
            msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_ONLYARGS) {
            xmlParseChunk(msr->xml->parsing_ctx, buf, size, 0);
            if (msr->xml->parsing_ctx->wellFormed != 1) {
                *error_msg = apr_psprintf(msr->mp, "XML: Failed to parse document.");
                return -1;
            }
        }

        if (msr->xml->parsing_ctx_arg != NULL &&
            msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_OFF) {
            if (xmlParseChunk(msr->xml->parsing_ctx_arg, buf, size, 0) != 0) {
                if (msr->xml->xml_error) {
                    *error_msg = msr->xml->xml_error;
                }
                else {
                    *error_msg = apr_psprintf(msr->mp, "XML: Failed to parse document for ARGS.");
                }
                return -1;
            }
        }
        if (msr->xml->xml_error) {
            *error_msg = msr->xml->xml_error;
            return -1;
        }
    }

    return 1;
}

/**
 * Finalise XML parsing.
 */
int xml_complete(modsec_rec *msr, char **error_msg) {
    assert(msr != NULL);
    assert(error_msg != NULL);
    *error_msg = NULL;

    /* Only if we have a context, meaning we've done some work. */
    if (msr->xml->parsing_ctx != NULL || msr->xml->parsing_ctx_arg != NULL) {
        if (msr->xml->parsing_ctx != NULL &&
            msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_ONLYARGS) {
            /* This is how we signal the end of parsing to libxml. */
            xmlParseChunk(msr->xml->parsing_ctx, NULL, 0, 1);

            /* Preserve the results for our reference. */
            msr->xml->well_formed = msr->xml->parsing_ctx->wellFormed;
            msr->xml->doc = msr->xml->parsing_ctx->myDoc;

            /* Clean up everything else. */
            xmlFreeParserCtxt(msr->xml->parsing_ctx);
            msr->xml->parsing_ctx = NULL;
            msr_log(msr, 4, "XML: Parsing complete (well_formed %u).", msr->xml->well_formed);

            if (msr->xml->well_formed != 1) {
                *error_msg = apr_psprintf(msr->mp, "XML: Failed to parse document.");
                return -1;
            }
        }

        if (msr->xml->parsing_ctx_arg != NULL &&
            msr->txcfg->parse_xml_into_args != MSC_XML_ARGS_OFF) {
            if (xmlParseChunk(msr->xml->parsing_ctx_arg, NULL, 0, 1) != 0) {
                if (msr->xml->xml_error) {
                    *error_msg = msr->xml->xml_error;
                }
                else {
                    *error_msg = apr_psprintf(msr->mp, "XML: Failed to parse document for ARGS.");
                }
                xmlFreeParserCtxt(msr->xml->parsing_ctx_arg);
                msr->xml->parsing_ctx_arg = NULL;
                return -1;
            }
            xmlFreeParserCtxt(msr->xml->parsing_ctx_arg);
            msr->xml->parsing_ctx_arg = NULL;
        }

    }

    return 1;
}

/**
 * Frees the resources used for XML parsing.
 */
apr_status_t xml_cleanup(modsec_rec *msr) {
    assert(msr != NULL);
    assert(msr->xml != NULL);
    if (msr->xml->parsing_ctx != NULL) {
        if (msr->xml->parsing_ctx->myDoc) {
            xmlFreeDoc(msr->xml->parsing_ctx->myDoc);
            if (msr->xml->parsing_ctx->myDoc == msr->xml->doc) {
                msr->xml->doc = NULL;
            }
        }
        xmlFreeParserCtxt(msr->xml->parsing_ctx);
        msr->xml->parsing_ctx = NULL;
    }
    if (msr->xml->parsing_ctx_arg != NULL) {

        if (msr->xml->parsing_ctx_arg->myDoc) {
            xmlFreeDoc(msr->xml->parsing_ctx_arg->myDoc);
        }

        xmlFreeParserCtxt(msr->xml->parsing_ctx_arg);
        msr->xml->parsing_ctx_arg = NULL;
    }
    if (msr->xml->doc != NULL) {
        xmlFreeDoc(msr->xml->doc);
        msr->xml->doc = NULL;
    }

    return 1;
}
