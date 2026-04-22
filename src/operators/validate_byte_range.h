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

#ifndef SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_
#define SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_

#include <array>
#include <string>
#include <memory>

#include "src/operators/operator.h"


namespace modsecurity {
namespace operators {

class ValidateByteRange : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit ValidateByteRange(std::unique_ptr<RunTimeString> param)
        : Operator("ValidateByteRange", std::move(param)) { }
    ~ValidateByteRange() override { }

    bool evaluate(Transaction *transaction, RuleWithActions *rule,
        const std::string &input,
        RuleMessage &ruleMessage) override;
    bool getRange(const std::string &rangeRepresentation, std::string *error);
    bool init(const std::string& file, std::string *error) override;

 private:
    static constexpr size_t kTableSize = 32;

    bool getRange(const std::string &rangeRepresentation,
        std::array<unsigned char, kTableSize> *targetTable,
        std::string *error) const;

    std::array<unsigned char, kTableSize> table{};
};

}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_VALIDATE_BYTE_RANGE_H_
