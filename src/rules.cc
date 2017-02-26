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

#include "modsecurity/rules.h"

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/transaction.h"
#include "src/parser/driver.h"
#include "src/utils/https_client.h"

using modsecurity::Parser::Driver;
using modsecurity::Utils::HttpsClient;

namespace modsecurity {


/**
 * @name    incrementReferenceCount
 * @brief   Increment the number of transactions using this class
 * @ingroup ModSecCore
 *
 * At certain point it is expected to have two differnt
 * groups of rules attached to a specific core, this happens
 * for instance when a webserver is reloading its
 * configurations, the old configurations/rules is available
 * for the old connections and the new rules are available 
 * for the newest connections.
 *
 * @return Number of the current transactions using this rules
 *
 */
void Rules::incrementReferenceCount(void) {
    this->m_referenceCount++;
}


/**
 * @name    decrementReferenceCount
 * @brief   Decrement the number of transactions using this class
 * @ingroup ModSecCore
 *
 * @return Number of the current transactions using this rules
 *
 */
void Rules::decrementReferenceCount(void) {
    this->m_referenceCount--;
    if (this->m_referenceCount == 0) {
        /**
         * FIXME: If there are new rules loaded (new instance
         *        of this class) and the reference is zero, we
         *        can delete this instance freeing some memory.
         *
         */
        /*
         * std::cout << "We may want to delete this rules instance" \
         *   << std::endl;
         */
    }
}


Rules::~Rules() {
    free(unicode_map_table);
    unicode_map_table = NULL;
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
int Rules::loadFromUri(const char *uri) {
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


int Rules::load(const char *file, const std::string &ref) {
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


int Rules::loadRemote(const char *key, const char *uri) {
    HttpsClient client;
    client.setKey(key);
    bool ret = client.download(uri);

    if (ret) {
        return this->load(client.content.c_str(), uri);
    }

    return -1;
}


int Rules::load(const char *plainRules) {
    return this->load(plainRules, "");
}


std::string Rules::getParserError() {
    return this->m_parserError.str();
}


int Rules::evaluate(int phase, Transaction *transaction) {
    if (phase > modsecurity::Phases::NUMBER_OF_PHASES) {
       return 0;
    }

    std::vector<Rule *> rules = m_rules[phase];

    debug(9, "This phase consists of " + std::to_string(rules.size()) + \
        " rule(s).");

    if (transaction->m_allowType == actions::disruptive::FromNowOneAllowType
        && phase != modsecurity::Phases::LoggingPhase) {
        debug(9, "Skipping all rules evaluation on this phase as request " \
            "through the utilization of an `allow' action.");
        return true;
    }
    if (transaction->m_allowType == actions::disruptive::RequestAllowType
        && phase <= modsecurity::Phases::RequestBodyPhase) {
        debug(9, "Skipping all rules evaluation on this phase as request " \
            "through the utilization of an `allow' action.");
        return true;
    }
    if (transaction->m_allowType != actions::disruptive::NoneAllowType) {
        transaction->m_allowType = actions::disruptive::NoneAllowType;
    }

    for (int i = 0; i < rules.size(); i++) {
        Rule *rule = rules[i];
        if (transaction->m_marker.empty() == false) {
            debug(9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' due to a SecMarker: " + transaction->m_marker);
            m_secmarker_skipped++;
            debug(9, "Rule: " + rule->m_marker);
            if (rule->m_secMarker && rule->m_marker == transaction->m_marker) {
                debug(4, "Out of a SecMarker after skip " \
                    + std::to_string(m_secmarker_skipped) + " rules.");
                transaction->m_marker.clear();
                m_secmarker_skipped = 0;
            }
        } else if (transaction->m_skip_next > 0) {
            transaction->m_skip_next--;
            debug(9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' due to a `skip' action. Still " + \
                std::to_string(transaction->m_skip_next) + " to be skipped.");
        } else if (transaction->m_allowType
            != actions::disruptive::NoneAllowType) {
            debug(9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "' as request trough the utilization of an `allow' action.");
        } else if (m_exceptions.contains(rule->m_ruleId)) {
            debug(9, "Skipped rule id '" + std::to_string(rule->m_ruleId) \
                + "'. Removed by an SecRuleRemove directive.");
        } else {
            rule->evaluate(transaction, NULL);
            if (transaction->m_it.disruptive == true) {
                debug(8, "Skipping this phase as this " \
                    "request was already intercepted.");
                break;
            }
        }
    }
    return 1;
}


int Rules::merge(Driver *from) {
    int amount_of_rules = 0;
    amount_of_rules = mergeProperties(
        dynamic_cast<RulesProperties *>(from),
        dynamic_cast<RulesProperties *>(this),
        &m_parserError);

    return amount_of_rules;
}


int Rules::merge(Rules *from) {
    int amount_of_rules = 0;
    amount_of_rules = mergeProperties(
        dynamic_cast<RulesProperties *>(from),
        dynamic_cast<RulesProperties *>(this),
        &m_parserError);

    return amount_of_rules;
}



void Rules::debug(int level, std::string message) {
    if (m_debugLog != NULL) {
        m_debugLog->write(level, message);
    }
}


void Rules::dump() {
    std::cout << "Rules: " << std::endl;
    for (int i = 0; i <= modsecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::vector<Rule *> rules = m_rules[i];
        std::cout << "Phase: " << std::to_string(i);
        std::cout << " (" << std::to_string(rules.size());
        std::cout << " rules)" << std::endl;
        for (int j = 0; j < rules.size(); j++) {
            std::cout << "    Rule ID: " << std::to_string(rules[j]->m_ruleId);
            std::cout << "--" << rules[j] << std::endl;
        }
    }
}


extern "C" Rules *msc_create_rules_set(void) {
    return new Rules();
}


extern "C" void msc_rules_dump(Rules *rules) {
    rules->dump();
}


extern "C" int msc_rules_merge(Rules *rules_dst,
    Rules *rules_from) {
    return rules_dst->merge(rules_from);
}


extern "C" int msc_rules_add_remote(Rules *rules,
    const char *key, const char *uri, const char **error) {
    int ret = rules->loadRemote(key, uri);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_add_file(Rules *rules, const char *file,
    const char **error) {
    int ret = rules->loadFromUri(file);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_add(Rules *rules, const char *plain_rules,
    const char **error) {
    int ret = rules->load(plain_rules);
    if (ret < 0) {
        *error = strdup(rules->getParserError().c_str());
    }
    return ret;
}


extern "C" int msc_rules_cleanup(Rules *rules) {
    delete rules;
    return true;
}


}  // namespace modsecurity

