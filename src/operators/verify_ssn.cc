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

#include "src/operators/verify_ssn.h"

#include <string>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {


bool VerifySSN::evaluate(Transaction *transaction, const std::string &str) {
    /**
     * @todo Implement the operator VerifySSN.
     *       Reference: https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#verifySSN
     */
    return true;
}


}  // namespace operators
}  // namespace modsecurity
