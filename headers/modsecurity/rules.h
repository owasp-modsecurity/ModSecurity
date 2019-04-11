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
#include <memory>
#endif

#include "modsecurity/rule.h"


#ifndef HEADERS_MODSECURITY_RULES_H_
#define HEADERS_MODSECURITY_RULES_H_

#ifdef __cplusplus
namespace modsecurity {
namespace actions {
namespace transformations {
class Transformation;
}
}

using RulesErrors = std::list<std::unique_ptr<std::string>>;
using RulesWarnings = std::list<std::unique_ptr<std::string>>;

class Rules {
 public:
    using container=std::vector<std::shared_ptr<Rule>>;
    using iterator=typename container::iterator;
    using const_iterator=typename container::const_iterator;

    int append(Rules *from);

    bool insert(const std::shared_ptr<Rule> &rule);

    size_t size() const;

    std::shared_ptr<Rule> operator[](int index) const;
    std::shared_ptr<Rule> at(int index) const;

    void fixDefaultActions(RulesWarnings *warnings, RulesErrors *errors);

    std::vector<std::shared_ptr<actions::Action> > m_defaultActions;
    std::vector<std::shared_ptr<actions::transformations::Transformation> > m_defaultTransformations;

    virtual void dump() {
        std::stringstream ss;
        dump(ss);
        std::cout << ss.str();
    };
    virtual void dump(std::stringstream &out);

    inline iterator begin() noexcept { return m_rules.begin(); }
    inline const_iterator cbegin() const noexcept { return m_rules.cbegin(); }
    inline iterator end() noexcept { return m_rules.end(); }
    inline const_iterator cend() const noexcept { return m_rules.cend(); }
 private:
     container m_rules;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULES_H_

