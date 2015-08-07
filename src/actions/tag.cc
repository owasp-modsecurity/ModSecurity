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

#include "actions/tag.h"

#include <iostream>
#include <string>

#include "actions/action.h"
#include "modsecurity/assay.h"
#include "src/utils.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace actions {

Tag::Tag(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind),
    m_tag(action) {
    m_tag.erase(0, 1);
    m_tag.pop_back();
}


bool Tag::evaluate(Rule *rule, Assay *assay) {
    std::string tag = MacroExpansion::expand(m_tag, assay);
    assay->debug(9, "Rule tag: " + tag);
    assay->ruleTags.push_back(tag);
    return true;
}

}  // namespace actions
}  // namespace ModSecurity
