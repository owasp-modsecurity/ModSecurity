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
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <list>
#endif

#ifndef HEADERS_MODSECURITY_PARSER_DEFAULT_DRIVER_TRAIL_H_
#define HEADERS_MODSECURITY_PARSER_DEFAULT_DRIVER_TRAIL_H_

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/audit_log.h"
#include "modsecurity/parser/driver_trail.h"


namespace modsecurity {
namespace Parser {

#ifdef __cplusplus
class DriverTrail;
#else
typedef struct DriverTrail_t DriverTrail;
#endif


class DefaultDriverTrail : public DriverTrail {
 public:
    int addSecRule(Rule *rule);
    int addSecAction(Rule *rule);
    int addSecMarker(std::string marker);
    int addSecRuleScript(Rule *rule);

    Rule *m_lastRule;
};


}  // namespace Parser
}  // namespace modsecurity

#endif  // HEADERS_MODSECURITY_PARSER_DEFAULT_DRIVER_TRAIL_H_
