/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2025 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef TEST_BENCHMARK_MODSEC_FIXTURE_H_
#define TEST_BENCHMARK_MODSEC_FIXTURE_H_

#include <string>
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

// Helper class to manage ModSecurity lifecycle
class ModSecFixture {
 public:
    modsecurity::ModSecurity* modsec;
    modsecurity::RulesSet* rules;

    ModSecFixture() {
        modsec = new modsecurity::ModSecurity();
        modsec->setConnectorInformation("ModSecurity-benchmark v1.0");
        rules = new modsecurity::RulesSet();
    }

    ~ModSecFixture() {
        delete rules;
        delete modsec;
    }

    // Delete copy and move operations since we manage raw pointers
    ModSecFixture(const ModSecFixture&) = delete;
    ModSecFixture& operator=(const ModSecFixture&) = delete;
    ModSecFixture(ModSecFixture&&) = delete;
    ModSecFixture& operator=(ModSecFixture&&) = delete;

    bool loadRules(const std::string& ruleConfig) {
        // Create a temporary rules object for this configuration
        modsecurity::RulesSet* tempRules = new modsecurity::RulesSet();
        int result = tempRules->load(ruleConfig.c_str());

        if (result < 0) {
            delete tempRules;
            return false;
        }

        // Replace old rules
        delete rules;
        rules = tempRules;
        return true;
    }

    modsecurity::Transaction* createTransaction() {
        return new modsecurity::Transaction(modsec, rules, nullptr);
    }
};

#endif  // TEST_BENCHMARK_MODSEC_FIXTURE_H_
