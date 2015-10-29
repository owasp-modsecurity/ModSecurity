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

#include "variables/variations/exclusion.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/assay.h"
#include "src/utils.h"

namespace ModSecurity {
namespace Variables {
namespace Variations {


std::list<transaction::Variable *> *
    Exclusion::evaluate(Assay *assay) {
    std::list<transaction::Variable *> *l =
        new std::list<transaction::Variable *>();
    assay->m_collections.resolve(this->name, l);
    return l;
}


}  // namespace Variations
}  // namespace Variables
}  // namespace ModSecurity
