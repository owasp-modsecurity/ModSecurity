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

#include "src/variables/remote_user.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <memory>

#include "modsecurity/transaction.h"
#include "src/utils/base64.h"

namespace modsecurity {
namespace variables {


void RemoteUser::evaluate(Transaction *transaction,
    VariableValues *l) {
    size_t pos;
    std::string base64;
    std::string header;

    VariableValues l2;
    transaction->m_variableRequestHeaders.resolve("authorization", &l2);

    if (l2.size() < 1) {
        return;
    }

    header = std::string(l2.at(0)->getValue());

    if (header.compare(0, 6, "Basic ") == 0) {
        base64 = std::string(header, 6, header.length());
    }

    base64 = Utils::Base64::decode(base64);

    pos = base64.find(":");
    if (pos == std::string::npos) {
        return;
    }
    transaction->m_variableRemoteUser.assign(std::string(base64, 0, pos));

    auto var = std::make_shared<VariableValue>(&l2[0]->getName(), &transaction->m_variableRemoteUser);

    for (auto &i : l2[0]->getOrigin()) {
        var->addOrigin(i);
    }
    l->push_back(std::move(var));
}


}  // namespace variables
}  // namespace modsecurity
