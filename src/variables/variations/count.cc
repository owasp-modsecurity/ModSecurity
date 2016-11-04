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

#include "src/variables/variations/count.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/transaction.h"


namespace modsecurity {
namespace Variables {
namespace Variations {


void Count::evaluateInternal(Transaction *transaction,
    std::vector<const collection::Variable *> *l) {
    std::vector<const collection::Variable *> *reslIn;
    int count = 0;

    reslIn = var->evaluate(transaction);

    for (auto &a : *reslIn) {
        count++;
    }

    while (reslIn->empty() == false) {
        delete reslIn->back();
        reslIn->pop_back();
    }
    delete reslIn;

    std::string res = std::to_string(count);

    l->push_back(new collection::Variable(std::string(var->m_name),
        std::string(res)));
}


}  // namespace Variations
}  // namespace Variables
}  // namespace modsecurity
