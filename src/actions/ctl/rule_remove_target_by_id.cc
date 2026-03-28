/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/actions/ctl/rule_remove_target_by_id.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "modsecurity/transaction.h"
#include "modsecurity/rule_remove_target_entry.h"
#include "src/utils/string.h"
#include "src/utils/regex.h"


namespace modsecurity {
namespace actions {
namespace ctl {


bool RuleRemoveTargetById::init(std::string *error) {
    std::string what(m_parser_payload, 21, m_parser_payload.size() - 21);
    std::vector<std::string> param = utils::string::split(what, ';');

    if (param.size() < 2) {
        error->assign(what + " is not a valid `ID;VARIABLE'");
        return false;
    }

    try {
        m_id = std::stoi(param[0]);
    } catch(...) {
        error->assign("Not able to convert '" + param[0] +
            "' into a number");
        return false;
    }

    m_target = param[1];

    // Detect regex format: COLLECTION:/pattern/ (e.g. ARGS:/mixpanel$/)
    if (m_target.size() >= 4) {
        size_t colon = m_target.find(':');
        if (colon != std::string::npos && colon + 2 < m_target.size() &&
            m_target[colon + 1] == '/' && m_target[m_target.size() - 1] == '/') {
            size_t pattern_start = colon + 2;
            size_t pattern_end = m_target.size() - 1;
            if (pattern_end > pattern_start) {
                std::string pattern = m_target.substr(pattern_start,
                    pattern_end - pattern_start);
                m_regex = std::make_unique<Utils::Regex>(pattern, true);
                if (m_regex->hasError()) {
                    error->assign("Invalid regex in ctl:ruleRemoveTargetById: " +
                        m_target);
                    return false;
                }
            }
        }
    }

    return true;
}

bool RuleRemoveTargetById::evaluate(RuleWithActions *rule, Transaction *transaction) {
    RuleRemoveTargetByIdEntry entry;
    entry.id = m_id;
    entry.target.literal = m_target;
    entry.target.regex = m_regex;
    transaction->m_ruleRemoveTargetById.push_back(std::move(entry));
    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
