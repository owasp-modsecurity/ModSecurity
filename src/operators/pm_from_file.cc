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

#include "operators/pm_from_file.h"

#include <string>

#include "operators/operator.h"
#include "utils/https_client.h"
#include "src/utils.h"

namespace modsecurity {
namespace operators {


bool PmFromFile::init(const std::string &config, std::string *error) {
    std::istream *iss;

    if (param.compare(0, 8, "https://") == 0) {
        Utils::HttpsClient client;
        bool ret = client.download(param);
        if (ret == false) {
            error->assign(client.error);
            return false;
        }
        iss = new std::stringstream(client.content);
    } else {
        std::string resource = find_resource(param, config);
        iss = new std::ifstream(resource, std::ios::in);

        if (((std::ifstream *)iss)->is_open() == false) {
            error->assign("Failed to open file: " + param);
            delete iss;
            return false;
        }
    }

    for (std::string line; std::getline(*iss, line); ) {
        acmp_add_pattern(m_p, line.c_str(), NULL, NULL, line.length());
    }

    //acmp_prepare(m_p);

    delete iss;
    return true;
}


}  // namespace operators
}  // namespace modsecurity
