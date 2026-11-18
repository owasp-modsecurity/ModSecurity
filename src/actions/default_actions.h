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
#include <string>
#include <vector>

#include "modsecurity/actions/action.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#ifndef SRC_ACTIONS_DEFAULT_ACTIONS_H_
#define SRC_ACTIONS_DEFAULT_ACTIONS_H_

#ifdef __cplusplus

namespace modsecurity {
namespace actions {


/**
 * The SecDefaultAction phases of one parsed configuration unit (one
 * Parser::Driver, i.e. one rules file or one block of rules handed to the
 * library). Shared by every rule of that unit.
 */
class DefaultActionsScope {
 public:
    /** The unit has a SecDefaultAction for the phase. */
    std::array<bool, modsecurity::Phases::NUMBER_OF_PHASES> m_defined{};
};


/**
 * The default actions a rule takes. The parser puts one at the front of the
 * action list of every rule it builds: a SecDefaultAction applies only to
 * the rules of its phase defined after it (#3766).
 *
 * In a unit that has a SecDefaultAction for the rule's phase, the rule takes
 * the list that was in force when it was defined, which is empty for a rule
 * defined before it. In a unit without one, the rule takes the defaults of
 * the rule set at run time, as before: those of the other rules files loaded
 * into the same rule set, and those a connector merges in from an enclosing
 * configuration context.
 *
 * The action itself does nothing; RuleWithActions finds it at the front of
 * its run-time actions and skips it there.
 */
class DefaultActions : public Action {
 public:
    using List = std::vector<std::shared_ptr<Action>>;

    DefaultActions(std::shared_ptr<DefaultActionsScope> scope,
        const List (&inForce)[modsecurity::Phases::NUMBER_OF_PHASES])
        : Action("DefaultActions", Kind::RunTimeOnlyIfMatchKind),
        m_scope(std::move(scope)),
        m_actions() {
        for (int i = 0; i < modsecurity::Phases::NUMBER_OF_PHASES; i++) {
            m_actions[i] = inForce[i];
        }
    }

    const List &forPhase(const Transaction *trans, int phase) const {
        if (m_scope->m_defined[phase]) {
            return m_actions[phase];
        }
        return trans->m_rules->m_defaultActions[phase];
    }

 private:
    std::shared_ptr<DefaultActionsScope> m_scope;
    std::array<List, modsecurity::Phases::NUMBER_OF_PHASES> m_actions;
};


}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // SRC_ACTIONS_DEFAULT_ACTIONS_H_
