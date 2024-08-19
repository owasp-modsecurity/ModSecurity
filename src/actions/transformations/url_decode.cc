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

#include "url_decode.h"

#include "src/utils/decode.h"


namespace modsecurity::actions::transformations {


UrlDecode::UrlDecode(const std::string &action) 
    : Transformation(action) {
    this->action_kind = 1;
}

bool UrlDecode::transform(std::string &value, const Transaction *trans) const {
    int invalid_count = 0;
    int changed;
    const auto new_len = utils::urldecode_nonstrict_inplace(
        (unsigned char*)value.data(),
        value.length(),
        &invalid_count,
        &changed);

    value.resize(new_len);
    return changed != 0;
}


}  // namespace modsecurity::actions::transformations
