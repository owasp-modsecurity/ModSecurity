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

#include "variables/variations/count.h"

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
    Count::evaluate(Assay *assay) {
    std::list<transaction::Variable *> *reslIn;
    std::list<transaction::Variable *> *reslOut =
        new std::list<transaction::Variable *>();
    int count = 0;

    reslIn = var->evaluate(assay);

    for (auto &a : *reslIn) {
        count++;
    }

    while (reslIn->empty() == false) {
        delete reslIn->front();
        reslIn->pop_front();
    }
    delete reslIn;

    std::string res = std::to_string(count);

    reslOut->push_back(new transaction::Variable(std::string(var->name),
        std::string(res)));

    return reslOut;
}


}  // namespace Variations
}  // namespace Variables
}  // namespace ModSecurity
