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

    static void id(Transaction *t,
        const RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
        std::string *a = new std::string(std::to_string(rule->getId()));
        VariableValue *var = new VariableValue(&m_rule, &m_rule_id,
            a
        );
        delete a;
        origin->m_offset = 0;
        origin->m_length = 0;
        var->addOrigin(std::move(origin));
        l->push_back(var);
    }



    static void rev(Transaction *t,
        const RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {

        if (rule->hasRevisionAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(rule->getRevision());
            VariableValue *var = new VariableValue(&m_rule, &m_rule_rev,
                a
            );
            delete a;
            origin->m_offset = 0;
            origin->m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(var);
        }
    }


    static void severity(Transaction *t,
        const RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {

        if (rule->hasSeverityAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(std::to_string(rule->getSeverity()));
            VariableValue *var = new VariableValue(&m_rule, &m_rule_severity,
                a
            );
            delete a;
            origin->m_offset = 0;
            origin->m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(var);
        }

    }

    static void logData(Transaction *t,
        const RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {

        if (rule->hasLogDataAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(rule->getLogData(t));
            VariableValue *var = new VariableValue(&m_rule, &m_rule_logdata,
                a
            );
            delete a;
            origin->m_offset = 0;
            origin->m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(var);
        }
    }

    static void msg(Transaction *t,
        const RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {

        if (rule->hasMessageAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(rule->getMessage(t));
            VariableValue *var = new VariableValue(&m_rule, &m_rule_msg,
                a
            );
            delete a;
            origin->m_offset = 0;
            origin->m_length = 0;
            var->addOrigin(std::move(origin));
            l->push_back(var);
        }
    }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {

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


    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {

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


    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) override {
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
