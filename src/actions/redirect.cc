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

#include "actions/redirect.h"

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "src/macro_expansion.h"

namespace ModSecurity {
namespace actions {

Redirect::~Redirect() {
}

Redirect::Redirect(const std::string& action)
    : Action(action, RunTimeOnlyIfMatchKind),
    m_url(action) {
    //m_url = m_url.erase(0, 9);
    if (m_url.at(0) == '\'') {
        m_url.erase(0, 1);
        if (m_url.size() > 0) {
            m_url.pop_back();
        }
    }
    m_status = 302;
}


bool Redirect::evaluate(Rule *rule, Assay *assay) {
    m_urlExpanded = MacroExpansion::expand(m_url, assay);
    assay->actions.push_back(this);
    return true;
}

void Redirect::fill_intervention(ModSecurityIntervention *i) {
    /* if it was changed before, lets keep it. */
    if (i->status == 200) {
        i->status = m_status;
    }
    i->url = m_urlExpanded.c_str();
    i->log = "Redirecting";
    i->disruptive = true;
}

}  // namespace actions
}  // namespace ModSecurity
