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

#include <vector>
#include <string>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_TIME_WDAY_H_
#define SRC_VARIABLES_TIME_WDAY_H_

#include "variables/variable.h"

namespace modsecurity {

class Assay;
namespace Variables {

class TimeWDay : public Variable {
 public:
    explicit TimeWDay(std::string _name)
        : Variable(_name) { }

    void evaluateInternal(Assay *assay,
        std::vector<const transaction::Variable *> *l) override;
};

}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_TIME_WDAY_H_
