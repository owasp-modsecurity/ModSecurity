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
#include "audit_log/audit_log.h"

using modsecurity::audit_log::AuditLog;
using modsecurity::Rule;

namespace modsecurity {
namespace Parser {

Driver::Driver()
  : trace_scanning(false),
  trace_parsing(false) {
      audit_log = new audit_log::AuditLog();
      audit_log->refCountIncrease();
  }


Driver::~Driver() {
    if (audit_log != NULL) {
        audit_log->refCountDecreaseAndCheck();
    }
    delete loc.back();
}


int Driver::addSecMarker(std::string marker) {
    for (int i = 0; i < ModSecurity::Phases::NUMBER_OF_PHASES; i++) {
        Rule *rule = new Rule(marker);
        rule->phase = i;
        rules[i].push_back(rule);
    }
    return 0;
}


int Driver::addSecAction(Rule *rule) {
    if (rule->phase > ModSecurity::Phases::NUMBER_OF_PHASES) {
        parserError << "Unknown phase: " << std::to_string(rule->phase);
        parserError << std::endl;
        return false;
    }

    rules[rule->phase].push_back(rule);

    return true;
}

int Driver::addSecRule(Rule *rule) {
    if (rule->phase > ModSecurity::Phases::NUMBER_OF_PHASES) {
        parserError << "Unknown phase: " << std::to_string(rule->phase);
        parserError << std::endl;
        return false;
    }

    if (lastRule && lastRule->chained && lastRule->chainedRule == NULL) {
        rule->phase = lastRule->phase;
        lastRule->chainedRule = rule;
        return true;
    }

    if (lastRule && lastRule->chained && lastRule->chainedRule != NULL) {
        Rule *a = lastRule->chainedRule;
        while (a->chained && a->chainedRule != NULL) {
            a = a->chainedRule;
        }
        if (a->chained && a->chainedRule == NULL) {
            a->chainedRule = rule;
            return true;
        }
    }

    /*
     * Checking if the rule has an ID and also checking if this ID is not used
     * by other rule
     */
    if (rule->rule_id == 0) {
        parserError << "Rules must have an ID. File: ";
        parserError << rule->m_fileName << " at line: ";
        parserError << std::to_string(rule->m_lineNumber) << std::endl;
        return false;
    }
    for (int i = 0; i < ModSecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::vector<Rule *> rules = this->rules[i];
        for (int j = 0; j < rules.size(); j++) {
            if (rules[j]->rule_id == rule->rule_id) {
                parserError << "Rule id: " << std::to_string(rule->rule_id) \
                    << " is duplicated" << std::endl;
                return false;
            }
        }
    }

    lastRule = rule;
    rules[rule->phase].push_back(rule);
    return true;
}


int Driver::parse(const std::string &f, const std::string &ref) {
    lastRule = NULL;
    loc.push_back(new yy::location());
    if (ref.empty()) {
        this->ref.push_back("<<reference missing or not informed>>");
    } else {
        this->ref.push_back(ref);
    }

    buffer = f;
    scan_begin();
    yy::seclang_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end();

    if (audit_log->init() == false) {
        parserError << "Problems while initializing the audit logs" \
            << std::endl;
        return false;
    }

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
        parserError << "Rules error. ";
        if (ref.empty() == false) {
            parserError << "File: " << ref.back() << ". ";
        }
        parserError << "Line: " << l.end.line << ". ";
        parserError << "Column: " << l.end.column << ". ";
    }

    if (m.empty() == false) {
        parserError << "" << m << " ";
    }

    if (c.empty() == false) {
        parserError << c;
    }
}


}  // namespace Parser
}  // namespace modsecurity
