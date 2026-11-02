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

#include "modsecurity/rule_remove_target_entry.h"

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

#include "src/utils/regex.h"


namespace modsecurity {

static bool collectionPrefixMatches(const std::string &literal,
                                    size_t litColon,
                                    const std::string &candidate,
                                    size_t candColon) {
    if (litColon != candColon) {
        return false;
    }
    return std::equal(literal.begin(), literal.begin() + litColon,
                      candidate.begin(),
                      [](char a, char b) {
                          return std::tolower(static_cast<unsigned char>(a)) ==
                                 std::tolower(static_cast<unsigned char>(b));
                      });
}


bool RuleRemoveTargetSpec::detectAndCompileRegex(
    const std::string &target,
    std::shared_ptr<Utils::Regex> *regex_out,
    const char *action_name,
    std::string *error) {
    if (regex_out == nullptr || error == nullptr || action_name == nullptr) {
        return false;
    }
    regex_out->reset();
    // Detect regex format: COLLECTION:/pattern/ (e.g. ARGS:/mixpanel$/)
    if (target.size() < 4) {
        return true;
    }
    size_t colon = target.find(':');
    if (colon == std::string::npos || colon + 1 >= target.size() ||
        target[colon + 1] != '/') {
        return true;
    }
    if (target[target.size() - 1] != '/' || target.size() < colon + 3) {
        error->assign(std::string("Unclosed regex or unsupported character "
            "in ctl:") + action_name + ": " + target);
        return false;
    }
    size_t pattern_start = colon + 2;
    size_t pattern_end = target.size() - 1;
    if (pattern_end <= pattern_start) {
        error->assign(std::string("Empty regex in ctl:") + action_name +
            ": " + target);
        return false;
    }
    std::string pattern = target.substr(pattern_start,
        pattern_end - pattern_start);
    auto compiled = std::make_shared<Utils::Regex>(pattern, true);
    if (compiled->hasError()) {
        error->assign(std::string("Invalid regex in ctl:") + action_name +
            ": " + target);
        return false;
    }
    *regex_out = std::move(compiled);
    return true;
}


bool RuleRemoveTargetSpec::matchesKeyWithCollection(
    const std::string &key,
    const std::string &keyWithCollection) const {
    if (regex) {
        size_t litColon = literal.find(':');
        size_t kwcColon = keyWithCollection.find(':');
        // Collection-scoped target cannot match a scalar variable.
        if (litColon != std::string::npos && kwcColon == std::string::npos) {
            return false;
        }
        // Regex targets match only the key portion; verify collection prefix
        // separately so ARGS:/.../ does not exclude REQUEST_HEADERS variables.
        if (litColon != std::string::npos && kwcColon != std::string::npos) {
            if (!collectionPrefixMatches(literal, litColon,
                                         keyWithCollection, kwcColon)) {
                return false;
            }
        }
        return regex->search(key) > 0;
    }
    return literal == keyWithCollection;
}


bool RuleRemoveTargetSpec::matchesFullName(const std::string &fullName) const {
    if (regex) {
        size_t litColon = literal.find(':');
        size_t fullColon = fullName.find(':');
        // Collection-scoped target cannot match a scalar variable.
        if (litColon != std::string::npos && fullColon == std::string::npos) {
            return false;
        }
        // Regex targets match only the key portion; verify collection prefix
        // separately so ARGS:/.../ does not exclude REQUEST_HEADERS variables.
        if (litColon != std::string::npos && fullColon != std::string::npos) {
            if (!collectionPrefixMatches(literal, litColon,
                                         fullName, fullColon)) {
                return false;
            }
        }
        std::string keyPart = (fullColon != std::string::npos &&
                               fullColon + 1 < fullName.size())
            ? fullName.substr(fullColon + 1) : fullName;
        // VariableRegex synthesizes fullName as "COLLECTION:regex(pattern)".
        // Never match that synthetic text with a target regex — key matching
        // happens at evaluate time via matchesKeyWithCollection.
        if (keyPart.size() >= 7 &&
            keyPart.compare(0, 6, "regex(") == 0 &&
            keyPart.back() == ')') {
            return false;
        }
        return regex->search(keyPart) > 0;
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

}  // namespace modsecurity
