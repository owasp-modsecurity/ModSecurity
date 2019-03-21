
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

#include "modsecurity/rules_set_properties.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
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

    void dump();

    Rules *operator[](int index);
    Rules *at(int index);
    size_t size() { return modsecurity::Phases::NUMBER_OF_PHASES; }

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
