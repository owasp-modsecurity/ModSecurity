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

#include "modsecurity/rules.h"

#include "modsecurity/rule_message.h"

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/utils/string.h"
#include "src/utils/regex.h"
#include "modsecurity/actions/action.h"
#include "src/actions/transformations/transformation.h"

namespace modsecurity {

RuleMessage::RuleMessage(Rule *rule, Transaction *trans) :
    m_accuracy(rule->m_accuracy),
    m_buf(""),
    m_clientIpAddress(trans->m_clientIpAddress),
    m_data(""),
    m_disruptiveMessage(""),
    m_id(trans->m_id),
    m_isDisruptive(false),
    m_match(""),
    m_maturity(rule->m_maturity),
    m_message(""),
    m_noAuditLog(false),
    m_opValue(""),
    m_phase(rule->m_phase - 1),
    m_reference(""),
    m_rev(rule->m_rev),
    m_rule(rule),
    m_ruleFile(rule->m_fileName),
    m_ruleId(rule->m_ruleId),
    m_ruleLine(rule->m_lineNumber),
    m_saveMessage(true),
    m_serverIpAddress(trans->m_serverIpAddress),
    m_severity(0),
    m_uriNoQueryStringDecoded(trans->m_uri_no_query_string_decoded),
    m_varValue(""),
    m_ver(rule->m_ver) { }


std::string RuleMessage::disruptiveErrorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("[client " + std::string(rm->m_clientIpAddress) + "]");
    msg.append(" ModSecurity: ");
    msg.append(rm->m_disruptiveMessage);
    msg.append(rm->m_match);
    msg.append(" [file \"" + std::string(rm->m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(rm->m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(rm->m_ruleId) + "\"]");
    msg.append(" [rev \"" + rm->m_rev + "\"]");
    msg.append(" [msg \"" + rm->m_message + "\"]");
    msg.append(" [data \"" + rm->m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(rm->m_severity) + "\"]");
    msg.append(" [ver \"" + rm->m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(rm->m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(rm->m_accuracy) + "\"]");
    for (auto &a : rm->m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [hostname \"" + std::string(rm->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + rm->m_uriNoQueryStringDecoded + "\"]");
    msg.append(" [unique_id \"" + rm->m_id + "\"]");
    msg.append(" [ref \"" + rm->m_reference + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


std::string RuleMessage::noClientErrorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("ModSecurity: Warning. ");
    msg.append(rm->m_match);
    msg.append(" [file \"" + std::string(rm->m_ruleFile) + "\"]");
    msg.append(" [line \"" + std::to_string(rm->m_ruleLine) + "\"]");
    msg.append(" [id \"" + std::to_string(rm->m_ruleId) + "\"]");
    msg.append(" [rev \"" + rm->m_rev + "\"]");
    msg.append(" [msg \"" + rm->m_message + "\"]");
    msg.append(" [data \"" + rm->m_data + "\"]");
    msg.append(" [severity \"" +
        std::to_string(rm->m_severity) + "\"]");
    msg.append(" [ver \"" + rm->m_ver + "\"]");
    msg.append(" [maturity \"" + std::to_string(rm->m_maturity) + "\"]");
    msg.append(" [accuracy \"" + std::to_string(rm->m_accuracy) + "\"]");
    for (auto &a : rm->m_tags) {
        msg.append(" [tag \"" + a + "\"]");
    }
    msg.append(" [ref \"" + rm->m_reference + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


std::string RuleMessage::errorLogTail(const RuleMessage *rm) {
    std::string msg;

    msg.append("[hostname \"" + std::string(rm->m_serverIpAddress) \
        + "\"]");
    msg.append(" [uri \"" + rm->m_uriNoQueryStringDecoded + "\"]");
    msg.append(" [unique_id \"" + rm->m_id + "\"]");

    return modsecurity::utils::string::toHexIfNeeded(msg);
}


std::string RuleMessage::errorLog(const RuleMessage *rm) {
    std::string msg;

    msg.append("[client " + std::string(rm->m_clientIpAddress) + "] ");
    msg.append(noClientErrorLog(rm));
    msg.append(" " + errorLogTail(rm));

    return msg;
}


std::string RuleMessage::log(const RuleMessage *rm) {
    std::string msg("");
    if (rm->m_isDisruptive) {
        msg.append(disruptiveErrorLog(rm));
    } else {
        msg.append(errorLog(rm));
    }

    return msg;
}


RuleMessageHighlight RuleMessage::computeHighlight(const RuleMessage *rm,
    const std::string buf) {
    RuleMessageHighlight ret;
    Utils::Regex variables("v([0-9]+),([0-9]+)");
    Utils::Regex operators("o([0-9]+),([0-9]+)");
    Utils::Regex transformations("t:(?:(?!t:).)+");

    std::string ref(rm->m_reference);
    std::list<Utils::SMatch> vars = variables.searchAll(ref);
    std::list<Utils::SMatch> ops = operators.searchAll(ref);
    std::list<Utils::SMatch> trans = transformations.searchAll(ref);

    std::string varValue;

    while (vars.size() > 0) {
        std::string value;
        RuleMessageHighlightArea a;
        vars.pop_back();
        std::string startingAt = vars.back().match;
        vars.pop_back();
        std::string size = vars.back().match;
        vars.pop_back();
        a.m_startingAt = std::stoi(startingAt);
        a.m_size = std::stoi(size);
        ret.m_variable.push_back(a);

        if ((stoi(startingAt) + stoi(size)) > buf.size()) {
            return ret;
        }

        value = std::string(buf, stoi(startingAt), stoi(size));
        if (varValue.size() > 0) {
            varValue.append(" " + value);
        } else {
            varValue.append(value);
        }
    }

    ret.m_value.push_back(std::make_pair("original value", varValue));
    while (trans.size() > 0) {
        modsecurity::actions::transformations::Transformation *t;
        std::string varValueRes;
        std::string transformation = trans.back().match.c_str();
        t = actions::transformations::Transformation::instantiate(
            transformation);

        varValueRes = t->evaluate(varValue, NULL);
        varValue.assign(varValueRes);
        ret.m_value.push_back(std::make_pair(transformation, varValue));
        trans.pop_back();
        delete t;
    }

    while (ops.size() > 0) {
        RuleMessageHighlightOperator o;
        ops.pop_back();
        std::string startingAt = ops.back().match;
        ops.pop_back();
        std::string size = ops.back().match;
        ops.pop_back();

        if ((stoi(startingAt) + stoi(size)) > buf.size()) {
            return ret;
        }

        o.m_area.m_startingAt = std::stoi(startingAt);
        o.m_area.m_size = std::stoi(size);
        o.m_value.assign(std::string(varValue, o.m_area.m_startingAt,
            o.m_area.m_size));
        ret.m_op.push_back(o);
    }

    return ret;
}


}  // namespace modsecurity
