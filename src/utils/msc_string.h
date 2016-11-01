/*
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

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#ifndef SRC_UTILS_MSC_STRING_H_
#define SRC_UTILS_MSC_STRING_H_

namespace modsecurity {
namespace utils {


class String {
 public:
    static std::string ascTime(time_t *t);
    static std::string dash_if_empty(const char *str);
    static std::string dash_if_empty(const std::string *str);
    static std::string limitTo(int amount, const std::string &str);
    static std::string removeBracketsIfNeeded(std::string a);
    static std::string string_to_hex(const std::string& input);
    static std::string toHexIfNeeded(const std::string &str);
    static std::string tolower(std::string str);
    static std::string toupper(std::string str);
    static std::vector<std::string> split(std::string str, char delimiter);
    static void chomp(std::string *str);
};


}  // namespace utils
}  // namespace modsecurity

#endif  // SRC_UTILS_MSC_STRING_H_
