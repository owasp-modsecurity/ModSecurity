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

#ifndef SRC_OPERATORS_VERIFY_CC_H_
#define SRC_OPERATORS_VERIFY_CC_H_

#include <pcre.h>
#include <string>

#include "operators/operator.h"

namespace ModSecurity {
namespace operators {

class VerifyCC : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    VerifyCC(std::string op, std::string param, bool negation)
        : Operator(op, param, negation) { }

    int luhnVerify(const char *ccnumber, int len);
    bool evaluate(Assay *assay, const std::string &input) override;
    bool init(const std::string &param, const char **error) override;
 private:
    pcre *m_pc;
    pcre_extra *m_pce;
};

}  // namespace operators
}  // namespace ModSecurity


#endif  // SRC_OPERATORS_VERIFY_CC_H_
