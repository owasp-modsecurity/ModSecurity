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

#ifdef __cplusplus
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#endif


#ifndef HEADERS_MODSECURITY_RULES_H_
#define HEADERS_MODSECURITY_RULES_H_

#ifndef __cplusplus
typedef struct Rules_t Rules;
typedef struct Assay_t Assay;
#endif

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"
#include "modsecurity/debug_log.h"


#ifdef __cplusplus
class Driver;

namespace ModSecurity {
class Rule;
class AuditLog;

/** @ingroup ModSecurity_CPP_API */
class Rules {
 public:
    Rules()
        : m_referenceCount(0),
        requestBodyLimit(0),
        responseBodyLimit(0),
        m_custom_debug_log(NULL) { }

    explicit Rules(DebugLog *custom_log)
        : m_referenceCount(0),
        m_custom_debug_log(custom_log) { }

    ~Rules();

    void incrementReferenceCount(void);
    void decrementReferenceCount(void);

    /**
    * FIXME:
    *
    * names should follow a patterner
    *
    */
    int loadFromUri(char *uri);
    int loadRemote(char *key, char *uri);
    int load(const char *rules);

    int merge(Driver *driver);
    int merge(Rules *rules);

    int evaluate(int phase, Assay *assay);

    std::vector<Rule *> rules[7];  // Number of Phases.

    /**
     *
     * The RuleEngine enumerator consists in mapping the different states
     * of the rule engine.
     *
     */
    enum RuleEngine {
    /**
     * Rules won't be evaluated if Rule Engine is set to DisabledRuleEngine
     *
     */
     DisabledRuleEngine,
    /**
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
    int sec_audit_type;
    bool sec_audit_engine;
    bool sec_request_body_access;
    bool sec_response_body_access;
    std::string audit_log_path;
    std::string audit_log_parts;
    std::string debug_log_path;
    int debug_level;
    DebugLog *debug_log;
    void debug(int level, std::string message);
    std::list<std::string> components;

    int requestBodyLimit;
    int responseBodyLimit;
    AuditLog *audit_log;

 private:
    int m_referenceCount;
    DebugLog *m_custom_debug_log;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

Rules *msc_create_rules_set();
int msc_rules_merge(Rules *rules_dst, Rules *rules_from);
int msc_rules_add_remote(Rules *rules, char *key, char *uri);
int msc_rules_add_file(Rules *rules, char *file);
int msc_rules_add(Rules *rules, const char *plain_rules);
int msc_rules_cleanup(Rules *rules);

#ifdef __cplusplus
}
}  // namespace ModSecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_H_
