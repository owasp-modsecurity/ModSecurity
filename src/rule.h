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

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
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
            std::vector<actions::Action *> *_actions,
            std::string fileName,
            int lineNumber
        );
    explicit Rule(std::string marker);

    ~Rule();
    bool evaluate(Assay *assay);
    bool evaluateActions(Assay *assay);

    operators::Operator *op;
    std::vector<actions::Action *> actions_conf;
    std::vector<actions::Action *> actions_runtime_pre;
    std::vector<actions::Action *> actions_runtime_pos;

    std::vector<Variables::Variable *> *variables;
    int phase;
    long rule_id;

    Rule *chainedRule;
    bool chained;

    void refCountDecreaseAndCheck() {
        this->m_referenceCount--;
        if (this->m_referenceCount == 0) {
            delete this;
        }
    }

    void refCountIncrease() {
        this->m_referenceCount++;
    }

    std::string rev;

    std::string m_marker;
    bool m_secmarker;
    std::string m_fileName;
    int m_lineNumber;

 private:
    bool m_unconditional;
    int m_referenceCount;
};

}  // namespace ModSecurity
#endif


#endif  // SRC_RULE_H_


