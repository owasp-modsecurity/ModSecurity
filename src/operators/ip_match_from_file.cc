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

#include "operators/ip_match_from_file.h"

#include <string.h>

#include <string>

#include "operators/operator.h"

namespace modsecurity {
namespace operators {


bool IpMatchFromFile::init(const std::string &file,
    std::string *error) {
    std::string e("");
    bool res = false;

    if (m_param.compare(0, 8, "https://") == 0) {
        res = m_tree.addFromUrl(m_param, &e);
    } else {
        res = m_tree.addFromFile(m_param, &e);
    }

    if (res == false) {
        error->assign(e);
    }

    return res;
}


}  // namespace operators
}  // namespace modsecurity
