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

#include <unistd.h>
#include <stdio.h>
#include <string.h>


#include <modsecurity/modsecurity.h>
#include <modsecurity/rules.h>
#include <modsecurity/rule_message.h>
#include <modsecurity/parser/driver_trail.h>
#include <modsecurity/parser/driver.h>

#include <string>
#include <memory>


class CustomDriverTrail : public modsecurity::Parser::DriverTrail {
 public:
    int addSecRule(Rule *rule) { 
        std::cout << "Adding: " << std::to_string(rule->m_ruleId) << std::endl;
        return true;
    };
    int addSecAction(Rule *rule) { return 0; };
    int addSecMarker(std::string marker) { return 0; };
    int addSecRuleScript(Rule *rule) { return 0; };

    Rule *m_lastRule;
};


int main(int argc, char **argv) {
    modsecurity::ModSecurity *modsec;
    //modsecurity::Parser::Driver *driver = new modsecurity::Parser::Driver(new CustomDriverTrail());
    //if (argc < 2) {
        //std::cout << "Use " << *argv << " test-case-file.conf";
        //std::cout << std::endl << std::endl;
        //return -1;
    //}
    //*(argv++);

    std::string rules_arg(*argv);

    /**
     * ModSecurity initial setup
     *
     */
    modsec = new modsecurity::ModSecurity();
    modsec->setConnectorInformation("ModSecurity-test v0.0.1-alpha" \
        " (ModSecurity test)");

    /**
     * loading the rules....
     *
     */


    //if (driver->parseFile(rules_arg.c_str()) < 0) {
        //std::cout << "Problems loading the rules..." << std::endl;
        //return -1;
    //}

    //delete driver;
    delete modsec;
}


