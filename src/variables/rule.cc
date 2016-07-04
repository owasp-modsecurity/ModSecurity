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

#include "variables/rule.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "modsecurity/transaction.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/rules.h"

#include "src/request_body_processor/xml.h"
#include "src/actions/action.h"
#include "src/actions/severity.h"
#include "src/actions/xmlns.h"
#include "src/actions/log_data.h"
#include "src/actions/msg.h"

namespace modsecurity {
namespace Variables {

void Rule::evaluateInternal(Transaction *t,
    modsecurity::Rule *rule,
    std::vector<const collection::Variable *> *l) {
    std::map<std::string, std::string> envs;

    // id
    envs.insert(std::pair<std::string, std::string>("RULE:id",
        std::to_string(rule->rule_id)));    

    // rev
    envs.insert(std::pair<std::string, std::string>("RULE:rev",
        rule->m_rev));

    // severity
    std::vector<actions::Action *> acts = rule->getActionsByName("severity");
    for (actions::Action *i : acts) {
        actions::Severity *a = reinterpret_cast<actions::Severity *>(i);
        if (a) {
            envs.insert(std::pair<std::string, std::string>("RULE:severity",
                std::to_string(a->m_severity)));
        }
    }

    // logdata
    acts = rule->getActionsByName("logdata");
    for (actions::Action *i : acts) {
        actions::LogData *a = reinterpret_cast<actions::LogData *>(i);
        if (a) {
            envs.insert(std::pair<std::string, std::string>("RULE:logdata",
                a->data(t)));
        }
    }

    // msg
    acts = rule->getActionsByName("msg");
    for (actions::Action *i : acts) {
        actions::Msg *a = reinterpret_cast<actions::Msg *>(i);
        if (a) {
            envs.insert(std::pair<std::string, std::string>("RULE:msg",
                a->data(t)));
        }
    }

    for (auto& x : envs) {
        if ((x.first.substr(0, m_name.size() + 1).compare(m_name + ":") != 0)
            && (x.first != m_name)) {
            continue;
        }
        l->push_back(new collection::Variable(x.first, x.second));
    }
}

}  // namespace Variables
}  // namespace modsecurity
