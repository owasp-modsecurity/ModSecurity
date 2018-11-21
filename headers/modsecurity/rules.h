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

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#endif


#ifndef HEADERS_MODSECURITY_RULES_H_
#define HEADERS_MODSECURITY_RULES_H_

#include "modsecurity/rules_set_properties.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"

#ifdef __cplusplus
namespace modsecurity {


class Rules : public std::vector<Rule *> {
 public:
    void dump() {
        for (int j = 0; j < size(); j++) {
            std::cout << "    Rule ID: " << std::to_string(at(j)->m_ruleId);
            std::cout << "--" << at(j) << std::endl;
        }
    }

    int append(Rules *from, const std::vector<int64_t> &ids, std::ostringstream *err) {
        size_t j = 0;
        for (; j < from->size(); j++) {
            Rule *rule = from->at(j);
            if (std::binary_search(ids.begin(), ids.end(), rule->m_ruleId)) {
                if (err != NULL) {
                    *err << "Rule id: " << std::to_string(rule->m_ruleId) \
                         << " is duplicated" << std::endl;
                }
                return -1;
            }
            rule->refCountIncrease();
        }
        insert(end(), from->begin(), from->end());
        return j;
    }
};


}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_H_
