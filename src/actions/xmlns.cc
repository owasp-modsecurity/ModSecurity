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

#include "actions/xmlns.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {


bool XmlNS::init(std::string *error) {
    size_t pos;
    std::string http = "http://";

    pos = m_parser_payload.find("=");
    if (pos == std::string::npos) {
        error->assign("XMLS: Bad format, missing equals sign.");
        return false;
    }
    m_name = std::string(m_parser_payload, 0, pos);
    m_value = std::string(m_parser_payload, pos+1, m_parser_payload.size());

    if (m_value.empty() || m_name.empty()) {
        error->assign("XMLS: XMLNS is invalid. Expecting a " \
            "name=value format.");
        return false;
    }

    if (m_value.at(0) == '\'' && m_value.size() > 3) {
        m_value.erase(0, 1);
        m_value.pop_back();
    }

    if (m_value.compare(0, http.length(), http) != 0) {
        error->assign("XMLS: Missing xmlns href for prefix: " \
            "`" + m_value + "'.");
        return false;
    }

    return true;
}


}  // namespace actions
}  // namespace modsecurity
