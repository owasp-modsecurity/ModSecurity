/**
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
#include <stack>
#include <vector>
#endif

#ifndef SRC_RULE_H_
#define SRC_RULE_H_

#include "modsecurity/modsecurity.h"
#include "variables/variable.h"

#ifdef __cplusplus

namespace ModSecurity {

class Rule {
 public:
    Rule(operators::Operator *_op,
            std::vector<Variables::Variable *> *_variables,
            std::vector<actions::Action *> *_actions);

    bool evaluate(Assay *assay);

    operators::Operator *op;
    std::vector<actions::Action *> actions_conf;
    std::vector<actions::Action *> actions_runtime_pre;
    std::vector<actions::Action *> actions_runtime_pos;

    std::vector<Variables::Variable *> *variables;
    int phase;
    double rule_id;

    Rule *chainedRule;
    bool chained;
};

}  // namespace ModSecurity
#endif


#endif  // SRC_RULE_H_


