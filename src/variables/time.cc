/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/variables/time.h"

#include "utils/time_format.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/transaction.h"

namespace modsecurity {
namespace variables {

void Time::evaluate(Transaction *transaction,
    RuleWithActions *rule,
    VariableValueList *l) {

    transaction->m_variableTime = get_formatted_time_string_now<200>("%H:%M:%S");

    l->emplace_back(&m_retName,
        &transaction->m_variableTime);
}


}  // namespace variables
}  // namespace modsecurity
