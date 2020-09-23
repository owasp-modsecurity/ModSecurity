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

#include "modsecurity/modsecurity.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/rules_set.h"

#ifndef SRC_TRANSFORMATION_RESULT_H_
#define SRC_TRANSFORMATION_RESULT_H_


namespace modsecurity {


class TransformationResult {
 public:
    explicit TransformationResult(
        ModSecString &after,
        const std::string *transformation = nullptr)
        : m_after(after),
        m_transformation(transformation) { };

    explicit TransformationResult(
        ModSecString *after)
        : m_after(*after),
        m_transformation(nullptr) { };

    TransformationResult(const TransformationResult &t2)
        : m_after(t2.m_after),
        m_transformation(t2.m_transformation) { };


    ModSecString *getAfter() {
        return &m_after;
    }


    const std::string *getTransformationName() const {
        return m_transformation;
    }


 private:
    ModSecString m_after;
    const std::string *m_transformation;
};

using TransformationsResults = std::list<TransformationResult>;

}  // namespace modsecurity



#endif  //  SRC_TRANSFORMATION_RESULT_H_

