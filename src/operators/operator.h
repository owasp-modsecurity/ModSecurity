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

#ifdef __cplusplus
#include <string>
#endif

#ifndef SRC_OPERATORS_OPERATOR_H__
#define SRC_OPERATORS_OPERATOR_H__

#include "modsecurity/assay.h"

#ifdef __cplusplus
namespace ModSecurity {
namespace operators {

class Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    Operator();
    Operator(std::string po, std::string param, bool invert);

    std::string op;
    std::string param;

    virtual bool evaluate(Assay *assay);
    virtual bool evaluate(Assay *assay, const std::string &str);
    static Operator *instantiate(std::string op);
 protected:
    bool debug(Assay *assay, int x, std::string a);
};

}  // namespace operators
}  // namespace ModSecurity
#endif

#endif  // SRC_OPERATORS_OPERATOR_H__

