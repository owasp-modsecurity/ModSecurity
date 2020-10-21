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

#include "src/operators/contains_word.h"

#include <string>

#include "src/operators/operator.h"
#include "modsecurity/rule_message.h"

namespace modsecurity {
namespace operators {

bool ContainsWord::acceptableChar(const std::string& a, size_t pos) {
    if (a.size() - 1 < pos) {
        return false;
    }

    if ((a.at(pos) >= 65 && a.at(pos) <= 90) ||
        (a.at(pos) >= 97 && a.at(pos) <= 122)) {
        return false;
    }

    return true;
}

bool ContainsWord::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string &str, RuleMessage *ruleMessage) {
    std::string paramTarget(m_string->evaluate(transaction));

    if (paramTarget.empty()) {
        return true;
    }
    if (str.empty()) {
        return false;
    }
    if (str == paramTarget) {
        return true;
    }

    size_t pos = str.find(paramTarget);
    while (pos != std::string::npos) {
        if (pos == 0 && acceptableChar(str, paramTarget.size())) {
            logOffset(ruleMessage, 0, paramTarget.size());
            return true;
        }
        if (pos + paramTarget.size() == str.size() &&
            acceptableChar(str, pos - 1)) {
            logOffset(ruleMessage, pos, paramTarget.size());
            return true;
        }
        if (acceptableChar(str, pos - 1) &&
            acceptableChar(str, pos + paramTarget.size())) {
            logOffset(ruleMessage, pos, paramTarget.size());
            return true;
        }
        pos = str.find(paramTarget, pos + 1);
    }

    return false;
}


}  // namespace operators
}  // namespace modsecurity
