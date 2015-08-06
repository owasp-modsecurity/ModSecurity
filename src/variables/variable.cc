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

#include "variables/variable.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "modsecurity/assay.h"
#include "variations/exclusion.h"

using ModSecurity::Variables::Variations::Exclusion;

namespace ModSecurity {
namespace Variables {

std::list<std::pair<std::string, std::string>>
    Variable::evaluate(Assay *assay) {
    return assay->resolve_variable(this->name);
}

std::string Variable::to_s(
    std::vector<Variable *> *variables) {
    std::string ret;
    std::string except("");
    for (int i = 0; i < variables->size() ; i++) {
        std::string name = variables->at(i)->name;
        Exclusion *e =  dynamic_cast<Exclusion *>(variables->at(i));
        if (e != NULL) {
            if (except.empty()) {
                except = except + name;
            } else {
                except = except + "|" + name;
            }
            continue;
        }

        if (i == 0) {
            ret = ret + name;
        } else {
            ret = ret + "|" + name;
        }
    }

    if (except.empty() == false) {
        ret = ret + ", except for: " + except;
    }
    return ret;
}


}  // namespace Variables
}  // namespace ModSecurity
