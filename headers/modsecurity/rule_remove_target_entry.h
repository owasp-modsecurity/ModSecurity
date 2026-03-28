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

#ifndef HEADERS_MODSECURITY_RULE_REMOVE_TARGET_ENTRY_H_
#define HEADERS_MODSECURITY_RULE_REMOVE_TARGET_ENTRY_H_

#include <memory>
#include <string>

#include "src/utils/regex.h"

namespace modsecurity {

/**
 * Shared target-matching logic for ctl:ruleRemoveTarget{ById,ByTag}.
 * Supports literal target (e.g. ARGS:pwd) or regex (e.g. ARGS:/^json\.\d+\.JobDescription$/).
 * Regex is compiled at config load time.
 */
struct RuleRemoveTargetSpec {
    std::string literal;
    std::shared_ptr<Utils::Regex> regex;

    bool matchesKeyWithCollection(const std::string &key,
                                  const std::string &keyWithCollection) const {
        if (regex) {
            return regex->searchAll(key).size() > 0;
        }
        return literal == keyWithCollection;
    }

    bool matchesFullName(const std::string &fullName) const {
        if (regex) {
            size_t colon = fullName.find(':');
            std::string keyPart = (colon != std::string::npos && colon + 1 < fullName.size())
                ? fullName.substr(colon + 1) : fullName;
            return regex->searchAll(keyPart).size() > 0;
        }
        if (literal.size() != fullName.size()) {
            return false;
        }
        return std::equal(literal.begin(), literal.end(), fullName.begin(),
            [](char a, char b) {
                return std::tolower(static_cast<unsigned char>(a)) ==
                       std::tolower(static_cast<unsigned char>(b));
            });
    }
};


struct RuleRemoveTargetByIdEntry {
    int id;
    RuleRemoveTargetSpec target;
};


struct RuleRemoveTargetByTagEntry {
    std::string tag;
    RuleRemoveTargetSpec target;
};

}  // namespace modsecurity

#endif  // HEADERS_MODSECURITY_RULE_REMOVE_TARGET_ENTRY_H_
