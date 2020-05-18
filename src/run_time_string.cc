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
#include "src/rule_with_operator.h"


namespace modsecurity {


void RunTimeString::appendText(const std::string &text) {
    std::unique_ptr<RunTimeElementHolder> r(new RunTimeElementHolder);
    r->m_string = text;
    m_elements.push_back(std::move(r));
}


void RunTimeString::appendVar(
    std::unique_ptr<modsecurity::variables::Variable> var) {
    std::unique_ptr<RunTimeElementHolder> r(new RunTimeElementHolder);
    r->m_variable = std::move(var);
    m_elements.push_back(std::move(r));
    m_containsMacro = true;
}


std::string RunTimeString::evaluate(Transaction *transaction) {
    std::string retString;
    // FIXME: Educated guess the size of retString based on the size of the elements.
    for (auto &element : m_elements) {
        if (element->m_string.size() > 0) {
            retString.append(element->m_string);
        } else if (element->m_variable != nullptr && transaction != nullptr) {
            std::vector<const VariableValue *> l;
            element->m_variable->evaluate(transaction, &l);
            if (!l.empty()) {
                retString.append(l[0]->getValue());
            }
            for (auto &i : l) {
                delete i;
            }
        }
    }
    return retString;
}

}  // namespace modsecurity
