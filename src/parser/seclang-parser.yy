%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define parser_class_name {seclang_parser}
%define api.token.constructor
%define api.value.type variant
//%define api.namespace {ModSecurity::yy}
%define parse.assert
%code requires
{
# include <string>

namespace ModSecurity {
namespace Parser {
class Driver;
}
}

#include "actions/action.h"
#include "actions/set_var.h"
#include "actions/severity.h"
#include "actions/msg.h"
#include "actions/log_data.h"
#include "actions/rev.h"
#include "actions/tag.h"
#include "actions/transformations/transformation.h"
#include "operators/operator.h"
#include "rule.h"
#include "utils/geo_lookup.h"
#include "audit_log.h"

#include "variables/variations/count.h"
#include "variables/variations/exclusion.h"
#include "variables/duration.h"
#include "variables/env.h"
#include "variables/highest_severity.h"
#include "variables/modsec_build.h"
#include "variables/time_day.h"
#include "variables/time_epoch.h"
#include "variables/time.h"
#include "variables/time_hour.h"
#include "variables/time_min.h"
#include "variables/time_mon.h"
#include "variables/time_sec.h"
#include "variables/time_wday.h"
#include "variables/time_year.h"

using ModSecurity::actions::Action;
using ModSecurity::actions::SetVar;
using ModSecurity::actions::Severity;
using ModSecurity::actions::Tag;
using ModSecurity::actions::Rev;
using ModSecurity::actions::Msg;
using ModSecurity::actions::LogData;
using ModSecurity::actions::transformations::Transformation;
using ModSecurity::operators::Operator;
using ModSecurity::Rule;
using ModSecurity::Utils::GeoLookup;

using ModSecurity::Variables::Variations::Count;
using ModSecurity::Variables::Variations::Exclusion;
using ModSecurity::Variables::Duration;
using ModSecurity::Variables::Env;
using ModSecurity::Variables::HighestSeverity;
using ModSecurity::Variables::ModsecBuild;
using ModSecurity::Variables::Time;
using ModSecurity::Variables::TimeDay;
using ModSecurity::Variables::TimeEpoch;
using ModSecurity::Variables::TimeHour;
using ModSecurity::Variables::TimeMin;
using ModSecurity::Variables::TimeMon;
using ModSecurity::Variables::TimeSec;
using ModSecurity::Variables::TimeWDay;
using ModSecurity::Variables::TimeYear;
using ModSecurity::Variables::Variable;


#define CHECK_VARIATION_DECL \
    Variable *var = NULL; \
    bool t = false;

#define CHECK_VARIATION(a) \
    if (var == NULL) { \
        if (name.at(0) == std::string(#a).at(0)) { \
            name.erase(0, 1); \
            t = true ; \
        } \
    } else { \
        t = false; \
    } \
    if (t)


/**
 * %destructor { code } THING
 *
 * %destructor is not working as expected. Apparently it was fixed on a most recent,
 * version of Bison. We are not interested to limit the usage to this newest version,
 * thus, we have to deal with memory leak when rules failed to load. Not that big
 * problem, as we don't really continue when it fails (only for SecRemoteRules).
 *
 * Information about destructor:
 * http://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 *
 * Patch:
 * https://www.mail-archive.com/search?l=bug-bison@gnu.org&q=subject:%22Destructor+miscompilation+with+C%2B%2B+variants%3F%22&o=newest&f=1
 */

}
// The parsing context.
%param { ModSecurity::Parser::Driver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose
%code
{
#include "parser/driver.h"
}
%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  COMMA    ","
  SPACE
  PIPE
;

%left SPACE CONFIG_VALUE_RELEVANT_ONLY CONFIG_VALUE_ON CONFIG_VALUE_OFF
%token <std::string> QUOTATION_MARK
%token <std::string> DIRECTIVE
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT_ACTION
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT_ACTION

%token <std::string> CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION
%token <std::string> CONFIG_DIR_PCRE_MATCH_LIMIT
%token <std::string> CONGIG_DIR_RESPONSE_BODY_MP
%token <std::string> CONGIG_DIR_SEC_TMP_DIR
%token <std::string> CONGIG_DIR_SEC_DATA_DIR
%token <std::string> CONGIG_DIR_SEC_ARG_SEP
%token <std::string> CONGIG_DIR_SEC_COOKIE_FORMAT
%token <std::string> CONGIG_DIR_SEC_STATUS_ENGINE
%token <std::string> CONFIG_DIR_UNICODE_MAP_FILE

%token <std::string> CONFIG_DIR_RULE_ENG
%token <std::string> CONFIG_DIR_REQ_BODY
%token <std::string> CONFIG_DIR_RES_BODY
%token <std::string> CONFIG_VALUE_ON
%token <std::string> CONFIG_VALUE_OFF
%token <std::string> CONFIG_VALUE_DETC
%token <std::string> CONFIG_VALUE_SERIAL
%token <std::string> CONFIG_VALUE_PARALLEL
%token <std::string> CONFIG_VALUE_RELEVANT_ONLY
%token <std::string> CONFIG_VALUE_PROCESS_PARTIAL
%token <std::string> CONFIG_VALUE_REJECT
%token <std::string> CONFIG_VALUE_ABORT
%token <std::string> CONFIG_VALUE_WARN

%token <std::string> CONFIG_DIR_AUDIT_DIR
%token <std::string> CONFIG_DIR_AUDIT_DIR_MOD
%token <std::string> CONFIG_DIR_AUDIT_ENG
%token <std::string> CONFIG_DIR_AUDIT_FLE_MOD
%token <std::string> CONFIG_DIR_AUDIT_LOG
%token <std::string> CONFIG_DIR_AUDIT_LOG2
%token <std::string> CONFIG_DIR_AUDIT_LOG_P
%token <std::string> CONFIG_DIR_AUDIT_STS
%token <std::string> CONFIG_DIR_AUDIT_TPE

%token <std::string> CONFIG_COMPONENT_SIG

%token <std::string> CONFIG_DIR_DEBUG_LOG
%token <std::string> CONFIG_DIR_DEBUG_LVL

%token <std::string> VARIABLE
%token <std::string> RUN_TIME_VAR_DUR
%token <std::string> RUN_TIME_VAR_ENV
%token <std::string> RUN_TIME_VAR_BLD
%token <std::string> RUN_TIME_VAR_HSV

%token <std::string> RUN_TIME_VAR_TIME
%token <std::string> RUN_TIME_VAR_TIME_DAY
%token <std::string> RUN_TIME_VAR_TIME_EPOCH
%token <std::string> RUN_TIME_VAR_TIME_HOUR
%token <std::string> RUN_TIME_VAR_TIME_MIN
%token <std::string> RUN_TIME_VAR_TIME_MON
%token <std::string> RUN_TIME_VAR_TIME_SEC
%token <std::string> RUN_TIME_VAR_TIME_WDAY
%token <std::string> RUN_TIME_VAR_TIME_YEAR

%token <std::string> CONFIG_SEC_REMOTE_RULES_FAIL_ACTION

%token <std::string> CONFIG_DIR_GEO_DB

%token <std::string> OPERATOR
%token <std::string> FREE_TEXT
%token <std::string> ACTION
%token <std::string> ACTION_SEVERITY
%token <std::string> ACTION_SETVAR
%token <std::string> ACTION_MSG
%token <std::string> ACTION_TAG
%token <std::string> ACTION_REV
%token <std::string> LOG_DATA
%token <std::string> TRANSFORMATION
%token <std::string> ACTION_CTL_BDY_XML
%token <std::string> ACTION_CTL_BDY_JSON
%token <std::string> ACTION_CTL_AUDIT_LOG_PARTS

%type <std::vector<Action *> *> actions
%type <std::vector<Variable *> *> variables
%type <Variable *> var
%type <Action *> act

%printer { yyoutput << $$; } <*>;
%%
%start input;


input:
    %empty
    | input line
    | SPACE
    ;

line: expression
    | SPACE expression
    | expression SPACE
    ;

audit_log:
    /* SecAuditLogDirMode */
    CONFIG_DIR_AUDIT_DIR_MOD
      {
        driver.audit_log->setStorageDirMode(strtol($1.c_str(), NULL, 8));
      }

    /* SecAuditLogStorageDir */
    | CONFIG_DIR_AUDIT_DIR
      {
        driver.audit_log->setStorageDir($1);
      }

    /* SecAuditEngine */
    | CONFIG_DIR_AUDIT_ENG SPACE CONFIG_VALUE_RELEVANT_ONLY
      {
        driver.audit_log->setStatus(ModSecurity::AuditLog::RelevantOnlyAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG SPACE CONFIG_VALUE_OFF
      {
        driver.audit_log->setStatus(ModSecurity::AuditLog::OffAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG SPACE CONFIG_VALUE_ON
      {
        driver.audit_log->setStatus(ModSecurity::AuditLog::OnAuditLogStatus);
      }

    /* SecAuditLogFileMode */
    | CONFIG_DIR_AUDIT_FLE_MOD
      {
        driver.audit_log->setFileMode(strtol($1.c_str(), NULL, 8));
      }

    /* SecAuditLog2 */
    | CONFIG_DIR_AUDIT_LOG2
      {
        driver.audit_log->setFilePath2($1);
      }

    /* SecAuditLogParts */
    | CONFIG_DIR_AUDIT_LOG_P
      {
        driver.audit_log->setParts($1);
      }

    /* SecAuditLog */
    | CONFIG_DIR_AUDIT_LOG
      {
        driver.audit_log->setFilePath1($1);
      }

    /* SecAuditLogRelevantStatus */
    | CONFIG_DIR_AUDIT_STS
      {
        std::string relevant_status($1);
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.audit_log->setRelevantStatus(relevant_status);
      }

    /* SecAuditLogType */
    | CONFIG_DIR_AUDIT_TPE SPACE CONFIG_VALUE_SERIAL
      {
        driver.audit_log->setType(ModSecurity::AuditLog::SerialAuditLogType);
      }
    | CONFIG_DIR_AUDIT_TPE SPACE CONFIG_VALUE_PARALLEL
      {
        driver.audit_log->setType(ModSecurity::AuditLog::ParallelAuditLogType);
      }
    ;

expression:
    audit_log
    | DIRECTIVE SPACE variables SPACE OPERATOR SPACE QUOTATION_MARK actions SPACE QUOTATION_MARK
    | DIRECTIVE SPACE variables SPACE OPERATOR SPACE QUOTATION_MARK actions QUOTATION_MARK
      {
        Operator *op = Operator::instantiate($5);
        const char *error = NULL;
        if (op->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }
        Rule *rule = new Rule(
            /* op */ op,
            /* variables */ $3,
            /* actions */ $8
            );
        driver.addSecRule(rule);
      }
    | DIRECTIVE SPACE variables SPACE FREE_TEXT SPACE QUOTATION_MARK actions SPACE QUOTATION_MARK
    | DIRECTIVE SPACE variables SPACE FREE_TEXT SPACE QUOTATION_MARK actions QUOTATION_MARK
      {
        Operator *op = Operator::instantiate("@pm " + $5);
        const char *error = NULL;
        if (op->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }
        Rule *rule = new Rule(
            /* op */ op,
            /* variables */ $3,
            /* actions */ $8
            );
        driver.addSecRule(rule);
      }
    | CONFIG_DIR_RULE_ENG SPACE CONFIG_VALUE_OFF
      {
        driver.secRuleEngine = ModSecurity::Rules::DisabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG SPACE CONFIG_VALUE_ON
      {
        driver.secRuleEngine = ModSecurity::Rules::EnabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG SPACE CONFIG_VALUE_DETC
      {
        driver.secRuleEngine = ModSecurity::Rules::DetectionOnlyRuleEngine;
      }
    | CONFIG_DIR_REQ_BODY SPACE CONFIG_VALUE_ON
      {
        driver.secRequestBodyAccess = true;
      }
    | CONFIG_DIR_REQ_BODY SPACE CONFIG_VALUE_OFF
      {
        driver.secRequestBodyAccess = false;
      }
    | CONFIG_DIR_RES_BODY SPACE CONFIG_VALUE_ON
      {
        driver.secResponseBodyAccess = true;
      }
    | CONFIG_DIR_RES_BODY SPACE CONFIG_VALUE_OFF
      {
        driver.secResponseBodyAccess = false;
      }
    | CONFIG_COMPONENT_SIG
      {
        driver.components.push_back($1);
      }
    /* Debug log: start */
    | CONFIG_DIR_DEBUG_LVL
      {
        if (driver.m_debugLog != NULL) {
          driver.m_debugLog->setDebugLogLevel(atoi($1.c_str()));
        } else {
            driver.parserError << "Internal error, there is no DebugLog ";
            driver.parserError << "object associated with the driver class";
            YYERROR;
        }
      }
    | CONFIG_DIR_DEBUG_LOG
      {
        if (driver.m_debugLog != NULL) {
            driver.m_debugLog->setDebugLogFile($1);
        } else {
            driver.parserError << "Internal error, there is no DebugLog ";
            driver.parserError << "object associated with the driver class";
            YYERROR;
        }
      }
    /* Debug log: end */
    | CONFIG_DIR_GEO_DB
      {
        GeoLookup::getInstance().setDataBase($1);
      }
    /* Body limits */
    | CONFIG_DIR_REQ_BODY_LIMIT
      {
        driver.requestBodyLimit = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT
      {
        driver.requestBodyNoFilesLimit = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT
      {
        driver.requestBodyInMemoryLimit = atoi($1.c_str());
      }
    | CONFIG_DIR_RES_BODY_LIMIT
      {
        driver.responseBodyLimit = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION SPACE CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.requestBodyLimitAction = ModSecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION SPACE CONFIG_VALUE_REJECT
      {
        driver.requestBodyLimitAction = ModSecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION SPACE CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.responseBodyLimitAction = ModSecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION SPACE CONFIG_VALUE_REJECT
      {
        driver.responseBodyLimitAction = ModSecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION SPACE CONFIG_VALUE_ABORT
      {
        driver.remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION SPACE CONFIG_VALUE_WARN
      {
        driver.remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
    | CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION
    | CONFIG_DIR_PCRE_MATCH_LIMIT
    | CONGIG_DIR_RESPONSE_BODY_MP
    | CONGIG_DIR_SEC_TMP_DIR
    | CONGIG_DIR_SEC_DATA_DIR
    | CONGIG_DIR_SEC_ARG_SEP
    | CONGIG_DIR_SEC_COOKIE_FORMAT
    | CONGIG_DIR_SEC_STATUS_ENGINE
    | CONFIG_DIR_UNICODE_MAP_FILE
      {
      }
    ;

variables:
    variables PIPE var
      {
        std::vector<Variable *> *v = $1;
        v->push_back($3);
        $$ = $1;
      }
    | var
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back($1);
        $$ = variables;
      }
    ;

var:
    VARIABLE
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Variable(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Variable(name)); }
        if (!var) { var = new Variable(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_DUR
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Duration(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Duration(name)); }
        if (!var) { var = new Duration(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_ENV
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Env(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Env(name)); }
        if (!var) { var = new Env(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_BLD
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new ModsecBuild(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new ModsecBuild(name)); }
        if (!var) { var = new ModsecBuild(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_HSV
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new HighestSeverity(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new HighestSeverity(name)); }
        if (!var) { var = new HighestSeverity(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Time(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Time(name)); }
        if (!var) { var = new Time(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_DAY
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeDay(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeDay(name)); }
        if (!var) { var = new TimeDay(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_EPOCH
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeEpoch(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeEpoch(name)); }
        if (!var) { var = new TimeEpoch(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_HOUR
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeHour(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeHour(name)); }
        if (!var) { var = new TimeHour(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_MIN
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeMin(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeMin(name)); }
        if (!var) { var = new TimeMin(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_MON
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeMon(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeMon(name)); }
        if (!var) { var = new TimeMon(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_SEC
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeSec(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeSec(name)); }
        if (!var) { var = new TimeSec(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_WDAY
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeWDay(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeWDay(name)); }
        if (!var) { var = new TimeWDay(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_TIME_YEAR
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new TimeYear(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new TimeYear(name)); }
        if (!var) { var = new TimeYear(name); }
        $$ = var;
      }
    ;

act:
    ACTION
      {
        $$ = Action::instantiate($1);
      }
    | TRANSFORMATION
      {
        $$ = Transformation::instantiate($1);
      }
    | ACTION_SEVERITY
      {
        $$ =  new Severity($1);
      }
    | ACTION_SETVAR
      {
        std::string error;
        SetVar *setVar = new SetVar($1);

        if (setVar->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }

        $$ = setVar;
      }
    | LOG_DATA
      {
        $$ = new LogData($1);
      }
    | ACTION_MSG
      {
        $$ = new Msg($1);
      }
    | ACTION_TAG
      {
        $$ = new Tag($1);
      }
    | ACTION_REV
      {
        $$ = new Rev($1);
      }
    | ACTION_CTL_BDY_XML
      {
        /* not ready yet. */
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_BDY_JSON
      {
        /* not ready yet. */
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_AUDIT_LOG_PARTS
      {
        /* not ready yet. */
        $$ = Action::instantiate($1);
      }
    ;

actions:
    actions COMMA SPACE act
      {
        std::vector<Action *> *a = $1;
        a->push_back($4);
        $$ = $1;
      }
    | actions COMMA act
      {
        std::vector<Action *> *a = $1;
        a->push_back($3);
        $$ = $1;
      }
    | SPACE act
      {
        std::vector<Action *> *a = new std::vector<Action *>;
        a->push_back($2);
        $$ = a;
      }
    | act
      {
        std::vector<Action *> *a = new std::vector<Action *>;
        a->push_back($1);
        $$ = a;
      }
    ;

%%
void
yy::seclang_parser::error (const location_type& l,
                          const std::string& m)
{
    driver.error (l, m);
}
