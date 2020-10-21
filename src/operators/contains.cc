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

#include "src/operators/contains.h"

#include <string>


namespace modsecurity {
namespace operators {

bool Contains::evaluate(Transaction *transaction,
    RuleWithActions *rule,
    const bpstd::string_view &input,
    RuleMessage *ruleMessage) {
    std::string p(m_string->evaluate(transaction));
    size_t offset = input.to_string().find(p);

    bool contains = offset != std::string::npos;

    if (contains && transaction) {
        logOffset(ruleMessage, offset, p.size());
        transaction->m_matched.push_back(p);
    }

    return contains;
}

}  // namespace operators
}  // namespace modsecurity
