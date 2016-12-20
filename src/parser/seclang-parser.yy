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


#include "src/actions/accuracy.h"
#include "src/actions/audit_log.h"
#include "src/actions/capture.h"
#include "src/actions/chain.h"
#include "src/actions/ctl/audit_log_parts.h"
#include "src/actions/ctl/request_body_access.h"
#include "src/actions/ctl/request_body_processor_json.h"
#include "src/actions/ctl/request_body_processor_xml.h"
#include "src/actions/ctl/rule_remove_by_id.h"
#include "src/actions/ctl/rule_remove_target_by_id.h"
#include "src/actions/ctl/rule_remove_target_by_tag.h"
#include "src/actions/data/status.h"
#include "src/actions/disruptive/allow.h"
#include "src/actions/disruptive/block.h"
#include "src/actions/disruptive/deny.h"
#include "src/actions/disruptive/pass.h"
#include "src/actions/disruptive/redirect.h"
#include "src/actions/init_col.h"
#include "src/actions/log_data.h"
#include "src/actions/log.h"
#include "src/actions/maturity.h"
#include "src/actions/msg.h"
#include "src/actions/multi_match.h"
#include "src/actions/no_audit_log.h"
#include "src/actions/no_log.h"
#include "src/actions/phase.h"
#include "src/actions/rev.h"
#include "src/actions/rule_id.h"
#include "src/actions/set_sid.h"
#include "src/actions/set_uid.h"
#include "src/actions/set_var.h"
#include "src/actions/severity.h"
#include "src/actions/skip_after.h"
#include "src/actions/skip.h"
#include "src/actions/tag.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/ver.h"
#include "src/actions/xmlns.h"

#include "src/actions/transformations/hex_encode.h"
#include "src/actions/transformations/parity_even_7bit.h"
#include "src/actions/transformations/utf8_to_unicode.h"
#include "src/actions/transformations/parity_zero_7bit.h"
#include "src/actions/transformations/sql_hex_decode.h"
#include "src/actions/transformations/replace_comments.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/url_decode.h"
#include "src/actions/transformations/lower_case.h"
#include "src/actions/transformations/hex_decode.h"
#include "src/actions/transformations/url_encode.h"
#include "src/actions/transformations/js_decode.h"
#include "src/actions/transformations/url_decode_uni.h"
#include "src/actions/transformations/parity_odd_7bit.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/transformations/trim_right.h"
#include "src/actions/transformations/escape_seq_decode.h"
#include "src/actions/transformations/base64_decode_ext.h"
#include "src/actions/transformations/base64_decode.h"
#include "src/actions/transformations/trim.h"
#include "src/actions/transformations/cmd_line.h"
#include "src/actions/transformations/replace_nulls.h"
#include "src/actions/transformations/md5.h"
#include "src/actions/transformations/length.h"
#include "src/actions/transformations/sha1.h"
#include "src/actions/transformations/compress_whitespace.h"
#include "src/actions/transformations/normalise_path_win.h"
#include "src/actions/transformations/remove_nulls.h"
#include "src/actions/transformations/remove_comments.h"
#include "src/actions/transformations/normalise_path.h"
#include "src/actions/transformations/html_entity_decode.h"
#include "src/actions/transformations/trim_left.h"
#include "src/actions/transformations/remove_comments_char.h"
#include "src/actions/transformations/base64_encode.h"
#include "src/actions/transformations/remove_whitespace.h"
#include "src/actions/transformations/css_decode.h"



#include "modsecurity/audit_log.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/rule.h"
#include "src/operators/operator.h"
#include "src/utils/geo_lookup.h"
#include "src/utils/string.h"
#include "src/utils/system.h"
#include "src/variables/xml.h"
#include "src/variables/duration.h"
#include "src/variables/env.h"
#include "src/variables/highest_severity.h"
#include "src/variables/modsec_build.h"
#include "src/variables/remote_user.h"
#include "src/variables/rule.h"
#include "src/variables/time.h"
#include "src/variables/time_day.h"
#include "src/variables/time_epoch.h"
#include "src/variables/time_hour.h"
#include "src/variables/time_min.h"
#include "src/variables/time_mon.h"
#include "src/variables/time_sec.h"
#include "src/variables/time_wday.h"
#include "src/variables/time_year.h"
#include "src/variables/tx.h"
#include "src/variables/variations/count.h"
#include "src/variables/variations/exclusion.h"


using modsecurity::ModSecurity;
using modsecurity::Rule;
using modsecurity::Utils::GeoLookup;
using modsecurity::Variables::Duration;
using modsecurity::Variables::Env;
using modsecurity::Variables::HighestSeverity;
using modsecurity::Variables::ModsecBuild;
using modsecurity::Variables::RemoteUser;
using modsecurity::Variables::TimeDay;
using modsecurity::Variables::TimeEpoch;
using modsecurity::Variables::TimeHour;
using modsecurity::Variables::TimeMin;
using modsecurity::Variables::TimeMon;
using modsecurity::Variables::TimeSec;
using modsecurity::Variables::TimeWDay;
using modsecurity::Variables::TimeYear;
using modsecurity::Variables::Time;
using modsecurity::Variables::Tx;
using modsecurity::Variables::Variable;
using modsecurity::Variables::Variations::Count;
using modsecurity::Variables::Variations::Exclusion;
using modsecurity::Variables::XML;

using namespace modsecurity;

using modsecurity::operators::Operator;



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


#define ACTION_NOT_SUPPORTED(a, b) \
    driver.error(b, "Action: " + std::string(a) + " is not yet supported."); \
    YYERROR;

#define ACTION_INIT(a, b) \
    std::string error; \
    if (a->init(&error) == false) { \
        driver.error(b, error); \
        YYERROR; \
    }

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
#include "src/parser/driver.h"
}
%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  COMMA    ","
  PIPE
;

%token <std::string> ACTION_ACCURACY
%token <std::string> ACTION_ALLOW
%token <std::string> ACTION_APPEND
%token <std::string> ACTION_AUDIT_LOG
%token <std::string> ACTION_BLOCK
%token <std::string> ACTION_CAPTURE
%token <std::string> ACTION_CHAIN
%token <std::string> ACTION_CTL_AUDIT_ENGINE
%token <std::string> ACTION_CTL_AUDIT_LOG_PARTS
%token <std::string> ACTION_CTL_BDY_JSON
%token <std::string> ACTION_CTL_BDY_XML
%token <std::string> ACTION_CTL_FORCE_REQ_BODY_VAR
%token <std::string> ACTION_CTL_REQUEST_BODY_ACCESS
%token <std::string> ACTION_CTL_RULE_ENGINE
%token <std::string> ACTION_CTL_RULE_REMOVE_BY_ID
%token <std::string> ACTION_CTL_RULE_REMOVE_TARGET_BY_ID
%token <std::string> ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG
%token <std::string> ACTION_DENY
%token <std::string> ACTION_DEPRECATE_VAR
%token <std::string> ACTION_DROP
%token <std::string> ACTION_EXEC
%token <std::string> ACTION_EXPIRE_VAR
%token <std::string> ACTION_ID
%token <std::string> ACTION_INITCOL
%token <std::string> ACTION_LOG_DATA
%token <std::string> ACTION_LOG
%token <std::string> ACTION_MATURITY
%token <std::string> ACTION_MSG
%token <std::string> ACTION_MULTI_MATCH
%token <std::string> ACTION_NO_AUDIT_LOG
%token <std::string> ACTION_NO_LOG
%token <std::string> ACTION_PASS
%token <std::string> ACTION_PAUSE
%token <std::string> ACTION_PHASE
%token <std::string> ACTION_PREPEND
%token <std::string> ACTION_PROXY
%token <std::string> ACTION_REDIRECT
%token <std::string> ACTION_REV
%token <std::string> ACTION_SANATISE_ARG
%token <std::string> ACTION_SANATISE_MATCHED
%token <std::string> ACTION_SANATISE_MATCHED_BYTES
%token <std::string> ACTION_SANATISE_REQUEST_HEADER
%token <std::string> ACTION_SANATISE_RESPONSE_HEADER
%token <std::string> ACTION_SETENV
%token <std::string> ACTION_SETRSC
%token <std::string> ACTION_SETSID
%token <std::string> ACTION_SETUID
%token <std::string> ACTION_SETVAR
%token <std::string> ACTION_SEVERITY
%token <std::string> ACTION_SKIP
%token <std::string> ACTION_SKIP_AFTER
%token <std::string> ACTION_STATUS
%token <std::string> ACTION_TAG
%token <std::string> ACTION_VER
%token <std::string> ACTION_XMLNS

%token <std::string> ACTION_TRANSFORMATION_CMD_LINE
%token <std::string> ACTION_TRANSFORMATION_COMPRESS_WHITESPACE
%token <std::string> ACTION_TRANSFORMATION_CSS_DECODE
%token <std::string> ACTION_TRANSFORMATION_HEX_ENCODE
%token <std::string> ACTION_TRANSFORMATION_HTML_ENTITY_DECODE
%token <std::string> ACTION_TRANSFORMATION_JS_DECODE
%token <std::string> ACTION_TRANSFORMATION_LENGTH
%token <std::string> ACTION_TRANSFORMATION_LOWERCASE
%token <std::string> ACTION_TRANSFORMATION_MD5
%token <std::string> ACTION_TRANSFORMATION_NONE
%token <std::string> ACTION_TRANSFORMATION_NORMALISE_PATH
%token <std::string> ACTION_TRANSFORMATION_NORMALISE_PATH_WIN
%token <std::string> ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT
%token <std::string> ACTION_TRANSFORMATION_PARITY_ODD_7_BIT
%token <std::string> ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT
%token <std::string> ACTION_TRANSFORMATION_REMOVE_COMMENTS
%token <std::string> ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR
%token <std::string> ACTION_TRANSFORMATION_REMOVE_NULLS
%token <std::string> ACTION_TRANSFORMATION_REMOVE_WHITESPACE
%token <std::string> ACTION_TRANSFORMATION_REPLACE_COMMENTS
%token <std::string> ACTION_TRANSFORMATION_REPLACE_NULLS
%token <std::string> ACTION_TRANSFORMATION_SHA1
%token <std::string> ACTION_TRANSFORMATION_SQL_HEX_DECODE
%token <std::string> ACTION_TRANSFORMATION_TRIM
%token <std::string> ACTION_TRANSFORMATION_URL_DECODE
%token <std::string> ACTION_TRANSFORMATION_URL_DECODE_UNI
%token <std::string> ACTION_TRANSFORMATION_UTF8_TO_UNICODE



%token <std::string> CONFIG_COMPONENT_SIG
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
%token <std::string> CONFIG_DIR_GEO_DB
%token <std::string> CONFIG_DIR_PCRE_MATCH_LIMIT
%token <std::string> CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION
%token <std::string> CONFIG_DIR_REQ_BODY
%token <std::string> CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT
%token <std::string> CONFIG_DIR_REQ_BODY_LIMIT_ACTION
%token <std::string> CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT
%token <std::string> CONFIG_DIR_RES_BODY
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT
%token <std::string> CONFIG_DIR_RES_BODY_LIMIT_ACTION
%token <std::string> CONFIG_DIR_RULE_ENG
%token <std::string> CONFIG_DIR_SEC_ACTION
%token <std::string> CONFIG_DIR_SEC_DEFAULT_ACTION
%token <std::string> CONFIG_DIR_SEC_MARKER
%token <std::string> CONFIG_DIR_UNICODE_MAP_FILE
%token <std::string> CONFIG_SEC_COLLECTION_TIMEOUT
%token <std::string> CONFIG_SEC_REMOTE_RULES_FAIL_ACTION
%token <std::string> CONFIG_SEC_RULE_REMOVE_BY_ID
%token <std::string> CONFIG_UPDLOAD_KEEP_FILES
%token <std::string> CONFIG_UPDLOAD_SAVE_TMP_FILES
%token <std::string> CONFIG_UPLOAD_DIR
%token <std::string> CONFIG_UPLOAD_FILE_LIMIT
%token <std::string> CONFIG_UPLOAD_FILE_MODE
%token <std::string> CONFIG_VALUE_ABORT
%token <std::string> CONFIG_VALUE_DETC
%token <std::string> CONFIG_VALUE_HTTPS
%token <std::string> CONFIG_VALUE_OFF
%token <std::string> CONFIG_VALUE_ON
%token <std::string> CONFIG_VALUE_PARALLEL
%token <std::string> CONFIG_VALUE_PROCESS_PARTIAL
%token <std::string> CONFIG_VALUE_REJECT
%token <std::string> CONFIG_VALUE_RELEVANT_ONLY
%token <std::string> CONFIG_VALUE_SERIAL
%token <std::string> CONFIG_VALUE_WARN
%token <std::string> CONFIG_XML_EXTERNAL_ENTITY
%token <std::string> CONGIG_DIR_RESPONSE_BODY_MP
%token <std::string> CONGIG_DIR_SEC_ARG_SEP
%token <std::string> CONGIG_DIR_SEC_COOKIE_FORMAT
%token <std::string> CONGIG_DIR_SEC_DATA_DIR
%token <std::string> CONGIG_DIR_SEC_STATUS_ENGINE
%token <std::string> CONGIG_DIR_SEC_TMP_DIR
%token <std::string> DIRECTIVE
%token <std::string> DIRECTIVE_SECRULESCRIPT
%token <std::string> FREE_TEXT

%token <std::string> OPERATOR
%token <std::string> OPERATOR_GEOIP
%token <std::string> QUOTATION_MARK
%token <std::string> RUN_TIME_VAR_BLD
%token <std::string> RUN_TIME_VAR_DUR
%token <std::string> RUN_TIME_VAR_ENV
%token <std::string> RUN_TIME_VAR_HSV
%token <std::string> RUN_TIME_VAR_REMOTE_USER
%token <std::string> RUN_TIME_VAR_RULE
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
%token <std::string> VARIABLE
%token <std::string> VARIABLE_COL
%token <std::string> VARIABLE_STATUS
%token <std::string> VARIABLE_TX

%type <actions::Action *> act
%type <std::vector<actions::Action *> *> actings
%type <std::vector<actions::Action *> *> actions

%type <std::vector<Variable *> *> variables
%type <Operator *> op
%type <Variable *> var


%printer { yyoutput << $$; } <*>;

%%
%start input;


input:
    END
      {
        return 0;
      }
    | input line
    | line
    ;

line: expression
    ;

audit_log:
    /* SecAuditLogDirMode */
    CONFIG_DIR_AUDIT_DIR_MOD
      {
        driver.m_auditLog->setStorageDirMode(strtol($1.c_str(), NULL, 8));
      }

    /* SecAuditLogStorageDir */
    | CONFIG_DIR_AUDIT_DIR
      {
        driver.m_auditLog->setStorageDir($1);
      }

    /* SecAuditEngine */
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_RELEVANT_ONLY
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_OFF
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
    | CONFIG_DIR_AUDIT_ENG CONFIG_VALUE_ON
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }

    /* SecAuditLogFileMode */
    | CONFIG_DIR_AUDIT_FLE_MOD
      {
        driver.m_auditLog->setFileMode(strtol($1.c_str(), NULL, 8));
      }

    /* SecAuditLog2 */
    | CONFIG_DIR_AUDIT_LOG2
      {
        driver.m_auditLog->setFilePath2($1);
      }

    /* SecAuditLogParts */
    | CONFIG_DIR_AUDIT_LOG_P
      {
        driver.m_auditLog->setParts($1);
      }

    /* SecAuditLog */
    | CONFIG_DIR_AUDIT_LOG
      {
        driver.m_auditLog->setFilePath1($1);
      }

    /* SecAuditLogRelevantStatus */
    | CONFIG_DIR_AUDIT_STS
      {
        std::string relevant_status($1);
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }

    /* SecAuditLogType */
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_SERIAL
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_PARALLEL
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
    | CONFIG_DIR_AUDIT_TPE CONFIG_VALUE_HTTPS
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }

    /* Upload */
    | CONFIG_UPDLOAD_KEEP_FILES CONFIG_VALUE_ON
      {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
    | CONFIG_UPDLOAD_KEEP_FILES CONFIG_VALUE_OFF
      {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
    | CONFIG_UPLOAD_FILE_LIMIT
      {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol($1.c_str(), NULL, 10);
      }
    | CONFIG_UPLOAD_FILE_MODE
      {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol($1.c_str(), NULL, 8);
      }
    | CONFIG_UPLOAD_DIR
      {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = $1;
      }
    | CONFIG_UPDLOAD_SAVE_TMP_FILES CONFIG_VALUE_ON
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
    | CONFIG_UPDLOAD_SAVE_TMP_FILES CONFIG_VALUE_OFF
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
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

actions:
    actions COMMA act
      {
        std::vector<actions::Action *> *a = $1;
        ACTION_INIT($3, @0)
        a->push_back($3);
        $$ = $1;
      }
    | act
      {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>;
        ACTION_INIT($1, @0)
        a->push_back($1);
        $$ = a;
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
    ;

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
        std::vector<actions::Action *> *actions = $2;
        std::vector<actions::Action *> checkedActions;
        int definedPhase = -1;
        int secRuleDefinedPhase = -1;
        for (actions::Action *a : *actions) {
            actions::Phase *phase = dynamic_cast<actions::Phase *>(a);
            if (phase != NULL) {
                definedPhase = phase->m_phase;
                secRuleDefinedPhase = phase->m_secRulesPhase;
                delete phase;
            } else if (a->action_kind == actions::Action::RunTimeOnlyIfMatchKind ||
                a->action_kind == actions::Action::RunTimeBeforeMatchAttemptKind) {
                actions::transformations::None *none = dynamic_cast<actions::transformations::None *>(a);
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
            definedPhase = modsecurity::Phases::RequestHeadersPhase;
        }

        if (!driver.defaultActions[definedPhase].empty()) {
            std::stringstream ss;
            ss << "SecDefaultActions can only be placed once per phase and configuration context. Phase ";
            ss << secRuleDefinedPhase;
            ss << " was informed already.";
            driver.error(@0, ss.str());
            YYERROR;
        }

        for (actions::Action *a : checkedActions) {
            driver.defaultActions[definedPhase].push_back(a);
        }

        delete actions;
      }
    | CONFIG_DIR_SEC_MARKER
      {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded($1));
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_OFF
      {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_ON
      {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
    | CONFIG_DIR_RULE_ENG CONFIG_VALUE_DETC
      {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
    | CONFIG_DIR_REQ_BODY CONFIG_VALUE_ON
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
    | CONFIG_DIR_REQ_BODY CONFIG_VALUE_OFF
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
    | CONFIG_DIR_RES_BODY CONFIG_VALUE_ON
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
    | CONFIG_DIR_RES_BODY CONFIG_VALUE_OFF
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
    | CONFIG_COMPONENT_SIG
      {
        driver.m_components.push_back($1);
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
            std::string error;
            driver.m_debugLog->setDebugLogFile($1, &error);
            if (error.size() > 0) {
                std::stringstream ss;
                ss << "Failed to start DebugLog: " << error;
                driver.error(@0, ss.str());
                YYERROR;
            }
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
        std::string file = modsecurity::utils::find_resource($1,
            driver.ref.back());
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
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT
      {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT
      {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi($1.c_str());
      }
    | CONFIG_DIR_RES_BODY_LIMIT
      {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi($1.c_str());
      }
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_REQ_BODY_LIMIT_ACTION CONFIG_VALUE_REJECT
      {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION CONFIG_VALUE_PROCESS_PARTIAL
      {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
    | CONFIG_DIR_RES_BODY_LIMIT_ACTION CONFIG_VALUE_REJECT
      {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION CONFIG_VALUE_ABORT
      {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
    | CONFIG_SEC_REMOTE_RULES_FAIL_ACTION CONFIG_VALUE_WARN
      {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
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
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
    | CONFIG_XML_EXTERNAL_ENTITY CONFIG_VALUE_ON
      {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
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
        CHECK_VARIATION(&) { var = new Count(new Variable(name, Variable::VariableKind::DirectVariable)); }
        CHECK_VARIATION(!) { var = new Exclusion(new Variable(name, Variable::VariableKind::DirectVariable)); }
        if (!var) { var = new Variable(name, Variable::VariableKind::DirectVariable); }
        $$ = var;
    /*
        std::string name($1);
        CHECK_VARIATION_DECL
        CHECK_VARIATION(&) { var = new Count(
            new modsecurity::Variables::Rule(name)); }
        CHECK_VARIATION(!) { var = new Exclusion(
            new modsecurity::Variables::Rule(name)); }
        if (!var) { var = new modsecurity::Variables::Rule(name); }
        $$ = var;
      */
      }
    ;

act:
    ACTION_ACCURACY
      {
        $$ = new actions::Accuracy($1);
      }
    | ACTION_ALLOW
      {
        $$ = new actions::disruptive::Allow($1);
      }
    | ACTION_APPEND
      {
        ACTION_NOT_SUPPORTED("Append", @0);
      }
    | ACTION_AUDIT_LOG
      {
        $$ = new modsecurity::actions::AuditLog($1);
      }
    | ACTION_BLOCK
      {
        $$ = new actions::disruptive::Block($1);
      }
    | ACTION_CAPTURE
      {
        $$ = new actions::Capture($1);
      }
    | ACTION_CHAIN
      {
        $$ = new actions::Chain($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_RELEVANT_ONLY
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_AUDIT_LOG_PARTS
      {
        $$ = new actions::ctl::AuditLogParts($1);
      }
    | ACTION_CTL_BDY_JSON
      {
        $$ = new modsecurity::actions::ctl::RequestBodyProcessorJSON($1);
      }
    | ACTION_CTL_BDY_XML
      {
        $$ = new modsecurity::actions::ctl::RequestBodyProcessorXML($1);
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_REQUEST_BODY_ACCESS CONFIG_VALUE_ON
      {
        $$ = new modsecurity::actions::ctl::RequestBodyAccess($1 + "true");
      }
    | ACTION_CTL_REQUEST_BODY_ACCESS CONFIG_VALUE_OFF
      {
        $$ = new modsecurity::actions::ctl::RequestBodyAccess($1 + "false");
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_DETC
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_CTL_RULE_REMOVE_BY_ID
      {
        $$ = new modsecurity::actions::ctl::RuleRemoveById($1);
      }
    | ACTION_CTL_RULE_REMOVE_TARGET_BY_ID
      {
        $$ = new modsecurity::actions::ctl::RuleRemoveTargetById($1);
      }
    | ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG
      {
        $$ = new modsecurity::actions::ctl::RuleRemoveTargetByTag($1);
      }
    | ACTION_DENY
      {
        $$ = new modsecurity::actions::disruptive::Deny($1);
      }
    | ACTION_DEPRECATE_VAR
      {
        ACTION_NOT_SUPPORTED("DeprecateVar", @0);
      }
    | ACTION_DROP
      {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_EXEC
      {
        ACTION_NOT_SUPPORTED("Exec", @0);
      }
    | ACTION_EXPIRE_VAR
      {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        $$ = new actions::Action($1);
      }
    | ACTION_ID
      {
        $$ = new actions::RuleId($1);
      }
    | ACTION_INITCOL
      {
        $$ = new actions::InitCol($1);
      }
    | ACTION_LOG_DATA
      {
        $$ = new actions::LogData($1);
      }
    | ACTION_LOG
      {
        $$ = new actions::Log($1);
      }
    | ACTION_MATURITY
      {
        $$ = new actions::Maturity($1);
      }
    | ACTION_MSG
      {
        $$ = new actions::Msg($1);
      }
    | ACTION_MULTI_MATCH
      {
        $$ = new actions::MultiMatch($1);
      }
    | ACTION_NO_AUDIT_LOG
      {
        $$ = new actions::NoAuditLog($1);
      }
    | ACTION_NO_LOG
      {
        $$ = new actions::NoLog($1);
      }
    | ACTION_PASS
      {
        $$ = new actions::disruptive::Pass($1);
      }
    | ACTION_PAUSE
      {
        ACTION_NOT_SUPPORTED("Pause", @0);
      }
    | ACTION_PHASE
      {
        $$ = new actions::Phase($1);
      }
    | ACTION_PREPEND
      {
        ACTION_NOT_SUPPORTED("Prepend", @0);
      }
    | ACTION_PROXY
      {
        ACTION_NOT_SUPPORTED("Proxy", @0);
      }
    | ACTION_REDIRECT
      {
        $$ = new actions::disruptive::Redirect($1);
      }
    | ACTION_REV
      {
        $$ = new actions::Rev($1);
      }
    | ACTION_SANATISE_ARG
      {
        ACTION_NOT_SUPPORTED("SanatiseArg", @0);
      }
    | ACTION_SANATISE_MATCHED
      {
        ACTION_NOT_SUPPORTED("SanatiseMatched", @0);
      }
    | ACTION_SANATISE_MATCHED_BYTES
      {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", @0);
      }
    | ACTION_SANATISE_REQUEST_HEADER
      {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", @0);
      }
    | ACTION_SANATISE_RESPONSE_HEADER
      {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", @0);
      }
    | ACTION_SETENV
      {
        ACTION_NOT_SUPPORTED("SetEnv", @0);
      }
    | ACTION_SETRSC
      {
        ACTION_NOT_SUPPORTED("SetRSC", @0);
      }
    | ACTION_SETSID
      {
        $$ = new modsecurity::actions::SetSID($1);
      }
    | ACTION_SETUID
      {
        $$ = new modsecurity::actions::SetUID($1);
      }
    | ACTION_SETVAR
      {
        $$ = new modsecurity::actions::SetVar($1);
      }
    | ACTION_SEVERITY
      {
        $$ =  new modsecurity::actions::Severity($1);
      }
    | ACTION_SKIP
      {
        $$ = new modsecurity::actions::Skip($1);
      }
    | ACTION_SKIP_AFTER
      {
        $$ = new modsecurity::actions::SkipAfter($1);
      }
    | ACTION_STATUS
      {
        $$ = new modsecurity::actions::data::Status($1);
      }
    | ACTION_TAG
      {
        $$ = new modsecurity::actions::Tag($1);
      }
    | ACTION_VER
      {
        $$ = new modsecurity::actions::Ver($1);
      }
    | ACTION_XMLNS
      {
        $$ = new modsecurity::actions::XmlNS($1);
      }
    | ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT
      {
        $$ = new modsecurity::actions::transformations::ParityZero7bit($1);
      }
    | ACTION_TRANSFORMATION_PARITY_ODD_7_BIT
      {
        $$ = new modsecurity::actions::transformations::ParityOdd7bit($1);
      }
    | ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT
      {
        $$ = new modsecurity::actions::transformations::ParityEven7bit($1);
      }
    | ACTION_TRANSFORMATION_SQL_HEX_DECODE
      {
        $$ = new modsecurity::actions::transformations::SqlHexDecode($1);
      }
    | ACTION_TRANSFORMATION_CMD_LINE
      {
        $$ = new modsecurity::actions::transformations::CmdLine($1);
      }
    | ACTION_TRANSFORMATION_SHA1
      {
        $$ = new modsecurity::actions::transformations::Sha1($1);
      }
    | ACTION_TRANSFORMATION_MD5
      {
        $$ = new modsecurity::actions::transformations::Md5($1);
      }
    | ACTION_TRANSFORMATION_HEX_ENCODE
      {
        $$ = new modsecurity::actions::transformations::HexEncode($1);
      }
    | ACTION_TRANSFORMATION_LOWERCASE
      {
        $$ = new modsecurity::actions::transformations::LowerCase($1);
      }
    | ACTION_TRANSFORMATION_URL_DECODE_UNI
      {
        $$ = new modsecurity::actions::transformations::UrlDecodeUni($1);
      }
    | ACTION_TRANSFORMATION_URL_DECODE
      {
        $$ = new modsecurity::actions::transformations::UrlDecode($1);
      }
    | ACTION_TRANSFORMATION_NONE
      {
        $$ = new modsecurity::actions::transformations::None($1);
      }
    | ACTION_TRANSFORMATION_COMPRESS_WHITESPACE
      {
        $$ = new modsecurity::actions::transformations::CompressWhitespace($1);
      }
    | ACTION_TRANSFORMATION_REMOVE_WHITESPACE
      {
        $$ = new modsecurity::actions::transformations::RemoveWhitespace($1);
      }
    | ACTION_TRANSFORMATION_REPLACE_NULLS
      {
        $$ = new modsecurity::actions::transformations::ReplaceNulls($1);
      }
    | ACTION_TRANSFORMATION_REMOVE_NULLS
      {
        $$ = new modsecurity::actions::transformations::RemoveNulls($1);
      }
    | ACTION_TRANSFORMATION_HTML_ENTITY_DECODE
      {
        $$ = new modsecurity::actions::transformations::HtmlEntityDecode($1);
      }
    | ACTION_TRANSFORMATION_JS_DECODE
      {
        $$ = new modsecurity::actions::transformations::JsDecode($1);
      }
    | ACTION_TRANSFORMATION_CSS_DECODE
      {
        $$ = new modsecurity::actions::transformations::CssDecode($1);
      }
    | ACTION_TRANSFORMATION_TRIM
      {
        $$ = new modsecurity::actions::transformations::Trim($1);
      }
    | ACTION_TRANSFORMATION_NORMALISE_PATH_WIN
      {
        $$ = new modsecurity::actions::transformations::NormalisePathWin($1);
      }
    | ACTION_TRANSFORMATION_NORMALISE_PATH
      {
        $$ = new modsecurity::actions::transformations::NormalisePath($1);
      }
    | ACTION_TRANSFORMATION_LENGTH
      {
        $$ = new modsecurity::actions::transformations::Length($1);
      }
    | ACTION_TRANSFORMATION_UTF8_TO_UNICODE
      {
        $$ = new modsecurity::actions::transformations::Utf8ToUnicode($1);
      }
    | ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR
      {
        $$ = new modsecurity::actions::transformations::RemoveCommentsChar($1);
      }
    | ACTION_TRANSFORMATION_REMOVE_COMMENTS
      {
        $$ = new modsecurity::actions::transformations::RemoveComments($1);
      }
    | ACTION_TRANSFORMATION_REPLACE_COMMENTS
      {
        $$ = new modsecurity::actions::transformations::ReplaceComments($1);
      }
    ;

%%

void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
