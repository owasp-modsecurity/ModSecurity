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

#include "normalise_path_win.h"

#include "normalise_path.h"


namespace modsecurity::actions::transformations {


bool NormalisePathWin::transform(std::string &value, const Transaction *trans) const {
    int _changed;

    char *tmp = reinterpret_cast<char *>(
        malloc(sizeof(char) * value.size() + 1));
    memcpy(tmp, value.c_str(), value.size() + 1);
    tmp[value.size()] = '\0';

    int i = NormalisePath::normalize_path_inplace(
        reinterpret_cast<unsigned char *>(tmp),
        value.size(), 1, &_changed);

    std::string ret("");
    ret.assign(tmp, i);
    free(tmp);

    const auto changed = ret != value;
    value = ret;
    return changed;
}


}  // namespace modsecurity::actions::transformations
