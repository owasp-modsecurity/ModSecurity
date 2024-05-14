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
    unsigned char *val(NULL);
    int invalid_count = 0;
    int _changed;

    val = (unsigned char *) malloc(sizeof(char) * value.size() + 1);
    memcpy(val, value.c_str(), value.size() + 1);
    val[value.size()] = '\0';

    int size = utils::urldecode_nonstrict_inplace(val, value.size(),
        &invalid_count, &_changed);
    std::string out;

    out.append((const char *)val, size);

    free(val);

    const auto changed = out != value;
    value = out;
    return changed;
}


}  // namespace modsecurity::actions::transformations
