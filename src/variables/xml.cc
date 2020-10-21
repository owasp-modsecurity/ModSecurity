/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/variables/xml.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef WITH_LIBXML2
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/transaction.h"
#include "modsecurity/rules_set_properties.h"
#include "modsecurity/rules_set.h"

#include "src/request_body_processor/xml.h"
#include "modsecurity/actions/action.h"
#include "src/actions/xmlns.h"
#include "src/rule_with_actions.h"


namespace modsecurity {
namespace variables {

#ifndef WITH_LIBXML2
void XML_WithNSPath::evaluate(const Transaction *t,
    VariableValues *l) const noexcept { }
#else

void XML_WithNSPath::evaluate(const Transaction *t,
    VariableValues *l) const noexcept {
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    xmlNodeSetPtr nodes;
    std::string param;
    const xmlChar* xpathExpr = NULL;
    int i;
    //size_t pos;

    param = *getVariableKey();
    /*
    pos = m_name.find_first_of(":");
    if (pos == std::string::npos) {
        param = "";
    } else {
        param = std::string(m_name, pos+1, m_name.length() - (pos + 1));
    }
    */
    /* Is there an XML document tree at all? */
    if (t->m_xml->m_data.doc == NULL) {
        /* Sorry, we've got nothing to give! */
        return;
    }

    /* Process the XPath expression. */
    xpathExpr = (const xmlChar*)param.c_str();
    xpathCtx = xmlXPathNewContext(t->m_xml->m_data.doc);
    if (xpathCtx == NULL) {
        ms_dbg_a(t, 1, "XML: Unable to create new XPath context. : ");
        return;
    }

    if (getRule() == NULL) {
        ms_dbg_a(t, 2, "XML: Can't look for xmlns, internal error.");
    } else {
        XmlNSsPtr acts = getRule()->getXmlNSsPtr();
        for (auto &z : acts) {
            if (xmlXPathRegisterNs(xpathCtx, (const xmlChar*)z->getScope().c_str(),
                    (const xmlChar*)z->getHref().c_str()) != 0) {
                ms_dbg_a(t, 1, "Failed to register XML namespace href \"" + \
                    z->getHref() + "\" prefix \"" + z->getScope() + "\".");
                return;
            }

            ms_dbg_a(t, 4, "Registered XML namespace href \"" + z->getHref() + \
                "\" prefix \"" + z->getScope() + "\"");
        }
    }

    /* Initialise XPath expression. */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL) {
        ms_dbg_a(t, 1, "XML: Unable to evaluate xpath expression.");
        xmlXPathFreeContext(xpathCtx);
        return;
    }
    /* Evaluate XPath expression. */
    nodes = xpathObj->nodesetval;
    if (nodes == NULL) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return;
    }
    /* Create one variable for each node in the result. */
    for (i = 0; i < nodes->nodeNr; i++) {
        char *content;
        content = reinterpret_cast<char *>(
            xmlNodeGetContent(nodes->nodeTab[i]));
        if (content != NULL) {
            std::string a(content);
            if (!m_keyExclusion.toOmit(*getVariableKeyWithCollection())) {
                l->push_back(std::make_shared<VariableValue>(getVariableKeyWithCollection().get(), &a));
            }
            xmlFree(content);
         }
    }
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
}

#endif

}  // namespace variables
}  // namespace modsecurity
