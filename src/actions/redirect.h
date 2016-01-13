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

#ifndef SRC_ACTIONS_REDIRECT_H_
#define SRC_ACTIONS_REDIRECT_H_

#ifdef __cplusplus
class Transaction;

namespace modsecurity {
class Transaction;

namespace actions {

class Redirect : public Action {
 public:
    explicit Redirect(const std::string &action);
    ~Redirect() override;

    bool evaluate(Rule *rule, Transaction *transaction) override;
    void fill_intervention(ModSecurityIntervention *i) override;
    bool isDisruptive() override { return true; }
 private:
    int m_status;
    std::string m_urlExpanded;
    std::string m_url;
};

}  // namespace actions
}  // namespace modsecurity
#endif

#endif  // SRC_ACTIONS_REDIRECT_H_
