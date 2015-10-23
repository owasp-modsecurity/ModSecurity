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

#include "actions/transformations/normalise_path_win.h"

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


namespace ModSecurity {
namespace actions {
namespace transformations {


std::string NormalisePathWin::evaluate(std::string value,
    Assay *assay) {
    int changed;

    char *tmp = (char *) malloc(sizeof(char) * value.size() + 1);
    memcpy(tmp, value.c_str(), value.size() + 1);
    tmp[value.size()] = '\0';

    int i = normalize_path_inplace((unsigned char *)tmp,
        value.size(), 1, &changed);

    std::string ret("");
    ret.assign(tmp, i);
    free(tmp);

    return ret;
}


}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
