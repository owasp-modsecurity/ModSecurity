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

#ifndef SRC_OPERATORS_PM_H_
#define SRC_OPERATORS_PM_H_

#include <string>
#include <list>
#include <memory>
#include <utility>

#include "src/operators/operator.h"
#ifdef WITH_HS
#include "src/utils/hyperscan.h"
#else
#include "src/utils/acmp.h"
#endif

namespace modsecurity {
namespace operators {


class Pm : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit Pm(std::unique_ptr<RunTimeString> param)
        : Operator("Pm", std::move(param)) {
#ifndef WITH_HS
        m_p = acmp_create(0);
#endif
    }
    explicit Pm(const std::string &n, std::unique_ptr<RunTimeString> param)
        : Operator(n, std::move(param)) {
#ifndef WITH_HS
        m_p = acmp_create(0);
#endif
    }
    ~Pm();
    bool evaluate(Transaction *transaction, RuleWithActions *rule,
        const std::string &str,
        std::shared_ptr<RuleMessage> ruleMessage) override;


    bool init(const std::string &file, std::string *error) override;
#ifndef WITH_HS
    void postOrderTraversal(acmp_btree_node_t *node);
    void cleanup(acmp_node_t *n);
#endif

 protected:
#ifdef WITH_HS
    std::shared_ptr<Utils::HyperscanPm> m_hs =
        std::make_shared<Utils::HyperscanPm>();
#else
    ACMP *m_p;
#endif

 private:
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_t m_lock;
#endif

};


}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_PM_H_
