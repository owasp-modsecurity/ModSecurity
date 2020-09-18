/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_REMOTE_USER_H_
#define SRC_VARIABLES_REMOTE_USER_H_

#include "src/variables/variable.h"
#include "src/utils/base64.h"

namespace modsecurity {

class Transaction;
namespace variables {


class RemoteUser : public Variable {
 public:
    explicit RemoteUser(const std::string &_name)
        : Variable(_name),
        m_retName("REMOTE_USER") { }

    void evaluate(Transaction *transaction,
        VariableValues *l) override;

    static std::pair<std::string, VariableOrigin> parserRemoteUser(Transaction *transaction) {
        size_t pos;
        std::string base64;
        std::string header;

        VariableValues l2;
        transaction->m_variableRequestHeaders.resolve("authorization", &l2);

        if (l2.size() < 1) {
            goto err;
        }

        header = std::string(l2.at(0)->getValue());

        if (header.compare(0, 6, "Basic ") == 0) {
            base64 = std::string(header, 6, header.length());
        }

        base64 = Utils::Base64::decode(base64);

        pos = base64.find(":");
        if (pos == std::string::npos) {
            goto err;
        }

        return std::make_pair(std::string(base64, 0, pos), l2[0]->getOrigin()[0]);
err:
        return std::make_pair(std::string(""), VariableOrigin());

    }

    std::string m_retName;
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_REMOTE_USER_H_

