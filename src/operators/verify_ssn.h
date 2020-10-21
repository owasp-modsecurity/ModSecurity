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

#ifndef SRC_OPERATORS_VERIFY_SSN_H_
#define SRC_OPERATORS_VERIFY_SSN_H_

#include <string>
#include <memory>
#include <utility>

#include "src/operators/operator.h"
#include "src/utils/regex.h"


namespace modsecurity {
using Utils::SMatch;
using Utils::regex_search;
using Utils::Regex;

namespace operators {

class VerifySSN : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit VerifySSN(std::unique_ptr<RunTimeString> param)
        : Operator("VerifySSN", std::move(param)) {
        m_re = new Regex(m_param);
    }

    ~VerifySSN() {
        delete m_re;
    }

    bool operator=(const VerifySSN &a) = delete;
    VerifySSN(const VerifySSN &a) = delete;

    bool evaluate(Transaction *transaction,
        RuleWithActions *rule,
        const bpstd::string_view &input,
        RuleMessage *ruleMessage) override;



 private:
    static bool verify(const char *ssnumber, int len);
    static int convert_to_int(const char c);

    Regex *m_re;
};

}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_VERIFY_SSN_H_
