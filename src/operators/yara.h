/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2018 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef SRC_OPERATORS_YARA_H_
#define SRC_OPERATORS_YARA_H_

#include <string>
#include <memory>
#include <utility>

#ifdef WITH_YARA
#include <yara.h>
#endif

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {


struct yara_chunk {
    char *data;
    struct yara_chunk *next;
};

class Yara : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    explicit Yara(std::unique_ptr<RunTimeString> param)
        : Operator("Yara", std::move(param)),
        m_head(NULL),
        m_threshold(0) { }
    ~Yara();

    bool evaluate(Transaction *transaction, const std::string &std) override;

    bool init(const std::string &param, std::string *error) override;
 private:
    int m_threshold;
    struct yara_chunk *m_head;
};

}  // namespace operators
}  // namespace modsecurity


#endif  // SRC_OPERATORS_YARA_H_
