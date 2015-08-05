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

namespace ModSecurity {
namespace actions {

Redirect::~Redirect() {
}

Redirect::Redirect(const std::string& action)
    : Action(action, RunTimeOnlyIfMatchKind),
    url(action) {
    this->url = this->url.erase(0, 9);
    this->status = 302;
}


bool Redirect::evaluate(Rule *rule, Assay *assay) {
    assay->actions.push_back(this);
    return true;
}

void Redirect::fill_intervention(ModSecurityIntervention *i) {
    /* if it was changed before, lets keep it. */
    if (i->status == 200) {
        i->status = this->status;
    }
    // reinterpret_cast<char *>
    i->url = this->url.c_str();  //** TODO: wheee */
    i->log = "Redirecting";
}

}  // namespace actions
}  // namespace ModSecurity
