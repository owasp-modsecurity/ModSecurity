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

#include "src/parser/driver.h"

#include "src/parser/seclang-parser.hh"
#include "modsecurity/audit_log.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/parser/default_driver_trail.h"

using modsecurity::audit_log::AuditLog;
using modsecurity::Rule;

namespace modsecurity {
namespace Parser {


Driver::Driver() :
    m_traceScanning(false),
    m_traceParsing(false),
    m_trail(new DefaultDriverTrail()) { }


Driver::Driver(DriverTrail *trail) :
    m_traceScanning(false),
    m_traceParsing(false),
    m_trail(trail) { }


Driver::~Driver() {
    while (m_location.empty() == false) {
        yy::location *a = m_location.back();
        m_location.pop_back();
        delete a;
    }

    if (m_trail != NULL) {
        delete m_trail;
        m_trail = NULL;
    }
}


int Driver::parse(const std::string &f, const std::string &ref) {
    //m_trail->m_lastRule = NULL;
    m_location.push_back(new yy::location());
    if (m_reference.empty()) {
        m_reference.push_back("<<reference missing or not informed>>");
    } else {
        m_reference.push_back(ref);
    }

    if (f.empty()) {
        return 1;
    }

    buffer = f;
    scanBegin();
    yy::seclang_parser parser(*this);
    parser.set_debug_level(m_traceParsing);
    int res = parser.parse();
    scanEnd();

    /*
    if (m_auditLog->init(&error) == false) {
        m_parserError << "Problems while initializing the audit logs: " \
            << error << std::endl;
        return false;
    }
    */

    return res == 0;
}


int Driver::parseFile(const std::string &f) {
    std::ifstream t(f);
    std::string str;

    if (utils::isFile(f) == false) {
        m_parserError << "Failed to open the file: " << f << std::endl;
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
    if (m_trail->m_error.tellp() > 0) {
        m_parserError << m_trail;
    }
    if (m_parserError.tellp() == 0) {
        m_parserError << "Rules error. ";
        if (m_reference.empty() == false) {
            m_parserError << "File: " << m_reference.back() << ". ";
        }
        m_parserError << "Line: " << l.end.line << ". ";
        m_parserError << "Column: " << l.end.column - 1 << ". ";
    }

    if (m.empty() == false) {
        m_parserError << "" << m << " ";
    }

    if (c.empty() == false) {
        m_parserError << c;
    }
}


}  // namespace Parser
}  // namespace modsecurity
