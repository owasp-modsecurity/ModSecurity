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

#include "actions/transformations/sha1.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "actions/transformations/transformation.h"
#include "utils/sha1.h"
#include "src/utils.h"


namespace modsecurity {
namespace actions {
namespace transformations {

Sha1::Sha1(std::string action)
    : Transformation(action) {
    this->action_kind = 1;
}

std::string Sha1::evaluate(std::string value,
    Transaction *transaction) {

    Utils::SHA1 sha1;
    sha1.update(&value);
    std::string sha1_bin = sha1.final_bin();

    return sha1_bin;
}

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
