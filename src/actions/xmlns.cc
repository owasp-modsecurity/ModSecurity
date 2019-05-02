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


#include "src/actions/xmlns.h"

#include <string>

#include "modsecurity/actions/action.h"


namespace modsecurity {
namespace actions {


bool XmlNS::init(std::string *error) {
    size_t pos;
    std::string http = "http://";

    pos = m_parserPayload.find("=");
    if (pos == std::string::npos) {
        error->assign("XMLS: Bad format, missing equals sign.");
        return false;
    }
    m_scope = std::string(m_parserPayload, 0, pos);
    m_href = std::string(m_parserPayload, pos+1, m_parserPayload.size());

    if (m_href.empty() || m_scope.empty()) {
        error->assign("XMLS: XMLNS is invalid. Expecting a " \
            "name=value format.");
        return false;
    }

    if (m_href.at(0) == '\'' && m_href.size() > 3) {
        m_href.erase(0, 1);
        m_href.pop_back();
    }

    if (m_href.compare(0, http.length(), http) != 0) {
        error->assign("XMLS: Missing xmlns href for prefix: " \
            "`" + m_href + "'.");
        return false;
    }

    return true;
}


}  // namespace actions
}  // namespace modsecurity
