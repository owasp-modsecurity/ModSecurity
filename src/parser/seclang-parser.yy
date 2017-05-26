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

#include "src/operators/begins_with.h"
#include "src/operators/contains.h"
#include "src/operators/contains_word.h"
#include "src/operators/detect_sqli.h"
#include "src/operators/detect_xss.h"
#include "src/operators/ends_with.h"
#include "src/operators/eq.h"
#include "src/operators/fuzzy_hash.h"
#include "src/operators/ge.h"
#include "src/operators/geo_lookup.h"
#include "src/operators/gsblookup.h"
#include "src/operators/gt.h"
#include "src/operators/inspect_file.h"
#include "src/operators/ip_match_f.h"
#include "src/operators/ip_match_from_file.h"
#include "src/operators/ip_match.h"
#include "src/operators/le.h"
#include "src/operators/lt.h"
#include "src/operators/no_match.h"
#include "src/operators/operator.h"
#include "src/operators/pm_f.h"
#include "src/operators/pm_from_file.h"
#include "src/operators/pm.h"
#include "src/operators/rbl.h"
#include "src/operators/rsub.h"
#include "src/operators/rx.h"
#include "src/operators/str_eq.h"
#include "src/operators/str_match.h"
#include "src/operators/unconditional_match.h"
#include "src/operators/validate_byte_range.h"
#include "src/operators/validate_dtd.h"
#include "src/operators/validate_hash.h"
#include "src/operators/validate_schema.h"
#include "src/operators/validate_url_encoding.h"
#include "src/operators/validate_utf8_encoding.h"
#include "src/operators/verify_cc.h"
#include "src/operators/verify_cpf.h"
#include "src/operators/verify_ssn.h"
#include "src/operators/within.h"


#include "modsecurity/audit_log.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/rule.h"
#include "src/operators/operator.h"
#include "src/utils/geo_lookup.h"
#include "src/utils/string.h"
#include "src/utils/system.h"
#include "src/variables/args_combined_size.h"
#include "src/variables/args_get.h"
#include "src/variables/args_get_names.h"
#include "src/variables/args.h"
#include "src/variables/args_names.h"
#include "src/variables/args_post.h"
#include "src/variables/args_post_names.h"
#include "src/variables/auth_type.h"
#include "src/variables/duration.h"
#include "src/variables/env.h"
#include "src/variables/files_combined_size.h"
#include "src/variables/files.h"
#include "src/variables/files_names.h"
#include "src/variables/files_sizes.h"
#include "src/variables/files_tmp_content.h"
#include "src/variables/files_tmp_names.h"
#include "src/variables/full_request.h"
#include "src/variables/full_request_length.h"
#include "src/variables/geo.h"
#include "src/variables/highest_severity.h"
#include "src/variables/inbound_data_error.h"
#include "src/variables/matched_var.h"
#include "src/variables/matched_var_name.h"
#include "src/variables/matched_vars.h"
#include "src/variables/matched_vars_names.h"
#include "src/variables/modsec_build.h"
#include "src/variables/multipart_crlf_lf_lines.h"
#include "src/variables/multipart_data_after.h"
#include "src/variables/multipart_file_limit_exceeded.h"
#include "src/variables/multipart_file_name.h"
#include "src/variables/multipart_header_folding.h"
#include "src/variables/multipart_invalid_header_folding.h"
#include "src/variables/multipart_invalid_quoting.h"
#include "src/variables/multipart_name.h"
#include "src/variables/multipart_strict_error.h"
#include "src/variables/multipart_unmatched_boundary.h"
#include "src/variables/outbound_data_error.h"
#include "src/variables/path_info.h"
#include "src/variables/query_string.h"
#include "src/variables/remote_addr.h"
#include "src/variables/remote_host.h"
#include "src/variables/remote_port.h"
#include "src/variables/remote_user.h"
#include "src/variables/reqbody_error.h"
#include "src/variables/reqbody_error_msg.h"
#include "src/variables/reqbody_processor_error.h"
#include "src/variables/reqbody_processor_error_msg.h"
#include "src/variables/reqbody_processor.h"
#include "src/variables/request_base_name.h"
#include "src/variables/request_body.h"
#include "src/variables/request_body_length.h"
#include "src/variables/request_cookies.h"
#include "src/variables/request_cookies_names.h"
#include "src/variables/request_file_name.h"
#include "src/variables/request_headers.h"
#include "src/variables/request_headers_names.h"
#include "src/variables/request_line.h"
#include "src/variables/request_method.h"
#include "src/variables/request_protocol.h"
#include "src/variables/request_uri.h"
#include "src/variables/request_uri_raw.h"
#include "src/variables/resource.h"
#include "src/variables/response_body.h"
#include "src/variables/response_content_length.h"
#include "src/variables/response_content_type.h"
#include "src/variables/response_headers.h"
#include "src/variables/response_headers_names.h"
#include "src/variables/response_protocol.h"
#include "src/variables/response_status.h"
#include "src/variables/rule.h"
#include "src/variables/server_addr.h"
#include "src/variables/server_name.h"
#include "src/variables/server_port.h"
#include "src/variables/session_id.h"
#include "src/variables/time_day.h"
#include "src/variables/time_epoch.h"
#include "src/variables/time.h"
#include "src/variables/time_hour.h"
#include "src/variables/time_min.h"
#include "src/variables/time_mon.h"
#include "src/variables/time_sec.h"
#include "src/variables/time_wday.h"
#include "src/variables/time_year.h"
#include "src/variables/tx.h"
#include "src/variables/unique_id.h"
#include "src/variables/url_encoded_error.h"
#include "src/variables/user_id.h"
#include "src/variables/variable.h"
#include "src/variables/xml.h"
#include "src/variables/ip.h"
#include "src/variables/global.h"
#include "src/variables/session.h"
#include "src/variables/status.h"

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
using modsecurity::Variables::Tx_DictElement;
using modsecurity::Variables::Tx_NoDictElement;
using modsecurity::Variables::Tx_DictElementRegexp;

using modsecurity::Variables::Ip_DictElement;
using modsecurity::Variables::Ip_NoDictElement;
using modsecurity::Variables::Ip_DictElementRegexp;

using modsecurity::Variables::Global_DictElement;
using modsecurity::Variables::Global_NoDictElement;
using modsecurity::Variables::Global_DictElementRegexp;

using modsecurity::Variables::Session_DictElement;
using modsecurity::Variables::Session_NoDictElement;
using modsecurity::Variables::Session_DictElementRegexp;



using modsecurity::Variables::Variable;
using modsecurity::Variables::VariableModificatorExclusion;
using modsecurity::Variables::VariableModificatorCount;
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


#define OPERATOR_NOT_SUPPORTED(a, b) \
    driver.error(b, "Operator: " + std::string(a) + " is not yet supported."); \
    YYERROR;


#define ACTION_INIT(a, b) \
    std::string error; \
    if (a->init(&error) == false) { \
        driver.error(b, error); \
        YYERROR; \
    }

#define OPERATOR_CONTAINER(a, b) \
    std::unique_ptr<Operator> c(b); \
    a = std::move(c);

#define ACTION_CONTAINER(a, b) \
    std::unique_ptr<actions::Action> c(b); \
    a = std::move(c);

#define VARIABLE_CONTAINER(a, b) \
    std::unique_ptr<Variable> c(b); \
    a = std::move(c);


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
  NEW_LINE
  VAR_COUNT
  VAR_EXCLUSION
  VARIABLE_ARGS
  VARIABLE_ARGS_POST
  VARIABLE_ARGS_GET
  VARIABLE_FILES_SIZES
  VARIABLE_FILES_NAMES
  VARIABLE_FILES_TMP_CONTENT
  VARIABLE_MULTIPART_FILENAME
  VARIABLE_MULTIPART_NAME
  VARIABLE_MATCHED_VARS_NAMES
  VARIABLE_MATCHED_VARS
  VARIABLE_FILES
  VARIABLE_REQUEST_COOKIES
  VARIABLE_REQUEST_HEADERS
  VARIABLE_RESPONSE_HEADERS
  VARIABLE_GEO
  VARIABLE_REQUEST_COOKIES_NAMES
  VARIABLE_ARGS_COMBINED_SIZE
  VARIABLE_ARGS_GET_NAMES
  VARIABLE_RULE
  VARIABLE_ARGS_NAMES           "Variable ARGS_NAMES"
  VARIABLE_ARGS_POST_NAMES
  VARIABLE_AUTH_TYPE            "AUTH_TYPE"
  VARIABLE_FILES_COMBINED_SIZE  "FILES_COMBINED_SIZE"
  VARIABLE_FILES_TMP_NAMES       "FILES_TMPNAMES"
  VARIABLE_FULL_REQUEST         "FULL_REQUEST"
  VARIABLE_FULL_REQUEST_LENGTH  "FULL_REQUEST_LENGTH"
  VARIABLE_INBOUND_DATA_ERROR   "INBOUND_DATA_ERROR"
  VARIABLE_MATCHED_VAR          "MATCHED_VAR"
  VARIABLE_MATCHED_VAR_NAME     "MATCHED_VAR_NAME"
  VARIABLE_MULTIPART_CRLF_LF_LINES    "MULTIPART_CRLF_LF_LINES"
  VARIABLE_MULTIPART_DATA_AFTER "MULTIPART_DATA_AFTER"
  VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED       "MULTIPART_FILE_LIMIT_EXCEEDED"
  VARIABLE_MULTIPART_HEADER_FOLDING            "MULTIPART_HEADER_FOLDING"
  VARIABLE_MULTIPART_INVALID_HEADER_FOLDING    "MULTIPART_INVALID_HEADER_FOLDING"
  VARIABLE_MULTIPART_INVALID_QUOTING           "MULTIPART_INVALID_QUOTING"
  VARIABLE_MULTIPART_STRICT_ERROR              "MULTIPART_STRICT_ERROR"
  VARIABLE_MULTIPART_UNMATCHED_BOUNDARY        "MULTIPART_UNMATCHED_BOUNDARY"
  VARIABLE_OUTBOUND_DATA_ERROR  "OUTBOUND_DATA_ERROR"
  VARIABLE_PATH_INFO            "PATH_INFO"
  VARIABLE_QUERY_STRING         "QUERY_STRING"
  VARIABLE_REMOTE_ADDR          "REMOTE_ADDR"
  VARIABLE_REMOTE_HOST          "REMOTE_HOST"
  VARIABLE_REMOTE_PORT          "REMOTE_PORT"
  VARIABLE_REQBODY_ERROR_MSG    "REQBODY_ERROR_MSG"
  VARIABLE_REQBODY_ERROR        "REQBODY_ERROR"
  VARIABLE_REQBODY_PROCESSOR_ERROR_MSG    "REQBODY_PROCESSOR_ERROR_MSG"
  VARIABLE_REQBODY_PROCESSOR_ERROR        "REQBODY_PROCESSOR_ERROR"
  VARIABLE_REQBODY_PROCESSOR    "REQBODY_PROCESSOR"
  VARIABLE_REQUEST_BASENAME     "REQUEST_BASENAME"
  VARIABLE_REQUEST_BODY_LENGTH  "REQUEST_BODY_LENGTH"
  VARIABLE_REQUEST_BODY         "REQUEST_BODY"
  VARIABLE_REQUEST_FILE_NAME     "REQUEST_FILENAME"
  VARIABLE_REQUEST_HEADERS_NAMES
  VARIABLE_REQUEST_LINE         "REQUEST_LINE"
  VARIABLE_REQUEST_METHOD       "REQUEST_METHOD"
  VARIABLE_REQUEST_PROTOCOL     "REQUEST_PROTOCOL"
  VARIABLE_REQUEST_URI_RAW      "REQUEST_URI_RAW"
  VARIABLE_REQUEST_URI          "REQUEST_URI"
  VARIABLE_RESOURCE             "RESOURCE"
  VARIABLE_RESPONSE_BODY        "RESPONSE_BODY"
  VARIABLE_RESPONSE_CONTENT_LENGTH    "RESPONSE_CONTENT_LENGTH"
  VARIABLE_RESPONSE_CONTENT_TYPE
  VARIABLE_RESPONSE_HEADERS_NAMES
  VARIABLE_RESPONSE_PROTOCOL    "RESPONSE_PROTOCOL"
  VARIABLE_RESPONSE_STATUS      "RESPONSE_STATUS"
  VARIABLE_SERVER_ADDR          "SERVER_ADDR"
  VARIABLE_SERVER_NAME          "SERVER_NAME"
  VARIABLE_SERVER_PORT          "SERVER_PORT"
  VARIABLE_SESSION_ID           "SESSIONID"
  VARIABLE_UNIQUE_ID            "UNIQUE_ID"
  VARIABLE_URL_ENCODED_ERROR    "URLENCODED_ERROR"
  VARIABLE_USER_ID               "USERID"
  VARIABLE_STATUS                              "VARIABLE_STATUS"
  VARIABLE_IP                                  "VARIABLE_IP"
  VARIABLE_GLOBAL                              "VARIABLE_GLOBAL"
  VARIABLE_TX                                  "VARIABLE_TX"
  VARIABLE_SESSION                             "VARIABLE_SESSION"
  VARIABLE_USER                                 "VARIABLE_USER"
  RUN_TIME_VAR_ENV                             "RUN_TIME_VAR_ENV"
  RUN_TIME_VAR_XML                             "RUN_TIME_VAR_XML"

  ACTION_SETVAR                                "SetVar"
  SETVAR_OPERATION_EQUALS
  SETVAR_OPERATION_EQUALS_PLUS
  SETVAR_OPERATION_EQUALS_MINUS
  NOT                                          "NOT"
  ;

%token <std::string>
  ACTION_ACCURACY                              "Accuracy"
  ACTION_ALLOW                                 "Allow"
  ACTION_APPEND                                "Append"
  ACTION_AUDIT_LOG                             "AuditLog"
  ACTION_BLOCK                                 "Block"
  ACTION_CAPTURE                               "Capture"
  ACTION_CHAIN                                 "Chain"
  ACTION_CTL_AUDIT_ENGINE                      "ACTION_CTL_AUDIT_ENGINE"
  ACTION_CTL_AUDIT_LOG_PARTS                   "ACTION_CTL_AUDIT_LOG_PARTS"
  ACTION_CTL_BDY_JSON                          "ACTION_CTL_BDY_JSON"
  ACTION_CTL_BDY_XML                           "ACTION_CTL_BDY_XML"
  ACTION_CTL_FORCE_REQ_BODY_VAR                "ACTION_CTL_FORCE_REQ_BODY_VAR"
  ACTION_CTL_REQUEST_BODY_ACCESS               "ACTION_CTL_REQUEST_BODY_ACCESS"
  ACTION_CTL_RULE_ENGINE                       "ACTION_CTL_RULE_ENGINE"
  ACTION_CTL_RULE_REMOVE_BY_ID                 "ACTION_CTL_RULE_REMOVE_BY_ID"
  ACTION_CTL_RULE_REMOVE_TARGET_BY_ID          "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
  ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG         "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
  ACTION_DENY                                  "Deny"
  ACTION_DEPRECATE_VAR                         "DeprecateVar"
  ACTION_DROP                                  "Drop"
  ACTION_EXEC                                  "Exec"
  ACTION_EXPIRE_VAR                            "ExpireVar"
  ACTION_ID                                    "Id"
  ACTION_INITCOL                               "InitCol"
  ACTION_LOG                                   "Log"
  ACTION_LOG_DATA                              "LogData"
  ACTION_MATURITY                              "Maturity"
  ACTION_MSG                                   "Msg"
  ACTION_MULTI_MATCH                           "MultiMatch"
  ACTION_NO_AUDIT_LOG                          "NoAuditLog"
  ACTION_NO_LOG                                "NoLog"
  ACTION_PASS                                  "Pass"
  ACTION_PAUSE                                 "Pause"
  ACTION_PHASE                                 "Phase"
  ACTION_PREPEND                               "Prepend"
  ACTION_PROXY                                 "Proxy"
  ACTION_REDIRECT                              "Redirect"
  ACTION_REV                                   "Rev"
  ACTION_SANATISE_ARG                          "SanatiseArg"
  ACTION_SANATISE_MATCHED                      "SanatiseMatched"
  ACTION_SANATISE_MATCHED_BYTES                "SanatiseMatchedBytes"
  ACTION_SANATISE_REQUEST_HEADER               "SanatiseRequestHeader"
  ACTION_SANATISE_RESPONSE_HEADER              "SanatiseResponseHeader"
  ACTION_SETENV                                "SetEnv"
  ACTION_SETRSC                                "SetSrc"
  ACTION_SETSID                                "SetSid"
  ACTION_SETUID                                "SetUID"
  ACTION_SEVERITY                              "Severity"
  ACTION_SKIP                                  "Skip"
  ACTION_SKIP_AFTER                            "SkipAfter"
  ACTION_STATUS                                "Status"
  ACTION_TAG                                   "Tag"
  ACTION_TRANSFORMATION_CMD_LINE               "ACTION_TRANSFORMATION_CMD_LINE"
  ACTION_TRANSFORMATION_COMPRESS_WHITESPACE    "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
  ACTION_TRANSFORMATION_CSS_DECODE             "ACTION_TRANSFORMATION_CSS_DECODE"
  ACTION_TRANSFORMATION_HEX_ENCODE             "ACTION_TRANSFORMATION_HEX_ENCODE"
  ACTION_TRANSFORMATION_HEX_DECODE             "ACTION_TRANSFORMATION_HEX_DECODE"
  ACTION_TRANSFORMATION_HTML_ENTITY_DECODE     "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
  ACTION_TRANSFORMATION_JS_DECODE              "ACTION_TRANSFORMATION_JS_DECODE"
  ACTION_TRANSFORMATION_LENGTH                 "ACTION_TRANSFORMATION_LENGTH"
  ACTION_TRANSFORMATION_LOWERCASE              "ACTION_TRANSFORMATION_LOWERCASE"
  ACTION_TRANSFORMATION_MD5                    "ACTION_TRANSFORMATION_MD5"
  ACTION_TRANSFORMATION_NONE                   "ACTION_TRANSFORMATION_NONE"
  ACTION_TRANSFORMATION_NORMALISE_PATH         "ACTION_TRANSFORMATION_NORMALISE_PATH"
  ACTION_TRANSFORMATION_NORMALISE_PATH_WIN     "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
  ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT      "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
  ACTION_TRANSFORMATION_PARITY_ODD_7_BIT       "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
  ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT      "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
  ACTION_TRANSFORMATION_REMOVE_COMMENTS        "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
  ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR   "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
  ACTION_TRANSFORMATION_REMOVE_NULLS           "ACTION_TRANSFORMATION_REMOVE_NULLS"
  ACTION_TRANSFORMATION_REMOVE_WHITESPACE      "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
  ACTION_TRANSFORMATION_REPLACE_COMMENTS       "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
  ACTION_TRANSFORMATION_REPLACE_NULLS          "ACTION_TRANSFORMATION_REPLACE_NULLS"
  ACTION_TRANSFORMATION_SHA1                   "ACTION_TRANSFORMATION_SHA1"
  ACTION_TRANSFORMATION_SQL_HEX_DECODE         "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
  ACTION_TRANSFORMATION_TRIM                   "ACTION_TRANSFORMATION_TRIM"
  ACTION_TRANSFORMATION_URL_DECODE             "ACTION_TRANSFORMATION_URL_DECODE"
  ACTION_TRANSFORMATION_URL_DECODE_UNI         "ACTION_TRANSFORMATION_URL_DECODE_UNI"
  ACTION_TRANSFORMATION_UTF8_TO_UNICODE        "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
  ACTION_VER                                   "Ver"
  ACTION_XMLNS                                 "xmlns"
  CONFIG_COMPONENT_SIG                         "CONFIG_COMPONENT_SIG"
  CONFIG_DIR_AUDIT_DIR                         "CONFIG_DIR_AUDIT_DIR"
  CONFIG_DIR_AUDIT_DIR_MOD                     "CONFIG_DIR_AUDIT_DIR_MOD"
  CONFIG_DIR_AUDIT_ENG                         "CONFIG_DIR_AUDIT_ENG"
  CONFIG_DIR_AUDIT_FLE_MOD                     "CONFIG_DIR_AUDIT_FLE_MOD"
  CONFIG_DIR_AUDIT_LOG                         "CONFIG_DIR_AUDIT_LOG"
  CONFIG_DIR_AUDIT_LOG2                        "CONFIG_DIR_AUDIT_LOG2"
  CONFIG_DIR_AUDIT_LOG_P                       "CONFIG_DIR_AUDIT_LOG_P"
  CONFIG_DIR_AUDIT_STS                         "CONFIG_DIR_AUDIT_STS"
  CONFIG_DIR_AUDIT_TPE                         "CONFIG_DIR_AUDIT_TPE"
  CONFIG_DIR_DEBUG_LOG                         "CONFIG_DIR_DEBUG_LOG"
  CONFIG_DIR_DEBUG_LVL                         "CONFIG_DIR_DEBUG_LVL"
  CONFIG_DIR_GEO_DB                            "CONFIG_DIR_GEO_DB"
  CONFIG_DIR_PCRE_MATCH_LIMIT                  "CONFIG_DIR_PCRE_MATCH_LIMIT"
  CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION        "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
  CONFIG_DIR_REQ_BODY                          "CONFIG_DIR_REQ_BODY"
  CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT          "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
  CONFIG_DIR_REQ_BODY_LIMIT                    "CONFIG_DIR_REQ_BODY_LIMIT"
  CONFIG_DIR_REQ_BODY_LIMIT_ACTION             "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
  CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT           "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
  CONFIG_DIR_RES_BODY                          "CONFIG_DIR_RES_BODY"
  CONFIG_DIR_RES_BODY_LIMIT                    "CONFIG_DIR_RES_BODY_LIMIT"
  CONFIG_DIR_RES_BODY_LIMIT_ACTION             "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
  CONFIG_DIR_RULE_ENG                          "CONFIG_DIR_RULE_ENG"
  CONFIG_DIR_SEC_ACTION                        "CONFIG_DIR_SEC_ACTION"
  CONFIG_DIR_SEC_DEFAULT_ACTION                "CONFIG_DIR_SEC_DEFAULT_ACTION"
  CONFIG_DIR_SEC_MARKER                        "CONFIG_DIR_SEC_MARKER"
  CONFIG_DIR_UNICODE_MAP_FILE                  "CONFIG_DIR_UNICODE_MAP_FILE"
  CONFIG_SEC_COLLECTION_TIMEOUT                "CONFIG_SEC_COLLECTION_TIMEOUT"
  CONFIG_SEC_REMOTE_RULES_FAIL_ACTION          "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
  CONFIG_SEC_RULE_REMOVE_BY_ID                 "CONFIG_SEC_RULE_REMOVE_BY_ID"
  CONFIG_UPDLOAD_KEEP_FILES                    "CONFIG_UPDLOAD_KEEP_FILES"
  CONFIG_UPDLOAD_SAVE_TMP_FILES                "CONFIG_UPDLOAD_SAVE_TMP_FILES"
  CONFIG_UPLOAD_DIR                            "CONFIG_UPLOAD_DIR"
  CONFIG_UPLOAD_FILE_LIMIT                     "CONFIG_UPLOAD_FILE_LIMIT"
  CONFIG_UPLOAD_FILE_MODE                      "CONFIG_UPLOAD_FILE_MODE"
  CONFIG_VALUE_ABORT                           "CONFIG_VALUE_ABORT"
  CONFIG_VALUE_DETC                            "CONFIG_VALUE_DETC"
  CONFIG_VALUE_HTTPS                           "CONFIG_VALUE_HTTPS"
  CONFIG_VALUE_OFF                             "CONFIG_VALUE_OFF"
  CONFIG_VALUE_ON                              "CONFIG_VALUE_ON"
  CONFIG_VALUE_PARALLEL                        "CONFIG_VALUE_PARALLEL"
  CONFIG_VALUE_PROCESS_PARTIAL                 "CONFIG_VALUE_PROCESS_PARTIAL"
  CONFIG_VALUE_REJECT                          "CONFIG_VALUE_REJECT"
  CONFIG_VALUE_RELEVANT_ONLY                   "CONFIG_VALUE_RELEVANT_ONLY"
  CONFIG_VALUE_SERIAL                          "CONFIG_VALUE_SERIAL"
  CONFIG_VALUE_WARN                            "CONFIG_VALUE_WARN"
  CONFIG_XML_EXTERNAL_ENTITY                   "CONFIG_XML_EXTERNAL_ENTITY"
  CONGIG_DIR_RESPONSE_BODY_MP                  "CONGIG_DIR_RESPONSE_BODY_MP"
  CONGIG_DIR_SEC_ARG_SEP                       "CONGIG_DIR_SEC_ARG_SEP"
  CONGIG_DIR_SEC_COOKIE_FORMAT                 "CONGIG_DIR_SEC_COOKIE_FORMAT"
  CONGIG_DIR_SEC_DATA_DIR                      "CONGIG_DIR_SEC_DATA_DIR"
  CONGIG_DIR_SEC_STATUS_ENGINE                 "CONGIG_DIR_SEC_STATUS_ENGINE"
  CONGIG_DIR_SEC_TMP_DIR                       "CONGIG_DIR_SEC_TMP_DIR"
  DIRECTIVE                                    "DIRECTIVE"
  DIRECTIVE_SECRULESCRIPT                      "DIRECTIVE_SECRULESCRIPT"
  FREE_TEXT                                    "FREE_TEXT"
  OPERATOR                                     "OPERATOR"
  OPERATOR_BEGINS_WITH                         "OPERATOR_BEGINS_WITH"
  OPERATOR_CONTAINS                            "OPERATOR_CONTAINS"
  OPERATOR_CONTAINS_WORD                       "OPERATOR_CONTAINS_WORD"
  OPERATOR_DETECT_SQLI                         "OPERATOR_DETECT_SQLI"
  OPERATOR_DETECT_XSS                          "OPERATOR_DETECT_XSS"
  OPERATOR_ENDS_WITH                           "OPERATOR_ENDS_WITH"
  OPERATOR_EQ                                  "OPERATOR_EQ"
  OPERATOR_FUZZY_HASH                          "OPERATOR_FUZZY_HASH"
  OPERATOR_GE                                  "OPERATOR_GE"
  OPERATOR_GEOLOOKUP                           "OPERATOR_GEOLOOKUP"
  OPERATOR_GSB_LOOKUP                          "OPERATOR_GSB_LOOKUP"
  OPERATOR_GT                                  "OPERATOR_GT"
  OPERATOR_INSPECT_FILE                        "OPERATOR_INSPECT_FILE"
  OPERATOR_IP_MATCH                            "OPERATOR_IP_MATCH"
  OPERATOR_IP_MATCH_FROM_FILE                  "OPERATOR_IP_MATCH_FROM_FILE"
  OPERATOR_LE                                  "OPERATOR_LE"
  OPERATOR_LT                                  "OPERATOR_LT"
  OPERATOR_PM                                  "OPERATOR_PM"
  OPERATOR_PM_FROM_FILE                        "OPERATOR_PM_FROM_FILE"
  OPERATOR_RBL                                 "OPERATOR_RBL"
  OPERATOR_RSUB                                "OPERATOR_RSUB"
  OPERATOR_RX                                  "OPERATOR_RX"
  OPERATOR_RX_CONTENT_ONLY                     "Operator RX (content only)"
  OPERATOR_STR_EQ                              "OPERATOR_STR_EQ"
  OPERATOR_STR_MATCH                           "OPERATOR_STR_MATCH"
  OPERATOR_UNCONDITIONAL_MATCH                 "OPERATOR_UNCONDITIONAL_MATCH"
  OPERATOR_VALIDATE_BYTE_RANGE                 "OPERATOR_VALIDATE_BYTE_RANGE"
  OPERATOR_VALIDATE_DTD                        "OPERATOR_VALIDATE_DTD"
  OPERATOR_VALIDATE_HASH                       "OPERATOR_VALIDATE_HASH"
  OPERATOR_VALIDATE_SCHEMA                     "OPERATOR_VALIDATE_SCHEMA"
  OPERATOR_VALIDATE_URL_ENCODING               "OPERATOR_VALIDATE_URL_ENCODING"
  OPERATOR_VALIDATE_UTF8_ENCODING              "OPERATOR_VALIDATE_UTF8_ENCODING"
  OPERATOR_VERIFY_CC                           "OPERATOR_VERIFY_CC"
  OPERATOR_VERIFY_CPF                          "OPERATOR_VERIFY_CPF"
  OPERATOR_VERIFY_SSN                          "OPERATOR_VERIFY_SSN"
  OPERATOR_WITHIN                              "OPERATOR_WITHIN"
  OP_QUOTE                                     "OP_QUOTE"
  QUOTATION_MARK                               "QUOTATION_MARK"
  RUN_TIME_VAR_BLD                             "RUN_TIME_VAR_BLD"
  RUN_TIME_VAR_DUR                             "RUN_TIME_VAR_DUR"
  RUN_TIME_VAR_HSV                             "RUN_TIME_VAR_HSV"
  RUN_TIME_VAR_REMOTE_USER                     "RUN_TIME_VAR_REMOTE_USER"
  RUN_TIME_VAR_TIME                            "RUN_TIME_VAR_TIME"
  RUN_TIME_VAR_TIME_DAY                        "RUN_TIME_VAR_TIME_DAY"
  RUN_TIME_VAR_TIME_EPOCH                      "RUN_TIME_VAR_TIME_EPOCH"
  RUN_TIME_VAR_TIME_HOUR                       "RUN_TIME_VAR_TIME_HOUR"
  RUN_TIME_VAR_TIME_MIN                        "RUN_TIME_VAR_TIME_MIN"
  RUN_TIME_VAR_TIME_MON                        "RUN_TIME_VAR_TIME_MON"
  RUN_TIME_VAR_TIME_SEC                        "RUN_TIME_VAR_TIME_SEC"
  RUN_TIME_VAR_TIME_WDAY                       "RUN_TIME_VAR_TIME_WDAY"
  RUN_TIME_VAR_TIME_YEAR                       "RUN_TIME_VAR_TIME_YEAR"
  VARIABLE                                     "VARIABLE"
  DICT_ELEMENT                                 "Dictionary element"
  DICT_ELEMENT_REGEXP                          "Dictionary element, selected by regexp"
;

%type <std::unique_ptr<actions::Action>> act

%type <std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > > >
  actions_may_quoted
  actions
;

%type <std::unique_ptr<Operator>>
  op_before_init
  op
;

%type <std::unique_ptr<std::vector<std::unique_ptr<Variable> > > > variables_may_be_quoted
%type <std::unique_ptr<std::vector<std::unique_ptr<Variable> > > > variables
%type <std::unique_ptr<Variable>> var


//%printer { yyoutput << $$; } <*>;

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

actions:
    QUOTATION_MARK actions_may_quoted QUOTATION_MARK
      {
        $$ = std::move($2);
      }
    | actions_may_quoted
      {
        $$ = std::move($1);
      }
    ;

actions_may_quoted:
    actions_may_quoted COMMA act
      {
        ACTION_INIT($3, @0)
        $1->push_back(std::move($3));
        $$ = std::move($1);
      }
    | act
      {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT($1, @0)
        b->push_back(std::move($1));
        $$ = std::move(b);
      }
    ;

op:
    op_before_init
      {
        $$ = std::move($1);
        std::string error;
        if ($$->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | NOT op_before_init
      {
        $$ = std::move($2);
        $$->m_negation = true;
        std::string error;
        if ($$->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | OPERATOR_RX_CONTENT_ONLY
      {
        OPERATOR_CONTAINER($$, new operators::Rx(utils::string::removeBracketsIfNeeded($1)));
        std::string error;
        if ($$->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    | NOT OPERATOR_RX_CONTENT_ONLY
      {
        OPERATOR_CONTAINER($$, new operators::Rx("!" + utils::string::removeBracketsIfNeeded($2)));
        std::string error;
        if ($$->init(driver.ref.back(), &error) == false) {
            driver.error(@0, error);
            YYERROR;
        }
      }
    ;

op_before_init:
    OPERATOR_UNCONDITIONAL_MATCH
      {
        OPERATOR_CONTAINER($$, new operators::UnconditionalMatch());
      }
    | OPERATOR_DETECT_SQLI
      {
        OPERATOR_CONTAINER($$, new operators::DetectSQLi());
      }
    | OPERATOR_DETECT_XSS
      {
        OPERATOR_CONTAINER($$, new operators::DetectXSS());
      }
    | OPERATOR_VALIDATE_URL_ENCODING
      {
        OPERATOR_CONTAINER($$, new operators::ValidateUrlEncoding());
      }
    | OPERATOR_VALIDATE_UTF8_ENCODING
      {
        OPERATOR_CONTAINER($$, new operators::ValidateUtf8Encoding());
      }
    | OPERATOR_INSPECT_FILE FREE_TEXT
      {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", @0);
      }
    | OPERATOR_FUZZY_HASH FREE_TEXT
      {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", @0);
      }
    | OPERATOR_VALIDATE_BYTE_RANGE FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::ValidateByteRange($2));
      }
    | OPERATOR_VALIDATE_DTD FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::ValidateDTD($2));
      }
    | OPERATOR_VALIDATE_HASH FREE_TEXT
      {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", @0);
      }
    | OPERATOR_VALIDATE_SCHEMA FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::ValidateSchema($2));
      }
    | OPERATOR_VERIFY_CC FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::VerifyCC($2));
      }
    | OPERATOR_VERIFY_CPF FREE_TEXT
      {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", @0);
      }
    | OPERATOR_VERIFY_SSN FREE_TEXT
      {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", @0);
      }
    | OPERATOR_GSB_LOOKUP FREE_TEXT
      {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", @0);
      }
    | OPERATOR_RSUB FREE_TEXT
      {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", @0);
      }
    | OPERATOR_WITHIN FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Within($2));
      }
    | OPERATOR_CONTAINS_WORD FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::ContainsWord($2));
      }
    | OPERATOR_CONTAINS FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Contains($2));
      }
    | OPERATOR_ENDS_WITH FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::EndsWith($2));
      }
    | OPERATOR_EQ FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Eq($2));
      }
    | OPERATOR_GE FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Ge($2));
      }
    | OPERATOR_GT FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Gt($2));
      }
    | OPERATOR_IP_MATCH_FROM_FILE FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::IpMatchF($2));
      }
    | OPERATOR_IP_MATCH FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::IpMatch($2));
      }
    | OPERATOR_LE FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Le($2));
      }
    | OPERATOR_LT FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Lt($2));
      }
    | OPERATOR_PM_FROM_FILE FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::PmFromFile($2));
      }
    | OPERATOR_PM FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Pm($2));
      }
    | OPERATOR_RBL FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Rbl($2));
      }
    | OPERATOR_RX FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::Rx($2));
      }
    | OPERATOR_STR_EQ FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::StrEq($2));
      }
    | OPERATOR_STR_MATCH FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::StrMatch($2));
      }
    | OPERATOR_BEGINS_WITH FREE_TEXT
      {
        OPERATOR_CONTAINER($$, new operators::BeginsWith($2));
      }
    | OPERATOR_GEOLOOKUP
      {
#ifdef WITH_GEOIP
        OPERATOR_CONTAINER($$, new operators::GeoLookup());
#else
        std::stringstream ss;
            ss << "This version of ModSecurity was not compiled with GeoIP support.";
            driver.error(@0, ss.str());
            YYERROR;
#endif  // WITH_GEOIP
      }
    ;

expression:
    audit_log
    | DIRECTIVE variables op actions
      {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *$4.get()) {
            a->push_back(i.release());
        }
        std::vector<Variable *> *v = new std::vector<Variable *>();
        for (auto &i : *$2.get()) {
            v->push_back(i.release());
        }

        Operator *op = $3.release();
        Rule *rule = new Rule(
            /* op */ op,
            /* variables */ v,
            /* actions */ a,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );
        if (driver.addSecRule(rule) == false) {
            delete rule;
            YYERROR;
        }
      }
    | DIRECTIVE variables op
      {
        std::vector<Variable *> *v = new std::vector<Variable *>();
        for (auto &i : *$2.get()) {
            v->push_back(i.release());
        }

        Rule *rule = new Rule(
            /* op */ $3.release(),
            /* variables */ v,
            /* actions */ NULL,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );
        if (driver.addSecRule(rule) == false) {
            delete rule;
            YYERROR;
        }
      }
    | CONFIG_DIR_SEC_ACTION actions
      {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *$2.get()) {
            a->push_back(i.release());
        }
        Rule *rule = new Rule(
            /* op */ NULL,
            /* variables */ NULL,
            /* actions */ a,
            /* file name */ driver.ref.back(),
            /* line number */ @0.end.line
            );
        driver.addSecAction(rule);
      }
    | DIRECTIVE_SECRULESCRIPT actions
      {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
    | CONFIG_DIR_SEC_DEFAULT_ACTION actions
      {
        std::vector<actions::Action *> *actions = new std::vector<actions::Action *>();
        for (auto &i : *$2.get()) {
            actions->push_back(i.release());
        }
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

        if (!driver.m_defaultActions[definedPhase].empty()) {
            std::stringstream ss;
            ss << "SecDefaultActions can only be placed once per phase and configuration context. Phase ";
            ss << secRuleDefinedPhase;
            ss << " was informed already.";
            driver.error(@0, ss.str());
            YYERROR;
        }

        for (actions::Action *a : checkedActions) {
            driver.m_defaultActions[definedPhase].push_back(a);
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
        std::string err;
        std::string file = modsecurity::utils::find_resource($1,
            driver.ref.back(), &err);
        if (file.empty()) {
            std::stringstream ss;
            ss << "Failed to load locate the GeoDB file from: " << $1 << " ";
            ss << err;
            driver.error(@0, ss.str());
            YYERROR;
        }
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
    variables_may_be_quoted
      {
        $$ = std::move($1);
      }
    | QUOTATION_MARK variables_may_be_quoted QUOTATION_MARK
      {
        $$ = std::move($2);
      }
    ;

variables_may_be_quoted:
    variables_may_be_quoted PIPE var
      {
        $1->push_back(std::move($3));
        $$ = std::move($1);
      }
    | variables_may_be_quoted PIPE VAR_EXCLUSION var
      {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move($4)));
        $1->push_back(std::move(c));
        $$ = std::move($1);
      }
    | variables_may_be_quoted PIPE VAR_COUNT var
      {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move($4)));
        $1->push_back(std::move(c));
        $$ = std::move($1);
      }
    | var
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move($1));
        $$ = std::move(b);
      }
    | VAR_EXCLUSION var
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move($2)));
        b->push_back(std::move(c));
        $$ = std::move(b);
      }
    | VAR_COUNT var
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move($2)));
        b->push_back(std::move(c));
        $$ = std::move(b);
      }
    ;

var:
    VARIABLE_ARGS DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Args_DictElement($2));
      }
    | VARIABLE_ARGS DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Args_DictElementRegexp($2));
      }
    | VARIABLE_ARGS
      {
        VARIABLE_CONTAINER($$, new Variables::Args_NoDictElement());
      }
    | VARIABLE_ARGS_POST DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsPost_DictElement($2));
      }
    | VARIABLE_ARGS_POST DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsPost_DictElementRegexp($2));
      }
    | VARIABLE_ARGS_POST
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsPost_NoDictElement());
      }
    | VARIABLE_ARGS_GET DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsGet_DictElement($2));
      }
    | VARIABLE_ARGS_GET DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsGet_DictElementRegexp($2));
      }
    | VARIABLE_ARGS_GET
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsGet_NoDictElement());
      }
    | VARIABLE_FILES_SIZES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::FilesSizes_DictElement($2));
      }
    | VARIABLE_FILES_SIZES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::FilesSizes_DictElementRegexp($2));
      }
    | VARIABLE_FILES_SIZES
      {
        VARIABLE_CONTAINER($$, new Variables::FilesSizes_NoDictElement());
      }
    | VARIABLE_FILES_NAMES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::FilesNames_DictElement($2));
      }
    | VARIABLE_FILES_NAMES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::FilesNames_DictElementRegexp($2));
      }
    | VARIABLE_FILES_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::FilesNames_NoDictElement());
      }
    | VARIABLE_FILES_TMP_CONTENT DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpContent_DictElement($2));
      }
    | VARIABLE_FILES_TMP_CONTENT DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpContent_DictElementRegexp($2));
      }
    | VARIABLE_FILES_TMP_CONTENT
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpContent_NoDictElement());
      }
    | VARIABLE_MULTIPART_FILENAME DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartFileName_DictElement($2));
      }
    | VARIABLE_MULTIPART_FILENAME DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartFileName_DictElementRegexp($2));
      }
    | VARIABLE_MULTIPART_FILENAME
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartFileName_NoDictElement());
      }
    | VARIABLE_MULTIPART_NAME DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartName_DictElement($2));
      }
    | VARIABLE_MULTIPART_NAME DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartName_DictElementRegexp($2));
      }
    | VARIABLE_MULTIPART_NAME
      {
        VARIABLE_CONTAINER($$, new Variables::MultiPartName_NoDictElement());
      }
    | VARIABLE_MATCHED_VARS_NAMES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVarsNames_DictElement($2));
      }
    | VARIABLE_MATCHED_VARS_NAMES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVarsNames_DictElementRegexp($2));
      }
    | VARIABLE_MATCHED_VARS_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVarsNames_NoDictElement());
      }
    | VARIABLE_MATCHED_VARS DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVars_DictElement($2));
      }
    | VARIABLE_MATCHED_VARS DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVars_DictElementRegexp($2));
      }
    | VARIABLE_MATCHED_VARS
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVars_NoDictElement());
      }
    | VARIABLE_FILES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Files_DictElement($2));
      }
    | VARIABLE_FILES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Files_DictElementRegexp($2));
      }
    | VARIABLE_FILES
      {
        VARIABLE_CONTAINER($$, new Variables::Files_NoDictElement());
      }
    | VARIABLE_REQUEST_COOKIES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookies_DictElement($2));
      }
    | VARIABLE_REQUEST_COOKIES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookies_DictElementRegexp($2));
      }
    | VARIABLE_REQUEST_COOKIES
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookies_NoDictElement());
      }
    | VARIABLE_REQUEST_HEADERS DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::RequestHeaders_DictElement($2));
      }
    | VARIABLE_REQUEST_HEADERS DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::RequestHeaders_DictElementRegexp($2));
      }
    | VARIABLE_REQUEST_HEADERS
      {
        VARIABLE_CONTAINER($$, new Variables::RequestHeaders_NoDictElement());
      }
    | VARIABLE_RESPONSE_HEADERS DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseHeaders_DictElement($2));
      }
    | VARIABLE_RESPONSE_HEADERS DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseHeaders_DictElementRegexp($2));
      }
    | VARIABLE_RESPONSE_HEADERS
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseHeaders_NoDictElement());
      }
    | VARIABLE_GEO DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Geo_DictElement($2));
      }
    | VARIABLE_GEO DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Geo_DictElementRegexp($2));
      }
    | VARIABLE_GEO
      {
        VARIABLE_CONTAINER($$, new Variables::Geo_NoDictElement());
      }
    | VARIABLE_REQUEST_COOKIES_NAMES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookiesNames_DictElement($2));
      }
    | VARIABLE_REQUEST_COOKIES_NAMES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookiesNames_DictElementRegexp($2));
      }
    | VARIABLE_REQUEST_COOKIES_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::RequestCookiesNames_NoDictElement());
      }
    | VARIABLE_RULE DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Rule_DictElement($2));
      }
    | VARIABLE_RULE DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Rule_DictElementRegexp($2));
      }
    | VARIABLE_RULE
      {
        VARIABLE_CONTAINER($$, new Variables::Rule_NoDictElement());
      }
    | RUN_TIME_VAR_ENV DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Env("ENV:" + $2));
      }
    | RUN_TIME_VAR_ENV DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Env("ENV:" + $2));
      }
    | RUN_TIME_VAR_ENV
      {
        VARIABLE_CONTAINER($$, new Variables::Env("ENV"));
      }
    | RUN_TIME_VAR_XML DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::XML("XML:" + $2));
      }
    | RUN_TIME_VAR_XML DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::XML("XML:" + $2));
      }
    | RUN_TIME_VAR_XML
      {
        VARIABLE_CONTAINER($$, new Variables::XML_NoDictElement());
      }
    | VARIABLE_FILES_TMP_NAMES DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpNames_DictElement($2));
      }
    | VARIABLE_FILES_TMP_NAMES DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpNames_DictElementRegexp($2));
      }
    | VARIABLE_FILES_TMP_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::FilesTmpNames_NoDictElement());
      }

    | VARIABLE_IP DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Ip_DictElement($2));
      }
    | VARIABLE_IP DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Ip_DictElementRegexp($2));
      }
    | VARIABLE_IP
      {
        VARIABLE_CONTAINER($$, new Variables::Ip_NoDictElement());
      }

    | VARIABLE_GLOBAL DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Global_DictElement($2));
      }
    | VARIABLE_GLOBAL DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Global_DictElementRegexp($2));
      }
    | VARIABLE_GLOBAL
      {
        VARIABLE_CONTAINER($$, new Variables::Global_NoDictElement());
      }

    | VARIABLE_TX DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Tx_DictElement($2));
      }
    | VARIABLE_TX DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Tx_DictElementRegexp($2));
      }
    | VARIABLE_TX
      {
        VARIABLE_CONTAINER($$, new Variables::Tx_NoDictElement());
      }

    | VARIABLE_SESSION DICT_ELEMENT
      {
        VARIABLE_CONTAINER($$, new Variables::Session_DictElement($2));
      }
    | VARIABLE_SESSION DICT_ELEMENT_REGEXP
      {
        VARIABLE_CONTAINER($$, new Variables::Session_DictElementRegexp($2));
      }
    | VARIABLE_SESSION
      {
        VARIABLE_CONTAINER($$, new Variables::Session_NoDictElement());
      }



    | VARIABLE_ARGS_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsNames());
      }
    | VARIABLE_ARGS_GET_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsGetNames());
      }
    | VARIABLE_ARGS_POST_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsPostNames());
      }
    | VARIABLE_REQUEST_HEADERS_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::RequestHeadersNames());
      }
    | VARIABLE_RESPONSE_CONTENT_TYPE
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseContentType());
      }
    | VARIABLE_RESPONSE_HEADERS_NAMES
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseHeadersNames());
      }
    | VARIABLE_ARGS_COMBINED_SIZE
      {
        VARIABLE_CONTAINER($$, new Variables::ArgsCombinedSize());
      }
   | VARIABLE_AUTH_TYPE
      {
        VARIABLE_CONTAINER($$, new Variables::AuthType());
      }
    | VARIABLE_FILES_COMBINED_SIZE
      {
        VARIABLE_CONTAINER($$, new Variables::FilesCombinedSize());
      }
    | VARIABLE_FULL_REQUEST
      {
        VARIABLE_CONTAINER($$, new Variables::FullRequest());
      }
    | VARIABLE_FULL_REQUEST_LENGTH
      {
        VARIABLE_CONTAINER($$, new Variables::FullRequestLength());
      }
    | VARIABLE_INBOUND_DATA_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::InboundDataError());
      }
    | VARIABLE_MATCHED_VAR
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVar());
      }
    | VARIABLE_MATCHED_VAR_NAME
      {
        VARIABLE_CONTAINER($$, new Variables::MatchedVarName());
      }
    | VARIABLE_MULTIPART_CRLF_LF_LINES
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartCrlfLFLines());
      }
    | VARIABLE_MULTIPART_DATA_AFTER
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartDateAfter());
      }
    | VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartFileLimitExceeded());
      }
    | VARIABLE_MULTIPART_HEADER_FOLDING
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartHeaderFolding());
      }
    | VARIABLE_MULTIPART_INVALID_HEADER_FOLDING
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartInvalidHeaderFolding());
      }
    | VARIABLE_MULTIPART_INVALID_QUOTING
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartInvalidQuoting());
      }
    | VARIABLE_MULTIPART_STRICT_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartStrictError());
      }
    | VARIABLE_MULTIPART_UNMATCHED_BOUNDARY
      {
        VARIABLE_CONTAINER($$, new Variables::MultipartUnmatchedBoundary());
      }
    | VARIABLE_OUTBOUND_DATA_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::OutboundDataError());
      }
    | VARIABLE_PATH_INFO
      {
        VARIABLE_CONTAINER($$, new Variables::PathInfo());
      }
    | VARIABLE_QUERY_STRING
      {
        VARIABLE_CONTAINER($$, new Variables::QueryString());
      }
    | VARIABLE_REMOTE_ADDR
      {
        VARIABLE_CONTAINER($$, new Variables::RemoteAddr());
      }
    | VARIABLE_REMOTE_HOST
      {
        VARIABLE_CONTAINER($$, new Variables::RemoteHost());
      }
    | VARIABLE_REMOTE_PORT
      {
        VARIABLE_CONTAINER($$, new Variables::RemotePort());
      }
    | VARIABLE_REQBODY_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::ReqbodyError());
      }
    | VARIABLE_REQBODY_ERROR_MSG
      {
        VARIABLE_CONTAINER($$, new Variables::ReqbodyErrorMsg());
      }
    | VARIABLE_REQBODY_PROCESSOR
      {
        VARIABLE_CONTAINER($$, new Variables::ReqbodyProcessor());
      }
    | VARIABLE_REQBODY_PROCESSOR_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::ReqbodyProcessorError());
      }
    | VARIABLE_REQBODY_PROCESSOR_ERROR_MSG
      {
        VARIABLE_CONTAINER($$, new Variables::ReqbodyProcessorErrorMsg());
      }
    | VARIABLE_REQUEST_BASENAME
      {
        VARIABLE_CONTAINER($$, new Variables::RequestBasename());
      }
    | VARIABLE_REQUEST_BODY
      {
        VARIABLE_CONTAINER($$, new Variables::RequestBody());
      }
    | VARIABLE_REQUEST_BODY_LENGTH
      {
        VARIABLE_CONTAINER($$, new Variables::RequestBodyLength());
      }
    | VARIABLE_REQUEST_FILE_NAME
      {
        VARIABLE_CONTAINER($$, new Variables::RequestFilename());
      }
    | VARIABLE_REQUEST_LINE
      {
        VARIABLE_CONTAINER($$, new Variables::RequestLine());
      }
    | VARIABLE_REQUEST_METHOD
      {
        VARIABLE_CONTAINER($$, new Variables::RequestMethod());
      }
    | VARIABLE_REQUEST_PROTOCOL
      {
        VARIABLE_CONTAINER($$, new Variables::RequestProtocol());
      }
    | VARIABLE_REQUEST_URI
      {
        VARIABLE_CONTAINER($$, new Variables::RequestURI());
      }
    | VARIABLE_REQUEST_URI_RAW
      {
        VARIABLE_CONTAINER($$, new Variables::RequestURIRaw());
      }
    | VARIABLE_RESOURCE
      {
        VARIABLE_CONTAINER($$, new Variables::Resource());
      }
    | VARIABLE_RESPONSE_BODY
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseBody());
      }
    | VARIABLE_RESPONSE_CONTENT_LENGTH
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseContentLength());
      }
    | VARIABLE_RESPONSE_PROTOCOL
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseProtocol());
      }
    | VARIABLE_RESPONSE_STATUS
      {
        VARIABLE_CONTAINER($$, new Variables::ResponseStatus());
      }
    | VARIABLE_SERVER_ADDR
      {
        VARIABLE_CONTAINER($$, new Variables::ServerAddr());
      }
    | VARIABLE_SERVER_NAME
      {
        VARIABLE_CONTAINER($$, new Variables::ServerName());
      }
    | VARIABLE_SERVER_PORT
      {
        VARIABLE_CONTAINER($$, new Variables::ServerPort());
      }
    | VARIABLE_SESSION_ID
      {
        VARIABLE_CONTAINER($$, new Variables::SessionID());
      }
    | VARIABLE_UNIQUE_ID
      {
        VARIABLE_CONTAINER($$, new Variables::UniqueID());
      }
    | VARIABLE_URL_ENCODED_ERROR
      {
        VARIABLE_CONTAINER($$, new Variables::UrlEncodedError());
      }
    | VARIABLE_USER_ID
      {
        VARIABLE_CONTAINER($$, new Variables::UserID());
      }
    | VARIABLE_STATUS
      {
        VARIABLE_CONTAINER($$, new Variables::Status());
      }
    | RUN_TIME_VAR_DUR
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        $$ = std::move(c);
      }

    | RUN_TIME_VAR_BLD
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_HSV
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_REMOTE_USER
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_DAY
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_EPOCH
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_HOUR
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_MIN
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_MON
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_SEC
      {
        std::string name($1);
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_WDAY
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        $$ = std::move(c);
      }
    | RUN_TIME_VAR_TIME_YEAR
      {
        std::string name($1);
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        $$ = std::move(c);
      }
    ;

act:
    ACTION_ACCURACY
      {
        ACTION_CONTAINER($$, new actions::Accuracy($1));
      }
    | ACTION_ALLOW
      {
        ACTION_CONTAINER($$, new actions::disruptive::Allow($1));
      }
    | ACTION_APPEND
      {
        ACTION_NOT_SUPPORTED("Append", @0);
      }
    | ACTION_AUDIT_LOG
      {
        ACTION_CONTAINER($$, new actions::AuditLog($1));
      }
    | ACTION_BLOCK
      {
        ACTION_CONTAINER($$, new actions::disruptive::Block($1));
      }
    | ACTION_CAPTURE
      {
        ACTION_CONTAINER($$, new actions::Capture($1));
      }
    | ACTION_CHAIN
      {
        ACTION_CONTAINER($$, new actions::Chain($1));
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_AUDIT_ENGINE CONFIG_VALUE_RELEVANT_ONLY
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_AUDIT_LOG_PARTS
      {
        ACTION_CONTAINER($$, new actions::ctl::AuditLogParts($1));
      }
    | ACTION_CTL_BDY_JSON
      {
        ACTION_CONTAINER($$, new actions::ctl::RequestBodyProcessorJSON($1));
      }
    | ACTION_CTL_BDY_XML
      {
        ACTION_CONTAINER($$, new actions::ctl::RequestBodyProcessorXML($1));
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_FORCE_REQ_BODY_VAR CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_REQUEST_BODY_ACCESS CONFIG_VALUE_ON
      {
        ACTION_CONTAINER($$, new actions::ctl::RequestBodyAccess($1 + "true"));
      }
    | ACTION_CTL_REQUEST_BODY_ACCESS CONFIG_VALUE_OFF
      {
        ACTION_CONTAINER($$, new actions::ctl::RequestBodyAccess($1 + "false"));
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_ON
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_OFF
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_RULE_ENGINE CONFIG_VALUE_DETC
      {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_CTL_RULE_REMOVE_BY_ID
      {
        ACTION_CONTAINER($$, new actions::ctl::RuleRemoveById($1));
      }
    | ACTION_CTL_RULE_REMOVE_TARGET_BY_ID
      {
        ACTION_CONTAINER($$, new actions::ctl::RuleRemoveTargetById($1));
      }
    | ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG
      {
        ACTION_CONTAINER($$, new actions::ctl::RuleRemoveTargetByTag($1));
      }
    | ACTION_DENY
      {
        ACTION_CONTAINER($$, new actions::disruptive::Deny($1));
      }
    | ACTION_DEPRECATE_VAR
      {
        ACTION_NOT_SUPPORTED("DeprecateVar", @0);
      }
    | ACTION_DROP
      {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_EXEC
      {
        ACTION_NOT_SUPPORTED("Exec", @0);
      }
    | ACTION_EXPIRE_VAR
      {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER($$, new actions::Action($1));
      }
    | ACTION_ID
      {
        ACTION_CONTAINER($$, new actions::RuleId($1));
      }
    | ACTION_INITCOL
      {
        ACTION_CONTAINER($$, new actions::InitCol($1));
      }
    | ACTION_LOG_DATA
      {
        ACTION_CONTAINER($$, new actions::LogData($1));
      }
    | ACTION_LOG
      {
        ACTION_CONTAINER($$, new actions::Log($1));
      }
    | ACTION_MATURITY
      {
        ACTION_CONTAINER($$, new actions::Maturity($1));
      }
    | ACTION_MSG
      {
        ACTION_CONTAINER($$, new actions::Msg($1));
      }
    | ACTION_MULTI_MATCH
      {
        ACTION_CONTAINER($$, new actions::MultiMatch($1));
      }
    | ACTION_NO_AUDIT_LOG
      {
        ACTION_CONTAINER($$, new actions::NoAuditLog($1));
      }
    | ACTION_NO_LOG
      {
        ACTION_CONTAINER($$, new actions::NoLog($1));
      }
    | ACTION_PASS
      {
        ACTION_CONTAINER($$, new actions::disruptive::Pass($1));
      }
    | ACTION_PAUSE
      {
        ACTION_NOT_SUPPORTED("Pause", @0);
      }
    | ACTION_PHASE
      {
        ACTION_CONTAINER($$, new actions::Phase($1));
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
        ACTION_CONTAINER($$, new actions::disruptive::Redirect($1));
      }
    | ACTION_REV
      {
        ACTION_CONTAINER($$, new actions::Rev($1));
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
        ACTION_CONTAINER($$, new actions::SetSID($1));
      }
    | ACTION_SETUID
      {
        ACTION_CONTAINER($$, new actions::SetUID($1));
      }
    | ACTION_SETVAR NOT VARIABLE
      {
        ACTION_CONTAINER($$, new actions::SetVar(actions::SetVarOperation::unsetOperation, $3));
      }
    | ACTION_SETVAR VARIABLE
      {
        ACTION_CONTAINER($$, new actions::SetVar(actions::SetVarOperation::setToOneOperation, $2));
      }
    | ACTION_SETVAR VARIABLE SETVAR_OPERATION_EQUALS FREE_TEXT
      {
        ACTION_CONTAINER($$, new actions::SetVar(actions::SetVarOperation::setOperation, $2, $4));
      }
    | ACTION_SETVAR VARIABLE SETVAR_OPERATION_EQUALS_PLUS FREE_TEXT
      {
        ACTION_CONTAINER($$, new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, $2, $4));
      }
    | ACTION_SETVAR VARIABLE SETVAR_OPERATION_EQUALS_MINUS FREE_TEXT
      {
        ACTION_CONTAINER($$, new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, $2, $4));
      }
    | ACTION_SEVERITY
      {
        ACTION_CONTAINER($$, new actions::Severity($1));
      }
    | ACTION_SKIP
      {
        ACTION_CONTAINER($$, new actions::Skip($1));
      }
    | ACTION_SKIP_AFTER
      {
        ACTION_CONTAINER($$, new actions::SkipAfter($1));
      }
    | ACTION_STATUS
      {
        ACTION_CONTAINER($$, new actions::data::Status($1));
      }
    | ACTION_TAG
      {
        ACTION_CONTAINER($$, new actions::Tag($1));
      }
    | ACTION_VER
      {
        ACTION_CONTAINER($$, new actions::Ver($1));
      }
    | ACTION_XMLNS
      {
        ACTION_CONTAINER($$, new actions::XmlNS($1));
      }
    | ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT
      {
        ACTION_CONTAINER($$, new actions::transformations::ParityZero7bit($1));
      }
    | ACTION_TRANSFORMATION_PARITY_ODD_7_BIT
      {
        ACTION_CONTAINER($$, new actions::transformations::ParityOdd7bit($1));
      }
    | ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT
      {
        ACTION_CONTAINER($$, new actions::transformations::ParityEven7bit($1));
      }
    | ACTION_TRANSFORMATION_SQL_HEX_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::SqlHexDecode($1));
      }
    | ACTION_TRANSFORMATION_CMD_LINE
      {
        ACTION_CONTAINER($$, new actions::transformations::CmdLine($1));
      }
    | ACTION_TRANSFORMATION_SHA1
      {
        ACTION_CONTAINER($$, new actions::transformations::Sha1($1));
      }
    | ACTION_TRANSFORMATION_MD5
      {
        ACTION_CONTAINER($$, new actions::transformations::Md5($1));
      }
    | ACTION_TRANSFORMATION_HEX_ENCODE
      {
        ACTION_CONTAINER($$, new actions::transformations::HexEncode($1));
      }
    | ACTION_TRANSFORMATION_HEX_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::HexDecode($1));
      }
    | ACTION_TRANSFORMATION_LOWERCASE
      {
        ACTION_CONTAINER($$, new actions::transformations::LowerCase($1));
      }
    | ACTION_TRANSFORMATION_URL_DECODE_UNI
      {
        ACTION_CONTAINER($$, new actions::transformations::UrlDecodeUni($1));
      }
    | ACTION_TRANSFORMATION_URL_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::UrlDecode($1));
      }
    | ACTION_TRANSFORMATION_NONE
      {
        ACTION_CONTAINER($$, new actions::transformations::None($1));
      }
    | ACTION_TRANSFORMATION_COMPRESS_WHITESPACE
      {
        ACTION_CONTAINER($$, new actions::transformations::CompressWhitespace($1));
      }
    | ACTION_TRANSFORMATION_REMOVE_WHITESPACE
      {
        ACTION_CONTAINER($$, new actions::transformations::RemoveWhitespace($1));
      }
    | ACTION_TRANSFORMATION_REPLACE_NULLS
      {
        ACTION_CONTAINER($$, new actions::transformations::ReplaceNulls($1));
      }
    | ACTION_TRANSFORMATION_REMOVE_NULLS
      {
        ACTION_CONTAINER($$, new actions::transformations::RemoveNulls($1));
      }
    | ACTION_TRANSFORMATION_HTML_ENTITY_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::HtmlEntityDecode($1));
      }
    | ACTION_TRANSFORMATION_JS_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::JsDecode($1));
      }
    | ACTION_TRANSFORMATION_CSS_DECODE
      {
        ACTION_CONTAINER($$, new actions::transformations::CssDecode($1));
      }
    | ACTION_TRANSFORMATION_TRIM
      {
        ACTION_CONTAINER($$, new actions::transformations::Trim($1));
      }
    | ACTION_TRANSFORMATION_NORMALISE_PATH_WIN
      {
        ACTION_CONTAINER($$, new actions::transformations::NormalisePathWin($1));
      }
    | ACTION_TRANSFORMATION_NORMALISE_PATH
      {
        ACTION_CONTAINER($$, new actions::transformations::NormalisePath($1));
      }
    | ACTION_TRANSFORMATION_LENGTH
      {
        ACTION_CONTAINER($$, new actions::transformations::Length($1));
      }
    | ACTION_TRANSFORMATION_UTF8_TO_UNICODE
      {
        ACTION_CONTAINER($$, new actions::transformations::Utf8ToUnicode($1));
      }
    | ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR
      {
        ACTION_CONTAINER($$, new actions::transformations::RemoveCommentsChar($1));
      }
    | ACTION_TRANSFORMATION_REMOVE_COMMENTS
      {
        ACTION_CONTAINER($$, new actions::transformations::RemoveComments($1));
      }
    | ACTION_TRANSFORMATION_REPLACE_COMMENTS
      {
        ACTION_CONTAINER($$, new actions::transformations::ReplaceComments($1));
      }
    ;

%%

void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
