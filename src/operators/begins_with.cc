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

#include "operators/begins_with.h"

#include <string>

#include "operators/operator.h"

namespace ModSecurity {
namespace operators {


bool BeginsWith::evaluate(Assay *assay, const std::string &str) {
    bool ret = false;

    if (str.size() < param.size()) {
        ret = false;
    } else if (!str.compare(0, param.size(), param)) {
         ret = true;
    }

    if (negation) {
        return !ret;
    }

    return ret;
}


}  // namespace operators
}  // namespace ModSecurity
