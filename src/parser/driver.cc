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

#include "parser/driver.h"

#include "parser/seclang-parser.hh"
#include "src/audit_log.h"

using ModSecurity::AuditLog;
using ModSecurity::Rule;

namespace ModSecurity {
namespace Parser {

Driver::Driver()
  : trace_scanning(false),
  trace_parsing(false) {
      audit_log = new AuditLog();
  }


Driver::~Driver() {
}


int Driver::addSecRule(Rule *rule) {
    if (rule->phase >= ModSecurity::Phases::NUMBER_OF_PHASES) {
        /** TODO: return an error message */
        return -1;
    }

    int size = this->rules[rule->phase].size();

    if (size == 0) {
        this->rules[rule->phase].push_back(rule);
        return true;
    }

    Rule *lastRule = this->rules[rule->phase][size-1];
    if (lastRule->chained && lastRule->chainedRule == NULL) {
        lastRule->chainedRule = rule;
        return true;
    }
    if (lastRule->chained && lastRule->chainedRule != NULL) {
        Rule *a = lastRule->chainedRule;
        while (a->chained && a->chainedRule != NULL) {
            a = a->chainedRule;
        }
        if (a->chained && a->chainedRule == NULL) {
            a->chainedRule = rule;
            return true;
        }
    }

    rules[rule->phase].push_back(rule);
    return true;
}


int Driver::parse(const std::string &f, const std::string &ref) {
    if (ref.empty()) {
        this->ref = "<<reference missing or not informed>>";
    } else {
        this->ref = ref;
    }

    buffer = f;
    scan_begin();
    yy::seclang_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();

    if (audit_log->init() == false) {
        return false;
    }
    scan_end();

    return res == 0;
}


int Driver::parseFile(const std::string &f) {
    std::ifstream t(f);
    std::string str;

    if (t.is_open() == false) {
        parserError << "Failed to open the file: " << f << std::endl;
        return false;
    }

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    return parse(str, f);
}


void Driver::error(const yy::location& l, const std::string& m) {
    error(l, m, "");
}


void Driver::error(const yy::location& l, const std::string& m,
    const std::string& c) {
    if (parserError.tellp() == 0) {
        parserError << "Configuration error, ";
        parserError << "File: " << ref << ". ";
        parserError << "Line: " << l.end.line << ". ";
        parserError << "Column: " << l.end.column << ". ";
    }
    if (c.empty() == false) {
        parserError << c;
    }
}


}  // namespace Parser
}  // namespace ModSecurity
