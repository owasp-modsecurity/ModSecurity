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

#include "actions/transformations/url_decode_uni.h"

#include <string.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/assay.h"
#include "actions/transformations/transformation.h"
#include "src/utils.h"


namespace modsecurity {
namespace actions {
namespace transformations {


std::string UrlDecodeUni::evaluate(std::string value,
    Assay *assay) {
    int changed = 0;
    char *tmp = strdup(value.c_str());
    urldecode_uni_nonstrict_inplace_ex(assay, (unsigned char *)tmp,
        value.size(), &changed);
    std::string ret("");
    ret.assign(tmp);
    free(tmp);

    return ret;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
