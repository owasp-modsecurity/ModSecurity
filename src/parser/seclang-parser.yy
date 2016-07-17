%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {seclang_parser}
%define api.token.constructor
%define api.value.type variant
//%define api.namespace {modsecurity::yy}
%define parse.assert
%code requires
{
#include <string>
#include <iterator>

namespace ModSecurity {
namespace Parser {
class Driver;
}
}

#include "modsecurity/modsecurity.h"

#include "actions/accuracy.h"
#include "actions/action.h"
#include "actions/audit_log.h"
#include "actions/ctl_audit_log_parts.h"
#include "actions/ctl_request_body_processor_json.h"
#include "actions/ctl_request_body_processor_xml.h"
#include "actions/init_col.h"
#include "actions/set_sid.h"
#include "actions/set_uid.h"
#include "actions/set_var.h"
#include "actions/severity.h"
#include "actions/skip.h"
#include "actions/skip_after.h"
#include "actions/msg.h"
#include "actions/phase.h"
#include "actions/allow.h"
#include "actions/log_data.h"
#include "actions/maturity.h"
#include "actions/redirect.h"
#include "actions/rev.h"
#include "actions/ver.h"
#include "actions/tag.h"
#include "actions/transformations/transformation.h"
#include "actions/transformations/none.h"
#include "actions/xmlns.h"
#include "operators/operator.h"
#include "modsecurity/rule.h"
#include "utils/geo_lookup.h"
#include "audit_log/audit_log.h"
#include "utils.h"

#include "variables/variations/count.h"
#include "variables/variations/exclusion.h"
#include "variables/duration.h"
#include "variables/env.h"
#include "variables/highest_severity.h"
#include "variables/modsec_build.h"
#include "variables/remote_user.h"
#include "variables/time_day.h"
#include "variables/time_epoch.h"
#include "variables/time.h"
#include "variables/time_hour.h"
#include "variables/time_min.h"
#include "variables/time_mon.h"
#include "variables/time_sec.h"
#include "variables/time_wday.h"
#include "variables/time_year.h"
#include "variables/tx.h"
#include "variables/xml.h"
#include "variables/rule.h"

using modsecurity::ModSecurity;

using modsecurity::actions::Accuracy;
using modsecurity::actions::Action;
using modsecurity::actions::CtlAuditLogParts;
using modsecurity::actions::CtlRequestBodyProcessorXML;
using modsecurity::actions::CtlRequestBodyProcessorJSON;
using modsecurity::actions::InitCol;
using modsecurity::actions::SetSID;
using modsecurity::actions::SetUID;
using modsecurity::actions::SetVar;
using modsecurity::actions::Severity;
using modsecurity::actions::Tag;
using modsecurity::actions::Redirect;
using modsecurity::actions::Rev;
using modsecurity::actions::Ver;
using modsecurity::actions::Msg;
using modsecurity::actions::Phase;
using modsecurity::actions::Allow;
using modsecurity::actions::transformations::None;
using modsecurity::actions::LogData;
using modsecurity::actions::Maturity;
using modsecurity::actions::transformations::Transformation;
using modsecurity::operators::Operator;
using modsecurity::Rule;
using modsecurity::Utils::GeoLookup;
using modsecurity::removeBracketsIfNeeded;

using modsecurity::Variables::Variations::Count;
using modsecurity::Variables::Variations::Exclusion;
using modsecurity::Variables::Duration;
using modsecurity::Variables::Env;
using modsecurity::Variables::HighestSeverity;
using modsecurity::Variables::ModsecBuild;
using modsecurity::Variables::RemoteUser;
using modsecurity::Variables::Time;
using modsecurity::Variables::TimeDay;
using modsecurity::Variables::TimeEpoch;
using modsecurity::Variables::TimeHour;
using modsecurity::Variables::TimeMin;
using modsecurity::Variables::TimeMon;
using modsecurity::Variables::TimeSec;
using modsecurity::Variables::TimeWDay;
using modsecurity::Variables::TimeYear;
using modsecurity::Variables::Variable;
using modsecurity::Variables::Tx;
using modsecurity::Variables::XML;


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
%param { modsecurity::Parser::Driver& driver }
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
  PIPE
;

%token <std::string> QUOTATION_MARK
%token <std::string> DIRECTIVE
%token <std::string> DIRECTIVE_SECRULESCRIPT
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
%token <std::string> CONFIG_VALUE_HTTPS
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

%token <std::string> CONFIG_SEC_RULE_REMOVE_BY_ID

%token <std::string> CONFIG_UPDLOAD_KEEP_FILES
%token <std::string> CONFIG_UPDLOAD_SAVE_TMP_FILES
%token <std::string> CONFIG_UPLOAD_FILE_LIMIT
%token <std::string> CONFIG_UPLOAD_FILE_MODE
%token <std::string> CONFIG_UPLOAD_DIR

%token <std::string> CONFIG_COMPONENT_SIG

%token <std::string> CONFIG_DIR_DEBUG_LOG
%token <std::string> CONFIG_DIR_DEBUG_LVL

%token <std::string> CONFIG_XML_EXTERNAL_ENTITY

%token <std::string> CONFIG_DIR_SEC_ACTION
%token <std::string> CONFIG_DIR_SEC_DEFAULT_ACTION
%token <std::string> CONFIG_DIR_SEC_MARKER

%token <std::string> VARIABLE
%token <std::string> VARIABLE_STATUS
%token <std::string> VARIABLE_TX
%token <std::string> VARIABLE_COL
%token <std::string> RUN_TIME_VAR_DUR
%token <std::string> RUN_TIME_VAR_ENV
%token <std::string> RUN_TIME_VAR_BLD
%token <std::string> RUN_TIME_VAR_HSV

%token <std::string> RUN_TIME_VAR_REMOTE_USER

%token <std::string> RUN_TIME_VAR_TIME
%token <std::string> RUN_TIME_VAR_TIME_DAY
%token <std::string> RUN_TIME_VAR_TIME_EPOCH
%token <std::string> RUN_TIME_VAR_TIME_HOUR
%token <std::string> RUN_TIME_VAR_TIME_MIN
%token <std::string> RUN_TIME_VAR_TIME_MON
%token <std::string> RUN_TIME_VAR_TIME_SEC
%token <std::string> RUN_TIME_VAR_TIME_WDAY
%token <std::string> RUN_TIME_VAR_TIME_YEAR
%token <std::string> RUN_TIME_VAR_XML
%token <std::string> RUN_TIME_VAR_RULE

%token <std::string> CONFIG_SEC_REMOTE_RULES_FAIL_ACTION

%token <std::string> CONFIG_DIR_GEO_DB

%token <std::string> OPERATOR
%token <std::string> OPERATOR_GEOIP
%token <std::string> FREE_TEXT
%token <std::string> ACTION
%token <std::string> ACTION_ACCURACY
%token <std::string> ACTION_EXEC
%token <std::string> ACTION_ALLOW
%token <std::string> ACTION_REDIRECT
%token <std::string> ACTION_SKIP_AFTER
%token <std::string> ACTION_SKIP
%token <std::string> ACTION_AUDIT_LOG
%token <std::string> ACTION_PHASE
%token <std::string> ACTION_SEVERITY
%token <std::string> ACTION_SETENV
%token <std::string> ACTION_SETVAR
%token <std::string> ACTION_SETSID
%token <std::string> ACTION_SETUID
%token <std::string> ACTION_EXPIREVAR
%token <std::string> ACTION_INITCOL
%token <std::string> ACTION_MSG
%token <std::string> ACTION_TAG
%token <std::string> ACTION_REV
%token <std::string> ACTION_VER
%token <std::string> ACTION_MATURITY
%token <std::string> ACTION_XMLNS
%token <std::string> LOG_DATA
%token <std::string> TRANSFORMATION
%token <std::string> ACTION_CTL_AUDIT_ENGINE
%token <std::string> ACTION_CTL_AUDIT_LOG_PARTS
%token <std::string> ACTION_CTL_BDY_JSON
%token <std::string> ACTION_CTL_BDY_XML
%token <std::string> ACTION_CTL_RULE_ENGINE
%token <std::string> ACTION_CTL_FORCE_REQ_BODY_VAR
%token <std::string> CONFIG_SEC_COLLECTION_TIMEOUT

%type <std::vector<Action *> *> actions
%type <std::vector<Variable *> *> variables
%type <Variable *> var
%type <Action *> act
%type <std::vector<Action *> *> actings
%type <Operator *> op


%printer { yyoutput << $$; } <*>;

%%
%start input;


input:
    input line
    | line
    ;

line: expression
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
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_RELEVANT_ONLY
      {
        driver.audit_log->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_OFF
      {
        driver.audit_log->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_ON
      {
        driver.audit_log->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
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
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_SERIAL
      {
        driver.audit_log->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_PARALLEL
      {
        driver.audit_log->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_HTTPS
      {
        driver.audit_log->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }

    /* Upload */
    | CONFIG_UPDLOAD_KEEP_FILES CONFIG_VALUE_ON
      {
        driver.uploadKeepFiles = true;
      }
    | CONFIG_UPDLOAD_KEEP_FILES CONFIG_VALUE_OFF
      {
        driver.uploadKeepFiles = false;
      }
    | CONFIG_UPLOAD_FILE_LIMIT
      {
        driver.uploadFileLimit = strtol($1.c_str(), NULL, 10);
      }
    | CONFIG_UPLOAD_FILE_MODE
      {
        driver.uploadFileMode = strtol($1.c_str(), NULL, 8);
      }
    | CONFIG_UPLOAD_DIR
      {
        driver.uploadDirectory = $1;
      }
    | CONFIG_UPDLOAD_SAVE_TMP_FILES CONFIG_VALUE_ON
      {
        driver.tmpSaveUploadedFiles = true;
      }
    | CONFIG_UPDLOAD_SAVE_TMP_FILES CONFIG_VALUE_OFF
      {
        driver.tmpSaveUploadedFiles = false;
      }
    ;

actings:
    QUOTATION_MARK actions QUOTATION_MARK
      {
        $$ = $2;
      }
    | actions
      {
        $$ = $1;
      }
    ;

op:
    OPERATOR
      {
        Operator *op = Operator::instantiate($1);
        std::string error;
        if (op->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
        $$ = op;
      }
    | OPERATOR_GEOIP
      {
#ifdef WITH_GEOIP
        Operator *op = Operator::instantiate($1);
        std::string error;
        if (op->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
        $$ = op;
#else
        std::stringstream ss;
            ss << "This version of ModSecurity was not compiled with GeoIP support.";
            driver.error(@0, ss.str());
            YYERROR;
#endif  // WITH_GEOIP
      }
    | FREE_TEXT
      {
        std::string text = std::string($1);
        text.pop_back();
        text.erase(0, 1);
        Operator *op = Operator::instantiate("\"@rx " + text + "\"");
        std::string error;
        if (op->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
        $$ = op;
      }

expression:
    audit_log
    | DIRECTIVE variables op actings
      {
        Rule *rule = new Rule(
            /* op */ $3,
            /* variables */ $2,
            /* actions */ $4,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );

        if (driver.addSecRule(rule) == false) {
            YYERROR;
        }
      }
    | DIRECTIVE variables op
      {
        Rule *rule = new Rule(
            /* op */ $3,
            /* variables */ $2,
            /* actions */ NULL,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );

        if (driver.addSecRule(rule) == false) {
            YYERROR;
        }
      }
    | CONFIG_DIR_SEC_ACTION actings
      {
        Rule *rule = new Rule(
            /* op */ NULL,
            /* variables */ NULL,
            /* actions */ $2,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );
        driver.addSecAction(rule);
      }
    | DIRECTIVE_SECRULESCRIPT actings
      {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
    | CONFIG_DIR_SEC_DEFAULT_ACTION actings
      {
        std::vector<Action *> *actions = $2;
        std::vector<Action *> checkedActions;
        int definedPhase = -1;
        int secRuleDefinedPhase = -1;
        for (Action *a : *actions) {
            Phase *phase = dynamic_cast<Phase *>(a);
            if (phase != NULL) {
                definedPhase = phase->m_phase;
                secRuleDefinedPhase = phase->m_secRulesPhase;
                delete phase;
            } else if (a->action_kind == Action::RunTimeOnlyIfMatchKind ||
                a->action_kind == Action::RunTimeBeforeMatchAttemptKind) {
                None *none = dynamic_cast<None *>(a);
                if (none != NULL) {
                    driver.error(@0, "The transformation none is not suitable to be part of the SecDefaultActions");
                    YYERROR;
                }
                checkedActions.push_back(a);
            } else {
                driver.error(@0, "The action '" + a->m_name + "' is not suitable to be part of the SecDefaultActions");
                YYERROR;
            }
        }
        if (definedPhase == -1) {
            definedPhase = modsecurity::ModSecurity::Phases::RequestHeadersPhase;
        }

        if (!driver.defaultActions[definedPhase].empty()) {
            std::stringstream ss;
            ss << "SecDefaultActions can only be placed once per phase and configuration context. Phase ";
            ss << secRuleDefinedPhase;
            ss << " was informed already.";
            driver.error(@0, ss.str());
            YYERROR;
        }

        for (Action *a : checkedActions) {
            driver.defaultActions[definedPhase].push_back(a);
        }

        delete actions;
      }
    | CONFIG_DIR_SEC_MARKER
      {
        driver.addSecMarker(removeBracketsIfNeeded($1));
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_OFF
      {
        driver.secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_ON
      {
        driver.secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_DETC
      {
        driver.secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
    | CONFIG_DIR_REQ_BODY CONFIG_VALUE_ON
      {
        driver.secRequestBodyAccess = true;
      }
    | CONFIG_DIR_REQ_BODY CONFIG_VALUE_OFF
      {
        driver.secRequestBodyAccess = false;
      }
    | CONFIG_DIR_RES_BODY CONFIG_VALUE_ON
      {
        driver.secResponseBodyAccess = true;
      }
    | CONFIG_DIR_RES_BODY CONFIG_VALUE_OFF
      {
        driver.secResponseBodyAccess = false;
      }
    | CONFIG_COMPONENT_SIG
      {
        driver.components.push_back($1);
      }
    | CONFIG_SEC_RULE_REMOVE_BY_ID
      {
        std::string error;
        if (driver.m_exceptions.load($1, &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveById: failed to load:";
            ss << $1;
            ss << ". ";
            ss << error;
            driver.error(@0, ss.str());
            YYERROR;
        }
      }
    /* Debug log: start */
    | CONFIG_DIR_DEBUG_LVL
      {
        if (driver.m_debugLog != NULL) {
          driver.m_debugLog->setDebugLogLevel(atoi($1.c_str()));
        } else {
            std::stringstream ss;
            ss << "Internal error, there is no DebugLog ";
            ss << "object associated with the driver class";
            driver.error(@0, ss.str());
            YYERROR;
        }
      }
    | CONFIG_DIR_DEBUG_LOG
      {
        if (driver.m_debugLog != NULL) {
            driver.m_debugLog->setDebugLogFile($1);
        } else {
            std::stringstream ss;
            ss << "Internal error, there is no DebugLog ";
            ss << "object associated with the driver class";
            driver.error(@0, ss.str());
            YYERROR;
        }
      }
    /* Debug log: end */
    | CONFIG_DIR_GEO_DB
      {
#ifdef WITH_GEOIP
        std::string file = modsecurity::find_resource($1, driver.ref.back());
        if (GeoLookup::getInstance().setDataBase(file) == false) {
            std::stringstream ss;
            ss << "Failed to load the GeoDB from: ";
            ss << file;
            driver.error(@0, ss.str());
            YYERROR;
        }
#else
        std::stringstream ss;
        ss << "This version of ModSecurity was not compiled with GeoIP support.";
        driver.error(@0, ss.str());
        YYERROR;
#endif  // WITH_GEOIP
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
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION CONFIG_VALUE_REJECT
      {
        driver.requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION CONFIG_VALUE_REJECT
      {
        driver.responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION CONFIG_VALUE_ABORT
      {
        driver.remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION CONFIG_VALUE_WARN
      {
        driver.remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
    | CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION
    | CONFIG_DIR_PCRE_MATCH_LIMIT
    | CONGIG_DIR_RESPONSE_BODY_MP
      {
        std::istringstream buf($1);
        std::istream_iterator<std::string> beg(buf), end;
        std::set<std::string> tokens(beg, end);
        for (std::set<std::string>::iterator it=tokens.begin();
            it!=tokens.end(); ++it)
        {
            driver.m_responseBodyTypeToBeInspected.insert(*it);
        }
      }
    | CONFIG_XML_EXTERNAL_ENTITY CONFIG_VALUE_OFF
      {
        driver.secXMLExternalEntity = false;
      }
    | CONFIG_XML_EXTERNAL_ENTITY CONFIG_VALUE_ON
      {
        driver.secXMLExternalEntity = true;
      }
    | CONGIG_DIR_SEC_TMP_DIR
    | CONGIG_DIR_SEC_DATA_DIR
    | CONGIG_DIR_SEC_ARG_SEP
    | CONGIG_DIR_SEC_COOKIE_FORMAT
    | CONGIG_DIR_SEC_STATUS_ENGINE
    | CONFIG_DIR_UNICODE_MAP_FILE
    | CONFIG_SEC_COLLECTION_TIMEOUT
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
        CHECK_VARIATION(&) { var = new Count(new Variable(name, Variable::VariableKind::DirectVariable)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Variable(name, Variable::VariableKind::DirectVariable)); }
        if (!var) { var = new Variable(name, Variable::VariableKind::DirectVariable); }
        $$ = var;
      }
    | VARIABLE_STATUS
      {
        std::string name($1);
        name.pop_back();
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Variable(name, Variable::VariableKind::DirectVariable)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Variable(name, Variable::VariableKind::DirectVariable)); }
        if (!var) { var = new Variable(name, Variable::VariableKind::DirectVariable); }
        $$ = var;
      }
    | VARIABLE_COL
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Variable(name, Variable::VariableKind::CollectionVarible)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Variable(name, Variable::VariableKind::CollectionVarible)); }
        if (!var) { var = new Variable(name, Variable::VariableKind::CollectionVarible); }
        $$ = var;
      }
    | VARIABLE_TX
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new Tx(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Tx(name)); }
        if (!var) { var = new Tx(name); }
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
    | RUN_TIME_VAR_REMOTE_USER
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new RemoteUser(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new RemoteUser(name)); }
        if (!var) { var = new RemoteUser(name); }
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
    | RUN_TIME_VAR_XML
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(new XML(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(new XML(name)); }
        if (!var) { var = new XML(name); }
        $$ = var;
      }
    | RUN_TIME_VAR_RULE
      {
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(
            new modsecurity::Variables::Rule(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(
            new modsecurity::Variables::Rule(name)); }
        if (!var) { var = new modsecurity::Variables::Rule(name); }
        $$ = var;
      }
    ;

act:
    ACTION
      {
        std::string error;
        $$ = Action::instantiate($1);

        if ($$->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }
      }
    | ACTION_ALLOW
      {
        std::string error;
        $$ = new Allow($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_PHASE
      {
        std::string error;
        $$ = new Phase($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_INITCOL
      {
        std::string error;
        $$ = new InitCol($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | TRANSFORMATION
      {
        std::string error;
        $$ = Transformation::instantiate($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_ACCURACY
      {
        std::string error;
        $$ = new Accuracy($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_EXEC
      {
        /*

        TODO: exec is not implemented yet.

        std::string error;
        Allow *exec = new Exec($1);

        if (exec->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }

        $$ = exec;
        */
        $$ = Action::instantiate($1);
      }
    | ACTION_REDIRECT
      {
        std::string error;
        $$ =  new Redirect($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_SEVERITY
      {
        std::string error;
        $$ =  new Severity($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_EXPIREVAR
      {
        std::string error;
        $$ = Action::instantiate($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_SETENV
      {
        /*

        TODO: setEnv is not implemented yet.

        std::string error;
        SetEnv *setEnv = new s($1);

        if (setEnv->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }

        $$ = setEnv;
        */
        $$ = Action::instantiate($1);
      }
    | ACTION_SETSID
      {
        std::string error;
        SetSID *setSID = new SetSID($1);

        if (setSID->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }

        $$ = setSID;
      }
    | ACTION_SETUID
      {
        std::string error;
        SetUID *setUID = new SetUID($1);

        if (setUID->init(&error) == false) {
            driver.parserError << error;
            YYERROR;
        }

        $$ = setUID;
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
    | ACTION_SKIP
      {
        std::string error;
        $$ = new modsecurity::actions::Skip($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_SKIP_AFTER
      {
        std::string error;
        $$ = new modsecurity::actions::SkipAfter($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_AUDIT_LOG
      {
        std::string error;
        $$ = new modsecurity::actions::AuditLog($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | LOG_DATA
      {
        std::string error;
        $$ = new LogData($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_MSG
      {
        std::string error;
        $$ = new Msg($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_TAG
      {
        std::string error;
        $$ = new Tag($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_REV
      {
        std::string error;
        $$ = new Rev($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_VER
      {
        std::string error;
        $$ = new Ver($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_MATURITY
      {
        std::string error;
        $$ = new Maturity($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_XMLNS
      {
        std::string error;
        $$ = new modsecurity::actions::XmlNS($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_CTL_BDY_XML
      {
        $$ = new modsecurity::actions::CtlRequestBodyProcessorXML($1);
      }
    | ACTION_CTL_BDY_JSON
      {
        $$ = new modsecurity::actions::CtlRequestBodyProcessorJSON($1);
      }
    | ACTION_CTL_AUDIT_LOG_PARTS
      {
        std::string error;
        $$ = new CtlAuditLogParts($1);
        if ($$->init(&error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_ON
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_OFF
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_ON
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_OFF
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_DETC
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_ON
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_OFF
      {
        $$ = Action::instantiate($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_RELEVANT_ONLY
      {
        $$ = Action::instantiate($1);
      }
    ;

actions:
    actions COMMA act
      {
        std::vector<Action *> *a = $1;
        a->push_back($3);
        $$ = $1;
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
