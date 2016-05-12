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

#include "variables/xml.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/transaction.h"

#include "src/request_body_processor/xml.h"

namespace modsecurity {
namespace Variables {

void XML::evaluateInternal(Transaction *t,
    std::vector<const collection::Variable *> *l) {
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    xmlNodeSetPtr nodes;
    std::string key;
    std::string param;
    const xmlChar* xpathExpr = NULL;
    int i;
    size_t pos;

    pos = m_name.find_first_of(":");
    if (pos == std::string::npos) {
        key = std::string(m_name, 0);
        param = "";
    } else {
        key = std::string(m_name, 0, pos);
        param = std::string(m_name, pos+1, m_name.length() - (pos + 1));
    }

    /* Is there an XML document tree at all? */
    if (t->m_xml->m_data.doc == NULL) {
        /* Sorry, we've got nothing to give! */
        return;
    }
    if (param.empty() == true) {
        /* Invocation without an XPath expression makes sense
         * with functions that manipulate the document tree.
         */
        l->push_back(new collection::Variable("XML",
            std::string("[XML document tree]" + param)));
        return;
    }
    /* Process the XPath expression. */
    xpathExpr = (const xmlChar*)param.c_str();
    xpathCtx = xmlXPathNewContext(t->m_xml->m_data.doc);
    if (xpathCtx == NULL) {
        t->debug(1, "XML: Unable to create new XPath context.");
        return;
    }
#if 0
    /* Look through the actionset of the associated rule
     * for the namespace information. Register them if any are found.
     */
    tarr = apr_table_elts(rule->actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_action *action = (msre_action *)telts[i].val;

        if (strcasecmp(action->metadata->name, "xmlns") == 0) {
            char *prefix, *href;

            if (parse_name_eq_value(mptmp, action->param, &prefix, &href) < 0) return -1;
            if ((prefix == NULL)||(href == NULL)) return -1;

            if(xmlXPathRegisterNs(xpathCtx, (const xmlChar*)prefix, (const xmlChar*)href) != 0) {
                msr_log(msr, 1, "Failed to register XML namespace href \"%s\" prefix \"%s\".",
                    log_escape(mptmp, prefix), log_escape(mptmp, href));
                return -1;
            }

            msr_log(msr, 4, "Registered XML namespace href \"%s\" prefix \"%s\".",
                log_escape(mptmp, prefix), log_escape(mptmp, href));
        }
    }
#endif

    /* Initialise XPath expression. */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL) {
        t->debug(1, "XML: Unable to evaluate xpath expression.");
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
        char *content = NULL;
        content = reinterpret_cast<char *>(
            xmlNodeGetContent(nodes->nodeTab[i]));
        if (content != NULL) {
            l->push_back(new collection::Variable(m_name,
                std::string(content)));
            xmlFree(content);
         }
    }
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
}


}  // namespace Variables
}  // namespace modsecurity
