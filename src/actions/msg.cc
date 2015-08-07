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

#include "actions/msg.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/assay.h"
#include "src/utils.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace actions {

Msg::Msg(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind),
    m_msg(action) {
    m_msg.erase(0, 1);
    m_msg.pop_back();
}


bool Msg::evaluate(Rule *rule, Assay *assay) {
    std::string msg = MacroExpansion::expand(m_msg, assay);
    assay->debug(9, "Saving msg: " + msg);
    assay->rulesMessages.push_back(msg);
    assay->serverLog(msg);
    return true;
}

}  // namespace actions
}  // namespace ModSecurity
