%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "src/parser/driver.h"
#include "src/parser/seclang-parser.hh"
#include "src/utils/https_client.h"
#include "src/utils/string.h"

using modsecurity::Parser::Driver;
using modsecurity::Utils::HttpsClient;

typedef yy::seclang_parser p;

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
%}
%option noyywrap nounput batch debug noinput

ACTION_ACCURACY                                 (?i:accuracy)
ACTION_ALLOW                                    ((?i:allow:(?i:REQUEST|PHASE))|(?i:phase:'(?i:REQUEST|PHASE)')|(?i:allow))
ACTION_APPEND                                   (?i:append)
ACTION_AUDIT_LOG                                (?i:auditlog)
ACTION_BLOCK                                    (?i:block)
ACTION_CAPTURE                                  (?i:capture)
ACTION_CHAIN                                    (?i:chain)
ACTION_CTL_AUDIT_ENGINE                         (?i:ctl:auditEngine)
ACTION_CTL_AUDIT_LOG_PARTS                      (?i:ctl:auditLogParts)
ACTION_CTL_BDY_JSON                             (?i:ctl:requestBodyProcessor=JSON)
ACTION_CTL_BDY_XML                              (?i:ctl:requestBodyProcessor=XML)
ACTION_CTL_FORCE_REQ_BODY_VAR                   (?i:ctl:forceRequestBodyVariable)
ACTION_CTL_REQUEST_BODY_ACCESS                  (?i:ctl:requestBodyAccess)
ACTION_CTL_RULE_ENGINE                          (?i:ctl:ruleEngine)
ACTION_CTL_RULE_REMOVE_BY_ID                    (?i:ctl:ruleRemoveById)
ACTION_CTL_RULE_REMOVE_TARGET_BY_ID             (?i:ctl:ruleRemoveTargetById)
ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG            (?i:ctl:ruleRemoveTargetByTag)
ACTION_DENY                                     (?i:deny)
ACTION_DEPRECATE_VAR                            (?i:deprecatevar)
ACTION_DROP                                     (?i:drop)
ACTION_EXEC                                     (?i:exec)
ACTION_EXPIRE_VAR                               (?i:expirevar)
ACTION_ID                                       (?i:id:[0-9]+|id:'[0-9]+')
ACTION_INITCOL                                  (?i:initcol)
ACTION_LOG_DATA                                 (?i:logdata)
ACTION_LOG                                      (?i:log)
ACTION_MATURITY                                 (?i:maturity)
ACTION_MSG                                      (?i:msg)
ACTION_MULTI_MATCH                              (?i:multiMatch)
ACTION_NO_AUDIT_LOG                             (?i:noauditlog)
ACTION_NO_LOG                                   (?i:nolog)
ACTION_PASS                                     (?i:pass)
ACTION_PAUSE                                    (?i:pause)
ACTION_PHASE                                    ((?i:phase:(?i:REQUEST|RESPONSE|LOGGING|[0-9]+))|(?i:phase:'(?i:REQUEST|RESPONSE|LOGGING|[0-9]+)'))
ACTION_PREPEND                                  (?i:prepend)
ACTION_PROXY                                    (?i:proxy)
ACTION_REDIRECT                                 (?i:redirect)
ACTION_REV                                      (?i:rev)
ACTION_SANATISE_ARG                             (?i:sanitiseArg)
ACTION_SANATISE_MATCHED_BYTES                   (?i:sanitiseMatchedBytes)
ACTION_SANATISE_MATCHED                         (?i:sanitiseMatched)
ACTION_SANATISE_REQUEST_HEADER                  (?i:sanitiseRequestHeader)
ACTION_SANATISE_RESPONSE_HEADER                 (?i:sanitiseResponseHeader)
ACTION_SETENV                                   (?i:setenv)
ACTION_SETRSC                                   (?i:setrsc)
ACTION_SETSID                                   (?i:setsid)
ACTION_SETUID                                   (?i:setuid)
ACTION_SETVAR                                   (?i:setvar)
ACTION_SEVERITY                                 (?i:severity)
ACTION_SEVERITY_VALUE                           (?i:(EMERGENCY|ALERT|CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG)|[0-9]+)
ACTION_SKIP_AFTER                               (?i:skipAfter)
ACTION_SKIP                                     (?i:skip)
ACTION_STATUS                                   (?i:status:[0-9]+)
ACTION_TAG                                      (?i:tag)
ACTION_VER                                      (?i:ver)
ACTION_XMLNS                                    (?i:xmlns)

ACTION_TRANSFORMATION_CMD_LINE                  (?i:t:cmdLine)
ACTION_TRANSFORMATION_COMPRESS_WHITESPACE       (?i:t:compressWhitespace)
ACTION_TRANSFORMATION_CSS_DECODE                (?i:t:cssDecode)
ACTION_TRANSFORMATION_HEX_ENCODE                (?i:t:hexEncode)
ACTION_TRANSFORMATION_HTML_ENTITY_DECODE        (?i:t:htmlEntityDecode)
ACTION_TRANSFORMATION_JS_DECODE                 (?i:t:jsDecode)
ACTION_TRANSFORMATION_LENGTH                    (?i:t:length)
ACTION_TRANSFORMATION_LOWERCASE                 (?i:t:lowercase)
ACTION_TRANSFORMATION_MD5                       (?i:t:md5)
ACTION_TRANSFORMATION_NONE                      (?i:t:none)
ACTION_TRANSFORMATION_NORMALISE_PATH            (?i:t:(normalisePath|normalizePath))
ACTION_TRANSFORMATION_NORMALISE_PATH_WIN        (?i:t:(normalisePathWin|normalizePathWin))
ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT         (?i:t:parityEven7bit)
ACTION_TRANSFORMATION_PARITY_ODD_7_BIT          (?i:t:parityOdd7bit)
ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT         (?i:t:parityZero7bit)
ACTION_TRANSFORMATION_REMOVE_COMMENTS           (?i:t:removeComments)
ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR      (?i:t:removeCommentsChar)
ACTION_TRANSFORMATION_REMOVE_NULLS              (?i:t:removeNulls)
ACTION_TRANSFORMATION_REMOVE_WHITESPACE         (?i:t:removeWhitespace)
ACTION_TRANSFORMATION_REPLACE_COMMENTS          (?i:t:replaceComments)
ACTION_TRANSFORMATION_REPLACE_NULLS             (?i:t:replaceNulls)
ACTION_TRANSFORMATION_SHA1                      (?i:t:sha1)
ACTION_TRANSFORMATION_SQL_HEX_DECODE            (?i:t:sqlHexDecode)
ACTION_TRANSFORMATION_TRIM                      (?i:t:trim)
ACTION_TRANSFORMATION_URL_DECODE                (?i:t:urlDecode)
ACTION_TRANSFORMATION_URL_DECODE_UNI            (?i:t:urlDecodeUni)
ACTION_TRANSFORMATION_UTF8_TO_UNICODE           (?i:t:utf8toUnicode)


AUDIT_PARTS                             [ABCDEFHJKIZ]+
COL_FREE_TEXT_SPACE_COMMA               ([^,"])+
COL_NAME                                [A-Za-z]+
CONFIG_COMPONENT_SIG                    (?i:SecComponentSignature)
CONFIG_DIR_AUDIT_DIR                    (?i:SecAuditLogStorageDir)
CONFIG_DIR_AUDIT_DIR_MOD                (?i:SecAuditLogDirMode)
CONFIG_DIR_AUDIT_ENG                    (?i:SecAuditEngine)
CONFIG_DIR_AUDIT_FLE_MOD                (?i:SecAuditLogFileMode)
CONFIG_DIR_AUDIT_LOG2                   (?i:SecAuditLog2)
CONFIG_DIR_AUDIT_LOG                    (?i:SecAuditLog)
CONFIG_DIR_AUDIT_LOG_P                  (?i:SecAuditLogParts)
CONFIG_DIR_AUDIT_STS                    (?i:SecAuditLogRelevantStatus)
CONFIG_DIR_AUDIT_TPE                    (?i:SecAuditLogType)
CONFIG_DIR_DEBUG_LOG                    (?i:SecDebugLog)
CONFIG_DIR_DEBUG_LVL                    (?i:SecDebugLogLevel)
CONFIG_DIR_GEO_DB                       (?i:SecGeoLookupDb)
CONFIG_DIR_PCRE_MATCH_LIMIT             (?i:SecPcreMatchLimit)
CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION   (?i:SecPcreMatchLimitRecursion)
CONFIG_DIR_REQ_BODY                     (?i:SecRequestBodyAccess)
CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT     (?i:SecRequestBodyInMemoryLimit)
CONFIG_DIR_REQ_BODY_LIMIT               (?i:SecRequestBodyLimit)
CONFIG_DIR_REQ_BODY_LIMIT_ACTION        (?i:SecRequestBodyLimitAction)
CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT      (?i:SecRequestBodyNoFilesLimit)
CONFIG_DIR_RES_BODY                     (?i:SecResponseBodyAccess)
CONFIG_DIR_RES_BODY_LIMIT               (?i:SecResponseBodyLimit)
CONFIG_DIR_RES_BODY_LIMIT_ACTION        (?i:SecResponseBodyLimitAction)
CONFIG_DIR_RULE_ENG                     (?i:SecRuleEngine)
CONFIG_DIR_SEC_ACTION                   (?i:SecAction)
CONFIG_DIR_SEC_DEFAULT_ACTION           (?i:SecDefaultAction)
CONFIG_DIR_SEC_MARKER                   (?i:SecMarker)
CONFIG_DIR_UNICODE_MAP_FILE             (?i:SecUnicodeMapFile)
CONFIG_INCLUDE                          (?i:Include)
CONFIG_SEC_COLLECTION_TIMEOUT           (?i:SecCollectionTimeout)
CONFIG_SEC_REMOTE_RULES                 (?i:SecRemoteRules)
CONFIG_SEC_REMOTE_RULES_FAIL_ACTION     (?i:SecRemoteRulesFailAction)
CONFIG_SEC_REMOVE_RULES_BY_ID           (?i:SecRuleRemoveById)
CONFIG_UPDLOAD_KEEP_FILES               (?i:SecUploadKeepFiles)
CONFIG_UPDLOAD_SAVE_TMP_FILES           (?i:SecTmpSaveUploadedFiles)
CONFIG_UPLOAD_DIR                       (?i:SecUploadDir)
CONFIG_UPLOAD_FILE_LIMIT                (?i:SecUploadFileLimit)
CONFIG_UPLOAD_FILE_MODE                 (?i:SecUploadFileMode)
CONFIG_VALUE_ABORT                      (?i:Abort)
CONFIG_VALUE_DETC                       (?i:DetectionOnly)
CONFIG_VALUE_HTTPS                      (?i:https)
CONFIG_VALUE_NUMBER                     [0-9]+
CONFIG_VALUE_OFF                        (?i:Off)
CONFIG_VALUE_ON                         (?i:On)
CONFIG_VALUE_PARALLEL                   (?i:Parallel|Concurrent)
CONFIG_VALUE_PATH                       [0-9A-Za-z_\/\.\-\*\:]+
CONFIG_VALUE_PROCESS_PARTIAL            (?i:ProcessPartial)
CONFIG_VALUE_REJECT                     (?i:Reject)
CONFIG_VALUE_RELEVANT_ONLY              (?i:RelevantOnly)
CONFIG_VALUE_SERIAL                     (?i:Serial)
CONFIG_VALUE_WARN                       (?i:Warn)
CONFIG_XML_EXTERNAL_ENTITY              (?i:SecXmlExternalEntity)
CONGIG_DIR_RESPONSE_BODY_MP             (?i:SecResponseBodyMimeType)
CONGIG_DIR_SEC_ARG_SEP                  (?i:SecArgumentSeparator)
CONGIG_DIR_SEC_COOKIE_FORMAT            (?i:SecCookieFormat)
CONGIG_DIR_SEC_DATA_DIR                 (?i:SecDataDir)
CONGIG_DIR_SEC_STATUS_ENGINE            (?i:SecStatusEngine)
CONGIG_DIR_SEC_TMP_DIR                  (?i:SecTmpDir)
DICT_ELEMENT                            [^ \t|]+
DIRECTIVE                               (?i:SecRule)
DIRECTIVE_SECRULESCRIPT                 (?i:SecRuleScript)
FREE_TEXT                               ([^\"]|([^\\]\\\"))+
FREE_TEXT_NEW_LINE                      [^\"|\n]+
FREE_TEXT_QUOTE                         ([^\']|([^\\]\\\'))+
FREE_TEXT_QUOTE_COMMA                   [^,\']+
FREE_TEXT_SPACE                         [^ \t]+
FREE_TEXT_SPACE_COMMA                   [^, \t]+
FREE_TEXT_SPACE_COMMA_QUOTE             [^, \t\"\n\r]+
NEW_LINE_FREE_TEXT                      [^, \t\"\n\r]+
OPERATORNOARG                           (?i:@unconditionalMatch|@detectSQLi|@detectXSS|@validateUrlEncoding|@validateUtf8Encoding)
OPERATOR                                (?i:(?:@inspectFile|@fuzzyHash|@validateByteRange|@validateDTD|@validateHash|@validateSchema|@verifyCC|@verifyCPF|@verifySSN|@gsbLookup|@rsub)|(?:\!{0,1})(?:@within|@containsWord|@contains|@endsWith|@eq|@ge|@gt|@ipMatchF|@ipMatch|@ipMatchFromFile|@le|@lt|@pmf|@pm|@pmFromFile|@rbl|@rx|@streq|@strmatch|@beginsWith))
OPERATOR_GEOIP                          (?i:@geoLookup)
REMOVE_RULE_BY                          [0-9A-Za-z_\/\.\-\*\:\;\]\[]+
RUN_TIME_VAR_BLD                        (?i:MODSEC_BUILD)
RUN_TIME_VAR_DUR                        (?i:DURATION)
RUN_TIME_VAR_ENV                        (?i:ENV)
RUN_TIME_VAR_HSV                        (?i:HIGHEST_SEVERITY)
RUN_TIME_VAR_REMOTE_USER                (?i:REMOTE_USER)
RUN_TIME_VAR_RULE                       (?i:RULE)
RUN_TIME_VAR_TIME                       (?i:TIME)
RUN_TIME_VAR_TIME_DAY                   (?i:TIME_DAY)
RUN_TIME_VAR_TIME_EPOCH                 (?i:TIME_EPOCH)
RUN_TIME_VAR_TIME_HOUR                  (?i:TIME_HOUR)
RUN_TIME_VAR_TIME_MIN                   (?i:TIME_MIN)
RUN_TIME_VAR_TIME_MON                   (?i:TIME_MON)
RUN_TIME_VAR_TIME_SEC                   (?i:TIME_SEC)
RUN_TIME_VAR_TIME_WDAY                  (?i:TIME_WDAY)
RUN_TIME_VAR_TIME_YEAR                  (?i:TIME_YEAR)
RUN_TIME_VAR_XML                        (?i:XML)
SOMETHING                               ["]{1}([^"]|([^\\"]\\\"))*["]{1}
VARIABLENOCOLON                         (?i:URLENCODED_ERROR|REQBODY_PROCESSOR_ERROR_MSG|REQBODY_PROCESSOR_ERROR|REQBODY_PROCESSOR|REQBODY_ERROR_MSG|REQBODY_ERROR|MULTIPART_FILE_LIMIT_EXCEEDED|MULTIPART_INVALID_QUOTING|MULTIPART_HEADER_FOLDING|MULTIPART_INVALID_HEADER_FOLDING|MULTIPART_STRICT_ERROR|MULTIPART_UNMATCHED_BOUNDARY|REMOTE_ADDR|REQUEST_LINE)
VARIABLE                                (?i:(SERVER_NAME|MULTIPART_DATA_AFTER|RESOURCE|ARGS_COMBINED_SIZE|ARGS_GET_NAMES|ARGS_POST_NAMES|FILES_TMPNAMES|FILES_COMBINED_SIZE|FULL_REQUEST_LENGTH|REQUEST_BODY_LENGTH|REQUEST_URI_RAW|UNIQUE_ID|SERVER_PORT|SERVER_ADDR|REMOTE_PORT|REMOTE_HOST|PATH_INFO|MULTIPART_CRLF_LF_LINES|MATCHED_VAR_NAME|MATCHED_VAR|INBOUND_DATA_ERROR|OUTBOUND_DATA_ERROR|FULL_REQUEST|AUTH_TYPE|ARGS_NAMES|REMOTE_ADDR|REQUEST_BASENAME|REQUEST_BODY|REQUEST_FILENAME|REQUEST_HEADERS_NAMES|REQUEST_METHOD|REQUEST_PROTOCOL|REQUEST_URI|RESPONSE_BODY|RESPONSE_CONTENT_LENGTH|RESPONSE_CONTENT_TYPE|RESPONSE_HEADERS_NAMES|RESPONSE_PROTOCOL|RESPONSE_STATUS|USERID|SESSIONID))
VARIABLE_COL                            (?i:(SESSION|GLOBAL|ARGS_POST|ARGS_GET|ARGS|FILES_SIZES|FILES_NAMES|FILES_TMP_CONTENT|MULTIPART_FILENAME|MULTIPART_NAME|MATCHED_VARS_NAMES|MATCHED_VARS|FILES|QUERY_STRING|REQUEST_COOKIES|REQUEST_HEADERS|RESPONSE_HEADERS|GEO|IP|REQUEST_COOKIES_NAMES))
VARIABLE_STATUS                         (?i:(STATUS[^:]))
VARIABLE_TX                             (?i:TX)
VARIABLE_WEBSERVER_ERROR_LOG            (?:WEBSERVER_ERROR_LOG)
VAR_FREE_TEXT_QUOTE                     ([^\']|([^\\]\\\'))+
VAR_FREE_TEXT_SPACE                     [^ \t\"]+
VAR_FREE_TEXT_SPACE_COMMA               [^, \t\"]+



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

{ACTION_APPEND}                                                         { return p::make_ACTION_APPEND(yytext, *driver.loc.back()); }
{ACTION_BLOCK}                                                          { return p::make_ACTION_BLOCK(yytext, *driver.loc.back()); }
{ACTION_CAPTURE}                                                        { return p::make_ACTION_CAPTURE(yytext, *driver.loc.back()); }
{ACTION_CHAIN}                                                          { return p::make_ACTION_CHAIN(yytext, *driver.loc.back()); }
{ACTION_DENY}                                                           { return p::make_ACTION_DENY(yytext, *driver.loc.back()); }
{ACTION_DEPRECATE_VAR}                                                  { return p::make_ACTION_DEPRECATE_VAR(yytext, *driver.loc.back()); }
{ACTION_DROP}                                                           { return p::make_ACTION_DROP(yytext, *driver.loc.back()); }
{ACTION_ID}                                                             { return p::make_ACTION_ID(yytext, *driver.loc.back()); }
{ACTION_LOG}                                                            { return p::make_ACTION_LOG(yytext, *driver.loc.back()); }
{ACTION_MULTI_MATCH}                                                    { return p::make_ACTION_MULTI_MATCH(yytext, *driver.loc.back()); }
{ACTION_NO_AUDIT_LOG}                                                   { return p::make_ACTION_NO_AUDIT_LOG(yytext, *driver.loc.back()); }
{ACTION_NO_LOG}                                                         { return p::make_ACTION_NO_LOG(yytext, *driver.loc.back()); }
{ACTION_PASS}                                                           { return p::make_ACTION_PASS(yytext, *driver.loc.back()); }
{ACTION_PAUSE}                                                          { return p::make_ACTION_PAUSE(yytext, *driver.loc.back()); }
{ACTION_PREPEND}                                                        { return p::make_ACTION_PREPEND(yytext, *driver.loc.back()); }
{ACTION_PROXY}                                                          { return p::make_ACTION_PROXY(yytext, *driver.loc.back()); }
{ACTION_SANATISE_ARG}                                                   { return p::make_ACTION_SANATISE_ARG(yytext, *driver.loc.back()); }
{ACTION_SANATISE_MATCHED}                                               { return p::make_ACTION_SANATISE_MATCHED(yytext, *driver.loc.back()); }
{ACTION_SANATISE_MATCHED_BYTES}                                         { return p::make_ACTION_SANATISE_MATCHED_BYTES(yytext, *driver.loc.back()); }
{ACTION_SANATISE_REQUEST_HEADER}                                        { return p::make_ACTION_SANATISE_REQUEST_HEADER(yytext, *driver.loc.back()); }
{ACTION_SANATISE_RESPONSE_HEADER}                                       { return p::make_ACTION_SANATISE_RESPONSE_HEADER(yytext, *driver.loc.back()); }
{ACTION_SETRSC}                                                         { return p::make_ACTION_SETRSC(yytext, *driver.loc.back()); }
{ACTION_STATUS}                                                         { return p::make_ACTION_STATUS(yytext, *driver.loc.back()); }
{ACTION_ACCURACY}:'{FREE_TEXT_QUOTE}'                                   { return p::make_ACTION_ACCURACY(yytext, *driver.loc.back()); }
{ACTION_ACCURACY}:{FREE_TEXT_QUOTE}                                     { return p::make_ACTION_ACCURACY(yytext, *driver.loc.back()); }
{ACTION_ALLOW}                                                          { return p::make_ACTION_ALLOW(yytext, *driver.loc.back()); }
{ACTION_AUDIT_LOG}                                                      { return p::make_ACTION_AUDIT_LOG(yytext, *driver.loc.back()); }
{ACTION_CTL_AUDIT_ENGINE}=                                              { return p::make_ACTION_CTL_AUDIT_ENGINE(yytext, *driver.loc.back()); }
{ACTION_CTL_AUDIT_LOG_PARTS}=[+|-]{AUDIT_PARTS}                         { return p::make_ACTION_CTL_AUDIT_LOG_PARTS(yytext, *driver.loc.back()); }
{ACTION_CTL_BDY_JSON}                                                   { return p::make_ACTION_CTL_BDY_JSON(yytext, *driver.loc.back()); }
{ACTION_CTL_BDY_XML}                                                    { return p::make_ACTION_CTL_BDY_XML(yytext, *driver.loc.back()); }
{ACTION_CTL_FORCE_REQ_BODY_VAR}=                                        { return p::make_ACTION_CTL_FORCE_REQ_BODY_VAR(yytext, *driver.loc.back()); }
{ACTION_CTL_REQUEST_BODY_ACCESS}=                                       { return p::make_ACTION_CTL_REQUEST_BODY_ACCESS(yytext, *driver.loc.back()); }
{ACTION_CTL_RULE_ENGINE}=                                               { return p::make_ACTION_CTL_RULE_ENGINE(yytext, *driver.loc.back()); }
{ACTION_CTL_RULE_REMOVE_BY_ID}[=]{REMOVE_RULE_BY}                       { return p::make_ACTION_CTL_RULE_REMOVE_BY_ID(yytext, *driver.loc.back()); }
{ACTION_CTL_RULE_REMOVE_TARGET_BY_ID}[=]{REMOVE_RULE_BY}                { return p::make_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID(yytext, *driver.loc.back()); }
{ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG}[=]{REMOVE_RULE_BY}               { return p::make_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG(yytext, *driver.loc.back()); }
{ACTION_EXEC}:'{VAR_FREE_TEXT_QUOTE}'                                   { return p::make_ACTION_EXEC(yytext, *driver.loc.back()); }
{ACTION_EXEC}:{VAR_FREE_TEXT_SPACE_COMMA}                               { return p::make_ACTION_EXEC(yytext, *driver.loc.back()); }
{ACTION_EXPIRE_VAR}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'       { return p::make_ACTION_EXPIRE_VAR(yytext, *driver.loc.back()); }
{ACTION_EXPIRE_VAR}:'{VAR_FREE_TEXT_QUOTE}'                             { return p::make_ACTION_EXPIRE_VAR(yytext, *driver.loc.back()); }
{ACTION_EXPIRE_VAR}:{VAR_FREE_TEXT_SPACE_COMMA}                         { return p::make_ACTION_EXPIRE_VAR(yytext, *driver.loc.back()); }
{ACTION_EXPIRE_VAR}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}   { return p::make_ACTION_EXPIRE_VAR(yytext, *driver.loc.back()); }
{ACTION_INITCOL}:{COL_NAME}={COL_FREE_TEXT_SPACE_COMMA}                 { return p::make_ACTION_INITCOL(yytext, *driver.loc.back()); }
{ACTION_MATURITY}:'{FREE_TEXT_QUOTE}'                                   { return p::make_ACTION_MATURITY(yytext, *driver.loc.back()); }
{ACTION_MATURITY}:{FREE_TEXT_QUOTE}                                     { return p::make_ACTION_MATURITY(yytext, *driver.loc.back()); }
{ACTION_MSG}:'{FREE_TEXT_QUOTE}'                                        { return p::make_ACTION_MSG(yytext, *driver.loc.back()); }
{ACTION_PHASE}                                                          { return p::make_ACTION_PHASE(yytext, *driver.loc.back()); }
{ACTION_REDIRECT}:{FREE_TEXT}                                           { return p::make_ACTION_REDIRECT(yytext, *driver.loc.back()); }
{ACTION_REV}:'{FREE_TEXT_QUOTE_COMMA}'                                  { return p::make_ACTION_REV(yytext, *driver.loc.back()); }
{ACTION_REV}:{FREE_TEXT_QUOTE_COMMA}                                    { return p::make_ACTION_REV(yytext, *driver.loc.back()); }
{ACTION_SETENV}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'           { return p::make_ACTION_SETENV(yytext, *driver.loc.back()); }
{ACTION_SETENV}:'{VAR_FREE_TEXT_QUOTE}'                                 { return p::make_ACTION_SETENV(yytext, *driver.loc.back()); }
{ACTION_SETENV}:{VAR_FREE_TEXT_SPACE_COMMA}                             { return p::make_ACTION_SETENV(yytext, *driver.loc.back()); }
{ACTION_SETENV}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}       { return p::make_ACTION_SETENV(yytext, *driver.loc.back()); }
{ACTION_SETSID}:'{VAR_FREE_TEXT_QUOTE}'                                 { return p::make_ACTION_SETSID(yytext, *driver.loc.back()); }
{ACTION_SETSID}:{VAR_FREE_TEXT_SPACE_COMMA}                             { return p::make_ACTION_SETSID(yytext, *driver.loc.back()); }
{ACTION_SETUID}:'{VAR_FREE_TEXT_QUOTE}'                                 { return p::make_ACTION_SETUID(yytext, *driver.loc.back()); }
{ACTION_SETUID}:{VAR_FREE_TEXT_SPACE_COMMA}                             { return p::make_ACTION_SETUID(yytext, *driver.loc.back()); }
{ACTION_SETVAR}:'{VAR_FREE_TEXT_QUOTE}={VAR_FREE_TEXT_QUOTE}'           { return p::make_ACTION_SETVAR(yytext, *driver.loc.back()); }
{ACTION_SETVAR}:'{VAR_FREE_TEXT_QUOTE}'                                 { return p::make_ACTION_SETVAR(yytext, *driver.loc.back()); }
{ACTION_SETVAR}:{VAR_FREE_TEXT_SPACE_COMMA}                             { return p::make_ACTION_SETVAR(yytext, *driver.loc.back()); }
{ACTION_SETVAR}:{VAR_FREE_TEXT_SPACE}={VAR_FREE_TEXT_SPACE_COMMA}       { return p::make_ACTION_SETVAR(yytext, *driver.loc.back()); }
{ACTION_SEVERITY}:'{ACTION_SEVERITY_VALUE}'                             { return p::make_ACTION_SEVERITY(yytext, *driver.loc.back()); }
{ACTION_SEVERITY}:{ACTION_SEVERITY_VALUE}                               { return p::make_ACTION_SEVERITY(yytext, *driver.loc.back()); }
{ACTION_SKIP_AFTER}:{FREE_TEXT_SPACE_COMMA_QUOTE}                       { return p::make_ACTION_SKIP_AFTER(yytext, *driver.loc.back()); }
{ACTION_SKIP}:{CONFIG_VALUE_NUMBER}                                     { return p::make_ACTION_SKIP(yytext, *driver.loc.back()); }
{ACTION_TAG}:'{FREE_TEXT_QUOTE}'                                        { return p::make_ACTION_TAG(yytext, *driver.loc.back()); }
{ACTION_VER}:'{FREE_TEXT_QUOTE}'                                        { return p::make_ACTION_VER(yytext, *driver.loc.back()); }
{ACTION_XMLNS}:{FREE_TEXT_SPACE_COMMA_QUOTE}                            { return p::make_ACTION_XMLNS(yytext, *driver.loc.back()); }

{ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT}                               { return p::make_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_PARITY_ODD_7_BIT}                                { return p::make_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT}                               { return p::make_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_SQL_HEX_DECODE}                                  { return p::make_ACTION_TRANSFORMATION_SQL_HEX_DECODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_CMD_LINE}                                        { return p::make_ACTION_TRANSFORMATION_CMD_LINE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_SHA1}                                            { return p::make_ACTION_TRANSFORMATION_SHA1(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_MD5}                                             { return p::make_ACTION_TRANSFORMATION_MD5(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_HEX_ENCODE}                                      { return p::make_ACTION_TRANSFORMATION_HEX_ENCODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_LOWERCASE}                                       { return p::make_ACTION_TRANSFORMATION_LOWERCASE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_URL_DECODE_UNI}                                  { return p::make_ACTION_TRANSFORMATION_URL_DECODE_UNI(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_URL_DECODE}                                      { return p::make_ACTION_TRANSFORMATION_URL_DECODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_NONE}                                            { return p::make_ACTION_TRANSFORMATION_NONE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_COMPRESS_WHITESPACE}                             { return p::make_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REMOVE_WHITESPACE}                               { return p::make_ACTION_TRANSFORMATION_REMOVE_WHITESPACE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REPLACE_NULLS}                                   { return p::make_ACTION_TRANSFORMATION_REPLACE_NULLS(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REMOVE_NULLS}                                    { return p::make_ACTION_TRANSFORMATION_REMOVE_NULLS(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_HTML_ENTITY_DECODE}                              { return p::make_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_JS_DECODE}                                       { return p::make_ACTION_TRANSFORMATION_JS_DECODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_CSS_DECODE}                                      { return p::make_ACTION_TRANSFORMATION_CSS_DECODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_TRIM}                                            { return p::make_ACTION_TRANSFORMATION_TRIM(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_NORMALISE_PATH_WIN}                              { return p::make_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_NORMALISE_PATH}                                  { return p::make_ACTION_TRANSFORMATION_NORMALISE_PATH(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_LENGTH}                                          { return p::make_ACTION_TRANSFORMATION_LENGTH(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_UTF8_TO_UNICODE}                                 { return p::make_ACTION_TRANSFORMATION_UTF8_TO_UNICODE(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR}                            { return p::make_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REMOVE_COMMENTS}                                 { return p::make_ACTION_TRANSFORMATION_REMOVE_COMMENTS(yytext, *driver.loc.back()); }
{ACTION_TRANSFORMATION_REPLACE_COMMENTS}                                { return p::make_ACTION_TRANSFORMATION_REPLACE_COMMENTS(yytext, *driver.loc.back()); }


{ACTION_LOG_DATA}:'{FREE_TEXT_QUOTE}'                                   { return p::make_ACTION_LOG_DATA(yytext, *driver.loc.back()); }
{CONFIG_COMPONENT_SIG}[ \t]+["]{FREE_TEXT}["]                           { return p::make_CONFIG_COMPONENT_SIG(strchr(yytext, ' ') + 2, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_DIR_MOD}[ ]{CONFIG_VALUE_NUMBER}                      { return p::make_CONFIG_DIR_AUDIT_DIR_MOD(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_DIR}[ ]{CONFIG_VALUE_PATH}                            { return p::make_CONFIG_DIR_AUDIT_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_ENG}                                                  { return p::make_CONFIG_DIR_AUDIT_ENG(yytext, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_FLE_MOD}[ ]{CONFIG_VALUE_NUMBER}                      { return p::make_CONFIG_DIR_AUDIT_FLE_MOD(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG2}[ ]{CONFIG_VALUE_PATH}                           { return p::make_CONFIG_DIR_AUDIT_LOG2(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG_P}[ ]{AUDIT_PARTS}                                { return p::make_CONFIG_DIR_AUDIT_LOG_P(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_LOG}[ ]{CONFIG_VALUE_PATH}                            { return p::make_CONFIG_DIR_AUDIT_LOG(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_STS}[ ]["]{FREE_TEXT}["]                              { return p::make_CONFIG_DIR_AUDIT_STS(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_AUDIT_TPE}                                                  { return p::make_CONFIG_DIR_AUDIT_TPE(yytext, *driver.loc.back()); }
{CONFIG_DIR_DEBUG_LOG}[ ]{CONFIG_VALUE_PATH}                            { return p::make_CONFIG_DIR_DEBUG_LOG(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_DEBUG_LVL}[ ]{CONFIG_VALUE_NUMBER}                          { return p::make_CONFIG_DIR_DEBUG_LVL(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_GEO_DB}[ ]{FREE_TEXT_NEW_LINE}                              { return p::make_CONFIG_DIR_GEO_DB(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION}[ ]{CONFIG_VALUE_NUMBER}         { return p::make_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_PCRE_MATCH_LIMIT}[ ]{CONFIG_VALUE_NUMBER}                   { return p::make_CONFIG_DIR_PCRE_MATCH_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}           { return p::make_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_LIMIT_ACTION}                                      { return p::make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}                     { return p::make_CONFIG_DIR_REQ_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT}[ ]{CONFIG_VALUE_NUMBER}            { return p::make_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_REQ_BODY}                                                   { return p::make_CONFIG_DIR_REQ_BODY(yytext, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY_LIMIT_ACTION}                                      { return p::make_CONFIG_DIR_RES_BODY_LIMIT_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY_LIMIT}[ ]{CONFIG_VALUE_NUMBER}                     { return p::make_CONFIG_DIR_RES_BODY_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_RES_BODY}                                                   { return p::make_CONFIG_DIR_RES_BODY(yytext, *driver.loc.back()); }
{CONFIG_DIR_RULE_ENG}                                                   { return p::make_CONFIG_DIR_RULE_ENG(yytext, *driver.loc.back()); }
{CONFIG_DIR_SEC_ACTION}                                                 { return p::make_CONFIG_DIR_SEC_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_SEC_DEFAULT_ACTION}                                         { return p::make_CONFIG_DIR_SEC_DEFAULT_ACTION(yytext, *driver.loc.back()); }
{CONFIG_DIR_SEC_MARKER}[ \t]+["]{NEW_LINE_FREE_TEXT}["]                 { return p::make_CONFIG_DIR_SEC_MARKER(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_SEC_MARKER}[ \t]+{NEW_LINE_FREE_TEXT}                       { return p::make_CONFIG_DIR_SEC_MARKER(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_DIR_UNICODE_MAP_FILE}[ ]{FREE_TEXT_NEW_LINE}                    { return p::make_CONFIG_DIR_UNICODE_MAP_FILE(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_SEC_REMOVE_RULES_BY_ID}[ ]{FREE_TEXT_NEW_LINE}                  { return p::make_CONFIG_SEC_RULE_REMOVE_BY_ID(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_UPDLOAD_KEEP_FILES}                                             { return p::make_CONFIG_UPDLOAD_KEEP_FILES(yytext, *driver.loc.back()); }
{CONFIG_UPDLOAD_SAVE_TMP_FILES}                                         { return p::make_CONFIG_UPDLOAD_SAVE_TMP_FILES(yytext, *driver.loc.back()); }
{CONFIG_UPLOAD_DIR}[ ]{CONFIG_VALUE_PATH}                               { return p::make_CONFIG_UPLOAD_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_UPLOAD_FILE_LIMIT}[ ]{CONFIG_VALUE_NUMBER}                      { return p::make_CONFIG_UPLOAD_FILE_LIMIT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_UPLOAD_FILE_MODE}[ ]{CONFIG_VALUE_NUMBER}                       { return p::make_CONFIG_UPLOAD_FILE_MODE(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONFIG_VALUE_ABORT}                                                    { return p::make_CONFIG_VALUE_ABORT(yytext, *driver.loc.back()); }
{CONFIG_VALUE_DETC}                                                     { return p::make_CONFIG_VALUE_DETC(yytext, *driver.loc.back()); }
{CONFIG_VALUE_HTTPS}                                                    { return p::make_CONFIG_VALUE_HTTPS(yytext, *driver.loc.back()); }
{CONFIG_VALUE_OFF}                                                      { return p::make_CONFIG_VALUE_OFF(yytext, *driver.loc.back()); }
{CONFIG_VALUE_ON}                                                       { return p::make_CONFIG_VALUE_ON(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PARALLEL}                                                 { return p::make_CONFIG_VALUE_PARALLEL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_PROCESS_PARTIAL}                                          { return p::make_CONFIG_VALUE_PROCESS_PARTIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_REJECT}                                                   { return p::make_CONFIG_VALUE_REJECT(yytext, *driver.loc.back()); }
{CONFIG_VALUE_RELEVANT_ONLY}                                            { return p::make_CONFIG_VALUE_RELEVANT_ONLY(yytext, *driver.loc.back()); }
{CONFIG_VALUE_SERIAL}                                                   { return p::make_CONFIG_VALUE_SERIAL(yytext, *driver.loc.back()); }
{CONFIG_VALUE_WARN}                                                     { return p::make_CONFIG_VALUE_WARN(yytext, *driver.loc.back()); }
{CONFIG_XML_EXTERNAL_ENTITY}                                            { return p::make_CONFIG_XML_EXTERNAL_ENTITY(yytext, *driver.loc.back()); }
{CONGIG_DIR_RESPONSE_BODY_MP}[ ]{FREE_TEXT_NEW_LINE}                    { return p::make_CONGIG_DIR_RESPONSE_BODY_MP(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_ARG_SEP}[ ]{FREE_TEXT_NEW_LINE}                         { return p::make_CONGIG_DIR_SEC_ARG_SEP(yytext, *driver.loc.back()); }
{CONGIG_DIR_SEC_COOKIE_FORMAT}[ ]{CONFIG_VALUE_NUMBER}                  { return p::make_CONGIG_DIR_SEC_COOKIE_FORMAT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_DATA_DIR}[ ]{CONFIG_VALUE_PATH}                         { return p::make_CONGIG_DIR_SEC_DATA_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{CONGIG_DIR_SEC_STATUS_ENGINE}[ ]{FREE_TEXT_NEW_LINE}                   { return p::make_CONGIG_DIR_SEC_STATUS_ENGINE(yytext, *driver.loc.back()); }
{CONGIG_DIR_SEC_TMP_DIR}[ ]{CONFIG_VALUE_PATH}                          { return p::make_CONGIG_DIR_SEC_TMP_DIR(strchr(yytext, ' ') + 1, *driver.loc.back()); }
{DIRECTIVE_SECRULESCRIPT}[ ]{CONFIG_VALUE_PATH}                         { return p::make_DIRECTIVE_SECRULESCRIPT(yytext, *driver.loc.back()); }
{DIRECTIVE}                                                             { return p::make_DIRECTIVE(yytext, *driver.loc.back()); }
{CONFIG_SEC_REMOTE_RULES_FAIL_ACTION}                                   { return p::make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION(yytext, *driver.loc.back()); }
{CONFIG_SEC_COLLECTION_TIMEOUT}[ ]{CONFIG_VALUE_NUMBER}                 { return p::make_CONFIG_SEC_COLLECTION_TIMEOUT(strchr(yytext, ' ') + 1, *driver.loc.back()); }
[ \t]*[\n]                                                              { driver.loc.back()->lines(1); driver.loc.back()->step(); }
#[ \t]*SecRule[^\\].*\\[ \t]*[\r\n]*                                    { driver.loc.back()->lines(1); driver.loc.back()->step(); BEGIN(COMMENT); }
#[ \t]*SecAction[^\\].*\\[ \t]*[^\\n]                                   { driver.loc.back()->lines(1); driver.loc.back()->step(); BEGIN(COMMENT);  }
#.*                                                                     { driver.loc.back()->step(); /* comment, just ignore. */ }
\r                                                                      { driver.loc.back()->step(); /* carriage return, just ignore. */}
["]                                                                     { return p::make_QUOTATION_MARK(yytext, *driver.loc.back()); }
[,]                                                                     { return p::make_COMMA(*driver.loc.back()); }



<INITIAL,EXPECTING_VARIABLE>{
[!&]?{RUN_TIME_VAR_BLD}                                                 { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_BLD(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_DUR}                                                 { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_DUR(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_ENV}(\:[\']{FREE_TEXT_QUOTE}[\'])?                   { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_ENV}(\:{DICT_ELEMENT})?                              { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_HSV}                                                 { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_HSV(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_REMOTE_USER}                                         { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_REMOTE_USER(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_RULE}(\:[\']{FREE_TEXT_QUOTE}[\'])?                  { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_RULE}(\:{DICT_ELEMENT})?                             { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_DAY}                                            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_DAY(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_EPOCH}                                          { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_EPOCH(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_HOUR}                                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_HOUR(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_MIN}                                            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_MIN(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_MON}                                            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_MON(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_SEC}                                            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_SEC(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME_YEAR}                                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_YEAR(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_TIME}                                                { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_XML}(\:[\']{FREE_TEXT_QUOTE}[\'])?                   { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
[!&]?{RUN_TIME_VAR_XML}(\:{DICT_ELEMENT})?                              { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
[!&]?{VARIABLENOCOLON}                                                  { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_COL}(\:[\']{FREE_TEXT_QUOTE}[\'])?                       { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_COL(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_COL}(\:{DICT_ELEMENT})?                                  { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_COL(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_STATUS}                                                  { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_STATUS(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_TX}(\:[\']{FREE_TEXT_QUOTE}[\'])?                        { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_TX(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_TX}(\:{DICT_ELEMENT})?                                   { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_TX(yytext, *driver.loc.back()); }
[!&]?{VARIABLE_WEBSERVER_ERROR_LOG}                                     { driver.error (*driver.loc.back(), "Variable VARIABLE_WEBSERVER_ERROR_LOG is not supported by libModSecurity", ""); throw p::syntax_error(*driver.loc.back(), "");}
[!&]?{VARIABLE}(\:[\']{FREE_TEXT_QUOTE}[\'])?                           { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
[!&]?{VARIABLE}(\:{DICT_ELEMENT})?                                      { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_BLD}["]                                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_BLD(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_DUR}["]                                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_DUR(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_ENV}(\:\'{FREE_TEXT_QUOTE}[\'])?["]               { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_ENV}(\:{DICT_ELEMENT})?["]                        { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_ENV(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_HSV}["]                                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_HSV(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_RULE}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_RULE}(\:{DICT_ELEMENT})?                          { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_RULE(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_XML}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]             { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
["][!&]?{RUN_TIME_VAR_XML}(\:{DICT_ELEMENT})?                           { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_XML(yytext, *driver.loc.back()); }
["][!&]?{VARIABLENOCOLON}["]                                            { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_COL}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]                 { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_COL(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_COL}(\:{DICT_ELEMENT})?                               { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_COL(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_TX}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]                  { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_TX(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE_TX}(\:{DICT_ELEMENT})?["]                             { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE_TX(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE}(\:[\']{FREE_TEXT_QUOTE}[\'])?["]                     { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
["][!&]?{VARIABLE}(\:{DICT_ELEMENT})?["]                                { BEGIN(EXPECTING_OPERATOR); return p::make_VARIABLE(yytext, *driver.loc.back()); }
[&]?{RUN_TIME_VAR_TIME_WDAY}                                            { BEGIN(EXPECTING_OPERATOR); return p::make_RUN_TIME_VAR_TIME_WDAY(yytext, *driver.loc.back()); }
}



<EXPECTING_OPERATOR>{
["]{OPERATOR}[ ]{FREE_TEXT}["]                                          { BEGIN(INITIAL); return p::make_OPERATOR(yytext, *driver.loc.back()); }
["]{OPERATORNOARG}[\t ]*["]                                             { BEGIN(INITIAL); return p::make_OPERATOR(yytext, *driver.loc.back()); }
["]{OPERATOR_GEOIP}[\t ]*["]                                            { BEGIN(INITIAL); return p::make_OPERATOR_GEOIP(yytext, *driver.loc.back()); }
{SOMETHING}                                                             { BEGIN(INITIAL); return p::make_FREE_TEXT(yytext, *driver.loc.back()); }
}


<INITIAL,EXPECTING_OPERATOR,EXPECTING_VARIABLE>{
[|]                                                                     { BEGIN(EXPECTING_VARIABLE); return p::make_PIPE(*driver.loc.back()); }
}

<INITIAL,EXPECTING_OPERATOR>{
[ \t]+                                                                  {  }
[ \t]*\\\n[ \t]*                                                        { driver.loc.back()->lines(1); driver.loc.back()->step(); }
[ \t]*\\\r\n[ \t]*                                                      { driver.loc.back()->lines(1); driver.loc.back()->step(); }
}

<COMMENT>{
.*[ \t]*\\\n[ \t]*                                                      { driver.loc.back()->lines(1); driver.loc.back()->step(); }
.*[ \t]*\\\r\n[ \t]*                                                    { driver.loc.back()->lines(1); driver.loc.back()->step(); }
.*[^\\]                                                                 { BEGIN(INITIAL); driver.loc.back()->lines(1); driver.loc.back()->step(); }
}



.                                                                       { driver.error (*driver.loc.back(), "invalid character", yytext); throw p::syntax_error(*driver.loc.back(), ""); }
<<EOF>> {
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
    if (!YY_CURRENT_BUFFER) {
        return p::make_END(*driver.loc.back());
    }
}


{CONFIG_INCLUDE}[ ]{CONFIG_VALUE_PATH} {
    const char *file = strchr(yytext, ' ') + 1;
    std::string fi = modsecurity::utils::find_resource(file, driver.ref.back());
    if (fi.empty() == true) {
        BEGIN(INITIAL);
        driver.error (*driver.loc.back(), "", file + std::string(": Not able to open file."));
        throw p::syntax_error(*driver.loc.back(), "");
    }
    std::list<std::string> files = modsecurity::utils::expandEnv(fi, 0);
    files.reverse();
    for (auto& s: files) {
        std::string f = modsecurity::utils::find_resource(s, driver.ref.back());
        yyin = fopen(f.c_str(), "r" );
        if (!yyin) {
            BEGIN(INITIAL);
            driver.error (*driver.loc.back(), "", s + std::string(": Not able to open file."));
            throw p::syntax_error(*driver.loc.back(), "");
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
    std::string fi = modsecurity::utils::find_resource(f, driver.ref.back());
    if (fi.empty() == true) {
        BEGIN(INITIAL);
        driver.error (*driver.loc.back(), "", file + std::string(": Not able to open file."));
        throw p::syntax_error(*driver.loc.back(), "");
    }
    std::list<std::string> files = modsecurity::utils::expandEnv(fi, 0);
    files.reverse();
    for (auto& s: files) {
        std::string f = modsecurity::utils::find_resource(s, driver.ref.back());
        yyin = fopen(f.c_str(), "r" );
        if (!yyin) {
            BEGIN(INITIAL);
            driver.error (*driver.loc.back(), "", s + std::string(": Not able to open file."));
            throw p::syntax_error(*driver.loc.back(), "");
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

    std::vector<std::string> conf = modsecurity::utils::string::split(yytext, ' ');
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
        if (driver.m_remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction) {
            /** TODO: Implement the server logging mechanism. */
        }
        if (driver.m_remoteRulesActionOnFailed == Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction) {
            driver.error (*driver.loc.back(), "", yytext + std::string(" - Failed to download: ") + c.error);
            throw p::syntax_error(*driver.loc.back(), "");
        }
    }

    yy_scan_string(c.content.c_str());
}


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

