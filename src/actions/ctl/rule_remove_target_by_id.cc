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


#include "src/actions/ctl/rule_remove_target_by_id.h"

#include <string>
#include <vector>
#include <utility>

#include "modsecurity/transaction.h"

#include "src/utils/string.h"


namespace modsecurity {
namespace actions {
namespace ctl {


bool RuleRemoveTargetById::init(std::string *error) {
    std::string what(m_parserPayload, 21, m_parserPayload.size() - 21);
    std::vector<std::string> param = utils::string::split(what, ';');

    if (param.size() < 2) {
        error->assign(what + " is not a valid `ID;VARIABLE'");
        return false;
    }

    size_t dash = param[0].find('-');
    if (dash != std::string::npos) {
        std::string n1s = std::string(param[0], 0, dash);
        std::string n2s = std::string(param[0], dash + 1, param[0].size() - (dash + 1));
        int n1n = 0;
        int n2n = 0;
        try {
            n1n = std::stoi(n1s);
        } catch(...) {
            error->assign("Not a number: " + n1s);
            return false;
        }
        try {
            n2n = std::stoi(n2s);
        } catch(...) {
            error->assign("Not a number: " + n2s);
            return false;
        }
        if (n1n > n2n) {
            error->assign("Invalid range: " + param[0]);
        }
        m_id = n1n;
        m_id_end_of_range = n2n;
    } else {
        try {
            m_id = std::stoi(param[0]);
        } catch(...) {
            error->assign("Not able to convert '" + param[0] + "' into a number");
            return false;
        }
    }

    m_target = param[1];

    return true;
}


bool RuleRemoveTargetById::execute(Transaction *transaction) const noexcept {
    if (m_id_end_of_range == 0) {
        transaction->m_ruleRemoveTargetById.push_back(std::make_pair(m_id, m_target));
    } else {
        std::pair<int, int> id_range = std::make_pair(m_id, m_id_end_of_range);
        transaction->m_ruleRemoveTargetByIdRange.push_back(std::make_pair(id_range, m_target));
    }
    return true;
}


}  // namespace ctl
}  // namespace actions
}  // namespace modsecurity
