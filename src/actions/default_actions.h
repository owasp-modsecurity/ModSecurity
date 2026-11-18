/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2026 OWASP (https://owasp.org)
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

#include <array>
#include <memory>
#include <utility>
#include <vector>

#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"

#ifndef SRC_ACTIONS_DEFAULT_ACTIONS_H_
#define SRC_ACTIONS_DEFAULT_ACTIONS_H_

#ifdef __cplusplus

namespace modsecurity {
namespace actions {


/**
 * The SecDefaultAction lists of one top-level parser load: one
 * Parser::Driver, i.e. one rules file or one block of rules handed to the
 * library, together with every file it pulls in with Include. Shared by every
 * rule of that load.
 */
class DefaultActionsScope {
 public:
    using List = std::vector<std::shared_ptr<Action>>;

    /** The load has a SecDefaultAction for the phase. */
    std::array<bool, modsecurity::Phases::NUMBER_OF_PHASES> m_defined{};
    /** Its actions; a load can have only one per phase. */
    std::array<List, modsecurity::Phases::NUMBER_OF_PHASES> m_actions{};
};


/**
 * The default actions a rule takes: a SecDefaultAction applies only to the
 * rules of its phase defined after it (#3766). Parser::Driver attaches one to
 * every rule it adds, except chained members, which take the defaults of
 * their chain starter.
 *
 * In a load that has a SecDefaultAction for the rule's phase, the rule takes
 * it if it was defined after it, and nothing if it was defined before it. In
 * a load without one, the rule takes the defaults of the rule set at run
 * time, as before: those of the other loads into the same rule set, and those
 * a connector merges in from an enclosing configuration context.
 */
class DefaultActions {
 public:
    using List = DefaultActionsScope::List;

    explicit DefaultActions(std::shared_ptr<const DefaultActionsScope> scope)
        : m_scope(std::move(scope)),
        m_definedBefore(m_scope->m_defined) { }

    /**
     * The list of the phase, or nullptr when the load has no SecDefaultAction
     * for it and the rule takes the defaults of the rule set.
     */
    const List *forPhase(int phase) const {
        static const List none;
        if (!m_scope->m_defined[phase]) {
            return nullptr;
        }
        return m_definedBefore[phase] ? &m_scope->m_actions[phase] : &none;
    }

 private:
    std::shared_ptr<const DefaultActionsScope> m_scope;
    /** The phases whose SecDefaultAction came before the rule. */
    std::array<bool, modsecurity::Phases::NUMBER_OF_PHASES> m_definedBefore;
};


}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // SRC_ACTIONS_DEFAULT_ACTIONS_H_
