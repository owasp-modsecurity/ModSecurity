%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "parser/driver.h"
#include "seclang-parser.hh"
#include "utils/https_client.h"
#include "utils.h"

using ModSecurity::Parser::Driver;
using ModSecurity::Utils::HttpsClient;
using ModSecurity::split;

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
%}
%option noyywrap nounput batch debug noinput

ACTION          (?i:accuracy|allow|append|auditlog|block|capture|chain|ctl|deny|deprecatevar|drop|exec|expirevar|id:[0-9]+|id:'[0-9]+'|initcol|log|logdata|maturity|msg|multiMatch|noauditlog|nolog|pass|pause|phase:[0-9]+|prepend|proxy|redirect:[A-Z0-9_\|\&\:\/\/\.]+|rev|sanitiseArg|sanitiseMatched|sanitiseMatchedBytes|sanitiseRequestHeader|sanitiseResponseHeader|setuid|setrsc|setsid|setenv|setvar|skip|skipAfter|status:[0-9]+|tag|ver|xmlns)
ACTION_SEVERITY (?i:severity:[0-9]+|severity:'[0-9]+'|severity:(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)|severity:'(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)')
DIRECTIVE       SecRule

CONFIG_DIRECTIVE SecRequestBodyNoFilesLimit|SecRequestBodyInMemoryLimit|SecPcreMatchLimitRecursion|SecPcreMatchLimit|SecResponseBodyMimeType|SecTmpDir|SecDataDir|SecArgumentSeparator|SecCookieFormat|SecStatusEngine

CONFIG_DIR_REQ_BODY_LIMIT (?i:SecRequestBodyLimit)
CONFIG_DIR_RES_BODY_LIMIT (?i:SecResponseBodyLimit)
CONFIG_DIR_REQ_BODY_LIMIT_ACTION (?i:SecRequestBodyLimitAction)
CONFIG_DIR_RES_BODY_LIMIT_ACTION (?i:SecResponseBodyLimitAction)

CONFIG_DIR_GEO_DB (?i:SecGeoLookupDb)

CONFIG_DIR_RULE_ENG SecRuleEngine
CONFIG_DIR_REQ_BODY SecRequestBodyAccess
CONFIG_DIR_RES_BODY SecResponseBodyAccess


CONFIG_DIR_AUDIT_DIR_MOD (?i:SecAuditLogDirMode)
CONFIG_DIR_AUDIT_DIR     (?i:SecAuditLogStorageDir)
CONFIG_DIR_AUDIT_ENG     (?i:SecAuditEngine)
CONFIG_DIR_AUDIT_FLE_MOD (?i:SecAuditLogFileMode)
CONFIG_DIR_AUDIT_LOG2    (?i:SecAuditLog2)
CONFIG_DIR_AUDIT_LOG_P   (?i:SecAuditLogParts)
CONFIG_DIR_AUDIT_LOG     (?i:SecAuditLog)
CONFIG_DIR_AUDIT_STS     (?i:SecAuditLogRelevantStatus)
CONFIG_DIR_AUDIT_TPE     (?i:SecAuditLogType)


CONFIG_DIR_DEBUG_LOG SecDebugLog
CONFIG_DIR_DEBUG_LVL SecDebugLogLevel

CONFIG_COMPONENT_SIG     (?i:SecComponentSignature)

CONFIG_INCLUDE                       (?i:Include)
CONFIG_SEC_REMOTE_RULES              (?i:SecRemoteRules)
CONFIG_SEC_REMOTE_RULES_FAIL_ACTION  (?i:SecRemoteRulesFailAction)


DICT_ELEMENT    [A-Za-z_]+


OPERATOR        (?i:(?:@inspectFile|@fuzzyHash|@validateByteRange|@validateDTD|@validateHash|@validateSchema|@verifyCC|@verifyCPF|@verifySSN|@gsbLookup|@rsub)|(?:\!{0,1})(?:@within|@containsWord|@contains|@endsWith|@eq|@ge|@gt|@ipMatchF|@ipMatch|@ipMatchFromFile|@le|@lt|@pmf|@pm|@pmFromFile|@rbl|@rx|@streq|@strmatch|@beginsWith))

OPERATORNOARG	(?i:@detectSQLi|@detectXSS|@geoLookup|@validateUrlEncoding|@validateUtf8Encoding)


TRANSFORMATION  t:(lowercase|urlDecodeUni|urlDecode|none|compressWhitespace|removeWhitespace|replaceNulls|removeNulls|htmlEntityDecode|jsDecode|cssDecode|trim)

VARIABLE          (?i:UNIQUE_ID|SERVER_PORT|SERVER_ADDR|REMOTE_PORT|REMOTE_HOST|MULTIPART_STRICT_ERROR|PATH_INFO|MULTIPART_NAME|MULTIPART_FILENAME|MULTIPART_CRLF_LF_LINES|MATCHED_VAR_NAME|MATCHED_VARS_NAMES|MATCHED_VAR|MATCHED_VARS|INBOUND_DATA_ERROR|OUTBOUND_DATA_ERROR|FULL_REQUEST|FILES|AUTH_TYPE|ARGS_NAMES|ARGS|QUERY_STRING|REMOTE_ADDR|REQUEST_BASENAME|REQUEST_BODY|REQUEST_COOKIES_NAMES|REQUEST_COOKIES|REQUEST_FILENAME|REQUEST_HEADERS_NAMES|REQUEST_HEADERS|REQUEST_METHOD|REQUEST_PROTOCOL|REQUEST_URI|RESPONSE_BODY|RESPONSE_CONTENT_LENGTH|RESPONSE_CONTENT_TYPE|RESPONSE_HEADERS_NAMES|RESPONSE_HEADERS|RESPONSE_PROTOCOL|RESPONSE_STATUS|TX|GEO)
RUN_TIME_VAR_DUR  (?i:DURATION)
RUN_TIME_VAR_ENV  (?i:ENV)
RUN_TIME_VAR_BLD  (?i:MODSEC_BUILD)
RUN_TIME_VAR_HSV  (?i:HIGHEST_SEVERITY)

RUN_TIME_VAR_TIME       (?i:TIME)
RUN_TIME_VAR_TIME_DAY   (?i:TIME_DAY)
RUN_TIME_VAR_TIME_EPOCH (?i:TIME_EPOCH)
RUN_TIME_VAR_TIME_HOUR  (?i:TIME_HOUR)
RUN_TIME_VAR_TIME_MIN   (?i:TIME_MIN)
RUN_TIME_VAR_TIME_MON   (?i:TIME_MON)
RUN_TIME_VAR_TIME_SEC   (?i:TIME_SEC)
RUN_TIME_VAR_TIME_WDAY  (?i:TIME_WDAY)
RUN_TIME_VAR_TIME_YEAR  (?i:TIME_YEAR)

VARIABLENOCOLON  (?i:REQBODY_ERROR|MULTIPART_STRICT_ERROR|MULTIPART_UNMATCHED_BOUNDARY|REMOTE_ADDR|REQUEST_LINE)

CONFIG_VALUE_ON On
CONFIG_VALUE_OFF    Off
CONFIG_VALUE_DETC    DetectOnly
CONFIG_VALUE_SERIAL Serial
CONFIG_VALUE_PARALLEL Parallel
CONFIG_VALUE_RELEVANT_ONLY RelevantOnly

CONFIG_VALUE_PROCESS_PARTIAL (?i:ProcessPartial)
CONFIG_VALUE_REJECT (?i:Reject)

CONFIG_VALUE_ABORT   (?i:Abort)
CONFIG_VALUE_WARN    (?i:Warn)

CONFIG_VALUE_PATH    [0-9A-Za-z_/\.\-]+
AUDIT_PARTS [ABCDEFHJKZ]+
CONFIG_VALUE_NUMBER [0-9]+

FREE_TEXT       [^\"]+
FREE_TEXT_NEW_LINE       [^\"|\n]+

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  driver.loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  driver.loc.step();
%}

{DIRECTIVE}                     { return yy::seclang_parser::make_DIRECTIVE(yytext, driver.loc); }
{TRANSFORMATION}                { return yy::seclang_parser::make_TRANSFORMATION(yytext, driver.loc); }
{CONFIG_DIR_RULE_ENG}           { return yy::seclang_parser::make_CONFIG_DIR_RULE_ENG(yytext, driver.loc); }
{CONFIG_DIR_RES_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY(yytext, driver.loc); }
{CONFIG_DIR_REQ_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY(yytext, driver.loc); }

%{ /* Audit log entries */ %}
{CONFIG_DIR_AUDIT_DIR}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_DIR_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR_MOD(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_ENG}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_ENG(yytext, driver.loc); }
{CONFIG_DIR_AUDIT_FLE_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_FLE_MOD(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_LOG2}[ ]{CONFIG_VALUE_PATH}      { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG2(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_LOG}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_LOG_P}[ ]{AUDIT_PARTS}           { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG_P(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_STS}[ ]["]{FREE_TEXT}["]         { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_STS(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_AUDIT_TPE}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_TPE(yytext, driver.loc); }

%{ /* Debug log entries */ %}
{CONFIG_DIR_DEBUG_LOG}[ ]{CONFIG_VALUE_PATH}    { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LOG(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_DEBUG_LVL}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LVL(strchr(yytext, ' ') + 1, driver.loc); }

%{ /* Variables */ %}
{VARIABLE}:?{DICT_ELEMENT}?             { return yy::seclang_parser::make_VARIABLE(yytext, driver.loc); }
{RUN_TIME_VAR_DUR}                      { return yy::seclang_parser::make_RUN_TIME_VAR_DUR(yytext, driver.loc); }
{RUN_TIME_VAR_ENV}:?{DICT_ELEMENT}?     { return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, driver.loc); }
{RUN_TIME_VAR_BLD}                      { return yy::seclang_parser::make_RUN_TIME_VAR_BLD(yytext, driver.loc); }
{RUN_TIME_VAR_HSV}                      { return yy::seclang_parser::make_RUN_TIME_VAR_HSV(yytext, driver.loc); }

%{ /* Variables: TIME */ %}
{RUN_TIME_VAR_TIME}        { return yy::seclang_parser::make_RUN_TIME_VAR_TIME(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_DAY}    { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_DAY(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_EPOCH}  { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_EPOCH(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_HOUR}   { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_HOUR(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_MIN}    { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MIN(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_MON}    { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MON(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_SEC}    { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_SEC(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_WDAY}   { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_WDAY(yytext, driver.loc); }
{RUN_TIME_VAR_TIME_YEAR}   { return yy::seclang_parser::make_RUN_TIME_VAR_TIME_YEAR(yytext, driver.loc); }

%{ /* Geo DB loopkup */ %}
{CONFIG_DIR_GEO_DB}[ ]{FREE_TEXT_NEW_LINE}      { return yy::seclang_parser::make_CONFIG_DIR_GEO_DB(strchr(yytext, ' ') + 1, driver.loc); }

%{ /* Request body limit */ %}
{CONFIG_DIR_REQ_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_REQ_BODY_LIMIT_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION(yytext, driver.loc); }
%{ /* Reponse body limit */ %}
{CONFIG_DIR_RES_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT(strchr(yytext, ' ') + 1, driver.loc); }
{CONFIG_DIR_RES_BODY_LIMIT_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT_ACTION(yytext, driver.loc); }

{CONFIG_COMPONENT_SIG}[ ]["]{FREE_TEXT}["] { return yy::seclang_parser::make_CONFIG_COMPONENT_SIG(strchr(yytext, ' ') + 2, driver.loc); }

{CONFIG_VALUE_WARN}             { return yy::seclang_parser::make_CONFIG_VALUE_WARN(yytext, driver.loc); }
{CONFIG_VALUE_ABORT}            { return yy::seclang_parser::make_CONFIG_VALUE_ABORT(yytext, driver.loc); }
{CONFIG_VALUE_ON}               { return yy::seclang_parser::make_CONFIG_VALUE_ON(yytext, driver.loc); }
{CONFIG_VALUE_OFF}              { return yy::seclang_parser::make_CONFIG_VALUE_OFF(yytext, driver.loc); }
{CONFIG_VALUE_SERIAL}           { return yy::seclang_parser::make_CONFIG_VALUE_SERIAL(yytext, driver.loc); }
{CONFIG_VALUE_PARALLEL}         { return yy::seclang_parser::make_CONFIG_VALUE_PARALLEL(yytext, driver.loc); }
{CONFIG_VALUE_DETC}             { return yy::seclang_parser::make_CONFIG_VALUE_DETC(yytext, driver.loc); }
{CONFIG_VALUE_RELEVANT_ONLY}    { return yy::seclang_parser::make_CONFIG_VALUE_RELEVANT_ONLY(yytext, driver.loc); }
{CONFIG_VALUE_PROCESS_PARTIAL}  { return yy::seclang_parser::make_CONFIG_VALUE_PROCESS_PARTIAL(yytext, driver.loc); }
{CONFIG_VALUE_REJECT}           { return yy::seclang_parser::make_CONFIG_VALUE_REJECT(yytext, driver.loc); }

["]{OPERATOR}[ ]{FREE_TEXT}["]  { return yy::seclang_parser::make_OPERATOR(yytext, driver.loc); }
["]{OPERATORNOARG}["]           { return yy::seclang_parser::make_OPERATOR(yytext, driver.loc); }
{ACTION}                        { return yy::seclang_parser::make_ACTION(yytext, driver.loc); }
{ACTION_SEVERITY}                        { return yy::seclang_parser::make_ACTION_SEVERITY(yytext, driver.loc); }
["]                             { return yy::seclang_parser::make_QUOTATION_MARK(driver.loc); }
[,]                             { return yy::seclang_parser::make_COMMA(driver.loc); }
[|]                             { return yy::seclang_parser::make_PIPE(driver.loc); }
{VARIABLENOCOLON}	   	{ return yy::seclang_parser::make_VARIABLE(yytext, driver.loc); }
[ \t]+                          { return yy::seclang_parser::make_SPACE(driver.loc); }
[\n]+                           { driver.loc.lines(yyleng); driver.loc.step(); }
.                               { driver.error (driver.loc, "invalid character", yytext); }
<<EOF>>                         {
                                    if (yyin) {
                                        fclose(yyin);
                                    }

                                    yypop_buffer_state();
                                    if (!YY_CURRENT_BUFFER)
                                    {
                                        return yy::seclang_parser::make_END(driver.loc);
                                    }
                                }

%{ /* Include external configurations */ %}
{CONFIG_INCLUDE}[ ]{CONFIG_VALUE_PATH} {
    const char *file = strchr(yytext, ' ') + 1;
    yyin = fopen(file, "r" );
    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
}

{CONFIG_SEC_REMOTE_RULES}[ ][^ ]+[ ][^\n\r ]+ {
    HttpsClient c;
    std::string key;
    std::string url;

    std::vector<std::string> conf = split(yytext, ' ');
    key = conf[1];
    url = conf[2];

    YY_BUFFER_STATE temp = YY_CURRENT_BUFFER;
    yypush_buffer_state(temp);

    bool ret = c.download(url);

    if (ret == false) {
        /**
         * TODO: Implement the fail action.
         *
         */
        if (driver.remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction) {
        }
        if (driver.remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction) {
        }
    }

    yy_scan_string(c.content.c_str());
}

{CONFIG_SEC_REMOTE_RULES_FAIL_ACTION}  { return yy::seclang_parser::make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION(yytext, driver.loc); }


%%

namespace ModSecurity {

bool Driver::scan_begin () {
    yy_flex_debug = trace_scanning;

    if (buffer.empty() == false) {
        yy_scan_string(buffer.c_str());
        return true;
    }
    return false;
}

void Driver::scan_end () {
}

}

