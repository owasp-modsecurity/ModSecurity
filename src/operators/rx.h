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

#ifndef SRC_OPERATORS_RX_H_
#define SRC_OPERATORS_RX_H_

#include <string>
#include <list>
#include <memory>
#include <utility>

#include "src/operators/operator.h"
#include "src/utils/regex.h"


namespace modsecurity {
using Utils::SMatch;
using Utils::regex_search;
using Utils::Regex;

namespace operators {


class Rx : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit Rx(std::unique_ptr<RunTimeString> param)
        : m_re(nullptr),
        Operator("Rx", std::move(param)) {
            m_couldContainsMacro = true;
        }

    ~Rx() {
        if (m_string->containsMacro() == false && m_re != NULL) {
            delete m_re;
            m_re = NULL;
        }
    }

    bool evaluate(Transaction *transaction,
        const RuleWithActions *rule,
        const bpstd::string_view &input,
        RuleMessage *ruleMessage) override;

    bool init(const std::string &file, std::string *error) override;

 private:
    Regex *m_re;
};


}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_RX_H_
