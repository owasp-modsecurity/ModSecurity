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

#ifndef SRC_OPERATORS_PM_FROM_FILE_H_
#define SRC_OPERATORS_PM_FROM_FILE_H_

#include <string>

#include "operators/pm.h"

#ifdef __cplusplus
namespace ModSecurity {
namespace operators {


class PmFromFile : public Pm {
 public:
    /** @ingroup ModSecurity_Operator */
    PmFromFile(std::string op, std::string param, bool negation)
        : Pm(op, param, negation) { }

    bool init(const char **error) override;
};


}  // namespace operators
}  // namespace ModSecurity

#endif


#endif  // SRC_OPERATORS_PM_FROM_FILE_H_
