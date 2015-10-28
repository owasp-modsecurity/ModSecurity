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

#include "variables/highest_severity.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/assay.h"

namespace ModSecurity {
namespace Variables {

std::list<transaction::Variable *> *
    HighestSeverity::evaluate(Assay *assay) {
    std::list<transaction::Variable *> *resl =
        new std::list<transaction::Variable *>();

    resl->push_back(new transaction::Variable("HIGHEST_SEVERITY",
        std::to_string(assay->highest_severity)));

    return resl;
}


}  // namespace Variables
}  // namespace ModSecurity
