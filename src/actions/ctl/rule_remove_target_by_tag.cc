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

#include "src/actions/ctl/rule_remove_target_by_tag.h"

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


bool RuleRemoveTargetByTag::init(std::string *error) {
    std::string what(m_parser_payload, 22, m_parser_payload.size() - 22);
    std::vector<std::string> param = utils::string::split(what, ';');

    if (param.size() < 2) {
        error->assign(what + " is not a valid `TAG;VARIABLE'");
        return false;
    }

    m_tag = param[0];
    m_target = param[1];

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
                    error->assign("Invalid regex in ctl:ruleRemoveTargetByTag: " +
                        m_target);
                    return false;
                }
            }
        }
    }

    return true;
}

bool RuleRemoveTargetByTag::evaluate(RuleWithActions *rule, Transaction *transaction) {
    RuleRemoveTargetByTagEntry entry;
    entry.tag = m_tag;
    entry.target.literal = m_target;
    entry.target.regex = m_regex;
    transaction->m_ruleRemoveTargetByTag.push_back(std::move(entry));
    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
