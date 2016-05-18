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

#include "operators/validate_schema.h"

#include <string>

#include "operators/operator.h"
#include "request_body_processor/xml.h"
#include "src/utils.h"


namespace modsecurity {
namespace operators {

bool ValidateSchema::init(const std::string &file, const char **error) {
    m_resource = find_resource(param, file);
    if (m_resource == "") {
        std::string f("XML: File not found: " + param + ".");
        *error = strdup(f.c_str());
        return false;
    }

    return true;
}


bool ValidateSchema::evaluate(Transaction *t,
    const std::string &str) {
    int rc;

    m_parserCtx = xmlSchemaNewParserCtxt(m_resource.c_str());
    if (m_parserCtx == NULL) {
        std::stringstream err;
        err << "XML: Failed to load Schema from file: ";
        err << m_resource;
        err << ". ";
        if (m_err.empty() == false) {
            err << m_err;
        }
        t->debug(4, err.str());
        return true;
    }

    xmlSchemaSetParserErrors(m_parserCtx,
        (xmlSchemaValidityErrorFunc)error_load,
        (xmlSchemaValidityWarningFunc)warn_load, &m_err);

    xmlThrDefSetGenericErrorFunc(m_parserCtx,
        null_error);

    xmlSetGenericErrorFunc(m_parserCtx,
        null_error);

    m_schema = xmlSchemaParse(m_parserCtx);
    if (m_schema == NULL) {
        std::stringstream err;
        err << "XML: Failed to load Schema: ";
        err << m_resource;
        err << ".";
        if (m_err.empty() == false) {
            err << " " << m_err;
        }
        t->debug(4, err.str());
        xmlSchemaFreeParserCtxt(m_parserCtx);
        return true;
    }

    m_validCtx = xmlSchemaNewValidCtxt(m_schema);
    if (m_validCtx == NULL) {
        std::stringstream err("XML: Failed to create validation context.");
        if (m_err.empty() == false) {
            err << " " << m_err;
        }
        t->debug(4, err.str());
        return true;
    }

    /* Send validator errors/warnings to msr_log */
    xmlSchemaSetValidErrors(m_validCtx,
        (xmlSchemaValidityErrorFunc)error_runtime,
        (xmlSchemaValidityWarningFunc)warn_runtime, t);

    if (t->m_xml->m_data.doc == NULL) {
        t->debug(4, "XML document tree could not be found for " \
            "schema validation.");
        return true;
    }

    if (t->m_xml->m_data.well_formed != 1) {
        t->debug(4, "XML: Schema validation failed because " \
            "content is not well formed.");
        return true;
    }

    /* Make sure there were no other generic processing errors */
    /*
    if (msr->msc_reqbody_error) {
        t->debug(4, "XML: Schema validation could not proceed due to previous"
                " processing errors.");
        return true;
    }
    */

    rc = xmlSchemaValidateDoc(m_validCtx, t->m_xml->m_data.doc);
    if (rc != 0) {
       t->debug(4, "XML: Schema validation failed.");
        xmlSchemaFree(m_schema);
        xmlSchemaFreeParserCtxt(m_parserCtx);
        return true; /* No match. */
    }

    t->debug(4, "XML: Successfully validated payload against " \
        "Schema: " + m_resource);

    return false;
}


}  // namespace operators
}  // namespace modsecurity
