
/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <array>
#endif


#ifndef HEADERS_MODSECURITY_RULES_SET_PHASES_H_
#define HEADERS_MODSECURITY_RULES_SET_PHASES_H_

#include "modsecurity/rules.h"

#ifdef __cplusplus

namespace modsecurity {
class RuleWithOperator;
namespace Parser {
class Driver;
}

/** @ingroup ModSecurity_CPP_API */
class RulesSetPhases {
 public:
    using container = std::array<Rules, modsecurity::Phases::NUMBER_OF_PHASES>;
    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    void insert(std::shared_ptr<Rule> rule);
    void append(RulesSetPhases *from);

    int append(RulesSetPhases *from, std::ostringstream *err);
    void dump();

    Rules *operator[](int index);
    Rules *at(int index);
    static size_t size() { return modsecurity::Phases::NUMBER_OF_PHASES; }

    void fixDefaultActions(RulesWarnings *warnings, RulesErrors *errors) {
        for (auto &phase : m_rulesAtPhase) {
            phase.fixDefaultActions(warnings, errors);
        }
    }

    inline iterator begin() noexcept { return m_rulesAtPhase.begin(); }
    inline const_iterator cbegin() const noexcept { return m_rulesAtPhase.cbegin(); }
    inline iterator end() noexcept { return m_rulesAtPhase.end(); }
    inline const_iterator cend() const noexcept { return m_rulesAtPhase.cend(); }

 private:
    container m_rulesAtPhase;
};


}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_SET_PHASES_H_