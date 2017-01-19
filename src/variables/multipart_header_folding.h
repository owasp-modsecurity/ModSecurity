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

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_MULTIPART_HEADER_FOLDING_H_
#define SRC_VARIABLES_MULTIPART_HEADER_FOLDING_H_

#include "src/variables/variable.h"

namespace modsecurity {

class Transaction;
namespace Variables {

class MultipartHeaderFolding : public Variable {
 public:
    MultipartHeaderFolding()
        : Variable("MULTIPART_HEADER_FOLDING") { }

    void evaluateInternal(Transaction *transaction,
        std::vector<const collection::Variable *> *l) {
        transaction->m_variableMultipartHeaderFolding.evaluate(l);
    }

};

}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_MULTIPART_HEADER_FOLDING_H_
