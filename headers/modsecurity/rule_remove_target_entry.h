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
 * Entry for ctl:ruleRemoveTargetById exclusion.
 * Supports literal target (e.g. ARGS:pwd) or regex (e.g. ARGS:/^json\.\d+\.JobDescription$/).
 * Regex is compiled at config load (maintainer's approach).
 */
struct RuleRemoveTargetByIdEntry {
    int id;
    std::string literal;
    std::shared_ptr<Utils::Regex> regex;  // shared: same compiled regex reused per request

    /**
     * Match VariableValue. For regex: match against key (dict element).
     * For literal: match against keyWithCollection (e.g. ARGS:mixpanel).
     */
    bool matches(const std::string &key, const std::string &keyWithCollection) const {
        if (regex) {
            return regex->searchAll(key).size() > 0;
        }
        return literal == keyWithCollection;
    }

    /**
     * Match Variable (for getFinalVars). Uses case-insensitive literal match.
     * Regex uses key from variable's fullName (extract part after colon).
     */
    bool matchesVariable(const std::string &fullName) const {
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

}  // namespace modsecurity

#endif  // HEADERS_MODSECURITY_RULE_REMOVE_TARGET_ENTRY_H_
