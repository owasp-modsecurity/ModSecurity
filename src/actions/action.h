/**
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

#include <string>

#include "modsecurity/intervention.h"

#ifndef SRC_ACTIONS_ACTION_H_
#define SRC_ACTIONS_ACTION_H_

#ifdef __cplusplus

namespace ModSecurity {
class Assay;
class Rule;

namespace actions {


class Action {
 public:
    explicit Action(std::string _action)
        : action_kind(2),
        action(_action) { }

    /**
     *
     * Define the action kind regarding to the execution time.
     * 
     * 
     */
    enum Kind {
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

    std::string action;
    int action_kind;
    std::string name;

    virtual std::string& evaluate(std::string exp,
        Assay *assay);
    virtual bool evaluate(Assay *assay);
    virtual bool evaluate(Rule *rule);

    static Action *instantiate(std::string action);

    virtual void fill_intervention(ModSecurityIntervention *intervention);
};


}  // namespace actions
}  // namespace ModSecurity

#endif

#endif  // SRC_ACTIONS_ACTION_H_
