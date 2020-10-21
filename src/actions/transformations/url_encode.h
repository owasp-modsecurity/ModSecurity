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

#include <string>

#include "modsecurity/actions/action.h"
#include "src/actions/transformations/transformation.h"

#ifndef SRC_ACTIONS_TRANSFORMATIONS_URL_ENCODE_H_
#define SRC_ACTIONS_TRANSFORMATIONS_URL_ENCODE_H_

#ifdef __cplusplus
namespace modsecurity {
class Transaction;

namespace actions {
namespace transformations {

class UrlEncode : public Transformation {
 public:
    explicit UrlEncode(const std::string &action)
        : Transformation(action) { };

    void execute(Transaction *t,
        ModSecString &in,
        ModSecString &out) override;

    static std::string url_enc(const char *input,
        unsigned int input_len, int *changed);
};

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // SRC_ACTIONS_TRANSFORMATIONS_URL_ENCODE_H_
