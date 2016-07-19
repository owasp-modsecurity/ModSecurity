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

#ifdef __cplusplus
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#endif

#ifndef HEADERS_MODSECURITY_RULES_EXCEPTIONS_H_
#define HEADERS_MODSECURITY_RULES_EXCEPTIONS_H_

#ifdef __cplusplus


namespace modsecurity {

class RulesExceptions {
 public:
    RulesExceptions() { }
    bool load(const std::string &data, std::string *error);
    bool addRange(int a, int b);
    bool addNumber(int a);
    bool contains(int a);

 private:
    std::list<std::pair<int, int> > m_ranges;
    std::list<int> m_numbers;
};

}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULES_EXCEPTIONS_H_

