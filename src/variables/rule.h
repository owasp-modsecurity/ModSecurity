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

#include <string>
#include <vector>
#include <list>

#ifndef SRC_VARIABLES_RULE_H_
#define SRC_VARIABLES_RULE_H_

#include "src/variables/variable.h"
#include "src/actions/severity.h"
#include "src/actions/log_data.h"
#include "src/actions/msg.h"


namespace modsecurity {

class Transaction;
namespace variables {


class Rule_DictElement : public VariableDictElement { \
 public:
    explicit Rule_DictElement(const std::string &dictElement)
        : VariableDictElement(std::string("RULE"), dictElement) { }

    static void id(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        RuleWithActions *r = rule;

        if (!r || r->m_ruleId == 0) {
            return;
        }
        std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
        std::string *a = new std::string(std::to_string(r->m_ruleId));
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
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        RuleWithActions *r = rule;

        if (!r) {
            return;
        }

        if (r->hasRevisionAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(r->getRevision());
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
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        RuleWithActions *r = rule;

        if (!r) {
            return;
        }

        if (r->hasSeverityAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(std::to_string(r->getSeverity()));
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
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        RuleWithActions *r = rule;

        if (!r) {
            return;
        }

        if (r->hasLogDataAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(r->getLogData(t));
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
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) {
        RuleWithActions *r = rule;

        if (!r) {
            return;
        }

        if (r->hasMessageAction()) {
            std::unique_ptr<VariableOrigin> origin(new VariableOrigin());
            std::string *a = new std::string(r->getMessage(t));
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
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        if (m_dictElement == "id") {
            id(t, rule, l);
            return;
        }
        if (m_dictElement == "rev") {
            rev(t, rule, l);
            return;
        }
        if (m_dictElement == "severity") {
            severity(t, rule, l);
            return;
        }
        if (m_dictElement == "logdata") {
            logData(t, rule, l);
            return;
        }
        if (m_dictElement == "msg") {
            msg(t, rule, l);
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


class Rule_DictElementRegexp : public VariableRegex {
 public:
    explicit Rule_DictElementRegexp(const std::string &regex)
        : VariableRegex("RULE", regex) { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        if (Utils::regex_search("id", m_r) > 0) {
            Rule_DictElement::id(t, rule, l);
            return;
        }
        if (Utils::regex_search("rev", m_r) > 0) {
            Rule_DictElement::rev(t, rule, l);
            return;
        }
        if (Utils::regex_search("severity", m_r) > 0) {
            Rule_DictElement::severity(t, rule, l);
            return;
        }
        if (Utils::regex_search("logdata", m_r) > 0) {
            Rule_DictElement::logData(t, rule, l);
            return;
        }
        if (Utils::regex_search("msg", m_r) > 0) {
            Rule_DictElement::msg(t, rule, l);
            return;
        }
    }
};


class Rule_NoDictElement : public Variable {
 public:
    explicit Rule_NoDictElement()
        : Variable("RULE") { }

    void evaluate(Transaction *t,
        RuleWithActions *rule,
        std::vector<const VariableValue *> *l) override {
        Rule_DictElement::id(t, rule, l);
        Rule_DictElement::rev(t, rule, l);
        Rule_DictElement::severity(t, rule, l);
        Rule_DictElement::logData(t, rule, l);
        Rule_DictElement::msg(t, rule, l);
    }
};

// DEFINE_VARIABLE_DICT(Rule, RULE, m_variableRule)


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_RULE_H_
