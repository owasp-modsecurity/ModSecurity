
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

#ifndef HEADERS_MODSECURITY_PARSER_DRIVER_H_
#define HEADERS_MODSECURITY_PARSER_DRIVER_H_

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/audit_log.h"
#include "modsecurity/parser/driver_trail.h"


using modsecurity::Rule;
using modsecurity::Rules;


namespace modsecurity {
namespace Parser {

#ifdef __cplusplus
class Driver;
#else
typedef struct Driver_t Driver;
#endif


class Driver {
 public:
    Driver(DriverTrail *trail);
    Driver();
    virtual ~Driver();

    bool scanBegin();
    void scanEnd();

    int parseFile(const std::string& f);
    int parse(const std::string& f, const std::string &ref);

    void error(const yy::location& l, const std::string& m);
    void error(const yy::location& l, const std::string& m,
        const std::string& c);


    int addSecRule(Rule *rule) {
        if (!m_trail) {
            return -1;
        }
        return m_trail->addSecRule(rule);
    }
    int addSecAction(Rule *rule) {
        if (!m_trail) {
            return -1;
        }
        return m_trail->addSecAction(rule);
    }
    int addSecMarker(std::string marker) {
        if (!m_trail) {
            return -1;
        }
        return m_trail->addSecMarker(marker);
    }
    int addSecRuleScript(RuleScript *rule) {
        if (!m_trail) {
            return -1;
        }
        return m_trail->addSecRuleScript(rule);
    }

    DriverTrail *m_trail;

    bool m_traceScanning;
    bool m_traceParsing;
    std::string m_file;
    std::list<yy::location *> m_location;
    std::list<std::string> m_reference;
    std::string buffer;
    std::ostringstream m_parserError;
};


}  // namespace Parser
}  // namespace modsecurity

#endif  // HEADERS_MODSECURITY_PARSER_DRIVER_H_
