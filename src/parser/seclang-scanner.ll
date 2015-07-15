%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "parser/driver.h"
# include "seclang-parser.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
%}
%option noyywrap nounput batch debug noinput
ACTION          (?i:accuracy|allow|append|auditlog|block|capture|chain|ctl|deny|deprecatevar|drop|exec|expirevar|id:[0-9]+|initcol|log|logdata|maturity|msg|multiMatch|noauditlog|nolog|pass|pause|phase:[0-9]+|prepend|proxy|redirect:[A-z0-9_\|\&\:\/\/\.]+|rev|sanitiseArg|sanitiseMatched|sanitiseMatchedBytes|sanitiseRequestHeader|sanitiseResponseHeader|severity|setuid|setrsc|setsid|setenv|setvar|skip|skipAfter|status:[0-9]+|tag|ver|xmlns|t)

DIRECTIVE       SecRule

CONFIG_DIRECTIVE SecRequestBodyLimitAction|SecRequestBodyNoFilesLimit|SecRequestBodyInMemoryLimit|SecRequestBodyLimit|SecPcreMatchLimitRecursion|SecPcreMatchLimit|SecResponseBodyMimeType|SecResponseBodyLimitAction|SecResponseBodyLimit|SecTmpDir|SecDataDir|SecArgumentSeparator|SecCookieFormat|SecStatusEngine


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

CONFIG_INCLUDE  Include
DICT_ELEMENT    [A-Za-z_]+


OPERATOR        (?i:(?:@inspectFile|@fuzzyHash|@validateByteRange|@validateDTD|@validateHash|@validateSchema|@verifyCC|@verifyCPF|@verifySSN|@gsbLookup|@rsub)|(?:\!{0,1})(?:@within|@containsWord|@contains|@endsWith|@eq|@ge|@gt|@ipMatchF|@ipMatch|@ipMatchFromFile|@le|@lt|@pmf|@pm|@pmFromFile|@rbl|@rx|@streq|@strmatch|@beginsWith))

OPERATORNOARG	(?i:@detectSQLi|@detectXSS|@geoLookup|@validateUrlEncoding|@validateUtf8Encoding)


TRANSFORMATION  t:(lowercase|urlDecodeUni|urlDecode|none|compressWhitespace|removeWhitespace|replaceNulls|removeNulls|htmlEntityDecode|jsDecode|cssDecode|trim)

VARIABLE          (?i:AUTH_TYPE|ARGS_NAMES|ARGS|QUERY_STRING|REMOTE_ADDR|REQUEST_BASENAME|REQUEST_BODY|REQUEST_COOKIES_NAMES|REQUEST_COOKIES|REQUEST_FILENAME|REQUEST_HEADERS_NAMES|REQUEST_HEADERS|REQUEST_METHOD|REQUEST_PROTOCOL|REQUEST_URI|RESPONSE_BODY|RESPONSE_CONTENT_LENGTH|RESPONSE_CONTENT_TYPE|RESPONSE_HEADERS_NAMES|RESPONSE_HEADERS|RESPONSE_PROTOCOL|RESPONSE_STATUS|TX)
RUN_TIME_VAR_DUR  (?i:DURATION)
RUN_TIME_VAR_ENV  (?i:ENV)

VARIABLENOCOLON  (?i:REQBODY_ERROR|MULTIPART_STRICT_ERROR|MULTIPART_UNMATCHED_BOUNDARY|REMOTE_ADDR|REQUEST_LINE)

CONFIG_VALUE_ON On
CONFIG_VALUE_OFF    Off
CONFIG_VALUE_DETC    DetectOnly
CONFIG_VALUE_SERIAL Serial
CONFIG_VALUE_PARALLEL Parallel
CONFIG_VALUE_RELEVANT_ONLY RelevantOnly

CONFIG_VALUE_PATH    [A-Za-z_/\.]+
AUDIT_PARTS [ABCDEFHJKZ]+
CONFIG_VALUE_NUMBER [0-9]+

FREE_TEXT       [^\"]+

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

{DIRECTIVE}                     { return yy::seclang_parser::make_DIRECTIVE(yytext, loc); }
{TRANSFORMATION}                { return yy::seclang_parser::make_TRANSFORMATION(yytext, loc); }
{CONFIG_DIR_RULE_ENG}           { return yy::seclang_parser::make_CONFIG_DIR_RULE_ENG(yytext, loc); }
{CONFIG_DIR_RES_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_RES_BODY(yytext, loc); }
{CONFIG_DIR_REQ_BODY}           { return yy::seclang_parser::make_CONFIG_DIR_REQ_BODY(yytext, loc); }

%{ /* Audit log entries */ %}
{CONFIG_DIR_AUDIT_DIR}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_DIR_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_DIR_MOD(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_ENG}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_ENG(yytext, loc); }
{CONFIG_DIR_AUDIT_FLE_MOD}[ ]{CONFIG_VALUE_NUMBER} { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_FLE_MOD(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_LOG2}[ ]{CONFIG_VALUE_PATH}      { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG2(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_LOG}[ ]{CONFIG_VALUE_PATH}       { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_LOG_P}[ ]{AUDIT_PARTS}           { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_LOG_P(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_STS}[ ]["]{FREE_TEXT}["]         { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_STS(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_AUDIT_TPE}                             { return yy::seclang_parser::make_CONFIG_DIR_AUDIT_TPE(yytext, loc); }

%{ /* Debug log entries */ %}
{CONFIG_DIR_DEBUG_LOG}[ ]{CONFIG_VALUE_PATH}    { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LOG(strchr(yytext, ' ') + 1, loc); }
{CONFIG_DIR_DEBUG_LVL}[ ]{CONFIG_VALUE_NUMBER}  { return yy::seclang_parser::make_CONFIG_DIR_DEBUG_LVL(strchr(yytext, ' ') + 1, loc); }

%{ /* Variables */ %}
{VARIABLE}:?{DICT_ELEMENT}?             { return yy::seclang_parser::make_VARIABLE(yytext, loc); }
{RUN_TIME_VAR_DUR}                      { return yy::seclang_parser::make_RUN_TIME_VAR_DUR(yytext, loc); }
{RUN_TIME_VAR_ENV}:?{DICT_ELEMENT}?     { return yy::seclang_parser::make_RUN_TIME_VAR_ENV(yytext, loc); }


{CONFIG_COMPONENT_SIG}[ ]["]{FREE_TEXT}["] { return yy::seclang_parser::make_CONFIG_COMPONENT_SIG(strchr(yytext, ' ') + 2, loc); }

{CONFIG_VALUE_ON}               { return yy::seclang_parser::make_CONFIG_VALUE_ON(yytext, loc); }
{CONFIG_VALUE_OFF}              { return yy::seclang_parser::make_CONFIG_VALUE_OFF(yytext, loc); }
{CONFIG_VALUE_SERIAL}           { return yy::seclang_parser::make_CONFIG_VALUE_SERIAL(yytext, loc); }
{CONFIG_VALUE_PARALLEL}         { return yy::seclang_parser::make_CONFIG_VALUE_PARALLEL(yytext, loc); }
{CONFIG_VALUE_DETC}             { return yy::seclang_parser::make_CONFIG_VALUE_DETC(yytext, loc); }
{CONFIG_VALUE_RELEVANT_ONLY}    { return yy::seclang_parser::make_CONFIG_VALUE_RELEVANT_ONLY(yytext, loc); }
["]{OPERATOR}[ ]{FREE_TEXT}["]  { return yy::seclang_parser::make_OPERATOR(yytext, loc); }
["]{OPERATORNOARG}["]           { return yy::seclang_parser::make_OPERATOR(yytext, loc); }
{ACTION}                        { return yy::seclang_parser::make_ACTION(yytext, loc); }
["]                             { return yy::seclang_parser::make_QUOTATION_MARK(loc); }
[,]                             { return yy::seclang_parser::make_COMMA(loc); }
[|]                             { return yy::seclang_parser::make_PIPE(loc); }
{VARIABLENOCOLON}	   	{ return yy::seclang_parser::make_VARIABLE(yytext, loc); }
[ \t]+                          { return yy::seclang_parser::make_SPACE(loc); }
\n                              { return yy::seclang_parser::make_NEW_LINE(loc); }
.                               driver.error (loc, "invalid character");
<<EOF>>                         { return yy::seclang_parser::make_END(loc); }

%%

void
Driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      exit (EXIT_FAILURE);
    }
}



void
Driver::scan_end ()
{
  fclose (yyin);
}

