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

#include "actions/transformations/js_decode.h"

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


std::string JsDecode::evaluate(std::string value,
    Assay *assay) {

    char *val = reinterpret_cast<char *>(
        malloc(sizeof(char) * value.size() + 1));
    memcpy(val, value.c_str(), value.size() + 1);
    val[value.size()] = '\0';

    js_decode_nonstrict_inplace((unsigned char *)val, value.size());
    std::string ret("");
    ret.assign(val);
    free(val);

    return ret;
}


}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
