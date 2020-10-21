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

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "modsecurity/rules_set.h"
#include "src/rule_marker.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/parser/driver.h"
#include "src/utils/https_client.h"
#include "modsecurity/rules.h"

using modsecurity::Parser::Driver;
using modsecurity::Utils::HttpsClient;

namespace modsecurity {

    void Rules::fixDefaultActions(RulesWarnings *warnings, RulesErrors *errors) {
        for (size_t i = 0; i < m_rules.size(); i++) {
            auto &rule = m_rules[i];

            RuleWithActions *r = dynamic_cast<RuleWithActions *>(rule.get());
            if (!r) {
                continue;
            }

            if (dynamic_cast<RuleWithOperator *>(rule.get())) {
                RuleWithOperator *op = new RuleWithOperator(*dynamic_cast<RuleWithOperator *>(rule.get()));
                std::unique_ptr<RuleWithOperator> nrp(op);
                m_rules[i] = std::move(nrp);
            } else if (dynamic_cast<RuleUnconditional *>(rule.get())) {
                RuleUnconditional *un = new RuleUnconditional(*dynamic_cast<RuleUnconditional *>(rule.get()));
                std::unique_ptr<RuleUnconditional> nrp(un);
                m_rules[i] = std::move(nrp);
            } else if (dynamic_cast<RuleScript *>(rule.get())) {
                RuleScript *rs = new RuleScript(*dynamic_cast<RuleScript *>(rule.get()));
                std::unique_ptr<RuleScript> nrp(rs);
                m_rules[i] = std::move(nrp);
            } else {
                RuleWithActions *nr = new RuleWithActions(*dynamic_cast<RuleWithActions *>(rule.get()));
                std::unique_ptr<RuleWithActions> nrp(nr);
                m_rules[i] = std::move(nrp);
            }

            RuleWithActions *nr = dynamic_cast<RuleWithActions *>(m_rules[i].get());
            nr->clearDefaultActions();
            for (auto a : m_defaultActions) {
                nr->addAction(a, 1);
            }
            for (auto a : m_defaultTransformations) {
                nr->addTransformation(a, 1);
            }


        }
    }


/**
 * @name    loadFromUri
 * @brief   load rules from a give uri
 * @ingroup ModSecCore
 *
 * Load the rules from a given uri into memory into memory in
 * the format expected by ModSecurity core.
 *
 * @param uri Full path to the rules file.
 *
 * @return Number of rules loaded, -1 if failed.
 * @retval true  Rules where loaded successfully.
 * @retval false Problem loading the rules.
 *
 */
int RulesSet::loadFromUri(const char *uri) {
    Driver *driver = new Driver();

    if (driver->parseFile(uri) == 0) {
        m_parserError << driver->m_parserError.str();
        delete driver;
        return -1;
    }

    int rules = this->merge(driver);
    delete driver;

    return rules;
}


int RulesSet::load(const char *file, const std::string &ref) {
    Driver *driver = new Driver();

    if (driver->parse(file, ref) == 0) {
        m_parserError << driver->m_parserError.str();
        delete driver;
        return -1;
    }
    int rules = this->merge(driver);
    if (rules == -1) {
        m_parserError << driver->m_parserError.str();
        delete driver;
        return -1;
    }
    delete driver;

    return rules;
}


int RulesSet::loadRemote(const char *key, const char *uri) {
    HttpsClient client;
    client.setKey(key);
    bool ret = client.download(uri);

    if (ret) {
        return this->load(client.content.c_str(), uri);
    }

    return -1;
}


int RulesSet::load(const char *plainRules) {
    return this->load(plainRules, "");
}


bool RulesSet::containsDuplicatedIds(RulesWarnings *warning, RulesErrors *error) {
    std::multimap<RuleId, Rule *> allIds;
    std::set<RuleId> duplicatedIds;
    for (auto &rules : m_rulesSetPhases) {
        for (auto &j : rules) {
            RuleWithActions *rule = dynamic_cast<RuleWithActions *>(j.get());
            if (rule) {
                allIds.insert(std::pair<RuleId, Rule *>(rule->getId(), rule));
            }
        }
    }

    auto id = allIds.begin();
    auto next = id;
    if (id != allIds.end()) {
        next++;
    }
    while (id != allIds.end() && next != allIds.end()) {
        if (id->first == next->first) {
            duplicatedIds.insert(id->first);
        }
        id++;
        next++;
    }

    for (auto i : duplicatedIds) {
        auto ret = allIds.equal_range(i);
        std::stringstream ss;

        ss << "There are multiple rules defined with ";
        ss << "same id. The ID " << i << " is defined at: " << std::endl;
        for (auto it = ret.first; it != ret.second; ++it) {
            auto rule = it->second;
            ss << "  " << *rule->getFileName() << ":";
            ss << rule->getLineNumber() << std::endl;
        }

        std::unique_ptr<std::string> e(new std::string(ss.str()));
        error->push_back(std::move(e));
    }

    return !duplicatedIds.empty();
}



std::string RulesSet::getParserError() {
    return this->m_parserError.str();
}


int RulesSet::evaluate(int phase, Transaction *t) {
    if (phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
       return 0;
    }

    Rules *rules = m_rulesSetPhases[phase];

    ms_dbg_a(t, 9, "This phase consists of " \
        + std::to_string(rules->size()) + " rule(s).");

    if (t->m_allowType == actions::disruptive::FromNowOnAllowType
        && phase != modsecurity::Phases::LoggingPhase) {
        ms_dbg_a(t, 9, "Skipping all rules evaluation on this phase as request " \
            "through the utilization of an `allow' action.");
        return true;
    }
    if (t->m_allowType == actions::disruptive::RequestAllowType
        && phase <= modsecurity::Phases::RequestBodyPhase) {
        ms_dbg_a(t, 9, "Skipping all rules evaluation on this phase as request " \
            "through the utilization of an `allow' action.");
        return true;
    }
    //if (t->m_allowType != actions::disruptive::NoneAllowType) {
    t->m_allowType = actions::disruptive::NoneAllowType;
    //}

    for (int i = 0; i < rules->size(); i++) {
        // FIXME: This is not meant to be here. At the end of this refactoring,
        //        the shared pointer won't be used.
        auto rule = rules->at(i);
        if (t->isInsideAMarker()) {
            RuleMarker *ruleMarker = dynamic_cast<RuleMarker *>(rule.get());
            if (!ruleMarker) {
                ms_dbg_a(t, 9, "Skipped rule id '" + rule->getReference() \
                    + "' due to a SecMarker: " + *t->getCurrentMarker());
            } else {
                rule->evaluate(t);
            }
        } else if (t->m_skip_next > 0) {
            t->m_skip_next--;
            ms_dbg_a(t, 9, "Skipped rule id '" + rule->getReference() \
                + "' due to a `skip' action. Still " + \
                std::to_string(t->m_skip_next) + " to be skipped.");
        } else if (t->m_allowType
            != actions::disruptive::NoneAllowType) {
            ms_dbg_a(t, 9, "Skipped rule id '" + rule->getReference() \
                + "' as request trough the utilization of an `allow' action.");
        } else {
            Rule *base = rule.get();
            RuleWithActions *ruleWithActions = dynamic_cast<RuleWithActions *>(base);
            // FIXME: Those should be treated inside the rule itself
            if (ruleWithActions && m_exceptions.contains(ruleWithActions->getId())) {
                ms_dbg_a(t, 9, "Skipped rule id '" + rule->getReference() \
                    + "'. Removed by an SecRuleRemove directive.");
                continue;
            }
            bool remove_rule = false;
            if (ruleWithActions && m_exceptions.m_remove_rule_by_msg.empty() == false) {
                for (auto &z : m_exceptions.m_remove_rule_by_msg) {
                    if (ruleWithActions->containsMsg(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + ruleWithActions->getReference() \
                            + "'. Removed by a SecRuleRemoveByMsg directive.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }

            if (ruleWithActions && m_exceptions.m_remove_rule_by_tag.empty() == false) {
                for (auto &z : m_exceptions.m_remove_rule_by_tag) {
                    if (ruleWithActions->containsTag(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + ruleWithActions->getReference() \
                            + "'. Removed by a SecRuleRemoveByTag directive.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }


            if (ruleWithActions) {
                for (auto &z : t->m_ruleRemoveByTag) {
                    if (ruleWithActions->containsTag(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + ruleWithActions->getReference() \
                            + "'. Skipped due to a ruleRemoveByTag action.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }

            rule->evaluate(t);
            if (t->m_it.disruptive > 0) {

                ms_dbg_a(t, 8, "Skipping this phase as this " \
                    "request was already intercepted.");
                break;
            }
        }
    }
    return 1;
}


int RulesSet::merge(Driver *from) {
    int amount_of_rules = 0;
    RulesErrors errors;
    RulesWarnings warnings;

    m_rulesSetPhases.append(&from->m_rulesSetPhases);
    mergeProperties(
        dynamic_cast<RulesSetProperties *>(from),
        dynamic_cast<RulesSetProperties *>(this),
        &warnings, &errors);

    m_rulesSetPhases.fixDefaultActions(&warnings, &errors);
    containsDuplicatedIds(&warnings, &errors);

    if (!errors.empty()) {
        for (auto &i : errors) {
            m_parserError << "*** Error: " << *i << std::endl;
        }
        return -1;
    }

    return amount_of_rules;
}


int RulesSet::merge(RulesSet *from) {
    int amount_of_rules = 0;
    RulesErrors errors;
    RulesWarnings warnings;

    m_rulesSetPhases.append(&from->m_rulesSetPhases);
    mergeProperties(
        dynamic_cast<RulesSetProperties *>(from),
        dynamic_cast<RulesSetProperties *>(this),
        &warnings, &errors);

    m_rulesSetPhases.fixDefaultActions(&warnings, &errors);
    containsDuplicatedIds(&warnings, &errors);

    if (!errors.empty()) {
        for (auto &i : errors) {
            m_parserError << "*** Error: " << *i << std::endl;
        }
        return -1;
    }

    return amount_of_rules;
}


void RulesSet::debug(int level, const std::string &id,
    const std::string &uri, const std::string &msg) {
    if (m_debugLog != NULL) {
        m_debugLog->write(level, id, uri, msg);
    }
}


void RulesSet::dump() {
    m_rulesSetPhases.dump();
}


extern "C" RulesSet *msc_create_rules_set(void) {
    return new RulesSet();
}


extern "C" void msc_rules_dump(RulesSet *rules) {
    rules->dump();
}


extern "C" int msc_rules_merge(RulesSet *rules_dst,
    RulesSet *rules_from, const char **error) {
    int ret = rules_dst->merge(rules_from);
    if (ret < 0) {
        *error = strdup(rules_dst->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_add_remote(RulesSet *rules,
    const char *key, const char *uri, const char **error) {
    int ret = rules->loadRemote(key, uri);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_add_file(RulesSet *rules, const char *file,
    const char **error) {
    int ret = rules->loadFromUri(file);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_add(RulesSet *rules, const char *plain_rules,
    const char **error) {
    int ret = rules->load(plain_rules);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_cleanup(RulesSet *rules) {
    delete rules;
    return true;
}


}  // namespace modsecurity

