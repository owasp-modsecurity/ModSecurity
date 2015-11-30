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
#ifndef SRC_OPERATORS_IP_MATCH_FROM_FILE_H_
#define SRC_OPERATORS_IP_MATCH_FROM_FILE_H_

#include <string>

#include "operators/ip_match.h"

#ifdef __cplusplus
namespace modsecurity {
namespace operators {

class IpMatchFromFile : public IpMatch {
 public:
    /** @ingroup ModSecurity_Operator */
    IpMatchFromFile(std::string op, std::string param, bool negation)
        : IpMatch(op, param, negation) { }

    bool init(const std::string& file, const char **error) override;
};

}  // namespace operators
}  // namespace ModSecurity
#endif


#endif  // SRC_OPERATORS_IP_MATCH_FROM_FILE_H_
