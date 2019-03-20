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


class Rules {
 public:
    int append(Rules *from,
        const std::vector<RuleId> &ids,
        std::ostringstream *err);

    bool insert(std::shared_ptr<Rule> rule);

    bool insert(std::shared_ptr<Rule> rule,
        const std::vector<RuleId> *ids,
        std::ostringstream *err);

    size_t size();

    std::shared_ptr<Rule> operator[](int index);
    std::shared_ptr<Rule> at(int index);

    std::vector<std::shared_ptr<Rule> > m_rules;
    void dump();

};


}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_H_
