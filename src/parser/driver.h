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
#include "src/rule.h"
#include "modsecurity/rules.h"
#include "src/audit_log.h"

#include "parser/seclang-parser.hh"

using ModSecurity::Rule;
using ModSecurity::Rules;


# define YY_DECL \
  yy::seclang_parser::symbol_type yylex(Driver& driver)

YY_DECL;


#ifdef __cplusplus
class Driver;
#else
typedef struct Driver_t Driver;
#endif


class Driver : public Rules {
/**
 * @todo Place driver and parser under the correct namespace.
 * 
 */
 public:
    Driver();
    virtual ~Driver();

    int addSecRule(Rule *rule);

    int result;

    // Handling the scanner.
    void scan_begin();
    void scan_end();
    bool trace_scanning;

    // Run the parser on file F.
    // Return 0 on success.
    int parseFile(const std::string& f);
    int parse(const std::string& f);

    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    std::string file;

    // Whether parser traces should be generated.
    bool trace_parsing;


    // Error handling.
    void error(const yy::location& l, const std::string& m);
    void parser_error(const yy::location& l, const std::string& m);
    void error(const yy::location& l, const std::string& m,
        const std::string& c);

    std::string buffer;
};


#endif  // SRC_PARSER_DRIVER_H_
