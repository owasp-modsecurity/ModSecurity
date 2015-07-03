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

%left ARGS CONFIG_VALUE_ON CONFIG_VALUE_OFF CONFIG_VALUE
%token <std::string> DIRECTIVE
%token <std::string> CONFIG_DIRECTIVE
%token <std::string> CONFIG_DIR_RULE_ENG
%token <std::string> CONFIG_DIR_REQ_BODY
%token <std::string> CONFIG_DIR_RES_BODY
%token <std::string> CONFIG_DIR_AUDIT_ENG
%token <std::string> CONFIG_DIR_AUDIT_TPE
%token <std::string> CONFIG_VALUE
%token <std::string> CONFIG_VALUE_ON
%token <std::string> CONFIG_VALUE_OFF
%token <std::string> CONFIG_VALUE_DETC
%token <std::string> CONFIG_VALUE_SERIAL
%token <std::string> CONFIG_VALUE_PARALLEL
%token <std::string> CONFIG_DIR_AUDIT_LOG
%token <std::string> CONFIG_DIR_AUDIT_LOG_P
%token <std::string> CONFIG_DIR_DEBUG_LOG
%token <std::string> CONFIG_DIR_DEBUG_LVL
%token <std::string> OPERATOR
%token <std::string> ACTION
%token <std::string> VARIABLE
%token <std::string> TRANSFORMATION

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

expression:
    DIRECTIVE SPACE variables SPACE OPERATOR SPACE QUOTATION_MARK actions QUOTATION_MARK
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
    | CONFIG_DIR_AUDIT_ENG SPACE CONFIG_VALUE_ON
      {
        driver.sec_audit_engine = true;
      }
    | CONFIG_DIR_AUDIT_ENG SPACE CONFIG_VALUE_OFF
      {
        driver.sec_audit_engine = false;
      }
    | CONFIG_DIR_AUDIT_TPE SPACE CONFIG_VALUE_SERIAL
      {
        driver.sec_audit_type = 0;
      }
    | CONFIG_DIR_AUDIT_TPE SPACE CONFIG_VALUE_PARALLEL
      {
        driver.sec_audit_type = 1;
      }
    | CONFIG_DIR_AUDIT_LOG
      {
        //driver.audit_log_path = $1;
      }
    | CONFIG_DIR_AUDIT_LOG_P
      {
        //driver.audit_log_parts = $1;
      }
    | CONFIG_DIR_DEBUG_LVL
      {
        driver.debug_level = atoi($1.c_str());
      }
    | CONFIG_DIR_DEBUG_LOG
      {
        driver.debug_log_path = $1;
      }

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
