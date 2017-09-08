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

#ifndef SRC_OPERATORS_PM_H_
#define SRC_OPERATORS_PM_H_

#include <string>
#include <list>
#include <memory>

#include "src/operators/operator.h"
#include "src/utils/acmp.h"


namespace modsecurity {
namespace operators {


class Pm : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    Pm(std::string op, std::string param, bool negation)
        : Operator(op, param, negation) {
        m_p = acmp_create(0);
    }
    Pm(std::string op, std::string param)
        : Operator(op, param) {
        m_p = acmp_create(0);
    }
    explicit Pm(std::string param)
        : Operator("Pm", param) {
        m_p = acmp_create(0);
    }
    ~Pm();
    bool evaluate(Transaction *transaction, Rule *rule,
        const std::string &str,
        std::shared_ptr<RuleMessage> ruleMessage) override;


    bool init(const std::string &file, std::string *error) override;
    void postOrderTraversal(acmp_btree_node_t *node);
    void cleanup(acmp_node_t *n);

 protected:
    ACMP *m_p;

#ifdef MODSEC_MUTEX_ON_PM
 private:
    pthread_mutex_t m_lock;
#endif
};


}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_PM_H_
