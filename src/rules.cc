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

#include "modsecurity/rules.h"

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"
#include "src/utils.h"
#include "parser/driver.h"

namespace ModSecurity {


/**
 * @name    incrementReferenceCount
 * @brief   Increment the number of assays using this class
 * @ingroup ModSecCore
 *
 * At certain point it is expected to have two differnt
 * groups of rules attached to a specific core, this happens
 * for instance when a webserver is reloading its
 * configurations, the old configurations/rules is available
 * for the old connections and the new rules are available 
 * for the newest connections.
 *
 * @return Number of the current assays using this rules
 *
 */
void Rules::incrementReferenceCount(void) {
    this->m_referenceCount++;
}

/**
 * @name    decrementReferenceCount
 * @brief   Decrement the number of assays using this class
 * @ingroup ModSecCore
 *
 * @return Number of the current assays using this rules
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
 * @return If rules were loaded successfully or not.
 * @retval true  Rules where loaded successfully.
 * @retval false Problem loading the rules.
 *
 */
int Rules::loadFromUri(char *uri) {
    std::cout << "Loading rules from: " << uri << std::endl;

    Driver *driver = new Driver();
    driver->parse(uri);
    this->merge(driver);
    delete driver;

    return true;
}


int Rules::loadRemote(char *key, char *uri) {
    return true;
}


int Rules::load(const char *plain_rules) {
    bool ret = true;
    /**
     * @todo rgg. we should make the parser work out of the buffer.
     *
     */
    std::ofstream myfile;
    myfile.open("/tmp/modsec_ugly_hack.txt");
    myfile << plain_rules;
    myfile.close();

    Driver *driver = new Driver();
    if (driver->parse("/tmp/modsec_ugly_hack.txt")) {
        ret = false;
    }
    this->merge(driver);
    delete driver;

    return ret;
}


int Rules::evaluate(int phase, Assay *assay) {
    if (phase > ModSecurity::Phases::NUMBER_OF_PHASES) {
       return 0;
    }

    std::vector<Rule *> rules = this->rules[phase];

    debug(9, "This phase consists of " + std::to_string(rules.size()) + \
        " rule(s).");

    for (int i = 0; i < rules.size(); i++) {
        Rule *rule = rules[i];
        rule->evaluate(assay);
    }
    return 1;
}


int Rules::merge(Driver *from) {
    for (int i = 0; i < ModSecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::vector<Rule *> rules = from->rules[i];
        for (int j = 0; j < rules.size(); j++) {
            Rule *rule = rules[j];
            this->rules[i].push_back(rule);
        }
    }

    this->secRuleEngine = from->secRuleEngine;
    this->sec_audit_type = from->sec_audit_type;
    this->sec_audit_engine = from->sec_audit_engine;
    this->sec_request_body_access = from->sec_request_body_access;
    this->sec_response_body_access = from->sec_response_body_access;
    this->debug_log_path = from->debug_log_path;
    this->debug_level = from->debug_level;
    this->components = from->components;

    if (m_custom_debug_log) {
        this->debug_log = m_custom_debug_log->new_instance();
    } else {
        this->debug_log = new DebugLog();
    }

    this->audit_log = from->audit_log;

    this->debug_log->setDebugLevel(this->debug_level);
    this->debug_log->setOutputFile(this->debug_log_path);

    return 0;
}

void Rules::debug(int level, std::string message) {
    this->debug_log->write_log(level, message);
}

int Rules::merge(Rules *from) {
    for (int i = 0; i < ModSecurity::Phases::NUMBER_OF_PHASES; i++) {
        std::vector<Rule *> rules = from->rules[i];
        for (int j = 0; j < rules.size(); j++) {
            Rule *rule = rules[j];
            this->rules[i].push_back(rule);
        }
    }

    this->secRuleEngine = from->secRuleEngine;
    this->sec_audit_type = from->sec_audit_type;
    this->sec_audit_engine = from->sec_audit_engine;
    this->sec_request_body_access = from->sec_request_body_access;
    this->sec_response_body_access = from->sec_response_body_access;
    this->components = from->components;

    this->debug_log = from->debug_log;

    /*
    if (from->debug_log->isConfigured())
    {
        if (this->debug_log_path.compare(from->debug_log_path) != 0)
        {
            this->debug_log = new DebugLog();
            this->debug_log->setDebugLevel(from->debug_level);
            this->debug_log->setOutputFile(this->debug_log_path);
        }
        if (this->debug_level != from->debug_level)
        {
            this->debug_log->setDebugLevel(this->debug_log);
        }
    }
    else
    {
    }
    */

    return 0;
}


extern "C" Rules *msc_create_rules_set() {
    Rules *rules = new Rules();

    return rules;
}


extern "C" int msc_rules_merge(Rules *rules_dst,
    Rules *rules_from) {
    rules_dst->merge(rules_from);

    return 0;
}


extern "C" int msc_rules_add_remote(Rules *rules,
    char *key, char *uri) {
    rules->loadRemote(key, uri);

    return 0;
}


extern "C" int msc_rules_add_file(Rules *rules, char *file) {
    rules->loadFromUri(file);

    return 0;
}


extern "C" int msc_rules_add(Rules *rules, const char *plain_rules) {
    rules->load(plain_rules);

    return 0;
}

}  // namespace ModSecurity

