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

std::string RuleMessage::_details(const RuleMessage *rm) {
    std::string msg;

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

    return msg;
}


std::string RuleMessage::_errorLogTail(const RuleMessage *rm) {
    std::string msg;

    msg.append("[hostname \"" + std::string(rm->m_serverIpAddress) + "\"]");
    msg.append(" [uri \"" + rm->m_uriNoQueryStringDecoded + "\"]");
    msg.append(" [unique_id \"" + rm->m_id + "\"]");

    return msg;
}

std::string RuleMessage::log(const RuleMessage *rm, int props, int code) {
    std::string msg("");

    if (props & ClientLogMessageInfo) {
        msg.append("[client " + std::string(rm->m_clientIpAddress) + "] ");
    }

    if (rm->m_isDisruptive) {
        msg.append("ModSecurity: Access denied with code ");
        if (code == -1) {
            msg.append("%d");
        } else {
            msg.append(std::to_string(code));
        }
        msg.append(" (phase ");
        msg.append(std::to_string(rm->m_rule->m_phase - 1) + "). ");
    } else {
        msg.append("ModSecurity: Warning. ");
    }

    msg.append(rm->m_match);
    msg.append(_details(rm));

    if (props & ErrorLogTailLogMessageInfo) {
        msg.append(" " + _errorLogTail(rm));
    }

    return modsecurity::utils::string::toHexIfNeeded(msg);
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
