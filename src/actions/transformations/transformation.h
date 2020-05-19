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


#include <algorithm>
#include <string>

#include "modsecurity/modsecurity.h"
#include "modsecurity/actions/action.h"

#include "src/actions/action_allowed_in_sec_default_action.h"


#ifndef SRC_ACTIONS_TRANSFORMATIONS_TRANSFORMATION_H_
#define SRC_ACTIONS_TRANSFORMATIONS_TRANSFORMATION_H_


namespace modsecurity {
namespace actions {
namespace transformations {


class Transformation : public ActionAllowedAsSecDefaultAction {
 public:
    virtual void execute(const Transaction *t,
        const ModSecString &in,
        ModSecString &out) noexcept = 0;

    virtual ~Transformation()
    { }

    static Transformation* instantiate(const std::string &name);

 private:
    static bool match(const std::string &a, const std::string &b) noexcept {
        return ((a.size() == b.size())
            && std::equal(a.begin(), a.end(), b.begin(),
                [](const char & c1, const char & c2) {
				    return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
				}));
    }
};


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity


#endif  // SRC_ACTIONS_TRANSFORMATIONS_TRANSFORMATION_H_
