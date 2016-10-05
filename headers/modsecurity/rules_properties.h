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


#ifdef __cplusplus
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#endif


#ifndef HEADERS_MODSECURITY_RULES_PROPERTIES_H_
#define HEADERS_MODSECURITY_RULES_PROPERTIES_H_


#include "modsecurity/modsecurity.h"
#include "modsecurity/rule.h"
#include "modsecurity/rules_exceptions.h"


#ifdef __cplusplus

namespace modsecurity {
class RulesExceptions;
namespace audit_log {
class AuditLog;
}
namespace actions {
class Action;
}
namespace Parser {
class Driver;
}


/** @ingroup ModSecurity_CPP_API */
class ConfigInt {
 public:
    ConfigInt() : m_set(false), m_value(0) { }
    bool m_set;
    int m_value;
};


class ConfigDouble {
 public:
    ConfigDouble() : m_set(false), m_value(0) { }
    bool m_set;
    double m_value;
};


class ConfigString {
 public:
    ConfigString() : m_set(false), m_value("") { }
    bool m_set;
    std::string m_value;
};


class RulesProperties {
 public:
    RulesProperties() : m_auditLog(NULL),
        m_debugLog(new DebugLog()),
        m_httpblKey(""),
        m_remoteRulesActionOnFailed(PropertyNotSetRemoteRulesAction),
        m_secRequestBodyAccess(PropertyNotSetConfigBoolean),
        m_secResponseBodyAccess(PropertyNotSetConfigBoolean),
        m_secXMLExternalEntity(PropertyNotSetConfigBoolean),
        m_requestBodyLimitAction(PropertyNotSetBodyLimitAction),
        m_responseBodyLimitAction(PropertyNotSetBodyLimitAction),
        m_secRuleEngine(PropertyNotSetRuleEngine),
        m_uploadKeepFiles(PropertyNotSetConfigBoolean),
        m_tmpSaveUploadedFiles(PropertyNotSetConfigBoolean) { }


    explicit RulesProperties(DebugLog *debugLog) : m_auditLog(NULL),
        m_debugLog(debugLog),
        m_httpblKey(""),
        m_remoteRulesActionOnFailed(PropertyNotSetRemoteRulesAction),
        m_secRequestBodyAccess(PropertyNotSetConfigBoolean),
        m_secResponseBodyAccess(PropertyNotSetConfigBoolean),
        m_secXMLExternalEntity(PropertyNotSetConfigBoolean),
        m_requestBodyLimitAction(PropertyNotSetBodyLimitAction),
        m_responseBodyLimitAction(PropertyNotSetBodyLimitAction),
        m_secRuleEngine(PropertyNotSetRuleEngine),
        m_uploadKeepFiles(PropertyNotSetConfigBoolean),
        m_tmpSaveUploadedFiles(PropertyNotSetConfigBoolean) { }


    ~RulesProperties() {
        delete m_debugLog;
    }


    /**
     *
     *
     */
    enum ConfigBoolean {
        TrueConfigBoolean,
        FalseConfigBoolean,
        PropertyNotSetConfigBoolean
    };


    /**
     *
     * The RuleEngine enumerator consists in mapping the different states
     * of the rule engine.
     *
     */
    enum RuleEngine {
        /**
         *
         * Rules won't be evaluated if Rule Engine is set to DisabledRuleEngine
         *
         */
        DisabledRuleEngine,
        /**
         *
         * Rules will be evaluated and disturb actions will take place if needed.
         *
         */
        EnabledRuleEngine,
        /**
         * Rules will be evaluated but it won't generate any disruptive action.
         *
         */
        DetectionOnlyRuleEngine,
        /**
         *
         */
        PropertyNotSetRuleEngine
    };


    /**
     *
     * Defines what actions should be taken in case the body (response or
     * request) is bigger than the expected size.
     *
     */
    enum BodyLimitAction {
     /**
      *
      * Process partial
      *
      */
     ProcessPartialBodyLimitAction,
     /**
      *
      * Reject the request
      *
      */
     RejectBodyLimitAction,
     /**
      *
      */
     PropertyNotSetBodyLimitAction
    };


    /**
     *
     * Defines what actions should be taken in case the remote rules failed to
     * be downloaded (independent of the circumstances)
     *
     *
     */
    enum OnFailedRemoteRulesAction {
     /**
      *
      * Abort
      *
      */
     AbortOnFailedRemoteRulesAction,
     /**
      *
      * Warn on logging
      *
      */
     WarnOnFailedRemoteRulesAction,
     /**
      *
      */
     PropertyNotSetRemoteRulesAction
    };


    static const char *ruleEngineStateString(RuleEngine i) {
        switch (i) {
        case DisabledRuleEngine:
            return "Disabled";
        case EnabledRuleEngine:
            return "Enabled";
        case DetectionOnlyRuleEngine:
            return "DetectionOnly";
        case PropertyNotSetRuleEngine:
            return "PropertyNotSet/DetectionOnly";
        }
        return NULL;
    }


    static std::string configBooleanString(ConfigBoolean i) {
        switch (i) {
        case TrueConfigBoolean:
            return "True";
        case FalseConfigBoolean:
            return "False";
        case PropertyNotSetConfigBoolean:
            return "Not set";
        }
        return NULL;
    }


    static int mergeProperties(RulesProperties *from, RulesProperties *to,
                               std::ostringstream *err) {
        int amount_of_rules = 0;

        amount_of_rules = appendRules(from->rules, to->rules, err);
        if (amount_of_rules < 0) {
            return amount_of_rules;
        }

        if (from->m_secRuleEngine != PropertyNotSetRuleEngine) {
            to->m_secRuleEngine = from->m_secRuleEngine;
        }

        if (from->m_secRequestBodyAccess != PropertyNotSetConfigBoolean) {
            to->m_secRequestBodyAccess = from->m_secRequestBodyAccess;
        }

        if (from->m_secResponseBodyAccess != PropertyNotSetConfigBoolean) {
            to->m_secResponseBodyAccess = from->m_secResponseBodyAccess;
        }

        if (from->m_secXMLExternalEntity != PropertyNotSetConfigBoolean) {
            to->m_secXMLExternalEntity = from->m_secXMLExternalEntity;
        }

        if (from->m_uploadKeepFiles != PropertyNotSetConfigBoolean) {
            to->m_uploadKeepFiles = from->m_uploadKeepFiles;
        }

        if (from->m_tmpSaveUploadedFiles != PropertyNotSetConfigBoolean) {
            to->m_tmpSaveUploadedFiles = from->m_tmpSaveUploadedFiles;
        }

        to->m_components = from->m_components;

        if (from->m_requestBodyLimit.m_set == true) {
            to->m_requestBodyLimit.m_value = from->m_requestBodyLimit.m_value;
        }

        if (from->m_responseBodyLimit.m_set == true) {
            to->m_responseBodyLimit.m_value = from->m_responseBodyLimit.m_value;
        }

        if (from->m_requestBodyLimitAction != PropertyNotSetBodyLimitAction) {
            to->m_requestBodyLimitAction = from->m_requestBodyLimitAction;
        }

        if (from->m_responseBodyLimitAction != PropertyNotSetBodyLimitAction) {
            to->m_responseBodyLimitAction = from->m_responseBodyLimitAction;
        }


        if (from->m_uploadFileLimit.m_set == true) {
            to->m_uploadFileLimit.m_value = from->m_uploadFileLimit.m_value;
        }

        if (from->m_uploadFileMode.m_set == true) {
            to->m_uploadFileMode.m_value = from->m_uploadFileMode.m_value;
        }

        to->m_uploadDirectory = from->m_uploadDirectory;

        to->m_exceptions = from->m_exceptions;

        for (std::set<std::string>::iterator
                it = from->m_responseBodyTypeToBeInspected.begin();
                it != from->m_responseBodyTypeToBeInspected.end(); ++it) {
            to->m_responseBodyTypeToBeInspected.insert(*it);
        }

        /*
         *
         * default Actions is something per configuration context, there is
         * need to merge anything.
         *
         */
        for (int i = 0; i <= 8; i++) {
            std::vector<actions::Action *> actions = from->defaultActions[i];
            to->defaultActions[i].clear();
            for (int j = 0; j < actions.size(); j++) {
                actions::Action *action = actions[j];
                to->defaultActions[i].push_back(action);
            }
        }

        if (from->m_debugLog && to->m_debugLog &&
                from->m_debugLog->isLogFileSet()) {
            to->m_debugLog->setDebugLogFile(
                from->m_debugLog->getDebugLogFile());
        }

        if (from->m_debugLog && to->m_debugLog &&
                from->m_debugLog->isLogLevelSet()) {
            to->m_debugLog->setDebugLogLevel(
                from->m_debugLog->getDebugLogLevel());
        }

        return amount_of_rules;
    }


    static int appendRules(std::vector<modsecurity::Rule *> from[8],
        std::vector<modsecurity::Rule *> to[8],
        std::ostringstream *err) {
        int amount_of_rules = 0;
        for (int i = 0; i <= 8; i++) {
            std::vector<modsecurity::Rule *> rules_to = to[i];
            std::vector<modsecurity::Rule *> rules_from = from[i];

            for (int j = 0; j < rules_from.size(); j++) {
                Rule *rule = rules_from[j];
                for (int z = 0; z < rules_to.size(); z++) {
                    Rule *rule_ckc = rules_to[z];
                    if (rule_ckc->rule_id == rule->rule_id) {
                        if (err != NULL) {
                            *err << "Rule id: " \
                                 << std::to_string(rule->rule_id) \
                                 << " is duplicated" << std::endl;
                        }
                        return -1;
                    }
                }
                amount_of_rules++;
                rules_to.push_back(rule);
                rule->refCountIncrease();
            }
        }
        return amount_of_rules;
    }


    std::vector<modsecurity::Rule *> *getRulesForPhase(int phase) {
        if (phase > 7) {
            return NULL;
        }
        return &rules[phase];
    }


    audit_log::AuditLog *m_auditLog;
    BodyLimitAction m_requestBodyLimitAction;
    BodyLimitAction m_responseBodyLimitAction;
    ConfigBoolean m_secRequestBodyAccess;
    ConfigBoolean m_secResponseBodyAccess;
    ConfigBoolean m_secXMLExternalEntity;
    ConfigBoolean m_tmpSaveUploadedFiles;
    ConfigBoolean m_uploadKeepFiles;
    ConfigDouble m_requestBodyInMemoryLimit;
    ConfigDouble m_requestBodyLimit;
    ConfigDouble m_requestBodyNoFilesLimit;
    ConfigDouble m_responseBodyLimit;
    ConfigInt m_uploadFileLimit;
    ConfigInt m_uploadFileMode;
    DebugLog *m_debugLog;
    OnFailedRemoteRulesAction m_remoteRulesActionOnFailed;
    RuleEngine m_secRuleEngine;
    RulesExceptions m_exceptions;
    std::list<std::string> m_components;
    std::ostringstream m_parserError;
    std::set<std::string> m_responseBodyTypeToBeInspected;
    std::string m_auditLogParts;
    std::string m_auditLogPath;
    std::string m_httpblKey;
    std::string m_uploadDirectory;
    std::string m_uploadTmpDirectory;
    std::vector<actions::Action *> defaultActions[8];
    std::vector<modsecurity::Rule *> rules[8];
};

#endif

#ifdef __cplusplus
}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_PROPERTIES_H_
