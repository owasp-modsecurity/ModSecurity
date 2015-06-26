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

#include <string>

#include "actions/action.h"
#include "actions/transformations/transformation.h"

#ifndef SRC_ACTIONS_TRANSFORMATIONS_JS_DECODE_H_
#define SRC_ACTIONS_TRANSFORMATIONS_JS_DECODE_H_

#ifdef __cplusplus
namespace ModSecurity {
class Assay;

namespace actions {
namespace transformations {

class JsDecode : public Transformation {
 public:
    explicit JsDecode(std::string action);
    std::string& evaluate(std::string exp,
        Assay *assay) override;
};

}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity

#endif

#endif  // SRC_ACTIONS_TRANSFORMATIONS_JS_DECODE_H_
