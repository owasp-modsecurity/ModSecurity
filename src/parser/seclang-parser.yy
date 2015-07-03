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

using ModSecurity::actions::Action;
using ModSecurity::actions::transformations::Transformation;
using ModSecurity::operators::Operator;
using ModSecurity::Variable;
using ModSecurity::Rule;

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
  NEW_LINE
  UNKNOWN
  FREE_TEXT
;

%left ARGS CONFIG_VALUE_RELEVANT_ONLY CONFIG_VALUE_ON CONFIG_VALUE_OFF CONFIG_VALUE
%token <std::string> DIRECTIVE
%token <std::string> CONFIG_DIRECTIVE
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

%token <std::string> CONFIG_DIR_AUDIT_DIR
%token <std::string> CONFIG_DIR_AUDIT_DIR_MOD
%token <std::string> CONFIG_DIR_AUDIT_ENG
%token <std::string> CONFIG_DIR_AUDIT_FLE_MOD
%token <std::string> CONFIG_DIR_AUDIT_LOG
%token <std::string> CONFIG_DIR_AUDIT_LOG2
%token <std::string> CONFIG_DIR_AUDIT_LOG_P
%token <std::string> CONFIG_DIR_AUDIT_STS
%token <std::string> CONFIG_DIR_AUDIT_TPE

%token <std::string> CONFIG_DIR_DEBUG_LOG
%token <std::string> CONFIG_DIR_DEBUG_LVL

%token <std::string> OPERATOR
%token <std::string> ACTION
%token <std::string> VARIABLE
%token <std::string> TRANSFORMATION

%token <double> CONFIG_VALUE_NUMBER

%type <std::vector<Action *> *> actions
%type <std::vector<Variable> *> variables

%printer { yyoutput << $$; } <*>;
%%
%start secrule;


secrule:
    | secrule line

line: 
    expression NEW_LINE
    | SPACE expression NEW_LINE
    | NEW_LINE
    | SPACE NEW_LINE
    | SPACE

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
        driver.sec_rule_engine = 0;
      }
    | CONFIG_DIR_RULE_ENG SPACE CONFIG_VALUE_ON
      {
        driver.sec_rule_engine = 1;
      }
    | CONFIG_DIR_RULE_ENG SPACE CONFIG_VALUE_DETC
      {
        driver.sec_rule_engine = 2;
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

variables:
    variables PIPE VARIABLE
      {
        std::vector<Variable> *v = $1;
        v->push_back(Variable($3));
        $$ = $1;
      }
    | VARIABLE
      {
        std::vector<Variable> *variables = new std::vector<Variable>;
        variables->push_back(Variable($1));
        $$ = variables;
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


%%
void
yy::seclang_parser::error (const location_type& l,
                          const std::string& m)
{
    driver.error (l, m);
}
