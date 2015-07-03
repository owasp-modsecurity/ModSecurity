/**
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "regression/custom_debug_log.h"

#include <iostream>
#include <string>
#include <regex>

#include "modsecurity/debug_log.h"

namespace modsecurity_test {

CustomDebugLog *CustomDebugLog::new_instance() {
    return new CustomDebugLog();
}


bool CustomDebugLog::write_log(int level, const std::string& message) {
    m_log << "[" << level << "] " << message << std::endl;
}


bool CustomDebugLog::contains(const std::string& pattern) {
    std::regex re(pattern);
    std::smatch match;
    std::string s = m_log.str();
    return (std::regex_search(s, match, re) && match.size() >= 1);
}

std::string CustomDebugLog::log_messages() {
    return m_log.str();
}


}  // namespace modsecurity_test
