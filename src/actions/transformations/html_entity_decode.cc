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

#include "actions/transformations/html_entity_decode.h"

#include <string.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/transaction.h"
#include "actions/transformations/transformation.h"
#include "src/utils.h"


namespace modsecurity {
namespace actions {
namespace transformations {


std::string HtmlEntityDecode::evaluate(std::string value,
    Transaction *transaction) {

    if (HtmlEntityDecodeInstantCache::getInstance().count(value) > 0) {
        return HtmlEntityDecodeInstantCache::getInstance().at(value);
    }

    char *tmp = strdup(value.c_str());

    // FIXME: html_entities_decode_inplace is not working as expected
    //        temporary disabled to perform the audit_log tests.
    //  html_entities_decode_inplace((unsigned char *)tmp, value.size());
    std::string ret("");
    ret.assign(tmp);
    free(tmp);

    HtmlEntityDecodeInstantCache::getInstance().cache(value, ret);

    return ret;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
