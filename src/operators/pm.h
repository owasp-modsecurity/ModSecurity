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

#include "operators/operator.h"
#include "utils/acmp.h"

#ifdef __cplusplus
namespace ModSecurity {
namespace operators {


class Pm : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    Pm(std::string op, std::string param, bool negation)
        : Operator(op, param, negation) {
        m_p = acmp_create(0);
    }
    ~Pm();
    void replaceAll(std::string str, const std::string& from,
        const std::string& to);
    bool evaluate(Assay *assay, const std::string &input) override;

    bool init(const std::string &file, const char **error) override;
    void postOrderTraversal(acmp_btree_node_t *node);

    std::list<std::string> matched;
 protected:
    ACMP *m_p;
};


}  // namespace operators
}  // namespace ModSecurity
#endif


#endif  // SRC_OPERATORS_PM_H_
