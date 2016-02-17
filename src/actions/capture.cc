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

#include "modsecurity/transaction.h"

#include "modsecurity/rule.h"
#include "operators/operator.h"
#include "operators/pm.h"
#include "operators/rx.h"
#include "operators/contains.h"
#include "operators/detect_sqli.h"

namespace modsecurity {
namespace actions {

bool Capture::evaluate(Rule *rule, Transaction *transaction) {
    if (transaction->m_matched.empty()) {
        return false;
    }

    int i = 0;
    while (transaction->m_matched.empty() == false) {
        transaction->m_collections.storeOrUpdateFirst("TX",
            std::to_string(i), transaction->m_matched.back());
        transaction->m_matched.pop_back();
        i++;
    }
    return true;
}

}  // namespace actions
}  // namespace modsecurity
