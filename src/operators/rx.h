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

#ifndef SRC_OPERATORS_RX_H_
#define SRC_OPERATORS_RX_H_

#include <string>
#include <list>

#include "operators/operator.h"
#include "utils/regex.h"

#ifdef __cplusplus
namespace modsecurity {
using Utils::SMatch;
using Utils::regex_search;
using Utils::Regex;

namespace operators {


class Rx : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    Rx(std::string op, std::string param, bool negation)
        : Operator(op, param, negation),
        m_param(param) {
            m_re = new Regex(param);
        }

    ~Rx() {
        delete m_re;
    }
    bool evaluate(Transaction *transaction, const std::string &input);

 private:
    std::string m_param;
    Regex *m_re;
};


}  // namespace operators
}  // namespace modsecurity

#endif


#endif  // SRC_OPERATORS_RX_H_
