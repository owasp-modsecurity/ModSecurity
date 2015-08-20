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

ACTION          (?i:accuracy|allow|append|auditlog|block|capture|chain|deny|deprecatevar|drop|exec|expirevar|id:[0-9]+|id:'[0-9]+'|initcol|log|logdata|maturity|multiMatch|noauditlog|nolog|pass|pause|phase:[0-9]+|prepend|proxy|redirect:[A-Z0-9_\|\&\:\/\/\.]+|sanitiseArg|sanitiseMatched|sanitiseMatchedBytes|sanitiseRequestHeader|sanitiseResponseHeader|setuid|setrsc|setsid|setenv|skip|skipAfter|status:[0-9]+|ver|xmlns)
ACTION_SEVERITY (?i:severity:[0-9]+|severity:'[0-9]+'|severity:(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)|severity:'(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)')
ACTION_SETVAR   (?i:setvar)
ACTION_MSG      (?i:msg)
ACTION_TAG      (?i:tag)
ACTION_REV      (?i:rev)
ACTION_CTL_BDY_XML ctl:requestBodyProcessor=XML
ACTION_CTL_BDY_JSON ctl:requestBodyProcessor=JSON
DIRECTIVE       SecRule

CONFIG_DIRECTIVE SecPcreMatchLimitRecursion|SecPcreMatchLimit|SecResponseBodyMimeType|SecTmpDir|SecDataDir|SecArgumentSeparator|SecCookieFormat|SecStatusEngine

CONFIG_DIR_REQ_BOYD_IN_MEMORY_LIMIT (?i:SecRequestBodyInMemoryLimit)

CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT (?i:SecRequestBodyNoFilesLimit)
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


DICT_ELEMENT    [A-Za-z_0-9-]+


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
CONFIG_VALUE_DETC    DetectionOnly
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

%x EXPECTING_OPERATOR

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  driver.loc.back()->columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  driver.loc.back()->step();
%}

{DIRECTIVE}                     { return yy::seclang_parser::make_DIRECTIVE(yytext, *driver.loc.back()); }
{TRANSFORMATION}                { return yy::seclang_parser::make_TRANSFORMATION(yytext, *driver.loc.back()); }
{CONFIG_DIR_RULE_ENG}           { return yy::seclang_parser::make_CONFIG_DIR_RULE_ENG(yytext, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY(yytext, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY(yytext, *driver.loc.back()); }

%{ /* Audit log entries */ %}
{CONFIG_DIR_AUDIT_DIR}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_DIR_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR_MOD(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_ENG}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_ENG(yytext, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_FLE_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_FLE_MOD(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG2}[ ]{CONFIG_VALUE_PATH}      { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG2(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG_P}[ ]{AUDIT_PARTS}           { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG_P(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_STS}[ ]["]{FREE_TEXT}["]         { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_STS(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_TPE}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_TPE(yytext, *driver.loc.back()); }

%{ /* Debug log entries */ %}
{CONFIG_DIR_DEBUG_LOG}[ ]{CONFIG_VALUE_PATH}    { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LOG(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_DEBUG_LVL}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LVL(strchr(yytext, ' ') + 1, *driver.loc.back()); }

<INITIAL,EXPECTING_OPERATOR>{
%{ /* Variables */ %}
[!|&]?{VARIABLE}:?{DICT_ELEMENT}?             { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_DUR}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_DUR(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_ENV}:?{DICT_ELEMENT}?     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_BLD}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_BLD(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_HSV}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_HSV(yytext, *driver.loc.back()); }

%{ /* Variables: TIME */ %}
[!|&]?{RUN_TIME_VAR_TIME}        { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_DAY}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_DAY(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_EPOCH}  { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_EPOCH(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_HOUR}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_HOUR(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_MIN}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MIN(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_MON}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MON(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_SEC}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_SEC(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_WDAY}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_WDAY(yytext, *driver.loc.back()); }
[!|&]?{RUN_TIME_VAR_TIME_YEAR}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_YEAR(yytext, *driver.loc.back()); }
}


%{ /* Geo DB loopkup */ %}
{CONFIG_DIR_GEO_DB}[ ]{FREE_TEXT_NEW_LINE}      { return yy::seclang_parser::make_CONFIG_DIR_GEO_DB(strchr(yytext, ' ') + 1, *driver.loc.back()); }

%{ /* Request body limit */ %}
{CONFIG_DIR_REQ_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_LIMIT_ACTION}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}{ return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
%{ /* Reponse body limit */ %}
{CONFIG_DIR_RES_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY_LIMIT_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT_ACTION(yytext, *driver.loc.back()); }

{CONFIG_COMPONENT_SIG}[ ]["]{FREE_TEXT}["] { return yy::seclang_parser::make_CONFIG_COMPONENT_SIG(strchr(yytext, ' ') + 2, *driver.loc.back()); }

{CONFIG_VALUE_WARN}             { return yy::seclang_parser::make_CONFIG_VALUE_WARN(yytext, *driver.loc.back()); }
{CONFIG_VALUE_ABORT}            { return yy::seclang_parser::make_CONFIG_VALUE_ABORT(yytext, *driver.loc.back()); }
{CONFIG_VALUE_ON}               { return yy::seclang_parser::make_CONFIG_VALUE_ON(yytext, *driver.loc.back()); }
{CONFIG_VALUE_OFF}              { return yy::seclang_parser::make_CONFIG_VALUE_OFF(yytext, *driver.loc.back()); }
{CONFIG_VALUE_SERIAL}           { return yy::seclang_parser::make_CONFIG_VALUE_SERIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PARALLEL}         { return yy::seclang_parser::make_CONFIG_VALUE_PARALLEL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_DETC}             { return yy::seclang_parser::make_CONFIG_VALUE_DETC(yytext, *driver.loc.back()); }
{CONFIG_VALUE_RELEVANT_ONLY}    { return yy::seclang_parser::make_CONFIG_VALUE_RELEVANT_ONLY(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PROCESS_PARTIAL}  { return yy::seclang_parser::make_CONFIG_VALUE_PROCESS_PARTIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_REJECT}           { return yy::seclang_parser::make_CONFIG_VALUE_REJECT(yytext, *driver.loc.back()); }


<EXPECTING_OPERATOR>{
["][^@]{FREE_TEXT}["]  { BEGIN(INITIAL); return yy::seclang_parser::make_FREE_TEXT(yytext, *driver.loc.back()); }
["]{OPERATOR}[ ]{FREE_TEXT}["]  { BEGIN(INITIAL); return yy::seclang_parser::make_OPERATOR(yytext, *driver.loc.back()); }
["]{OPERATORNOARG}["]           { BEGIN(INITIAL); return yy::seclang_parser::make_OPERATOR(yytext, *driver.loc.back()); }
}

{ACTION}                        { return yy::seclang_parser::make_ACTION(yytext, *driver.loc.back()); }
{ACTION_SEVERITY}                        { return yy::seclang_parser::make_ACTION_SEVERITY(yytext, *driver.loc.back()); }
{ACTION_SETVAR}:{FREE_TEXT}={FREE_TEXT}  {
                                    return yy::seclang_parser::make_ACTION_SETVAR(strchr(yytext, ':') + 1, *driver.loc.back());
                                         }
{ACTION_SETVAR}:{FREE_TEXT}              {
                                    return yy::seclang_parser::make_ACTION_SETVAR(strchr(yytext, ':') + 1, *driver.loc.back());
                                         }
{ACTION_MSG}:'{FREE_TEXT}'      { return yy::seclang_parser::make_ACTION_MSG(strchr(yytext, ':') + 1, *driver.loc.back()); }
{ACTION_TAG}:'{FREE_TEXT}'      { return yy::seclang_parser::make_ACTION_TAG(strchr(yytext, ':') + 1, *driver.loc.back()); }
{ACTION_REV}:'{FREE_TEXT}'      { return yy::seclang_parser::make_ACTION_REV(strchr(yytext, ':') + 1, *driver.loc.back()); }
{ACTION_CTL_BDY_XML}              { return yy::seclang_parser::make_ACTION_CTL_BDY_XML(yytext, *driver.loc.back()); }
{ACTION_CTL_BDY_JSON}             { return yy::seclang_parser::make_ACTION_CTL_BDY_JSON(yytext, *driver.loc.back()); }


["]                             { return yy::seclang_parser::make_QUOTATION_MARK(yytext, *driver.loc.back()); }
[,]                             { return yy::seclang_parser::make_COMMA(*driver.loc.back()); }
<INITIAL,EXPECTING_OPERATOR>{
[|]                             { return yy::seclang_parser::make_PIPE(*driver.loc.back()); }
}
{VARIABLENOCOLON}	   	{ return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
<INITIAL,EXPECTING_OPERATOR>{
[ \t]+                          { return yy::seclang_parser::make_SPACE(*driver.loc.back()); }
[ \t]+\\\n                            { /* ignore */ }
[ \t]+\\\r\n                          { /* ignore */ }
}
[\n]+                           { driver.loc.back()->lines(yyleng); driver.loc.back()->step(); }
#.*                             { /* comment, just ignore. */ }
.                               { driver.error (*driver.loc.back(), "invalid character", yytext); }
<<EOF>>                         {
                                    if (driver.ref.size() > 0) {
                                        driver.ref.pop_back();
                                    }

                                    if (driver.loc.size() > 1) {
                                        yy::location *l = driver.loc.back();
                                        driver.loc.pop_back();
                                        delete l;
                                    }

                                    if (yyin) {
                                        fclose(yyin);
                                    }

                                    yypop_buffer_state();
                                    if (!YY_CURRENT_BUFFER)
                                    {
                                        return yy::seclang_parser::make_END(*driver.loc.back());
                                    }
                                }

%{ /* Include external configurations */ %}
{CONFIG_INCLUDE}[ ]{CONFIG_VALUE_PATH} {
    const char *file = strchr(yytext, ' ') + 1;
    yyin = fopen(file, "r" );
    if (!yyin) {
        BEGIN(INITIAL);
        driver.error (*driver.loc.back(), "", yytext + std::string(": Not able to open file."));
        throw yy::seclang_parser::syntax_error(*driver.loc.back(), "");
    }
    driver.ref.push_back(file);
    driver.loc.push_back(new yy::location());
    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
}

{CONFIG_SEC_REMOTE_RULES}[ ][^ ]+[ ][^\n\r ]+ {
    HttpsClient c;
    std::string key;
    std::string url;

    std::vector<std::string> conf = split(yytext, ' ');
    key = conf[1];
    url = conf[2];

    driver.ref.push_back(url);
    driver.loc.push_back(new yy::location());
    YY_BUFFER_STATE temp = YY_CURRENT_BUFFER;
    yypush_buffer_state(temp);

    bool ret = c.download(url);

    if (ret == false) {
        BEGIN(INITIAL);
        if (driver.remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction) {
            /** TODO: Implement the server logging mechanism. */
        }
        if (driver.remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction) {
            driver.error (*driver.loc.back(), "", yytext + std::string(" - Failed to download: ") + c.error);
            throw yy::seclang_parser::syntax_error(*driver.loc.back(), "");
        }
    }

    yy_scan_string(c.content.c_str());
}

{CONFIG_SEC_REMOTE_RULES_FAIL_ACTION}  { return yy::seclang_parser::make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION(yytext, *driver.loc.back()); }


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
    BEGIN(INITIAL);
}

}

