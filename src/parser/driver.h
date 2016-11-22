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

#ifndef SRC_PARSER_DRIVER_H_
#define SRC_PARSER_DRIVER_H_

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "modsecurity/rules_properties.h"
#include "src/audit_log/audit_log.h"

#include "src/parser/seclang-parser.hh"

using modsecurity::Rule;
using modsecurity::Rules;


# define YY_DECL \
  yy::seclang_parser::symbol_type yylex(modsecurity::Parser::Driver& driver)

YY_DECL;

namespace modsecurity {
namespace Parser {

#ifdef __cplusplus
class Driver;
#else
typedef struct Driver_t Driver;
#endif


class Driver : public RulesProperties {
 public:
    Driver();
    virtual ~Driver();

    int addSecRule(Rule *rule);
    int addSecAction(Rule *rule);
    int addSecMarker(std::string marker);
    void configureCollectionBackend(std::string engine, std::string *error);

    int result;

    bool scan_begin();
    void scan_end();
    bool trace_scanning;

    int parseFile(const std::string& f);
    int parse(const std::string& f, const std::string &ref);

    void save_orig(std::string orig);
    std::string get_orig();

    std::string file;

    bool trace_parsing;

    void error(const yy::location& l, const std::string& m);
    void error(const yy::location& l, const std::string& m,
        const std::string& c);

    std::list<yy::location *> loc;

    std::list<std::string> ref;
    std::string buffer;
    Rule *lastRule;

    std::string m_orig;
};


}  // namespace Parser
}  // namespace modsecurity

#endif  // SRC_PARSER_DRIVER_H_
