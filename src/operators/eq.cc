/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/operators/eq.h"

#include <string>
#include <charconv>

#include "src/operators/operator.h"


namespace modsecurity {
namespace operators {


bool Eq::evaluate(Transaction *transaction, const std::string &input) {
    int p = 0;
    int i = 0;
    std::string pt(m_string->evaluate(transaction));

    const auto conv_res = std::from_chars(pt.data(), pt.data() + pt.size(), p);
    if (conv_res.ec == std::errc::invalid_argument) {
        p = 0;
    }
    const auto conv_res2 = std::from_chars(input.data(), input.data() + input.size(), i);
    if (conv_res2.ec == std::errc::invalid_argument) {
        i = 0;
    }

    return p == i;
}


}  // namespace operators
}  // namespace modsecurity
