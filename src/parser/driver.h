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

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/audit_log.h"
#include "src/parser/seclang-parser.hh"
#include "src/rule_script.h"
#include "modsecurity/parser/driver_trail.h"


using modsecurity::Rule;
using modsecurity::Rules;

#ifndef SRC_PARSER_DRIVER_H_
#define SRC_PARSER_DRIVER_H_

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

}  // namespace Parser
}  // namespace modsecurity

#include "modsecurity/parser/driver.h"

#endif  // SRC_PARSER_DRIVER_H_
