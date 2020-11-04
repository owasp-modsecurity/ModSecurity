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

#ifndef SRC_OPERATORS_FUZZY_HASH_H_
#define SRC_OPERATORS_FUZZY_HASH_H_

#include <string>
#include <memory>
#include <utility>

#ifdef WITH_SSDEEP
#include <fuzzy.h>
#endif

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {


struct fuzzy_hash_chunk {
    char *data;
    struct fuzzy_hash_chunk *next;
};

class FuzzyHash : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit FuzzyHash(std::unique_ptr<RunTimeString> param)
        : Operator("FuzzyHash", std::move(param)),
        m_threshold(0),
        m_head(NULL) { }
    ~FuzzyHash();

    bool evaluate(Transaction *transaction,
        const RuleWithActions *rule,
        const bpstd::string_view &input,
        RuleMessage *ruleMessage) override;

    bool init(const std::string &file, std::string *error) override;
 private:
    int m_threshold;
    struct fuzzy_hash_chunk *m_head;
};

}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_FUZZY_HASH_H_
