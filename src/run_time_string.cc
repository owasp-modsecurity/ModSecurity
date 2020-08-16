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


#include <memory>

#include "src/run_time_string.h"

#include "modsecurity/variable_value.h"
#include "modsecurity/transaction.h"
#include "src/variables/rule.h"
#include "src/variables/tx.h"
#include "src/variables/highest_severity.h"
#include "src/utils/string.h"
#include "src/variables/variable.h"


namespace modsecurity {


void RunTimeString::appendText(const std::string &text) {
    m_elements.emplace_back(text);
}


void RunTimeString::appendVar(
    std::unique_ptr<modsecurity::variables::Variable> var) {
    m_elements.emplace_back(var);
    m_containsMacro = true;
}


std::string RunTimeString::evaluate(Transaction *t) {
    return evaluate(t, NULL);
}


std::string RunTimeString::evaluate(Transaction *t, Rule *r) {
    std::string s;
    for (auto &element : m_elements) {
        if (element.m_string.size()) {
            s.append(element.m_string);
        } else if (element.m_var != NULL && t != NULL) {
            VariableValueList l;
            // FIXME: This cast should be removed.
            RuleWithOperator *rr = dynamic_cast<RuleWithOperator *>(r);
            element.m_var->evaluate(t, rr, &l);
            if (l.size() > 0) {
                s.append(l.front().getValue());
            }
        }
    }
    return s;
}

}  // namespace modsecurity
