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

#include "parser/driver.h"

#include "parser/seclang-parser.hh"

Driver::Driver()
  : trace_scanning(false),
  trace_parsing(false),
  requestBodyLimit(0),
  responseBodyLimit(0),
  audit_log(new ModSecurity::AuditLog()) {
}


Driver::~Driver() {
}


int Driver::addSecRule(ModSecurity::Rule *rule) {
    if (rule->phase >= ModSecurity::ModSecurity::Phases::NUMBER_OF_PHASES) {
        /** TODO: return an error message */
        return -1;
    }

    int size = this->rules[rule->phase].size();

    if (size == 0) {
        this->rules[rule->phase].push_back(rule);
        return true;
    }

    ModSecurity::Rule *lastRule = this->rules[rule->phase][size-1];
    if (lastRule->chained && lastRule->chainedRule == NULL) {
        lastRule->chainedRule = rule;
        return true;
    }
    if (lastRule->chained && lastRule->chainedRule != NULL) {
        ModSecurity::Rule *a = lastRule->chainedRule;
        while (a->chained && a->chainedRule != NULL) {
            a = a->chainedRule;
        }
        if (a->chained && a->chainedRule == NULL) {
            a->chainedRule = rule;
            return true;
        }
    }

    this->rules[rule->phase].push_back(rule);

    return true;
}


int Driver::parse(const std::string &f) {
    file = f;
    scan_begin();
    yy::seclang_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    // yy_scan_buffer

    int res = parser.parse();

    if (this->audit_log->init() == false) {
        return false;
    }

    scan_end();
    return res;
}


int Driver::parseFile(const std::string &f) {
    file = f;
    scan_begin();
    yy::seclang_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();

    // std::cout << "Leaving the parser: " << res << std::endl;

    scan_end();
    return res;
}


void Driver::error(const yy::location& l, const std::string& m,
    const std::string& c) {
    if (parserError.tellp() == 0) {
        parserError << "Parser error, ";
        parserError << "Filename: " << file << ". ";
        parserError << "Line: " << l.end.line << ". ";
        parserError << "Column: " << l.end.column << ". ";
    }
    parserError << c;
}


void Driver::parser_error(const yy::location& l, const std::string& m) {
    parserError << ". " << m << "." << std::endl;
}
