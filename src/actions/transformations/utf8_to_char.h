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

#include <string>

#include "modsecurity/actions/action.h"
#include "src/actions/transformations/transformation.h"

#ifndef SRC_ACTION_TRANSFORMATION_UTF8_TO_CHAR_H_
#define SRC_ACTION_TRANSFORMATION_UTF8_TO_CHAR_H_

namespace modsecurity {
class Transaction;

namespace actions {
namespace transformations {

class Utf8ToChar : public Transformation {
 public:
    explicit Utf8ToChar(std::string action) : Transformation(action) { }

    std::string evaluate(std::string exp,
        Transaction *transaction) override;
};

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity


#endif  // SRC_ACTION_TRANSFORMATION_NEUTRALIZE_SAFE_ENC_H_
