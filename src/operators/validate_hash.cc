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

#include "src/operators/validate_hash.h"

#include <string>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {

bool ValidateHash::evaluate(Transaction *transaction,
    const RuleWithActions *rule,
    const bpstd::string_view &str,
    RuleMessage *ruleMessage) {
    /**
     * @todo Implement the operator ValidateHash.
     *       Reference: https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#validateHash
     */
    return true;
}


}  // namespace operators
}  // namespace modsecurity
