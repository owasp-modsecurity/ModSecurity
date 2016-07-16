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

#include "actions/action.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"

#include "actions/block.h"
#include "actions/chain.h"
#include "actions/deny.h"
#include "actions/redirect.h"
#include "actions/status.h"
#include "actions/rule_id.h"
#include "actions/phase.h"
#include "actions/severity.h"
#include "actions/capture.h"
#include "actions/pass.h"
#include "actions/log.h"



#define IF_MATCH(a) \
    if (op.compare(1, std::strlen(#a), #a) == 0)

namespace modsecurity {
namespace actions {


std::string Action::evaluate(std::string value,
    Transaction *transaction) {
    return value;
}


bool Action::evaluate(Rule *rule, Transaction *transaction) {
    return true;
}


void Action::fillIntervention(ModSecurityIntervention *i) {
}

Action *Action::instantiate(const std::string& name) {
    std::string status("status:");
    std::string redirect("redirect:");
    std::string block("block");
    std::string phase("phase:");
    std::string rule_id("id:");

    if (name.compare(0, status.length(), status) == 0) {
        return new Status(name);
    }
    if (name.compare(0, redirect.length(), redirect) == 0) {
        return new Redirect(name);
    }
    if (name.compare(0, block.length(), block) == 0) {
        return new Block(name);
    }
    if (name.compare(0, phase.length(), phase) == 0) {
        return new Phase(name);
    }
    if (name.compare(0, rule_id.length(), rule_id) == 0) {
        return new RuleId(name);
    }
    if (name == "chain") {
        return new Chain(name);
    }
    if (name == "capture") {
        return new Capture(name);
    }
    if (name == "pass") {
        return new Pass(name);
    }
    if (name == "deny") {
        return new Deny(name);
    }
    if (name == "log") {
        return new Log(name);
    }
    return new Action(name);
}

}  // namespace actions
}  // namespace modsecurity
