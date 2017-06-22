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

#include "modsecurity/rules_exceptions.h"

#include <string>

#include "src/utils/string.h"


namespace modsecurity {


bool RulesExceptions::load(const std::string &a, std::string *error) {
    bool added = false;
    std::vector<std::string> toRemove = utils::string::ssplit(a, ' ');
    for (std::string &a : toRemove) {
        std::string b = modsecurity::utils::string::parserSanitizer(a);
        if (b.size() == 0) {
            continue;
        }

        size_t dash = b.find('-');
        if (dash != std::string::npos) {
            std::string n1s = std::string(b, 0, dash);
            std::string n2s = std::string(b, dash + 1, b.size() - (dash + 1));
            int n1n = 0;
            int n2n = 0;
            try {
                n1n = std::stoi(n1s);
                added = true;
            } catch (...) {
                error->assign("Not a number: " + n1s);
                return false;
            }
            try {
                n2n = std::stoi(n2s);
                added = true;
            } catch (...) {
                error->assign("Not a number: " + n2s);
                return false;
            }

            if (n1s > n2s) {
                error->assign("Invalid range: " + b);
                return false;
            }
            addRange(n1n, n2n);
            added = true;
        } else {
            try {
                int num = std::stoi(b);
                addNumber(num);
                added = true;
            } catch (...) {
                error->assign("Not a number or range: " + b);
                return false;
            }
        }
    }

    if (added) {
        return true;
    }

    error->assign("Not a number or range: " + a);
    return false;
}


bool RulesExceptions::addNumber(int a) {
    m_numbers.push_back(a);
    return true;
}


bool RulesExceptions::addRange(int a, int b) {
    m_ranges.push_back(std::make_pair(a, b));
    return true;
}


bool RulesExceptions::contains(int a) {
    for (int z : m_numbers) {
        if (a == z) {
            return true;
        }
    }

    for (auto z : m_ranges) {
        if (z.first <= a && z.second >= a) {
            return true;
        }
    }

    return false;
}


bool RulesExceptions::merge(const RulesExceptions& from) {
    for (int a : from.m_numbers) {
        bool ret = addNumber(a);
        if (ret == false) {
            return ret;
        }
    }
    for (auto b : from.m_ranges) {
        bool ret = addRange(b.first, b.second);
        if (ret == false) {
            return ret;
        }
    }

    return true;
}


}  // namespace modsecurity

