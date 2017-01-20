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

#include "src/operators/pm.h"


namespace modsecurity {
namespace operators {


class PmFromFile : public Pm {
 public:
    /** @ingroup ModSecurity_Operator */
    PmFromFile(std::string op, std::string param, bool negation)
        : Pm(op, param, negation) { }
    PmFromFile(std::string op, std::string param)
        : Pm(op, param) { }
    explicit PmFromFile(std::string param)
        : Pm("PmFromFile", param) { }
    bool init(const std::string &file, std::string *error) override;
};


}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_PM_FROM_FILE_H_
