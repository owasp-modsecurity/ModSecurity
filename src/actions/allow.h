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

#ifndef SRC_ACTIONS_ALLOW_H_
#define SRC_ACTIONS_ALLOW_H_

#ifdef __cplusplus
class Transaction;

namespace modsecurity {
class Transaction;
class Rule;

namespace actions {

enum AllowType : short {
  /**
   *
   */
  NoneAllowType,
  /**
   *
   */
  RequestAllowType,
  /**
   *
   */
  PhaseAllowType,
  /**
   *
   */
  FromNowOneAllowType,
};


class Allow : public Action {
 public:
    explicit Allow(std::string action)
        : Action(action, RunTimeOnlyIfMatchKind),
        m_allowType(NoneAllowType) { }


    bool init(std::string *error) override;
    bool evaluate(Rule *rule, Transaction *transaction) override;

    AllowType m_allowType;

    static std::string allowTypeToName (AllowType a) {
        if (a == NoneAllowType) {
            return "None";
        } else if (a == RequestAllowType) {
            return "Request";
        } else if (a == PhaseAllowType) {
            return "Phase";
        } else if (a == FromNowOneAllowType) {
            return "FromNowOne";
        } else {
            return "Unknown";
        }
    }
};

}  // namespace actions
}  // namespace modsecurity
#endif

#endif  // SRC_ACTIONS_ALLOW_H_
