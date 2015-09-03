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

#include "actions/capture.h"

#include <iostream>
#include <string>
#include <list>

#include "modsecurity/assay.h"

#include "src/rule.h"
#include "operators/operator.h"
#include "operators/pm.h"
#include "operators/rx.h"
#include "operators/contains.h"

namespace ModSecurity {
namespace actions {

bool Capture::evaluate(Rule *rule, Assay *assay) {
    operators::Operator *op = rule->op;
    std::list<std::string> match;

    operators::Pm *pm = dynamic_cast<operators::Pm *>(op);
    operators::Rx *rx = dynamic_cast<operators::Rx *>(op);
    operators::Contains *contains = dynamic_cast<operators::Contains *>(op);

    if (pm != NULL) {
        match = pm->matched;
    }

    if (rx != NULL) {
        match = rx->matched;
    }

    if (contains != NULL) {
        match = contains->matched;
    }

    if (match.empty()) {
        return false;
    }

    int i = 0;
    while (match.empty() == false) {
        assay->setCollection("TX", std::to_string(i), match.back());
        match.pop_back();
        i++;
    }

    return true;
}

}  // namespace actions
}  // namespace ModSecurity
