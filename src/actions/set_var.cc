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

#include "src/actions/set_var.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/macro_expansion.h"
#include "src/utils/string.h"


namespace modsecurity {
namespace actions {


bool SetVar::init(std::string *error) {
    size_t pos;

    // Resolv operation
    m_operation = setToOne;
    pos = m_parser_payload.find("=");
    if (pos != std::string::npos) {
        m_operation = setOperation;
    }
    pos = m_parser_payload.find("=+");
    if (pos != std::string::npos) {
        m_operation = sumAndSetOperation;
    }
    pos = m_parser_payload.find("=-");
    if (pos != std::string::npos) {
        m_operation = substractAndSetOperation;
    }

    // Collection name
    pos = m_parser_payload.find(".");
    if (pos != std::string::npos) {
        m_collectionName = std::string(m_parser_payload, 0, pos);
        m_collectionName = utils::string::toupper(m_collectionName);
    } else {
        error->assign("Missing the collection and/or variable name");
        return false;
    }

    // Variable name
    if (m_operation == setToOne) {
        m_variableName = std::string(m_parser_payload, pos + 1,
            m_parser_payload.length()
            - (pos + 1));
    } else {
        size_t pos2 = m_parser_payload.find("=");
        m_variableName = std::string(m_parser_payload, pos + 1,
            pos2 - (pos + 1));
        if (pos2 + 2 > m_parser_payload.length()) {
            m_predicate = "";
        } else {
            if (m_operation == setOperation) {
                m_predicate = std::string(m_parser_payload, pos2 + 1,
                    m_parser_payload.length() - (pos2));
            } else {
                m_predicate = std::string(m_parser_payload, pos2 + 2,
                    m_parser_payload.length()
                    - (pos2 + 1));
            }
        }
    }

    if (m_collectionName.empty() || m_variableName.empty()) {
        error->assign("Something wrong with the input format");
        return false;
    }

    return true;
}


bool SetVar::evaluate(Rule *rule, Transaction *transm_parser_payload) {
    std::string targetValue;
    std::string m_variableNameExpanded = MacroExpansion::expand(m_variableName,
        rule, transm_parser_payload);
    std::string resolvedPre = MacroExpansion::expand(m_predicate,
        rule, transm_parser_payload);

    if (m_operation == setOperation) {
        targetValue = resolvedPre;
    } else if (m_operation == setToOne) {
        targetValue = std::string("1");
    } else {
        int pre = 0;
        int value = 0;

        try {
            pre = stoi(resolvedPre);
        } catch (...) {
            pre = 0;
        }

        try {
            std::string *resolvedValue =
                transm_parser_payload->m_collections.resolveFirst(
                    m_collectionName,
                    m_variableNameExpanded);
            if (resolvedValue == NULL || resolvedValue->empty()) {
                value = 0;
            } else {
                value = stoi(*resolvedValue);
            }
        } catch (...) {
            value = 0;
        }

        if (m_operation == sumAndSetOperation) {
            targetValue = std::to_string(value + pre);
        } else if (m_operation == substractAndSetOperation) {
            targetValue = std::to_string(value - pre);
        }
    }

#ifndef NO_LOGS
    transm_parser_payload->debug(8, "Saving variable: " + m_collectionName \
        + ":" + m_variableNameExpanded + " with value: " + targetValue);
#endif
    transm_parser_payload->m_collections.storeOrUpdateFirst(m_collectionName,
        m_variableNameExpanded, targetValue);

    return true;
}

}  // namespace actions
}  // namespace modsecurity
