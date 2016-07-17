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
#include "modsecurity/transaction.h"
#include "modsecurity/rules_exceptions.h"

#ifdef __cplusplus

namespace modsecurity {
class Rule;
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
class RulesProperties {
 public:
    RulesProperties()
        : audit_log(NULL),
        m_debugLog(new DebugLog()),
        m_httpbl_key(""),
        remoteRulesActionOnFailed(AbortOnFailedRemoteRulesAction),
        requestBodyLimit(0),
        requestBodyNoFilesLimit(0),
        requestBodyInMemoryLimit(0),
        secRequestBodyAccess(false),
        secResponseBodyAccess(false),
        secXMLExternalEntity(false),
        requestBodyLimitAction(ProcessPartialBodyLimitAction),
        responseBodyLimit(0),
        responseBodyLimitAction(ProcessPartialBodyLimitAction),
        secRuleEngine(DetectionOnlyRuleEngine),
        uploadKeepFiles(0),
        uploadFileLimit(100),
        uploadFileMode(0),
        tmpSaveUploadedFiles(false) { }

    explicit RulesProperties(DebugLog *debugLog)
        : audit_log(NULL),
        m_debugLog(debugLog),
        m_httpbl_key(""),
        remoteRulesActionOnFailed(AbortOnFailedRemoteRulesAction),
        requestBodyLimit(0),
        requestBodyNoFilesLimit(0),
        requestBodyInMemoryLimit(0),
        secRequestBodyAccess(false),
        secResponseBodyAccess(false),
        secXMLExternalEntity(false),
        requestBodyLimitAction(ProcessPartialBodyLimitAction),
        responseBodyLimit(0),
        responseBodyLimitAction(ProcessPartialBodyLimitAction),
        secRuleEngine(DetectionOnlyRuleEngine),
        uploadKeepFiles(0),
        uploadFileLimit(100),
        uploadFileMode(0),
        tmpSaveUploadedFiles(false) { }

/*
    RulesProperties(const RulesProperties &other)
        : audit_log(other.audit_log),
        m_debugLog(other.m_debugLog),
        remoteRulesActionOnFailed(other.remoteRulesActionOnFailed),
        requestBodyLimit(other.requestBodyLimit),
        requestBodyNoFilesLimit(other.requestBodyNoFilesLimit),
        requestBodyInMemoryLimit(other.requestBodyInMemoryLimit),
        secRequestBodyAccess(other.secRequestBodyAccess),
        secResponseBodyAccess(other.secResponseBodyAccess),
        requestBodyLimitAction(other.requestBodyLimitAction),
        responseBodyLimit(other.responseBodyLimit),
        responseBodyLimitAction(other.responseBodyLimitAction),
        secRuleEngine(other.secRuleEngine) { }
*/
    ~RulesProperties() {
        delete m_debugLog;
    }

    std::vector<Rule *> rules[7];
    std::vector<Rule *> * getRulesForPhase(int phase) {
        if (phase > 7) {
            return NULL;
        }
        return &rules[phase];
    }

    //  ModSecurity::Phases::NUMBER_OF_PHASES
    std::vector<actions::Action *> defaultActions[7];
    //  ModSecurity::Phases::NUMBER_OF_PHASES

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
     DetectionOnlyRuleEngine
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
     RejectBodyLimitAction
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
     WarnOnFailedRemoteRulesAction
    };


    static const char *ruleEngineStateString(RuleEngine i) {
      switch (i) {
      case DisabledRuleEngine:
          return "Disabled";
      case EnabledRuleEngine:
          return "Enabled";
      case DetectionOnlyRuleEngine:
          return "DetectionOnly";
      }
      return NULL;
    }


    RuleEngine secRuleEngine;
    double requestBodyNoFilesLimit;
    double requestBodyInMemoryLimit;
    double requestBodyLimit;
    double responseBodyLimit;
    BodyLimitAction requestBodyLimitAction;
    BodyLimitAction responseBodyLimitAction;

    bool secRequestBodyAccess;
    bool secResponseBodyAccess;
    bool secXMLExternalEntity;
    std::string audit_log_path;
    std::string audit_log_parts;
    std::list<std::string> components;
    std::set<std::string> m_responseBodyTypeToBeInspected;

    DebugLog *m_debugLog;

    std::string m_httpbl_key;
    std::ostringstream parserError;

    bool uploadKeepFiles;
    int uploadFileLimit;
    int uploadFileMode;
    std::string uploadDirectory;
    std::string uploadTmpDirectory;
    bool tmpSaveUploadedFiles;

    audit_log::AuditLog *audit_log;

    OnFailedRemoteRulesAction remoteRulesActionOnFailed;

    RulesExceptions m_exceptions;
};
#endif

#ifdef __cplusplus
}  // namespace modsecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_PROPERTIES_H_
