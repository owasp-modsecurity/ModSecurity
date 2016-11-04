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

#ifndef SRC_OPERATORS_VERIFY_CPF_H_
#define SRC_OPERATORS_VERIFY_CPF_H_

#include <string>

#include "src/operators/operator.h"

#ifdef __cplusplus
namespace modsecurity {
namespace operators {

class VerifyCPF : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    VerifyCPF(std::string o, std::string p, bool i);
    bool evaluate(Transaction *transaction, const std::string  &str) override;
};

}  // namespace operators
}  // namespace modsecurity

#endif


#endif  // SRC_OPERATORS_VERIFY_CPF_H_
