/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <string>
#include <vector>
#include <list>

#ifndef SRC_VARIABLES_RULE_H_
#define SRC_VARIABLES_RULE_H_

#include "src/variables/variable.h"
#include "src/actions/severity.h"
#include "src/actions/log_data.h"
#include "src/actions/msg.h"
#include "src/rule_with_actions.h"


namespace modsecurity {

class Transaction;
namespace variables {


class Rule_DictElement : public RuleVariable, public VariableDictElement {
 public:
    explicit Rule_DictElement(const std::string &dictElement)
        : RuleVariable(),
        VariableDictElement(
            std::string("RULE"),
            dictElement
        )
    { };


    Rule_DictElement(const Rule_DictElement &r)
        : RuleVariable(),
        VariableDictElement(r)
    { };


    virtual Variable *clone() override {
        return new Rule_DictElement(*this);
    };

    static void id(const Transaction *t,
        const RuleWithActions *rule,
        VariableValues *l) {
        auto var = std::make_shared<VariableValue>(&m_rule, &m_rule_id, std::unique_ptr<std::string>(new std::string(std::to_string(rule->getId()))));
        VariableOrigin origin;
        origin.m_offset = 0;
        origin.m_length = 0;
        var->addOrigin(std::move(origin));
        l->push_back(std::move(var));
    }



    static void rev(const Transaction *t,
        const RuleWithActions *rule,
        VariableValues *l) {

        if (rule->hasRevisionAction()) {
            auto var = std::make_shared<VariableValue>(&m_rule, &m_rule_rev, std::unique_ptr<std::string>(new std::string(rule->getRevision())));
            VariableOrigin origin;
            origin.m_offset = 0;
            origin.m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(var);
            l->push_back(std::move(var));
        }
    }


    static void severity(const Transaction *t,
        const RuleWithActions *rule,
        VariableValues *l) {

        if (rule->hasSeverityAction()) {
            auto var = std::make_shared<VariableValue>(&m_rule, &m_rule_severity, std::unique_ptr<std::string>(new std::string(std::to_string(rule->getSeverity()))));
            VariableOrigin origin;
            origin.m_offset = 0;
            origin.m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(std::move(var));
        }

    }

    static void logData(const Transaction *t,
        const RuleWithActions *rule,
        VariableValues *l) {

        if (rule->hasLogDataAction()) {
            auto var = std::make_shared<VariableValue>(&m_rule, &m_rule_logdata, std::unique_ptr<std::string>(new std::string(rule->getLogData(t))));
            VariableOrigin origin;
            origin.m_offset = 0;
            origin.m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(std::move(var));
        }
    }

    static void msg(const Transaction *t,
        const RuleWithActions *rule,
        VariableValues *l) {

        if (rule->hasMessageAction()) {
            auto var = std::make_shared<VariableValue>(&m_rule, &m_rule_msg, std::unique_ptr<std::string>(new std::string(rule->getMessage(t))));
            VariableOrigin origin;
            origin.m_offset = 0;
            origin.m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(std::move(var));
        }
    }

    void evaluate(const Transaction *t,
        VariableValues *l) const noexcept override {

        if (m_dictElement == "id") {
            id(t, getRule(), l);
            return;
        }
        if (m_dictElement == "rev") {
            rev(t, getRule(), l);
            return;
        }
        if (m_dictElement == "severity") {
            severity(t, getRule(), l);
            return;
        }
        if (m_dictElement == "logdata") {
            logData(t, getRule(), l);
            return;
        }
        if (m_dictElement == "msg") {
            msg(t, getRule(), l);
            return;
        }
    }

    static const std::string m_rule;
    static const std::string m_rule_id;
    static const std::string m_rule_rev;
    static const std::string m_rule_severity;
    static const std::string m_rule_logdata;
    static const std::string m_rule_msg;
};


class Rule_DictElementRegexp : public RuleVariable, public VariableRegex {
 public:
    explicit Rule_DictElementRegexp(const std::string &regex)
        : RuleVariable(),
        VariableRegex("RULE", regex)
    { };


    Rule_DictElementRegexp(const Rule_DictElementRegexp &r)
        : RuleVariable(),
        VariableRegex(r)
    { };


    void evaluate(const Transaction *t,
        VariableValues *l) const noexcept override {

        if (Utils::regex_search("id", m_r) > 0) {
            Rule_DictElement::id(t, getRule(), l);
            return;
        }
        if (Utils::regex_search("rev", m_r) > 0) {
            Rule_DictElement::rev(t, getRule(), l);
            return;
        }
        if (Utils::regex_search("severity", m_r) > 0) {
            Rule_DictElement::severity(t, getRule(), l);
            return;
        }
        if (Utils::regex_search("logdata", m_r) > 0) {
            Rule_DictElement::logData(t, getRule(), l);
            return;
        }
        if (Utils::regex_search("msg", m_r) > 0) {
            Rule_DictElement::msg(t, getRule(), l);
            return;
        }
    }

    virtual Variable *clone() override {
        return new Rule_DictElementRegexp(*this);
    };
};


class Rule_NoDictElement : public RuleVariable, public Variable  {
 public:
    Rule_NoDictElement()
        : RuleVariable(),
        Variable("RULE")
    { };


    explicit Rule_NoDictElement(const Rule_DictElementRegexp &r)
        : RuleVariable(),
        Variable(r)
    { };


    void evaluate(const Transaction *t,
        VariableValues *l) const noexcept override {
        Rule_DictElement::id(t, getRule(), l);
        Rule_DictElement::rev(t, getRule(), l);
        Rule_DictElement::severity(t, getRule(), l);
        Rule_DictElement::logData(t, getRule(), l);
        Rule_DictElement::msg(t, getRule(), l);
    }

    virtual Variable *clone() override {
        return new Rule_NoDictElement(*this);
    };
};

// DEFINE_VARIABLE_DICT(Rule, RULE, m_variableRule)


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_RULE_H_
