/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/actions/phase.h"

#include <iostream>
#include <string>
#include <charconv>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "modsecurity/modsecurity.h"
#include "src/utils/string.h"


namespace modsecurity {
namespace actions {

bool Phase::init(std::string *error) {
    std::string a = utils::string::tolower(m_parser_payload);
    m_phase = -1;


    const auto conv_res = std::from_chars(m_parser_payload.data(), m_parser_payload.data() + m_parser_payload.size(), m_phase);
    if (conv_res.ec == std::errc::invalid_argument) {
        // Conversion Fail
        if (a == "request") {
            m_phase = modsecurity::Phases::RequestBodyPhase;
            m_secRulesPhase = 2;
        } else if (a == "response") {
            m_phase = modsecurity::Phases::ResponseBodyPhase;
            m_secRulesPhase = 4;
        } else if (a == "logging") {
            m_phase = modsecurity::Phases::LoggingPhase;
            m_secRulesPhase = 5;
        }
    } else {
        // Conversion Done
        if (m_phase == 0) {
            m_phase = modsecurity::Phases::ConnectionPhase;
            m_secRulesPhase = 0;
        } else if (m_phase == 1) {
            m_phase = modsecurity::Phases::RequestHeadersPhase;
            m_secRulesPhase = 1;
        } else if (m_phase == 2) {
            m_phase = modsecurity::Phases::RequestBodyPhase;
            m_secRulesPhase = 2;
        } else if (m_phase == 3) {
            m_phase = modsecurity::Phases::ResponseHeadersPhase;
            m_secRulesPhase = 3;
        } else if (m_phase == 4) {
            m_phase = modsecurity::Phases::ResponseBodyPhase;
            m_secRulesPhase = 4;
        } else if (m_phase == 5) {
            m_phase = modsecurity::Phases::LoggingPhase;
            m_secRulesPhase = 5;
        } else {
            error->assign("Unknown phase: " + m_parser_payload);
            return false;
        }
    }
    return true;
}


bool Phase::evaluate(RuleWithActions *rule, Transaction *transaction) {
    rule->setPhase(m_phase);
    return true;
}

}  // namespace actions
}  // namespace modsecurity
