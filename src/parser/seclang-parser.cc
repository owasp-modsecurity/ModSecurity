// A Bison parser, made by GNU Bison 3.5.3.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "seclang-parser.hh"


// Unqualified %code blocks.
#line 324 "seclang-parser.yy"

#include "src/parser/driver.h"

#line 49 "seclang-parser.cc"


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
      yystack_print_ ();                \
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
#line 140 "seclang-parser.cc"


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
  | Symbol types.  |
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

  seclang_parser::symbol_number_type
  seclang_parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  seclang_parser::stack_symbol_type::stack_symbol_type ()
  {}

  seclang_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 145: // "Accuracy"
      case 146: // "Allow"
      case 147: // "Append"
      case 148: // "AuditLog"
      case 149: // "Block"
      case 150: // "Capture"
      case 151: // "Chain"
      case 152: // "ACTION_CTL_AUDIT_ENGINE"
      case 153: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 154: // "ACTION_CTL_BDY_JSON"
      case 155: // "ACTION_CTL_BDY_XML"
      case 156: // "ACTION_CTL_BDY_URLENCODED"
      case 157: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 158: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 160: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 162: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 163: // "Deny"
      case 164: // "DeprecateVar"
      case 165: // "Drop"
      case 166: // "Exec"
      case 167: // "ExpireVar"
      case 168: // "Id"
      case 169: // "InitCol"
      case 170: // "Log"
      case 171: // "LogData"
      case 172: // "Maturity"
      case 173: // "Msg"
      case 174: // "MultiMatch"
      case 175: // "NoAuditLog"
      case 176: // "NoLog"
      case 177: // "Pass"
      case 178: // "Pause"
      case 179: // "Phase"
      case 180: // "Prepend"
      case 181: // "Proxy"
      case 182: // "Redirect"
      case 183: // "Rev"
      case 184: // "SanitiseArg"
      case 185: // "SanitiseMatched"
      case 186: // "SanitiseMatchedBytes"
      case 187: // "SanitiseRequestHeader"
      case 188: // "SanitiseResponseHeader"
      case 189: // "SetEnv"
      case 190: // "SetRsc"
      case 191: // "SetSid"
      case 192: // "SetUID"
      case 193: // "Severity"
      case 194: // "Skip"
      case 195: // "SkipAfter"
      case 196: // "Status"
      case 197: // "Tag"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 200: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 201: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 202: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 203: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 206: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 207: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 208: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 209: // "ACTION_TRANSFORMATION_LENGTH"
      case 210: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 211: // "ACTION_TRANSFORMATION_MD5"
      case 212: // "ACTION_TRANSFORMATION_NONE"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 214: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 215: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 221: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 223: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 224: // "ACTION_TRANSFORMATION_SHA1"
      case 225: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 226: // "ACTION_TRANSFORMATION_TRIM"
      case 227: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 228: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 229: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 230: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 232: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 233: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 234: // "Ver"
      case 235: // "xmlns"
      case 236: // "CONFIG_COMPONENT_SIG"
      case 237: // "CONFIG_CONN_ENGINE"
      case 238: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 239: // "CONFIG_SEC_WEB_APP_ID"
      case 240: // "CONFIG_SEC_SERVER_SIG"
      case 241: // "CONFIG_DIR_AUDIT_DIR"
      case 242: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 243: // "CONFIG_DIR_AUDIT_ENG"
      case 244: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 245: // "CONFIG_DIR_AUDIT_LOG"
      case 246: // "CONFIG_DIR_AUDIT_LOG2"
      case 247: // "CONFIG_DIR_AUDIT_LOG_P"
      case 248: // "CONFIG_DIR_AUDIT_STS"
      case 249: // "CONFIG_DIR_AUDIT_TPE"
      case 250: // "CONFIG_DIR_DEBUG_LOG"
      case 251: // "CONFIG_DIR_DEBUG_LVL"
      case 252: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 253: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 254: // "CONFIG_SEC_HASH_ENGINE"
      case 255: // "CONFIG_SEC_HASH_KEY"
      case 256: // "CONFIG_SEC_HASH_PARAM"
      case 257: // "CONFIG_SEC_HASH_METHOD_RX"
      case 258: // "CONFIG_SEC_HASH_METHOD_PM"
      case 259: // "CONFIG_SEC_CHROOT_DIR"
      case 260: // "CONFIG_DIR_GEO_DB"
      case 261: // "CONFIG_DIR_GSB_DB"
      case 262: // "CONFIG_SEC_GUARDIAN_LOG"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 264: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 265: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 266: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 267: // "CONFIG_SEC_SENSOR_ID"
      case 268: // "CONFIG_DIR_ARGS_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY"
      case 270: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 271: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 272: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 273: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY"
      case 275: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 276: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 277: // "CONFIG_SEC_RULE_INHERITANCE"
      case 278: // "CONFIG_SEC_RULE_PERF_TIME"
      case 279: // "CONFIG_DIR_RULE_ENG"
      case 280: // "CONFIG_DIR_SEC_ACTION"
      case 281: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 282: // "CONFIG_DIR_SEC_MARKER"
      case 283: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 284: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 285: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 286: // "CONFIG_SEC_HTTP_BLKEY"
      case 287: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 288: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 290: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 291: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 293: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 294: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 295: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 296: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 297: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 298: // "CONFIG_UPLOAD_DIR"
      case 299: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 300: // "CONFIG_UPLOAD_FILE_MODE"
      case 301: // "CONFIG_VALUE_ABORT"
      case 302: // "CONFIG_VALUE_DETC"
      case 303: // "CONFIG_VALUE_HTTPS"
      case 304: // "CONFIG_VALUE_OFF"
      case 305: // "CONFIG_VALUE_ON"
      case 306: // "CONFIG_VALUE_PARALLEL"
      case 307: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 308: // "CONFIG_VALUE_REJECT"
      case 309: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 310: // "CONFIG_VALUE_SERIAL"
      case 311: // "CONFIG_VALUE_WARN"
      case 312: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 313: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 314: // "CONGIG_DIR_SEC_ARG_SEP"
      case 315: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 316: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 317: // "CONGIG_DIR_SEC_DATA_DIR"
      case 318: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 319: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 320: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 321: // "CONGIG_DIR_SEC_TMP_DIR"
      case 322: // "DIRECTIVE"
      case 323: // "DIRECTIVE_SECRULESCRIPT"
      case 324: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 325: // "QUOTATION_MARK"
      case 326: // "RUN_TIME_VAR_BLD"
      case 327: // "RUN_TIME_VAR_DUR"
      case 328: // "RUN_TIME_VAR_HSV"
      case 329: // "RUN_TIME_VAR_REMOTE_USER"
      case 330: // "RUN_TIME_VAR_TIME"
      case 331: // "RUN_TIME_VAR_TIME_DAY"
      case 332: // "RUN_TIME_VAR_TIME_EPOCH"
      case 333: // "RUN_TIME_VAR_TIME_HOUR"
      case 334: // "RUN_TIME_VAR_TIME_MIN"
      case 335: // "RUN_TIME_VAR_TIME_MON"
      case 336: // "RUN_TIME_VAR_TIME_SEC"
      case 337: // "RUN_TIME_VAR_TIME_WDAY"
      case 338: // "RUN_TIME_VAR_TIME_YEAR"
      case 339: // "VARIABLE"
      case 340: // "Dictionary element"
      case 341: // "Dictionary element, selected by regexp"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 348: // op
      case 349: // op_before_init
        value.YY_MOVE_OR_COPY< std::unique_ptr<Operator> > (YY_MOVE (that.value));
        break;

      case 357: // run_time_string
        value.YY_MOVE_OR_COPY< std::unique_ptr<RunTimeString> > (YY_MOVE (that.value));
        break;

      case 354: // var
        value.YY_MOVE_OR_COPY< std::unique_ptr<Variable> > (YY_MOVE (that.value));
        break;

      case 355: // act
      case 356: // setvar_action
        value.YY_MOVE_OR_COPY< std::unique_ptr<actions::Action> > (YY_MOVE (that.value));
        break;

      case 351: // variables
      case 352: // variables_pre_process
      case 353: // variables_may_be_quoted
        value.YY_MOVE_OR_COPY< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (YY_MOVE (that.value));
        break;

      case 346: // actions
      case 347: // actions_may_quoted
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
    switch (that.type_get ())
    {
      case 145: // "Accuracy"
      case 146: // "Allow"
      case 147: // "Append"
      case 148: // "AuditLog"
      case 149: // "Block"
      case 150: // "Capture"
      case 151: // "Chain"
      case 152: // "ACTION_CTL_AUDIT_ENGINE"
      case 153: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 154: // "ACTION_CTL_BDY_JSON"
      case 155: // "ACTION_CTL_BDY_XML"
      case 156: // "ACTION_CTL_BDY_URLENCODED"
      case 157: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 158: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 160: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 162: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 163: // "Deny"
      case 164: // "DeprecateVar"
      case 165: // "Drop"
      case 166: // "Exec"
      case 167: // "ExpireVar"
      case 168: // "Id"
      case 169: // "InitCol"
      case 170: // "Log"
      case 171: // "LogData"
      case 172: // "Maturity"
      case 173: // "Msg"
      case 174: // "MultiMatch"
      case 175: // "NoAuditLog"
      case 176: // "NoLog"
      case 177: // "Pass"
      case 178: // "Pause"
      case 179: // "Phase"
      case 180: // "Prepend"
      case 181: // "Proxy"
      case 182: // "Redirect"
      case 183: // "Rev"
      case 184: // "SanitiseArg"
      case 185: // "SanitiseMatched"
      case 186: // "SanitiseMatchedBytes"
      case 187: // "SanitiseRequestHeader"
      case 188: // "SanitiseResponseHeader"
      case 189: // "SetEnv"
      case 190: // "SetRsc"
      case 191: // "SetSid"
      case 192: // "SetUID"
      case 193: // "Severity"
      case 194: // "Skip"
      case 195: // "SkipAfter"
      case 196: // "Status"
      case 197: // "Tag"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 200: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 201: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 202: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 203: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 206: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 207: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 208: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 209: // "ACTION_TRANSFORMATION_LENGTH"
      case 210: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 211: // "ACTION_TRANSFORMATION_MD5"
      case 212: // "ACTION_TRANSFORMATION_NONE"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 214: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 215: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 221: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 223: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 224: // "ACTION_TRANSFORMATION_SHA1"
      case 225: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 226: // "ACTION_TRANSFORMATION_TRIM"
      case 227: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 228: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 229: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 230: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 232: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 233: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 234: // "Ver"
      case 235: // "xmlns"
      case 236: // "CONFIG_COMPONENT_SIG"
      case 237: // "CONFIG_CONN_ENGINE"
      case 238: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 239: // "CONFIG_SEC_WEB_APP_ID"
      case 240: // "CONFIG_SEC_SERVER_SIG"
      case 241: // "CONFIG_DIR_AUDIT_DIR"
      case 242: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 243: // "CONFIG_DIR_AUDIT_ENG"
      case 244: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 245: // "CONFIG_DIR_AUDIT_LOG"
      case 246: // "CONFIG_DIR_AUDIT_LOG2"
      case 247: // "CONFIG_DIR_AUDIT_LOG_P"
      case 248: // "CONFIG_DIR_AUDIT_STS"
      case 249: // "CONFIG_DIR_AUDIT_TPE"
      case 250: // "CONFIG_DIR_DEBUG_LOG"
      case 251: // "CONFIG_DIR_DEBUG_LVL"
      case 252: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 253: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 254: // "CONFIG_SEC_HASH_ENGINE"
      case 255: // "CONFIG_SEC_HASH_KEY"
      case 256: // "CONFIG_SEC_HASH_PARAM"
      case 257: // "CONFIG_SEC_HASH_METHOD_RX"
      case 258: // "CONFIG_SEC_HASH_METHOD_PM"
      case 259: // "CONFIG_SEC_CHROOT_DIR"
      case 260: // "CONFIG_DIR_GEO_DB"
      case 261: // "CONFIG_DIR_GSB_DB"
      case 262: // "CONFIG_SEC_GUARDIAN_LOG"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 264: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 265: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 266: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 267: // "CONFIG_SEC_SENSOR_ID"
      case 268: // "CONFIG_DIR_ARGS_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY"
      case 270: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 271: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 272: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 273: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY"
      case 275: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 276: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 277: // "CONFIG_SEC_RULE_INHERITANCE"
      case 278: // "CONFIG_SEC_RULE_PERF_TIME"
      case 279: // "CONFIG_DIR_RULE_ENG"
      case 280: // "CONFIG_DIR_SEC_ACTION"
      case 281: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 282: // "CONFIG_DIR_SEC_MARKER"
      case 283: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 284: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 285: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 286: // "CONFIG_SEC_HTTP_BLKEY"
      case 287: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 288: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 290: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 291: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 293: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 294: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 295: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 296: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 297: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 298: // "CONFIG_UPLOAD_DIR"
      case 299: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 300: // "CONFIG_UPLOAD_FILE_MODE"
      case 301: // "CONFIG_VALUE_ABORT"
      case 302: // "CONFIG_VALUE_DETC"
      case 303: // "CONFIG_VALUE_HTTPS"
      case 304: // "CONFIG_VALUE_OFF"
      case 305: // "CONFIG_VALUE_ON"
      case 306: // "CONFIG_VALUE_PARALLEL"
      case 307: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 308: // "CONFIG_VALUE_REJECT"
      case 309: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 310: // "CONFIG_VALUE_SERIAL"
      case 311: // "CONFIG_VALUE_WARN"
      case 312: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 313: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 314: // "CONGIG_DIR_SEC_ARG_SEP"
      case 315: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 316: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 317: // "CONGIG_DIR_SEC_DATA_DIR"
      case 318: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 319: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 320: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 321: // "CONGIG_DIR_SEC_TMP_DIR"
      case 322: // "DIRECTIVE"
      case 323: // "DIRECTIVE_SECRULESCRIPT"
      case 324: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 325: // "QUOTATION_MARK"
      case 326: // "RUN_TIME_VAR_BLD"
      case 327: // "RUN_TIME_VAR_DUR"
      case 328: // "RUN_TIME_VAR_HSV"
      case 329: // "RUN_TIME_VAR_REMOTE_USER"
      case 330: // "RUN_TIME_VAR_TIME"
      case 331: // "RUN_TIME_VAR_TIME_DAY"
      case 332: // "RUN_TIME_VAR_TIME_EPOCH"
      case 333: // "RUN_TIME_VAR_TIME_HOUR"
      case 334: // "RUN_TIME_VAR_TIME_MIN"
      case 335: // "RUN_TIME_VAR_TIME_MON"
      case 336: // "RUN_TIME_VAR_TIME_SEC"
      case 337: // "RUN_TIME_VAR_TIME_WDAY"
      case 338: // "RUN_TIME_VAR_TIME_YEAR"
      case 339: // "VARIABLE"
      case 340: // "Dictionary element"
      case 341: // "Dictionary element, selected by regexp"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 348: // op
      case 349: // op_before_init
        value.move< std::unique_ptr<Operator> > (YY_MOVE (that.value));
        break;

      case 357: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (YY_MOVE (that.value));
        break;

      case 354: // var
        value.move< std::unique_ptr<Variable> > (YY_MOVE (that.value));
        break;

      case 355: // act
      case 356: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (YY_MOVE (that.value));
        break;

      case 351: // variables
      case 352: // variables_pre_process
      case 353: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (YY_MOVE (that.value));
        break;

      case 346: // actions
      case 347: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  seclang_parser::stack_symbol_type&
  seclang_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 145: // "Accuracy"
      case 146: // "Allow"
      case 147: // "Append"
      case 148: // "AuditLog"
      case 149: // "Block"
      case 150: // "Capture"
      case 151: // "Chain"
      case 152: // "ACTION_CTL_AUDIT_ENGINE"
      case 153: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 154: // "ACTION_CTL_BDY_JSON"
      case 155: // "ACTION_CTL_BDY_XML"
      case 156: // "ACTION_CTL_BDY_URLENCODED"
      case 157: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 158: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 160: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 162: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 163: // "Deny"
      case 164: // "DeprecateVar"
      case 165: // "Drop"
      case 166: // "Exec"
      case 167: // "ExpireVar"
      case 168: // "Id"
      case 169: // "InitCol"
      case 170: // "Log"
      case 171: // "LogData"
      case 172: // "Maturity"
      case 173: // "Msg"
      case 174: // "MultiMatch"
      case 175: // "NoAuditLog"
      case 176: // "NoLog"
      case 177: // "Pass"
      case 178: // "Pause"
      case 179: // "Phase"
      case 180: // "Prepend"
      case 181: // "Proxy"
      case 182: // "Redirect"
      case 183: // "Rev"
      case 184: // "SanitiseArg"
      case 185: // "SanitiseMatched"
      case 186: // "SanitiseMatchedBytes"
      case 187: // "SanitiseRequestHeader"
      case 188: // "SanitiseResponseHeader"
      case 189: // "SetEnv"
      case 190: // "SetRsc"
      case 191: // "SetSid"
      case 192: // "SetUID"
      case 193: // "Severity"
      case 194: // "Skip"
      case 195: // "SkipAfter"
      case 196: // "Status"
      case 197: // "Tag"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 200: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 201: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 202: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 203: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 206: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 207: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 208: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 209: // "ACTION_TRANSFORMATION_LENGTH"
      case 210: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 211: // "ACTION_TRANSFORMATION_MD5"
      case 212: // "ACTION_TRANSFORMATION_NONE"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 214: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 215: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 221: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 223: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 224: // "ACTION_TRANSFORMATION_SHA1"
      case 225: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 226: // "ACTION_TRANSFORMATION_TRIM"
      case 227: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 228: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 229: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 230: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 232: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 233: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 234: // "Ver"
      case 235: // "xmlns"
      case 236: // "CONFIG_COMPONENT_SIG"
      case 237: // "CONFIG_CONN_ENGINE"
      case 238: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 239: // "CONFIG_SEC_WEB_APP_ID"
      case 240: // "CONFIG_SEC_SERVER_SIG"
      case 241: // "CONFIG_DIR_AUDIT_DIR"
      case 242: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 243: // "CONFIG_DIR_AUDIT_ENG"
      case 244: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 245: // "CONFIG_DIR_AUDIT_LOG"
      case 246: // "CONFIG_DIR_AUDIT_LOG2"
      case 247: // "CONFIG_DIR_AUDIT_LOG_P"
      case 248: // "CONFIG_DIR_AUDIT_STS"
      case 249: // "CONFIG_DIR_AUDIT_TPE"
      case 250: // "CONFIG_DIR_DEBUG_LOG"
      case 251: // "CONFIG_DIR_DEBUG_LVL"
      case 252: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 253: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 254: // "CONFIG_SEC_HASH_ENGINE"
      case 255: // "CONFIG_SEC_HASH_KEY"
      case 256: // "CONFIG_SEC_HASH_PARAM"
      case 257: // "CONFIG_SEC_HASH_METHOD_RX"
      case 258: // "CONFIG_SEC_HASH_METHOD_PM"
      case 259: // "CONFIG_SEC_CHROOT_DIR"
      case 260: // "CONFIG_DIR_GEO_DB"
      case 261: // "CONFIG_DIR_GSB_DB"
      case 262: // "CONFIG_SEC_GUARDIAN_LOG"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 264: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 265: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 266: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 267: // "CONFIG_SEC_SENSOR_ID"
      case 268: // "CONFIG_DIR_ARGS_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY"
      case 270: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 271: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 272: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 273: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY"
      case 275: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 276: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 277: // "CONFIG_SEC_RULE_INHERITANCE"
      case 278: // "CONFIG_SEC_RULE_PERF_TIME"
      case 279: // "CONFIG_DIR_RULE_ENG"
      case 280: // "CONFIG_DIR_SEC_ACTION"
      case 281: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 282: // "CONFIG_DIR_SEC_MARKER"
      case 283: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 284: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 285: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 286: // "CONFIG_SEC_HTTP_BLKEY"
      case 287: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 288: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 290: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 291: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 293: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 294: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 295: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 296: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 297: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 298: // "CONFIG_UPLOAD_DIR"
      case 299: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 300: // "CONFIG_UPLOAD_FILE_MODE"
      case 301: // "CONFIG_VALUE_ABORT"
      case 302: // "CONFIG_VALUE_DETC"
      case 303: // "CONFIG_VALUE_HTTPS"
      case 304: // "CONFIG_VALUE_OFF"
      case 305: // "CONFIG_VALUE_ON"
      case 306: // "CONFIG_VALUE_PARALLEL"
      case 307: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 308: // "CONFIG_VALUE_REJECT"
      case 309: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 310: // "CONFIG_VALUE_SERIAL"
      case 311: // "CONFIG_VALUE_WARN"
      case 312: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 313: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 314: // "CONGIG_DIR_SEC_ARG_SEP"
      case 315: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 316: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 317: // "CONGIG_DIR_SEC_DATA_DIR"
      case 318: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 319: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 320: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 321: // "CONGIG_DIR_SEC_TMP_DIR"
      case 322: // "DIRECTIVE"
      case 323: // "DIRECTIVE_SECRULESCRIPT"
      case 324: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 325: // "QUOTATION_MARK"
      case 326: // "RUN_TIME_VAR_BLD"
      case 327: // "RUN_TIME_VAR_DUR"
      case 328: // "RUN_TIME_VAR_HSV"
      case 329: // "RUN_TIME_VAR_REMOTE_USER"
      case 330: // "RUN_TIME_VAR_TIME"
      case 331: // "RUN_TIME_VAR_TIME_DAY"
      case 332: // "RUN_TIME_VAR_TIME_EPOCH"
      case 333: // "RUN_TIME_VAR_TIME_HOUR"
      case 334: // "RUN_TIME_VAR_TIME_MIN"
      case 335: // "RUN_TIME_VAR_TIME_MON"
      case 336: // "RUN_TIME_VAR_TIME_SEC"
      case 337: // "RUN_TIME_VAR_TIME_WDAY"
      case 338: // "RUN_TIME_VAR_TIME_YEAR"
      case 339: // "VARIABLE"
      case 340: // "Dictionary element"
      case 341: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 348: // op
      case 349: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 357: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 354: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 355: // act
      case 356: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 351: // variables
      case 352: // variables_pre_process
      case 353: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 346: // actions
      case 347: // actions_may_quoted
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
    switch (that.type_get ())
    {
      case 145: // "Accuracy"
      case 146: // "Allow"
      case 147: // "Append"
      case 148: // "AuditLog"
      case 149: // "Block"
      case 150: // "Capture"
      case 151: // "Chain"
      case 152: // "ACTION_CTL_AUDIT_ENGINE"
      case 153: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 154: // "ACTION_CTL_BDY_JSON"
      case 155: // "ACTION_CTL_BDY_XML"
      case 156: // "ACTION_CTL_BDY_URLENCODED"
      case 157: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 158: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 160: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 162: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 163: // "Deny"
      case 164: // "DeprecateVar"
      case 165: // "Drop"
      case 166: // "Exec"
      case 167: // "ExpireVar"
      case 168: // "Id"
      case 169: // "InitCol"
      case 170: // "Log"
      case 171: // "LogData"
      case 172: // "Maturity"
      case 173: // "Msg"
      case 174: // "MultiMatch"
      case 175: // "NoAuditLog"
      case 176: // "NoLog"
      case 177: // "Pass"
      case 178: // "Pause"
      case 179: // "Phase"
      case 180: // "Prepend"
      case 181: // "Proxy"
      case 182: // "Redirect"
      case 183: // "Rev"
      case 184: // "SanitiseArg"
      case 185: // "SanitiseMatched"
      case 186: // "SanitiseMatchedBytes"
      case 187: // "SanitiseRequestHeader"
      case 188: // "SanitiseResponseHeader"
      case 189: // "SetEnv"
      case 190: // "SetRsc"
      case 191: // "SetSid"
      case 192: // "SetUID"
      case 193: // "Severity"
      case 194: // "Skip"
      case 195: // "SkipAfter"
      case 196: // "Status"
      case 197: // "Tag"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 200: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 201: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 202: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 203: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 206: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 207: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 208: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 209: // "ACTION_TRANSFORMATION_LENGTH"
      case 210: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 211: // "ACTION_TRANSFORMATION_MD5"
      case 212: // "ACTION_TRANSFORMATION_NONE"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 214: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 215: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 221: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 223: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 224: // "ACTION_TRANSFORMATION_SHA1"
      case 225: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 226: // "ACTION_TRANSFORMATION_TRIM"
      case 227: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 228: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 229: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 230: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 232: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 233: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 234: // "Ver"
      case 235: // "xmlns"
      case 236: // "CONFIG_COMPONENT_SIG"
      case 237: // "CONFIG_CONN_ENGINE"
      case 238: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 239: // "CONFIG_SEC_WEB_APP_ID"
      case 240: // "CONFIG_SEC_SERVER_SIG"
      case 241: // "CONFIG_DIR_AUDIT_DIR"
      case 242: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 243: // "CONFIG_DIR_AUDIT_ENG"
      case 244: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 245: // "CONFIG_DIR_AUDIT_LOG"
      case 246: // "CONFIG_DIR_AUDIT_LOG2"
      case 247: // "CONFIG_DIR_AUDIT_LOG_P"
      case 248: // "CONFIG_DIR_AUDIT_STS"
      case 249: // "CONFIG_DIR_AUDIT_TPE"
      case 250: // "CONFIG_DIR_DEBUG_LOG"
      case 251: // "CONFIG_DIR_DEBUG_LVL"
      case 252: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 253: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 254: // "CONFIG_SEC_HASH_ENGINE"
      case 255: // "CONFIG_SEC_HASH_KEY"
      case 256: // "CONFIG_SEC_HASH_PARAM"
      case 257: // "CONFIG_SEC_HASH_METHOD_RX"
      case 258: // "CONFIG_SEC_HASH_METHOD_PM"
      case 259: // "CONFIG_SEC_CHROOT_DIR"
      case 260: // "CONFIG_DIR_GEO_DB"
      case 261: // "CONFIG_DIR_GSB_DB"
      case 262: // "CONFIG_SEC_GUARDIAN_LOG"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 264: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 265: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 266: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 267: // "CONFIG_SEC_SENSOR_ID"
      case 268: // "CONFIG_DIR_ARGS_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY"
      case 270: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 271: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 272: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 273: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY"
      case 275: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 276: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 277: // "CONFIG_SEC_RULE_INHERITANCE"
      case 278: // "CONFIG_SEC_RULE_PERF_TIME"
      case 279: // "CONFIG_DIR_RULE_ENG"
      case 280: // "CONFIG_DIR_SEC_ACTION"
      case 281: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 282: // "CONFIG_DIR_SEC_MARKER"
      case 283: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 284: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 285: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 286: // "CONFIG_SEC_HTTP_BLKEY"
      case 287: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 288: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 290: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 291: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 293: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 294: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 295: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 296: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 297: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 298: // "CONFIG_UPLOAD_DIR"
      case 299: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 300: // "CONFIG_UPLOAD_FILE_MODE"
      case 301: // "CONFIG_VALUE_ABORT"
      case 302: // "CONFIG_VALUE_DETC"
      case 303: // "CONFIG_VALUE_HTTPS"
      case 304: // "CONFIG_VALUE_OFF"
      case 305: // "CONFIG_VALUE_ON"
      case 306: // "CONFIG_VALUE_PARALLEL"
      case 307: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 308: // "CONFIG_VALUE_REJECT"
      case 309: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 310: // "CONFIG_VALUE_SERIAL"
      case 311: // "CONFIG_VALUE_WARN"
      case 312: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 313: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 314: // "CONGIG_DIR_SEC_ARG_SEP"
      case 315: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 316: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 317: // "CONGIG_DIR_SEC_DATA_DIR"
      case 318: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 319: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 320: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 321: // "CONGIG_DIR_SEC_TMP_DIR"
      case 322: // "DIRECTIVE"
      case 323: // "DIRECTIVE_SECRULESCRIPT"
      case 324: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 325: // "QUOTATION_MARK"
      case 326: // "RUN_TIME_VAR_BLD"
      case 327: // "RUN_TIME_VAR_DUR"
      case 328: // "RUN_TIME_VAR_HSV"
      case 329: // "RUN_TIME_VAR_REMOTE_USER"
      case 330: // "RUN_TIME_VAR_TIME"
      case 331: // "RUN_TIME_VAR_TIME_DAY"
      case 332: // "RUN_TIME_VAR_TIME_EPOCH"
      case 333: // "RUN_TIME_VAR_TIME_HOUR"
      case 334: // "RUN_TIME_VAR_TIME_MIN"
      case 335: // "RUN_TIME_VAR_TIME_MON"
      case 336: // "RUN_TIME_VAR_TIME_SEC"
      case 337: // "RUN_TIME_VAR_TIME_WDAY"
      case 338: // "RUN_TIME_VAR_TIME_YEAR"
      case 339: // "VARIABLE"
      case 340: // "Dictionary element"
      case 341: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 348: // op
      case 349: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 357: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 354: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 355: // act
      case 356: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 351: // variables
      case 352: // variables_pre_process
      case 353: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 346: // actions
      case 347: // actions_may_quoted
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
  seclang_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
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
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
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
#line 317 "seclang-parser.yy"
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = new std::string(driver.file);
}

#line 1366 "seclang-parser.cc"


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
        YYCDEBUG << "Reading a token: ";
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

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
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
      case 145: // "Accuracy"
      case 146: // "Allow"
      case 147: // "Append"
      case 148: // "AuditLog"
      case 149: // "Block"
      case 150: // "Capture"
      case 151: // "Chain"
      case 152: // "ACTION_CTL_AUDIT_ENGINE"
      case 153: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 154: // "ACTION_CTL_BDY_JSON"
      case 155: // "ACTION_CTL_BDY_XML"
      case 156: // "ACTION_CTL_BDY_URLENCODED"
      case 157: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 158: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 160: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 162: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 163: // "Deny"
      case 164: // "DeprecateVar"
      case 165: // "Drop"
      case 166: // "Exec"
      case 167: // "ExpireVar"
      case 168: // "Id"
      case 169: // "InitCol"
      case 170: // "Log"
      case 171: // "LogData"
      case 172: // "Maturity"
      case 173: // "Msg"
      case 174: // "MultiMatch"
      case 175: // "NoAuditLog"
      case 176: // "NoLog"
      case 177: // "Pass"
      case 178: // "Pause"
      case 179: // "Phase"
      case 180: // "Prepend"
      case 181: // "Proxy"
      case 182: // "Redirect"
      case 183: // "Rev"
      case 184: // "SanitiseArg"
      case 185: // "SanitiseMatched"
      case 186: // "SanitiseMatchedBytes"
      case 187: // "SanitiseRequestHeader"
      case 188: // "SanitiseResponseHeader"
      case 189: // "SetEnv"
      case 190: // "SetRsc"
      case 191: // "SetSid"
      case 192: // "SetUID"
      case 193: // "Severity"
      case 194: // "Skip"
      case 195: // "SkipAfter"
      case 196: // "Status"
      case 197: // "Tag"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 200: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 201: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 202: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 203: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 206: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 207: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 208: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 209: // "ACTION_TRANSFORMATION_LENGTH"
      case 210: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 211: // "ACTION_TRANSFORMATION_MD5"
      case 212: // "ACTION_TRANSFORMATION_NONE"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 214: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 215: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 221: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 223: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 224: // "ACTION_TRANSFORMATION_SHA1"
      case 225: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 226: // "ACTION_TRANSFORMATION_TRIM"
      case 227: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 228: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 229: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 230: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 232: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 233: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 234: // "Ver"
      case 235: // "xmlns"
      case 236: // "CONFIG_COMPONENT_SIG"
      case 237: // "CONFIG_CONN_ENGINE"
      case 238: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 239: // "CONFIG_SEC_WEB_APP_ID"
      case 240: // "CONFIG_SEC_SERVER_SIG"
      case 241: // "CONFIG_DIR_AUDIT_DIR"
      case 242: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 243: // "CONFIG_DIR_AUDIT_ENG"
      case 244: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 245: // "CONFIG_DIR_AUDIT_LOG"
      case 246: // "CONFIG_DIR_AUDIT_LOG2"
      case 247: // "CONFIG_DIR_AUDIT_LOG_P"
      case 248: // "CONFIG_DIR_AUDIT_STS"
      case 249: // "CONFIG_DIR_AUDIT_TPE"
      case 250: // "CONFIG_DIR_DEBUG_LOG"
      case 251: // "CONFIG_DIR_DEBUG_LVL"
      case 252: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 253: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 254: // "CONFIG_SEC_HASH_ENGINE"
      case 255: // "CONFIG_SEC_HASH_KEY"
      case 256: // "CONFIG_SEC_HASH_PARAM"
      case 257: // "CONFIG_SEC_HASH_METHOD_RX"
      case 258: // "CONFIG_SEC_HASH_METHOD_PM"
      case 259: // "CONFIG_SEC_CHROOT_DIR"
      case 260: // "CONFIG_DIR_GEO_DB"
      case 261: // "CONFIG_DIR_GSB_DB"
      case 262: // "CONFIG_SEC_GUARDIAN_LOG"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 264: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 265: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 266: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 267: // "CONFIG_SEC_SENSOR_ID"
      case 268: // "CONFIG_DIR_ARGS_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY"
      case 270: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 271: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 272: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 273: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY"
      case 275: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 276: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 277: // "CONFIG_SEC_RULE_INHERITANCE"
      case 278: // "CONFIG_SEC_RULE_PERF_TIME"
      case 279: // "CONFIG_DIR_RULE_ENG"
      case 280: // "CONFIG_DIR_SEC_ACTION"
      case 281: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 282: // "CONFIG_DIR_SEC_MARKER"
      case 283: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 284: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 285: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 286: // "CONFIG_SEC_HTTP_BLKEY"
      case 287: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 288: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 290: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 291: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 293: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 294: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 295: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 296: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 297: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 298: // "CONFIG_UPLOAD_DIR"
      case 299: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 300: // "CONFIG_UPLOAD_FILE_MODE"
      case 301: // "CONFIG_VALUE_ABORT"
      case 302: // "CONFIG_VALUE_DETC"
      case 303: // "CONFIG_VALUE_HTTPS"
      case 304: // "CONFIG_VALUE_OFF"
      case 305: // "CONFIG_VALUE_ON"
      case 306: // "CONFIG_VALUE_PARALLEL"
      case 307: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 308: // "CONFIG_VALUE_REJECT"
      case 309: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 310: // "CONFIG_VALUE_SERIAL"
      case 311: // "CONFIG_VALUE_WARN"
      case 312: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 313: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 314: // "CONGIG_DIR_SEC_ARG_SEP"
      case 315: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 316: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 317: // "CONGIG_DIR_SEC_DATA_DIR"
      case 318: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 319: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 320: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 321: // "CONGIG_DIR_SEC_TMP_DIR"
      case 322: // "DIRECTIVE"
      case 323: // "DIRECTIVE_SECRULESCRIPT"
      case 324: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 325: // "QUOTATION_MARK"
      case 326: // "RUN_TIME_VAR_BLD"
      case 327: // "RUN_TIME_VAR_DUR"
      case 328: // "RUN_TIME_VAR_HSV"
      case 329: // "RUN_TIME_VAR_REMOTE_USER"
      case 330: // "RUN_TIME_VAR_TIME"
      case 331: // "RUN_TIME_VAR_TIME_DAY"
      case 332: // "RUN_TIME_VAR_TIME_EPOCH"
      case 333: // "RUN_TIME_VAR_TIME_HOUR"
      case 334: // "RUN_TIME_VAR_TIME_MIN"
      case 335: // "RUN_TIME_VAR_TIME_MON"
      case 336: // "RUN_TIME_VAR_TIME_SEC"
      case 337: // "RUN_TIME_VAR_TIME_WDAY"
      case 338: // "RUN_TIME_VAR_TIME_YEAR"
      case 339: // "VARIABLE"
      case 340: // "Dictionary element"
      case 341: // "Dictionary element, selected by regexp"
        yylhs.value.emplace< std::string > ();
        break;

      case 348: // op
      case 349: // op_before_init
        yylhs.value.emplace< std::unique_ptr<Operator> > ();
        break;

      case 357: // run_time_string
        yylhs.value.emplace< std::unique_ptr<RunTimeString> > ();
        break;

      case 354: // var
        yylhs.value.emplace< std::unique_ptr<Variable> > ();
        break;

      case 355: // act
      case 356: // setvar_action
        yylhs.value.emplace< std::unique_ptr<actions::Action> > ();
        break;

      case 351: // variables
      case 352: // variables_pre_process
      case 353: // variables_may_be_quoted
        yylhs.value.emplace< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 346: // actions
      case 347: // actions_may_quoted
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
  case 2:
#line 709 "seclang-parser.yy"
      {
        return 0;
      }
#line 1724 "seclang-parser.cc"
    break;

  case 6:
#line 722 "seclang-parser.yy"
      {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8));
      }
#line 1732 "seclang-parser.cc"
    break;

  case 7:
#line 728 "seclang-parser.yy"
      {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as < std::string > ());
      }
#line 1740 "seclang-parser.cc"
    break;

  case 8:
#line 734 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1748 "seclang-parser.cc"
    break;

  case 9:
#line 738 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1756 "seclang-parser.cc"
    break;

  case 10:
#line 742 "seclang-parser.yy"
      {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1764 "seclang-parser.cc"
    break;

  case 11:
#line 748 "seclang-parser.yy"
      {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8));
      }
#line 1772 "seclang-parser.cc"
    break;

  case 12:
#line 754 "seclang-parser.yy"
      {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as < std::string > ());
      }
#line 1780 "seclang-parser.cc"
    break;

  case 13:
#line 760 "seclang-parser.yy"
      {
        driver.m_auditLog->setParts(yystack_[0].value.as < std::string > ());
      }
#line 1788 "seclang-parser.cc"
    break;

  case 14:
#line 766 "seclang-parser.yy"
      {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as < std::string > ());
      }
#line 1796 "seclang-parser.cc"
    break;

  case 15:
#line 771 "seclang-parser.yy"
      {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1804 "seclang-parser.cc"
    break;

  case 16:
#line 776 "seclang-parser.yy"
      {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1812 "seclang-parser.cc"
    break;

  case 17:
#line 782 "seclang-parser.yy"
      {
        std::string relevant_status(yystack_[0].value.as < std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1821 "seclang-parser.cc"
    break;

  case 18:
#line 789 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1829 "seclang-parser.cc"
    break;

  case 19:
#line 793 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1837 "seclang-parser.cc"
    break;

  case 20:
#line 797 "seclang-parser.yy"
      {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1845 "seclang-parser.cc"
    break;

  case 21:
#line 803 "seclang-parser.yy"
      {
        driver.m_uploadKeepFiles = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 1853 "seclang-parser.cc"
    break;

  case 22:
#line 807 "seclang-parser.yy"
      {
        driver.m_uploadKeepFiles = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 1861 "seclang-parser.cc"
    break;

  case 23:
#line 811 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecUploadKeepFiles RelevantOnly is not currently supported. Accepted values are On or Off");
        YYERROR;
      }
#line 1870 "seclang-parser.cc"
    break;

  case 24:
#line 816 "seclang-parser.yy"
      {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 10);
      }
#line 1879 "seclang-parser.cc"
    break;

  case 25:
#line 821 "seclang-parser.yy"
      {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as < std::string > ().c_str(), NULL, 8);
      }
#line 1888 "seclang-parser.cc"
    break;

  case 26:
#line 826 "seclang-parser.yy"
      {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as < std::string > ();
      }
#line 1897 "seclang-parser.cc"
    break;

  case 27:
#line 831 "seclang-parser.yy"
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 1905 "seclang-parser.cc"
    break;

  case 28:
#line 835 "seclang-parser.yy"
      {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 1913 "seclang-parser.cc"
    break;

  case 29:
#line 842 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1921 "seclang-parser.cc"
    break;

  case 30:
#line 846 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1929 "seclang-parser.cc"
    break;

  case 31:
#line 853 "seclang-parser.yy"
      {
        ACTION_INIT(yystack_[0].value.as < std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1939 "seclang-parser.cc"
    break;

  case 32:
#line 859 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as < std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1950 "seclang-parser.cc"
    break;

  case 33:
#line 869 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as < std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[0].location.end.filename, &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1963 "seclang-parser.cc"
    break;

  case 34:
#line 878 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as < std::unique_ptr<Operator> > ());
        yylhs.value.as < std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[1].location.end.filename, &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1977 "seclang-parser.cc"
    break;

  case 35:
#line 888 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[0].location.end.filename, &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1990 "seclang-parser.cc"
    break;

  case 36:
#line 897 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
        yylhs.value.as < std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as < std::unique_ptr<Operator> > ()->init(*yystack_[1].location.end.filename, &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 2004 "seclang-parser.cc"
    break;

  case 37:
#line 910 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 2012 "seclang-parser.cc"
    break;

  case 38:
#line 914 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 2020 "seclang-parser.cc"
    break;

  case 39:
#line 918 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 2028 "seclang-parser.cc"
    break;

  case 40:
#line 922 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 2036 "seclang-parser.cc"
    break;

  case 41:
#line 926 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 2044 "seclang-parser.cc"
    break;

  case 42:
#line 930 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::InspectFile(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2052 "seclang-parser.cc"
    break;

  case 43:
#line 934 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::FuzzyHash(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2060 "seclang-parser.cc"
    break;

  case 44:
#line 938 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateByteRange(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2068 "seclang-parser.cc"
    break;

  case 45:
#line 942 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateDTD(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2076 "seclang-parser.cc"
    break;

  case 46:
#line 946 "seclang-parser.yy"
      {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 2085 "seclang-parser.cc"
    break;

  case 47:
#line 951 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ValidateSchema(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2093 "seclang-parser.cc"
    break;

  case 48:
#line 955 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifyCC(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2101 "seclang-parser.cc"
    break;

  case 49:
#line 959 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifyCPF(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2109 "seclang-parser.cc"
    break;

  case 50:
#line 963 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifySSN(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2117 "seclang-parser.cc"
    break;

  case 51:
#line 967 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::VerifySVNR(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2125 "seclang-parser.cc"
    break;

  case 52:
#line 971 "seclang-parser.yy"
      {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 2134 "seclang-parser.cc"
    break;

  case 53:
#line 976 "seclang-parser.yy"
      {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 2143 "seclang-parser.cc"
    break;

  case 54:
#line 981 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Within(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2151 "seclang-parser.cc"
    break;

  case 55:
#line 985 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::ContainsWord(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2159 "seclang-parser.cc"
    break;

  case 56:
#line 989 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Contains(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2167 "seclang-parser.cc"
    break;

  case 57:
#line 993 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::EndsWith(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2175 "seclang-parser.cc"
    break;

  case 58:
#line 997 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Eq(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2183 "seclang-parser.cc"
    break;

  case 59:
#line 1001 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Ge(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2191 "seclang-parser.cc"
    break;

  case 60:
#line 1005 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Gt(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2199 "seclang-parser.cc"
    break;

  case 61:
#line 1009 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::IpMatchF(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2207 "seclang-parser.cc"
    break;

  case 62:
#line 1013 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::IpMatch(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2215 "seclang-parser.cc"
    break;

  case 63:
#line 1017 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Le(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2223 "seclang-parser.cc"
    break;

  case 64:
#line 1021 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Lt(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2231 "seclang-parser.cc"
    break;

  case 65:
#line 1025 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::PmFromFile(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2239 "seclang-parser.cc"
    break;

  case 66:
#line 1029 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Pm(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2247 "seclang-parser.cc"
    break;

  case 67:
#line 1033 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rbl(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2255 "seclang-parser.cc"
    break;

  case 68:
#line 1037 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2263 "seclang-parser.cc"
    break;

  case 69:
#line 1041 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::StrEq(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2271 "seclang-parser.cc"
    break;

  case 70:
#line 1045 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::StrMatch(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2279 "seclang-parser.cc"
    break;

  case 71:
#line 1049 "seclang-parser.yy"
      {
        OPERATOR_CONTAINER(yylhs.value.as < std::unique_ptr<Operator> > (), new operators::BeginsWith(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 2287 "seclang-parser.cc"
    break;

  case 72:
#line 1053 "seclang-parser.yy"
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
#line 2302 "seclang-parser.cc"
    break;

  case 74:
#line 1068 "seclang-parser.yy"
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
#line 2336 "seclang-parser.cc"
    break;

  case 75:
#line 1098 "seclang-parser.yy"
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
#line 2359 "seclang-parser.cc"
    break;

  case 76:
#line 1117 "seclang-parser.yy"
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
#line 2382 "seclang-parser.cc"
    break;

  case 77:
#line 1136 "seclang-parser.yy"
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
#line 2414 "seclang-parser.cc"
    break;

  case 78:
#line 1164 "seclang-parser.yy"
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
#line 2475 "seclang-parser.cc"
    break;

  case 79:
#line 1221 "seclang-parser.yy"
      {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as < std::string > ()),
            /* file name */ std::unique_ptr<std::string>(new std::string(*yystack_[0].location.end.filename)),
            /* line number */ yystack_[0].location.end.line
        );
      }
#line 2486 "seclang-parser.cc"
    break;

  case 80:
#line 1228 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::DisabledRuleEngine;
      }
#line 2494 "seclang-parser.cc"
    break;

  case 81:
#line 1232 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::EnabledRuleEngine;
      }
#line 2502 "seclang-parser.cc"
    break;

  case 82:
#line 1236 "seclang-parser.yy"
      {
        driver.m_secRuleEngine = modsecurity::RulesSet::DetectionOnlyRuleEngine;
      }
#line 2510 "seclang-parser.cc"
    break;

  case 83:
#line 1240 "seclang-parser.yy"
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 2518 "seclang-parser.cc"
    break;

  case 84:
#line 1244 "seclang-parser.yy"
      {
        driver.m_secRequestBodyAccess = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 2526 "seclang-parser.cc"
    break;

  case 85:
#line 1248 "seclang-parser.yy"
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 2534 "seclang-parser.cc"
    break;

  case 86:
#line 1252 "seclang-parser.yy"
      {
        driver.m_secResponseBodyAccess = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 2542 "seclang-parser.cc"
    break;

  case 87:
#line 1256 "seclang-parser.yy"
      {
        if (yystack_[0].value.as < std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as < std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 2555 "seclang-parser.cc"
    break;

  case 88:
#line 1265 "seclang-parser.yy"
      {
        driver.m_components.push_back(yystack_[0].value.as < std::string > ());
      }
#line 2563 "seclang-parser.cc"
    break;

  case 89:
#line 1269 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecConnEngine is not yet supported.");
        YYERROR;
      }
#line 2572 "seclang-parser.cc"
    break;

  case 90:
#line 1274 "seclang-parser.yy"
      {
      }
#line 2579 "seclang-parser.cc"
    break;

  case 91:
#line 1277 "seclang-parser.yy"
      {
        driver.m_secWebAppId.m_value = yystack_[0].value.as < std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 2588 "seclang-parser.cc"
    break;

  case 92:
#line 1282 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2597 "seclang-parser.cc"
    break;

  case 93:
#line 1287 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2606 "seclang-parser.cc"
    break;

  case 94:
#line 1292 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecDisableBackendCompression is not supported.");
        YYERROR;
      }
#line 2615 "seclang-parser.cc"
    break;

  case 95:
#line 1297 "seclang-parser.yy"
      {
      }
#line 2622 "seclang-parser.cc"
    break;

  case 96:
#line 1300 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecContentInjection is not yet supported.");
        YYERROR;
      }
#line 2631 "seclang-parser.cc"
    break;

  case 97:
#line 1305 "seclang-parser.yy"
      {
      }
#line 2638 "seclang-parser.cc"
    break;

  case 98:
#line 1308 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecChrootDir is not supported.");
        YYERROR;
      }
#line 2647 "seclang-parser.cc"
    break;

  case 99:
#line 1313 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecHashEngine is not yet supported.");
        YYERROR;
      }
#line 2656 "seclang-parser.cc"
    break;

  case 100:
#line 1318 "seclang-parser.yy"
      {
      }
#line 2663 "seclang-parser.cc"
    break;

  case 101:
#line 1321 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashKey is not yet supported.");
        YYERROR;
      }
#line 2672 "seclang-parser.cc"
    break;

  case 102:
#line 1326 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashParam is not yet supported.");
        YYERROR;
      }
#line 2681 "seclang-parser.cc"
    break;

  case 103:
#line 1331 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashMethodRx is not yet supported.");
        YYERROR;
      }
#line 2690 "seclang-parser.cc"
    break;

  case 104:
#line 1336 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecHashMethodPm is not yet supported.");
        YYERROR;
      }
#line 2699 "seclang-parser.cc"
    break;

  case 105:
#line 1341 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecGsbLookupDb is not supported.");
        YYERROR;
      }
#line 2708 "seclang-parser.cc"
    break;

  case 106:
#line 1346 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecGuardianLog is not supported.");
        YYERROR;
      }
#line 2717 "seclang-parser.cc"
    break;

  case 107:
#line 1351 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecInterceptOnError is not yet supported.");
        YYERROR;
      }
#line 2726 "seclang-parser.cc"
    break;

  case 108:
#line 1356 "seclang-parser.yy"
      {
      }
#line 2733 "seclang-parser.cc"
    break;

  case 109:
#line 1359 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecConnReadStateLimit is not yet supported.");
        YYERROR;
      }
#line 2742 "seclang-parser.cc"
    break;

  case 110:
#line 1364 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecConnWriteStateLimit is not yet supported.");
        YYERROR;
      }
#line 2751 "seclang-parser.cc"
    break;

  case 111:
#line 1369 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecSensorId is not yet supported.");
        YYERROR;
      }
#line 2760 "seclang-parser.cc"
    break;

  case 112:
#line 1374 "seclang-parser.yy"
      {
        driver.error(yystack_[2].location, "SecRuleInheritance is not yet supported.");
        YYERROR;
      }
#line 2769 "seclang-parser.cc"
    break;

  case 113:
#line 1379 "seclang-parser.yy"
      {
      }
#line 2776 "seclang-parser.cc"
    break;

  case 114:
#line 1382 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecRulePerfTime is not yet supported.");
        YYERROR;
      }
#line 2785 "seclang-parser.cc"
    break;

  case 115:
#line 1387 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecStreamInBodyInspection is not supported.");
        YYERROR;
      }
#line 2794 "seclang-parser.cc"
    break;

  case 116:
#line 1392 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecStreamOutBodyInspection is not supported.");
        YYERROR;
      }
#line 2803 "seclang-parser.cc"
    break;

  case 117:
#line 1397 "seclang-parser.yy"
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
#line 2820 "seclang-parser.cc"
    break;

  case 118:
#line 1410 "seclang-parser.yy"
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
#line 2837 "seclang-parser.cc"
    break;

  case 119:
#line 1423 "seclang-parser.yy"
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
#line 2854 "seclang-parser.cc"
    break;

  case 120:
#line 1436 "seclang-parser.yy"
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
#line 2871 "seclang-parser.cc"
    break;

  case 121:
#line 1449 "seclang-parser.yy"
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
#line 2888 "seclang-parser.cc"
    break;

  case 122:
#line 1462 "seclang-parser.yy"
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
#line 2918 "seclang-parser.cc"
    break;

  case 123:
#line 1488 "seclang-parser.yy"
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
#line 2949 "seclang-parser.cc"
    break;

  case 124:
#line 1516 "seclang-parser.yy"
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
#line 2965 "seclang-parser.cc"
    break;

  case 125:
#line 1528 "seclang-parser.yy"
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
#line 2988 "seclang-parser.cc"
    break;

  case 126:
#line 1548 "seclang-parser.yy"
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
#line 3019 "seclang-parser.cc"
    break;

  case 127:
#line 1575 "seclang-parser.yy"
      {
        driver.m_argumentsLimit.m_set = true;
        driver.m_argumentsLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3028 "seclang-parser.cc"
    break;

  case 128:
#line 1581 "seclang-parser.yy"
      {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3037 "seclang-parser.cc"
    break;

  case 129:
#line 1586 "seclang-parser.yy"
      {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3046 "seclang-parser.cc"
    break;

  case 130:
#line 1591 "seclang-parser.yy"
      {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 3059 "seclang-parser.cc"
    break;

  case 131:
#line 1600 "seclang-parser.yy"
      {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as < std::string > ().c_str());
      }
#line 3068 "seclang-parser.cc"
    break;

  case 132:
#line 1605 "seclang-parser.yy"
      {
        driver.m_requestBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 3076 "seclang-parser.cc"
    break;

  case 133:
#line 1609 "seclang-parser.yy"
      {
        driver.m_requestBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::RejectBodyLimitAction;
      }
#line 3084 "seclang-parser.cc"
    break;

  case 134:
#line 1613 "seclang-parser.yy"
      {
        driver.m_responseBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 3092 "seclang-parser.cc"
    break;

  case 135:
#line 1617 "seclang-parser.yy"
      {
        driver.m_responseBodyLimitAction = modsecurity::RulesSet::BodyLimitAction::RejectBodyLimitAction;
      }
#line 3100 "seclang-parser.cc"
    break;

  case 136:
#line 1621 "seclang-parser.yy"
      {
        driver.m_remoteRulesActionOnFailed = RulesSet::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 3108 "seclang-parser.cc"
    break;

  case 137:
#line 1625 "seclang-parser.yy"
      {
        driver.m_remoteRulesActionOnFailed = RulesSet::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 3116 "seclang-parser.cc"
    break;

  case 140:
#line 1639 "seclang-parser.yy"
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
#line 3132 "seclang-parser.cc"
    break;

  case 141:
#line 1651 "seclang-parser.yy"
      {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 3142 "seclang-parser.cc"
    break;

  case 142:
#line 1657 "seclang-parser.yy"
      {
        driver.m_secXMLExternalEntity = modsecurity::RulesSetProperties::FalseConfigBoolean;
      }
#line 3150 "seclang-parser.cc"
    break;

  case 143:
#line 1661 "seclang-parser.yy"
      {
        driver.m_secXMLExternalEntity = modsecurity::RulesSetProperties::TrueConfigBoolean;
      }
#line 3158 "seclang-parser.cc"
    break;

  case 144:
#line 1665 "seclang-parser.yy"
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
#line 3173 "seclang-parser.cc"
    break;

  case 147:
#line 1686 "seclang-parser.yy"
      {
        if (atoi(yystack_[0].value.as < std::string > ().c_str()) == 1) {
          driver.error(yystack_[1].location, "SecCookieFormat 1 is not yet supported.");
          YYERROR;
        }
      }
#line 3184 "seclang-parser.cc"
    break;

  case 148:
#line 1693 "seclang-parser.yy"
      {
        driver.error(yystack_[1].location, "SecCookieV0Separator is not yet supported.");
        YYERROR;
      }
#line 3193 "seclang-parser.cc"
    break;

  case 150:
#line 1703 "seclang-parser.yy"
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
#line 3251 "seclang-parser.cc"
    break;

  case 151:
#line 1757 "seclang-parser.yy"
      {
/* Parser error disabled to avoid breaking default CRS installations with crs-setup.conf-recommended
        driver.error(@0, "SecCollectionTimeout is not yet supported.");
        YYERROR;
*/
      }
#line 3262 "seclang-parser.cc"
    break;

  case 152:
#line 1764 "seclang-parser.yy"
      {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as < std::string > ();
      }
#line 3271 "seclang-parser.cc"
    break;

  case 153:
#line 1772 "seclang-parser.yy"
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
#line 3309 "seclang-parser.cc"
    break;

  case 154:
#line 1809 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3317 "seclang-parser.cc"
    break;

  case 155:
#line 1813 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3325 "seclang-parser.cc"
    break;

  case 156:
#line 1820 "seclang-parser.yy"
      {
        yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3334 "seclang-parser.cc"
    break;

  case 157:
#line 1825 "seclang-parser.yy"
      {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3344 "seclang-parser.cc"
    break;

  case 158:
#line 1831 "seclang-parser.yy"
      {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 3354 "seclang-parser.cc"
    break;

  case 159:
#line 1837 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3364 "seclang-parser.cc"
    break;

  case 160:
#line 1843 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3375 "seclang-parser.cc"
    break;

  case 161:
#line 1850 "seclang-parser.yy"
      {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as < std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 3386 "seclang-parser.cc"
    break;

  case 162:
#line 1860 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3394 "seclang-parser.cc"
    break;

  case 163:
#line 1864 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3402 "seclang-parser.cc"
    break;

  case 164:
#line 1868 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Args_NoDictElement());
      }
#line 3410 "seclang-parser.cc"
    break;

  case 165:
#line 1872 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3418 "seclang-parser.cc"
    break;

  case 166:
#line 1876 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3426 "seclang-parser.cc"
    break;

  case 167:
#line 1880 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPost_NoDictElement());
      }
#line 3434 "seclang-parser.cc"
    break;

  case 168:
#line 1884 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3442 "seclang-parser.cc"
    break;

  case 169:
#line 1888 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3450 "seclang-parser.cc"
    break;

  case 170:
#line 1892 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGet_NoDictElement());
      }
#line 3458 "seclang-parser.cc"
    break;

  case 171:
#line 1896 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3466 "seclang-parser.cc"
    break;

  case 172:
#line 1900 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3474 "seclang-parser.cc"
    break;

  case 173:
#line 1904 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesSizes_NoDictElement());
      }
#line 3482 "seclang-parser.cc"
    break;

  case 174:
#line 1908 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3490 "seclang-parser.cc"
    break;

  case 175:
#line 1912 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3498 "seclang-parser.cc"
    break;

  case 176:
#line 1916 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesNames_NoDictElement());
      }
#line 3506 "seclang-parser.cc"
    break;

  case 177:
#line 1920 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3514 "seclang-parser.cc"
    break;

  case 178:
#line 1924 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3522 "seclang-parser.cc"
    break;

  case 179:
#line 1928 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpContent_NoDictElement());
      }
#line 3530 "seclang-parser.cc"
    break;

  case 180:
#line 1932 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3538 "seclang-parser.cc"
    break;

  case 181:
#line 1936 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3546 "seclang-parser.cc"
    break;

  case 182:
#line 1940 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartFileName_NoDictElement());
      }
#line 3554 "seclang-parser.cc"
    break;

  case 183:
#line 1944 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3562 "seclang-parser.cc"
    break;

  case 184:
#line 1948 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3570 "seclang-parser.cc"
    break;

  case 185:
#line 1952 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultiPartName_NoDictElement());
      }
#line 3578 "seclang-parser.cc"
    break;

  case 186:
#line 1956 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3586 "seclang-parser.cc"
    break;

  case 187:
#line 1960 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3594 "seclang-parser.cc"
    break;

  case 188:
#line 1964 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarsNames_NoDictElement());
      }
#line 3602 "seclang-parser.cc"
    break;

  case 189:
#line 1968 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3610 "seclang-parser.cc"
    break;

  case 190:
#line 1972 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3618 "seclang-parser.cc"
    break;

  case 191:
#line 1976 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVars_NoDictElement());
      }
#line 3626 "seclang-parser.cc"
    break;

  case 192:
#line 1980 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3634 "seclang-parser.cc"
    break;

  case 193:
#line 1984 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3642 "seclang-parser.cc"
    break;

  case 194:
#line 1988 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Files_NoDictElement());
      }
#line 3650 "seclang-parser.cc"
    break;

  case 195:
#line 1992 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3658 "seclang-parser.cc"
    break;

  case 196:
#line 1996 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3666 "seclang-parser.cc"
    break;

  case 197:
#line 2000 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookies_NoDictElement());
      }
#line 3674 "seclang-parser.cc"
    break;

  case 198:
#line 2004 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3682 "seclang-parser.cc"
    break;

  case 199:
#line 2008 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3690 "seclang-parser.cc"
    break;

  case 200:
#line 2012 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeaders_NoDictElement());
      }
#line 3698 "seclang-parser.cc"
    break;

  case 201:
#line 2016 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3706 "seclang-parser.cc"
    break;

  case 202:
#line 2020 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3714 "seclang-parser.cc"
    break;

  case 203:
#line 2024 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeaders_NoDictElement());
      }
#line 3722 "seclang-parser.cc"
    break;

  case 204:
#line 2028 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3730 "seclang-parser.cc"
    break;

  case 205:
#line 2032 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3738 "seclang-parser.cc"
    break;

  case 206:
#line 2036 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Geo_NoDictElement());
      }
#line 3746 "seclang-parser.cc"
    break;

  case 207:
#line 2040 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3754 "seclang-parser.cc"
    break;

  case 208:
#line 2044 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3762 "seclang-parser.cc"
    break;

  case 209:
#line 2048 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestCookiesNames_NoDictElement());
      }
#line 3770 "seclang-parser.cc"
    break;

  case 210:
#line 2052 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3778 "seclang-parser.cc"
    break;

  case 211:
#line 2056 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3786 "seclang-parser.cc"
    break;

  case 212:
#line 2060 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Rule_NoDictElement());
      }
#line 3794 "seclang-parser.cc"
    break;

  case 213:
#line 2064 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV:" + yystack_[0].value.as < std::string > ()));
      }
#line 3802 "seclang-parser.cc"
    break;

  case 214:
#line 2068 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV:" + yystack_[0].value.as < std::string > ()));
      }
#line 3810 "seclang-parser.cc"
    break;

  case 215:
#line 2072 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Env("ENV"));
      }
#line 3818 "seclang-parser.cc"
    break;

  case 216:
#line 2076 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML("XML:" + yystack_[0].value.as < std::string > ()));
      }
#line 3826 "seclang-parser.cc"
    break;

  case 217:
#line 2080 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML("XML:" + yystack_[0].value.as < std::string > ()));
      }
#line 3834 "seclang-parser.cc"
    break;

  case 218:
#line 2084 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::XML_NoDictElement());
      }
#line 3842 "seclang-parser.cc"
    break;

  case 219:
#line 2088 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3850 "seclang-parser.cc"
    break;

  case 220:
#line 2092 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3858 "seclang-parser.cc"
    break;

  case 221:
#line 2096 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesTmpNames_NoDictElement());
      }
#line 3866 "seclang-parser.cc"
    break;

  case 222:
#line 2100 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3874 "seclang-parser.cc"
    break;

  case 223:
#line 2104 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3882 "seclang-parser.cc"
    break;

  case 224:
#line 2108 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3890 "seclang-parser.cc"
    break;

  case 225:
#line 2112 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Resource_NoDictElement());
      }
#line 3898 "seclang-parser.cc"
    break;

  case 226:
#line 2116 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3906 "seclang-parser.cc"
    break;

  case 227:
#line 2120 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3914 "seclang-parser.cc"
    break;

  case 228:
#line 2124 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3922 "seclang-parser.cc"
    break;

  case 229:
#line 2128 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Ip_NoDictElement());
      }
#line 3930 "seclang-parser.cc"
    break;

  case 230:
#line 2132 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3938 "seclang-parser.cc"
    break;

  case 231:
#line 2136 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3946 "seclang-parser.cc"
    break;

  case 232:
#line 2140 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3954 "seclang-parser.cc"
    break;

  case 233:
#line 2144 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Global_NoDictElement());
      }
#line 3962 "seclang-parser.cc"
    break;

  case 234:
#line 2148 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 3970 "seclang-parser.cc"
    break;

  case 235:
#line 2152 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 3978 "seclang-parser.cc"
    break;

  case 236:
#line 2156 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 3986 "seclang-parser.cc"
    break;

  case 237:
#line 2160 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::User_NoDictElement());
      }
#line 3994 "seclang-parser.cc"
    break;

  case 238:
#line 2164 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 4002 "seclang-parser.cc"
    break;

  case 239:
#line 2168 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4010 "seclang-parser.cc"
    break;

  case 240:
#line 2172 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4018 "seclang-parser.cc"
    break;

  case 241:
#line 2176 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Tx_NoDictElement());
      }
#line 4026 "seclang-parser.cc"
    break;

  case 242:
#line 2180 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DynamicElement(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 4034 "seclang-parser.cc"
    break;

  case 243:
#line 2184 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4042 "seclang-parser.cc"
    break;

  case 244:
#line 2188 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4050 "seclang-parser.cc"
    break;

  case 245:
#line 2192 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Session_NoDictElement());
      }
#line 4058 "seclang-parser.cc"
    break;

  case 246:
#line 2196 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4066 "seclang-parser.cc"
    break;

  case 247:
#line 2200 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4074 "seclang-parser.cc"
    break;

  case 248:
#line 2204 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsNames_NoDictElement());
      }
#line 4082 "seclang-parser.cc"
    break;

  case 249:
#line 2208 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4090 "seclang-parser.cc"
    break;

  case 250:
#line 2212 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4098 "seclang-parser.cc"
    break;

  case 251:
#line 2216 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsGetNames_NoDictElement());
      }
#line 4106 "seclang-parser.cc"
    break;

  case 252:
#line 2221 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4114 "seclang-parser.cc"
    break;

  case 253:
#line 2225 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4122 "seclang-parser.cc"
    break;

  case 254:
#line 2229 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsPostNames_NoDictElement());
      }
#line 4130 "seclang-parser.cc"
    break;

  case 255:
#line 2234 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4138 "seclang-parser.cc"
    break;

  case 256:
#line 2238 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4146 "seclang-parser.cc"
    break;

  case 257:
#line 2242 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestHeadersNames_NoDictElement());
      }
#line 4154 "seclang-parser.cc"
    break;

  case 258:
#line 2247 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseContentType());
      }
#line 4162 "seclang-parser.cc"
    break;

  case 259:
#line 2252 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_DictElement(yystack_[0].value.as < std::string > ()));
      }
#line 4170 "seclang-parser.cc"
    break;

  case 260:
#line 2256 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as < std::string > ()));
      }
#line 4178 "seclang-parser.cc"
    break;

  case 261:
#line 2260 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseHeadersNames_NoDictElement());
      }
#line 4186 "seclang-parser.cc"
    break;

  case 262:
#line 2264 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ArgsCombinedSize());
      }
#line 4194 "seclang-parser.cc"
    break;

  case 263:
#line 2268 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::AuthType());
      }
#line 4202 "seclang-parser.cc"
    break;

  case 264:
#line 2272 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FilesCombinedSize());
      }
#line 4210 "seclang-parser.cc"
    break;

  case 265:
#line 2276 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FullRequest());
      }
#line 4218 "seclang-parser.cc"
    break;

  case 266:
#line 2280 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::FullRequestLength());
      }
#line 4226 "seclang-parser.cc"
    break;

  case 267:
#line 2284 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::InboundDataError());
      }
#line 4234 "seclang-parser.cc"
    break;

  case 268:
#line 2288 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVar());
      }
#line 4242 "seclang-parser.cc"
    break;

  case 269:
#line 2292 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MatchedVarName());
      }
#line 4250 "seclang-parser.cc"
    break;

  case 270:
#line 2296 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartBoundaryQuoted());
      }
#line 4258 "seclang-parser.cc"
    break;

  case 271:
#line 2300 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartBoundaryWhiteSpace());
      }
#line 4266 "seclang-parser.cc"
    break;

  case 272:
#line 2304 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartCrlfLFLines());
      }
#line 4274 "seclang-parser.cc"
    break;

  case 273:
#line 2308 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartDateAfter());
      }
#line 4282 "seclang-parser.cc"
    break;

  case 274:
#line 2312 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartDateBefore());
      }
#line 4290 "seclang-parser.cc"
    break;

  case 275:
#line 2316 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartFileLimitExceeded());
      }
#line 4298 "seclang-parser.cc"
    break;

  case 276:
#line 2320 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartHeaderFolding());
      }
#line 4306 "seclang-parser.cc"
    break;

  case 277:
#line 2324 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidHeaderFolding());
      }
#line 4314 "seclang-parser.cc"
    break;

  case 278:
#line 2328 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidPart());
      }
#line 4322 "seclang-parser.cc"
    break;

  case 279:
#line 2332 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartInvalidQuoting());
      }
#line 4330 "seclang-parser.cc"
    break;

  case 280:
#line 2336 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartLFLine());
      }
#line 4338 "seclang-parser.cc"
    break;

  case 281:
#line 2340 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartMissingSemicolon());
      }
#line 4346 "seclang-parser.cc"
    break;

  case 282:
#line 2344 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartMissingSemicolon());
      }
#line 4354 "seclang-parser.cc"
    break;

  case 283:
#line 2348 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartStrictError());
      }
#line 4362 "seclang-parser.cc"
    break;

  case 284:
#line 2352 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::MultipartUnmatchedBoundary());
      }
#line 4370 "seclang-parser.cc"
    break;

  case 285:
#line 2356 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::OutboundDataError());
      }
#line 4378 "seclang-parser.cc"
    break;

  case 286:
#line 2360 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::PathInfo());
      }
#line 4386 "seclang-parser.cc"
    break;

  case 287:
#line 2364 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::QueryString());
      }
#line 4394 "seclang-parser.cc"
    break;

  case 288:
#line 2368 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemoteAddr());
      }
#line 4402 "seclang-parser.cc"
    break;

  case 289:
#line 2372 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemoteHost());
      }
#line 4410 "seclang-parser.cc"
    break;

  case 290:
#line 2376 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RemotePort());
      }
#line 4418 "seclang-parser.cc"
    break;

  case 291:
#line 2380 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyError());
      }
#line 4426 "seclang-parser.cc"
    break;

  case 292:
#line 2384 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyErrorMsg());
      }
#line 4434 "seclang-parser.cc"
    break;

  case 293:
#line 2388 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessor());
      }
#line 4442 "seclang-parser.cc"
    break;

  case 294:
#line 2392 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessorError());
      }
#line 4450 "seclang-parser.cc"
    break;

  case 295:
#line 2396 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ReqbodyProcessorErrorMsg());
      }
#line 4458 "seclang-parser.cc"
    break;

  case 296:
#line 2400 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBasename());
      }
#line 4466 "seclang-parser.cc"
    break;

  case 297:
#line 2404 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBody());
      }
#line 4474 "seclang-parser.cc"
    break;

  case 298:
#line 2408 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestBodyLength());
      }
#line 4482 "seclang-parser.cc"
    break;

  case 299:
#line 2412 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestFilename());
      }
#line 4490 "seclang-parser.cc"
    break;

  case 300:
#line 2416 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestLine());
      }
#line 4498 "seclang-parser.cc"
    break;

  case 301:
#line 2420 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestMethod());
      }
#line 4506 "seclang-parser.cc"
    break;

  case 302:
#line 2424 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestProtocol());
      }
#line 4514 "seclang-parser.cc"
    break;

  case 303:
#line 2428 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestURI());
      }
#line 4522 "seclang-parser.cc"
    break;

  case 304:
#line 2432 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::RequestURIRaw());
      }
#line 4530 "seclang-parser.cc"
    break;

  case 305:
#line 2436 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseBody());
      }
#line 4538 "seclang-parser.cc"
    break;

  case 306:
#line 2440 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseContentLength());
      }
#line 4546 "seclang-parser.cc"
    break;

  case 307:
#line 2444 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseProtocol());
      }
#line 4554 "seclang-parser.cc"
    break;

  case 308:
#line 2448 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ResponseStatus());
      }
#line 4562 "seclang-parser.cc"
    break;

  case 309:
#line 2452 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerAddr());
      }
#line 4570 "seclang-parser.cc"
    break;

  case 310:
#line 2456 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerName());
      }
#line 4578 "seclang-parser.cc"
    break;

  case 311:
#line 2460 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::ServerPort());
      }
#line 4586 "seclang-parser.cc"
    break;

  case 312:
#line 2464 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::SessionID());
      }
#line 4594 "seclang-parser.cc"
    break;

  case 313:
#line 2468 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UniqueID());
      }
#line 4602 "seclang-parser.cc"
    break;

  case 314:
#line 2472 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UrlEncodedError());
      }
#line 4610 "seclang-parser.cc"
    break;

  case 315:
#line 2476 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::UserID());
      }
#line 4618 "seclang-parser.cc"
    break;

  case 316:
#line 2480 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Status());
      }
#line 4626 "seclang-parser.cc"
    break;

  case 317:
#line 2484 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::Status());
      }
#line 4634 "seclang-parser.cc"
    break;

  case 318:
#line 2488 "seclang-parser.yy"
      {
        VARIABLE_CONTAINER(yylhs.value.as < std::unique_ptr<Variable> > (), new variables::WebAppId());
      }
#line 4642 "seclang-parser.cc"
    break;

  case 319:
#line 2492 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4653 "seclang-parser.cc"
    break;

  case 320:
#line 2500 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4664 "seclang-parser.cc"
    break;

  case 321:
#line 2507 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4675 "seclang-parser.cc"
    break;

  case 322:
#line 2514 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4686 "seclang-parser.cc"
    break;

  case 323:
#line 2521 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4697 "seclang-parser.cc"
    break;

  case 324:
#line 2528 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4708 "seclang-parser.cc"
    break;

  case 325:
#line 2535 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4719 "seclang-parser.cc"
    break;

  case 326:
#line 2542 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4730 "seclang-parser.cc"
    break;

  case 327:
#line 2549 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4741 "seclang-parser.cc"
    break;

  case 328:
#line 2556 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4752 "seclang-parser.cc"
    break;

  case 329:
#line 2563 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4763 "seclang-parser.cc"
    break;

  case 330:
#line 2570 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4774 "seclang-parser.cc"
    break;

  case 331:
#line 2577 "seclang-parser.yy"
      {
        std::string name(yystack_[0].value.as < std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as < std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4785 "seclang-parser.cc"
    break;

  case 332:
#line 2587 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as < std::string > ()));
      }
#line 4793 "seclang-parser.cc"
    break;

  case 333:
#line 2591 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as < std::string > ()));
      }
#line 4801 "seclang-parser.cc"
    break;

  case 334:
#line 2595 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 4809 "seclang-parser.cc"
    break;

  case 335:
#line 2599 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as < std::string > ()));
      }
#line 4817 "seclang-parser.cc"
    break;

  case 336:
#line 2603 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Block(yystack_[0].value.as < std::string > ()));
      }
#line 4825 "seclang-parser.cc"
    break;

  case 337:
#line 2607 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as < std::string > ()));
      }
#line 4833 "seclang-parser.cc"
    break;

  case 338:
#line 2611 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as < std::string > ()));
      }
#line 4841 "seclang-parser.cc"
    break;

  case 339:
#line 2615 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4850 "seclang-parser.cc"
    break;

  case 340:
#line 2620 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4859 "seclang-parser.cc"
    break;

  case 341:
#line 2625 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4868 "seclang-parser.cc"
    break;

  case 342:
#line 2630 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as < std::string > ()));
      }
#line 4876 "seclang-parser.cc"
    break;

  case 343:
#line 2634 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as < std::string > ()));
      }
#line 4884 "seclang-parser.cc"
    break;

  case 344:
#line 2638 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as < std::string > ()));
      }
#line 4892 "seclang-parser.cc"
    break;

  case 345:
#line 2642 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorURLENCODED(yystack_[0].value.as < std::string > ()));
      }
#line 4900 "seclang-parser.cc"
    break;

  case 346:
#line 2646 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4909 "seclang-parser.cc"
    break;

  case 347:
#line 2651 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as < std::string > ()));
      }
#line 4918 "seclang-parser.cc"
    break;

  case 348:
#line 2656 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as < std::string > () + "true"));
      }
#line 4926 "seclang-parser.cc"
    break;

  case 349:
#line 2660 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as < std::string > () + "false"));
      }
#line 4934 "seclang-parser.cc"
    break;

  case 350:
#line 2664 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 4942 "seclang-parser.cc"
    break;

  case 351:
#line 2668 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 4950 "seclang-parser.cc"
    break;

  case 352:
#line 2672 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 4958 "seclang-parser.cc"
    break;

  case 353:
#line 2676 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as < std::string > ()));
      }
#line 4966 "seclang-parser.cc"
    break;

  case 354:
#line 2680 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveByTag(yystack_[0].value.as < std::string > ()));
      }
#line 4974 "seclang-parser.cc"
    break;

  case 355:
#line 2684 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as < std::string > ()));
      }
#line 4982 "seclang-parser.cc"
    break;

  case 356:
#line 2688 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as < std::string > ()));
      }
#line 4990 "seclang-parser.cc"
    break;

  case 357:
#line 2692 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as < std::string > ()));
      }
#line 4998 "seclang-parser.cc"
    break;

  case 358:
#line 2696 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 5006 "seclang-parser.cc"
    break;

  case 359:
#line 2700 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Drop(yystack_[0].value.as < std::string > ()));
      }
#line 5014 "seclang-parser.cc"
    break;

  case 360:
#line 2704 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as < std::string > ()));
      }
#line 5022 "seclang-parser.cc"
    break;

  case 361:
#line 2708 "seclang-parser.yy"
      {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as < std::string > ()));
      }
#line 5031 "seclang-parser.cc"
    break;

  case 362:
#line 2713 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as < std::string > ()));
      }
#line 5039 "seclang-parser.cc"
    break;

  case 363:
#line 2717 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as < std::string > (), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5047 "seclang-parser.cc"
    break;

  case 364:
#line 2721 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5055 "seclang-parser.cc"
    break;

  case 365:
#line 2725 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as < std::string > ()));
      }
#line 5063 "seclang-parser.cc"
    break;

  case 366:
#line 2729 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as < std::string > ()));
      }
#line 5071 "seclang-parser.cc"
    break;

  case 367:
#line 2733 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5079 "seclang-parser.cc"
    break;

  case 368:
#line 2737 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as < std::string > ()));
      }
#line 5087 "seclang-parser.cc"
    break;

  case 369:
#line 2741 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as < std::string > ()));
      }
#line 5095 "seclang-parser.cc"
    break;

  case 370:
#line 2745 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as < std::string > ()));
      }
#line 5103 "seclang-parser.cc"
    break;

  case 371:
#line 2749 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as < std::string > ()));
      }
#line 5111 "seclang-parser.cc"
    break;

  case 372:
#line 2753 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 5119 "seclang-parser.cc"
    break;

  case 373:
#line 2757 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as < std::string > ()));
      }
#line 5127 "seclang-parser.cc"
    break;

  case 374:
#line 2761 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 5135 "seclang-parser.cc"
    break;

  case 375:
#line 2765 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 5143 "seclang-parser.cc"
    break;

  case 376:
#line 2769 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5151 "seclang-parser.cc"
    break;

  case 377:
#line 2773 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as < std::string > ()));
      }
#line 5159 "seclang-parser.cc"
    break;

  case 378:
#line 2777 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseArg", yystack_[1].location);
      }
#line 5167 "seclang-parser.cc"
    break;

  case 379:
#line 2781 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseMatched", yystack_[1].location);
      }
#line 5175 "seclang-parser.cc"
    break;

  case 380:
#line 2785 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseMatchedBytes", yystack_[1].location);
      }
#line 5183 "seclang-parser.cc"
    break;

  case 381:
#line 2789 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseRequestHeader", yystack_[1].location);
      }
#line 5191 "seclang-parser.cc"
    break;

  case 382:
#line 2793 "seclang-parser.yy"
      {
        ACTION_NOT_SUPPORTED("SanitiseResponseHeader", yystack_[1].location);
      }
#line 5199 "seclang-parser.cc"
    break;

  case 383:
#line 2797 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetENV(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5207 "seclang-parser.cc"
    break;

  case 384:
#line 2801 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5215 "seclang-parser.cc"
    break;

  case 385:
#line 2805 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5223 "seclang-parser.cc"
    break;

  case 386:
#line 2809 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5231 "seclang-parser.cc"
    break;

  case 387:
#line 2813 "seclang-parser.yy"
      {
        yylhs.value.as < std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as < std::unique_ptr<actions::Action> > ());
      }
#line 5239 "seclang-parser.cc"
    break;

  case 388:
#line 2817 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as < std::string > ()));
      }
#line 5247 "seclang-parser.cc"
    break;

  case 389:
#line 2821 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as < std::string > ()));
      }
#line 5255 "seclang-parser.cc"
    break;

  case 390:
#line 2825 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as < std::string > ()));
      }
#line 5263 "seclang-parser.cc"
    break;

  case 391:
#line 2829 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as < std::string > ()));
      }
#line 5271 "seclang-parser.cc"
    break;

  case 392:
#line 2833 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5279 "seclang-parser.cc"
    break;

  case 393:
#line 2837 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as < std::string > ()));
      }
#line 5287 "seclang-parser.cc"
    break;

  case 394:
#line 2841 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as < std::string > ()));
      }
#line 5295 "seclang-parser.cc"
    break;

  case 395:
#line 2845 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5303 "seclang-parser.cc"
    break;

  case 396:
#line 2849 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5311 "seclang-parser.cc"
    break;

  case 397:
#line 2853 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as < std::string > ()));
      }
#line 5319 "seclang-parser.cc"
    break;

  case 398:
#line 2857 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5327 "seclang-parser.cc"
    break;

  case 399:
#line 2861 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Encode(yystack_[0].value.as < std::string > ()));
      }
#line 5335 "seclang-parser.cc"
    break;

  case 400:
#line 2865 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Decode(yystack_[0].value.as < std::string > ()));
      }
#line 5343 "seclang-parser.cc"
    break;

  case 401:
#line 2869 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Base64DecodeExt(yystack_[0].value.as < std::string > ()));
      }
#line 5351 "seclang-parser.cc"
    break;

  case 402:
#line 2873 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as < std::string > ()));
      }
#line 5359 "seclang-parser.cc"
    break;

  case 403:
#line 2877 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as < std::string > ()));
      }
#line 5367 "seclang-parser.cc"
    break;

  case 404:
#line 2881 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as < std::string > ()));
      }
#line 5375 "seclang-parser.cc"
    break;

  case 405:
#line 2885 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::EscapeSeqDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5383 "seclang-parser.cc"
    break;

  case 406:
#line 2889 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as < std::string > ()));
      }
#line 5391 "seclang-parser.cc"
    break;

  case 407:
#line 2893 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5399 "seclang-parser.cc"
    break;

  case 408:
#line 2897 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as < std::string > ()));
      }
#line 5407 "seclang-parser.cc"
    break;

  case 409:
#line 2901 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as < std::string > ()));
      }
#line 5415 "seclang-parser.cc"
    break;

  case 410:
#line 2905 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as < std::string > ()));
      }
#line 5423 "seclang-parser.cc"
    break;

  case 411:
#line 2909 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5431 "seclang-parser.cc"
    break;

  case 412:
#line 2913 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::UrlEncode(yystack_[0].value.as < std::string > ()));
      }
#line 5439 "seclang-parser.cc"
    break;

  case 413:
#line 2917 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as < std::string > ()));
      }
#line 5447 "seclang-parser.cc"
    break;

  case 414:
#line 2921 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as < std::string > ()));
      }
#line 5455 "seclang-parser.cc"
    break;

  case 415:
#line 2925 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as < std::string > ()));
      }
#line 5463 "seclang-parser.cc"
    break;

  case 416:
#line 2929 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as < std::string > ()));
      }
#line 5471 "seclang-parser.cc"
    break;

  case 417:
#line 2933 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as < std::string > ()));
      }
#line 5479 "seclang-parser.cc"
    break;

  case 418:
#line 2937 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5487 "seclang-parser.cc"
    break;

  case 419:
#line 2941 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5495 "seclang-parser.cc"
    break;

  case 420:
#line 2945 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as < std::string > ()));
      }
#line 5503 "seclang-parser.cc"
    break;

  case 421:
#line 2949 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as < std::string > ()));
      }
#line 5511 "seclang-parser.cc"
    break;

  case 422:
#line 2953 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::TrimLeft(yystack_[0].value.as < std::string > ()));
      }
#line 5519 "seclang-parser.cc"
    break;

  case 423:
#line 2957 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::TrimRight(yystack_[0].value.as < std::string > ()));
      }
#line 5527 "seclang-parser.cc"
    break;

  case 424:
#line 2961 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as < std::string > ()));
      }
#line 5535 "seclang-parser.cc"
    break;

  case 425:
#line 2965 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as < std::string > ()));
      }
#line 5543 "seclang-parser.cc"
    break;

  case 426:
#line 2969 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as < std::string > ()));
      }
#line 5551 "seclang-parser.cc"
    break;

  case 427:
#line 2973 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as < std::string > ()));
      }
#line 5559 "seclang-parser.cc"
    break;

  case 428:
#line 2977 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as < std::string > ()));
      }
#line 5567 "seclang-parser.cc"
    break;

  case 429:
#line 2981 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as < std::string > ()));
      }
#line 5575 "seclang-parser.cc"
    break;

  case 430:
#line 2985 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as < std::string > ()));
      }
#line 5583 "seclang-parser.cc"
    break;

  case 431:
#line 2992 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
      }
#line 5591 "seclang-parser.cc"
    break;

  case 432:
#line 2996 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ())));
      }
#line 5599 "seclang-parser.cc"
    break;

  case 433:
#line 3000 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5607 "seclang-parser.cc"
    break;

  case 434:
#line 3004 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5615 "seclang-parser.cc"
    break;

  case 435:
#line 3008 "seclang-parser.yy"
      {
        ACTION_CONTAINER(yylhs.value.as < std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, std::move(yystack_[2].value.as < std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as < std::unique_ptr<RunTimeString> > ())));
      }
#line 5623 "seclang-parser.cc"
    break;

  case 436:
#line 3015 "seclang-parser.yy"
      {
        yystack_[1].value.as < std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as < std::string > ());
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as < std::unique_ptr<RunTimeString> > ());
      }
#line 5632 "seclang-parser.cc"
    break;

  case 437:
#line 3020 "seclang-parser.yy"
      {
        yystack_[1].value.as < std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as < std::unique_ptr<RunTimeString> > ());
      }
#line 5641 "seclang-parser.cc"
    break;

  case 438:
#line 3025 "seclang-parser.yy"
      {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as < std::string > ());
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 5651 "seclang-parser.cc"
    break;

  case 439:
#line 3031 "seclang-parser.yy"
      {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as < std::unique_ptr<Variable> > ()));
        yylhs.value.as < std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 5661 "seclang-parser.cc"
    break;


#line 5665 "seclang-parser.cc"

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
      YY_STACK_PRINT ();

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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
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

  // Generate an error message.
  std::string
  seclang_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

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
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short seclang_parser::yypact_ninf_ = -422;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short
  seclang_parser::yypact_[] =
  {
    2498,  -422,  -249,  -422,   -85,  -422,  -245,  -422,  -422,  -422,
    -422,  -422,  -273,  -422,  -422,  -422,  -422,  -422,  -285,  -422,
    -422,  -422,  -153,  -151,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -146,  -422,
    -422,  -147,  -422,  -142,  -422,  -143,  -132,  -422,  -259,  2843,
    2843,  -422,  -422,  -422,  -422,  -130,  -298,  -422,  -422,  -422,
    1145,  1145,  1145,  2843,  -267,  -128,  -422,  -422,  -422,  -126,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  1145,
    2843,  2658,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  2116,  -255,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -265,  -422,  -422,  -422,  -422,  -124,  -122,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    2210,  -422,  2210,  -422,  2210,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  2210,  -422,  -422,  -422,  -422,  -422,  -422,
    2210,  2210,  2210,  2210,  -422,  -422,  -422,  -422,  2210,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  3025,  -422,     5,
    -422,  -422,  -422,  -422,  -422,  -422,  2539,  2539,  -156,  -154,
    -152,  -150,  -148,  -145,  -141,  -138,  -136,  -134,  -131,  -129,
    -127,  -125,  -123,  -121,  -422,  -119,  -117,  -115,  -113,  -422,
    -422,  -111,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -109,  -422,  -422,
    -422,  -422,  -422,    51,  -422,  -422,  -422,  -107,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
     455,   544,   633,   965,  1054,  -105,  -103,  1476,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,     6,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  1656,  -422,  -422,  -422,  -422,  2539,
     -49,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  2314,  2314,  2314,  2314,  2314,  2314,
    2314,  2314,  2314,     4,  3025,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,  -422,
    -422,  -422,  -422,  2314,  -422,  -422,  -422,  -422,  2314,  -422,
    -422,  2314,  -422,  -422,  2314,  -422,  -422,  2314,  -422,  -422,
    2314,  -422,  -422,  -422,  -422,     9,  1567,  1985,  2210,  2210,
    2210,  -422,  -422,  2210,  2210,  2210,  -422,  2210,  2210,  2210,
    2210,  2210,  2210,  2210,  2210,  2210,  2210,  2210,  2210,  2210,
    2210,  2210,  -422,  2210,  2210,  2210,  2210,  -422,  -422,  2210,
    2210,  2210,  2210,  2210,  2843,  -422,  2314,  -422,  2210,  2210,
    2210,  -422,  -422,  -422,  -422,  -422,  2539,  2539,  -422,  -422,
    2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,
    2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,
    2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,  2314,
    2314,  -422,  2314,  2314,  2314,  -422,  -422
  };

  const short
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   141,     0,    88,     0,    87,    91,    92,
       7,     6,     0,    11,    14,    12,    13,    17,     0,   125,
     124,    93,     0,     0,   101,   102,   103,   104,    98,   126,
     105,   106,   139,   138,   109,   110,   111,   127,     0,   130,
     128,     0,   129,     0,   131,     0,     0,   114,     0,     0,
       0,    79,   150,   151,   152,     0,     0,   117,   119,   118,
       0,     0,     0,     0,     0,     0,    26,    24,    25,     0,
     140,   146,   147,   148,   145,   149,   115,   116,   144,     0,
       0,     0,     4,    73,     5,    97,    96,    15,    16,    90,
      89,     9,    10,     8,    20,    19,    18,    95,    94,   100,
      99,    84,    83,   132,   133,    86,    85,   134,   135,   113,
     112,    82,    80,    81,     0,     0,   332,   333,   334,   335,
     336,   337,   338,     0,   342,   343,   344,   345,     0,     0,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
       0,   365,     0,   366,     0,   368,   369,   370,   371,   372,
     373,   374,   375,     0,   377,   378,   379,   380,   381,   382,
       0,     0,     0,     0,   388,   389,   390,   391,     0,   399,
     400,   401,   402,   414,   420,   405,   406,   407,   418,   419,
     426,   408,   404,   413,   425,   424,   397,   396,   395,   429,
     428,   417,   415,   430,   416,   403,   398,   421,   422,   423,
     409,   412,   411,   410,   427,   393,   394,     0,    76,    30,
      32,    78,   108,   107,   136,   137,     0,     0,   164,   167,
     170,   173,   176,   179,   182,   185,   188,   191,   194,   197,
     200,   203,   206,   209,   262,   251,   212,   248,   254,   263,
     264,   221,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   292,   291,
     295,   294,   293,   296,   298,   297,   299,   257,   300,   301,
     302,   304,   303,   225,   305,   306,   258,   261,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   318,   316,   317,
     229,   233,   241,   245,   237,   215,   218,     0,   320,   319,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   120,   154,   159,   121,   122,   123,    22,    21,    23,
      28,    27,   142,   143,     0,   153,    77,     1,     3,     0,
     432,   387,   352,   351,   350,   340,   339,   341,   347,   346,
     349,   348,   438,   439,   363,   364,   367,   376,   383,   384,
     385,   386,   392,     0,     0,   161,   160,   162,   163,   165,
     166,   168,   169,   171,   172,   174,   175,   177,   178,   180,
     181,   183,   184,   186,   187,   189,   190,   192,   193,   195,
     196,   198,   199,   201,   202,   204,   205,   207,   208,   249,
     250,   210,   211,   246,   247,   252,   253,   219,   220,   255,
     256,   223,   224,   222,   259,   260,   227,   228,   226,   231,
     232,   230,   239,   240,   238,   243,   244,   242,   235,   236,
     234,   213,   214,   216,   217,     0,     0,     0,     0,     0,
       0,    38,    39,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,    75,    33,    35,   431,     0,     0,
       0,   436,   437,    29,    31,   155,     0,     0,   156,    34,
      36,    71,    56,    55,    57,    58,    43,    59,    52,    60,
      42,    61,    62,    63,    64,    65,    66,    67,    53,    68,
      69,    70,    44,    45,    46,    47,    48,    49,    50,    51,
      54,    74,   433,   434,   435,   158,   157
  };

  const short
  seclang_parser::yypgoto_[] =
  {
    -422,  -422,   -71,  -422,   -46,  -193,  -422,  -421,  -422,  -422,
     -56,  -283,   -60,  -342,  -422,  -133
  };

  const short
  seclang_parser::yydefgoto_[] =
  {
      -1,    81,    82,    83,   208,   209,   474,   475,    84,   334,
     321,   322,   353,   210,   341,   354
  };

  const short
  seclang_parser::yytable_[] =
  {
     323,   323,   323,   214,   211,   324,   325,   364,   364,   355,
     338,   356,   436,   215,   363,   436,   489,   326,    94,   323,
     357,    95,   484,   335,   435,    96,     0,   358,   359,   360,
     361,    91,    92,     0,   336,   362,    93,   327,   328,   345,
     346,     0,   329,   111,   347,   112,   113,   342,     0,   343,
     344,   478,   479,   480,   340,    85,    86,    87,    88,    89,
      90,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     413,    97,    98,    99,   100,     0,   365,   366,   101,   102,
     103,   104,   105,   106,   107,   108,     0,   418,   421,   424,
     427,   430,   109,   110,   212,   213,   330,   331,   332,   333,
     348,   349,   350,   351,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,     0,   377,   378,     0,     0,   379,
     380,   476,   381,   382,   383,   384,   385,   386,     0,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   414,   415,   431,   432,   433,   434,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   477,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   482,   482,   482,   482,   482,   482,
     482,   482,   482,     0,   490,   491,   492,   493,     0,     0,
     494,   495,   496,     0,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   483,
     512,   513,   514,   515,   485,     0,   516,   517,   518,   519,
     520,     0,     0,     0,     0,   522,   523,   524,     0,     0,
       0,     0,     0,   482,     0,     0,     0,     0,   482,     0,
       0,   482,     0,     0,   482,     0,     0,   482,     0,     0,
     482,     0,     0,     0,     0,   352,   488,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   411,   412,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   526,   521,     0,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,     0,   482,   482,   482,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   416,   417,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   352,     0,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   422,   423,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,   428,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   486,   487,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,     0,     0,     0,     0,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,     0,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,     0,     0,     0,
       0,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,     0,     0,     0,     0,     0,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
       0,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,     0,     0,     0,     0,   339,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   352,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,     0,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   481,     4,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,     0,     0,     0,     0,   337,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,   114,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,     0,     0,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206
  };

  const short
  seclang_parser::yycheck_[] =
  {
      60,    61,    62,   301,    50,    61,    62,     3,     3,   142,
      81,   144,     6,   311,   207,     6,   437,    63,   303,    79,
     153,   306,   364,    79,   307,   310,    -1,   160,   161,   162,
     163,   304,   305,    -1,    80,   168,   309,   304,   305,   304,
     305,    -1,   309,   302,   309,   304,   305,   302,    -1,   304,
     305,   100,   101,   102,   114,   304,   305,   142,   143,   304,
     305,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     283,   304,   305,   304,   305,    -1,   216,   217,   304,   305,
     307,   308,   304,   305,   307,   308,    -1,   300,   301,   302,
     303,   304,   304,   305,   304,   305,   304,   305,   304,   305,
     304,   305,   304,   305,   340,   341,   340,   341,   340,   341,
     340,   341,   340,   341,    -1,   340,   341,    -1,    -1,   340,
     341,   334,   340,   341,   340,   341,   340,   341,    -1,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
     360,   361,   362,    -1,   437,   438,   439,   440,    -1,    -1,
     443,   444,   445,    -1,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   325,
     463,   464,   465,   466,   325,    -1,   469,   470,   471,   472,
     473,    -1,    -1,    -1,    -1,   478,   479,   480,    -1,    -1,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,   418,    -1,
      -1,   421,    -1,    -1,   424,    -1,    -1,   427,    -1,    -1,
     430,    -1,    -1,    -1,    -1,   324,   436,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
      -1,   340,   341,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   486,   487,   474,    -1,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,    -1,   522,   523,   524,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    10,    11,    12,    13,    14,    15,
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
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,
      -1,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,    -1,   340,   341,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,    -1,   340,   341,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,    -1,   340,   341,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,     8,     9,    10,    11,    12,    13,    14,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,
      -1,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,    -1,   340,   341,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,    -1,   340,   341,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,     8,     9,    10,    11,    12,
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
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,    -1,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    10,    11,    12,    13,
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
      90,    91,    92,    93,    94,    95,    96,    97,    98,   324,
      -1,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,   324,   141,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,    -1,    -1,    -1,    -1,    -1,     0,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,    -1,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,    99,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   325,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235
  };

  const short
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   141,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   343,   344,   345,   350,   304,   305,   142,   143,   304,
     305,   304,   305,   309,   303,   306,   310,   304,   305,   304,
     305,   304,   305,   307,   308,   304,   305,   307,   308,   304,
     305,   302,   304,   305,    99,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   325,   346,   347,
     355,   346,   304,   305,   301,   311,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   352,   353,   354,   352,   352,   346,   304,   305,   309,
     304,   305,   304,   305,   351,   352,   346,     0,   344,   103,
     354,   356,   302,   304,   305,   304,   305,   309,   304,   305,
     304,   305,   324,   354,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   347,     3,   354,   354,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   340,   341,   340,   341,   340,   341,   340,
     341,   340,   341,   357,   340,   341,   340,   341,   357,   340,
     341,   357,   340,   341,   357,   340,   341,   357,   340,   341,
     357,   340,   341,   340,   341,   353,     6,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   348,   349,   357,   354,   100,   101,
     102,   324,   354,   325,   355,   325,     8,     9,   354,   349,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   346,   357,   357,   357,   354,   354
  };

  const short
  seclang_parser::yyr1_[] =
  {
       0,   342,   343,   343,   343,   344,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   346,
     346,   347,   347,   348,   348,   348,   348,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   351,   352,   352,   353,   353,   353,   353,
     353,   353,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   356,   356,   356,   356,   356,   357,   357,   357,   357
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
       2,     2,     1,     1,     4,     3,     2,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       2,     1,     1,     1,     2,     2,     2,     2,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     4,     1,
       2,     2,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     2,     1,     2,     2,     2,     1,
       2,     2,     2,     1,     2,     2,     2,     1,     2,     2,
       2,     1,     2,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     3,     3,     3,     2,     2,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"",
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
  "\"OPERATOR_STR_EQ\"", "\"OPERATOR_STR_MATCH\"",
  "\"OPERATOR_UNCONDITIONAL_MATCH\"", "\"OPERATOR_VALIDATE_BYTE_RANGE\"",
  "\"OPERATOR_VALIDATE_DTD\"", "\"OPERATOR_VALIDATE_HASH\"",
  "\"OPERATOR_VALIDATE_SCHEMA\"", "\"OPERATOR_VALIDATE_URL_ENCODING\"",
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
  "\"ACTION_TRANSFORMATION_LOWERCASE\"", "\"ACTION_TRANSFORMATION_MD5\"",
  "\"ACTION_TRANSFORMATION_NONE\"",
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

#if YYDEBUG
  const short
  seclang_parser::yyrline_[] =
  {
       0,   708,   708,   712,   713,   716,   721,   727,   733,   737,
     741,   747,   753,   759,   765,   770,   775,   781,   788,   792,
     796,   802,   806,   810,   815,   820,   825,   830,   834,   841,
     845,   852,   858,   868,   877,   887,   896,   909,   913,   917,
     921,   925,   929,   933,   937,   941,   945,   950,   954,   958,
     962,   966,   970,   975,   980,   984,   988,   992,   996,  1000,
    1004,  1008,  1012,  1016,  1020,  1024,  1028,  1032,  1036,  1040,
    1044,  1048,  1052,  1066,  1067,  1097,  1116,  1135,  1163,  1220,
    1227,  1231,  1235,  1239,  1243,  1247,  1251,  1255,  1264,  1268,
    1273,  1276,  1281,  1286,  1291,  1296,  1299,  1304,  1307,  1312,
    1317,  1320,  1325,  1330,  1335,  1340,  1345,  1350,  1355,  1358,
    1363,  1368,  1373,  1378,  1381,  1386,  1391,  1396,  1409,  1422,
    1435,  1448,  1461,  1487,  1515,  1527,  1547,  1574,  1580,  1585,
    1590,  1599,  1604,  1608,  1612,  1616,  1620,  1624,  1628,  1633,
    1638,  1650,  1656,  1660,  1664,  1675,  1684,  1685,  1692,  1697,
    1702,  1756,  1763,  1771,  1808,  1812,  1819,  1824,  1830,  1836,
    1842,  1849,  1859,  1863,  1867,  1871,  1875,  1879,  1883,  1887,
    1891,  1895,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1927,
    1931,  1935,  1939,  1943,  1947,  1951,  1955,  1959,  1963,  1967,
    1971,  1975,  1979,  1983,  1987,  1991,  1995,  1999,  2003,  2007,
    2011,  2015,  2019,  2023,  2027,  2031,  2035,  2039,  2043,  2047,
    2051,  2055,  2059,  2063,  2067,  2071,  2075,  2079,  2083,  2087,
    2091,  2095,  2099,  2103,  2107,  2111,  2115,  2119,  2123,  2127,
    2131,  2135,  2139,  2143,  2147,  2151,  2155,  2159,  2163,  2167,
    2171,  2175,  2179,  2183,  2187,  2191,  2195,  2199,  2203,  2207,
    2211,  2215,  2220,  2224,  2228,  2233,  2237,  2241,  2246,  2251,
    2255,  2259,  2263,  2267,  2271,  2275,  2279,  2283,  2287,  2291,
    2295,  2299,  2303,  2307,  2311,  2315,  2319,  2323,  2327,  2331,
    2335,  2339,  2343,  2347,  2351,  2355,  2359,  2363,  2367,  2371,
    2375,  2379,  2383,  2387,  2391,  2395,  2399,  2403,  2407,  2411,
    2415,  2419,  2423,  2427,  2431,  2435,  2439,  2443,  2447,  2451,
    2455,  2459,  2463,  2467,  2471,  2475,  2479,  2483,  2487,  2491,
    2499,  2506,  2513,  2520,  2527,  2534,  2541,  2548,  2555,  2562,
    2569,  2576,  2586,  2590,  2594,  2598,  2602,  2606,  2610,  2614,
    2619,  2624,  2629,  2633,  2637,  2641,  2645,  2650,  2655,  2659,
    2663,  2667,  2671,  2675,  2679,  2683,  2687,  2691,  2695,  2699,
    2703,  2707,  2712,  2716,  2720,  2724,  2728,  2732,  2736,  2740,
    2744,  2748,  2752,  2756,  2760,  2764,  2768,  2772,  2776,  2780,
    2784,  2788,  2792,  2796,  2800,  2804,  2808,  2812,  2816,  2820,
    2824,  2828,  2832,  2836,  2840,  2844,  2848,  2852,  2856,  2860,
    2864,  2868,  2872,  2876,  2880,  2884,  2888,  2892,  2896,  2900,
    2904,  2908,  2912,  2916,  2920,  2924,  2928,  2932,  2936,  2940,
    2944,  2948,  2952,  2956,  2960,  2964,  2968,  2972,  2976,  2980,
    2984,  2991,  2995,  2999,  3003,  3007,  3014,  3019,  3024,  3030
  };

  // Print the state stack on the debug stream.
  void
  seclang_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  seclang_parser::yy_reduce_print_ (int yyrule)
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
#line 7152 "seclang-parser.cc"

#line 3037 "seclang-parser.yy"


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
