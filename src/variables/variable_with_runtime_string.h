/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/variables/variable.h"
#include "src/run_time_string.h"

#ifndef SRC_VARIABLES_VARIABLE_WITH_RUN_TIME_STRING_H_
#define SRC_VARIABLES_VARIABLE_WITH_RUN_TIME_STRING_H_

namespace modsecurity {
namespace variables {

class VariableWithRunTimeString : public Variable {
 public:
    VariableWithRunTimeString(const std::string &name, std::unique_ptr<RunTimeString> string)
        : Variable(name),
        m_string(std::move(string))
    { };

    VariableWithRunTimeString(const VariableWithRunTimeString &v)
        : Variable(v),
        m_string(std::unique_ptr<RunTimeString>(new RunTimeString(*v.m_string.get())))
    { };

    VariableWithRunTimeString& operator=(const VariableWithRunTimeString& v)
    {
        m_string = std::unique_ptr<RunTimeString>(new RunTimeString(*v.m_string.get()));
        return *this;
    }

    virtual void populate(RuleWithActions *rule) {
        if (m_string) {
            m_string->populate(rule);
        }
    }

    std::string evaluateRunTimeString(Transaction *t) {
        return m_string->evaluate(t);
    }

 protected:
    std::unique_ptr<RunTimeString> m_string;
};


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_VARIABLE_WITH_RUN_TIME_STRING_H_
