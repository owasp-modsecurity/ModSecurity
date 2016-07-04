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

#include "operators/validate_dtd.h"

#include <string>

#include "request_body_processor/xml.h"
#include "src/utils.h"
#include "operators/operator.h"

namespace modsecurity {
namespace operators {


bool ValidateDTD::init(const std::string &file, std::string *error) {
    m_resource = find_resource(param, file);
    if (m_resource == "") {
        error->assign("XML: File not found: " + param + ".");
        return false;
    }

    xmlThrDefSetGenericErrorFunc(NULL,
        null_error);

    xmlSetGenericErrorFunc(NULL,
        null_error);

    return true;
}


bool ValidateDTD::evaluate(Transaction *t, const std::string &str) {
    xmlValidCtxtPtr cvp;

    m_dtd = xmlParseDTD(NULL, (const xmlChar *)m_resource.c_str());
    if (m_dtd == NULL) {
        std::string err = std::string("XML: Failed to load DTD: ") \
            + m_resource;
        t->debug(4, err);
        return true;
    }

    if (t->m_xml->m_data.doc == NULL) {
        t->debug(4, "XML document tree could not "\
            "be found for DTD validation.");
        return true;
    }

    if (t->m_xml->m_data.well_formed != 1) {
        t->debug(4, "XML: DTD validation failed because " \
            "content is not well formed.");
        return true;
    }

#if 0
    /* Make sure there were no other generic processing errors */
    if (msr->msc_reqbody_error) {
        *error_msg = apr_psprintf(msr->mp,
                "XML: DTD validation could not proceed due to previous"
                " processing errors.");
        return 1;
    }
#endif

    cvp = xmlNewValidCtxt();
    if (cvp == NULL) {
        t->debug(4, "XML: Failed to create a validation context.");
        return true;
    }

    /* Send validator errors/warnings to msr_log */
    cvp->error = (xmlSchemaValidityErrorFunc)error_runtime;
    cvp->warning = (xmlSchemaValidityErrorFunc)warn_runtime;
    cvp->userData = t;

    if (!xmlValidateDtd(cvp, t->m_xml->m_data.doc, m_dtd)) {
        t->debug(4, "XML: DTD validation failed.");
        xmlFreeValidCtxt(cvp);
        return true;
    }

    t->debug(4, std::string("XML: Successfully validated " \
        "payload against DTD: ") + m_resource);

    xmlFreeValidCtxt(cvp);

    return false;
}


}  // namespace operators
}  // namespace modsecurity
