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

#include "actions/action.h"

#ifndef SRC_ACTIONS_PHASE_H_
#define SRC_ACTIONS_PHASE_H_

#ifdef __cplusplus
class Assay;

namespace ModSecurity {
class Assay;
class Rule;

namespace actions {


class Phase : public Action {
 public:
    explicit Phase(std::string action);

    bool init(std::string *error) override;
    bool evaluate(Rule *rule, Assay *assay) override;
    int phase;
    int m_secRulesPhase;
};

}  // namespace actions
}  // namespace ModSecurity
#endif

#endif  // SRC_ACTIONS_PHASE_H_
