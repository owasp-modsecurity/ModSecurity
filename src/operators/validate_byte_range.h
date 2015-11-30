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

#ifndef SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_
#define SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_

#include <string>
#include <vector>
#include <cstring>

#include "operators/operator.h"

#ifdef __cplusplus
namespace modsecurity {
namespace operators {

class ValidateByteRange : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    ValidateByteRange(std::string op, std::string param, bool negation)
        : Operator(op, param, negation) {
            std::memset(table, '\0', sizeof(char) * 32);
        }

    ~ValidateByteRange() override { }

    bool evaluate(Assay *assay, const std::string &input) override;
    bool getRange(const std::string &rangeRepresentation, const char **error);
    bool init(const std::string& file, const char **error) override;
 private:
    std::vector<std::string> ranges;
    char table[32];
};

}  // namespace operators
}  // namespace ModSecurity

#endif


#endif  // SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_
