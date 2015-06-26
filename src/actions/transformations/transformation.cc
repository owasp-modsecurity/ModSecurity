/**
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

#include "actions/transformations/transformation.h"

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "actions/transformations/trim.h"
#include "actions/transformations/lowercase.h"
#include "actions/action.h"

namespace ModSecurity {
namespace actions {
namespace transformations {

Transformation::Transformation(std::string action)
    : Action(action) {
    this->name = this->action;
    this->name.erase(0, 2);
    this->action_kind = 1;
}

std::string &Transformation::evaluate(std::string value,
    Assay *assay) {
    return value;
}

Transformation* Transformation::instantiate(std::string a) {
    if (a == "t:trim") {
        return new transformations::Trim(a);
    }
    if (a == "t:lowercase") {
        return new LowerCase(a);
    }

    return new Transformation(a);
}

}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
