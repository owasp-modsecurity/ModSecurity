%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "parser/driver.h"
#include "seclang-parser.hh"
#include "utils/https_client.h"
#include "utils.h"

using modsecurity::Parser::Driver;
using modsecurity::Utils::HttpsClient;
using modsecurity::split;

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
%}
%option noyywrap nounput batch debug noinput

ACTION          (?i:accuracy|append|block|capture|chain|deny|deprecatevar|drop|expirevar|id:[0-9]+|id:'[0-9]+'|log|multiMatch|noauditlog|nolog|pass|pause|prepend|proxy|sanitiseArg|sanitiseMatched|sanitiseMatchedBytes|sanitiseRequestHeader|sanitiseResponseHeader|setrsc|setenv|status:[0-9]+)
ACTION_XMLNS    (?i:xmlns)
ACTION_ALLOW    ((?i:allow:(?i:REQUEST|PHASE))|(?i:phase:'(?i:REQUEST|PHASE)')|(?i:allow))
ACTION_INITCOL  (?i:initcol)

ACTION_ACCURACY (?i:accuracy)
ACTION_REDIRECT (?i:redirect)
ACTION_SKIP (?i:skip)
ACTION_SKIP_AFTER (?i:skipAfter)
ACTION_PHASE    ((?i:phase:(?i:REQUEST|RESPONSE|LOGGING|[0-9]+))|(?i:phase:'(?i:REQUEST|RESPONSE|LOGGING|[0-9]+)'))
ACTION_AUDIT_LOG (?i:auditlog)
ACTION_EXEC     (?i:exec)
ACTION_SEVERITY (?i:severity)
ACTION_SEVERITY_VALUE (?i:(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)|[0-9]+)
ACTION_SETVAR   (?i:setvar)
ACTION_SETENV   (?i:setenv)
ACTION_SETSID   (?i:setsid)
ACTION_SETUID   (?i:setuid)
ACTION_EXPIREVAR (?i:expirevar)
ACTION_MSG      (?i:msg)
ACTION_TAG      (?i:tag)
ACTION_REV      (?i:rev)
ACTION_VER      (?i:ver)
ACTION_MATURITY (?i:maturity)
ACTION_CTL_BDY_XML         (?i:ctl:requestBodyProcessor=XML)
ACTION_CTL_BDY_JSON        (?i:ctl:requestBodyProcessor=JSON)
ACTION_CTL_AUDIT_LOG_PARTS (?i:ctl:auditLogParts)
ACTION_CTL_RULE_ENGINE     (?i:ctl:ruleEngine)
ACTION_CTL_AUDIT_ENGINE    (?i:ctl:auditEngine)
ACTION_CTL_FORCE_REQ_BODY_VAR (?i:ctl:forceRequestBodyVariable)
DIRECTIVE       (?i:SecRule)
DIRECTIVE_SECRULESCRIPT (?i:SecRuleScript)
LOG_DATA        (?i:logdata)

CONFIG_DIR_SEC_DEFAULT_ACTION (?i:SecDefaultAction)
CONFIG_DIR_SEC_ACTION (?i:SecAction)
CONFIG_DIR_SEC_MARKER (?i:SecMarker)

CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION (?i:SecPcreMatchLimitRecursion)
CONFIG_DIR_PCRE_MATCH_LIMIT (?i:SecPcreMatchLimit)
CONGIG_DIR_RESPONSE_BODY_MP (?i:SecResponseBodyMimeType)
CONGIG_DIR_SEC_TMP_DIR (?i:SecTmpDir)
CONGIG_DIR_SEC_DATA_DIR (?i:SecDataDir)
CONGIG_DIR_SEC_ARG_SEP (?i:SecArgumentSeparator)
CONGIG_DIR_SEC_COOKIE_FORMAT (?i:SecCookieFormat)
CONGIG_DIR_SEC_STATUS_ENGINE (?i:SecStatusEngine)

CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT (?i:SecRequestBodyInMemoryLimit)
CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT (?i:SecRequestBodyNoFilesLimit)
CONFIG_DIR_REQ_BODY_LIMIT (?i:SecRequestBodyLimit)
CONFIG_DIR_RES_BODY_LIMIT (?i:SecResponseBodyLimit)
CONFIG_DIR_REQ_BODY_LIMIT_ACTION (?i:SecRequestBodyLimitAction)
CONFIG_DIR_RES_BODY_LIMIT_ACTION (?i:SecResponseBodyLimitAction)

CONFIG_DIR_GEO_DB (?i:SecGeoLookupDb)

CONFIG_DIR_RULE_ENG (?i:SecRuleEngine)
CONFIG_DIR_REQ_BODY (?i:SecRequestBodyAccess)
CONFIG_DIR_RES_BODY (?i:SecResponseBodyAccess)

CONFIG_XML_EXTERNAL_ENTITY (?i:SecXmlExternalEntity)

CONFIG_DIR_AUDIT_DIR_MOD (?i:SecAuditLogDirMode)
CONFIG_DIR_AUDIT_DIR     (?i:SecAuditLogStorageDir)
CONFIG_DIR_AUDIT_ENG     (?i:SecAuditEngine)
CONFIG_DIR_AUDIT_FLE_MOD (?i:SecAuditLogFileMode)
CONFIG_DIR_AUDIT_LOG2    (?i:SecAuditLog2)
CONFIG_DIR_AUDIT_LOG_P   (?i:SecAuditLogParts)
CONFIG_DIR_AUDIT_LOG     (?i:SecAuditLog)
CONFIG_DIR_AUDIT_STS     (?i:SecAuditLogRelevantStatus)
CONFIG_DIR_AUDIT_TPE     (?i:SecAuditLogType)


CONFIG_UPLOAD_FILE_LIMIT        (?i:SecUploadFileLimit)
CONFIG_UPLOAD_FILE_MODE         (?i:SecUploadFileMode)
CONFIG_UPDLOAD_KEEP_FILES       (?i:SecUploadKeepFiles)
CONFIG_UPDLOAD_SAVE_TMP_FILES   (?i:SecTmpSaveUploadedFiles)
CONFIG_UPLOAD_DIR               (?i:SecUploadDir)


CONFIG_SEC_COLLECTION_TIMEOUT (?i:SecCollectionTimeout)

CONFIG_DIR_DEBUG_LOG (?i:SecDebugLog)
CONFIG_DIR_DEBUG_LVL (?i:SecDebugLogLevel)

CONFIG_COMPONENT_SIG     (?i:SecComponentSignature)

CONFIG_INCLUDE                       (?i:Include)
CONFIG_SEC_REMOTE_RULES              (?i:SecRemoteRules)
CONFIG_SEC_REMOTE_RULES_FAIL_ACTION  (?i:SecRemoteRulesFailAction)

CONFIG_SEC_REMOVE_RULES_BY_ID        (?i:SecRuleRemoveById)

DICT_ELEMENT    [^ \t|]+


OPERATOR        (?i:(?:@inspectFile|@fuzzyHash|@validateByteRange|@validateDTD|@validateHash|@validateSchema|@verifyCC|@verifyCPF|@verifySSN|@gsbLookup|@rsub)|(?:\!{0,1})(?:@within|@containsWord|@contains|@endsWith|@eq|@ge|@gt|@ipMatchF|@ipMatch|@ipMatchFromFile|@le|@lt|@pmf|@pm|@pmFromFile|@rbl|@rx|@streq|@strmatch|@beginsWith))

OPERATORNOARG	(?i:@unconditionalMatch|@detectSQLi|@detectXSS|@validateUrlEncoding|@validateUtf8Encoding)
OPERATOR_GEOIP  (?i:@geoLookup)

TRANSFORMATION  t:(?i:(parityZero7bit|parityOdd7bit|parityEven7bit|sqlHexDecode|cmdLine|sha1|md5|hexEncode|lowercase|urlDecodeUni|urlDecode|none|compressWhitespace|removeWhitespace|replaceNulls|removeNulls|htmlEntityDecode|jsDecode|cssDecode|trim|normalizePathWin|normalisePathWin|normalisePath|length|utf8toUnicode|urldecode|removeCommentsChar|removeComments|replaceComments))


VARIABLE (?i:(SERVER_NAME|MULTIPART_DATA_AFTER|RESOURCE|ARGS_COMBINED_SIZE|ARGS_GET_NAMES|ARGS_POST_NAMES|FILES_TMPNAMES|FILES_COMBINED_SIZE|FULL_REQUEST_LENGTH|REQUEST_BODY_LENGTH|REQUEST_URI_RAW|UNIQUE_ID|SERVER_PORT|SERVER_ADDR|REMOTE_PORT|REMOTE_HOST|PATH_INFO|MULTIPART_CRLF_LF_LINES|MATCHED_VAR_NAME|MATCHED_VAR|INBOUND_DATA_ERROR|OUTBOUND_DATA_ERROR|FULL_REQUEST|AUTH_TYPE|ARGS_NAMES|REMOTE_ADDR|REQUEST_BASENAME|REQUEST_BODY|REQUEST_FILENAME|REQUEST_HEADERS_NAMES|REQUEST_METHOD|REQUEST_PROTOCOL|REQUEST_URI|RESPONSE_BODY|RESPONSE_CONTENT_LENGTH|RESPONSE_CONTENT_TYPE|RESPONSE_HEADERS_NAMES|RESPONSE_PROTOCOL|RESPONSE_STATUS|USERID|SESSIONID))
VARIABLE_COL (?i:(SESSION|GLOBAL|ARGS_POST|ARGS_GET|ARGS|FILES_SIZES|FILES_NAMES|FILES_TMP_CONTENT|MULTIPART_FILENAME|MULTIPART_NAME|MATCHED_VARS_NAMES|MATCHED_VARS|FILES|QUERY_STRING|REQUEST_COOKIES|REQUEST_HEADERS|RESPONSE_HEADERS|GEO|IP|REQUEST_COOKIES_NAMES))
VARIABLE_STATUS (?i:(STATUS[^:]))

VARIABLE_TX (?i:TX)
VARIABLE_WEBSERVER_ERROR_LOG (?:WEBSERVER_ERROR_LOG)

RUN_TIME_VAR_REMOTE_USER  (?i:REMOTE_USER)
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
RUN_TIME_VAR_XML        (?i:XML)
RUN_TIME_VAR_RULE       (?i:RULE)

VARIABLENOCOLON  (?i:URLENCODED_ERROR|REQBODY_PROCESSOR_ERROR_MSG|REQBODY_PROCESSOR_ERROR|REQBODY_PROCESSOR|REQBODY_ERROR_MSG|REQBODY_ERROR|MULTIPART_FILE_LIMIT_EXCEEDED|MULTIPART_INVALID_QUOTING|MULTIPART_HEADER_FOLDING|MULTIPART_INVALID_HEADER_FOLDING|MULTIPART_STRICT_ERROR|MULTIPART_UNMATCHED_BOUNDARY|REMOTE_ADDR|REQUEST_LINE)


CONFIG_VALUE_ON (?i:On)
CONFIG_VALUE_OFF    (?i:Off)
CONFIG_VALUE_DETC    (?i:DetectionOnly)
CONFIG_VALUE_SERIAL (?i:Serial)
CONFIG_VALUE_PARALLEL (?i:Parallel|Concurrent)
CONFIG_VALUE_HTTPS     (?i:https)
CONFIG_VALUE_RELEVANT_ONLY (?i:RelevantOnly)

CONFIG_VALUE_PROCESS_PARTIAL (?i:ProcessPartial)
CONFIG_VALUE_REJECT (?i:Reject)

CONFIG_VALUE_ABORT   (?i:Abort)
CONFIG_VALUE_WARN    (?i:Warn)

CONFIG_VALUE_PATH    [0-9A-Za-z_\/\.\-\*\:]+
AUDIT_PARTS [ABCDEFHJKIZ]+
CONFIG_VALUE_NUMBER [0-9]+

FREE_TEXT       ([^\"]|(\\\"))+

NEW_LINE_FREE_TEXT [^, \t\"\n\r]+
FREE_TEXT_NEW_LINE       [^\"|\n]+
FREE_TEXT_QUOTE ([^\']|([^\\]\\\'))+
FREE_TEXT_SPACE [^ \t]+
FREE_TEXT_SPACE_COMMA [^, \t]+
FREE_TEXT_SPACE_COMMA_QUOTE [^, \t\"\n\r]+
FREE_TEXT_QUOTE_COMMA [^,\']+

COL_NAME [A-Za-z]+
COL_FREE_TEXT_SPACE_COMMA ([^,"])+

VAR_FREE_TEXT_QUOTE ([^\']|([^\\]\\\'))+
VAR_FREE_TEXT_SPACE_COMMA [^, \t\"]+
VAR_FREE_TEXT_SPACE [^ \t\"]+


SOMETHING ["]{1}([^"]|([^\\"]\\\"))*["]{1}

CONFIG_DIR_UNICODE_MAP_FILE (?i:SecUnicodeMapFile)

%x EXPECTING_OPERATOR COMMENT EXPECTING_VARIABLE

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
{DIRECTIVE_SECRULESCRIPT}[ ]{CONFIG_VALUE_PATH} { return yy::seclang_parser::make_DIRECTIVE_SECRULESCRIPT(yytext, *driver.loc.back()); }
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
{ACTION_CTL_AUDIT_LOG_PARTS}=[+|-]{AUDIT_PARTS}    { return yy::seclang_parser::make_ACTION_CTL_AUDIT_LOG_PARTS(yytext, *driver.loc.back()); }
{ACTION_CTL_RULE_ENGINE}=                          { return yy::seclang_parser::make_ACTION_CTL_RULE_ENGINE(yytext, *driver.loc.back()); }
{ACTION_CTL_AUDIT_ENGINE}=                         { return yy::seclang_parser::make_ACTION_CTL_AUDIT_ENGINE(yytext, *driver.loc.back()); }
{ACTION_CTL_FORCE_REQ_BODY_VAR}=                   { return yy::seclang_parser::make_ACTION_CTL_FORCE_REQ_BODY_VAR(yytext, *driver.loc.back()); }

%{ /* Remove Rules */ %}
{CONFIG_SEC_REMOVE_RULES_BY_ID}[ ]{FREE_TEXT_NEW_LINE}      { return yy::seclang_parser::make_CONFIG_SEC_RULE_REMOVE_BY_ID(strchr(yytext, ' ') + 1, *driver.loc.back()); }

%{ /* Upload */ %}
{CONFIG_UPLOAD_FILE_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_UPLOAD_FILE_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_UPLOAD_FILE_MODE}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_UPLOAD_FILE_MODE(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_UPDLOAD_KEEP_FILES}  { return yy::seclang_parser::make_CONFIG_UPDLOAD_KEEP_FILES(yytext, *driver.loc.back()); }
{CONFIG_UPDLOAD_SAVE_TMP_FILES}  { return yy::seclang_parser::make_CONFIG_UPDLOAD_SAVE_TMP_FILES(yytext, *driver.loc.back()); }
{CONFIG_UPLOAD_DIR}[ ]{CONFIG_VALUE_PATH}         { return yy::seclang_parser::make_CONFIG_UPLOAD_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }

%{ /* Debug log entries */ %}
{CONFIG_DIR_DEBUG_LOG}[ ]{CONFIG_VALUE_PATH}    { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LOG(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_DEBUG_LVL}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LVL(strchr(yytext, ' ') + 1, *driver.loc.back()); }

<INITIAL,EXPECTING_VARIABLE>{
%{ /* Variables */ %}
[!&]?{VARIABLE_WEBSERVER_ERROR_LOG}     { driver.error (*driver.loc.back(), "Variable VARIABLE_WEBSERVER_ERROR_LOG is not supported by libModSecurity", ""); throw yy::seclang_parser::syntax_error(*driver.loc.back(), "");}
[!&]?{VARIABLE}(\:{DICT_ELEMENT})?        { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
[!&]?{VARIABLE}(\:[\']{FREE_TEXT_QUOTE}[\'])?        { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_STATUS}  { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_STATUS(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_COL}(\:{DICT_ELEMENT})?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_COL(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_COL}(\:[\']{FREE_TEXT_QUOTE}[\'])?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_COL(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_XML}(\:{DICT_ELEMENT})?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_XML}(\:[\']{FREE_TEXT_QUOTE}[\'])?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_RULE}(\:{DICT_ELEMENT})?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_RULE}(\:[\']{FREE_TEXT_QUOTE}[\'])?    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_TX}(\:{DICT_ELEMENT})?     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_TX(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_TX}(\:[\']{FREE_TEXT_QUOTE}[\'])?     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_TX(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_DUR}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_DUR(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_REMOTE_USER}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_REMOTE_USER(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_ENV}(\:{DICT_ELEMENT})?     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_ENV}(\:[\']{FREE_TEXT_QUOTE}[\'])?     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_BLD}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_BLD(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_HSV}                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_HSV(yytext, *driver.loc.back()); }
[!&]?{VARIABLENOCOLON}                       { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }


["][!&]?{VARIABLE}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE}(\:{DICT_ELEMENT})?["]     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_TX}(\:{DICT_ELEMENT})?["]  { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_TX(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_TX}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]  { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_TX(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_COL}(\:{DICT_ELEMENT})? { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_COL(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_COL}(\:[\']{FREE_TEXT_QUOTE}[\'])?["] { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE_COL(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_XML}(\:{DICT_ELEMENT})? { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_XML}(\:[\']{FREE_TEXT_QUOTE}[\'])?["] { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_RULE}(\:{DICT_ELEMENT})? { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_RULE}(\:[\']{FREE_TEXT_QUOTE}[\'])?["] { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }

["][!&]?{RUN_TIME_VAR_DUR}["]                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_DUR(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_ENV}(\:{DICT_ELEMENT})?["]     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_ENV}(\:\'{FREE_TEXT_QUOTE}[\'])?["]     { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_BLD}["]                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_BLD(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_HSV}["]                      { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_HSV(yytext, *driver.loc.back()); }
["][!&]?{VARIABLENOCOLON}["]                       { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_VARIABLE(yytext, *driver.loc.back()); }

%{ /* Variables: TIME */ %}
[!&]?{RUN_TIME_VAR_TIME}        { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_DAY}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_DAY(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_EPOCH}  { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_EPOCH(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_HOUR}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_HOUR(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_MIN}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MIN(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_MON}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_MON(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_SEC}    { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_SEC(yytext, *driver.loc.back()); }
[&]?{RUN_TIME_VAR_TIME_WDAY}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_WDAY(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_YEAR}   { BEGIN(EXPECTING_OPERATOR); return yy::seclang_parser::make_RUN_TIME_VAR_TIME_YEAR(yytext, *driver.loc.back()); }
}


%{ /* Geo DB loopkup */ %}
{CONFIG_DIR_GEO_DB}[ ]{FREE_TEXT_NEW_LINE}      { return yy::seclang_parser::make_CONFIG_DIR_GEO_DB(strchr(yytext, ' ') + 1, *driver.loc.back()); }

%{ /* Request body limit */ %}
{CONFIG_DIR_REQ_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}             { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT}[ ]{CONFIG_VALUE_NUMBER}    { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_LIMIT_ACTION}                              { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}   { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
%{ /* Reponse body limit */ %}
{CONFIG_DIR_RES_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY_LIMIT_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT_ACTION(yytext, *driver.loc.back()); }

{CONFIG_COMPONENT_SIG}[ \t]+["]{FREE_TEXT}["] { return yy::seclang_parser::make_CONFIG_COMPONENT_SIG(strchr(yytext, ' ') + 2, *driver.loc.back()); }

%{ /* Other configurations */ %}
{CONFIG_XML_EXTERNAL_ENTITY} { return yy::seclang_parser::make_CONFIG_XML_EXTERNAL_ENTITY(yytext, *driver.loc.back()); }
{CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_PCRE_MATCH_LIMIT}[ ]{CONFIG_VALUE_NUMBER}    { return yy::seclang_parser::make_CONFIG_DIR_PCRE_MATCH_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_RESPONSE_BODY_MP}[ ]{FREE_TEXT_NEW_LINE}   { return yy::seclang_parser::make_CONGIG_DIR_RESPONSE_BODY_MP(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_TMP_DIR}[ ]{CONFIG_VALUE_PATH}        { return yy::seclang_parser::make_CONGIG_DIR_SEC_TMP_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_DATA_DIR}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONGIG_DIR_SEC_DATA_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_ARG_SEP}[ ]{FREE_TEXT_NEW_LINE}        { return yy::seclang_parser::make_CONGIG_DIR_SEC_ARG_SEP(yytext, *driver.loc.back()); }
{CONGIG_DIR_SEC_COOKIE_FORMAT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONGIG_DIR_SEC_COOKIE_FORMAT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_STATUS_ENGINE}[ ]{FREE_TEXT_NEW_LINE}  { return yy::seclang_parser::make_CONGIG_DIR_SEC_STATUS_ENGINE(yytext, *driver.loc.back()); }
{CONFIG_DIR_UNICODE_MAP_FILE}[ ]{FREE_TEXT_NEW_LINE} { return yy::seclang_parser::make_CONFIG_DIR_UNICODE_MAP_FILE(strchr(yytext, ' ') + 1, *driver.loc.back()); }

{CONFIG_VALUE_WARN}             { return yy::seclang_parser::make_CONFIG_VALUE_WARN(yytext, *driver.loc.back()); }
{CONFIG_VALUE_ABORT}            { return yy::seclang_parser::make_CONFIG_VALUE_ABORT(yytext, *driver.loc.back()); }
{CONFIG_VALUE_ON}               { return yy::seclang_parser::make_CONFIG_VALUE_ON(yytext, *driver.loc.back()); }
{CONFIG_VALUE_OFF}              { return yy::seclang_parser::make_CONFIG_VALUE_OFF(yytext, *driver.loc.back()); }
{CONFIG_VALUE_SERIAL}           { return yy::seclang_parser::make_CONFIG_VALUE_SERIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PARALLEL}         { return yy::seclang_parser::make_CONFIG_VALUE_PARALLEL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_HTTPS}            { return yy::seclang_parser::make_CONFIG_VALUE_HTTPS(yytext, *driver.loc.back()); }
{CONFIG_VALUE_DETC}             { return yy::seclang_parser::make_CONFIG_VALUE_DETC(yytext, *driver.loc.back()); }
{CONFIG_VALUE_RELEVANT_ONLY}    { return yy::seclang_parser::make_CONFIG_VALUE_RELEVANT_ONLY(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PROCESS_PARTIAL}  { return yy::seclang_parser::make_CONFIG_VALUE_PROCESS_PARTIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_REJECT}           { return yy::seclang_parser::make_CONFIG_VALUE_REJECT(yytext, *driver.loc.back()); }

{CONFIG_DIR_SEC_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_SEC_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_SEC_DEFAULT_ACTION} { return yy::seclang_parser::make_CONFIG_DIR_SEC_DEFAULT_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_SEC_MARKER}[ \t]+{NEW_LINE_FREE_TEXT} { return yy::seclang_parser::make_CONFIG_DIR_SEC_MARKER(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_SEC_MARKER}[ \t]+["]{NEW_LINE_FREE_TEXT}["] { return yy::seclang_parser::make_CONFIG_DIR_SEC_MARKER(strchr(yytext, ' ') + 1, *driver.loc.back()); }

<EXPECTING_OPERATOR>{
["]{OPERATOR}[ ]{FREE_TEXT}["]  { BEGIN(INITIAL); return yy::seclang_parser::make_OPERATOR(yytext, *driver.loc.back()); }
["]{OPERATORNOARG}[\t ]*["]     { BEGIN(INITIAL); return yy::seclang_parser::make_OPERATOR(yytext, *driver.loc.back()); }
["]{OPERATOR_GEOIP}[\t ]*["]    { BEGIN(INITIAL); return yy::seclang_parser::make_OPERATOR_GEOIP(yytext, *driver.loc.back()); }
{SOMETHING}           { BEGIN(INITIAL); return yy::seclang_parser::make_FREE_TEXT(yytext, *driver.loc.back()); }
}

{ACTION}                        { return yy::seclang_parser::make_ACTION(yytext, *driver.loc.back()); }
{ACTION_PHASE}                  { return yy::seclang_parser::make_ACTION_PHASE(yytext, *driver.loc.back()); }
{ACTION_SKIP}:{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_ACTION_SKIP(yytext, *driver.loc.back()); }
{ACTION_SKIP_AFTER}:{FREE_TEXT_SPACE_COMMA_QUOTE} { return yy::seclang_parser::make_ACTION_SKIP_AFTER(yytext, *driver.loc.back()); }
{ACTION_AUDIT_LOG}              { return yy::seclang_parser::make_ACTION_AUDIT_LOG(yytext, *driver.loc.back()); }

{ACTION_SEVERITY}:{ACTION_SEVERITY_VALUE}       { return yy::seclang_parser::make_ACTION_SEVERITY(yytext, *driver.loc.back()); }
{ACTION_SEVERITY}:'{ACTION_SEVERITY_VALUE}'     { return yy::seclang_parser::make_ACTION_SEVERITY(yytext, *driver.loc.back()); }

{ACTION_EXEC}:'{VAR_FREE_TEXT_QUOTE}'              {
                                    return yy::seclang_parser::make_ACTION_EXEC(yytext, *driver.loc.back());
                                         }

{ACTION_EXEC}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_EXEC(yytext, *driver.loc.back());
                                         }

{ACTION_EXPIREVAR}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'  {
                                    return yy::seclang_parser::make_ACTION_EXPIREVAR(yytext, *driver.loc.back());
                                         }
{ACTION_EXPIREVAR}:'{VAR_FREE_TEXT_QUOTE}'              {
                                    return yy::seclang_parser::make_ACTION_EXPIREVAR(yytext, *driver.loc.back());
                                         }
{ACTION_EXPIREVAR}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_EXPIREVAR(yytext, *driver.loc.back());
                                         }
{ACTION_EXPIREVAR}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_EXPIREVAR(yytext, *driver.loc.back());
                                         }

{ACTION_SETENV}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'  {
                                    return yy::seclang_parser::make_ACTION_SETENV(yytext, *driver.loc.back());
                                         }
{ACTION_SETENV}:'{VAR_FREE_TEXT_QUOTE}'              {
                                    return yy::seclang_parser::make_ACTION_SETENV(yytext, *driver.loc.back());
                                         }
{ACTION_SETENV}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETENV(yytext, *driver.loc.back());
                                         }
{ACTION_SETENV}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETENV(yytext, *driver.loc.back());
                                         }

{ACTION_SETSID}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETSID(yytext, *driver.loc.back());
                                         }
{ACTION_SETSID}:'{VAR_FREE_TEXT_QUOTE}'      {
                                    return yy::seclang_parser::make_ACTION_SETSID(yytext, *driver.loc.back());
                                         }

{ACTION_SETUID}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETUID(yytext, *driver.loc.back());
                                         }
{ACTION_SETUID}:'{VAR_FREE_TEXT_QUOTE}'      {
                                    return yy::seclang_parser::make_ACTION_SETUID(yytext, *driver.loc.back());
                                         }

{ACTION_SETVAR}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'  {
                                    return yy::seclang_parser::make_ACTION_SETVAR(yytext, *driver.loc.back());
                                         }
{ACTION_SETVAR}:'{VAR_FREE_TEXT_QUOTE}'              {
                                    return yy::seclang_parser::make_ACTION_SETVAR(yytext, *driver.loc.back());
                                         }
{ACTION_SETVAR}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETVAR(yytext, *driver.loc.back());
                                         }
{ACTION_SETVAR}:{VAR_FREE_TEXT_SPACE_COMMA}  {
                                    return yy::seclang_parser::make_ACTION_SETVAR(yytext, *driver.loc.back());
                                         }
{ACTION_XMLNS}:{FREE_TEXT_SPACE_COMMA_QUOTE} { return yy::seclang_parser::make_ACTION_XMLNS(yytext, *driver.loc.back()); }

{LOG_DATA}:'{FREE_TEXT_QUOTE}'         { return yy::seclang_parser::make_LOG_DATA(yytext, *driver.loc.back()); }
{ACTION_MSG}:'{FREE_TEXT_QUOTE}'      { return yy::seclang_parser::make_ACTION_MSG(yytext, *driver.loc.back()); }
{ACTION_ALLOW}                  { return yy::seclang_parser::make_ACTION_ALLOW(yytext, *driver.loc.back()); }
{ACTION_REDIRECT}:{FREE_TEXT} { return yy::seclang_parser::make_ACTION_REDIRECT(yytext, *driver.loc.back()); }
{ACTION_TAG}:'{FREE_TEXT_QUOTE}'      { return yy::seclang_parser::make_ACTION_TAG(yytext, *driver.loc.back()); }
{ACTION_REV}:'{FREE_TEXT_QUOTE_COMMA}'      { return yy::seclang_parser::make_ACTION_REV(yytext, *driver.loc.back()); }
{ACTION_REV}:{FREE_TEXT_QUOTE_COMMA}      { return yy::seclang_parser::make_ACTION_REV(yytext, *driver.loc.back()); }
{ACTION_VER}:'{FREE_TEXT_QUOTE}'      { return yy::seclang_parser::make_ACTION_VER(yytext, *driver.loc.back()); }
{ACTION_MATURITY}:'{FREE_TEXT_QUOTE}'      { return yy::seclang_parser::make_ACTION_MATURITY(yytext, *driver.loc.back()); }
{ACTION_MATURITY}:{FREE_TEXT_QUOTE}      { return yy::seclang_parser::make_ACTION_MATURITY(yytext, *driver.loc.back()); }
{ACTION_ACCURACY}:'{FREE_TEXT_QUOTE}'      { return yy::seclang_parser::make_ACTION_ACCURACY(yytext, *driver.loc.back()); }
{ACTION_ACCURACY}:{FREE_TEXT_QUOTE}      { return yy::seclang_parser::make_ACTION_ACCURACY(yytext, *driver.loc.back()); }
{ACTION_CTL_BDY_XML}              { return yy::seclang_parser::make_ACTION_CTL_BDY_XML(yytext, *driver.loc.back()); }
{ACTION_CTL_BDY_JSON}             { return yy::seclang_parser::make_ACTION_CTL_BDY_JSON(yytext, *driver.loc.back()); }
{ACTION_INITCOL}:{COL_NAME}={COL_FREE_TEXT_SPACE_COMMA} { return yy::seclang_parser::make_ACTION_INITCOL(yytext, *driver.loc.back()); }

["]                             { return yy::seclang_parser::make_QUOTATION_MARK(yytext, *driver.loc.back()); }
[,]                             { return yy::seclang_parser::make_COMMA(*driver.loc.back()); }
<INITIAL,EXPECTING_OPERATOR,EXPECTING_VARIABLE>{
[|]                             { BEGIN(EXPECTING_VARIABLE); return yy::seclang_parser::make_PIPE(*driver.loc.back()); }
}

<INITIAL,EXPECTING_OPERATOR>{
[ \t]+                          {  }
[ \t]*\\\n[ \t]*                { driver.loc.back()->lines(1); driver.loc.back()->step(); }
[ \t]*\\\r\n[ \t]*              { driver.loc.back()->lines(1); driver.loc.back()->step(); }
}

<COMMENT>{
.*[ \t]*\\\n[ \t]*                { driver.loc.back()->lines(1); driver.loc.back()->step(); }
.*[ \t]*\\\r\n[ \t]*              { driver.loc.back()->lines(1); driver.loc.back()->step(); }
.*[^\\]               { BEGIN(INITIAL); driver.loc.back()->lines(1); driver.loc.back()->step(); }
.*[^\\]               { BEGIN(INITIAL); driver.loc.back()->lines(1); driver.loc.back()->step(); }
}

[ \t]*[\n]                           { driver.loc.back()->lines(1); driver.loc.back()->step(); }
#[ \t]*SecRule[^\\].*\\[ \t]*[\r\n]*        { driver.loc.back()->lines(1); driver.loc.back()->step(); BEGIN(COMMENT); }
#[ \t]*SecAction[^\\].*\\[ \t]*[^\\n]      { driver.loc.back()->lines(1); driver.loc.back()->step(); BEGIN(COMMENT);  }
#.*                             { driver.loc.back()->step(); /* comment, just ignore. */ }
\r                              { driver.loc.back()->step(); /* carriage return, just ignore. */}
.                               { driver.error (*driver.loc.back(), "invalid character", yytext); throw yy::seclang_parser::syntax_error(*driver.loc.back(), ""); }
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
    for (auto& s: modsecurity::expandEnv(file, 0)) {
        std::string f = modsecurity::find_resource(s, driver.ref.back());
        yyin = fopen(f.c_str(), "r" );
        if (!yyin) {
            BEGIN(INITIAL);
            driver.error (*driver.loc.back(), "", s + std::string(": Not able to open file."));
            throw yy::seclang_parser::syntax_error(*driver.loc.back(), "");
        }
        driver.ref.push_back(f);
        driver.loc.push_back(new yy::location());
        yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
    }
}

{CONFIG_INCLUDE}[ ]["]{CONFIG_VALUE_PATH}["] {
    const char *file = strchr(yytext, ' ') + 1;
    char *f = strdup(file + 1);
    f[strlen(f)-1] = '\0';
    for (auto& s: modsecurity::expandEnv(f, 0)) {
        std::string f = modsecurity::find_resource(s, driver.ref.back());
        yyin = fopen(f.c_str(), "r" );
        if (!yyin) {
            BEGIN(INITIAL);
            driver.error (*driver.loc.back(), "", s + std::string(": Not able to open file."));
            throw yy::seclang_parser::syntax_error(*driver.loc.back(), "");
        }
        driver.ref.push_back(f.c_str());
        driver.loc.push_back(new yy::location());
        yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

    }
    free(f);
}


{CONFIG_SEC_REMOTE_RULES}[ ][^ ]+[ ][^\n\r ]+ {
    HttpsClient c;
    std::string key;
    std::string url;

    std::vector<std::string> conf = split(yytext, ' ');
    key = conf[1];
    url = conf[2];
    c.setKey(key);

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
{CONFIG_SEC_COLLECTION_TIMEOUT}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_SEC_COLLECTION_TIMEOUT(strchr(yytext, ' ') + 1, *driver.loc.back()); }

%%

namespace modsecurity {

bool Driver::scan_begin () {
    yy_flex_debug = trace_scanning;

    if (buffer.empty() == false) {
        yy_scan_string(buffer.c_str());
        return true;
    }
    return false;
}

void Driver::scan_end () {
    yylex_destroy();
    BEGIN(INITIAL);
}

}

