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

#include "modsecurity/rules_set.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/parser/driver.h"
#include "src/utils/https_client.h"
#include "modsecurity/rules.h"

using modsecurity::Parser::Driver;
using modsecurity::Utils::HttpsClient;

namespace modsecurity {


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

    if (driver->parseFile(uri) == false) {
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

    if (driver->parse(file, ref) == false) {
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


std::string RulesSet::getParserError() {
    return this->m_parserError.str();
}


int RulesSet::evaluate(int phase, Transaction *t) {
    if (phase >= modsecurity::Phases::NUMBER_OF_PHASES) {
       return 0;
    }

    std::vector<Rule *> *rules = m_rulesSetPhases[phase];

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
        Rule *rule = rules->at(i);
        if (t->m_marker.empty() == false) {
            ms_dbg_a(t, 9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' due to a SecMarker: " + t->m_marker);
#ifndef NO_LOGS
            m_secmarker_skipped++;
#endif
            ms_dbg_a(t, 9, "Rule: " + rule->m_marker);

            if (rule->m_secMarker && rule->m_marker == t->m_marker) {
                ms_dbg_a(t, 4, "Out of a SecMarker after skip " \
                    + std::to_string(m_secmarker_skipped) + " rules.");
                t->m_marker.clear();
#ifndef NO_LOGS
                m_secmarker_skipped = 0;
#endif
            }
        } else if (t->m_skip_next > 0) {
            t->m_skip_next--;
            ms_dbg_a(t, 9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' due to a `skip' action. Still " + \
                std::to_string(t->m_skip_next) + " to be skipped.");
        } else if (t->m_allowType
            != actions::disruptive::NoneAllowType) {
            ms_dbg_a(t, 9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' as request trough the utilization of an `allow' action.");
        } else if (m_exceptions.contains(rule->m_ruleId)) {
            ms_dbg_a(t, 9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "'. Removed by an SecRuleRemove directive.");
        } else {
            bool remove_rule = false;
            if (m_exceptions.m_remove_rule_by_msg.empty() == false) {
                for (auto &z : m_exceptions.m_remove_rule_by_msg) {
                    if (rule->containsMsg(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + std::to_string(rule->m_ruleId) \
                            + "'. Removed by a SecRuleRemoveByMsg directive.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }

            if (m_exceptions.m_remove_rule_by_tag.empty() == false) {
                for (auto &z : m_exceptions.m_remove_rule_by_tag) {
                    if (rule->containsTag(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + std::to_string(rule->m_ruleId) \
                            + "'. Removed by a SecRuleRemoveByTag directive.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }

            if (t->m_ruleRemoveByTag.empty() == false) {
                for (auto &z : t->m_ruleRemoveByTag) {
                    if (rule->containsTag(z, t) == true) {
                        ms_dbg_a(t, 9, "Skipped rule id '" \
                            + std::to_string(rule->m_ruleId) \
                            + "'. Skipped due to a ruleRemoveByTag action.");
                        remove_rule = true;
                        break;
                    }
                }
                if (remove_rule) {
                    continue;
                }
            }

            rule->evaluate(t, NULL);
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

    amount_of_rules = m_rulesSetPhases.append(&from->m_rulesSetPhases,
        &m_parserError);
    mergeProperties(
        dynamic_cast<RulesSetProperties *>(from),
        dynamic_cast<RulesSetProperties *>(this),
        &m_parserError);

    return amount_of_rules;
}


int RulesSet::merge(RulesSet *from) {
    int amount_of_rules = 0;

    amount_of_rules = m_rulesSetPhases.append(&from->m_rulesSetPhases,
        &m_parserError);
    mergeProperties(
        dynamic_cast<RulesSetProperties *>(from),
        dynamic_cast<RulesSetProperties *>(this),
        &m_parserError);

    return amount_of_rules;
}


void RulesSet::debug(int level, const std::string &id,
    const std::string &uri, const std::string &msg) {
    if (m_debugLog != NULL) {
        m_debugLog->write(level, id, uri, msg);
    }
}


void RulesSet::dump() const {
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

