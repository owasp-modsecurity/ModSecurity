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

#include "operators/ip_match.h"

#include <string.h>
#include <string>

#include "utils/msc_tree.h"
#include "operators/operator.h"

namespace modsecurity {
namespace operators {


bool IpMatch::init(const std::string &file, const char **error) {
    std::string e("");
    bool res = m_tree.addFromBuffer(param, &e);

    if (res == false) {
        *error = e.c_str();
    }

    return res;
}


bool IpMatch::evaluate(Assay *assay, const std::string &input) {
    return m_tree.contains(input);
}


}  // namespace operators
}  // namespace modsecurity
