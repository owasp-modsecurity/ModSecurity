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

#include "actions/severity.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/assay.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {

Severity::Severity(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind) {
    std::string a = action;
    if (tolower(a) == "emergency") {
        this->m_severity = 0;
    } else if (tolower(a) == "alert") {
        this->m_severity = 1;
    } else if (tolower(a) == "critical") {
        this->m_severity = 2;
    } else if (tolower(a) == "error") {
        this->m_severity = 3;
    } else if (tolower(a) == "warning") {
        this->m_severity = 4;
    } else if (tolower(a) == "notice") {
        this->m_severity = 5;
    } else if (tolower(a) == "info") {
        this->m_severity = 6;
    } else if (tolower(a) == "debug") {
        this->m_severity = 7;
    } else {
        this->m_severity = std::stod(a);
    }
}


bool Severity::evaluate(Rule *rule, Assay *assay) {
#ifndef NO_LOGS
    assay->debug(9, "This rule severity is: " + \
        std::to_string(this->m_severity) + " current assay is: " + \
        std::to_string(assay->highest_severity));
#endif

    if (assay->highest_severity > this->m_severity) {
        assay->highest_severity = this->m_severity;
    }
    return true;
}

}  // namespace actions
}  // namespace ModSecurity
