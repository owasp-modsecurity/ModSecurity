// A Bison parser, made by GNU Bison 3.7.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "seclang-parser.hh"


// Unqualified %code blocks.
#line 326 "seclang-parser.yy"

#include "src/parser/driver.h"

#line 50 "seclang-parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 142 "seclang-parser.cc"

  /// Build a parser object.
  seclang_parser::seclang_parser (modsecurity::Parser::Driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  seclang_parser::~seclang_parser ()
  {}

  seclang_parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  seclang_parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  seclang_parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  seclang_parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  seclang_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  seclang_parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  seclang_parser::symbol_kind_type
  seclang_parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  seclang_parser::stack_symbol_type::stack_symbol_type ()
  {}

  seclang_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_ACTION_ACCURACY: // "Accuracy"
      case symbol_kind::S_ACTION_ALLOW: // "Allow"
      case symbol_kind::S_ACTION_APPEND: // "Append"
      case symbol_kind::S_ACTION_AUDIT_LOG: // "AuditLog"
      case symbol_kind::S_ACTION_BLOCK: // "Block"
      case symbol_kind::S_ACTION_CAPTURE: // "Capture"
      case symbol_kind::S_ACTION_CHAIN: // "Chain"
      case symbol_kind::S_ACTION_CTL_AUDIT_ENGINE: // "ACTION_CTL_AUDIT_ENGINE"
      case symbol_kind::S_ACTION_CTL_AUDIT_LOG_PARTS: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case symbol_kind::S_ACTION_CTL_BDY_JSON: // "ACTION_CTL_BDY_JSON"
      case symbol_kind::S_ACTION_CTL_BDY_XML: // "ACTION_CTL_BDY_XML"
      case symbol_kind::S_ACTION_CTL_BDY_URLENCODED: // "ACTION_CTL_BDY_URLENCODED"
      case symbol_kind::S_ACTION_CTL_FORCE_REQ_BODY_VAR: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case symbol_kind::S_ACTION_CTL_REQUEST_BODY_ACCESS: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_ID: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_TAG: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case symbol_kind::S_ACTION_DENY: // "Deny"
      case symbol_kind::S_ACTION_DEPRECATE_VAR: // "DeprecateVar"
      case symbol_kind::S_ACTION_DROP: // "Drop"
      case symbol_kind::S_ACTION_EXEC: // "Exec"
      case symbol_kind::S_ACTION_EXPIRE_VAR: // "ExpireVar"
      case symbol_kind::S_ACTION_ID: // "Id"
      case symbol_kind::S_ACTION_INITCOL: // "InitCol"
      case symbol_kind::S_ACTION_LOG: // "Log"
      case symbol_kind::S_ACTION_LOG_DATA: // "LogData"
      case symbol_kind::S_ACTION_MATURITY: // "Maturity"
      case symbol_kind::S_ACTION_MSG: // "Msg"
      case symbol_kind::S_ACTION_MULTI_MATCH: // "MultiMatch"
      case symbol_kind::S_ACTION_NO_AUDIT_LOG: // "NoAuditLog"
      case symbol_kind::S_ACTION_NO_LOG: // "NoLog"
      case symbol_kind::S_ACTION_PASS: // "Pass"
      case symbol_kind::S_ACTION_PAUSE: // "Pause"
      case symbol_kind::S_ACTION_PHASE: // "Phase"
      case symbol_kind::S_ACTION_PREPEND: // "Prepend"
      case symbol_kind::S_ACTION_PROXY: // "Proxy"
      case symbol_kind::S_ACTION_REDIRECT: // "Redirect"
      case symbol_kind::S_ACTION_REV: // "Rev"
      case symbol_kind::S_ACTION_SANITISE_ARG: // "SanitiseArg"
      case symbol_kind::S_ACTION_SANITISE_MATCHED: // "SanitiseMatched"
      case symbol_kind::S_ACTION_SANITISE_MATCHED_BYTES: // "SanitiseMatchedBytes"
      case symbol_kind::S_ACTION_SANITISE_REQUEST_HEADER: // "SanitiseRequestHeader"
      case symbol_kind::S_ACTION_SANITISE_RESPONSE_HEADER: // "SanitiseResponseHeader"
      case symbol_kind::S_ACTION_SETENV: // "SetEnv"
      case symbol_kind::S_ACTION_SETRSC: // "SetRsc"
      case symbol_kind::S_ACTION_SETSID: // "SetSid"
      case symbol_kind::S_ACTION_SETUID: // "SetUID"
      case symbol_kind::S_ACTION_SEVERITY: // "Severity"
      case symbol_kind::S_ACTION_SKIP: // "Skip"
      case symbol_kind::S_ACTION_SKIP_AFTER: // "SkipAfter"
      case symbol_kind::S_ACTION_STATUS: // "Status"
      case symbol_kind::S_ACTION_TAG: // "Tag"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_ENCODE: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case symbol_kind::S_ACTION_TRANSFORMATION_CMD_LINE: // "ACTION_TRANSFORMATION_CMD_LINE"
      case symbol_kind::S_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_CSS_DECODE: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_ENCODE: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_DECODE: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_JS_DECODE: // "ACTION_TRANSFORMATION_JS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_LENGTH: // "ACTION_TRANSFORMATION_LENGTH"
      case symbol_kind::S_ACTION_TRANSFORMATION_LOWERCASE: // "ACTION_TRANSFORMATION_LOWERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_PHP_ARGS_NAMES: // "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
      case symbol_kind::S_ACTION_TRANSFORMATION_MD5: // "ACTION_TRANSFORMATION_MD5"
      case symbol_kind::S_ACTION_TRANSFORMATION_NONE: // "ACTION_TRANSFORMATION_NONE"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_NULLS: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_WHITESPACE: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_COMMENTS: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_NULLS: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_SHA1: // "ACTION_TRANSFORMATION_SHA1"
      case symbol_kind::S_ACTION_TRANSFORMATION_SQL_HEX_DECODE: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM: // "ACTION_TRANSFORMATION_TRIM"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_LEFT: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_RIGHT: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case symbol_kind::S_ACTION_TRANSFORMATION_UPPERCASE: // "ACTION_TRANSFORMATION_UPPERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_ENCODE: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE: // "ACTION_TRANSFORMATION_URL_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE_UNI: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case symbol_kind::S_ACTION_TRANSFORMATION_UTF8_TO_UNICODE: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case symbol_kind::S_ACTION_VER: // "Ver"
      case symbol_kind::S_ACTION_XMLNS: // "xmlns"
      case symbol_kind::S_CONFIG_COMPONENT_SIG: // "CONFIG_COMPONENT_SIG"
      case symbol_kind::S_CONFIG_CONN_ENGINE: // "CONFIG_CONN_ENGINE"
      case symbol_kind::S_CONFIG_SEC_ARGUMENT_SEPARATOR: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case symbol_kind::S_CONFIG_SEC_WEB_APP_ID: // "CONFIG_SEC_WEB_APP_ID"
      case symbol_kind::S_CONFIG_SEC_SERVER_SIG: // "CONFIG_SEC_SERVER_SIG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR: // "CONFIG_DIR_AUDIT_DIR"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR_MOD: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_ENG: // "CONFIG_DIR_AUDIT_ENG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_FLE_MOD: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG: // "CONFIG_DIR_AUDIT_LOG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG2: // "CONFIG_DIR_AUDIT_LOG2"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG_P: // "CONFIG_DIR_AUDIT_LOG_P"
      case symbol_kind::S_CONFIG_DIR_AUDIT_STS: // "CONFIG_DIR_AUDIT_STS"
      case symbol_kind::S_CONFIG_DIR_AUDIT_TPE: // "CONFIG_DIR_AUDIT_TPE"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LOG: // "CONFIG_DIR_DEBUG_LOG"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LVL: // "CONFIG_DIR_DEBUG_LVL"
      case symbol_kind::S_CONFIG_SEC_CACHE_TRANSFORMATIONS: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case symbol_kind::S_CONFIG_SEC_DISABLE_BACKEND_COMPRESS: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case symbol_kind::S_CONFIG_SEC_HASH_ENGINE: // "CONFIG_SEC_HASH_ENGINE"
      case symbol_kind::S_CONFIG_SEC_HASH_KEY: // "CONFIG_SEC_HASH_KEY"
      case symbol_kind::S_CONFIG_SEC_HASH_PARAM: // "CONFIG_SEC_HASH_PARAM"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_RX: // "CONFIG_SEC_HASH_METHOD_RX"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_PM: // "CONFIG_SEC_HASH_METHOD_PM"
      case symbol_kind::S_CONFIG_SEC_CHROOT_DIR: // "CONFIG_SEC_CHROOT_DIR"
      case symbol_kind::S_CONFIG_DIR_GEO_DB: // "CONFIG_DIR_GEO_DB"
      case symbol_kind::S_CONFIG_DIR_GSB_DB: // "CONFIG_DIR_GSB_DB"
      case symbol_kind::S_CONFIG_SEC_GUARDIAN_LOG: // "CONFIG_SEC_GUARDIAN_LOG"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case symbol_kind::S_CONFIG_SEC_CONN_R_STATE_LIMIT: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_CONN_W_STATE_LIMIT: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_SENSOR_ID: // "CONFIG_SEC_SENSOR_ID"
      case symbol_kind::S_CONFIG_DIR_ARGS_LIMIT: // "CONFIG_DIR_ARGS_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY: // "CONFIG_DIR_REQ_BODY"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT_ACTION: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY: // "CONFIG_DIR_RES_BODY"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT: // "CONFIG_DIR_RES_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT_ACTION: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_INHERITANCE: // "CONFIG_SEC_RULE_INHERITANCE"
      case symbol_kind::S_CONFIG_SEC_RULE_PERF_TIME: // "CONFIG_SEC_RULE_PERF_TIME"
      case symbol_kind::S_CONFIG_DIR_RULE_ENG: // "CONFIG_DIR_RULE_ENG"
      case symbol_kind::S_CONFIG_DIR_SEC_ACTION: // "CONFIG_DIR_SEC_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_DEFAULT_ACTION: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_MARKER: // "CONFIG_DIR_SEC_MARKER"
      case symbol_kind::S_CONFIG_DIR_UNICODE_MAP_FILE: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case symbol_kind::S_CONFIG_DIR_UNICODE_CODE_PAGE: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case symbol_kind::S_CONFIG_SEC_COLLECTION_TIMEOUT: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case symbol_kind::S_CONFIG_SEC_HTTP_BLKEY: // "CONFIG_SEC_HTTP_BLKEY"
      case symbol_kind::S_CONFIG_SEC_INTERCEPT_ON_ERROR: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case symbol_kind::S_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_ID: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_MSG: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_TAG: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case symbol_kind::S_CONFIG_UPDLOAD_KEEP_FILES: // "CONFIG_UPDLOAD_KEEP_FILES"
      case symbol_kind::S_CONFIG_UPDLOAD_SAVE_TMP_FILES: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case symbol_kind::S_CONFIG_UPLOAD_DIR: // "CONFIG_UPLOAD_DIR"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_LIMIT: // "CONFIG_UPLOAD_FILE_LIMIT"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_MODE: // "CONFIG_UPLOAD_FILE_MODE"
      case symbol_kind::S_CONFIG_VALUE_ABORT: // "CONFIG_VALUE_ABORT"
      case symbol_kind::S_CONFIG_VALUE_DETC: // "CONFIG_VALUE_DETC"
      case symbol_kind::S_CONFIG_VALUE_HTTPS: // "CONFIG_VALUE_HTTPS"
      case symbol_kind::S_CONFIG_VALUE_OFF: // "CONFIG_VALUE_OFF"
      case symbol_kind::S_CONFIG_VALUE_ON: // "CONFIG_VALUE_ON"
      case symbol_kind::S_CONFIG_VALUE_PARALLEL: // "CONFIG_VALUE_PARALLEL"
      case symbol_kind::S_CONFIG_VALUE_PROCESS_PARTIAL: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case symbol_kind::S_CONFIG_VALUE_REJECT: // "CONFIG_VALUE_REJECT"
      case symbol_kind::S_CONFIG_VALUE_RELEVANT_ONLY: // "CONFIG_VALUE_RELEVANT_ONLY"
      case symbol_kind::S_CONFIG_VALUE_SERIAL: // "CONFIG_VALUE_SERIAL"
      case symbol_kind::S_CONFIG_VALUE_WARN: // "CONFIG_VALUE_WARN"
      case symbol_kind::S_CONFIG_XML_EXTERNAL_ENTITY: // "CONFIG_XML_EXTERNAL_ENTITY"
      case symbol_kind::S_CONGIG_DIR_RESPONSE_BODY_MP: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case symbol_kind::S_CONGIG_DIR_SEC_ARG_SEP: // "CONGIG_DIR_SEC_ARG_SEP"
      case symbol_kind::S_CONGIG_DIR_SEC_COOKIE_FORMAT: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case symbol_kind::S_CONFIG_SEC_COOKIEV0_SEPARATOR: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case symbol_kind::S_CONGIG_DIR_SEC_DATA_DIR: // "CONGIG_DIR_SEC_DATA_DIR"
      case symbol_kind::S_CONGIG_DIR_SEC_STATUS_ENGINE: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case symbol_kind::S_CONFIG_SEC_STREAM_IN_BODY_INSPECTION: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case symbol_kind::S_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case symbol_kind::S_CONGIG_DIR_SEC_TMP_DIR: // "CONGIG_DIR_SEC_TMP_DIR"
      case symbol_kind::S_DIRECTIVE: // "DIRECTIVE"
      case symbol_kind::S_DIRECTIVE_SECRULESCRIPT: // "DIRECTIVE_SECRULESCRIPT"
      case symbol_kind::S_FREE_TEXT_QUOTE_MACRO_EXPANSION: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case symbol_kind::S_QUOTATION_MARK: // "QUOTATION_MARK"
      case symbol_kind::S_RUN_TIME_VAR_BLD: // "RUN_TIME_VAR_BLD"
      case symbol_kind::S_RUN_TIME_VAR_DUR: // "RUN_TIME_VAR_DUR"
      case symbol_kind::S_RUN_TIME_VAR_HSV: // "RUN_TIME_VAR_HSV"
      case symbol_kind::S_RUN_TIME_VAR_REMOTE_USER: // "RUN_TIME_VAR_REMOTE_USER"
      case symbol_kind::S_RUN_TIME_VAR_TIME: // "RUN_TIME_VAR_TIME"
      case symbol_kind::S_RUN_TIME_VAR_TIME_DAY: // "RUN_TIME_VAR_TIME_DAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_EPOCH: // "RUN_TIME_VAR_TIME_EPOCH"
      case symbol_kind::S_RUN_TIME_VAR_TIME_HOUR: // "RUN_TIME_VAR_TIME_HOUR"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MIN: // "RUN_TIME_VAR_TIME_MIN"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MON: // "RUN_TIME_VAR_TIME_MON"
      case symbol_kind::S_RUN_TIME_VAR_TIME_SEC: // "RUN_TIME_VAR_TIME_SEC"
      case symbol_kind::S_RUN_TIME_VAR_TIME_WDAY: // "RUN_TIME_VAR_TIME_WDAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_YEAR: // "RUN_TIME_VAR_TIME_YEAR"
      case symbol_kind::S_VARIABLE: // "VARIABLE"
      case symbol_kind::S_DICT_ELEMENT: // "Dictionary element"
      case symbol_kind::S_DICT_ELEMENT_REGEXP: // "Dictionary element, selected by regexp"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_op: // op
      case symbol_kind::S_op_before_init: // op_before_init
        value.YY_MOVE_OR_COPY< std::unique_ptr<Operator> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_run_time_string: // run_time_string
        value.YY_MOVE_OR_COPY< std::unique_ptr<RunTimeString> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_var: // var
        value.YY_MOVE_OR_COPY< std::unique_ptr<Variable> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_act: // act
      case symbol_kind::S_setvar_action: // setvar_action
        value.YY_MOVE_OR_COPY< std::unique_ptr<actions::Action> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variables: // variables
      case symbol_kind::S_variables_pre_process: // variables_pre_process
      case symbol_kind::S_variables_may_be_quoted: // variables_may_be_quoted
        value.YY_MOVE_OR_COPY< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_actions: // actions
      case symbol_kind::S_actions_may_quoted: // actions_may_quoted
        value.YY_MOVE_OR_COPY< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  seclang_parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_ACTION_ACCURACY: // "Accuracy"
      case symbol_kind::S_ACTION_ALLOW: // "Allow"
      case symbol_kind::S_ACTION_APPEND: // "Append"
      case symbol_kind::S_ACTION_AUDIT_LOG: // "AuditLog"
      case symbol_kind::S_ACTION_BLOCK: // "Block"
      case symbol_kind::S_ACTION_CAPTURE: // "Capture"
      case symbol_kind::S_ACTION_CHAIN: // "Chain"
      case symbol_kind::S_ACTION_CTL_AUDIT_ENGINE: // "ACTION_CTL_AUDIT_ENGINE"
      case symbol_kind::S_ACTION_CTL_AUDIT_LOG_PARTS: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case symbol_kind::S_ACTION_CTL_BDY_JSON: // "ACTION_CTL_BDY_JSON"
      case symbol_kind::S_ACTION_CTL_BDY_XML: // "ACTION_CTL_BDY_XML"
      case symbol_kind::S_ACTION_CTL_BDY_URLENCODED: // "ACTION_CTL_BDY_URLENCODED"
      case symbol_kind::S_ACTION_CTL_FORCE_REQ_BODY_VAR: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case symbol_kind::S_ACTION_CTL_REQUEST_BODY_ACCESS: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_ID: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_TAG: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case symbol_kind::S_ACTION_DENY: // "Deny"
      case symbol_kind::S_ACTION_DEPRECATE_VAR: // "DeprecateVar"
      case symbol_kind::S_ACTION_DROP: // "Drop"
      case symbol_kind::S_ACTION_EXEC: // "Exec"
      case symbol_kind::S_ACTION_EXPIRE_VAR: // "ExpireVar"
      case symbol_kind::S_ACTION_ID: // "Id"
      case symbol_kind::S_ACTION_INITCOL: // "InitCol"
      case symbol_kind::S_ACTION_LOG: // "Log"
      case symbol_kind::S_ACTION_LOG_DATA: // "LogData"
      case symbol_kind::S_ACTION_MATURITY: // "Maturity"
      case symbol_kind::S_ACTION_MSG: // "Msg"
      case symbol_kind::S_ACTION_MULTI_MATCH: // "MultiMatch"
      case symbol_kind::S_ACTION_NO_AUDIT_LOG: // "NoAuditLog"
      case symbol_kind::S_ACTION_NO_LOG: // "NoLog"
      case symbol_kind::S_ACTION_PASS: // "Pass"
      case symbol_kind::S_ACTION_PAUSE: // "Pause"
      case symbol_kind::S_ACTION_PHASE: // "Phase"
      case symbol_kind::S_ACTION_PREPEND: // "Prepend"
      case symbol_kind::S_ACTION_PROXY: // "Proxy"
      case symbol_kind::S_ACTION_REDIRECT: // "Redirect"
      case symbol_kind::S_ACTION_REV: // "Rev"
      case symbol_kind::S_ACTION_SANITISE_ARG: // "SanitiseArg"
      case symbol_kind::S_ACTION_SANITISE_MATCHED: // "SanitiseMatched"
      case symbol_kind::S_ACTION_SANITISE_MATCHED_BYTES: // "SanitiseMatchedBytes"
      case symbol_kind::S_ACTION_SANITISE_REQUEST_HEADER: // "SanitiseRequestHeader"
      case symbol_kind::S_ACTION_SANITISE_RESPONSE_HEADER: // "SanitiseResponseHeader"
      case symbol_kind::S_ACTION_SETENV: // "SetEnv"
      case symbol_kind::S_ACTION_SETRSC: // "SetRsc"
      case symbol_kind::S_ACTION_SETSID: // "SetSid"
      case symbol_kind::S_ACTION_SETUID: // "SetUID"
      case symbol_kind::S_ACTION_SEVERITY: // "Severity"
      case symbol_kind::S_ACTION_SKIP: // "Skip"
      case symbol_kind::S_ACTION_SKIP_AFTER: // "SkipAfter"
      case symbol_kind::S_ACTION_STATUS: // "Status"
      case symbol_kind::S_ACTION_TAG: // "Tag"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_ENCODE: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case symbol_kind::S_ACTION_TRANSFORMATION_CMD_LINE: // "ACTION_TRANSFORMATION_CMD_LINE"
      case symbol_kind::S_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_CSS_DECODE: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_ENCODE: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_DECODE: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_JS_DECODE: // "ACTION_TRANSFORMATION_JS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_LENGTH: // "ACTION_TRANSFORMATION_LENGTH"
      case symbol_kind::S_ACTION_TRANSFORMATION_LOWERCASE: // "ACTION_TRANSFORMATION_LOWERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_PHP_ARGS_NAMES: // "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
      case symbol_kind::S_ACTION_TRANSFORMATION_MD5: // "ACTION_TRANSFORMATION_MD5"
      case symbol_kind::S_ACTION_TRANSFORMATION_NONE: // "ACTION_TRANSFORMATION_NONE"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_NULLS: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_WHITESPACE: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_COMMENTS: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_NULLS: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_SHA1: // "ACTION_TRANSFORMATION_SHA1"
      case symbol_kind::S_ACTION_TRANSFORMATION_SQL_HEX_DECODE: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM: // "ACTION_TRANSFORMATION_TRIM"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_LEFT: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_RIGHT: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case symbol_kind::S_ACTION_TRANSFORMATION_UPPERCASE: // "ACTION_TRANSFORMATION_UPPERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_ENCODE: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE: // "ACTION_TRANSFORMATION_URL_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE_UNI: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case symbol_kind::S_ACTION_TRANSFORMATION_UTF8_TO_UNICODE: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case symbol_kind::S_ACTION_VER: // "Ver"
      case symbol_kind::S_ACTION_XMLNS: // "xmlns"
      case symbol_kind::S_CONFIG_COMPONENT_SIG: // "CONFIG_COMPONENT_SIG"
      case symbol_kind::S_CONFIG_CONN_ENGINE: // "CONFIG_CONN_ENGINE"
      case symbol_kind::S_CONFIG_SEC_ARGUMENT_SEPARATOR: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case symbol_kind::S_CONFIG_SEC_WEB_APP_ID: // "CONFIG_SEC_WEB_APP_ID"
      case symbol_kind::S_CONFIG_SEC_SERVER_SIG: // "CONFIG_SEC_SERVER_SIG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR: // "CONFIG_DIR_AUDIT_DIR"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR_MOD: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_ENG: // "CONFIG_DIR_AUDIT_ENG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_FLE_MOD: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG: // "CONFIG_DIR_AUDIT_LOG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG2: // "CONFIG_DIR_AUDIT_LOG2"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG_P: // "CONFIG_DIR_AUDIT_LOG_P"
      case symbol_kind::S_CONFIG_DIR_AUDIT_STS: // "CONFIG_DIR_AUDIT_STS"
      case symbol_kind::S_CONFIG_DIR_AUDIT_TPE: // "CONFIG_DIR_AUDIT_TPE"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LOG: // "CONFIG_DIR_DEBUG_LOG"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LVL: // "CONFIG_DIR_DEBUG_LVL"
      case symbol_kind::S_CONFIG_SEC_CACHE_TRANSFORMATIONS: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case symbol_kind::S_CONFIG_SEC_DISABLE_BACKEND_COMPRESS: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case symbol_kind::S_CONFIG_SEC_HASH_ENGINE: // "CONFIG_SEC_HASH_ENGINE"
      case symbol_kind::S_CONFIG_SEC_HASH_KEY: // "CONFIG_SEC_HASH_KEY"
      case symbol_kind::S_CONFIG_SEC_HASH_PARAM: // "CONFIG_SEC_HASH_PARAM"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_RX: // "CONFIG_SEC_HASH_METHOD_RX"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_PM: // "CONFIG_SEC_HASH_METHOD_PM"
      case symbol_kind::S_CONFIG_SEC_CHROOT_DIR: // "CONFIG_SEC_CHROOT_DIR"
      case symbol_kind::S_CONFIG_DIR_GEO_DB: // "CONFIG_DIR_GEO_DB"
      case symbol_kind::S_CONFIG_DIR_GSB_DB: // "CONFIG_DIR_GSB_DB"
      case symbol_kind::S_CONFIG_SEC_GUARDIAN_LOG: // "CONFIG_SEC_GUARDIAN_LOG"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case symbol_kind::S_CONFIG_SEC_CONN_R_STATE_LIMIT: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_CONN_W_STATE_LIMIT: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_SENSOR_ID: // "CONFIG_SEC_SENSOR_ID"
      case symbol_kind::S_CONFIG_DIR_ARGS_LIMIT: // "CONFIG_DIR_ARGS_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY: // "CONFIG_DIR_REQ_BODY"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT_ACTION: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY: // "CONFIG_DIR_RES_BODY"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT: // "CONFIG_DIR_RES_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT_ACTION: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_INHERITANCE: // "CONFIG_SEC_RULE_INHERITANCE"
      case symbol_kind::S_CONFIG_SEC_RULE_PERF_TIME: // "CONFIG_SEC_RULE_PERF_TIME"
      case symbol_kind::S_CONFIG_DIR_RULE_ENG: // "CONFIG_DIR_RULE_ENG"
      case symbol_kind::S_CONFIG_DIR_SEC_ACTION: // "CONFIG_DIR_SEC_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_DEFAULT_ACTION: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_MARKER: // "CONFIG_DIR_SEC_MARKER"
      case symbol_kind::S_CONFIG_DIR_UNICODE_MAP_FILE: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case symbol_kind::S_CONFIG_DIR_UNICODE_CODE_PAGE: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case symbol_kind::S_CONFIG_SEC_COLLECTION_TIMEOUT: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case symbol_kind::S_CONFIG_SEC_HTTP_BLKEY: // "CONFIG_SEC_HTTP_BLKEY"
      case symbol_kind::S_CONFIG_SEC_INTERCEPT_ON_ERROR: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case symbol_kind::S_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_ID: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_MSG: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_TAG: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case symbol_kind::S_CONFIG_UPDLOAD_KEEP_FILES: // "CONFIG_UPDLOAD_KEEP_FILES"
      case symbol_kind::S_CONFIG_UPDLOAD_SAVE_TMP_FILES: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case symbol_kind::S_CONFIG_UPLOAD_DIR: // "CONFIG_UPLOAD_DIR"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_LIMIT: // "CONFIG_UPLOAD_FILE_LIMIT"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_MODE: // "CONFIG_UPLOAD_FILE_MODE"
      case symbol_kind::S_CONFIG_VALUE_ABORT: // "CONFIG_VALUE_ABORT"
      case symbol_kind::S_CONFIG_VALUE_DETC: // "CONFIG_VALUE_DETC"
      case symbol_kind::S_CONFIG_VALUE_HTTPS: // "CONFIG_VALUE_HTTPS"
      case symbol_kind::S_CONFIG_VALUE_OFF: // "CONFIG_VALUE_OFF"
      case symbol_kind::S_CONFIG_VALUE_ON: // "CONFIG_VALUE_ON"
      case symbol_kind::S_CONFIG_VALUE_PARALLEL: // "CONFIG_VALUE_PARALLEL"
      case symbol_kind::S_CONFIG_VALUE_PROCESS_PARTIAL: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case symbol_kind::S_CONFIG_VALUE_REJECT: // "CONFIG_VALUE_REJECT"
      case symbol_kind::S_CONFIG_VALUE_RELEVANT_ONLY: // "CONFIG_VALUE_RELEVANT_ONLY"
      case symbol_kind::S_CONFIG_VALUE_SERIAL: // "CONFIG_VALUE_SERIAL"
      case symbol_kind::S_CONFIG_VALUE_WARN: // "CONFIG_VALUE_WARN"
      case symbol_kind::S_CONFIG_XML_EXTERNAL_ENTITY: // "CONFIG_XML_EXTERNAL_ENTITY"
      case symbol_kind::S_CONGIG_DIR_RESPONSE_BODY_MP: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case symbol_kind::S_CONGIG_DIR_SEC_ARG_SEP: // "CONGIG_DIR_SEC_ARG_SEP"
      case symbol_kind::S_CONGIG_DIR_SEC_COOKIE_FORMAT: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case symbol_kind::S_CONFIG_SEC_COOKIEV0_SEPARATOR: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case symbol_kind::S_CONGIG_DIR_SEC_DATA_DIR: // "CONGIG_DIR_SEC_DATA_DIR"
      case symbol_kind::S_CONGIG_DIR_SEC_STATUS_ENGINE: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case symbol_kind::S_CONFIG_SEC_STREAM_IN_BODY_INSPECTION: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case symbol_kind::S_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case symbol_kind::S_CONGIG_DIR_SEC_TMP_DIR: // "CONGIG_DIR_SEC_TMP_DIR"
      case symbol_kind::S_DIRECTIVE: // "DIRECTIVE"
      case symbol_kind::S_DIRECTIVE_SECRULESCRIPT: // "DIRECTIVE_SECRULESCRIPT"
      case symbol_kind::S_FREE_TEXT_QUOTE_MACRO_EXPANSION: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case symbol_kind::S_QUOTATION_MARK: // "QUOTATION_MARK"
      case symbol_kind::S_RUN_TIME_VAR_BLD: // "RUN_TIME_VAR_BLD"
      case symbol_kind::S_RUN_TIME_VAR_DUR: // "RUN_TIME_VAR_DUR"
      case symbol_kind::S_RUN_TIME_VAR_HSV: // "RUN_TIME_VAR_HSV"
      case symbol_kind::S_RUN_TIME_VAR_REMOTE_USER: // "RUN_TIME_VAR_REMOTE_USER"
      case symbol_kind::S_RUN_TIME_VAR_TIME: // "RUN_TIME_VAR_TIME"
      case symbol_kind::S_RUN_TIME_VAR_TIME_DAY: // "RUN_TIME_VAR_TIME_DAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_EPOCH: // "RUN_TIME_VAR_TIME_EPOCH"
      case symbol_kind::S_RUN_TIME_VAR_TIME_HOUR: // "RUN_TIME_VAR_TIME_HOUR"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MIN: // "RUN_TIME_VAR_TIME_MIN"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MON: // "RUN_TIME_VAR_TIME_MON"
      case symbol_kind::S_RUN_TIME_VAR_TIME_SEC: // "RUN_TIME_VAR_TIME_SEC"
      case symbol_kind::S_RUN_TIME_VAR_TIME_WDAY: // "RUN_TIME_VAR_TIME_WDAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_YEAR: // "RUN_TIME_VAR_TIME_YEAR"
      case symbol_kind::S_VARIABLE: // "VARIABLE"
      case symbol_kind::S_DICT_ELEMENT: // "Dictionary element"
      case symbol_kind::S_DICT_ELEMENT_REGEXP: // "Dictionary element, selected by regexp"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_op: // op
      case symbol_kind::S_op_before_init: // op_before_init
        value.move< std::unique_ptr<Operator> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_run_time_string: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_var: // var
        value.move< std::unique_ptr<Variable> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_act: // act
      case symbol_kind::S_setvar_action: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variables: // variables
      case symbol_kind::S_variables_pre_process: // variables_pre_process
      case symbol_kind::S_variables_may_be_quoted: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_actions: // actions
      case symbol_kind::S_actions_may_quoted: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  seclang_parser::stack_symbol_type&
  seclang_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_ACTION_ACCURACY: // "Accuracy"
      case symbol_kind::S_ACTION_ALLOW: // "Allow"
      case symbol_kind::S_ACTION_APPEND: // "Append"
      case symbol_kind::S_ACTION_AUDIT_LOG: // "AuditLog"
      case symbol_kind::S_ACTION_BLOCK: // "Block"
      case symbol_kind::S_ACTION_CAPTURE: // "Capture"
      case symbol_kind::S_ACTION_CHAIN: // "Chain"
      case symbol_kind::S_ACTION_CTL_AUDIT_ENGINE: // "ACTION_CTL_AUDIT_ENGINE"
      case symbol_kind::S_ACTION_CTL_AUDIT_LOG_PARTS: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case symbol_kind::S_ACTION_CTL_BDY_JSON: // "ACTION_CTL_BDY_JSON"
      case symbol_kind::S_ACTION_CTL_BDY_XML: // "ACTION_CTL_BDY_XML"
      case symbol_kind::S_ACTION_CTL_BDY_URLENCODED: // "ACTION_CTL_BDY_URLENCODED"
      case symbol_kind::S_ACTION_CTL_FORCE_REQ_BODY_VAR: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case symbol_kind::S_ACTION_CTL_REQUEST_BODY_ACCESS: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_ID: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_TAG: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case symbol_kind::S_ACTION_DENY: // "Deny"
      case symbol_kind::S_ACTION_DEPRECATE_VAR: // "DeprecateVar"
      case symbol_kind::S_ACTION_DROP: // "Drop"
      case symbol_kind::S_ACTION_EXEC: // "Exec"
      case symbol_kind::S_ACTION_EXPIRE_VAR: // "ExpireVar"
      case symbol_kind::S_ACTION_ID: // "Id"
      case symbol_kind::S_ACTION_INITCOL: // "InitCol"
      case symbol_kind::S_ACTION_LOG: // "Log"
      case symbol_kind::S_ACTION_LOG_DATA: // "LogData"
      case symbol_kind::S_ACTION_MATURITY: // "Maturity"
      case symbol_kind::S_ACTION_MSG: // "Msg"
      case symbol_kind::S_ACTION_MULTI_MATCH: // "MultiMatch"
      case symbol_kind::S_ACTION_NO_AUDIT_LOG: // "NoAuditLog"
      case symbol_kind::S_ACTION_NO_LOG: // "NoLog"
      case symbol_kind::S_ACTION_PASS: // "Pass"
      case symbol_kind::S_ACTION_PAUSE: // "Pause"
      case symbol_kind::S_ACTION_PHASE: // "Phase"
      case symbol_kind::S_ACTION_PREPEND: // "Prepend"
      case symbol_kind::S_ACTION_PROXY: // "Proxy"
      case symbol_kind::S_ACTION_REDIRECT: // "Redirect"
      case symbol_kind::S_ACTION_REV: // "Rev"
      case symbol_kind::S_ACTION_SANITISE_ARG: // "SanitiseArg"
      case symbol_kind::S_ACTION_SANITISE_MATCHED: // "SanitiseMatched"
      case symbol_kind::S_ACTION_SANITISE_MATCHED_BYTES: // "SanitiseMatchedBytes"
      case symbol_kind::S_ACTION_SANITISE_REQUEST_HEADER: // "SanitiseRequestHeader"
      case symbol_kind::S_ACTION_SANITISE_RESPONSE_HEADER: // "SanitiseResponseHeader"
      case symbol_kind::S_ACTION_SETENV: // "SetEnv"
      case symbol_kind::S_ACTION_SETRSC: // "SetRsc"
      case symbol_kind::S_ACTION_SETSID: // "SetSid"
      case symbol_kind::S_ACTION_SETUID: // "SetUID"
      case symbol_kind::S_ACTION_SEVERITY: // "Severity"
      case symbol_kind::S_ACTION_SKIP: // "Skip"
      case symbol_kind::S_ACTION_SKIP_AFTER: // "SkipAfter"
      case symbol_kind::S_ACTION_STATUS: // "Status"
      case symbol_kind::S_ACTION_TAG: // "Tag"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_ENCODE: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case symbol_kind::S_ACTION_TRANSFORMATION_CMD_LINE: // "ACTION_TRANSFORMATION_CMD_LINE"
      case symbol_kind::S_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_CSS_DECODE: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_ENCODE: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_DECODE: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_JS_DECODE: // "ACTION_TRANSFORMATION_JS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_LENGTH: // "ACTION_TRANSFORMATION_LENGTH"
      case symbol_kind::S_ACTION_TRANSFORMATION_LOWERCASE: // "ACTION_TRANSFORMATION_LOWERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_PHP_ARGS_NAMES: // "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
      case symbol_kind::S_ACTION_TRANSFORMATION_MD5: // "ACTION_TRANSFORMATION_MD5"
      case symbol_kind::S_ACTION_TRANSFORMATION_NONE: // "ACTION_TRANSFORMATION_NONE"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_NULLS: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_WHITESPACE: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_COMMENTS: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_NULLS: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_SHA1: // "ACTION_TRANSFORMATION_SHA1"
      case symbol_kind::S_ACTION_TRANSFORMATION_SQL_HEX_DECODE: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM: // "ACTION_TRANSFORMATION_TRIM"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_LEFT: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_RIGHT: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case symbol_kind::S_ACTION_TRANSFORMATION_UPPERCASE: // "ACTION_TRANSFORMATION_UPPERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_ENCODE: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE: // "ACTION_TRANSFORMATION_URL_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE_UNI: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case symbol_kind::S_ACTION_TRANSFORMATION_UTF8_TO_UNICODE: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case symbol_kind::S_ACTION_VER: // "Ver"
      case symbol_kind::S_ACTION_XMLNS: // "xmlns"
      case symbol_kind::S_CONFIG_COMPONENT_SIG: // "CONFIG_COMPONENT_SIG"
      case symbol_kind::S_CONFIG_CONN_ENGINE: // "CONFIG_CONN_ENGINE"
      case symbol_kind::S_CONFIG_SEC_ARGUMENT_SEPARATOR: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case symbol_kind::S_CONFIG_SEC_WEB_APP_ID: // "CONFIG_SEC_WEB_APP_ID"
      case symbol_kind::S_CONFIG_SEC_SERVER_SIG: // "CONFIG_SEC_SERVER_SIG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR: // "CONFIG_DIR_AUDIT_DIR"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR_MOD: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_ENG: // "CONFIG_DIR_AUDIT_ENG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_FLE_MOD: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG: // "CONFIG_DIR_AUDIT_LOG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG2: // "CONFIG_DIR_AUDIT_LOG2"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG_P: // "CONFIG_DIR_AUDIT_LOG_P"
      case symbol_kind::S_CONFIG_DIR_AUDIT_STS: // "CONFIG_DIR_AUDIT_STS"
      case symbol_kind::S_CONFIG_DIR_AUDIT_TPE: // "CONFIG_DIR_AUDIT_TPE"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LOG: // "CONFIG_DIR_DEBUG_LOG"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LVL: // "CONFIG_DIR_DEBUG_LVL"
      case symbol_kind::S_CONFIG_SEC_CACHE_TRANSFORMATIONS: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case symbol_kind::S_CONFIG_SEC_DISABLE_BACKEND_COMPRESS: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case symbol_kind::S_CONFIG_SEC_HASH_ENGINE: // "CONFIG_SEC_HASH_ENGINE"
      case symbol_kind::S_CONFIG_SEC_HASH_KEY: // "CONFIG_SEC_HASH_KEY"
      case symbol_kind::S_CONFIG_SEC_HASH_PARAM: // "CONFIG_SEC_HASH_PARAM"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_RX: // "CONFIG_SEC_HASH_METHOD_RX"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_PM: // "CONFIG_SEC_HASH_METHOD_PM"
      case symbol_kind::S_CONFIG_SEC_CHROOT_DIR: // "CONFIG_SEC_CHROOT_DIR"
      case symbol_kind::S_CONFIG_DIR_GEO_DB: // "CONFIG_DIR_GEO_DB"
      case symbol_kind::S_CONFIG_DIR_GSB_DB: // "CONFIG_DIR_GSB_DB"
      case symbol_kind::S_CONFIG_SEC_GUARDIAN_LOG: // "CONFIG_SEC_GUARDIAN_LOG"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case symbol_kind::S_CONFIG_SEC_CONN_R_STATE_LIMIT: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_CONN_W_STATE_LIMIT: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_SENSOR_ID: // "CONFIG_SEC_SENSOR_ID"
      case symbol_kind::S_CONFIG_DIR_ARGS_LIMIT: // "CONFIG_DIR_ARGS_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY: // "CONFIG_DIR_REQ_BODY"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT_ACTION: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY: // "CONFIG_DIR_RES_BODY"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT: // "CONFIG_DIR_RES_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT_ACTION: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_INHERITANCE: // "CONFIG_SEC_RULE_INHERITANCE"
      case symbol_kind::S_CONFIG_SEC_RULE_PERF_TIME: // "CONFIG_SEC_RULE_PERF_TIME"
      case symbol_kind::S_CONFIG_DIR_RULE_ENG: // "CONFIG_DIR_RULE_ENG"
      case symbol_kind::S_CONFIG_DIR_SEC_ACTION: // "CONFIG_DIR_SEC_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_DEFAULT_ACTION: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_MARKER: // "CONFIG_DIR_SEC_MARKER"
      case symbol_kind::S_CONFIG_DIR_UNICODE_MAP_FILE: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case symbol_kind::S_CONFIG_DIR_UNICODE_CODE_PAGE: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case symbol_kind::S_CONFIG_SEC_COLLECTION_TIMEOUT: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case symbol_kind::S_CONFIG_SEC_HTTP_BLKEY: // "CONFIG_SEC_HTTP_BLKEY"
      case symbol_kind::S_CONFIG_SEC_INTERCEPT_ON_ERROR: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case symbol_kind::S_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_ID: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_MSG: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_TAG: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case symbol_kind::S_CONFIG_UPDLOAD_KEEP_FILES: // "CONFIG_UPDLOAD_KEEP_FILES"
      case symbol_kind::S_CONFIG_UPDLOAD_SAVE_TMP_FILES: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case symbol_kind::S_CONFIG_UPLOAD_DIR: // "CONFIG_UPLOAD_DIR"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_LIMIT: // "CONFIG_UPLOAD_FILE_LIMIT"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_MODE: // "CONFIG_UPLOAD_FILE_MODE"
      case symbol_kind::S_CONFIG_VALUE_ABORT: // "CONFIG_VALUE_ABORT"
      case symbol_kind::S_CONFIG_VALUE_DETC: // "CONFIG_VALUE_DETC"
      case symbol_kind::S_CONFIG_VALUE_HTTPS: // "CONFIG_VALUE_HTTPS"
      case symbol_kind::S_CONFIG_VALUE_OFF: // "CONFIG_VALUE_OFF"
      case symbol_kind::S_CONFIG_VALUE_ON: // "CONFIG_VALUE_ON"
      case symbol_kind::S_CONFIG_VALUE_PARALLEL: // "CONFIG_VALUE_PARALLEL"
      case symbol_kind::S_CONFIG_VALUE_PROCESS_PARTIAL: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case symbol_kind::S_CONFIG_VALUE_REJECT: // "CONFIG_VALUE_REJECT"
      case symbol_kind::S_CONFIG_VALUE_RELEVANT_ONLY: // "CONFIG_VALUE_RELEVANT_ONLY"
      case symbol_kind::S_CONFIG_VALUE_SERIAL: // "CONFIG_VALUE_SERIAL"
      case symbol_kind::S_CONFIG_VALUE_WARN: // "CONFIG_VALUE_WARN"
      case symbol_kind::S_CONFIG_XML_EXTERNAL_ENTITY: // "CONFIG_XML_EXTERNAL_ENTITY"
      case symbol_kind::S_CONGIG_DIR_RESPONSE_BODY_MP: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case symbol_kind::S_CONGIG_DIR_SEC_ARG_SEP: // "CONGIG_DIR_SEC_ARG_SEP"
      case symbol_kind::S_CONGIG_DIR_SEC_COOKIE_FORMAT: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case symbol_kind::S_CONFIG_SEC_COOKIEV0_SEPARATOR: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case symbol_kind::S_CONGIG_DIR_SEC_DATA_DIR: // "CONGIG_DIR_SEC_DATA_DIR"
      case symbol_kind::S_CONGIG_DIR_SEC_STATUS_ENGINE: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case symbol_kind::S_CONFIG_SEC_STREAM_IN_BODY_INSPECTION: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case symbol_kind::S_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case symbol_kind::S_CONGIG_DIR_SEC_TMP_DIR: // "CONGIG_DIR_SEC_TMP_DIR"
      case symbol_kind::S_DIRECTIVE: // "DIRECTIVE"
      case symbol_kind::S_DIRECTIVE_SECRULESCRIPT: // "DIRECTIVE_SECRULESCRIPT"
      case symbol_kind::S_FREE_TEXT_QUOTE_MACRO_EXPANSION: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case symbol_kind::S_QUOTATION_MARK: // "QUOTATION_MARK"
      case symbol_kind::S_RUN_TIME_VAR_BLD: // "RUN_TIME_VAR_BLD"
      case symbol_kind::S_RUN_TIME_VAR_DUR: // "RUN_TIME_VAR_DUR"
      case symbol_kind::S_RUN_TIME_VAR_HSV: // "RUN_TIME_VAR_HSV"
      case symbol_kind::S_RUN_TIME_VAR_REMOTE_USER: // "RUN_TIME_VAR_REMOTE_USER"
      case symbol_kind::S_RUN_TIME_VAR_TIME: // "RUN_TIME_VAR_TIME"
      case symbol_kind::S_RUN_TIME_VAR_TIME_DAY: // "RUN_TIME_VAR_TIME_DAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_EPOCH: // "RUN_TIME_VAR_TIME_EPOCH"
      case symbol_kind::S_RUN_TIME_VAR_TIME_HOUR: // "RUN_TIME_VAR_TIME_HOUR"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MIN: // "RUN_TIME_VAR_TIME_MIN"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MON: // "RUN_TIME_VAR_TIME_MON"
      case symbol_kind::S_RUN_TIME_VAR_TIME_SEC: // "RUN_TIME_VAR_TIME_SEC"
      case symbol_kind::S_RUN_TIME_VAR_TIME_WDAY: // "RUN_TIME_VAR_TIME_WDAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_YEAR: // "RUN_TIME_VAR_TIME_YEAR"
      case symbol_kind::S_VARIABLE: // "VARIABLE"
      case symbol_kind::S_DICT_ELEMENT: // "Dictionary element"
      case symbol_kind::S_DICT_ELEMENT_REGEXP: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_op: // op
      case symbol_kind::S_op_before_init: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case symbol_kind::S_run_time_string: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case symbol_kind::S_var: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case symbol_kind::S_act: // act
      case symbol_kind::S_setvar_action: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case symbol_kind::S_variables: // variables
      case symbol_kind::S_variables_pre_process: // variables_pre_process
      case symbol_kind::S_variables_may_be_quoted: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case symbol_kind::S_actions: // actions
      case symbol_kind::S_actions_may_quoted: // actions_may_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  seclang_parser::stack_symbol_type&
  seclang_parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_ACTION_ACCURACY: // "Accuracy"
      case symbol_kind::S_ACTION_ALLOW: // "Allow"
      case symbol_kind::S_ACTION_APPEND: // "Append"
      case symbol_kind::S_ACTION_AUDIT_LOG: // "AuditLog"
      case symbol_kind::S_ACTION_BLOCK: // "Block"
      case symbol_kind::S_ACTION_CAPTURE: // "Capture"
      case symbol_kind::S_ACTION_CHAIN: // "Chain"
      case symbol_kind::S_ACTION_CTL_AUDIT_ENGINE: // "ACTION_CTL_AUDIT_ENGINE"
      case symbol_kind::S_ACTION_CTL_AUDIT_LOG_PARTS: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case symbol_kind::S_ACTION_CTL_BDY_JSON: // "ACTION_CTL_BDY_JSON"
      case symbol_kind::S_ACTION_CTL_BDY_XML: // "ACTION_CTL_BDY_XML"
      case symbol_kind::S_ACTION_CTL_BDY_URLENCODED: // "ACTION_CTL_BDY_URLENCODED"
      case symbol_kind::S_ACTION_CTL_FORCE_REQ_BODY_VAR: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case symbol_kind::S_ACTION_CTL_REQUEST_BODY_ACCESS: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_ID: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_TAG: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case symbol_kind::S_ACTION_DENY: // "Deny"
      case symbol_kind::S_ACTION_DEPRECATE_VAR: // "DeprecateVar"
      case symbol_kind::S_ACTION_DROP: // "Drop"
      case symbol_kind::S_ACTION_EXEC: // "Exec"
      case symbol_kind::S_ACTION_EXPIRE_VAR: // "ExpireVar"
      case symbol_kind::S_ACTION_ID: // "Id"
      case symbol_kind::S_ACTION_INITCOL: // "InitCol"
      case symbol_kind::S_ACTION_LOG: // "Log"
      case symbol_kind::S_ACTION_LOG_DATA: // "LogData"
      case symbol_kind::S_ACTION_MATURITY: // "Maturity"
      case symbol_kind::S_ACTION_MSG: // "Msg"
      case symbol_kind::S_ACTION_MULTI_MATCH: // "MultiMatch"
      case symbol_kind::S_ACTION_NO_AUDIT_LOG: // "NoAuditLog"
      case symbol_kind::S_ACTION_NO_LOG: // "NoLog"
      case symbol_kind::S_ACTION_PASS: // "Pass"
      case symbol_kind::S_ACTION_PAUSE: // "Pause"
      case symbol_kind::S_ACTION_PHASE: // "Phase"
      case symbol_kind::S_ACTION_PREPEND: // "Prepend"
      case symbol_kind::S_ACTION_PROXY: // "Proxy"
      case symbol_kind::S_ACTION_REDIRECT: // "Redirect"
      case symbol_kind::S_ACTION_REV: // "Rev"
      case symbol_kind::S_ACTION_SANITISE_ARG: // "SanitiseArg"
      case symbol_kind::S_ACTION_SANITISE_MATCHED: // "SanitiseMatched"
      case symbol_kind::S_ACTION_SANITISE_MATCHED_BYTES: // "SanitiseMatchedBytes"
      case symbol_kind::S_ACTION_SANITISE_REQUEST_HEADER: // "SanitiseRequestHeader"
      case symbol_kind::S_ACTION_SANITISE_RESPONSE_HEADER: // "SanitiseResponseHeader"
      case symbol_kind::S_ACTION_SETENV: // "SetEnv"
      case symbol_kind::S_ACTION_SETRSC: // "SetRsc"
      case symbol_kind::S_ACTION_SETSID: // "SetSid"
      case symbol_kind::S_ACTION_SETUID: // "SetUID"
      case symbol_kind::S_ACTION_SEVERITY: // "Severity"
      case symbol_kind::S_ACTION_SKIP: // "Skip"
      case symbol_kind::S_ACTION_SKIP_AFTER: // "SkipAfter"
      case symbol_kind::S_ACTION_STATUS: // "Status"
      case symbol_kind::S_ACTION_TAG: // "Tag"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_ENCODE: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case symbol_kind::S_ACTION_TRANSFORMATION_CMD_LINE: // "ACTION_TRANSFORMATION_CMD_LINE"
      case symbol_kind::S_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_CSS_DECODE: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_ENCODE: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_DECODE: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_JS_DECODE: // "ACTION_TRANSFORMATION_JS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_LENGTH: // "ACTION_TRANSFORMATION_LENGTH"
      case symbol_kind::S_ACTION_TRANSFORMATION_LOWERCASE: // "ACTION_TRANSFORMATION_LOWERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_PHP_ARGS_NAMES: // "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
      case symbol_kind::S_ACTION_TRANSFORMATION_MD5: // "ACTION_TRANSFORMATION_MD5"
      case symbol_kind::S_ACTION_TRANSFORMATION_NONE: // "ACTION_TRANSFORMATION_NONE"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_NULLS: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_WHITESPACE: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_COMMENTS: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_NULLS: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_SHA1: // "ACTION_TRANSFORMATION_SHA1"
      case symbol_kind::S_ACTION_TRANSFORMATION_SQL_HEX_DECODE: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM: // "ACTION_TRANSFORMATION_TRIM"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_LEFT: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_RIGHT: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case symbol_kind::S_ACTION_TRANSFORMATION_UPPERCASE: // "ACTION_TRANSFORMATION_UPPERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_ENCODE: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE: // "ACTION_TRANSFORMATION_URL_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE_UNI: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case symbol_kind::S_ACTION_TRANSFORMATION_UTF8_TO_UNICODE: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case symbol_kind::S_ACTION_VER: // "Ver"
      case symbol_kind::S_ACTION_XMLNS: // "xmlns"
      case symbol_kind::S_CONFIG_COMPONENT_SIG: // "CONFIG_COMPONENT_SIG"
      case symbol_kind::S_CONFIG_CONN_ENGINE: // "CONFIG_CONN_ENGINE"
      case symbol_kind::S_CONFIG_SEC_ARGUMENT_SEPARATOR: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case symbol_kind::S_CONFIG_SEC_WEB_APP_ID: // "CONFIG_SEC_WEB_APP_ID"
      case symbol_kind::S_CONFIG_SEC_SERVER_SIG: // "CONFIG_SEC_SERVER_SIG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR: // "CONFIG_DIR_AUDIT_DIR"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR_MOD: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_ENG: // "CONFIG_DIR_AUDIT_ENG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_FLE_MOD: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG: // "CONFIG_DIR_AUDIT_LOG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG2: // "CONFIG_DIR_AUDIT_LOG2"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG_P: // "CONFIG_DIR_AUDIT_LOG_P"
      case symbol_kind::S_CONFIG_DIR_AUDIT_STS: // "CONFIG_DIR_AUDIT_STS"
      case symbol_kind::S_CONFIG_DIR_AUDIT_TPE: // "CONFIG_DIR_AUDIT_TPE"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LOG: // "CONFIG_DIR_DEBUG_LOG"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LVL: // "CONFIG_DIR_DEBUG_LVL"
      case symbol_kind::S_CONFIG_SEC_CACHE_TRANSFORMATIONS: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case symbol_kind::S_CONFIG_SEC_DISABLE_BACKEND_COMPRESS: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case symbol_kind::S_CONFIG_SEC_HASH_ENGINE: // "CONFIG_SEC_HASH_ENGINE"
      case symbol_kind::S_CONFIG_SEC_HASH_KEY: // "CONFIG_SEC_HASH_KEY"
      case symbol_kind::S_CONFIG_SEC_HASH_PARAM: // "CONFIG_SEC_HASH_PARAM"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_RX: // "CONFIG_SEC_HASH_METHOD_RX"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_PM: // "CONFIG_SEC_HASH_METHOD_PM"
      case symbol_kind::S_CONFIG_SEC_CHROOT_DIR: // "CONFIG_SEC_CHROOT_DIR"
      case symbol_kind::S_CONFIG_DIR_GEO_DB: // "CONFIG_DIR_GEO_DB"
      case symbol_kind::S_CONFIG_DIR_GSB_DB: // "CONFIG_DIR_GSB_DB"
      case symbol_kind::S_CONFIG_SEC_GUARDIAN_LOG: // "CONFIG_SEC_GUARDIAN_LOG"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case symbol_kind::S_CONFIG_SEC_CONN_R_STATE_LIMIT: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_CONN_W_STATE_LIMIT: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_SENSOR_ID: // "CONFIG_SEC_SENSOR_ID"
      case symbol_kind::S_CONFIG_DIR_ARGS_LIMIT: // "CONFIG_DIR_ARGS_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY: // "CONFIG_DIR_REQ_BODY"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT_ACTION: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY: // "CONFIG_DIR_RES_BODY"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT: // "CONFIG_DIR_RES_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT_ACTION: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_INHERITANCE: // "CONFIG_SEC_RULE_INHERITANCE"
      case symbol_kind::S_CONFIG_SEC_RULE_PERF_TIME: // "CONFIG_SEC_RULE_PERF_TIME"
      case symbol_kind::S_CONFIG_DIR_RULE_ENG: // "CONFIG_DIR_RULE_ENG"
      case symbol_kind::S_CONFIG_DIR_SEC_ACTION: // "CONFIG_DIR_SEC_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_DEFAULT_ACTION: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_MARKER: // "CONFIG_DIR_SEC_MARKER"
      case symbol_kind::S_CONFIG_DIR_UNICODE_MAP_FILE: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case symbol_kind::S_CONFIG_DIR_UNICODE_CODE_PAGE: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case symbol_kind::S_CONFIG_SEC_COLLECTION_TIMEOUT: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case symbol_kind::S_CONFIG_SEC_HTTP_BLKEY: // "CONFIG_SEC_HTTP_BLKEY"
      case symbol_kind::S_CONFIG_SEC_INTERCEPT_ON_ERROR: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case symbol_kind::S_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_ID: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_MSG: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_TAG: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case symbol_kind::S_CONFIG_UPDLOAD_KEEP_FILES: // "CONFIG_UPDLOAD_KEEP_FILES"
      case symbol_kind::S_CONFIG_UPDLOAD_SAVE_TMP_FILES: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case symbol_kind::S_CONFIG_UPLOAD_DIR: // "CONFIG_UPLOAD_DIR"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_LIMIT: // "CONFIG_UPLOAD_FILE_LIMIT"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_MODE: // "CONFIG_UPLOAD_FILE_MODE"
      case symbol_kind::S_CONFIG_VALUE_ABORT: // "CONFIG_VALUE_ABORT"
      case symbol_kind::S_CONFIG_VALUE_DETC: // "CONFIG_VALUE_DETC"
      case symbol_kind::S_CONFIG_VALUE_HTTPS: // "CONFIG_VALUE_HTTPS"
      case symbol_kind::S_CONFIG_VALUE_OFF: // "CONFIG_VALUE_OFF"
      case symbol_kind::S_CONFIG_VALUE_ON: // "CONFIG_VALUE_ON"
      case symbol_kind::S_CONFIG_VALUE_PARALLEL: // "CONFIG_VALUE_PARALLEL"
      case symbol_kind::S_CONFIG_VALUE_PROCESS_PARTIAL: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case symbol_kind::S_CONFIG_VALUE_REJECT: // "CONFIG_VALUE_REJECT"
      case symbol_kind::S_CONFIG_VALUE_RELEVANT_ONLY: // "CONFIG_VALUE_RELEVANT_ONLY"
      case symbol_kind::S_CONFIG_VALUE_SERIAL: // "CONFIG_VALUE_SERIAL"
      case symbol_kind::S_CONFIG_VALUE_WARN: // "CONFIG_VALUE_WARN"
      case symbol_kind::S_CONFIG_XML_EXTERNAL_ENTITY: // "CONFIG_XML_EXTERNAL_ENTITY"
      case symbol_kind::S_CONGIG_DIR_RESPONSE_BODY_MP: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case symbol_kind::S_CONGIG_DIR_SEC_ARG_SEP: // "CONGIG_DIR_SEC_ARG_SEP"
      case symbol_kind::S_CONGIG_DIR_SEC_COOKIE_FORMAT: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case symbol_kind::S_CONFIG_SEC_COOKIEV0_SEPARATOR: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case symbol_kind::S_CONGIG_DIR_SEC_DATA_DIR: // "CONGIG_DIR_SEC_DATA_DIR"
      case symbol_kind::S_CONGIG_DIR_SEC_STATUS_ENGINE: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case symbol_kind::S_CONFIG_SEC_STREAM_IN_BODY_INSPECTION: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case symbol_kind::S_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case symbol_kind::S_CONGIG_DIR_SEC_TMP_DIR: // "CONGIG_DIR_SEC_TMP_DIR"
      case symbol_kind::S_DIRECTIVE: // "DIRECTIVE"
      case symbol_kind::S_DIRECTIVE_SECRULESCRIPT: // "DIRECTIVE_SECRULESCRIPT"
      case symbol_kind::S_FREE_TEXT_QUOTE_MACRO_EXPANSION: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case symbol_kind::S_QUOTATION_MARK: // "QUOTATION_MARK"
      case symbol_kind::S_RUN_TIME_VAR_BLD: // "RUN_TIME_VAR_BLD"
      case symbol_kind::S_RUN_TIME_VAR_DUR: // "RUN_TIME_VAR_DUR"
      case symbol_kind::S_RUN_TIME_VAR_HSV: // "RUN_TIME_VAR_HSV"
      case symbol_kind::S_RUN_TIME_VAR_REMOTE_USER: // "RUN_TIME_VAR_REMOTE_USER"
      case symbol_kind::S_RUN_TIME_VAR_TIME: // "RUN_TIME_VAR_TIME"
      case symbol_kind::S_RUN_TIME_VAR_TIME_DAY: // "RUN_TIME_VAR_TIME_DAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_EPOCH: // "RUN_TIME_VAR_TIME_EPOCH"
      case symbol_kind::S_RUN_TIME_VAR_TIME_HOUR: // "RUN_TIME_VAR_TIME_HOUR"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MIN: // "RUN_TIME_VAR_TIME_MIN"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MON: // "RUN_TIME_VAR_TIME_MON"
      case symbol_kind::S_RUN_TIME_VAR_TIME_SEC: // "RUN_TIME_VAR_TIME_SEC"
      case symbol_kind::S_RUN_TIME_VAR_TIME_WDAY: // "RUN_TIME_VAR_TIME_WDAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_YEAR: // "RUN_TIME_VAR_TIME_YEAR"
      case symbol_kind::S_VARIABLE: // "VARIABLE"
      case symbol_kind::S_DICT_ELEMENT: // "Dictionary element"
      case symbol_kind::S_DICT_ELEMENT_REGEXP: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_op: // op
      case symbol_kind::S_op_before_init: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case symbol_kind::S_run_time_string: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case symbol_kind::S_var: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case symbol_kind::S_act: // act
      case symbol_kind::S_setvar_action: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case symbol_kind::S_variables: // variables
      case symbol_kind::S_variables_pre_process: // variables_pre_process
      case symbol_kind::S_variables_may_be_quoted: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case symbol_kind::S_actions: // actions
      case symbol_kind::S_actions_may_quoted: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  seclang_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  seclang_parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YYUSE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  seclang_parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  seclang_parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  seclang_parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  seclang_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  seclang_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  seclang_parser::debug_level_type
  seclang_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  seclang_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  seclang_parser::state_type
  seclang_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  seclang_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  seclang_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  seclang_parser::operator() ()
  {
    return parse ();
  }

  int
  seclang_parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 319 "seclang-parser.yy"
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = new std::string(driver.file);
}

#line 1328 "seclang-parser.cc"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_ACTION_ACCURACY: // "Accuracy"
      case symbol_kind::S_ACTION_ALLOW: // "Allow"
      case symbol_kind::S_ACTION_APPEND: // "Append"
      case symbol_kind::S_ACTION_AUDIT_LOG: // "AuditLog"
      case symbol_kind::S_ACTION_BLOCK: // "Block"
      case symbol_kind::S_ACTION_CAPTURE: // "Capture"
      case symbol_kind::S_ACTION_CHAIN: // "Chain"
      case symbol_kind::S_ACTION_CTL_AUDIT_ENGINE: // "ACTION_CTL_AUDIT_ENGINE"
      case symbol_kind::S_ACTION_CTL_AUDIT_LOG_PARTS: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case symbol_kind::S_ACTION_CTL_BDY_JSON: // "ACTION_CTL_BDY_JSON"
      case symbol_kind::S_ACTION_CTL_BDY_XML: // "ACTION_CTL_BDY_XML"
      case symbol_kind::S_ACTION_CTL_BDY_URLENCODED: // "ACTION_CTL_BDY_URLENCODED"
      case symbol_kind::S_ACTION_CTL_FORCE_REQ_BODY_VAR: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case symbol_kind::S_ACTION_CTL_REQUEST_BODY_ACCESS: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_ID: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_BY_TAG: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case symbol_kind::S_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case symbol_kind::S_ACTION_DENY: // "Deny"
      case symbol_kind::S_ACTION_DEPRECATE_VAR: // "DeprecateVar"
      case symbol_kind::S_ACTION_DROP: // "Drop"
      case symbol_kind::S_ACTION_EXEC: // "Exec"
      case symbol_kind::S_ACTION_EXPIRE_VAR: // "ExpireVar"
      case symbol_kind::S_ACTION_ID: // "Id"
      case symbol_kind::S_ACTION_INITCOL: // "InitCol"
      case symbol_kind::S_ACTION_LOG: // "Log"
      case symbol_kind::S_ACTION_LOG_DATA: // "LogData"
      case symbol_kind::S_ACTION_MATURITY: // "Maturity"
      case symbol_kind::S_ACTION_MSG: // "Msg"
      case symbol_kind::S_ACTION_MULTI_MATCH: // "MultiMatch"
      case symbol_kind::S_ACTION_NO_AUDIT_LOG: // "NoAuditLog"
      case symbol_kind::S_ACTION_NO_LOG: // "NoLog"
      case symbol_kind::S_ACTION_PASS: // "Pass"
      case symbol_kind::S_ACTION_PAUSE: // "Pause"
      case symbol_kind::S_ACTION_PHASE: // "Phase"
      case symbol_kind::S_ACTION_PREPEND: // "Prepend"
      case symbol_kind::S_ACTION_PROXY: // "Proxy"
      case symbol_kind::S_ACTION_REDIRECT: // "Redirect"
      case symbol_kind::S_ACTION_REV: // "Rev"
      case symbol_kind::S_ACTION_SANITISE_ARG: // "SanitiseArg"
      case symbol_kind::S_ACTION_SANITISE_MATCHED: // "SanitiseMatched"
      case symbol_kind::S_ACTION_SANITISE_MATCHED_BYTES: // "SanitiseMatchedBytes"
      case symbol_kind::S_ACTION_SANITISE_REQUEST_HEADER: // "SanitiseRequestHeader"
      case symbol_kind::S_ACTION_SANITISE_RESPONSE_HEADER: // "SanitiseResponseHeader"
      case symbol_kind::S_ACTION_SETENV: // "SetEnv"
      case symbol_kind::S_ACTION_SETRSC: // "SetRsc"
      case symbol_kind::S_ACTION_SETSID: // "SetSid"
      case symbol_kind::S_ACTION_SETUID: // "SetUID"
      case symbol_kind::S_ACTION_SEVERITY: // "Severity"
      case symbol_kind::S_ACTION_SKIP: // "Skip"
      case symbol_kind::S_ACTION_SKIP_AFTER: // "SkipAfter"
      case symbol_kind::S_ACTION_STATUS: // "Status"
      case symbol_kind::S_ACTION_TAG: // "Tag"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_ENCODE: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case symbol_kind::S_ACTION_TRANSFORMATION_CMD_LINE: // "ACTION_TRANSFORMATION_CMD_LINE"
      case symbol_kind::S_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_CSS_DECODE: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_ENCODE: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HEX_DECODE: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_JS_DECODE: // "ACTION_TRANSFORMATION_JS_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_LENGTH: // "ACTION_TRANSFORMATION_LENGTH"
      case symbol_kind::S_ACTION_TRANSFORMATION_LOWERCASE: // "ACTION_TRANSFORMATION_LOWERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_PHP_ARGS_NAMES: // "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
      case symbol_kind::S_ACTION_TRANSFORMATION_MD5: // "ACTION_TRANSFORMATION_MD5"
      case symbol_kind::S_ACTION_TRANSFORMATION_NONE: // "ACTION_TRANSFORMATION_NONE"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case symbol_kind::S_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_NULLS: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REMOVE_WHITESPACE: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_COMMENTS: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case symbol_kind::S_ACTION_TRANSFORMATION_REPLACE_NULLS: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case symbol_kind::S_ACTION_TRANSFORMATION_SHA1: // "ACTION_TRANSFORMATION_SHA1"
      case symbol_kind::S_ACTION_TRANSFORMATION_SQL_HEX_DECODE: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM: // "ACTION_TRANSFORMATION_TRIM"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_LEFT: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case symbol_kind::S_ACTION_TRANSFORMATION_TRIM_RIGHT: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case symbol_kind::S_ACTION_TRANSFORMATION_UPPERCASE: // "ACTION_TRANSFORMATION_UPPERCASE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_ENCODE: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE: // "ACTION_TRANSFORMATION_URL_DECODE"
      case symbol_kind::S_ACTION_TRANSFORMATION_URL_DECODE_UNI: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case symbol_kind::S_ACTION_TRANSFORMATION_UTF8_TO_UNICODE: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case symbol_kind::S_ACTION_VER: // "Ver"
      case symbol_kind::S_ACTION_XMLNS: // "xmlns"
      case symbol_kind::S_CONFIG_COMPONENT_SIG: // "CONFIG_COMPONENT_SIG"
      case symbol_kind::S_CONFIG_CONN_ENGINE: // "CONFIG_CONN_ENGINE"
      case symbol_kind::S_CONFIG_SEC_ARGUMENT_SEPARATOR: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case symbol_kind::S_CONFIG_SEC_WEB_APP_ID: // "CONFIG_SEC_WEB_APP_ID"
      case symbol_kind::S_CONFIG_SEC_SERVER_SIG: // "CONFIG_SEC_SERVER_SIG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR: // "CONFIG_DIR_AUDIT_DIR"
      case symbol_kind::S_CONFIG_DIR_AUDIT_DIR_MOD: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_ENG: // "CONFIG_DIR_AUDIT_ENG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_FLE_MOD: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG: // "CONFIG_DIR_AUDIT_LOG"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG2: // "CONFIG_DIR_AUDIT_LOG2"
      case symbol_kind::S_CONFIG_DIR_AUDIT_LOG_P: // "CONFIG_DIR_AUDIT_LOG_P"
      case symbol_kind::S_CONFIG_DIR_AUDIT_STS: // "CONFIG_DIR_AUDIT_STS"
      case symbol_kind::S_CONFIG_DIR_AUDIT_TPE: // "CONFIG_DIR_AUDIT_TPE"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LOG: // "CONFIG_DIR_DEBUG_LOG"
      case symbol_kind::S_CONFIG_DIR_DEBUG_LVL: // "CONFIG_DIR_DEBUG_LVL"
      case symbol_kind::S_CONFIG_SEC_CACHE_TRANSFORMATIONS: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case symbol_kind::S_CONFIG_SEC_DISABLE_BACKEND_COMPRESS: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case symbol_kind::S_CONFIG_SEC_HASH_ENGINE: // "CONFIG_SEC_HASH_ENGINE"
      case symbol_kind::S_CONFIG_SEC_HASH_KEY: // "CONFIG_SEC_HASH_KEY"
      case symbol_kind::S_CONFIG_SEC_HASH_PARAM: // "CONFIG_SEC_HASH_PARAM"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_RX: // "CONFIG_SEC_HASH_METHOD_RX"
      case symbol_kind::S_CONFIG_SEC_HASH_METHOD_PM: // "CONFIG_SEC_HASH_METHOD_PM"
      case symbol_kind::S_CONFIG_SEC_CHROOT_DIR: // "CONFIG_SEC_CHROOT_DIR"
      case symbol_kind::S_CONFIG_DIR_GEO_DB: // "CONFIG_DIR_GEO_DB"
      case symbol_kind::S_CONFIG_DIR_GSB_DB: // "CONFIG_DIR_GSB_DB"
      case symbol_kind::S_CONFIG_SEC_GUARDIAN_LOG: // "CONFIG_SEC_GUARDIAN_LOG"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case symbol_kind::S_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case symbol_kind::S_CONFIG_SEC_CONN_R_STATE_LIMIT: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_CONN_W_STATE_LIMIT: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case symbol_kind::S_CONFIG_SEC_SENSOR_ID: // "CONFIG_SEC_SENSOR_ID"
      case symbol_kind::S_CONFIG_DIR_ARGS_LIMIT: // "CONFIG_DIR_ARGS_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY: // "CONFIG_DIR_REQ_BODY"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_LIMIT_ACTION: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY: // "CONFIG_DIR_RES_BODY"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT: // "CONFIG_DIR_RES_BODY_LIMIT"
      case symbol_kind::S_CONFIG_DIR_RES_BODY_LIMIT_ACTION: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_INHERITANCE: // "CONFIG_SEC_RULE_INHERITANCE"
      case symbol_kind::S_CONFIG_SEC_RULE_PERF_TIME: // "CONFIG_SEC_RULE_PERF_TIME"
      case symbol_kind::S_CONFIG_DIR_RULE_ENG: // "CONFIG_DIR_RULE_ENG"
      case symbol_kind::S_CONFIG_DIR_SEC_ACTION: // "CONFIG_DIR_SEC_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_DEFAULT_ACTION: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case symbol_kind::S_CONFIG_DIR_SEC_MARKER: // "CONFIG_DIR_SEC_MARKER"
      case symbol_kind::S_CONFIG_DIR_UNICODE_MAP_FILE: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case symbol_kind::S_CONFIG_DIR_UNICODE_CODE_PAGE: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case symbol_kind::S_CONFIG_SEC_COLLECTION_TIMEOUT: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case symbol_kind::S_CONFIG_SEC_HTTP_BLKEY: // "CONFIG_SEC_HTTP_BLKEY"
      case symbol_kind::S_CONFIG_SEC_INTERCEPT_ON_ERROR: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case symbol_kind::S_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_ID: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_MSG: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_REMOVE_BY_TAG: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case symbol_kind::S_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case symbol_kind::S_CONFIG_UPDLOAD_KEEP_FILES: // "CONFIG_UPDLOAD_KEEP_FILES"
      case symbol_kind::S_CONFIG_UPDLOAD_SAVE_TMP_FILES: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case symbol_kind::S_CONFIG_UPLOAD_DIR: // "CONFIG_UPLOAD_DIR"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_LIMIT: // "CONFIG_UPLOAD_FILE_LIMIT"
      case symbol_kind::S_CONFIG_UPLOAD_FILE_MODE: // "CONFIG_UPLOAD_FILE_MODE"
      case symbol_kind::S_CONFIG_VALUE_ABORT: // "CONFIG_VALUE_ABORT"
      case symbol_kind::S_CONFIG_VALUE_DETC: // "CONFIG_VALUE_DETC"
      case symbol_kind::S_CONFIG_VALUE_HTTPS: // "CONFIG_VALUE_HTTPS"
      case symbol_kind::S_CONFIG_VALUE_OFF: // "CONFIG_VALUE_OFF"
      case symbol_kind::S_CONFIG_VALUE_ON: // "CONFIG_VALUE_ON"
      case symbol_kind::S_CONFIG_VALUE_PARALLEL: // "CONFIG_VALUE_PARALLEL"
      case symbol_kind::S_CONFIG_VALUE_PROCESS_PARTIAL: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case symbol_kind::S_CONFIG_VALUE_REJECT: // "CONFIG_VALUE_REJECT"
      case symbol_kind::S_CONFIG_VALUE_RELEVANT_ONLY: // "CONFIG_VALUE_RELEVANT_ONLY"
      case symbol_kind::S_CONFIG_VALUE_SERIAL: // "CONFIG_VALUE_SERIAL"
      case symbol_kind::S_CONFIG_VALUE_WARN: // "CONFIG_VALUE_WARN"
      case symbol_kind::S_CONFIG_XML_EXTERNAL_ENTITY: // "CONFIG_XML_EXTERNAL_ENTITY"
      case symbol_kind::S_CONGIG_DIR_RESPONSE_BODY_MP: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case symbol_kind::S_CONGIG_DIR_SEC_ARG_SEP: // "CONGIG_DIR_SEC_ARG_SEP"
      case symbol_kind::S_CONGIG_DIR_SEC_COOKIE_FORMAT: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case symbol_kind::S_CONFIG_SEC_COOKIEV0_SEPARATOR: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case symbol_kind::S_CONGIG_DIR_SEC_DATA_DIR: // "CONGIG_DIR_SEC_DATA_DIR"
      case symbol_kind::S_CONGIG_DIR_SEC_STATUS_ENGINE: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case symbol_kind::S_CONFIG_SEC_STREAM_IN_BODY_INSPECTION: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case symbol_kind::S_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case symbol_kind::S_CONGIG_DIR_SEC_TMP_DIR: // "CONGIG_DIR_SEC_TMP_DIR"
      case symbol_kind::S_DIRECTIVE: // "DIRECTIVE"
      case symbol_kind::S_DIRECTIVE_SECRULESCRIPT: // "DIRECTIVE_SECRULESCRIPT"
      case symbol_kind::S_FREE_TEXT_QUOTE_MACRO_EXPANSION: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case symbol_kind::S_QUOTATION_MARK: // "QUOTATION_MARK"
      case symbol_kind::S_RUN_TIME_VAR_BLD: // "RUN_TIME_VAR_BLD"
      case symbol_kind::S_RUN_TIME_VAR_DUR: // "RUN_TIME_VAR_DUR"
      case symbol_kind::S_RUN_TIME_VAR_HSV: // "RUN_TIME_VAR_HSV"
      case symbol_kind::S_RUN_TIME_VAR_REMOTE_USER: // "RUN_TIME_VAR_REMOTE_USER"
      case symbol_kind::S_RUN_TIME_VAR_TIME: // "RUN_TIME_VAR_TIME"
      case symbol_kind::S_RUN_TIME_VAR_TIME_DAY: // "RUN_TIME_VAR_TIME_DAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_EPOCH: // "RUN_TIME_VAR_TIME_EPOCH"
      case symbol_kind::S_RUN_TIME_VAR_TIME_HOUR: // "RUN_TIME_VAR_TIME_HOUR"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MIN: // "RUN_TIME_VAR_TIME_MIN"
      case symbol_kind::S_RUN_TIME_VAR_TIME_MON: // "RUN_TIME_VAR_TIME_MON"
      case symbol_kind::S_RUN_TIME_VAR_TIME_SEC: // "RUN_TIME_VAR_TIME_SEC"
      case symbol_kind::S_RUN_TIME_VAR_TIME_WDAY: // "RUN_TIME_VAR_TIME_WDAY"
      case symbol_kind::S_RUN_TIME_VAR_TIME_YEAR: // "RUN_TIME_VAR_TIME_YEAR"
      case symbol_kind::S_VARIABLE: // "VARIABLE"
      case symbol_kind::S_DICT_ELEMENT: // "Dictionary element"
      case symbol_kind::S_DICT_ELEMENT_REGEXP: // "Dictionary element, selected by regexp"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_op: // op
      case symbol_kind::S_op_before_init: // op_before_init
        yylhs.value.emplace< std::unique_ptr<Operator> > ();
        break;

      case symbol_kind::S_run_time_string: // run_time_string
        yylhs.value.emplace< std::unique_ptr<RunTimeString> > ();
        break;

      case symbol_kind::S_var: // var
        yylhs.value.emplace< std::unique_ptr<Variable> > ();
        break;

      case symbol_kind::S_act: // act
      case symbol_kind::S_setvar_action: // setvar_action
        yylhs.value.emplace< std::unique_ptr<actions::Action> > ();
        break;

      case symbol_kind::S_variables: // variables
      case symbol_kind::S_variables_pre_process: // variables_pre_process
      case symbol_kind::S_variables_may_be_quoted: // variables_may_be_quoted
        yylhs.value.emplace< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case symbol_kind::S_actions: // actions
      case symbol_kind::S_actions_may_quoted: // actions_may_quoted
        yylhs.value.emplace< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // input: "end of file"
#line 713 "seclang-parser.yy"
      {
        return 0;
      }
#line 1698 "seclang-parser.cc"
    break;

  case 6: // audit_log: "CONFIG_DIR_AUDIT_DIR_MOD"
#line 726 "seclang-parser.yy"
      {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8));
      }
#line 1706 "seclang-parser.cc"
    break;

  case 7: // audit_log: "CONFIG_DIR_AUDIT_DIR"
#line 732 "seclang-parser.yy"
      {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as < std::string > ());
      }
#line 1714 "seclang-parser.cc"
    break;

  case 8: // audit_log: "CONFIG_DIR_AUDIT_ENG" "CONFIG_VALUE_RELEVANT_ONLY"
#line 738 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1722 "seclang-parser.cc"
    break;

  case 9: // audit_log: "CONFIG_DIR_AUDIT_ENG" "CONFIG_VALUE_OFF"
#line 742 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1730 "seclang-parser.cc"
    break;

  case 10: // audit_log: "CONFIG_DIR_AUDIT_ENG" "CONFIG_VALUE_ON"
#line 746 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1738 "seclang-parser.cc"
    break;

  case 11: // audit_log: "CONFIG_DIR_AUDIT_FLE_MOD"
#line 752 "seclang-parser.yy"
      {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8));
      }
#line 1746 "seclang-parser.cc"
    break;

  case 12: // audit_log: "CONFIG_DIR_AUDIT_LOG2"
#line 758 "seclang-parser.yy"
      {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as < std::string > ());
      }
#line 1754 "seclang-parser.cc"
    break;

  case 13: // audit_log: "CONFIG_DIR_AUDIT_LOG_P"
#line 764 "seclang-parser.yy"
      {
        driver.m_auditLog->setParts(yystack_[0].value.as < std::string > ());
      }
#line 1762 "seclang-parser.cc"
    break;

  case 14: // audit_log: "CONFIG_DIR_AUDIT_LOG"
#line 770 "seclang-parser.yy"
      {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as < std::string > ());
      }
#line 1770 "seclang-parser.cc"
    break;

  case 15: // audit_log: CONFIG_DIR_AUDIT_LOG_FMT JSON
#line 775 "seclang-parser.yy"
      {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1778 "seclang-parser.cc"
    break;

  case 16: // audit_log: CONFIG_DIR_AUDIT_LOG_FMT NATIVE
#line 780 "seclang-parser.yy"
      {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1786 "seclang-parser.cc"
    break;

  case 17: // audit_log: "CONFIG_DIR_AUDIT_STS"
#line 786 "seclang-parser.yy"
      {
        std::string relevant_status(yystack_[0].value.as < std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1795 "seclang-parser.cc"
    break;

  case 18: // audit_log: "CONFIG_DIR_AUDIT_TPE" "CONFIG_VALUE_SERIAL"
#line 793 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1803 "seclang-parser.cc"
    break;

  case 19: // audit_log: "CONFIG_DIR_AUDIT_TPE" "CONFIG_VALUE_PARALLEL"
#line 797 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1811 "seclang-parser.cc"
    break;

  case 20: // audit_log: "CONFIG_DIR_AUDIT_TPE" "CONFIG_VALUE_HTTPS"
#line 801 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1819 "seclang-parser.cc"
    break;

  case 21: // audit_log: "CONFIG_UPDLOAD_KEEP_FILES" "CONFIG_VALUE_ON"
#line 807 "seclang-parser.yy"
      {
        driver.m_uploadKeepFiles = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 1827 "seclang-parser.cc"
    break;

  case 22: // audit_log: "CONFIG_UPDLOAD_KEEP_FILES" "CONFIG_VALUE_OFF"
#line 811 "seclang-parser.yy"
      {
        driver.m_uploadKeepFiles = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 1835 "seclang-parser.cc"
    break;

  case 23: // audit_log: "CONFIG_UPDLOAD_KEEP_FILES" "CONFIG_VALUE_RELEVANT_ONLY"
#line 815 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecUploadKeepFiles RelevantOnly is not currently supported. Accepted values are On or Off");
        YYERROR;
      }
#line 1844 "seclang-parser.cc"
    break;

  case 24: // audit_log: "CONFIG_UPLOAD_FILE_LIMIT"
#line 820 "seclang-parser.yy"
      {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 10);
      }
#line 1853 "seclang-parser.cc"
    break;

  case 25: // audit_log: "CONFIG_UPLOAD_FILE_MODE"
#line 825 "seclang-parser.yy"
      {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8);
      }
#line 1862 "seclang-parser.cc"
    break;

  case 26: // audit_log: "CONFIG_UPLOAD_DIR"
#line 830 "seclang-parser.yy"
      {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as < std::string > ();
      }
#line 1871 "seclang-parser.cc"
    break;

  case 27: // audit_log: "CONFIG_UPDLOAD_SAVE_TMP_FILES" "CONFIG_VALUE_ON"
#line 835 "seclang-parser.yy"
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 1879 "seclang-parser.cc"
    break;

  case 28: // audit_log: "CONFIG_UPDLOAD_SAVE_TMP_FILES" "CONFIG_VALUE_OFF"
#line 839 "seclang-parser.yy"
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 1887 "seclang-parser.cc"
    break;

  case 29: // actions: "QUOTATION_MARK" actions_may_quoted "QUOTATION_MARK"
#line 846 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1895 "seclang-parser.cc"
    break;

  case 30: // actions: actions_may_quoted
#line 850 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1903 "seclang-parser.cc"
    break;

  case 31: // actions_may_quoted: actions_may_quoted "," act
#line 857 "seclang-parser.yy"
      {
        ACTION_INIT(yystack_[0].value.as < std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1913 "seclang-parser.cc"
    break;

  case 32: // actions_may_quoted: act
#line 863 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as < std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1924 "seclang-parser.cc"
    break;

  case 33: // op: op_before_init
#line 873 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as < std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[0].location.end.filename, &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1937 "seclang-parser.cc"
    break;

  case 34: // op: "NOT" op_before_init
#line 882 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as < std::unique_ptr<Operator> > ());
        yylhs.value.as < std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[1].location.end.filename, &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1951 "seclang-parser.cc"
    break;

  case 35: // op: run_time_string
#line 892 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[0].location.end.filename, &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1964 "seclang-parser.cc"
    break;

  case 36: // op: "NOT" run_time_string
#line 901 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
        yylhs.value.as < std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[1].location.end.filename, &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1978 "seclang-parser.cc"
    break;

  case 37: // op_before_init: "OPERATOR_UNCONDITIONAL_MATCH"
#line 914 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1986 "seclang-parser.cc"
    break;

  case 38: // op_before_init: "OPERATOR_DETECT_SQLI"
#line 918 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1994 "seclang-parser.cc"
    break;

  case 39: // op_before_init: "OPERATOR_DETECT_XSS"
#line 922 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 2002 "seclang-parser.cc"
    break;

  case 40: // op_before_init: "OPERATOR_VALIDATE_URL_ENCODING"
#line 926 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 2010 "seclang-parser.cc"
    break;

  case 41: // op_before_init: "OPERATOR_VALIDATE_UTF8_ENCODING"
#line 930 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 2018 "seclang-parser.cc"
    break;

  case 42: // op_before_init: "OPERATOR_INSPECT_FILE" run_time_string
#line 934 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::InspectFile(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2026 "seclang-parser.cc"
    break;

  case 43: // op_before_init: "OPERATOR_FUZZY_HASH" run_time_string
#line 938 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::FuzzyHash(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2034 "seclang-parser.cc"
    break;

  case 44: // op_before_init: "OPERATOR_VALIDATE_BYTE_RANGE" run_time_string
#line 942 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateByteRange(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2042 "seclang-parser.cc"
    break;

  case 45: // op_before_init: "OPERATOR_VALIDATE_DTD" run_time_string
#line 946 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateDTD(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2050 "seclang-parser.cc"
    break;

  case 46: // op_before_init: "OPERATOR_VALIDATE_HASH" run_time_string
#line 950 "seclang-parser.yy"
      {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 2059 "seclang-parser.cc"
    break;

  case 47: // op_before_init: "OPERATOR_VALIDATE_SCHEMA" run_time_string
#line 955 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateSchema(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2067 "seclang-parser.cc"
    break;

  case 48: // op_before_init: "OPERATOR_VERIFY_CC" run_time_string
#line 959 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifyCC(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2075 "seclang-parser.cc"
    break;

  case 49: // op_before_init: "OPERATOR_VERIFY_CPF" run_time_string
#line 963 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifyCPF(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2083 "seclang-parser.cc"
    break;

  case 50: // op_before_init: "OPERATOR_VERIFY_SSN" run_time_string
#line 967 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifySSN(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2091 "seclang-parser.cc"
    break;

  case 51: // op_before_init: "OPERATOR_VERIFY_SVNR" run_time_string
#line 971 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifySVNR(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2099 "seclang-parser.cc"
    break;

  case 52: // op_before_init: "OPERATOR_GSB_LOOKUP" run_time_string
#line 975 "seclang-parser.yy"
      {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 2108 "seclang-parser.cc"
    break;

  case 53: // op_before_init: "OPERATOR_RSUB" run_time_string
#line 980 "seclang-parser.yy"
      {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 2117 "seclang-parser.cc"
    break;

  case 54: // op_before_init: "OPERATOR_WITHIN" run_time_string
#line 985 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Within(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2125 "seclang-parser.cc"
    break;

  case 55: // op_before_init: "OPERATOR_CONTAINS_WORD" run_time_string
#line 989 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ContainsWord(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2133 "seclang-parser.cc"
    break;

  case 56: // op_before_init: "OPERATOR_CONTAINS" run_time_string
#line 993 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Contains(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2141 "seclang-parser.cc"
    break;

  case 57: // op_before_init: "OPERATOR_ENDS_WITH" run_time_string
#line 997 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::EndsWith(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2149 "seclang-parser.cc"
    break;

  case 58: // op_before_init: "OPERATOR_EQ" run_time_string
#line 1001 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Eq(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2157 "seclang-parser.cc"
    break;

  case 59: // op_before_init: "OPERATOR_GE" run_time_string
#line 1005 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Ge(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2165 "seclang-parser.cc"
    break;

  case 60: // op_before_init: "OPERATOR_GT" run_time_string
#line 1009 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Gt(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2173 "seclang-parser.cc"
    break;

  case 61: // op_before_init: "OPERATOR_IP_MATCH_FROM_FILE" run_time_string
#line 1013 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::IpMatchF(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2181 "seclang-parser.cc"
    break;

  case 62: // op_before_init: "OPERATOR_IP_MATCH" run_time_string
#line 1017 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::IpMatch(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2189 "seclang-parser.cc"
    break;

  case 63: // op_before_init: "OPERATOR_LE" run_time_string
#line 1021 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Le(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2197 "seclang-parser.cc"
    break;

  case 64: // op_before_init: "OPERATOR_LT" run_time_string
#line 1025 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Lt(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2205 "seclang-parser.cc"
    break;

  case 65: // op_before_init: "OPERATOR_PM_FROM_FILE" run_time_string
#line 1029 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::PmFromFile(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2213 "seclang-parser.cc"
    break;

  case 66: // op_before_init: "OPERATOR_PM" run_time_string
#line 1033 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Pm(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2221 "seclang-parser.cc"
    break;

  case 67: // op_before_init: "OPERATOR_RBL" run_time_string
#line 1037 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rbl(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2229 "seclang-parser.cc"
    break;

  case 68: // op_before_init: "OPERATOR_RX" run_time_string
#line 1041 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2237 "seclang-parser.cc"
    break;

  case 69: // op_before_init: "OPERATOR_RX_GLOBAL" run_time_string
#line 1045 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::RxGlobal(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2245 "seclang-parser.cc"
    break;

  case 70: // op_before_init: "OPERATOR_STR_EQ" run_time_string
#line 1049 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::StrEq(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2253 "seclang-parser.cc"
    break;

  case 71: // op_before_init: "OPERATOR_STR_MATCH" run_time_string
#line 1053 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::StrMatch(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2261 "seclang-parser.cc"
    break;

  case 72: // op_before_init: "OPERATOR_BEGINS_WITH" run_time_string
#line 1057 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::BeginsWith(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2269 "seclang-parser.cc"
    break;

  case 73: // op_before_init: "OPERATOR_GEOLOOKUP"
#line 1061 "seclang-parser.yy"
      {
#if defined(WITH_GEOIP) or defined(WITH_MAXMIND)
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::GeoLookup());
#else
        std::stringstream ss;
            ss << "This version of ModSecurity was not compiled with GeoIP or MaxMind support.";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
#endif  // WITH_GEOIP
      }
#line 2284 "seclang-parser.cc"
    break;

  case 75: // expression: "DIRECTIVE" variables op actions
#line 1076 "seclang-parser.yy"
      {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        std::vector<actions::transformations::Transformation *> *t = new std::vector<actions::transformations::Transformation *>();
        for (auto &i : *yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            if (dynamic_cast<actions::transformations::Transformation *>(i.get())) {
              t->push_back(dynamic_cast<actions::transformations::Transformation *>(i.release()));
            } else {
              a->push_back(i.release());
            }
        }
        variables::Variables *v = new variables::Variables();
        for (auto &i : *yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ().get()) {
            v->push_back(i.release());
        }

        Operator *op = yystack_[1].value.as < std::unique_ptr<Operator> > ().release();
        std::unique_ptr<RuleWithOperator> rule(new RuleWithOperator(
            /* op */ op,
            /* variables */ v,
            /* actions */ a,
            /* transformations */ t,
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[3].location.end.filename)),
            /* line number */ yystack_[3].location.end.line
            ));

        if (driver.addSecRule(std::move(rule)) == false) {
            YYERROR;
        }
      }
#line 2318 "seclang-parser.cc"
    break;

  case 76: // expression: "DIRECTIVE" variables op
#line 1106 "seclang-parser.yy"
      {
        variables::Variables *v = new variables::Variables();
        for (auto &i : *yystack_[1].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ().get()) {
            v->push_back(i.release());
        }

        std::unique_ptr<RuleWithOperator> rule(new RuleWithOperator(
            /* op */ yystack_[0].value.as < std::unique_ptr<Operator> > ().release(),
            /* variables */ v,
            /* actions */ NULL,
            /* transformations */ NULL,
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[2].location.end.filename)),
            /* line number */ yystack_[2].location.end.line
            ));
        if (driver.addSecRule(std::move(rule)) == false) {
            YYERROR;
        }
      }
#line 2341 "seclang-parser.cc"
    break;

  case 77: // expression: "CONFIG_DIR_SEC_ACTION" actions
#line 1125 "seclang-parser.yy"
      {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        std::vector<actions::transformations::Transformation *> *t = new std::vector<actions::transformations::Transformation *>();
        for (auto &i : *yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            if (dynamic_cast<actions::transformations::Transformation *>(i.get())) {
              t->push_back(dynamic_cast<actions::transformations::Transformation *>(i.release()));
            } else {
              a->push_back(i.release());
            }
        }
        std::unique_ptr<RuleUnconditional> rule(new RuleUnconditional(
            /* actions */ a,
            /* transformations */ t,
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[1].location.end.filename)),
            /* line number */ yystack_[1].location.end.line
            ));
        driver.addSecAction(std::move(rule));
      }
#line 2364 "seclang-parser.cc"
    break;

  case 78: // expression: "DIRECTIVE_SECRULESCRIPT" actions
#line 1144 "seclang-parser.yy"
      {
        std::string err;
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        std::vector<actions::transformations::Transformation *> *t = new std::vector<actions::transformations::Transformation *>();
        for (auto &i : *yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            if (dynamic_cast<actions::transformations::Transformation *>(i.get())) {
              t->push_back(dynamic_cast<actions::transformations::Transformation *>(i.release()));
            } else {
              a->push_back(i.release());
            }
        }
        std::unique_ptr<RuleScript> r(new RuleScript(
            /* path to script */ yystack_[1].value.as < std::string > (),
            /* actions */ a,
            /* transformations */ t,
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[1].location.end.filename)),
            /* line number */ yystack_[1].location.end.line
            ));

        if (r->init(&err) == false) {
            driver.error(yystack_[2].location, "Failed to load script: " + err);
            YYERROR;
        }
        if (driver.addSecRuleScript(std::move(r)) == false) {
            YYERROR;
        }
      }
#line 2396 "seclang-parser.cc"
    break;

  case 79: // expression: "CONFIG_DIR_SEC_DEFAULT_ACTION" actions
#line 1172 "seclang-parser.yy"
      {
        bool hasDisruptive = false;
        std::vector<actions::Action *> *actions = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            actions->push_back(i.release());
        }
        std::vector<actions::Action *> checkedActions;
        int definedPhase = -1;
        int secRuleDefinedPhase = -1;
        for (actions::Action *a : *actions) {
            actions::Phase *phase = dynamic_cast<actions::Phase *>(a);
            if (a->isDisruptive() == true && dynamic_cast<actions::Block *>(a) == NULL) {
                hasDisruptive = true;
            }
            if (phase != NULL) {
                definedPhase = phase->m_phase;
                secRuleDefinedPhase = phase->m_secRulesPhase;
                delete phase;
            } else if (a->action_kind == actions::Action::RunTimeOnlyIfMatchKind ||
                a->action_kind == actions::Action::RunTimeBeforeMatchAttemptKind) {
                                actions::transformations::None *none = dynamic_cast<actions::transformations::None *>(a);
                if (none != NULL) {
                    driver.error(yystack_[2].location, "The transformation none is not suitable to be part of the SecDefaultActions");
                    YYERROR;
                }
                checkedActions.push_back(a);
            } else {
                driver.error(yystack_[2].location, "The action '" + *a->m_name.get() + "' is not suitable to be part of the SecDefaultActions");
                YYERROR;
            }
        }
        if (definedPhase == -1) {
            definedPhase = modsecurity::Phases::RequestHeadersPhase;
        }

        if (hasDisruptive == false) {
            driver.error(yystack_[2].location, "SecDefaultAction must specify a disruptive action.");
            YYERROR;
        }

        if (!driver.m_defaultActions[definedPhase].empty()) {
            std::stringstream ss;
            ss << "SecDefaultActions can only be placed once per phase and configuration context. Phase ";
            ss << secRuleDefinedPhase;
            ss << " was informed already.";
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }

        for (actions::Action *a : checkedActions) {
            driver.m_defaultActions[definedPhase].push_back(
                std::unique_ptr<actions::Action>(a));
        }

        delete actions;
      }
#line 2457 "seclang-parser.cc"
    break;

  case 80: // expression: "CONFIG_DIR_SEC_MARKER"
#line 1229 "seclang-parser.yy"
      {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as < std::string > ()),
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[0].location.end.filename)),
            /* line number */ yystack_[0].location.end.line
        );
      }
#line 2468 "seclang-parser.cc"
    break;

  case 81: // expression: "CONFIG_DIR_RULE_ENG" "CONFIG_VALUE_OFF"
#line 1236 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::DisabledRuleEngine;
      }
#line 2476 "seclang-parser.cc"
    break;

  case 82: // expression: "CONFIG_DIR_RULE_ENG" "CONFIG_VALUE_ON"
#line 1240 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::EnabledRuleEngine;
      }
#line 2484 "seclang-parser.cc"
    break;

  case 83: // expression: "CONFIG_DIR_RULE_ENG" "CONFIG_VALUE_DETC"
#line 1244 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::DetectionOnlyRuleEngine;
      }
#line 2492 "seclang-parser.cc"
    break;

  case 84: // expression: "CONFIG_DIR_REQ_BODY" "CONFIG_VALUE_ON"
#line 1248 "seclang-parser.yy"
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 2500 "seclang-parser.cc"
    break;

  case 85: // expression: "CONFIG_DIR_REQ_BODY" "CONFIG_VALUE_OFF"
#line 1252 "seclang-parser.yy"
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 2508 "seclang-parser.cc"
    break;

  case 86: // expression: "CONFIG_DIR_RES_BODY" "CONFIG_VALUE_ON"
#line 1256 "seclang-parser.yy"
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 2516 "seclang-parser.cc"
    break;

  case 87: // expression: "CONFIG_DIR_RES_BODY" "CONFIG_VALUE_OFF"
#line 1260 "seclang-parser.yy"
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 2524 "seclang-parser.cc"
    break;

  case 88: // expression: "CONFIG_SEC_ARGUMENT_SEPARATOR"
#line 1264 "seclang-parser.yy"
      {
        if (yystack_[0].value.as < std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as < std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 2537 "seclang-parser.cc"
    break;

  case 89: // expression: "CONFIG_COMPONENT_SIG"
#line 1273 "seclang-parser.yy"
      {
        driver.m_components.push_back(yystack_[0].value.as < std::string > ());
      }
#line 2545 "seclang-parser.cc"
    break;

  case 90: // expression: "CONFIG_CONN_ENGINE" "CONFIG_VALUE_ON"
#line 1277 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecConnEngine is not yet supported.");
        YYERROR;
      }
#line 2554 "seclang-parser.cc"
    break;

  case 91: // expression: "CONFIG_CONN_ENGINE" "CONFIG_VALUE_OFF"
#line 1282 "seclang-parser.yy"
      {
      }
#line 2561 "seclang-parser.cc"
    break;

  case 92: // expression: "CONFIG_SEC_WEB_APP_ID"
#line 1285 "seclang-parser.yy"
      {
        driver.m_secWebAppId.m_value = yystack_[0].value.as < std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 2570 "seclang-parser.cc"
    break;

  case 93: // expression: "CONFIG_SEC_SERVER_SIG"
#line 1290 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2579 "seclang-parser.cc"
    break;

  case 94: // expression: "CONFIG_SEC_CACHE_TRANSFORMATIONS"
#line 1295 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2588 "seclang-parser.cc"
    break;

  case 95: // expression: "CONFIG_SEC_DISABLE_BACKEND_COMPRESS" "CONFIG_VALUE_ON"
#line 1300 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecDisableBackendCompression is not supported.");
        YYERROR;
      }
#line 2597 "seclang-parser.cc"
    break;

  case 96: // expression: "CONFIG_SEC_DISABLE_BACKEND_COMPRESS" "CONFIG_VALUE_OFF"
#line 1305 "seclang-parser.yy"
      {
      }
#line 2604 "seclang-parser.cc"
    break;

  case 97: // expression: "CONFIG_CONTENT_INJECTION" "CONFIG_VALUE_ON"
#line 1308 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecContentInjection is not yet supported.");
        YYERROR;
      }
#line 2613 "seclang-parser.cc"
    break;

  case 98: // expression: "CONFIG_CONTENT_INJECTION" "CONFIG_VALUE_OFF"
#line 1313 "seclang-parser.yy"
      {
      }
#line 2620 "seclang-parser.cc"
    break;

  case 99: // expression: "CONFIG_SEC_CHROOT_DIR"
#line 1316 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecChrootDir is not supported.");
        YYERROR;
      }
#line 2629 "seclang-parser.cc"
    break;

  case 100: // expression: "CONFIG_SEC_HASH_ENGINE" "CONFIG_VALUE_ON"
#line 1321 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecHashEngine is not yet supported.");
        YYERROR;
      }
#line 2638 "seclang-parser.cc"
    break;

  case 101: // expression: "CONFIG_SEC_HASH_ENGINE" "CONFIG_VALUE_OFF"
#line 1326 "seclang-parser.yy"
      {
      }
#line 2645 "seclang-parser.cc"
    break;

  case 102: // expression: "CONFIG_SEC_HASH_KEY"
#line 1329 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashKey is not yet supported.");
        YYERROR;
      }
#line 2654 "seclang-parser.cc"
    break;

  case 103: // expression: "CONFIG_SEC_HASH_PARAM"
#line 1334 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashParam is not yet supported.");
        YYERROR;
      }
#line 2663 "seclang-parser.cc"
    break;

  case 104: // expression: "CONFIG_SEC_HASH_METHOD_RX"
#line 1339 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashMethodRx is not yet supported.");
        YYERROR;
      }
#line 2672 "seclang-parser.cc"
    break;

  case 105: // expression: "CONFIG_SEC_HASH_METHOD_PM"
#line 1344 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashMethodPm is not yet supported.");
        YYERROR;
      }
#line 2681 "seclang-parser.cc"
    break;

  case 106: // expression: "CONFIG_DIR_GSB_DB"
#line 1349 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecGsbLookupDb is not supported.");
        YYERROR;
      }
#line 2690 "seclang-parser.cc"
    break;

  case 107: // expression: "CONFIG_SEC_GUARDIAN_LOG"
#line 1354 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecGuardianLog is not supported.");
        YYERROR;
      }
#line 2699 "seclang-parser.cc"
    break;

  case 108: // expression: "CONFIG_SEC_INTERCEPT_ON_ERROR" "CONFIG_VALUE_ON"
#line 1359 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecInterceptOnError is not yet supported.");
        YYERROR;
      }
#line 2708 "seclang-parser.cc"
    break;

  case 109: // expression: "CONFIG_SEC_INTERCEPT_ON_ERROR" "CONFIG_VALUE_OFF"
#line 1364 "seclang-parser.yy"
      {
      }
#line 2715 "seclang-parser.cc"
    break;

  case 110: // expression: "CONFIG_SEC_CONN_R_STATE_LIMIT"
#line 1367 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecConnReadStateLimit is not yet supported.");
        YYERROR;
      }
#line 2724 "seclang-parser.cc"
    break;

  case 111: // expression: "CONFIG_SEC_CONN_W_STATE_LIMIT"
#line 1372 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecConnWriteStateLimit is not yet supported.");
        YYERROR;
      }
#line 2733 "seclang-parser.cc"
    break;

  case 112: // expression: "CONFIG_SEC_SENSOR_ID"
#line 1377 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecSensorId is not yet supported.");
        YYERROR;
      }
#line 2742 "seclang-parser.cc"
    break;

  case 113: // expression: "CONFIG_SEC_RULE_INHERITANCE" "CONFIG_VALUE_ON"
#line 1382 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecRuleInheritance is not yet supported.");
        YYERROR;
      }
#line 2751 "seclang-parser.cc"
    break;

  case 114: // expression: "CONFIG_SEC_RULE_INHERITANCE" "CONFIG_VALUE_OFF"
#line 1387 "seclang-parser.yy"
      {
      }
#line 2758 "seclang-parser.cc"
    break;

  case 115: // expression: "CONFIG_SEC_RULE_PERF_TIME"
#line 1390 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecRulePerfTime is not yet supported.");
        YYERROR;
      }
#line 2767 "seclang-parser.cc"
    break;

  case 116: // expression: "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
#line 1395 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecStreamInBodyInspection is not supported.");
        YYERROR;
      }
#line 2776 "seclang-parser.cc"
    break;

  case 117: // expression: "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
#line 1400 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecStreamOutBodyInspection is not supported.");
        YYERROR;
      }
#line 2785 "seclang-parser.cc"
    break;

  case 118: // expression: "CONFIG_SEC_RULE_REMOVE_BY_ID"
#line 1405 "seclang-parser.yy"
      {
        std::string error;
        if (driver.m_exceptions.load(yystack_[0].value.as < std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveById: failed to load:";
            ss << yystack_[0].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2802 "seclang-parser.cc"
    break;

  case 119: // expression: "CONFIG_SEC_RULE_REMOVE_BY_TAG"
#line 1418 "seclang-parser.yy"
      {
        std::string error;
        if (driver.m_exceptions.loadRemoveRuleByTag(yystack_[0].value.as < std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveByTag: failed to load:";
            ss << yystack_[0].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2819 "seclang-parser.cc"
    break;

  case 120: // expression: "CONFIG_SEC_RULE_REMOVE_BY_MSG"
#line 1431 "seclang-parser.yy"
      {
        std::string error;
        if (driver.m_exceptions.loadRemoveRuleByMsg(yystack_[0].value.as < std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveByMsg: failed to load:";
            ss << yystack_[0].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2836 "seclang-parser.cc"
    break;

  case 121: // expression: "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG" variables_pre_process
#line 1444 "seclang-parser.yy"
      {
        std::string error;
        if (driver.m_exceptions.loadUpdateTargetByTag(yystack_[1].value.as < std::string > (), std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetByTag: failed to load:";
            ss << yystack_[1].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2853 "seclang-parser.cc"
    break;

  case 122: // expression: "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG" variables_pre_process
#line 1457 "seclang-parser.yy"
      {
        std::string error;
        if (driver.m_exceptions.loadUpdateTargetByMsg(yystack_[1].value.as < std::string > (), std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetByMsg: failed to load:";
            ss << yystack_[1].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2870 "seclang-parser.cc"
    break;

  case 123: // expression: "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID" variables_pre_process
#line 1470 "seclang-parser.yy"
      {
        std::string error;
        double ruleId;
        try {
            ruleId = std::stod(yystack_[1].value.as < std::string > ());
        } catch (...) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetById: failed to load:";
            ss << "The input \"" + yystack_[1].value.as < std::string > () + "\" does not ";
            ss << "seems to be a valid rule id.";
            ss << ". ";
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }

        if (driver.m_exceptions.loadUpdateTargetById(ruleId, std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetById: failed to load:";
            ss << yystack_[1].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2900 "seclang-parser.cc"
    break;

  case 124: // expression: "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID" actions
#line 1496 "seclang-parser.yy"
      {
        std::string error;
        double ruleId;
        try {
            ruleId = std::stod(yystack_[1].value.as < std::string > ());
        } catch (...) {
            std::stringstream ss;
            ss << "SecRuleUpdateActionById: failed to load:";
            ss << "The input \"" + yystack_[1].value.as < std::string > () + "\" does not ";
            ss << "seems to be a valid rule id.";
            ss << ". ";
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }


        if (driver.m_exceptions.loadUpdateActionById(ruleId, std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateActionById: failed to load:";
            ss << yystack_[1].value.as < std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2931 "seclang-parser.cc"
    break;

  case 125: // expression: "CONFIG_DIR_DEBUG_LVL"
#line 1524 "seclang-parser.yy"
      {
        if (driver.m_debugLog != NULL) {
          driver.m_debugLog->setDebugLogLevel(atoi(yystack_[0].value.as < std::string > ().c_str()));
        } else {
            std::stringstream ss;
            ss << "Internal error, there is no DebugLog ";
            ss << "object associated with the driver class";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2947 "seclang-parser.cc"
    break;

  case 126: // expression: "CONFIG_DIR_DEBUG_LOG"
#line 1536 "seclang-parser.yy"
      {
        if (driver.m_debugLog != NULL) {
            std::string error;
            driver.m_debugLog->setDebugLogFile(yystack_[0].value.as < std::string > (), &error);
            if (error.size() > 0) {
                std::stringstream ss;
                ss << "Failed to start DebugLog: " << error;
                driver.error(yystack_[1].location, ss.str());
                YYERROR;
            }
        } else {
            std::stringstream ss;
            ss << "Internal error, there is no DebugLog ";
            ss << "object associated with the driver class";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2970 "seclang-parser.cc"
    break;

  case 127: // expression: "CONFIG_DIR_GEO_DB"
#line 1556 "seclang-parser.yy"
      {
#if defined(WITH_GEOIP) or defined(WITH_MAXMIND)
        std::string err;
        std::string file = modsecurity::utils::find_resource(yystack_[0].value.as < std::string > (),
            *yystack_[0].location.end.filename, &err);
        if (file.empty()) {
            std::stringstream ss;
            ss << "Failed to load locate the GeoDB file from: " << yystack_[0].value.as < std::string > () << " ";
            ss << err;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
        if (Utils::GeoLookup::getInstance().setDataBase(file, &err) == false) {
            std::stringstream ss;
            ss << "Failed to load the GeoDB from: ";
            ss << file << ". " << err;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
#else
        std::stringstream ss;
        ss << "This version of ModSecurity was not compiled with GeoIP or MaxMind support.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
#endif  // WITH_GEOIP
      }
#line 3001 "seclang-parser.cc"
    break;

  case 128: // expression: "CONFIG_DIR_ARGS_LIMIT"
#line 1583 "seclang-parser.yy"
      {
        driver.m_argumentsLimit.m_set = true;
        driver.m_argumentsLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3010 "seclang-parser.cc"
    break;

  case 129: // expression: "CONFIG_DIR_REQ_BODY_LIMIT"
#line 1589 "seclang-parser.yy"
      {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3019 "seclang-parser.cc"
    break;

  case 130: // expression: "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
#line 1594 "seclang-parser.yy"
      {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3028 "seclang-parser.cc"
    break;

  case 131: // expression: "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
#line 1599 "seclang-parser.yy"
      {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 3041 "seclang-parser.cc"
    break;

  case 132: // expression: "CONFIG_DIR_RES_BODY_LIMIT"
#line 1608 "seclang-parser.yy"
      {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3050 "seclang-parser.cc"
    break;

  case 133: // expression: "CONFIG_DIR_REQ_BODY_LIMIT_ACTION" "CONFIG_VALUE_PROCESS_PARTIAL"
#line 1613 "seclang-parser.yy"
      {
        driver.m_requestBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 3058 "seclang-parser.cc"
    break;

  case 134: // expression: "CONFIG_DIR_REQ_BODY_LIMIT_ACTION" "CONFIG_VALUE_REJECT"
#line 1617 "seclang-parser.yy"
      {
        driver.m_requestBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::RejectBodyLimitAction;
      }
#line 3066 "seclang-parser.cc"
    break;

  case 135: // expression: "CONFIG_DIR_RES_BODY_LIMIT_ACTION" "CONFIG_VALUE_PROCESS_PARTIAL"
#line 1621 "seclang-parser.yy"
      {
        driver.m_responseBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 3074 "seclang-parser.cc"
    break;

  case 136: // expression: "CONFIG_DIR_RES_BODY_LIMIT_ACTION" "CONFIG_VALUE_REJECT"
#line 1625 "seclang-parser.yy"
      {
        driver.m_responseBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::RejectBodyLimitAction;
      }
#line 3082 "seclang-parser.cc"
    break;

  case 137: // expression: "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION" "CONFIG_VALUE_ABORT"
#line 1629 "seclang-parser.yy"
      {
        driver.m_remoteRulesActionOnFailed = RulesSet::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 3090 "seclang-parser.cc"
    break;

  case 138: // expression: "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION" "CONFIG_VALUE_WARN"
#line 1633 "seclang-parser.yy"
      {
        driver.m_remoteRulesActionOnFailed = RulesSet::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 3098 "seclang-parser.cc"
    break;

  case 141: // expression: "CONGIG_DIR_RESPONSE_BODY_MP"
#line 1647 "seclang-parser.yy"
      {
        std::istringstream buf(yystack_[0].value.as < std::string > ());
        std::istream_iterator<std::string> beg(buf), end;
        std::set<std::string> tokens(beg, end);
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        for (std::set<std::string>::iterator it=tokens.begin();
            it!=tokens.end(); ++it)
        {
            driver.m_responseBodyTypeToBeInspected.m_value.insert(*it);
        }
      }
#line 3114 "seclang-parser.cc"
    break;

  case 142: // expression: "CONGIG_DIR_RESPONSE_BODY_MP_CLEAR"
#line 1659 "seclang-parser.yy"
      {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 3124 "seclang-parser.cc"
    break;

  case 143: // expression: "CONFIG_XML_EXTERNAL_ENTITY" "CONFIG_VALUE_OFF"
#line 1665 "seclang-parser.yy"
      {
        driver.m_secXMLExternalEntity = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 3132 "seclang-parser.cc"
    break;

  case 144: // expression: "CONFIG_XML_EXTERNAL_ENTITY" "CONFIG_VALUE_ON"
#line 1669 "seclang-parser.yy"
      {
        driver.m_secXMLExternalEntity = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 3140 "seclang-parser.cc"
    break;

  case 145: // expression: "CONGIG_DIR_SEC_TMP_DIR"
#line 1673 "seclang-parser.yy"
      {
/* Parser error disabled to avoid breaking default installations with modsecurity.conf-recommended
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecTmpDir is no longer supported.";
        ss << " Instead, you can use your web server configurations to control when";
        ss << "and where to swap. ModSecurity will follow the web server decision.";
        driver.error(@0, ss.str());
        YYERROR;
*/
      }
#line 3155 "seclang-parser.cc"
    break;

  case 148: // expression: "CONGIG_DIR_SEC_COOKIE_FORMAT"
#line 1694 "seclang-parser.yy"
      {
        if (atoi(yystack_[0].value.as < std::string > ().c_str()) == 1) {
          driver.error(yystack_[1].location, "SecCookieFormat 1 is not yet supported.");
          YYERROR;
        }
      }
#line 3166 "seclang-parser.cc"
    break;

  case 149: // expression: "CONFIG_SEC_COOKIEV0_SEPARATOR"
#line 1701 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecCookieV0Separator is not yet supported.");
        YYERROR;
      }
#line 3175 "seclang-parser.cc"
    break;

  case 151: // expression: "CONFIG_DIR_UNICODE_MAP_FILE"
#line 1711 "seclang-parser.yy"
      {
        std::string error;
        std::vector<std::string> param;
        double num = 0;
        std::string f;
        std::string file;
        std::string err;
        param = utils::string::ssplit(yystack_[0].value.as < std::string > (), ' ');
        if (param.size() <= 1) {
            std::stringstream ss;
            ss << "Failed to process unicode map, missing ";
            ss << "parameter: " << yystack_[0].value.as < std::string > () << " ";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }

        try {
            num = std::stod(param.back());
        } catch (...) {
            std::stringstream ss;
            ss << "Failed to process unicode map, last parameter is ";
            ss << "expected to be a number: " << param.back() << " ";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
        param.pop_back();

        while (param.size() > 0) {
            if (f.empty()) {
                f = param.back();
            } else {
                f = param.back() + " " + f;
            }
            param.pop_back();
        }

        file = modsecurity::utils::find_resource(f, *yystack_[0].location.end.filename, &err);
        if (file.empty()) {
            std::stringstream ss;
            ss << "Failed to locate the unicode map file from: " << f << " ";
            ss << err;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }

        ConfigUnicodeMap::loadConfig(file, num, &driver, &error);

        if (!error.empty()) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }

      }
#line 3233 "seclang-parser.cc"
    break;

  case 152: // expression: "CONFIG_SEC_COLLECTION_TIMEOUT"
#line 1765 "seclang-parser.yy"
      {
/* Parser error disabled to avoid breaking default CRS installations with crs-setup.conf-recommended
        driver.error(@0, "SecCollectionTimeout is not yet supported.");
        YYERROR;
*/
      }
#line 3244 "seclang-parser.cc"
    break;

  case 153: // expression: "CONFIG_SEC_HTTP_BLKEY"
#line 1772 "seclang-parser.yy"
      {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as < std::string > ();
      }
#line 3253 "seclang-parser.cc"
    break;

  case 154: // variables: variables_pre_process
#line 1780 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable> > > originalList = std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> newList(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> newNewList(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> exclusionVars(new std::vector<std::unique_ptr<Variable>>());
        while (!originalList->empty()) {
            std::unique_ptr<Variable> var = std::move(originalList->back());
            originalList->pop_back();
            if (dynamic_cast<VariableModificatorExclusion*>(var.get())) {
                exclusionVars->push_back(std::move(var));
            } else {
                newList->push_back(std::move(var));
            }
        }

        while (!newList->empty()) {
            bool doNotAdd = false;
            std::unique_ptr<Variable> var = std::move(newList->back());
            newList->pop_back();
            for (auto &i : *exclusionVars) {
                if (*var == *i) {
                    doNotAdd = true;
                }
                if (i->belongsToCollection(var.get())) {
                    var->addsKeyExclusion(i.get());
                }
            }
            if (!doNotAdd) {
                newNewList->push_back(std::move(var));
            }
        }
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(newNewList);
      }
#line 3291 "seclang-parser.cc"
    break;

  case 155: // variables_pre_process: variables_may_be_quoted
#line 1817 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3299 "seclang-parser.cc"
    break;

  case 156: // variables_pre_process: "QUOTATION_MARK" variables_may_be_quoted "QUOTATION_MARK"
#line 1821 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3307 "seclang-parser.cc"
    break;

  case 157: // variables_may_be_quoted: variables_may_be_quoted PIPE var
#line 1828 "seclang-parser.yy"
      {
        yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3316 "seclang-parser.cc"
    break;

  case 158: // variables_may_be_quoted: variables_may_be_quoted PIPE VAR_EXCLUSION var
#line 1833 "seclang-parser.yy"
      {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3326 "seclang-parser.cc"
    break;

  case 159: // variables_may_be_quoted: variables_may_be_quoted PIPE VAR_COUNT var
#line 1839 "seclang-parser.yy"
      {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3336 "seclang-parser.cc"
    break;

  case 160: // variables_may_be_quoted: var
#line 1845 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3346 "seclang-parser.cc"
    break;

  case 161: // variables_may_be_quoted: VAR_EXCLUSION var
#line 1851 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3357 "seclang-parser.cc"
    break;

  case 162: // variables_may_be_quoted: VAR_COUNT var
#line 1858 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3368 "seclang-parser.cc"
    break;

  case 163: // var: VARIABLE_ARGS "Dictionary element"
#line 1868 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3376 "seclang-parser.cc"
    break;

  case 164: // var: VARIABLE_ARGS "Dictionary element, selected by regexp"
#line 1872 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3384 "seclang-parser.cc"
    break;

  case 165: // var: VARIABLE_ARGS
#line 1876 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_NoDictElement());
      }
#line 3392 "seclang-parser.cc"
    break;

  case 166: // var: VARIABLE_ARGS_POST "Dictionary element"
#line 1880 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3400 "seclang-parser.cc"
    break;

  case 167: // var: VARIABLE_ARGS_POST "Dictionary element, selected by regexp"
#line 1884 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3408 "seclang-parser.cc"
    break;

  case 168: // var: VARIABLE_ARGS_POST
#line 1888 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_NoDictElement());
      }
#line 3416 "seclang-parser.cc"
    break;

  case 169: // var: VARIABLE_ARGS_GET "Dictionary element"
#line 1892 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3424 "seclang-parser.cc"
    break;

  case 170: // var: VARIABLE_ARGS_GET "Dictionary element, selected by regexp"
#line 1896 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3432 "seclang-parser.cc"
    break;

  case 171: // var: VARIABLE_ARGS_GET
#line 1900 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_NoDictElement());
      }
#line 3440 "seclang-parser.cc"
    break;

  case 172: // var: VARIABLE_FILES_SIZES "Dictionary element"
#line 1904 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3448 "seclang-parser.cc"
    break;

  case 173: // var: VARIABLE_FILES_SIZES "Dictionary element, selected by regexp"
#line 1908 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3456 "seclang-parser.cc"
    break;

  case 174: // var: VARIABLE_FILES_SIZES
#line 1912 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_NoDictElement());
      }
#line 3464 "seclang-parser.cc"
    break;

  case 175: // var: VARIABLE_FILES_NAMES "Dictionary element"
#line 1916 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3472 "seclang-parser.cc"
    break;

  case 176: // var: VARIABLE_FILES_NAMES "Dictionary element, selected by regexp"
#line 1920 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3480 "seclang-parser.cc"
    break;

  case 177: // var: VARIABLE_FILES_NAMES
#line 1924 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_NoDictElement());
      }
#line 3488 "seclang-parser.cc"
    break;

  case 178: // var: VARIABLE_FILES_TMP_CONTENT "Dictionary element"
#line 1928 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3496 "seclang-parser.cc"
    break;

  case 179: // var: VARIABLE_FILES_TMP_CONTENT "Dictionary element, selected by regexp"
#line 1932 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3504 "seclang-parser.cc"
    break;

  case 180: // var: VARIABLE_FILES_TMP_CONTENT
#line 1936 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_NoDictElement());
      }
#line 3512 "seclang-parser.cc"
    break;

  case 181: // var: VARIABLE_MULTIPART_FILENAME "Dictionary element"
#line 1940 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3520 "seclang-parser.cc"
    break;

  case 182: // var: VARIABLE_MULTIPART_FILENAME "Dictionary element, selected by regexp"
#line 1944 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3528 "seclang-parser.cc"
    break;

  case 183: // var: VARIABLE_MULTIPART_FILENAME
#line 1948 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_NoDictElement());
      }
#line 3536 "seclang-parser.cc"
    break;

  case 184: // var: VARIABLE_MULTIPART_NAME "Dictionary element"
#line 1952 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3544 "seclang-parser.cc"
    break;

  case 185: // var: VARIABLE_MULTIPART_NAME "Dictionary element, selected by regexp"
#line 1956 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3552 "seclang-parser.cc"
    break;

  case 186: // var: VARIABLE_MULTIPART_NAME
#line 1960 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_NoDictElement());
      }
#line 3560 "seclang-parser.cc"
    break;

  case 187: // var: VARIABLE_MATCHED_VARS_NAMES "Dictionary element"
#line 1964 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3568 "seclang-parser.cc"
    break;

  case 188: // var: VARIABLE_MATCHED_VARS_NAMES "Dictionary element, selected by regexp"
#line 1968 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3576 "seclang-parser.cc"
    break;

  case 189: // var: VARIABLE_MATCHED_VARS_NAMES
#line 1972 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_NoDictElement());
      }
#line 3584 "seclang-parser.cc"
    break;

  case 190: // var: VARIABLE_MATCHED_VARS "Dictionary element"
#line 1976 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3592 "seclang-parser.cc"
    break;

  case 191: // var: VARIABLE_MATCHED_VARS "Dictionary element, selected by regexp"
#line 1980 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3600 "seclang-parser.cc"
    break;

  case 192: // var: VARIABLE_MATCHED_VARS
#line 1984 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_NoDictElement());
      }
#line 3608 "seclang-parser.cc"
    break;

  case 193: // var: VARIABLE_FILES "Dictionary element"
#line 1988 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3616 "seclang-parser.cc"
    break;

  case 194: // var: VARIABLE_FILES "Dictionary element, selected by regexp"
#line 1992 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3624 "seclang-parser.cc"
    break;

  case 195: // var: VARIABLE_FILES
#line 1996 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_NoDictElement());
      }
#line 3632 "seclang-parser.cc"
    break;

  case 196: // var: VARIABLE_REQUEST_COOKIES "Dictionary element"
#line 2000 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3640 "seclang-parser.cc"
    break;

  case 197: // var: VARIABLE_REQUEST_COOKIES "Dictionary element, selected by regexp"
#line 2004 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3648 "seclang-parser.cc"
    break;

  case 198: // var: VARIABLE_REQUEST_COOKIES
#line 2008 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_NoDictElement());
      }
#line 3656 "seclang-parser.cc"
    break;

  case 199: // var: VARIABLE_REQUEST_HEADERS "Dictionary element"
#line 2012 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3664 "seclang-parser.cc"
    break;

  case 200: // var: VARIABLE_REQUEST_HEADERS "Dictionary element, selected by regexp"
#line 2016 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3672 "seclang-parser.cc"
    break;

  case 201: // var: VARIABLE_REQUEST_HEADERS
#line 2020 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_NoDictElement());
      }
#line 3680 "seclang-parser.cc"
    break;

  case 202: // var: VARIABLE_RESPONSE_HEADERS "Dictionary element"
#line 2024 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3688 "seclang-parser.cc"
    break;

  case 203: // var: VARIABLE_RESPONSE_HEADERS "Dictionary element, selected by regexp"
#line 2028 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3696 "seclang-parser.cc"
    break;

  case 204: // var: VARIABLE_RESPONSE_HEADERS
#line 2032 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_NoDictElement());
      }
#line 3704 "seclang-parser.cc"
    break;

  case 205: // var: VARIABLE_GEO "Dictionary element"
#line 2036 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3712 "seclang-parser.cc"
    break;

  case 206: // var: VARIABLE_GEO "Dictionary element, selected by regexp"
#line 2040 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3720 "seclang-parser.cc"
    break;

  case 207: // var: VARIABLE_GEO
#line 2044 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_NoDictElement());
      }
#line 3728 "seclang-parser.cc"
    break;

  case 208: // var: VARIABLE_REQUEST_COOKIES_NAMES "Dictionary element"
#line 2048 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3736 "seclang-parser.cc"
    break;

  case 209: // var: VARIABLE_REQUEST_COOKIES_NAMES "Dictionary element, selected by regexp"
#line 2052 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3744 "seclang-parser.cc"
    break;

  case 210: // var: VARIABLE_REQUEST_COOKIES_NAMES
#line 2056 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_NoDictElement());
      }
#line 3752 "seclang-parser.cc"
    break;

  case 211: // var: VARIABLE_RULE "Dictionary element"
#line 2060 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3760 "seclang-parser.cc"
    break;

  case 212: // var: VARIABLE_RULE "Dictionary element, selected by regexp"
#line 2064 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3768 "seclang-parser.cc"
    break;

  case 213: // var: VARIABLE_RULE
#line 2068 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_NoDictElement());
      }
#line 3776 "seclang-parser.cc"
    break;

  case 214: // var: "RUN_TIME_VAR_ENV" "Dictionary element"
#line 2072 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV:" + yystack_[0].value.as < std::string > ()));
      }
#line 3784 "seclang-parser.cc"
    break;

  case 215: // var: "RUN_TIME_VAR_ENV" "Dictionary element, selected by regexp"
#line 2076 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV:" + yystack_[0].value.as < std::string > ()));
      }
#line 3792 "seclang-parser.cc"
    break;

  case 216: // var: "RUN_TIME_VAR_ENV"
#line 2080 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV"));
      }
#line 3800 "seclang-parser.cc"
    break;

  case 217: // var: "RUN_TIME_VAR_XML" "Dictionary element"
#line 2084 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML("XML:" + yystack_[0].value.as < std::string > ()));
      }
#line 3808 "seclang-parser.cc"
    break;

  case 218: // var: "RUN_TIME_VAR_XML" "Dictionary element, selected by regexp"
#line 2088 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML("XML:" + yystack_[0].value.as < std::string > ()));
      }
#line 3816 "seclang-parser.cc"
    break;

  case 219: // var: "RUN_TIME_VAR_XML"
#line 2092 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML_NoDictElement());
      }
#line 3824 "seclang-parser.cc"
    break;

  case 220: // var: "FILES_TMPNAMES" "Dictionary element"
#line 2096 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3832 "seclang-parser.cc"
    break;

  case 221: // var: "FILES_TMPNAMES" "Dictionary element, selected by regexp"
#line 2100 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3840 "seclang-parser.cc"
    break;

  case 222: // var: "FILES_TMPNAMES"
#line 2104 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_NoDictElement());
      }
#line 3848 "seclang-parser.cc"
    break;

  case 223: // var: "RESOURCE" run_time_string
#line 2108 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3856 "seclang-parser.cc"
    break;

  case 224: // var: "RESOURCE" "Dictionary element"
#line 2112 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3864 "seclang-parser.cc"
    break;

  case 225: // var: "RESOURCE" "Dictionary element, selected by regexp"
#line 2116 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3872 "seclang-parser.cc"
    break;

  case 226: // var: "RESOURCE"
#line 2120 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_NoDictElement());
      }
#line 3880 "seclang-parser.cc"
    break;

  case 227: // var: "VARIABLE_IP" run_time_string
#line 2124 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3888 "seclang-parser.cc"
    break;

  case 228: // var: "VARIABLE_IP" "Dictionary element"
#line 2128 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3896 "seclang-parser.cc"
    break;

  case 229: // var: "VARIABLE_IP" "Dictionary element, selected by regexp"
#line 2132 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3904 "seclang-parser.cc"
    break;

  case 230: // var: "VARIABLE_IP"
#line 2136 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_NoDictElement());
      }
#line 3912 "seclang-parser.cc"
    break;

  case 231: // var: "VARIABLE_GLOBAL" run_time_string
#line 2140 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3920 "seclang-parser.cc"
    break;

  case 232: // var: "VARIABLE_GLOBAL" "Dictionary element"
#line 2144 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3928 "seclang-parser.cc"
    break;

  case 233: // var: "VARIABLE_GLOBAL" "Dictionary element, selected by regexp"
#line 2148 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3936 "seclang-parser.cc"
    break;

  case 234: // var: "VARIABLE_GLOBAL"
#line 2152 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_NoDictElement());
      }
#line 3944 "seclang-parser.cc"
    break;

  case 235: // var: "VARIABLE_USER" run_time_string
#line 2156 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3952 "seclang-parser.cc"
    break;

  case 236: // var: "VARIABLE_USER" "Dictionary element"
#line 2160 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3960 "seclang-parser.cc"
    break;

  case 237: // var: "VARIABLE_USER" "Dictionary element, selected by regexp"
#line 2164 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3968 "seclang-parser.cc"
    break;

  case 238: // var: "VARIABLE_USER"
#line 2168 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_NoDictElement());
      }
#line 3976 "seclang-parser.cc"
    break;

  case 239: // var: "VARIABLE_TX" run_time_string
#line 2172 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3984 "seclang-parser.cc"
    break;

  case 240: // var: "VARIABLE_TX" "Dictionary element"
#line 2176 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3992 "seclang-parser.cc"
    break;

  case 241: // var: "VARIABLE_TX" "Dictionary element, selected by regexp"
#line 2180 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4000 "seclang-parser.cc"
    break;

  case 242: // var: "VARIABLE_TX"
#line 2184 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_NoDictElement());
      }
#line 4008 "seclang-parser.cc"
    break;

  case 243: // var: "VARIABLE_SESSION" run_time_string
#line 2188 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 4016 "seclang-parser.cc"
    break;

  case 244: // var: "VARIABLE_SESSION" "Dictionary element"
#line 2192 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4024 "seclang-parser.cc"
    break;

  case 245: // var: "VARIABLE_SESSION" "Dictionary element, selected by regexp"
#line 2196 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4032 "seclang-parser.cc"
    break;

  case 246: // var: "VARIABLE_SESSION"
#line 2200 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_NoDictElement());
      }
#line 4040 "seclang-parser.cc"
    break;

  case 247: // var: "Variable ARGS_NAMES" "Dictionary element"
#line 2204 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4048 "seclang-parser.cc"
    break;

  case 248: // var: "Variable ARGS_NAMES" "Dictionary element, selected by regexp"
#line 2208 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4056 "seclang-parser.cc"
    break;

  case 249: // var: "Variable ARGS_NAMES"
#line 2212 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_NoDictElement());
      }
#line 4064 "seclang-parser.cc"
    break;

  case 250: // var: VARIABLE_ARGS_GET_NAMES "Dictionary element"
#line 2216 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4072 "seclang-parser.cc"
    break;

  case 251: // var: VARIABLE_ARGS_GET_NAMES "Dictionary element, selected by regexp"
#line 2220 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4080 "seclang-parser.cc"
    break;

  case 252: // var: VARIABLE_ARGS_GET_NAMES
#line 2224 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_NoDictElement());
      }
#line 4088 "seclang-parser.cc"
    break;

  case 253: // var: VARIABLE_ARGS_POST_NAMES "Dictionary element"
#line 2229 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4096 "seclang-parser.cc"
    break;

  case 254: // var: VARIABLE_ARGS_POST_NAMES "Dictionary element, selected by regexp"
#line 2233 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4104 "seclang-parser.cc"
    break;

  case 255: // var: VARIABLE_ARGS_POST_NAMES
#line 2237 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_NoDictElement());
      }
#line 4112 "seclang-parser.cc"
    break;

  case 256: // var: VARIABLE_REQUEST_HEADERS_NAMES "Dictionary element"
#line 2242 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4120 "seclang-parser.cc"
    break;

  case 257: // var: VARIABLE_REQUEST_HEADERS_NAMES "Dictionary element, selected by regexp"
#line 2246 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4128 "seclang-parser.cc"
    break;

  case 258: // var: VARIABLE_REQUEST_HEADERS_NAMES
#line 2250 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_NoDictElement());
      }
#line 4136 "seclang-parser.cc"
    break;

  case 259: // var: VARIABLE_RESPONSE_CONTENT_TYPE
#line 2255 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseContentType());
      }
#line 4144 "seclang-parser.cc"
    break;

  case 260: // var: VARIABLE_RESPONSE_HEADERS_NAMES "Dictionary element"
#line 2260 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4152 "seclang-parser.cc"
    break;

  case 261: // var: VARIABLE_RESPONSE_HEADERS_NAMES "Dictionary element, selected by regexp"
#line 2264 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4160 "seclang-parser.cc"
    break;

  case 262: // var: VARIABLE_RESPONSE_HEADERS_NAMES
#line 2268 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_NoDictElement());
      }
#line 4168 "seclang-parser.cc"
    break;

  case 263: // var: VARIABLE_ARGS_COMBINED_SIZE
#line 2272 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsCombinedSize());
      }
#line 4176 "seclang-parser.cc"
    break;

  case 264: // var: "AUTH_TYPE"
#line 2276 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::AuthType());
      }
#line 4184 "seclang-parser.cc"
    break;

  case 265: // var: "FILES_COMBINED_SIZE"
#line 2280 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesCombinedSize());
      }
#line 4192 "seclang-parser.cc"
    break;

  case 266: // var: "FULL_REQUEST"
#line 2284 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FullRequest());
      }
#line 4200 "seclang-parser.cc"
    break;

  case 267: // var: "FULL_REQUEST_LENGTH"
#line 2288 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FullRequestLength());
      }
#line 4208 "seclang-parser.cc"
    break;

  case 268: // var: "INBOUND_DATA_ERROR"
#line 2292 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::InboundDataError());
      }
#line 4216 "seclang-parser.cc"
    break;

  case 269: // var: "MATCHED_VAR"
#line 2296 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVar());
      }
#line 4224 "seclang-parser.cc"
    break;

  case 270: // var: "MATCHED_VAR_NAME"
#line 2300 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarName());
      }
#line 4232 "seclang-parser.cc"
    break;

  case 271: // var: VARIABLE_MULTIPART_BOUNDARY_QUOTED
#line 2304 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartBoundaryQuoted());
      }
#line 4240 "seclang-parser.cc"
    break;

  case 272: // var: VARIABLE_MULTIPART_BOUNDARY_WHITESPACE
#line 2308 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartBoundaryWhiteSpace());
      }
#line 4248 "seclang-parser.cc"
    break;

  case 273: // var: "MULTIPART_CRLF_LF_LINES"
#line 2312 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartCrlfLFLines());
      }
#line 4256 "seclang-parser.cc"
    break;

  case 274: // var: "MULTIPART_DATA_AFTER"
#line 2316 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartDateAfter());
      }
#line 4264 "seclang-parser.cc"
    break;

  case 275: // var: VARIABLE_MULTIPART_DATA_BEFORE
#line 2320 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartDateBefore());
      }
#line 4272 "seclang-parser.cc"
    break;

  case 276: // var: "MULTIPART_FILE_LIMIT_EXCEEDED"
#line 2324 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartFileLimitExceeded());
      }
#line 4280 "seclang-parser.cc"
    break;

  case 277: // var: "MULTIPART_HEADER_FOLDING"
#line 2328 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartHeaderFolding());
      }
#line 4288 "seclang-parser.cc"
    break;

  case 278: // var: "MULTIPART_INVALID_HEADER_FOLDING"
#line 2332 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidHeaderFolding());
      }
#line 4296 "seclang-parser.cc"
    break;

  case 279: // var: VARIABLE_MULTIPART_INVALID_PART
#line 2336 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidPart());
      }
#line 4304 "seclang-parser.cc"
    break;

  case 280: // var: "MULTIPART_INVALID_QUOTING"
#line 2340 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidQuoting());
      }
#line 4312 "seclang-parser.cc"
    break;

  case 281: // var: VARIABLE_MULTIPART_LF_LINE
#line 2344 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartLFLine());
      }
#line 4320 "seclang-parser.cc"
    break;

  case 282: // var: VARIABLE_MULTIPART_MISSING_SEMICOLON
#line 2348 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartMissingSemicolon());
      }
#line 4328 "seclang-parser.cc"
    break;

  case 283: // var: VARIABLE_MULTIPART_SEMICOLON_MISSING
#line 2352 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartMissingSemicolon());
      }
#line 4336 "seclang-parser.cc"
    break;

  case 284: // var: "MULTIPART_STRICT_ERROR"
#line 2356 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartStrictError());
      }
#line 4344 "seclang-parser.cc"
    break;

  case 285: // var: "MULTIPART_UNMATCHED_BOUNDARY"
#line 2360 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartUnmatchedBoundary());
      }
#line 4352 "seclang-parser.cc"
    break;

  case 286: // var: "OUTBOUND_DATA_ERROR"
#line 2364 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::OutboundDataError());
      }
#line 4360 "seclang-parser.cc"
    break;

  case 287: // var: "PATH_INFO"
#line 2368 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::PathInfo());
      }
#line 4368 "seclang-parser.cc"
    break;

  case 288: // var: "QUERY_STRING"
#line 2372 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::QueryString());
      }
#line 4376 "seclang-parser.cc"
    break;

  case 289: // var: "REMOTE_ADDR"
#line 2376 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemoteAddr());
      }
#line 4384 "seclang-parser.cc"
    break;

  case 290: // var: "REMOTE_HOST"
#line 2380 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemoteHost());
      }
#line 4392 "seclang-parser.cc"
    break;

  case 291: // var: "REMOTE_PORT"
#line 2384 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemotePort());
      }
#line 4400 "seclang-parser.cc"
    break;

  case 292: // var: "REQBODY_ERROR"
#line 2388 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyError());
      }
#line 4408 "seclang-parser.cc"
    break;

  case 293: // var: "REQBODY_ERROR_MSG"
#line 2392 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyErrorMsg());
      }
#line 4416 "seclang-parser.cc"
    break;

  case 294: // var: "REQBODY_PROCESSOR"
#line 2396 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessor());
      }
#line 4424 "seclang-parser.cc"
    break;

  case 295: // var: "REQBODY_PROCESSOR_ERROR"
#line 2400 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessorError());
      }
#line 4432 "seclang-parser.cc"
    break;

  case 296: // var: "REQBODY_PROCESSOR_ERROR_MSG"
#line 2404 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessorErrorMsg());
      }
#line 4440 "seclang-parser.cc"
    break;

  case 297: // var: "REQUEST_BASENAME"
#line 2408 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBasename());
      }
#line 4448 "seclang-parser.cc"
    break;

  case 298: // var: "REQUEST_BODY"
#line 2412 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBody());
      }
#line 4456 "seclang-parser.cc"
    break;

  case 299: // var: "REQUEST_BODY_LENGTH"
#line 2416 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBodyLength());
      }
#line 4464 "seclang-parser.cc"
    break;

  case 300: // var: "REQUEST_FILENAME"
#line 2420 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestFilename());
      }
#line 4472 "seclang-parser.cc"
    break;

  case 301: // var: "REQUEST_LINE"
#line 2424 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestLine());
      }
#line 4480 "seclang-parser.cc"
    break;

  case 302: // var: "REQUEST_METHOD"
#line 2428 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestMethod());
      }
#line 4488 "seclang-parser.cc"
    break;

  case 303: // var: "REQUEST_PROTOCOL"
#line 2432 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestProtocol());
      }
#line 4496 "seclang-parser.cc"
    break;

  case 304: // var: "REQUEST_URI"
#line 2436 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestURI());
      }
#line 4504 "seclang-parser.cc"
    break;

  case 305: // var: "REQUEST_URI_RAW"
#line 2440 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestURIRaw());
      }
#line 4512 "seclang-parser.cc"
    break;

  case 306: // var: "RESPONSE_BODY"
#line 2444 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseBody());
      }
#line 4520 "seclang-parser.cc"
    break;

  case 307: // var: "RESPONSE_CONTENT_LENGTH"
#line 2448 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseContentLength());
      }
#line 4528 "seclang-parser.cc"
    break;

  case 308: // var: "RESPONSE_PROTOCOL"
#line 2452 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseProtocol());
      }
#line 4536 "seclang-parser.cc"
    break;

  case 309: // var: "RESPONSE_STATUS"
#line 2456 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseStatus());
      }
#line 4544 "seclang-parser.cc"
    break;

  case 310: // var: "SERVER_ADDR"
#line 2460 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerAddr());
      }
#line 4552 "seclang-parser.cc"
    break;

  case 311: // var: "SERVER_NAME"
#line 2464 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerName());
      }
#line 4560 "seclang-parser.cc"
    break;

  case 312: // var: "SERVER_PORT"
#line 2468 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerPort());
      }
#line 4568 "seclang-parser.cc"
    break;

  case 313: // var: "SESSIONID"
#line 2472 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::SessionID());
      }
#line 4576 "seclang-parser.cc"
    break;

  case 314: // var: "UNIQUE_ID"
#line 2476 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UniqueID());
      }
#line 4584 "seclang-parser.cc"
    break;

  case 315: // var: "URLENCODED_ERROR"
#line 2480 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UrlEncodedError());
      }
#line 4592 "seclang-parser.cc"
    break;

  case 316: // var: "USERID"
#line 2484 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UserID());
      }
#line 4600 "seclang-parser.cc"
    break;

  case 317: // var: "VARIABLE_STATUS"
#line 2488 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Status());
      }
#line 4608 "seclang-parser.cc"
    break;

  case 318: // var: "VARIABLE_STATUS_LINE"
#line 2492 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Status());
      }
#line 4616 "seclang-parser.cc"
    break;

  case 319: // var: "WEBAPPID"
#line 2496 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::WebAppId());
      }
#line 4624 "seclang-parser.cc"
    break;

  case 320: // var: "RUN_TIME_VAR_DUR"
#line 2500 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4635 "seclang-parser.cc"
    break;

  case 321: // var: "RUN_TIME_VAR_BLD"
#line 2508 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4646 "seclang-parser.cc"
    break;

  case 322: // var: "RUN_TIME_VAR_HSV"
#line 2515 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4657 "seclang-parser.cc"
    break;

  case 323: // var: "RUN_TIME_VAR_REMOTE_USER"
#line 2522 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4668 "seclang-parser.cc"
    break;

  case 324: // var: "RUN_TIME_VAR_TIME"
#line 2529 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4679 "seclang-parser.cc"
    break;

  case 325: // var: "RUN_TIME_VAR_TIME_DAY"
#line 2536 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4690 "seclang-parser.cc"
    break;

  case 326: // var: "RUN_TIME_VAR_TIME_EPOCH"
#line 2543 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4701 "seclang-parser.cc"
    break;

  case 327: // var: "RUN_TIME_VAR_TIME_HOUR"
#line 2550 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4712 "seclang-parser.cc"
    break;

  case 328: // var: "RUN_TIME_VAR_TIME_MIN"
#line 2557 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4723 "seclang-parser.cc"
    break;

  case 329: // var: "RUN_TIME_VAR_TIME_MON"
#line 2564 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4734 "seclang-parser.cc"
    break;

  case 330: // var: "RUN_TIME_VAR_TIME_SEC"
#line 2571 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4745 "seclang-parser.cc"
    break;

  case 331: // var: "RUN_TIME_VAR_TIME_WDAY"
#line 2578 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4756 "seclang-parser.cc"
    break;

  case 332: // var: "RUN_TIME_VAR_TIME_YEAR"
#line 2585 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4767 "seclang-parser.cc"
    break;

  case 333: // act: "Accuracy"
#line 2595 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as < std::string > ()));
      }
#line 4775 "seclang-parser.cc"
    break;

  case 334: // act: "Allow"
#line 2599 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as < std::string > ()));
      }
#line 4783 "seclang-parser.cc"
    break;

  case 335: // act: "Append"
#line 2603 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 4791 "seclang-parser.cc"
    break;

  case 336: // act: "AuditLog"
#line 2607 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as < std::string > ()));
      }
#line 4799 "seclang-parser.cc"
    break;

  case 337: // act: "Block"
#line 2611 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Block(yystack_[0].value.as < std::string > ()));
      }
#line 4807 "seclang-parser.cc"
    break;

  case 338: // act: "Capture"
#line 2615 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as < std::string > ()));
      }
#line 4815 "seclang-parser.cc"
    break;

  case 339: // act: "Chain"
#line 2619 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as < std::string > ()));
      }
#line 4823 "seclang-parser.cc"
    break;

  case 340: // act: "ACTION_CTL_AUDIT_ENGINE" "CONFIG_VALUE_ON"
#line 2623 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4832 "seclang-parser.cc"
    break;

  case 341: // act: "ACTION_CTL_AUDIT_ENGINE" "CONFIG_VALUE_OFF"
#line 2628 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4841 "seclang-parser.cc"
    break;

  case 342: // act: "ACTION_CTL_AUDIT_ENGINE" "CONFIG_VALUE_RELEVANT_ONLY"
#line 2633 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4850 "seclang-parser.cc"
    break;

  case 343: // act: "ACTION_CTL_AUDIT_LOG_PARTS"
#line 2638 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as < std::string > ()));
      }
#line 4858 "seclang-parser.cc"
    break;

  case 344: // act: "ACTION_CTL_BDY_JSON"
#line 2642 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as < std::string > ()));
      }
#line 4866 "seclang-parser.cc"
    break;

  case 345: // act: "ACTION_CTL_BDY_XML"
#line 2646 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as < std::string > ()));
      }
#line 4874 "seclang-parser.cc"
    break;

  case 346: // act: "ACTION_CTL_BDY_URLENCODED"
#line 2650 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorURLENCODED(yystack_[0].value.as < std::string > ()));
      }
#line 4882 "seclang-parser.cc"
    break;

  case 347: // act: "ACTION_CTL_FORCE_REQ_BODY_VAR" "CONFIG_VALUE_ON"
#line 2654 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4891 "seclang-parser.cc"
    break;

  case 348: // act: "ACTION_CTL_FORCE_REQ_BODY_VAR" "CONFIG_VALUE_OFF"
#line 2659 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4900 "seclang-parser.cc"
    break;

  case 349: // act: "ACTION_CTL_REQUEST_BODY_ACCESS" "CONFIG_VALUE_ON"
#line 2664 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as < std::string > () + "true"));
      }
#line 4908 "seclang-parser.cc"
    break;

  case 350: // act: "ACTION_CTL_REQUEST_BODY_ACCESS" "CONFIG_VALUE_OFF"
#line 2668 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as < std::string > () + "false"));
      }
#line 4916 "seclang-parser.cc"
    break;

  case 351: // act: "ACTION_CTL_RULE_ENGINE" "CONFIG_VALUE_ON"
#line 2672 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 4924 "seclang-parser.cc"
    break;

  case 352: // act: "ACTION_CTL_RULE_ENGINE" "CONFIG_VALUE_OFF"
#line 2676 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 4932 "seclang-parser.cc"
    break;

  case 353: // act: "ACTION_CTL_RULE_ENGINE" "CONFIG_VALUE_DETC"
#line 2680 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 4940 "seclang-parser.cc"
    break;

  case 354: // act: "ACTION_CTL_RULE_REMOVE_BY_ID"
#line 2684 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as < std::string > ()));
      }
#line 4948 "seclang-parser.cc"
    break;

  case 355: // act: "ACTION_CTL_RULE_REMOVE_BY_TAG"
#line 2688 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveByTag(yystack_[0].value.as < std::string > ()));
      }
#line 4956 "seclang-parser.cc"
    break;

  case 356: // act: "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
#line 2692 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as < std::string > ()));
      }
#line 4964 "seclang-parser.cc"
    break;

  case 357: // act: "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
#line 2696 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as < std::string > ()));
      }
#line 4972 "seclang-parser.cc"
    break;

  case 358: // act: "Deny"
#line 2700 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as < std::string > ()));
      }
#line 4980 "seclang-parser.cc"
    break;

  case 359: // act: "DeprecateVar"
#line 2704 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 4988 "seclang-parser.cc"
    break;

  case 360: // act: "Drop"
#line 2708 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Drop(yystack_[0].value.as < std::string > ()));
      }
#line 4996 "seclang-parser.cc"
    break;

  case 361: // act: "Exec"
#line 2712 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as < std::string > ()));
      }
#line 5004 "seclang-parser.cc"
    break;

  case 362: // act: "ExpireVar"
#line 2716 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as < std::string > ()));
      }
#line 5013 "seclang-parser.cc"
    break;

  case 363: // act: "Id"
#line 2721 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as < std::string > ()));
      }
#line 5021 "seclang-parser.cc"
    break;

  case 364: // act: "InitCol" run_time_string
#line 2725 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as < std::string > (), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5029 "seclang-parser.cc"
    break;

  case 365: // act: "LogData" run_time_string
#line 2729 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5037 "seclang-parser.cc"
    break;

  case 366: // act: "Log"
#line 2733 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as < std::string > ()));
      }
#line 5045 "seclang-parser.cc"
    break;

  case 367: // act: "Maturity"
#line 2737 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as < std::string > ()));
      }
#line 5053 "seclang-parser.cc"
    break;

  case 368: // act: "Msg" run_time_string
#line 2741 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5061 "seclang-parser.cc"
    break;

  case 369: // act: "MultiMatch"
#line 2745 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as < std::string > ()));
      }
#line 5069 "seclang-parser.cc"
    break;

  case 370: // act: "NoAuditLog"
#line 2749 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as < std::string > ()));
      }
#line 5077 "seclang-parser.cc"
    break;

  case 371: // act: "NoLog"
#line 2753 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as < std::string > ()));
      }
#line 5085 "seclang-parser.cc"
    break;

  case 372: // act: "Pass"
#line 2757 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as < std::string > ()));
      }
#line 5093 "seclang-parser.cc"
    break;

  case 373: // act: "Pause"
#line 2761 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 5101 "seclang-parser.cc"
    break;

  case 374: // act: "Phase"
#line 2765 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as < std::string > ()));
      }
#line 5109 "seclang-parser.cc"
    break;

  case 375: // act: "Prepend"
#line 2769 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 5117 "seclang-parser.cc"
    break;

  case 376: // act: "Proxy"
#line 2773 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 5125 "seclang-parser.cc"
    break;

  case 377: // act: "Redirect" run_time_string
#line 2777 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5133 "seclang-parser.cc"
    break;

  case 378: // act: "Rev"
#line 2781 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as < std::string > ()));
      }
#line 5141 "seclang-parser.cc"
    break;

  case 379: // act: "SanitiseArg"
#line 2785 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseArg", yystack_[1].location);
      }
#line 5149 "seclang-parser.cc"
    break;

  case 380: // act: "SanitiseMatched"
#line 2789 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseMatched", yystack_[1].location);
      }
#line 5157 "seclang-parser.cc"
    break;

  case 381: // act: "SanitiseMatchedBytes"
#line 2793 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseMatchedBytes", yystack_[1].location);
      }
#line 5165 "seclang-parser.cc"
    break;

  case 382: // act: "SanitiseRequestHeader"
#line 2797 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseRequestHeader", yystack_[1].location);
      }
#line 5173 "seclang-parser.cc"
    break;

  case 383: // act: "SanitiseResponseHeader"
#line 2801 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseResponseHeader", yystack_[1].location);
      }
#line 5181 "seclang-parser.cc"
    break;

  case 384: // act: "SetEnv" run_time_string
#line 2805 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetENV(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5189 "seclang-parser.cc"
    break;

  case 385: // act: "SetRsc" run_time_string
#line 2809 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5197 "seclang-parser.cc"
    break;

  case 386: // act: "SetSid" run_time_string
#line 2813 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5205 "seclang-parser.cc"
    break;

  case 387: // act: "SetUID" run_time_string
#line 2817 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5213 "seclang-parser.cc"
    break;

  case 388: // act: "SetVar" setvar_action
#line 2821 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ());
      }
#line 5221 "seclang-parser.cc"
    break;

  case 389: // act: "Severity"
#line 2825 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as < std::string > ()));
      }
#line 5229 "seclang-parser.cc"
    break;

  case 390: // act: "Skip"
#line 2829 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as < std::string > ()));
      }
#line 5237 "seclang-parser.cc"
    break;

  case 391: // act: "SkipAfter"
#line 2833 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as < std::string > ()));
      }
#line 5245 "seclang-parser.cc"
    break;

  case 392: // act: "Status"
#line 2837 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as < std::string > ()));
      }
#line 5253 "seclang-parser.cc"
    break;

  case 393: // act: "Tag" run_time_string
#line 2841 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5261 "seclang-parser.cc"
    break;

  case 394: // act: "Ver"
#line 2845 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as < std::string > ()));
      }
#line 5269 "seclang-parser.cc"
    break;

  case 395: // act: "xmlns"
#line 2849 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as < std::string > ()));
      }
#line 5277 "seclang-parser.cc"
    break;

  case 396: // act: "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
#line 2853 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5285 "seclang-parser.cc"
    break;

  case 397: // act: "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
#line 2857 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5293 "seclang-parser.cc"
    break;

  case 398: // act: "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
#line 2861 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5301 "seclang-parser.cc"
    break;

  case 399: // act: "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
#line 2865 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5309 "seclang-parser.cc"
    break;

  case 400: // act: "ACTION_TRANSFORMATION_BASE_64_ENCODE"
#line 2869 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Encode(yystack_[0].value.as < std::string > ()));
      }
#line 5317 "seclang-parser.cc"
    break;

  case 401: // act: "ACTION_TRANSFORMATION_BASE_64_DECODE"
#line 2873 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Decode(yystack_[0].value.as < std::string > ()));
      }
#line 5325 "seclang-parser.cc"
    break;

  case 402: // act: "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
#line 2877 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64DecodeExt(yystack_[0].value.as < std::string > ()));
      }
#line 5333 "seclang-parser.cc"
    break;

  case 403: // act: "ACTION_TRANSFORMATION_CMD_LINE"
#line 2881 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as < std::string > ()));
      }
#line 5341 "seclang-parser.cc"
    break;

  case 404: // act: "ACTION_TRANSFORMATION_SHA1"
#line 2885 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as < std::string > ()));
      }
#line 5349 "seclang-parser.cc"
    break;

  case 405: // act: "ACTION_TRANSFORMATION_MD5"
#line 2889 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as < std::string > ()));
      }
#line 5357 "seclang-parser.cc"
    break;

  case 406: // act: "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
#line 2893 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::EscapeSeqDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5365 "seclang-parser.cc"
    break;

  case 407: // act: "ACTION_TRANSFORMATION_HEX_ENCODE"
#line 2897 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as < std::string > ()));
      }
#line 5373 "seclang-parser.cc"
    break;

  case 408: // act: "ACTION_TRANSFORMATION_HEX_DECODE"
#line 2901 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5381 "seclang-parser.cc"
    break;

  case 409: // act: "ACTION_TRANSFORMATION_LOWERCASE"
#line 2905 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as < std::string > ()));
      }
#line 5389 "seclang-parser.cc"
    break;

  case 410: // act: "ACTION_TRANSFORMATION_PHP_ARGS_NAMES"
#line 2909 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::PhpArgsNames(yystack_[0].value.as < std::string > ()));
      }
#line 5397 "seclang-parser.cc"
    break;

  case 411: // act: "ACTION_TRANSFORMATION_UPPERCASE"
#line 2913 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as < std::string > ()));
      }
#line 5405 "seclang-parser.cc"
    break;

  case 412: // act: "ACTION_TRANSFORMATION_URL_DECODE_UNI"
#line 2917 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as < std::string > ()));
      }
#line 5413 "seclang-parser.cc"
    break;

  case 413: // act: "ACTION_TRANSFORMATION_URL_DECODE"
#line 2921 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5421 "seclang-parser.cc"
    break;

  case 414: // act: "ACTION_TRANSFORMATION_URL_ENCODE"
#line 2925 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlEncode(yystack_[0].value.as < std::string > ()));
      }
#line 5429 "seclang-parser.cc"
    break;

  case 415: // act: "ACTION_TRANSFORMATION_NONE"
#line 2929 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as < std::string > ()));
      }
#line 5437 "seclang-parser.cc"
    break;

  case 416: // act: "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
#line 2933 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as < std::string > ()));
      }
#line 5445 "seclang-parser.cc"
    break;

  case 417: // act: "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
#line 2937 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as < std::string > ()));
      }
#line 5453 "seclang-parser.cc"
    break;

  case 418: // act: "ACTION_TRANSFORMATION_REPLACE_NULLS"
#line 2941 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as < std::string > ()));
      }
#line 5461 "seclang-parser.cc"
    break;

  case 419: // act: "ACTION_TRANSFORMATION_REMOVE_NULLS"
#line 2945 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as < std::string > ()));
      }
#line 5469 "seclang-parser.cc"
    break;

  case 420: // act: "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
#line 2949 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5477 "seclang-parser.cc"
    break;

  case 421: // act: "ACTION_TRANSFORMATION_JS_DECODE"
#line 2953 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5485 "seclang-parser.cc"
    break;

  case 422: // act: "ACTION_TRANSFORMATION_CSS_DECODE"
#line 2957 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5493 "seclang-parser.cc"
    break;

  case 423: // act: "ACTION_TRANSFORMATION_TRIM"
#line 2961 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as < std::string > ()));
      }
#line 5501 "seclang-parser.cc"
    break;

  case 424: // act: "ACTION_TRANSFORMATION_TRIM_LEFT"
#line 2965 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::TrimLeft(yystack_[0].value.as < std::string > ()));
      }
#line 5509 "seclang-parser.cc"
    break;

  case 425: // act: "ACTION_TRANSFORMATION_TRIM_RIGHT"
#line 2969 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::TrimRight(yystack_[0].value.as < std::string > ()));
      }
#line 5517 "seclang-parser.cc"
    break;

  case 426: // act: "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
#line 2973 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as < std::string > ()));
      }
#line 5525 "seclang-parser.cc"
    break;

  case 427: // act: "ACTION_TRANSFORMATION_NORMALISE_PATH"
#line 2977 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as < std::string > ()));
      }
#line 5533 "seclang-parser.cc"
    break;

  case 428: // act: "ACTION_TRANSFORMATION_LENGTH"
#line 2981 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as < std::string > ()));
      }
#line 5541 "seclang-parser.cc"
    break;

  case 429: // act: "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
#line 2985 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as < std::string > ()));
      }
#line 5549 "seclang-parser.cc"
    break;

  case 430: // act: "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
#line 2989 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as < std::string > ()));
      }
#line 5557 "seclang-parser.cc"
    break;

  case 431: // act: "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
#line 2993 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as < std::string > ()));
      }
#line 5565 "seclang-parser.cc"
    break;

  case 432: // act: "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
#line 2997 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as < std::string > ()));
      }
#line 5573 "seclang-parser.cc"
    break;

  case 433: // setvar_action: "NOT" var
#line 3004 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
      }
#line 5581 "seclang-parser.cc"
    break;

  case 434: // setvar_action: var
#line 3008 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
      }
#line 5589 "seclang-parser.cc"
    break;

  case 435: // setvar_action: var SETVAR_OPERATION_EQUALS run_time_string
#line 3012 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5597 "seclang-parser.cc"
    break;

  case 436: // setvar_action: var SETVAR_OPERATION_EQUALS_PLUS run_time_string
#line 3016 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5605 "seclang-parser.cc"
    break;

  case 437: // setvar_action: var SETVAR_OPERATION_EQUALS_MINUS run_time_string
#line 3020 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5613 "seclang-parser.cc"
    break;

  case 438: // run_time_string: run_time_string "FREE_TEXT_QUOTE_MACRO_EXPANSION"
#line 3027 "seclang-parser.yy"
      {
        yystack_[1].value.as < std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as < std::string > ());
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as < std::unique_ptr<RunTimeString> > ());
      }
#line 5622 "seclang-parser.cc"
    break;

  case 439: // run_time_string: run_time_string var
#line 3032 "seclang-parser.yy"
      {
        yystack_[1].value.as < std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as < std::unique_ptr<RunTimeString> > ());
      }
#line 5631 "seclang-parser.cc"
    break;

  case 440: // run_time_string: "FREE_TEXT_QUOTE_MACRO_EXPANSION"
#line 3037 "seclang-parser.yy"
      {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as < std::string > ());
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 5641 "seclang-parser.cc"
    break;

  case 441: // run_time_string: var
#line 3043 "seclang-parser.yy"
      {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 5651 "seclang-parser.cc"
    break;


#line 5655 "seclang-parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  seclang_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  seclang_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  seclang_parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // seclang_parser::context.
  seclang_parser::context::context (const seclang_parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  seclang_parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
  seclang_parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  seclang_parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short seclang_parser::yypact_ninf_ = -423;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short
  seclang_parser::yypact_[] =
  {
    2508,  -423,  -251,  -423,   -86,  -423,  -157,  -423,  -423,  -423,
    -423,  -423,  -275,  -423,  -423,  -423,  -423,  -423,  -287,  -423,
    -423,  -423,  -154,  -152,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -147,  -423,
    -423,  -148,  -423,  -143,  -423,  -144,  -132,  -423,  -261,  2856,
    2856,  -423,  -423,  -423,  -423,  -130,  -300,  -423,  -423,  -423,
    1150,  1150,  1150,  2856,  -269,  -128,  -423,  -423,  -423,  -126,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  1150,
    2856,  2669,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  2126,  -257,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -267,  -423,  -423,  -423,  -423,  -124,  -122,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    2220,  -423,  2220,  -423,  2220,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  2220,  -423,  -423,  -423,  -423,  -423,  -423,
    2220,  2220,  2220,  2220,  -423,  -423,  -423,  -423,  2220,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  3039,  -423,
       5,  -423,  -423,  -423,  -423,  -423,  -423,  2551,  2551,  -156,
    -153,  -151,  -149,  -146,  -142,  -139,  -137,  -135,  -131,  -129,
    -127,  -125,  -123,  -121,  -119,  -423,  -117,  -115,  -113,  -111,
    -423,  -423,  -109,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -107,  -423,
    -423,  -423,  -423,  -423,    50,  -423,  -423,  -423,  -105,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,   458,   547,   636,   970,  1059,  -103,  -101,  1483,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,     6,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  1663,  -423,  -423,  -423,  -423,
    2551,   -49,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  2325,  2325,  2325,  2325,  2325,
    2325,  2325,  2325,  2325,     4,  3039,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  2325,  -423,  -423,  -423,  -423,  2325,
    -423,  -423,  2325,  -423,  -423,  2325,  -423,  -423,  2325,  -423,
    -423,  2325,  -423,  -423,  -423,  -423,     9,  1574,  1994,  2220,
    2220,  2220,  -423,  -423,  2220,  2220,  2220,  -423,  2220,  2220,
    2220,  2220,  2220,  2220,  2220,  2220,  2220,  2220,  2220,  2220,
    2220,  2220,  2220,  2220,  -423,  2220,  2220,  2220,  2220,  -423,
    -423,  2220,  2220,  2220,  2220,  2220,  2856,  -423,  2325,  -423,
    2220,  2220,  2220,  -423,  -423,  -423,  -423,  -423,  2551,  2551,
    -423,  -423,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,
    2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,
    2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,  2325,
    2325,  2325,  2325,  2325,  -423,  2325,  2325,  2325,  -423,  -423
  };

  const short
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   142,     0,    89,     0,    88,    92,    93,
       7,     6,     0,    11,    14,    12,    13,    17,     0,   126,
     125,    94,     0,     0,   102,   103,   104,   105,    99,   127,
     106,   107,   140,   139,   110,   111,   112,   128,     0,   131,
     129,     0,   130,     0,   132,     0,     0,   115,     0,     0,
       0,    80,   151,   152,   153,     0,     0,   118,   120,   119,
       0,     0,     0,     0,     0,     0,    26,    24,    25,     0,
     141,   147,   148,   149,   146,   150,   116,   117,   145,     0,
       0,     0,     4,    74,     5,    98,    97,    15,    16,    91,
      90,     9,    10,     8,    20,    19,    18,    96,    95,   101,
     100,    85,    84,   133,   134,    87,    86,   135,   136,   114,
     113,    83,    81,    82,     0,     0,   333,   334,   335,   336,
     337,   338,   339,     0,   343,   344,   345,   346,     0,     0,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,   366,     0,   367,     0,   369,   370,   371,   372,   373,
     374,   375,   376,     0,   378,   379,   380,   381,   382,   383,
       0,     0,     0,     0,   389,   390,   391,   392,     0,   400,
     401,   402,   403,   416,   422,   406,   407,   408,   420,   421,
     428,   409,   410,   405,   415,   427,   426,   398,   397,   396,
     431,   430,   419,   417,   432,   418,   404,   399,   423,   424,
     425,   411,   414,   413,   412,   429,   394,   395,     0,    77,
      30,    32,    79,   109,   108,   137,   138,     0,     0,   165,
     168,   171,   174,   177,   180,   183,   186,   189,   192,   195,
     198,   201,   204,   207,   210,   263,   252,   213,   249,   255,
     264,   265,   222,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   293,
     292,   296,   295,   294,   297,   299,   298,   300,   258,   301,
     302,   303,   305,   304,   226,   306,   307,   259,   262,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   319,   317,
     318,   230,   234,   242,   246,   238,   216,   219,     0,   321,
     320,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   121,   155,   160,   122,   123,   124,    22,    21,
      23,    28,    27,   143,   144,     0,   154,    78,     1,     3,
       0,   434,   388,   353,   352,   351,   341,   340,   342,   348,
     347,   350,   349,   440,   441,   364,   365,   368,   377,   384,
     385,   386,   387,   393,     0,     0,   162,   161,   163,   164,
     166,   167,   169,   170,   172,   173,   175,   176,   178,   179,
     181,   182,   184,   185,   187,   188,   190,   191,   193,   194,
     196,   197,   199,   200,   202,   203,   205,   206,   208,   209,
     250,   251,   211,   212,   247,   248,   253,   254,   220,   221,
     256,   257,   224,   225,   223,   260,   261,   228,   229,   227,
     232,   233,   231,   240,   241,   239,   244,   245,   243,   236,
     237,   235,   214,   215,   217,   218,     0,     0,     0,     0,
       0,     0,    38,    39,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,    76,    33,    35,   433,
       0,     0,     0,   438,   439,    29,    31,   156,     0,     0,
     157,    34,    36,    72,    56,    55,    57,    58,    43,    59,
      52,    60,    42,    61,    62,    63,    64,    65,    66,    67,
      53,    68,    69,    70,    71,    44,    45,    46,    47,    48,
      49,    50,    51,    54,    75,   435,   436,   437,   159,   158
  };

  const short
  seclang_parser::yypgoto_[] =
  {
    -423,  -423,   -71,  -423,   -46,  -194,  -423,  -422,  -423,  -423,
     -56,  -286,   -60,  -341,  -423,  -133
  };

  const short
  seclang_parser::yydefgoto_[] =
  {
      -1,    81,    82,    83,   209,   210,   476,   477,    84,   335,
     322,   323,   354,   211,   342,   355
  };

  const short
  seclang_parser::yytable_[] =
  {
     324,   324,   324,   215,   212,   325,   326,   365,   365,   356,
     339,   357,   437,   216,   364,   437,   491,   327,    94,   324,
     358,    95,   436,   336,   486,    96,     0,   359,   360,   361,
     362,    91,    92,     0,   337,   363,    93,   328,   329,   346,
     347,     0,   330,   111,   348,   112,   113,   343,     0,   344,
     345,   480,   481,   482,   341,    85,    86,    87,    88,     0,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,    89,
      90,   414,    97,    98,    99,   100,     0,   366,   367,   101,
     102,   103,   104,   105,   106,   107,   108,     0,   419,   422,
     425,   428,   431,     0,   109,   110,   213,   214,   331,   332,
     333,   334,   349,   350,   351,   352,   368,   369,     0,   370,
     371,   372,   373,   374,   375,     0,   376,   377,     0,     0,
     378,   379,   478,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   415,   416,   432,
     433,   434,   435,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   484,   484,   484,   484,   484,
     484,   484,   484,   484,     0,   492,   493,   494,   495,     0,
       0,   496,   497,   498,     0,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   485,   515,   516,   517,   518,   487,     0,   519,   520,
     521,   522,   523,     0,     0,     0,     0,   525,   526,   527,
       0,     0,     0,     0,   484,     0,     0,     0,     0,   484,
       0,     0,   484,     0,     0,   484,     0,     0,   484,     0,
       0,   484,     0,     0,     0,     0,   353,   490,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,   412,   413,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,   529,
     524,     0,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,     0,   484,   484,   484,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   353,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,     0,
     417,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,     0,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,     0,   423,   424,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   353,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,   426,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   353,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
       0,   429,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   488,   489,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,     0,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,     0,     0,     0,     0,     0,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,     0,     0,     0,     0,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,     0,   340,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,     0,
     353,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   353,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
       4,   483,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,     0,     0,     0,   338,
       0,     0,     0,     2,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,     0,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,     4,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,   114,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,     0,     0,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207
  };

  const short
  seclang_parser::yycheck_[] =
  {
      60,    61,    62,   303,    50,    61,    62,     3,     3,   142,
      81,   144,     6,   313,   208,     6,   438,    63,   305,    79,
     153,   308,   308,    79,   365,   312,    -1,   160,   161,   162,
     163,   306,   307,    -1,    80,   168,   311,   306,   307,   306,
     307,    -1,   311,   304,   311,   306,   307,   304,    -1,   306,
     307,   100,   101,   102,   114,   306,   307,   143,   144,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,   306,
     307,   284,   306,   307,   306,   307,    -1,   217,   218,   306,
     307,   309,   310,   306,   307,   309,   310,    -1,   301,   302,
     303,   304,   305,    -1,   306,   307,   306,   307,   306,   307,
     306,   307,   306,   307,   306,   307,   342,   343,    -1,   342,
     343,   342,   343,   342,   343,    -1,   342,   343,    -1,    -1,
     342,   343,   335,   342,   343,   342,   343,   342,   343,    -1,
      -1,   342,   343,   342,   343,   342,   343,   342,   343,   342,
     343,   342,   343,   342,   343,   342,   343,   342,   343,   342,
     343,   342,   343,   342,   343,   342,   343,   342,   343,   342,
     343,   342,   343,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,
     360,   361,   362,   363,    -1,   438,   439,   440,   441,    -1,
      -1,   444,   445,   446,    -1,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   327,   465,   466,   467,   468,   327,    -1,   471,   472,
     473,   474,   475,    -1,    -1,    -1,    -1,   480,   481,   482,
      -1,    -1,    -1,    -1,   414,    -1,    -1,    -1,    -1,   419,
      -1,    -1,   422,    -1,    -1,   425,    -1,    -1,   428,    -1,
      -1,   431,    -1,    -1,    -1,    -1,   326,   437,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,    -1,   342,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   478,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   488,   489,
     476,    -1,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,    -1,   525,   526,   527,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   326,    -1,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,    -1,
     342,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   326,    -1,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,    -1,   342,
     343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   326,    -1,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,    -1,   342,   343,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   326,    -1,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,    -1,   342,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   326,    -1,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
      -1,   342,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   326,
      -1,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     326,    -1,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   326,    -1,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     142,   326,    -1,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,    -1,    -1,    -1,     0,
      -1,    -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,    -1,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,    99,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   327,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237
  };

  const short
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   142,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   345,   346,   347,   352,   306,   307,   143,   144,   306,
     307,   306,   307,   311,   305,   308,   312,   306,   307,   306,
     307,   306,   307,   309,   310,   306,   307,   309,   310,   306,
     307,   304,   306,   307,    99,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   327,   348,
     349,   357,   348,   306,   307,   303,   313,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   354,   355,   356,   354,   354,   348,   306,   307,
     311,   306,   307,   306,   307,   353,   354,   348,     0,   346,
     103,   356,   358,   304,   306,   307,   306,   307,   311,   306,
     307,   306,   307,   326,   356,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   349,     3,   356,   356,   342,   343,
     342,   343,   342,   343,   342,   343,   342,   343,   342,   343,
     342,   343,   342,   343,   342,   343,   342,   343,   342,   343,
     342,   343,   342,   343,   342,   343,   342,   343,   342,   343,
     342,   343,   342,   343,   342,   343,   342,   343,   342,   343,
     342,   343,   342,   343,   359,   342,   343,   342,   343,   359,
     342,   343,   359,   342,   343,   359,   342,   343,   359,   342,
     343,   359,   342,   343,   342,   343,   355,     6,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   350,   351,   359,   356,
     100,   101,   102,   326,   356,   327,   357,   327,     8,     9,
     356,   351,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   348,   359,   359,   359,   356,   356
  };

  const short
  seclang_parser::yyr1_[] =
  {
       0,   344,   345,   345,   345,   346,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   348,
     348,   349,   349,   350,   350,   350,   350,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   353,   354,   354,   355,   355,   355,
     355,   355,   355,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   358,   358,   358,   358,   358,   359,   359,
     359,   359
  };

  const signed char
  seclang_parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     2,     2,     3,
       1,     3,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     4,     3,     2,     2,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       2,     2,     1,     1,     1,     2,     2,     2,     2,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     4,     4,
       1,     2,     2,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     2,     1,     2,     2,     2,
       1,     2,     2,     2,     1,     2,     2,     2,     1,     2,
       2,     2,     1,     2,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     3,     3,     2,     2,
       1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\",\"",
  "\"CONFIG_CONTENT_INJECTION\"", "\"CONGIG_DIR_RESPONSE_BODY_MP_CLEAR\"",
  "PIPE", "NEW_LINE", "VAR_COUNT", "VAR_EXCLUSION", "VARIABLE_ARGS",
  "VARIABLE_ARGS_POST", "VARIABLE_ARGS_GET", "VARIABLE_FILES_SIZES",
  "VARIABLE_FILES_NAMES", "VARIABLE_FILES_TMP_CONTENT",
  "VARIABLE_MULTIPART_FILENAME", "VARIABLE_MULTIPART_NAME",
  "VARIABLE_MATCHED_VARS_NAMES", "VARIABLE_MATCHED_VARS", "VARIABLE_FILES",
  "VARIABLE_REQUEST_COOKIES", "VARIABLE_REQUEST_HEADERS",
  "VARIABLE_RESPONSE_HEADERS", "VARIABLE_GEO",
  "VARIABLE_REQUEST_COOKIES_NAMES", "VARIABLE_ARGS_COMBINED_SIZE",
  "VARIABLE_ARGS_GET_NAMES", "VARIABLE_RULE", "\"Variable ARGS_NAMES\"",
  "VARIABLE_ARGS_POST_NAMES", "\"AUTH_TYPE\"", "\"FILES_COMBINED_SIZE\"",
  "\"FILES_TMPNAMES\"", "\"FULL_REQUEST\"", "\"FULL_REQUEST_LENGTH\"",
  "\"INBOUND_DATA_ERROR\"", "\"MATCHED_VAR\"", "\"MATCHED_VAR_NAME\"",
  "VARIABLE_MULTIPART_BOUNDARY_QUOTED",
  "VARIABLE_MULTIPART_BOUNDARY_WHITESPACE", "\"MULTIPART_CRLF_LF_LINES\"",
  "\"MULTIPART_DATA_AFTER\"", "VARIABLE_MULTIPART_DATA_BEFORE",
  "\"MULTIPART_FILE_LIMIT_EXCEEDED\"", "\"MULTIPART_HEADER_FOLDING\"",
  "\"MULTIPART_INVALID_HEADER_FOLDING\"",
  "VARIABLE_MULTIPART_INVALID_PART", "\"MULTIPART_INVALID_QUOTING\"",
  "VARIABLE_MULTIPART_LF_LINE", "VARIABLE_MULTIPART_MISSING_SEMICOLON",
  "VARIABLE_MULTIPART_SEMICOLON_MISSING", "\"MULTIPART_STRICT_ERROR\"",
  "\"MULTIPART_UNMATCHED_BOUNDARY\"", "\"OUTBOUND_DATA_ERROR\"",
  "\"PATH_INFO\"", "\"QUERY_STRING\"", "\"REMOTE_ADDR\"",
  "\"REMOTE_HOST\"", "\"REMOTE_PORT\"", "\"REQBODY_ERROR_MSG\"",
  "\"REQBODY_ERROR\"", "\"REQBODY_PROCESSOR_ERROR_MSG\"",
  "\"REQBODY_PROCESSOR_ERROR\"", "\"REQBODY_PROCESSOR\"",
  "\"REQUEST_BASENAME\"", "\"REQUEST_BODY_LENGTH\"", "\"REQUEST_BODY\"",
  "\"REQUEST_FILENAME\"", "VARIABLE_REQUEST_HEADERS_NAMES",
  "\"REQUEST_LINE\"", "\"REQUEST_METHOD\"", "\"REQUEST_PROTOCOL\"",
  "\"REQUEST_URI_RAW\"", "\"REQUEST_URI\"", "\"RESOURCE\"",
  "\"RESPONSE_BODY\"", "\"RESPONSE_CONTENT_LENGTH\"",
  "VARIABLE_RESPONSE_CONTENT_TYPE", "VARIABLE_RESPONSE_HEADERS_NAMES",
  "\"RESPONSE_PROTOCOL\"", "\"RESPONSE_STATUS\"", "\"SERVER_ADDR\"",
  "\"SERVER_NAME\"", "\"SERVER_PORT\"", "\"SESSIONID\"", "\"UNIQUE_ID\"",
  "\"URLENCODED_ERROR\"", "\"USERID\"", "\"WEBAPPID\"",
  "\"VARIABLE_STATUS\"", "\"VARIABLE_STATUS_LINE\"", "\"VARIABLE_IP\"",
  "\"VARIABLE_GLOBAL\"", "\"VARIABLE_TX\"", "\"VARIABLE_SESSION\"",
  "\"VARIABLE_USER\"", "\"RUN_TIME_VAR_ENV\"", "\"RUN_TIME_VAR_XML\"",
  "\"SetVar\"", "SETVAR_OPERATION_EQUALS", "SETVAR_OPERATION_EQUALS_PLUS",
  "SETVAR_OPERATION_EQUALS_MINUS", "\"NOT\"", "\"OPERATOR_BEGINS_WITH\"",
  "\"OPERATOR_CONTAINS\"", "\"OPERATOR_CONTAINS_WORD\"",
  "\"OPERATOR_DETECT_SQLI\"", "\"OPERATOR_DETECT_XSS\"",
  "\"OPERATOR_ENDS_WITH\"", "\"OPERATOR_EQ\"", "\"OPERATOR_FUZZY_HASH\"",
  "\"OPERATOR_GEOLOOKUP\"", "\"OPERATOR_GE\"", "\"OPERATOR_GSB_LOOKUP\"",
  "\"OPERATOR_GT\"", "\"OPERATOR_INSPECT_FILE\"",
  "\"OPERATOR_IP_MATCH_FROM_FILE\"", "\"OPERATOR_IP_MATCH\"",
  "\"OPERATOR_LE\"", "\"OPERATOR_LT\"", "\"OPERATOR_PM_FROM_FILE\"",
  "\"OPERATOR_PM\"", "\"OPERATOR_RBL\"", "\"OPERATOR_RSUB\"",
  "\"Operator RX (content only)\"", "\"OPERATOR_RX\"",
  "\"OPERATOR_RX_GLOBAL\"", "\"OPERATOR_STR_EQ\"",
  "\"OPERATOR_STR_MATCH\"", "\"OPERATOR_UNCONDITIONAL_MATCH\"",
  "\"OPERATOR_VALIDATE_BYTE_RANGE\"", "\"OPERATOR_VALIDATE_DTD\"",
  "\"OPERATOR_VALIDATE_HASH\"", "\"OPERATOR_VALIDATE_SCHEMA\"",
  "\"OPERATOR_VALIDATE_URL_ENCODING\"",
  "\"OPERATOR_VALIDATE_UTF8_ENCODING\"", "\"OPERATOR_VERIFY_CC\"",
  "\"OPERATOR_VERIFY_CPF\"", "\"OPERATOR_VERIFY_SSN\"",
  "\"OPERATOR_VERIFY_SVNR\"", "\"OPERATOR_WITHIN\"",
  "CONFIG_DIR_AUDIT_LOG_FMT", "JSON", "NATIVE",
  "\"ACTION_CTL_RULE_ENGINE\"", "\"Accuracy\"", "\"Allow\"", "\"Append\"",
  "\"AuditLog\"", "\"Block\"", "\"Capture\"", "\"Chain\"",
  "\"ACTION_CTL_AUDIT_ENGINE\"", "\"ACTION_CTL_AUDIT_LOG_PARTS\"",
  "\"ACTION_CTL_BDY_JSON\"", "\"ACTION_CTL_BDY_XML\"",
  "\"ACTION_CTL_BDY_URLENCODED\"", "\"ACTION_CTL_FORCE_REQ_BODY_VAR\"",
  "\"ACTION_CTL_REQUEST_BODY_ACCESS\"", "\"ACTION_CTL_RULE_REMOVE_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_BY_TAG\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG\"", "\"Deny\"",
  "\"DeprecateVar\"", "\"Drop\"", "\"Exec\"", "\"ExpireVar\"", "\"Id\"",
  "\"InitCol\"", "\"Log\"", "\"LogData\"", "\"Maturity\"", "\"Msg\"",
  "\"MultiMatch\"", "\"NoAuditLog\"", "\"NoLog\"", "\"Pass\"", "\"Pause\"",
  "\"Phase\"", "\"Prepend\"", "\"Proxy\"", "\"Redirect\"", "\"Rev\"",
  "\"SanitiseArg\"", "\"SanitiseMatched\"", "\"SanitiseMatchedBytes\"",
  "\"SanitiseRequestHeader\"", "\"SanitiseResponseHeader\"", "\"SetEnv\"",
  "\"SetRsc\"", "\"SetSid\"", "\"SetUID\"", "\"Severity\"", "\"Skip\"",
  "\"SkipAfter\"", "\"Status\"", "\"Tag\"",
  "\"ACTION_TRANSFORMATION_BASE_64_ENCODE\"",
  "\"ACTION_TRANSFORMATION_BASE_64_DECODE\"",
  "\"ACTION_TRANSFORMATION_BASE_64_DECODE_EXT\"",
  "\"ACTION_TRANSFORMATION_CMD_LINE\"",
  "\"ACTION_TRANSFORMATION_COMPRESS_WHITESPACE\"",
  "\"ACTION_TRANSFORMATION_CSS_DECODE\"",
  "\"ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE\"",
  "\"ACTION_TRANSFORMATION_HEX_ENCODE\"",
  "\"ACTION_TRANSFORMATION_HEX_DECODE\"",
  "\"ACTION_TRANSFORMATION_HTML_ENTITY_DECODE\"",
  "\"ACTION_TRANSFORMATION_JS_DECODE\"",
  "\"ACTION_TRANSFORMATION_LENGTH\"",
  "\"ACTION_TRANSFORMATION_LOWERCASE\"",
  "\"ACTION_TRANSFORMATION_PHP_ARGS_NAMES\"",
  "\"ACTION_TRANSFORMATION_MD5\"", "\"ACTION_TRANSFORMATION_NONE\"",
  "\"ACTION_TRANSFORMATION_NORMALISE_PATH\"",
  "\"ACTION_TRANSFORMATION_NORMALISE_PATH_WIN\"",
  "\"ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT\"",
  "\"ACTION_TRANSFORMATION_PARITY_ODD_7_BIT\"",
  "\"ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT\"",
  "\"ACTION_TRANSFORMATION_REMOVE_COMMENTS\"",
  "\"ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR\"",
  "\"ACTION_TRANSFORMATION_REMOVE_NULLS\"",
  "\"ACTION_TRANSFORMATION_REMOVE_WHITESPACE\"",
  "\"ACTION_TRANSFORMATION_REPLACE_COMMENTS\"",
  "\"ACTION_TRANSFORMATION_REPLACE_NULLS\"",
  "\"ACTION_TRANSFORMATION_SHA1\"",
  "\"ACTION_TRANSFORMATION_SQL_HEX_DECODE\"",
  "\"ACTION_TRANSFORMATION_TRIM\"", "\"ACTION_TRANSFORMATION_TRIM_LEFT\"",
  "\"ACTION_TRANSFORMATION_TRIM_RIGHT\"",
  "\"ACTION_TRANSFORMATION_UPPERCASE\"",
  "\"ACTION_TRANSFORMATION_URL_ENCODE\"",
  "\"ACTION_TRANSFORMATION_URL_DECODE\"",
  "\"ACTION_TRANSFORMATION_URL_DECODE_UNI\"",
  "\"ACTION_TRANSFORMATION_UTF8_TO_UNICODE\"", "\"Ver\"", "\"xmlns\"",
  "\"CONFIG_COMPONENT_SIG\"", "\"CONFIG_CONN_ENGINE\"",
  "\"CONFIG_SEC_ARGUMENT_SEPARATOR\"", "\"CONFIG_SEC_WEB_APP_ID\"",
  "\"CONFIG_SEC_SERVER_SIG\"", "\"CONFIG_DIR_AUDIT_DIR\"",
  "\"CONFIG_DIR_AUDIT_DIR_MOD\"", "\"CONFIG_DIR_AUDIT_ENG\"",
  "\"CONFIG_DIR_AUDIT_FLE_MOD\"", "\"CONFIG_DIR_AUDIT_LOG\"",
  "\"CONFIG_DIR_AUDIT_LOG2\"", "\"CONFIG_DIR_AUDIT_LOG_P\"",
  "\"CONFIG_DIR_AUDIT_STS\"", "\"CONFIG_DIR_AUDIT_TPE\"",
  "\"CONFIG_DIR_DEBUG_LOG\"", "\"CONFIG_DIR_DEBUG_LVL\"",
  "\"CONFIG_SEC_CACHE_TRANSFORMATIONS\"",
  "\"CONFIG_SEC_DISABLE_BACKEND_COMPRESS\"", "\"CONFIG_SEC_HASH_ENGINE\"",
  "\"CONFIG_SEC_HASH_KEY\"", "\"CONFIG_SEC_HASH_PARAM\"",
  "\"CONFIG_SEC_HASH_METHOD_RX\"", "\"CONFIG_SEC_HASH_METHOD_PM\"",
  "\"CONFIG_SEC_CHROOT_DIR\"", "\"CONFIG_DIR_GEO_DB\"",
  "\"CONFIG_DIR_GSB_DB\"", "\"CONFIG_SEC_GUARDIAN_LOG\"",
  "\"CONFIG_DIR_PCRE_MATCH_LIMIT\"",
  "\"CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION\"",
  "\"CONFIG_SEC_CONN_R_STATE_LIMIT\"", "\"CONFIG_SEC_CONN_W_STATE_LIMIT\"",
  "\"CONFIG_SEC_SENSOR_ID\"", "\"CONFIG_DIR_ARGS_LIMIT\"",
  "\"CONFIG_DIR_REQ_BODY\"", "\"CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT\"",
  "\"CONFIG_DIR_REQ_BODY_LIMIT\"", "\"CONFIG_DIR_REQ_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT\"", "\"CONFIG_DIR_RES_BODY\"",
  "\"CONFIG_DIR_RES_BODY_LIMIT\"", "\"CONFIG_DIR_RES_BODY_LIMIT_ACTION\"",
  "\"CONFIG_SEC_RULE_INHERITANCE\"", "\"CONFIG_SEC_RULE_PERF_TIME\"",
  "\"CONFIG_DIR_RULE_ENG\"", "\"CONFIG_DIR_SEC_ACTION\"",
  "\"CONFIG_DIR_SEC_DEFAULT_ACTION\"", "\"CONFIG_DIR_SEC_MARKER\"",
  "\"CONFIG_DIR_UNICODE_MAP_FILE\"", "\"CONFIG_DIR_UNICODE_CODE_PAGE\"",
  "\"CONFIG_SEC_COLLECTION_TIMEOUT\"", "\"CONFIG_SEC_HTTP_BLKEY\"",
  "\"CONFIG_SEC_INTERCEPT_ON_ERROR\"",
  "\"CONFIG_SEC_REMOTE_RULES_FAIL_ACTION\"",
  "\"CONFIG_SEC_RULE_REMOVE_BY_ID\"", "\"CONFIG_SEC_RULE_REMOVE_BY_MSG\"",
  "\"CONFIG_SEC_RULE_REMOVE_BY_TAG\"",
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG\"",
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG\"",
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID\"",
  "\"CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID\"",
  "\"CONFIG_UPDLOAD_KEEP_FILES\"", "\"CONFIG_UPDLOAD_SAVE_TMP_FILES\"",
  "\"CONFIG_UPLOAD_DIR\"", "\"CONFIG_UPLOAD_FILE_LIMIT\"",
  "\"CONFIG_UPLOAD_FILE_MODE\"", "\"CONFIG_VALUE_ABORT\"",
  "\"CONFIG_VALUE_DETC\"", "\"CONFIG_VALUE_HTTPS\"",
  "\"CONFIG_VALUE_OFF\"", "\"CONFIG_VALUE_ON\"",
  "\"CONFIG_VALUE_PARALLEL\"", "\"CONFIG_VALUE_PROCESS_PARTIAL\"",
  "\"CONFIG_VALUE_REJECT\"", "\"CONFIG_VALUE_RELEVANT_ONLY\"",
  "\"CONFIG_VALUE_SERIAL\"", "\"CONFIG_VALUE_WARN\"",
  "\"CONFIG_XML_EXTERNAL_ENTITY\"", "\"CONGIG_DIR_RESPONSE_BODY_MP\"",
  "\"CONGIG_DIR_SEC_ARG_SEP\"", "\"CONGIG_DIR_SEC_COOKIE_FORMAT\"",
  "\"CONFIG_SEC_COOKIEV0_SEPARATOR\"", "\"CONGIG_DIR_SEC_DATA_DIR\"",
  "\"CONGIG_DIR_SEC_STATUS_ENGINE\"",
  "\"CONFIG_SEC_STREAM_IN_BODY_INSPECTION\"",
  "\"CONFIG_SEC_STREAM_OUT_BODY_INSPECTION\"",
  "\"CONGIG_DIR_SEC_TMP_DIR\"", "\"DIRECTIVE\"",
  "\"DIRECTIVE_SECRULESCRIPT\"", "\"FREE_TEXT_QUOTE_MACRO_EXPANSION\"",
  "\"QUOTATION_MARK\"", "\"RUN_TIME_VAR_BLD\"", "\"RUN_TIME_VAR_DUR\"",
  "\"RUN_TIME_VAR_HSV\"", "\"RUN_TIME_VAR_REMOTE_USER\"",
  "\"RUN_TIME_VAR_TIME\"", "\"RUN_TIME_VAR_TIME_DAY\"",
  "\"RUN_TIME_VAR_TIME_EPOCH\"", "\"RUN_TIME_VAR_TIME_HOUR\"",
  "\"RUN_TIME_VAR_TIME_MIN\"", "\"RUN_TIME_VAR_TIME_MON\"",
  "\"RUN_TIME_VAR_TIME_SEC\"", "\"RUN_TIME_VAR_TIME_WDAY\"",
  "\"RUN_TIME_VAR_TIME_YEAR\"", "\"VARIABLE\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "variables_pre_process",
  "variables_may_be_quoted", "var", "act", "setvar_action",
  "run_time_string", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  seclang_parser::yyrline_[] =
  {
       0,   712,   712,   716,   717,   720,   725,   731,   737,   741,
     745,   751,   757,   763,   769,   774,   779,   785,   792,   796,
     800,   806,   810,   814,   819,   824,   829,   834,   838,   845,
     849,   856,   862,   872,   881,   891,   900,   913,   917,   921,
     925,   929,   933,   937,   941,   945,   949,   954,   958,   962,
     966,   970,   974,   979,   984,   988,   992,   996,  1000,  1004,
    1008,  1012,  1016,  1020,  1024,  1028,  1032,  1036,  1040,  1044,
    1048,  1052,  1056,  1060,  1074,  1075,  1105,  1124,  1143,  1171,
    1228,  1235,  1239,  1243,  1247,  1251,  1255,  1259,  1263,  1272,
    1276,  1281,  1284,  1289,  1294,  1299,  1304,  1307,  1312,  1315,
    1320,  1325,  1328,  1333,  1338,  1343,  1348,  1353,  1358,  1363,
    1366,  1371,  1376,  1381,  1386,  1389,  1394,  1399,  1404,  1417,
    1430,  1443,  1456,  1469,  1495,  1523,  1535,  1555,  1582,  1588,
    1593,  1598,  1607,  1612,  1616,  1620,  1624,  1628,  1632,  1636,
    1641,  1646,  1658,  1664,  1668,  1672,  1683,  1692,  1693,  1700,
    1705,  1710,  1764,  1771,  1779,  1816,  1820,  1827,  1832,  1838,
    1844,  1850,  1857,  1867,  1871,  1875,  1879,  1883,  1887,  1891,
    1895,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1927,  1931,
    1935,  1939,  1943,  1947,  1951,  1955,  1959,  1963,  1967,  1971,
    1975,  1979,  1983,  1987,  1991,  1995,  1999,  2003,  2007,  2011,
    2015,  2019,  2023,  2027,  2031,  2035,  2039,  2043,  2047,  2051,
    2055,  2059,  2063,  2067,  2071,  2075,  2079,  2083,  2087,  2091,
    2095,  2099,  2103,  2107,  2111,  2115,  2119,  2123,  2127,  2131,
    2135,  2139,  2143,  2147,  2151,  2155,  2159,  2163,  2167,  2171,
    2175,  2179,  2183,  2187,  2191,  2195,  2199,  2203,  2207,  2211,
    2215,  2219,  2223,  2228,  2232,  2236,  2241,  2245,  2249,  2254,
    2259,  2263,  2267,  2271,  2275,  2279,  2283,  2287,  2291,  2295,
    2299,  2303,  2307,  2311,  2315,  2319,  2323,  2327,  2331,  2335,
    2339,  2343,  2347,  2351,  2355,  2359,  2363,  2367,  2371,  2375,
    2379,  2383,  2387,  2391,  2395,  2399,  2403,  2407,  2411,  2415,
    2419,  2423,  2427,  2431,  2435,  2439,  2443,  2447,  2451,  2455,
    2459,  2463,  2467,  2471,  2475,  2479,  2483,  2487,  2491,  2495,
    2499,  2507,  2514,  2521,  2528,  2535,  2542,  2549,  2556,  2563,
    2570,  2577,  2584,  2594,  2598,  2602,  2606,  2610,  2614,  2618,
    2622,  2627,  2632,  2637,  2641,  2645,  2649,  2653,  2658,  2663,
    2667,  2671,  2675,  2679,  2683,  2687,  2691,  2695,  2699,  2703,
    2707,  2711,  2715,  2720,  2724,  2728,  2732,  2736,  2740,  2744,
    2748,  2752,  2756,  2760,  2764,  2768,  2772,  2776,  2780,  2784,
    2788,  2792,  2796,  2800,  2804,  2808,  2812,  2816,  2820,  2824,
    2828,  2832,  2836,  2840,  2844,  2848,  2852,  2856,  2860,  2864,
    2868,  2872,  2876,  2880,  2884,  2888,  2892,  2896,  2900,  2904,
    2908,  2912,  2916,  2920,  2924,  2928,  2932,  2936,  2940,  2944,
    2948,  2952,  2956,  2960,  2964,  2968,  2972,  2976,  2980,  2984,
    2988,  2992,  2996,  3003,  3007,  3011,  3015,  3019,  3026,  3031,
    3036,  3042
  };

  void
  seclang_parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  seclang_parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 7227 "seclang-parser.cc"

#line 3049 "seclang-parser.yy"


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
