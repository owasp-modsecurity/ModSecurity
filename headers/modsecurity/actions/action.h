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

#ifdef __cplusplus

#include <string>
#include <iostream>
#include <memory>

#endif

#include "modsecurity/intervention.h"
#include "modsecurity/rule.h"
#include "modsecurity/modsec_stack_allocator.h"

#ifndef HEADERS_MODSECURITY_ACTIONS_ACTION_H_
#define HEADERS_MODSECURITY_ACTIONS_ACTION_H_

#ifdef __cplusplus

namespace modsecurity {
class Transaction;
class RuleWithActions;


namespace actions {

/**
 *
 * Define the action kind regarding to the execution time.
 *
 *
 */
enum ActionKind {
/**
 *
 * Action that are executed while loading the configuration. For instance
 * the rule ID or the rule phase.
 *
 */
 ConfigurationKind,
/**
 *
 * Those are actions that demands to be executed before call the operator.
 * For instance the tranformations.
 *
 *
 */
 RunTimeBeforeMatchAttemptKind,
/**
 *
 * Actions that are executed after the execution of the operator, only if
 * the operator returned Match (or True). For instance the disruptive
 * actions.
 *
 */
 RunTimeOnlyIfMatchKind,
};

class Action {
 public:
    Action(const std::string& action,
        ActionKind kind = RunTimeOnlyIfMatchKind)
        : m_actionKind(kind),
        m_name(sort_name(action)),
        m_rule(nullptr),
        m_parserPayload(sort_payload(action))
    { };

    virtual ~Action() { }

    virtual bool init(std::string *error) { return true; }
    virtual void ruleInit(RuleWithActions *r) {
        m_rule = r;
    }

    virtual bool execute(Transaction *transaction) noexcept { return true; };
    inline virtual bool executeAsDefaulAction(Transaction *transaction, RuleWithActions *r) noexcept {
        return execute(transaction);
    };

    virtual bool isDisruptive() { return false; }
    virtual bool isAllowedInSecDefaultActions() { return false; }

    const ActionKind getActionKind() {
        return m_actionKind;
    }

    const std::string *getName() {
        return &m_name;
    }

 protected:
    const RuleWithActions *m_rule;
    const std::string m_parserPayload;


 private:
    const ActionKind m_actionKind;
    const std::string m_name;


    static size_t get_payload_pos(const std::string& data) {
        size_t pos = data.find(":");
        std::string t = "t:";

        if (data.compare(0, t.length(), t) == 0) {
            pos = data.find(":", 2);
        }

        return pos;
    }

    static std::string sort_name(const std::string& data) {
        size_t pos = get_payload_pos(data);
        if (pos == std::string::npos) {
            return data;
        }

        std::string ret(data, 0, pos);
        return ret;
    }

    static std::string sort_payload(const std::string& data) {
        size_t pos = get_payload_pos(data);
        std::string ret("");
        if (pos != std::string::npos) {
            ret = std::string(data, pos + 1, data.length());

            if (ret.at(0) == '\'' && ret.size() > 2) {
                ret.erase(0, 1);
                ret.pop_back();
            }
        }

        return ret;
    }
};


}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // HEADERS_MODSECURITY_ACTIONS_ACTION_H_
