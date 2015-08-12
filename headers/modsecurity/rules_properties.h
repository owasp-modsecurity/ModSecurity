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
#endif


#ifndef HEADERS_MODSECURITY_RULES_PROPERTIES_H_
#define HEADERS_MODSECURITY_RULES_PROPERTIES_H_

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"

#ifdef __cplusplus

namespace ModSecurity {
class Rule;
class AuditLog;
namespace Parser {
class Driver;
}

/** @ingroup ModSecurity_CPP_API */
class RulesProperties {
 public:
    RulesProperties()
        : audit_log(NULL),
        customDebugLog(NULL),
        remoteRulesActionOnFailed(AbortOnFailedRemoteRulesAction),
        requestBodyLimit(0),
        secRequestBodyAccess(false),
        secResponseBodyAccess(false),
        requestBodyLimitAction(ProcessPartialBodyLimitAction),
        responseBodyLimit(0),
        debugLevel(0),
        responseBodyLimitAction(ProcessPartialBodyLimitAction),
        secRuleEngine(DetectionOnlyRuleEngine) { }

    explicit RulesProperties(DebugLog *customDebugLog)
        : audit_log(NULL),
        customDebugLog(customDebugLog),
        remoteRulesActionOnFailed(AbortOnFailedRemoteRulesAction),
        secRequestBodyAccess(false),
        secResponseBodyAccess(false),
        debugLevel(0),
        requestBodyLimit(0),
        requestBodyLimitAction(ProcessPartialBodyLimitAction),
        responseBodyLimit(0),
        responseBodyLimitAction(ProcessPartialBodyLimitAction),
        secRuleEngine(DetectionOnlyRuleEngine) { }

    ~RulesProperties() { }

    std::vector<Rule *> rules[7];  // ModSecurity::Phases::NUMBER_OF_PHASES


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
    double requestBodyLimit;
    double responseBodyLimit;
    BodyLimitAction requestBodyLimitAction;
    BodyLimitAction responseBodyLimitAction;

    DebugLog *customDebugLog;

    bool secRequestBodyAccess;
    bool secResponseBodyAccess;
    std::string audit_log_path;
    std::string audit_log_parts;
    std::string debug_log_path;
    int debugLevel;
    std::list<std::string> components;


    std::ostringstream parserError;

    AuditLog *audit_log;

    OnFailedRemoteRulesAction remoteRulesActionOnFailed;
};
#endif

#ifdef __cplusplus
}  // namespace ModSecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_PROPERTIES_H_
