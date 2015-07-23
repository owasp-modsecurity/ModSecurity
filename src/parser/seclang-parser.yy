%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {seclang_parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
# include <string>

class Driver;

#include "actions/action.h"
#include "actions/transformations/transformation.h"
#include "operators/operator.h"
#include "rule.h"
#include "utils/geo_lookup.h"
#include "audit_log.h"

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
using ModSecurity::actions::transformations::Transformation;
using ModSecurity::operators::Operator;
using ModSecurity::Rule;
using ModSecurity::Utils::GeoLookup;

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

}
// The parsing context.
%param { Driver& driver }
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
  CONFIG_DIR_VAL    "+"
  COMMA    "*"
  QUOTATION_MARK  ")"
  SPACE
  PIPE
  UNKNOWN
  FREE_TEXT
;

%left ARGS CONFIG_VALUE_RELEVANT_ONLY CONFIG_VALUE_ON CONFIG_VALUE_OFF CONFIG_VALUE
%token <std::string> DIRECTIVE
%token <std::string> CONFIG_DIRECTIVE
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT_ACTION
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT_ACTION
%token <std::string> CONFIG_DIR_RULE_ENG
%token <std::string> CONFIG_DIR_REQ_BODY
%token <std::string> CONFIG_DIR_RES_BODY
%token <std::string> CONFIG_VALUE
%token <std::string> CONFIG_VALUE_ON
%token <std::string> CONFIG_VALUE_OFF
%token <std::string> CONFIG_VALUE_DETC
%token <std::string> CONFIG_VALUE_SERIAL
%token <std::string> CONFIG_VALUE_PARALLEL
%token <std::string> CONFIG_VALUE_RELEVANT_ONLY
%token <std::string> CONFIG_VALUE_PROCESS_PARTIAL
%token <std::string> CONFIG_VALUE_REJECT

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

%token <std::string> CONFIG_DIR_GEO_DB

%token <std::string> OPERATOR
%token <std::string> ACTION
%token <std::string> ACTION_SEVERITY
%token <std::string> TRANSFORMATION

%token <double> CONFIG_VALUE_NUMBER

%type <std::vector<Action *> *> actions
%type <std::vector<Variable *> *> variables

%printer { yyoutput << $$; } <*>;
%%
%start secrule;


secrule:
    | secrule line

line: 
    expression
    | SPACE expression
    | SPACE
      {

      }

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


expression:
    audit_log
    | DIRECTIVE SPACE variables SPACE OPERATOR SPACE QUOTATION_MARK actions QUOTATION_MARK
      {
        Rule *rule = new Rule(
            /* op */ Operator::instantiate($5),
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
        driver.sec_request_body_access = true;
      }
    | CONFIG_DIR_REQ_BODY SPACE CONFIG_VALUE_OFF
      {
        driver.sec_request_body_access = false;
      }
    | CONFIG_DIR_RES_BODY SPACE CONFIG_VALUE_ON
      {
        driver.sec_request_body_access = true;
      }
    | CONFIG_DIR_RES_BODY SPACE CONFIG_VALUE_OFF
      {
        driver.sec_request_body_access = false;
      }
    | CONFIG_COMPONENT_SIG
      {
        driver.components.push_back($1);
      }
    /* Debug log: start */
    | CONFIG_DIR_DEBUG_LVL
      {
        driver.debug_level = atoi($1.c_str());
      }
    | CONFIG_DIR_DEBUG_LOG
      {
        driver.debug_log_path = $1;
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

variables:
    variables PIPE VARIABLE
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new Variable($3));
        $$ = $1;
      }
    | VARIABLE
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new Variable($1));
        $$ = variables;
      }
    | variables PIPE RUN_TIME_VAR_DUR
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new Duration($3));
        $$ = $1;
      }
    | RUN_TIME_VAR_DUR
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new Duration($1));
        $$ = variables;
      }
    | variables PIPE RUN_TIME_VAR_ENV
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new Env($3));
        $$ = $1;
      }
    | RUN_TIME_VAR_ENV
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new Env($1));
        $$ = variables;
      }
    | variables PIPE RUN_TIME_VAR_BLD
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModsecBuild($3));
        $$ = $1;
      }
    | RUN_TIME_VAR_BLD
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModsecBuild($1));
        $$ = variables;
      }
    | variables PIPE RUN_TIME_VAR_HSV
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new HighestSeverity($3));
        $$ = $1;
      }
    | RUN_TIME_VAR_HSV
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new HighestSeverity($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new Time($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_DAY
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new TimeDay($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_EPOCH
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeEpoch($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_HOUR
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeHour($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_MIN
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeMin($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_MON
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeMon($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_SEC
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeSec($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_WDAY
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeWDay($1));
        $$ = variables;
      }
    | RUN_TIME_VAR_TIME_YEAR
      {
        std::vector<Variable *> *variables = new std::vector<Variable *>;
        variables->push_back(new ModSecurity::Variables::TimeYear($1));
        $$ = variables;
      }
    | variables PIPE RUN_TIME_VAR_TIME
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::Time($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_DAY
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeDay($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_EPOCH
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeEpoch($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_HOUR
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeHour($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_MIN
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeMin($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_MON
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeMon($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_SEC
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeSec($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_WDAY
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeWDay($3));
        $$ = $1;
      }
    | variables PIPE RUN_TIME_VAR_TIME_YEAR
      {
        std::vector<Variable *> *v = $1;
        v->push_back(new ModSecurity::Variables::TimeYear($3));
        $$ = $1;
      }

actions:
    actions COMMA SPACE ACTION
      {
        std::vector<Action *> *a = $1;
        a->push_back(Action::instantiate($4));
        $$ = $1;
      }

    | actions COMMA ACTION
      {
        std::vector<Action *> *a = $1;
        a->push_back(Action::instantiate($3));
        $$ = $1;
      }
    | SPACE ACTION
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Action::instantiate($2));
        $$ = actions;

      }
    | ACTION
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Action::instantiate($1));
        $$ = actions;
      }
    | actions COMMA SPACE TRANSFORMATION
      {
        std::vector<Action *> *a = $1;
        a->push_back(Transformation::instantiate($4));
        $$ = $1;
      }

    | actions COMMA TRANSFORMATION
      {
        std::vector<Action *> *a = $1;
        a->push_back(Transformation::instantiate($3));
        $$ = $1;
      }
    | SPACE TRANSFORMATION
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Transformation::instantiate($2));
        $$ = actions;

      }
    | TRANSFORMATION
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Transformation::instantiate($1));
        $$ = actions;
      }
    | actions COMMA SPACE ACTION_SEVERITY
      {
        std::vector<Action *> *a = $1;
        a->push_back(Action::instantiate($4));
        $$ = $1;
      }
    | actions COMMA ACTION_SEVERITY
      {
        std::vector<Action *> *a = $1;
        a->push_back(Action::instantiate($3));
        $$ = $1;
      }
    | SPACE ACTION_SEVERITY
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Action::instantiate($2));
        $$ = actions;

      }
    | ACTION_SEVERITY
      {
        std::vector<Action *> *actions = new std::vector<Action *>;
        actions->push_back(Action::instantiate($1));
        $$ = actions;
      }


%%
void
yy::seclang_parser::error (const location_type& l,
                          const std::string& m)
{
    driver.parser_error (l, m);
}
