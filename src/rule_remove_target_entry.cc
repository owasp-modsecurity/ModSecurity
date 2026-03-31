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
