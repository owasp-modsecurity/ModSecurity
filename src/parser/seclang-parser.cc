// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

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


// First part of user declarations.

#line 37 "seclang-parser.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "seclang-parser.hh"

// User implementation prologue.

#line 51 "seclang-parser.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 358 "seclang-parser.yy" // lalr1.cc:413

#include "src/parser/driver.h"

#line 57 "seclang-parser.cc" // lalr1.cc:413


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 143 "seclang-parser.cc" // lalr1.cc:479

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
        std::string yyr = "";
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
              // Fall through.
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
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  seclang_parser::~seclang_parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  seclang_parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  seclang_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  seclang_parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  seclang_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  seclang_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  seclang_parser::symbol_number_type
  seclang_parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  seclang_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  seclang_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 143: // "Accuracy"
      case 144: // "Allow"
      case 145: // "Append"
      case 146: // "AuditLog"
      case 147: // "Block"
      case 148: // "Capture"
      case 149: // "Chain"
      case 150: // "ACTION_CTL_AUDIT_ENGINE"
      case 151: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 152: // "ACTION_CTL_BDY_JSON"
      case 153: // "ACTION_CTL_BDY_XML"
      case 154: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 155: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 156: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 158: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 159: // "Deny"
      case 160: // "DeprecateVar"
      case 161: // "Drop"
      case 162: // "Exec"
      case 163: // "ExpireVar"
      case 164: // "Id"
      case 165: // "InitCol"
      case 166: // "Log"
      case 167: // "LogData"
      case 168: // "Maturity"
      case 169: // "Msg"
      case 170: // "MultiMatch"
      case 171: // "NoAuditLog"
      case 172: // "NoLog"
      case 173: // "Pass"
      case 174: // "Pause"
      case 175: // "Phase"
      case 176: // "Prepend"
      case 177: // "Proxy"
      case 178: // "Redirect"
      case 179: // "Rev"
      case 180: // "SanatiseArg"
      case 181: // "SanatiseMatched"
      case 182: // "SanatiseMatchedBytes"
      case 183: // "SanatiseRequestHeader"
      case 184: // "SanatiseResponseHeader"
      case 185: // "SetEnv"
      case 186: // "SetRsc"
      case 187: // "SetSid"
      case 188: // "SetUID"
      case 189: // "Severity"
      case 190: // "Skip"
      case 191: // "SkipAfter"
      case 192: // "Status"
      case 193: // "Tag"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 196: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 197: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 198: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 199: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 201: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 202: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 203: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_LENGTH"
      case 205: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 206: // "ACTION_TRANSFORMATION_MD5"
      case 207: // "ACTION_TRANSFORMATION_NONE"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 209: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 210: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 216: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 219: // "ACTION_TRANSFORMATION_SHA1"
      case 220: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 221: // "ACTION_TRANSFORMATION_TRIM"
      case 222: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 224: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 225: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 226: // "Ver"
      case 227: // "xmlns"
      case 228: // "CONFIG_COMPONENT_SIG"
      case 229: // "CONFIG_CONN_ENGINE"
      case 230: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 231: // "CONFIG_SEC_WEB_APP_ID"
      case 232: // "CONFIG_SEC_SERVER_SIG"
      case 233: // "CONFIG_DIR_AUDIT_DIR"
      case 234: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 235: // "CONFIG_DIR_AUDIT_ENG"
      case 236: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 237: // "CONFIG_DIR_AUDIT_LOG"
      case 238: // "CONFIG_DIR_AUDIT_LOG2"
      case 239: // "CONFIG_DIR_AUDIT_LOG_P"
      case 240: // "CONFIG_DIR_AUDIT_STS"
      case 241: // "CONFIG_DIR_AUDIT_TPE"
      case 242: // "CONFIG_DIR_DEBUG_LOG"
      case 243: // "CONFIG_DIR_DEBUG_LVL"
      case 244: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 245: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 246: // "CONFIG_SEC_HASH_ENGINE"
      case 247: // "CONFIG_SEC_HASH_KEY"
      case 248: // "CONFIG_SEC_HASH_PARAM"
      case 249: // "CONFIG_SEC_HASH_METHOD_RX"
      case 250: // "CONFIG_SEC_HASH_METHOD_PM"
      case 251: // "CONFIG_SEC_CHROOT_DIR"
      case 252: // "CONFIG_DIR_GEO_DB"
      case 253: // "CONFIG_DIR_GSB_DB"
      case 254: // "CONFIG_SEC_GUARDIAN_LOG"
      case 255: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 256: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 257: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 258: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 259: // "CONFIG_SEC_SENSOR_ID"
      case 260: // "CONFIG_DIR_REQ_BODY"
      case 261: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 262: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 263: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 264: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 265: // "CONFIG_DIR_RES_BODY"
      case 266: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 267: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 268: // "CONFIG_SEC_RULE_INHERITANCE"
      case 269: // "CONFIG_SEC_RULE_PERF_TIME"
      case 270: // "CONFIG_DIR_RULE_ENG"
      case 271: // "CONFIG_DIR_SEC_ACTION"
      case 272: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 273: // "CONFIG_DIR_SEC_MARKER"
      case 274: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 275: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 276: // "CONFIG_SEC_HTTP_BLKEY"
      case 277: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 278: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 279: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 280: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 281: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 282: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 283: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 284: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 285: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 286: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 287: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 288: // "CONFIG_UPLOAD_DIR"
      case 289: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 290: // "CONFIG_UPLOAD_FILE_MODE"
      case 291: // "CONFIG_VALUE_ABORT"
      case 292: // "CONFIG_VALUE_DETC"
      case 293: // "CONFIG_VALUE_HTTPS"
      case 294: // "CONFIG_VALUE_OFF"
      case 295: // "CONFIG_VALUE_ON"
      case 296: // "CONFIG_VALUE_PARALLEL"
      case 297: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 298: // "CONFIG_VALUE_REJECT"
      case 299: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 300: // "CONFIG_VALUE_SERIAL"
      case 301: // "CONFIG_VALUE_WARN"
      case 302: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 303: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 304: // "CONGIG_DIR_SEC_ARG_SEP"
      case 305: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 306: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 307: // "CONGIG_DIR_SEC_DATA_DIR"
      case 308: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 309: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 310: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 311: // "CONGIG_DIR_SEC_TMP_DIR"
      case 312: // "DIRECTIVE"
      case 313: // "DIRECTIVE_SECRULESCRIPT"
      case 314: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 315: // "QUOTATION_MARK"
      case 316: // "RUN_TIME_VAR_BLD"
      case 317: // "RUN_TIME_VAR_DUR"
      case 318: // "RUN_TIME_VAR_HSV"
      case 319: // "RUN_TIME_VAR_REMOTE_USER"
      case 320: // "RUN_TIME_VAR_TIME"
      case 321: // "RUN_TIME_VAR_TIME_DAY"
      case 322: // "RUN_TIME_VAR_TIME_EPOCH"
      case 323: // "RUN_TIME_VAR_TIME_HOUR"
      case 324: // "RUN_TIME_VAR_TIME_MIN"
      case 325: // "RUN_TIME_VAR_TIME_MON"
      case 326: // "RUN_TIME_VAR_TIME_SEC"
      case 327: // "RUN_TIME_VAR_TIME_WDAY"
      case 328: // "RUN_TIME_VAR_TIME_YEAR"
      case 329: // "VARIABLE"
      case 330: // "Dictionary element"
      case 331: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 338: // op
      case 339: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 346: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 343: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 344: // act
      case 345: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 341: // variables
      case 342: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 336: // actions
      case 337: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  seclang_parser::stack_symbol_type&
  seclang_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 143: // "Accuracy"
      case 144: // "Allow"
      case 145: // "Append"
      case 146: // "AuditLog"
      case 147: // "Block"
      case 148: // "Capture"
      case 149: // "Chain"
      case 150: // "ACTION_CTL_AUDIT_ENGINE"
      case 151: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 152: // "ACTION_CTL_BDY_JSON"
      case 153: // "ACTION_CTL_BDY_XML"
      case 154: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 155: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 156: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 158: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 159: // "Deny"
      case 160: // "DeprecateVar"
      case 161: // "Drop"
      case 162: // "Exec"
      case 163: // "ExpireVar"
      case 164: // "Id"
      case 165: // "InitCol"
      case 166: // "Log"
      case 167: // "LogData"
      case 168: // "Maturity"
      case 169: // "Msg"
      case 170: // "MultiMatch"
      case 171: // "NoAuditLog"
      case 172: // "NoLog"
      case 173: // "Pass"
      case 174: // "Pause"
      case 175: // "Phase"
      case 176: // "Prepend"
      case 177: // "Proxy"
      case 178: // "Redirect"
      case 179: // "Rev"
      case 180: // "SanatiseArg"
      case 181: // "SanatiseMatched"
      case 182: // "SanatiseMatchedBytes"
      case 183: // "SanatiseRequestHeader"
      case 184: // "SanatiseResponseHeader"
      case 185: // "SetEnv"
      case 186: // "SetRsc"
      case 187: // "SetSid"
      case 188: // "SetUID"
      case 189: // "Severity"
      case 190: // "Skip"
      case 191: // "SkipAfter"
      case 192: // "Status"
      case 193: // "Tag"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 196: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 197: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 198: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 199: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 201: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 202: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 203: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_LENGTH"
      case 205: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 206: // "ACTION_TRANSFORMATION_MD5"
      case 207: // "ACTION_TRANSFORMATION_NONE"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 209: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 210: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 216: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 219: // "ACTION_TRANSFORMATION_SHA1"
      case 220: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 221: // "ACTION_TRANSFORMATION_TRIM"
      case 222: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 224: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 225: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 226: // "Ver"
      case 227: // "xmlns"
      case 228: // "CONFIG_COMPONENT_SIG"
      case 229: // "CONFIG_CONN_ENGINE"
      case 230: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 231: // "CONFIG_SEC_WEB_APP_ID"
      case 232: // "CONFIG_SEC_SERVER_SIG"
      case 233: // "CONFIG_DIR_AUDIT_DIR"
      case 234: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 235: // "CONFIG_DIR_AUDIT_ENG"
      case 236: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 237: // "CONFIG_DIR_AUDIT_LOG"
      case 238: // "CONFIG_DIR_AUDIT_LOG2"
      case 239: // "CONFIG_DIR_AUDIT_LOG_P"
      case 240: // "CONFIG_DIR_AUDIT_STS"
      case 241: // "CONFIG_DIR_AUDIT_TPE"
      case 242: // "CONFIG_DIR_DEBUG_LOG"
      case 243: // "CONFIG_DIR_DEBUG_LVL"
      case 244: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 245: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 246: // "CONFIG_SEC_HASH_ENGINE"
      case 247: // "CONFIG_SEC_HASH_KEY"
      case 248: // "CONFIG_SEC_HASH_PARAM"
      case 249: // "CONFIG_SEC_HASH_METHOD_RX"
      case 250: // "CONFIG_SEC_HASH_METHOD_PM"
      case 251: // "CONFIG_SEC_CHROOT_DIR"
      case 252: // "CONFIG_DIR_GEO_DB"
      case 253: // "CONFIG_DIR_GSB_DB"
      case 254: // "CONFIG_SEC_GUARDIAN_LOG"
      case 255: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 256: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 257: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 258: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 259: // "CONFIG_SEC_SENSOR_ID"
      case 260: // "CONFIG_DIR_REQ_BODY"
      case 261: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 262: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 263: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 264: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 265: // "CONFIG_DIR_RES_BODY"
      case 266: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 267: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 268: // "CONFIG_SEC_RULE_INHERITANCE"
      case 269: // "CONFIG_SEC_RULE_PERF_TIME"
      case 270: // "CONFIG_DIR_RULE_ENG"
      case 271: // "CONFIG_DIR_SEC_ACTION"
      case 272: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 273: // "CONFIG_DIR_SEC_MARKER"
      case 274: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 275: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 276: // "CONFIG_SEC_HTTP_BLKEY"
      case 277: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 278: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 279: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 280: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 281: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 282: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 283: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 284: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 285: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 286: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 287: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 288: // "CONFIG_UPLOAD_DIR"
      case 289: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 290: // "CONFIG_UPLOAD_FILE_MODE"
      case 291: // "CONFIG_VALUE_ABORT"
      case 292: // "CONFIG_VALUE_DETC"
      case 293: // "CONFIG_VALUE_HTTPS"
      case 294: // "CONFIG_VALUE_OFF"
      case 295: // "CONFIG_VALUE_ON"
      case 296: // "CONFIG_VALUE_PARALLEL"
      case 297: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 298: // "CONFIG_VALUE_REJECT"
      case 299: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 300: // "CONFIG_VALUE_SERIAL"
      case 301: // "CONFIG_VALUE_WARN"
      case 302: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 303: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 304: // "CONGIG_DIR_SEC_ARG_SEP"
      case 305: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 306: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 307: // "CONGIG_DIR_SEC_DATA_DIR"
      case 308: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 309: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 310: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 311: // "CONGIG_DIR_SEC_TMP_DIR"
      case 312: // "DIRECTIVE"
      case 313: // "DIRECTIVE_SECRULESCRIPT"
      case 314: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 315: // "QUOTATION_MARK"
      case 316: // "RUN_TIME_VAR_BLD"
      case 317: // "RUN_TIME_VAR_DUR"
      case 318: // "RUN_TIME_VAR_HSV"
      case 319: // "RUN_TIME_VAR_REMOTE_USER"
      case 320: // "RUN_TIME_VAR_TIME"
      case 321: // "RUN_TIME_VAR_TIME_DAY"
      case 322: // "RUN_TIME_VAR_TIME_EPOCH"
      case 323: // "RUN_TIME_VAR_TIME_HOUR"
      case 324: // "RUN_TIME_VAR_TIME_MIN"
      case 325: // "RUN_TIME_VAR_TIME_MON"
      case 326: // "RUN_TIME_VAR_TIME_SEC"
      case 327: // "RUN_TIME_VAR_TIME_WDAY"
      case 328: // "RUN_TIME_VAR_TIME_YEAR"
      case 329: // "VARIABLE"
      case 330: // "Dictionary element"
      case 331: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 338: // op
      case 339: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 346: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 343: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 344: // act
      case 345: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 341: // variables
      case 342: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 336: // actions
      case 337: // actions_may_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
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
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  seclang_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  seclang_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  seclang_parser::yypop_ (unsigned int n)
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

  inline seclang_parser::state_type
  seclang_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  seclang_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  seclang_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  seclang_parser::parse ()
  {
    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 351 "/home/zimmerle/core-trustwave/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 857 "seclang-parser.cc" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 143: // "Accuracy"
      case 144: // "Allow"
      case 145: // "Append"
      case 146: // "AuditLog"
      case 147: // "Block"
      case 148: // "Capture"
      case 149: // "Chain"
      case 150: // "ACTION_CTL_AUDIT_ENGINE"
      case 151: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 152: // "ACTION_CTL_BDY_JSON"
      case 153: // "ACTION_CTL_BDY_XML"
      case 154: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 155: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 156: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 158: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 159: // "Deny"
      case 160: // "DeprecateVar"
      case 161: // "Drop"
      case 162: // "Exec"
      case 163: // "ExpireVar"
      case 164: // "Id"
      case 165: // "InitCol"
      case 166: // "Log"
      case 167: // "LogData"
      case 168: // "Maturity"
      case 169: // "Msg"
      case 170: // "MultiMatch"
      case 171: // "NoAuditLog"
      case 172: // "NoLog"
      case 173: // "Pass"
      case 174: // "Pause"
      case 175: // "Phase"
      case 176: // "Prepend"
      case 177: // "Proxy"
      case 178: // "Redirect"
      case 179: // "Rev"
      case 180: // "SanatiseArg"
      case 181: // "SanatiseMatched"
      case 182: // "SanatiseMatchedBytes"
      case 183: // "SanatiseRequestHeader"
      case 184: // "SanatiseResponseHeader"
      case 185: // "SetEnv"
      case 186: // "SetRsc"
      case 187: // "SetSid"
      case 188: // "SetUID"
      case 189: // "Severity"
      case 190: // "Skip"
      case 191: // "SkipAfter"
      case 192: // "Status"
      case 193: // "Tag"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 196: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 197: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 198: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 199: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 201: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 202: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 203: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 204: // "ACTION_TRANSFORMATION_LENGTH"
      case 205: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 206: // "ACTION_TRANSFORMATION_MD5"
      case 207: // "ACTION_TRANSFORMATION_NONE"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 209: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 210: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 216: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 219: // "ACTION_TRANSFORMATION_SHA1"
      case 220: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 221: // "ACTION_TRANSFORMATION_TRIM"
      case 222: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 224: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 225: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 226: // "Ver"
      case 227: // "xmlns"
      case 228: // "CONFIG_COMPONENT_SIG"
      case 229: // "CONFIG_CONN_ENGINE"
      case 230: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 231: // "CONFIG_SEC_WEB_APP_ID"
      case 232: // "CONFIG_SEC_SERVER_SIG"
      case 233: // "CONFIG_DIR_AUDIT_DIR"
      case 234: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 235: // "CONFIG_DIR_AUDIT_ENG"
      case 236: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 237: // "CONFIG_DIR_AUDIT_LOG"
      case 238: // "CONFIG_DIR_AUDIT_LOG2"
      case 239: // "CONFIG_DIR_AUDIT_LOG_P"
      case 240: // "CONFIG_DIR_AUDIT_STS"
      case 241: // "CONFIG_DIR_AUDIT_TPE"
      case 242: // "CONFIG_DIR_DEBUG_LOG"
      case 243: // "CONFIG_DIR_DEBUG_LVL"
      case 244: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 245: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 246: // "CONFIG_SEC_HASH_ENGINE"
      case 247: // "CONFIG_SEC_HASH_KEY"
      case 248: // "CONFIG_SEC_HASH_PARAM"
      case 249: // "CONFIG_SEC_HASH_METHOD_RX"
      case 250: // "CONFIG_SEC_HASH_METHOD_PM"
      case 251: // "CONFIG_SEC_CHROOT_DIR"
      case 252: // "CONFIG_DIR_GEO_DB"
      case 253: // "CONFIG_DIR_GSB_DB"
      case 254: // "CONFIG_SEC_GUARDIAN_LOG"
      case 255: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 256: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 257: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 258: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 259: // "CONFIG_SEC_SENSOR_ID"
      case 260: // "CONFIG_DIR_REQ_BODY"
      case 261: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 262: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 263: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 264: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 265: // "CONFIG_DIR_RES_BODY"
      case 266: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 267: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 268: // "CONFIG_SEC_RULE_INHERITANCE"
      case 269: // "CONFIG_SEC_RULE_PERF_TIME"
      case 270: // "CONFIG_DIR_RULE_ENG"
      case 271: // "CONFIG_DIR_SEC_ACTION"
      case 272: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 273: // "CONFIG_DIR_SEC_MARKER"
      case 274: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 275: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 276: // "CONFIG_SEC_HTTP_BLKEY"
      case 277: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 278: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 279: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 280: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 281: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 282: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 283: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 284: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 285: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 286: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 287: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 288: // "CONFIG_UPLOAD_DIR"
      case 289: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 290: // "CONFIG_UPLOAD_FILE_MODE"
      case 291: // "CONFIG_VALUE_ABORT"
      case 292: // "CONFIG_VALUE_DETC"
      case 293: // "CONFIG_VALUE_HTTPS"
      case 294: // "CONFIG_VALUE_OFF"
      case 295: // "CONFIG_VALUE_ON"
      case 296: // "CONFIG_VALUE_PARALLEL"
      case 297: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 298: // "CONFIG_VALUE_REJECT"
      case 299: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 300: // "CONFIG_VALUE_SERIAL"
      case 301: // "CONFIG_VALUE_WARN"
      case 302: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 303: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 304: // "CONGIG_DIR_SEC_ARG_SEP"
      case 305: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 306: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 307: // "CONGIG_DIR_SEC_DATA_DIR"
      case 308: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 309: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 310: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 311: // "CONGIG_DIR_SEC_TMP_DIR"
      case 312: // "DIRECTIVE"
      case 313: // "DIRECTIVE_SECRULESCRIPT"
      case 314: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 315: // "QUOTATION_MARK"
      case 316: // "RUN_TIME_VAR_BLD"
      case 317: // "RUN_TIME_VAR_DUR"
      case 318: // "RUN_TIME_VAR_HSV"
      case 319: // "RUN_TIME_VAR_REMOTE_USER"
      case 320: // "RUN_TIME_VAR_TIME"
      case 321: // "RUN_TIME_VAR_TIME_DAY"
      case 322: // "RUN_TIME_VAR_TIME_EPOCH"
      case 323: // "RUN_TIME_VAR_TIME_HOUR"
      case 324: // "RUN_TIME_VAR_TIME_MIN"
      case 325: // "RUN_TIME_VAR_TIME_MON"
      case 326: // "RUN_TIME_VAR_TIME_SEC"
      case 327: // "RUN_TIME_VAR_TIME_WDAY"
      case 328: // "RUN_TIME_VAR_TIME_YEAR"
      case 329: // "VARIABLE"
      case 330: // "Dictionary element"
      case 331: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 338: // op
      case 339: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 346: // run_time_string
        yylhs.value.build< std::unique_ptr<RunTimeString> > ();
        break;

      case 343: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 344: // act
      case 345: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 341: // variables
      case 342: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 336: // actions
      case 337: // actions_may_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 733 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1189 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 746 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1205 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1213 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 762 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 766 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1229 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 772 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1237 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 778 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1245 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 784 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1253 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 790 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1261 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 795 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 800 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1277 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 806 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 813 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 817 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 821 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 827 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 831 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 835 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecUploadKeepFiles RelevantOnly is not currently supported. Accepted values are On or Off");
        YYERROR;
      }
#line 1335 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 840 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1344 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 845 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 850 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1362 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 855 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1370 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 859 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1378 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 866 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 870 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1394 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 877 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 883 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 893 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 902 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1442 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 912 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 921 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1469 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 934 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1477 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 938 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1485 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 942 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1493 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 946 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1501 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 950 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 954 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::InspectFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::FuzzyHash(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 962 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 966 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1541 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 975 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 979 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 983 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCPF(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 987 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifySSN(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 991 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 996 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 1001 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 1005 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 1009 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1624 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 1013 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 1017 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1640 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 1021 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 1025 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 1029 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1664 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 1033 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1672 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 1037 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 1041 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 1045 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 1049 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1053 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1057 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1061 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 69:
#line 1065 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1736 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1069 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1744 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 1073 "seclang-parser.yy" // lalr1.cc:859
    {
#ifdef WITH_GEOIP
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::GeoLookup());
#else
        std::stringstream ss;
            ss << "This version of ModSecurity was not compiled with GeoIP support.";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
#endif  // WITH_GEOIP
      }
#line 1759 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1088 "seclang-parser.yy" // lalr1.cc:859
    {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            a->push_back(i.release());
        }
        std::vector<Variable *> *v = new std::vector<Variable *>();
        for (auto &i : *yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ().get()) {
            v->push_back(i.release());
        }

        Operator *op = yystack_[1].value.as< std::unique_ptr<Operator> > ().release();
        Rule *rule = new Rule(
            /* op */ op,
            /* variables */ v,
            /* actions */ a,
            /* file name */ driver.ref.back(),
            /* line number */ yystack_[4].location.end.line
            );
        if (driver.addSecRule(rule) == false) {
            delete rule;
            YYERROR;
        }
      }
#line 1787 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1112 "seclang-parser.yy" // lalr1.cc:859
    {
        std::vector<Variable *> *v = new std::vector<Variable *>();
        for (auto &i : *yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ().get()) {
            v->push_back(i.release());
        }

        Rule *rule = new Rule(
            /* op */ yystack_[0].value.as< std::unique_ptr<Operator> > ().release(),
            /* variables */ v,
            /* actions */ NULL,
            /* file name */ driver.ref.back(),
            /* line number */ yystack_[3].location.end.line
            );
        if (driver.addSecRule(rule) == false) {
            delete rule;
            YYERROR;
        }
      }
#line 1810 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1131 "seclang-parser.yy" // lalr1.cc:859
    {
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            a->push_back(i.release());
        }
        Rule *rule = new Rule(
            /* op */ NULL,
            /* variables */ NULL,
            /* actions */ a,
            /* file name */ driver.ref.back(),
            /* line number */ yystack_[2].location.end.line
            );
        driver.addSecAction(rule);
      }
#line 1829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1146 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string err;
        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            a->push_back(i.release());
        }
        RuleScript *r = new RuleScript(
            /* path to script */ yystack_[1].value.as< std::string > (),
            /* actions */ a,
            /* file name */ driver.ref.back(),
            /* line number */ yystack_[2].location.end.line
            );

        if (r->init(&err) == false) {
            driver.error(yystack_[2].location, "Failed to load script: " + err);
            delete r;
            YYERROR;
        }
        if (driver.addSecRuleScript(r) == false) {
            delete r;
            YYERROR;
        }
      }
#line 1857 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1170 "seclang-parser.yy" // lalr1.cc:859
    {
        bool hasDisruptive = false;
        std::vector<actions::Action *> *actions = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            actions->push_back(i.release());
        }
        std::vector<actions::Action *> checkedActions;
        int definedPhase = -1;
        int secRuleDefinedPhase = -1;
        for (actions::Action *a : *actions) {
            actions::Phase *phase = dynamic_cast<actions::Phase *>(a);
            if (a->isDisruptive() == true && dynamic_cast<actions::disruptive::Block *>(a) == NULL) {
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
                driver.error(yystack_[2].location, "The action '" + a->m_name + "' is not suitable to be part of the SecDefaultActions");
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
            driver.m_defaultActions[definedPhase].push_back(a);
        }

        delete actions;
      }
#line 1917 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1226 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1925 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1230 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1933 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1234 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1238 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1949 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1242 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1957 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1246 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1965 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1250 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1254 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1981 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1258 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 1994 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1267 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 2002 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1271 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecConnEngine is not yet supported.");
        YYERROR;
      }
#line 2011 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1276 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2018 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1279 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secWebAppId.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 2027 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1284 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2036 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1289 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2045 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1294 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecDisableBackendCompression is not supported.");
        YYERROR;
      }
#line 2054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1299 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1302 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecContentInjection is not yet supported.");
        YYERROR;
      }
#line 2070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1307 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2077 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1310 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecChrootDir is not supported.");
        YYERROR;
      }
#line 2086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1315 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecHashEngine is not yet supported.");
        YYERROR;
      }
#line 2095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1320 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1323 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecHashKey is not yet supported.");
        YYERROR;
      }
#line 2111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1328 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecHashParam is not yet supported.");
        YYERROR;
      }
#line 2120 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1333 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecHashMethodRx is not yet supported.");
        YYERROR;
      }
#line 2129 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1338 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecHashMethodPm is not yet supported.");
        YYERROR;
      }
#line 2138 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1343 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecGsbLookupDb is not supported.");
        YYERROR;
      }
#line 2147 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1348 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecGuardianLog is not supported.");
        YYERROR;
      }
#line 2156 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1353 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecInterceptOnError is not yet supported.");
        YYERROR;
      }
#line 2165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1358 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2172 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 1361 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecConnReadStateLimit is not yet supported.");
        YYERROR;
      }
#line 2181 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1366 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecConnWriteStateLimit is not yet supported.");
        YYERROR;
      }
#line 2190 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1371 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecSensorId is not yet supported.");
        YYERROR;
      }
#line 2199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1376 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecRuleInheritance is not yet supported.");
        YYERROR;
      }
#line 2208 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1381 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1384 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecRulePerfTime is not yet supported.");
        YYERROR;
      }
#line 2224 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1389 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecStreamInBodyInspection is not supported.");
        YYERROR;
      }
#line 2233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1394 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecStreamOutBodyInspection is not supported.");
        YYERROR;
      }
#line 2242 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1399 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        if (driver.m_exceptions.load(yystack_[0].value.as< std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveById: failed to load:";
            ss << yystack_[0].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2259 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1412 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        if (driver.m_exceptions.loadRemoveRuleByTag(yystack_[0].value.as< std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveByTag: failed to load:";
            ss << yystack_[0].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2276 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1425 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        if (driver.m_exceptions.loadRemoveRuleByMsg(yystack_[0].value.as< std::string > (), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleRemoveByMsg: failed to load:";
            ss << yystack_[0].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1438 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        if (driver.m_exceptions.loadUpdateTargetByTag(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetByTag: failed to load:";
            ss << yystack_[1].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1451 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        if (driver.m_exceptions.loadUpdateTargetByMsg(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetByMsg: failed to load:";
            ss << yystack_[1].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1464 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        double ruleId;
        try {
            ruleId = std::stod(yystack_[1].value.as< std::string > ());
        } catch (...) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetById: failed to load:";
            ss << "The input \"" + yystack_[1].value.as< std::string > () + "\" does not ";
            ss << "seems to be a valid rule id.";
            ss << ". ";
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }

        if (driver.m_exceptions.loadUpdateTargetById(ruleId, std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()), &error) == false) {
            std::stringstream ss;
            ss << "SecRuleUpdateTargetById: failed to load:";
            ss << yystack_[1].value.as< std::string > ();
            ss << ". ";
            ss << error;
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }
      }
#line 2357 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1490 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string error;
        double ruleId;
        try {
            ruleId = std::stod(yystack_[1].value.as< std::string > ());
        } catch (...) {
            std::stringstream ss;
            ss << "SecRuleUpdateActionById: failed to load:";
            ss << "The input \"" + yystack_[1].value.as< std::string > () + "\" does not ";
            ss << "seems to be a valid rule id.";
            ss << ". ";
            driver.error(yystack_[2].location, ss.str());
            YYERROR;
        }


        std::vector<actions::Action *> *a = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
            a->push_back(i.release());
        }

        driver.error(yystack_[2].location, "SecRuleUpdateActionById is not yet supported");
        YYERROR;
      }
#line 2386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1516 "seclang-parser.yy" // lalr1.cc:859
    {
        if (driver.m_debugLog != NULL) {
          driver.m_debugLog->setDebugLogLevel(atoi(yystack_[0].value.as< std::string > ().c_str()));
        } else {
            std::stringstream ss;
            ss << "Internal error, there is no DebugLog ";
            ss << "object associated with the driver class";
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
      }
#line 2402 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1528 "seclang-parser.yy" // lalr1.cc:859
    {
        if (driver.m_debugLog != NULL) {
            std::string error;
            driver.m_debugLog->setDebugLogFile(yystack_[0].value.as< std::string > (), &error);
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
#line 2425 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1548 "seclang-parser.yy" // lalr1.cc:859
    {
#ifdef WITH_GEOIP
        std::string err;
        std::string file = modsecurity::utils::find_resource(yystack_[0].value.as< std::string > (),
            driver.ref.back(), &err);
        if (file.empty()) {
            std::stringstream ss;
            ss << "Failed to load locate the GeoDB file from: " << yystack_[0].value.as< std::string > () << " ";
            ss << err;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
        if (GeoLookup::getInstance().setDataBase(file, &err) == false) {
            std::stringstream ss;
            ss << "Failed to load the GeoDB from: ";
            ss << file << ". " << err;
            driver.error(yystack_[1].location, ss.str());
            YYERROR;
        }
#else
        std::stringstream ss;
        ss << "This version of ModSecurity was not compiled with GeoIP support.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
#endif  // WITH_GEOIP
      }
#line 2456 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1576 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1581 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2474 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1586 "seclang-parser.yy" // lalr1.cc:859
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1595 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2496 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1600 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2504 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1604 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1608 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2520 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1612 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2528 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1616 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2536 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1620 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2544 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1634 "seclang-parser.yy" // lalr1.cc:859
    {
        std::istringstream buf(yystack_[0].value.as< std::string > ());
        std::istream_iterator<std::string> beg(buf), end;
        std::set<std::string> tokens(beg, end);
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        for (std::set<std::string>::iterator it=tokens.begin();
            it!=tokens.end(); ++it)
        {
            driver.m_responseBodyTypeToBeInspected.m_value.insert(*it);
        }
      }
#line 2560 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1646 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2570 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1652 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2578 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1656 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2586 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1660 "seclang-parser.yy" // lalr1.cc:859
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
#line 2601 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1681 "seclang-parser.yy" // lalr1.cc:859
    {
        if (atoi(yystack_[0].value.as< std::string > ().c_str()) == 1) {
          driver.error(yystack_[1].location, "SecCookieFormat 1 is not yet supported.");
          YYERROR;
        }
      }
#line 2612 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1688 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCookieV0Separator is not yet supported.");
        YYERROR;
      }
#line 2621 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1703 "seclang-parser.yy" // lalr1.cc:859
    {
/* Parser error disabled to avoid breaking default CRS installations with crs-setup.conf-recommended
        driver.error(@0, "SecCollectionTimeout is not yet supported.");
        YYERROR;
*/
      }
#line 2632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1710 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1718 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2649 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1722 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2657 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1729 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2666 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1734 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2676 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1740 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2686 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1746 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1752 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1759 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2718 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1769 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2726 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1773 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2734 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1777 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1781 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2750 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1785 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2758 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1789 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2766 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1793 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2774 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1797 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2782 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1801 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1805 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2798 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1809 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2806 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1813 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1817 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2822 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1821 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1825 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2838 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1829 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1833 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1837 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1841 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2870 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1845 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1849 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1853 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1857 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1861 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1865 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1869 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1873 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1877 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1881 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1885 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1889 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2966 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1893 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2974 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1897 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2982 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1901 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2990 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1905 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2998 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1909 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 3006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1913 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3014 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1917 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1921 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 3030 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1925 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3038 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1929 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3046 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1933 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 3054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1937 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1941 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1945 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 3078 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1949 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1953 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1957 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 3102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1961 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1965 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1969 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 3126 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1973 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 3134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1977 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 3142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1981 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 3150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1985 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 3158 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1989 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 3166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1993 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 3174 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1997 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3182 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 2001 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3190 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 2005 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 3198 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 2009 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 2013 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3214 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 2017 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 2021 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 3230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 2025 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3238 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 2029 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 2033 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 2037 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 3262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 2041 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 2045 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 2049 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 2053 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 3294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 2057 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 2061 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 2065 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 2069 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_NoDictElement());
      }
#line 3326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 2073 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3334 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 2077 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2081 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2085 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 3358 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2089 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3366 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2093 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2097 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3382 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2101 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3390 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2105 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3398 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2109 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3406 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2113 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3414 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2117 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2121 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2125 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2130 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2134 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3454 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2138 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3462 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2143 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2147 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2151 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2156 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2161 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2165 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2169 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2173 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2177 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2181 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2185 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2189 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2193 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2197 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2201 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2205 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryQuoted());
      }
#line 3590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2209 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryWhiteSpace());
      }
#line 3598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2213 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3606 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2217 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3614 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2221 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateBefore());
      }
#line 3622 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2225 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2229 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2233 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3646 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2237 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidPart());
      }
#line 3654 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2241 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3662 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2245 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartLFLine());
      }
#line 3670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2249 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartMissingSemicolon());
      }
#line 3678 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2253 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartMissingSemicolon());
      }
#line 3686 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2257 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3694 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2261 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3702 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2265 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3710 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2269 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3718 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2273 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3726 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2277 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3734 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2281 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2285 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3750 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2289 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3758 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2293 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3766 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2297 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3774 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2301 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3782 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2305 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2309 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3798 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2313 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3806 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2317 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2321 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3822 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2325 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2329 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3838 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2333 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2337 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2341 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2345 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3870 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2349 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2353 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2357 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2361 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2365 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2369 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2373 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2377 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2381 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2385 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2389 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2393 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::WebAppId());
      }
#line 3966 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2397 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3977 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2405 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3988 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2412 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3999 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2419 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4010 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2426 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4021 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2433 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4032 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2440 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4043 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2447 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2454 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4065 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2461 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4076 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2468 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2475 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4098 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2482 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 4109 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2492 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 4117 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2496 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 4125 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2500 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 4133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2504 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4141 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2508 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 4149 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2512 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 4157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2516 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 4165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2520 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 4174 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2525 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 4183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2530 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 4192 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2535 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 4200 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2539 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 4208 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2543 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 4216 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2547 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 4225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2552 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 4234 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2557 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 4242 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2561 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 4250 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2565 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 4258 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2569 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 4266 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2573 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 4274 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2577 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 4282 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2581 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 4290 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2585 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 4298 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2589 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 4306 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2593 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 4314 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2597 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4323 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2602 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 4331 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2606 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4340 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2611 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 4348 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2615 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4356 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2619 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4364 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2623 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 4372 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2627 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 4380 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2631 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4388 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2635 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4396 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2639 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 2643 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4412 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 2647 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4420 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 2651 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 2655 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4436 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 2659 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 2663 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 2667 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4460 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 2671 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4468 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 2675 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 373:
#line 2679 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4484 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 374:
#line 2683 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4492 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 375:
#line 2687 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 2691 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4508 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 2695 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4516 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 2699 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4524 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 2703 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4532 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 2707 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 2711 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 2715 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 383:
#line 2719 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4564 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 384:
#line 2723 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4572 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 385:
#line 2727 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4580 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 386:
#line 2731 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4588 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 387:
#line 2735 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4596 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 388:
#line 2739 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4604 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 389:
#line 2743 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4612 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 390:
#line 2747 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4620 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 391:
#line 2751 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4628 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 392:
#line 2755 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4636 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 393:
#line 2759 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Encode(yystack_[0].value.as< std::string > ()));
      }
#line 4644 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 394:
#line 2763 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Decode(yystack_[0].value.as< std::string > ()));
      }
#line 4652 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 395:
#line 2767 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64DecodeExt(yystack_[0].value.as< std::string > ()));
      }
#line 4660 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 396:
#line 2771 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4668 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 397:
#line 2775 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4676 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 398:
#line 2779 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4684 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 399:
#line 2783 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4692 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 400:
#line 2787 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4700 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 401:
#line 2791 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4708 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 402:
#line 2795 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4716 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 403:
#line 2799 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4724 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 404:
#line 2803 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4732 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 405:
#line 2807 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4740 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 406:
#line 2811 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4748 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 407:
#line 2815 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4756 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 408:
#line 2819 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 409:
#line 2823 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4772 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 410:
#line 2827 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4780 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 411:
#line 2831 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4788 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 412:
#line 2835 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 413:
#line 2839 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4804 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 414:
#line 2843 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4812 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 415:
#line 2847 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4820 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 416:
#line 2851 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4828 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 417:
#line 2855 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4836 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 418:
#line 2859 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4844 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 419:
#line 2863 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4852 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 420:
#line 2867 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4860 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 421:
#line 2874 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
      }
#line 4868 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 422:
#line 2878 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
      }
#line 4876 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 423:
#line 2882 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4884 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 424:
#line 2886 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4892 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 425:
#line 2890 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4900 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 426:
#line 2897 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4909 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 427:
#line 2902 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 428:
#line 2907 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4928 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 429:
#line 2913 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4938 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4942 "seclang-parser.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
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
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

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
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  seclang_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  seclang_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
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
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
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
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
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


  const short int seclang_parser::yypact_ninf_ = -375;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
    2680,  -375,  -274,  -375,   -91,  -375,  -243,  -375,  -375,  -375,
    -375,  -375,  -264,  -375,  -375,  -375,  -375,  -375,  -271,  -375,
    -375,  -375,  -153,  -149,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -147,  -375,  -375,
    -145,  -375,  -140,  -375,  -141,  -136,  -375,  -280,   -87,   -87,
    -375,  -375,  -375,  -375,  -129,  -288,  -375,  -375,  -375,  1442,
    1442,  1442,   -87,  -258,  -127,  -375,  -375,  -375,  -125,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  1442,   -87,
    2832,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  2260,  -250,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -256,  -375,  -375,  -375,  -123,  -121,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  2389,  -375,  2389,
    -375,  2389,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    2389,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  2389,  2389,
    2389,  -375,  -375,  -375,  -375,  2389,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    3025,  -375,     7,  -375,  -375,  -375,  -375,  -375,  -375,  2579,
    2579,  -155,  -152,  -150,  -148,  -146,  -144,  -142,  -139,  -135,
    -133,  -131,  -128,  -126,  -124,  -122,  -120,  -375,  -118,  -116,
    -114,  -112,  -375,  -375,  -110,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -108,  -375,  -375,  -375,  -375,  -375,   444,  -375,  -375,  -375,
    -106,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,   532,   620,   942,  1030,  1118,  -104,  -101,  1532,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,    18,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  1941,  -375,  -375,  -375,
    2579,   -53,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  2477,  2477,  2477,  2477,  2477,
    2477,  2477,  2477,     5,  3025,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
    -375,  -375,  -375,  2477,  -375,  -375,  -375,  -375,  2477,  -375,
    -375,  2477,  -375,  -375,  2477,  -375,  -375,  2477,  -375,  -375,
    2477,  -375,  -375,  -375,  -375,    10,  1622,  2070,  2389,  2389,
    2389,  -375,  -375,  2389,  2389,  2389,  -375,  2389,  2389,  2389,
    2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,
    2389,  2389,  -375,  2389,  2389,  2389,  2389,  -375,  -375,  2389,
    2389,  2389,  2389,   -87,  -375,  2477,  -375,  2389,  2389,  2389,
    -375,  -375,  -375,  -375,  -375,  2579,  2579,  -375,  -375,  2477,
    2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,
    2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,
    2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  2477,  -375,
    2477,  2477,  2477,  -375,  -375
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   139,     0,    87,     0,    86,    90,    91,
       7,     6,     0,    11,    14,    12,    13,    17,     0,   124,
     123,    92,     0,     0,   100,   101,   102,   103,    97,   125,
     104,   105,   137,   136,   108,   109,   110,     0,   128,   126,
       0,   127,     0,   129,     0,     0,   113,     0,     0,     0,
      78,   148,   149,   150,     0,     0,   116,   118,   117,     0,
       0,     0,     0,     0,     0,    26,    24,    25,     0,   138,
     144,   145,   146,   143,   147,   114,   115,   142,     0,     0,
       0,     4,    72,     5,    96,    95,    15,    16,    89,    88,
       9,    10,     8,    20,    19,    18,    94,    93,    99,    98,
      83,    82,   130,   131,    85,    84,   132,   133,   112,   111,
      81,    79,    80,     0,     0,   328,   329,   330,   331,   332,
     333,   334,     0,   338,   339,   340,     0,     0,   348,   349,
     350,   351,   352,   353,   354,   355,   356,     0,   359,     0,
     360,     0,   362,   363,   364,   365,   366,   367,   368,   369,
       0,   371,   372,   373,   374,   375,   376,   377,     0,     0,
       0,   382,   383,   384,   385,     0,   393,   394,   395,   396,
     406,   412,   399,   400,   410,   411,   416,   401,   398,   405,
     415,   414,   391,   390,   389,   419,   418,   409,   407,   420,
     408,   397,   392,   413,   402,   404,   403,   417,   387,   388,
       0,    75,    30,    32,    77,   107,   106,   134,   135,     0,
       0,   161,   164,   167,   170,   173,   176,   179,   182,   185,
     188,   191,   194,   197,   200,   203,   206,   259,   248,   209,
     245,   251,   260,   261,   218,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   289,   288,   292,   291,   290,   293,   295,   294,   296,
     254,   297,   298,   299,   301,   300,   222,   302,   303,   255,
     258,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     314,   313,   226,   230,   238,   242,   234,   212,   215,     0,
     316,   315,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   119,   151,   156,   120,   121,   122,    22,
      21,    23,    28,    27,   140,   141,     0,    76,     1,     3,
       0,   422,   381,   347,   346,   345,   336,   335,   337,   342,
     341,   344,   343,   428,   429,   357,   358,   361,   370,   378,
     379,   380,   386,     0,     0,   158,   157,   159,   160,   162,
     163,   165,   166,   168,   169,   171,   172,   174,   175,   177,
     178,   180,   181,   183,   184,   186,   187,   189,   190,   192,
     193,   195,   196,   198,   199,   201,   202,   204,   205,   246,
     247,   207,   208,   243,   244,   249,   250,   216,   217,   252,
     253,   220,   221,   219,   256,   257,   224,   225,   223,   228,
     229,   227,   236,   237,   235,   240,   241,   239,   232,   233,
     231,   210,   211,   213,   214,     0,     0,     0,     0,     0,
       0,    38,    39,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,    74,    33,    35,   421,     0,     0,     0,
     426,   427,    29,    31,   152,     0,     0,   153,    34,    36,
      70,    55,    54,    56,    57,    43,    58,    51,    59,    42,
      60,    61,    62,    63,    64,    65,    66,    52,    67,    68,
      69,    44,    45,    46,    47,    48,    49,    50,    53,    73,
     423,   424,   425,   155,   154
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -375,  -375,   -48,  -375,   -45,  -160,  -375,  -374,  -375,   -55,
    -156,   -59,  -205,  -375,  -132
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    80,    81,    82,   201,   202,   463,   464,    83,   313,
     314,   344,   203,   332,   345
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     315,   315,   315,   207,   204,   316,   317,   346,   354,   347,
     354,   113,   110,   208,   111,   112,   426,   318,   348,   315,
      84,    85,    93,   326,   426,    94,   349,   350,   351,    95,
      90,    91,   329,   352,   327,    92,   319,   320,   336,   337,
     353,   321,   333,   338,   334,   335,   467,   468,   469,    86,
      87,    88,    89,   478,   331,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    96,    97,   425,   403,    98,    99,   100,   101,   473,
     355,   356,   102,   103,   104,   105,   106,   107,   108,   109,
     408,   411,   414,   417,   420,   205,   206,   322,   323,   324,
     325,   339,   340,   341,   342,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
       0,   371,   372,     0,   465,   373,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   404,   405,   421,   422,   200,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,   471,   471,   471,
     471,   471,   471,   471,     0,   479,   480,   481,   482,     0,
       0,   483,   484,   485,     0,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     472,   501,   502,   503,   504,   474,     0,   505,   506,   507,
     508,     0,     0,     0,     0,   510,   511,   512,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,   471,
       0,     0,   471,     0,     0,   471,     0,     0,   471,     0,
       0,   471,     0,     0,     0,     0,     0,   477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   509,     0,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
       0,   471,   471,   471,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,     0,   401,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,   406,   407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,     0,
     409,   410,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,   412,   413,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,     0,
     415,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,     0,   418,   419,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     475,   476,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,     0,
       0,     0,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,     0,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,     0,     0,     0,
       1,     0,     0,     0,     2,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   470,     0,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     2,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,     4,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      59,    60,    61,   291,    49,    60,    61,   139,     3,   141,
       3,    98,   292,   301,   294,   295,     6,    62,   150,    78,
     294,   295,   293,    78,     6,   296,   158,   159,   160,   300,
     294,   295,    80,   165,    79,   299,   294,   295,   294,   295,
     200,   299,   292,   299,   294,   295,    99,   100,   101,   140,
     141,   294,   295,   427,   113,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   294,   295,   299,   276,   294,   295,   294,   295,   354,
     209,   210,   297,   298,   294,   295,   297,   298,   294,   295,
     292,   293,   294,   295,   296,   294,   295,   294,   295,   294,
     295,   294,   295,   294,   295,   330,   331,    -1,   330,   331,
     330,   331,   330,   331,   330,   331,   330,   331,   330,   331,
      -1,   330,   331,    -1,   326,   330,   331,   330,   331,   330,
     331,    -1,   330,   331,   330,   331,   330,   331,   330,   331,
     330,   331,   330,   331,   330,   331,   330,   331,   330,   331,
     330,   331,   330,   331,   330,   331,   330,   331,   315,   330,
     331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   345,   346,   347,   348,
     349,   350,   351,   352,    -1,   427,   428,   429,   430,    -1,
      -1,   433,   434,   435,    -1,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     315,   453,   454,   455,   456,   315,    -1,   459,   460,   461,
     462,    -1,    -1,    -1,    -1,   467,   468,   469,    -1,    -1,
      -1,    -1,    -1,    -1,   403,    -1,    -1,    -1,    -1,   408,
      -1,    -1,   411,    -1,    -1,   414,    -1,    -1,   417,    -1,
      -1,   420,    -1,    -1,    -1,    -1,    -1,   426,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   465,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   475,   476,   463,    -1,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
      -1,   510,   511,   512,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,    -1,   330,   331,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    -1,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,    -1,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    -1,
     330,   331,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    -1,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,    -1,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    -1,
     330,   331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,    -1,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,    -1,   330,   331,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   314,    -1,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,    -1,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
       0,    -1,    -1,    -1,     4,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,    -1,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,    -1,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,    -1,    -1,    -1,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   139,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     333,   334,   335,   340,   294,   295,   140,   141,   294,   295,
     294,   295,   299,   293,   296,   300,   294,   295,   294,   295,
     294,   295,   297,   298,   294,   295,   297,   298,   294,   295,
     292,   294,   295,    98,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     315,   336,   337,   344,   336,   294,   295,   291,   301,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   341,   342,   343,   341,   341,   336,   294,
     295,   299,   294,   295,   294,   295,   341,   336,     0,   334,
     102,   343,   345,   292,   294,   295,   294,   295,   299,   294,
     295,   294,   295,   314,   343,   346,   346,   346,   346,   346,
     346,   346,   346,   337,     3,   343,   343,   330,   331,   330,
     331,   330,   331,   330,   331,   330,   331,   330,   331,   330,
     331,   330,   331,   330,   331,   330,   331,   330,   331,   330,
     331,   330,   331,   330,   331,   330,   331,   330,   331,   330,
     331,   330,   331,   330,   331,   330,   331,   330,   331,   330,
     331,   330,   331,   346,   330,   331,   330,   331,   346,   330,
     331,   346,   330,   331,   346,   330,   331,   346,   330,   331,
     346,   330,   331,   330,   331,   342,     6,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   338,   339,   346,   343,    99,   100,   101,
     314,   343,   315,   344,   315,     8,     9,   343,   339,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   336,
     346,   346,   346,   343,   343
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   332,   333,   333,   333,   334,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   336,
     336,   337,   337,   338,   338,   338,   338,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   341,   341,   342,   342,   342,   342,   342,   342,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   345,   346,   346,   346,   346
  };

  const unsigned char
  seclang_parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     2,     2,     3,
       1,     3,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     4,     3,     2,     2,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     2,     2,
       1,     1,     1,     2,     2,     2,     2,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     4,     4,     1,     2,     2,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     2,     1,     2,     2,     2,     1,     2,     2,     2,
       1,     2,     2,     2,     1,     2,     2,     2,     1,     2,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     2,     1,     1,     1,
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
  "\"VARIABLE_STATUS\"", "\"VARIABLE_IP\"", "\"VARIABLE_GLOBAL\"",
  "\"VARIABLE_TX\"", "\"VARIABLE_SESSION\"", "\"VARIABLE_USER\"",
  "\"RUN_TIME_VAR_ENV\"", "\"RUN_TIME_VAR_XML\"", "\"SetVar\"",
  "SETVAR_OPERATION_EQUALS", "SETVAR_OPERATION_EQUALS_PLUS",
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
  "\"OPERATOR_WITHIN\"", "CONFIG_DIR_AUDIT_LOG_FMT", "JSON", "NATIVE",
  "\"ACTION_CTL_RULE_ENGINE\"", "\"Accuracy\"", "\"Allow\"", "\"Append\"",
  "\"AuditLog\"", "\"Block\"", "\"Capture\"", "\"Chain\"",
  "\"ACTION_CTL_AUDIT_ENGINE\"", "\"ACTION_CTL_AUDIT_LOG_PARTS\"",
  "\"ACTION_CTL_BDY_JSON\"", "\"ACTION_CTL_BDY_XML\"",
  "\"ACTION_CTL_FORCE_REQ_BODY_VAR\"",
  "\"ACTION_CTL_REQUEST_BODY_ACCESS\"", "\"ACTION_CTL_RULE_REMOVE_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG\"", "\"Deny\"",
  "\"DeprecateVar\"", "\"Drop\"", "\"Exec\"", "\"ExpireVar\"", "\"Id\"",
  "\"InitCol\"", "\"Log\"", "\"LogData\"", "\"Maturity\"", "\"Msg\"",
  "\"MultiMatch\"", "\"NoAuditLog\"", "\"NoLog\"", "\"Pass\"", "\"Pause\"",
  "\"Phase\"", "\"Prepend\"", "\"Proxy\"", "\"Redirect\"", "\"Rev\"",
  "\"SanatiseArg\"", "\"SanatiseMatched\"", "\"SanatiseMatchedBytes\"",
  "\"SanatiseRequestHeader\"", "\"SanatiseResponseHeader\"", "\"SetEnv\"",
  "\"SetRsc\"", "\"SetSid\"", "\"SetUID\"", "\"Severity\"", "\"Skip\"",
  "\"SkipAfter\"", "\"Status\"", "\"Tag\"",
  "\"ACTION_TRANSFORMATION_BASE_64_ENCODE\"",
  "\"ACTION_TRANSFORMATION_BASE_64_DECODE\"",
  "\"ACTION_TRANSFORMATION_BASE_64_DECODE_EXT\"",
  "\"ACTION_TRANSFORMATION_CMD_LINE\"",
  "\"ACTION_TRANSFORMATION_COMPRESS_WHITESPACE\"",
  "\"ACTION_TRANSFORMATION_CSS_DECODE\"",
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
  "\"ACTION_TRANSFORMATION_TRIM\"", "\"ACTION_TRANSFORMATION_UPPERCASE\"",
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
  "\"CONFIG_SEC_SENSOR_ID\"", "\"CONFIG_DIR_REQ_BODY\"",
  "\"CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT\"",
  "\"CONFIG_DIR_REQ_BODY_LIMIT\"", "\"CONFIG_DIR_REQ_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT\"", "\"CONFIG_DIR_RES_BODY\"",
  "\"CONFIG_DIR_RES_BODY_LIMIT\"", "\"CONFIG_DIR_RES_BODY_LIMIT_ACTION\"",
  "\"CONFIG_SEC_RULE_INHERITANCE\"", "\"CONFIG_SEC_RULE_PERF_TIME\"",
  "\"CONFIG_DIR_RULE_ENG\"", "\"CONFIG_DIR_SEC_ACTION\"",
  "\"CONFIG_DIR_SEC_DEFAULT_ACTION\"", "\"CONFIG_DIR_SEC_MARKER\"",
  "\"CONFIG_DIR_UNICODE_MAP_FILE\"", "\"CONFIG_SEC_COLLECTION_TIMEOUT\"",
  "\"CONFIG_SEC_HTTP_BLKEY\"", "\"CONFIG_SEC_INTERCEPT_ON_ERROR\"",
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
  "expression", "variables", "variables_may_be_quoted", "var", "act",
  "setvar_action", "run_time_string", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   732,   732,   736,   737,   740,   745,   751,   757,   761,
     765,   771,   777,   783,   789,   794,   799,   805,   812,   816,
     820,   826,   830,   834,   839,   844,   849,   854,   858,   865,
     869,   876,   882,   892,   901,   911,   920,   933,   937,   941,
     945,   949,   953,   957,   961,   965,   969,   974,   978,   982,
     986,   990,   995,  1000,  1004,  1008,  1012,  1016,  1020,  1024,
    1028,  1032,  1036,  1040,  1044,  1048,  1052,  1056,  1060,  1064,
    1068,  1072,  1086,  1087,  1111,  1130,  1145,  1169,  1225,  1229,
    1233,  1237,  1241,  1245,  1249,  1253,  1257,  1266,  1270,  1275,
    1278,  1283,  1288,  1293,  1298,  1301,  1306,  1309,  1314,  1319,
    1322,  1327,  1332,  1337,  1342,  1347,  1352,  1357,  1360,  1365,
    1370,  1375,  1380,  1383,  1388,  1393,  1398,  1411,  1424,  1437,
    1450,  1463,  1489,  1515,  1527,  1547,  1575,  1580,  1585,  1594,
    1599,  1603,  1607,  1611,  1615,  1619,  1623,  1628,  1633,  1645,
    1651,  1655,  1659,  1670,  1679,  1680,  1687,  1692,  1697,  1702,
    1709,  1717,  1721,  1728,  1733,  1739,  1745,  1751,  1758,  1768,
    1772,  1776,  1780,  1784,  1788,  1792,  1796,  1800,  1804,  1808,
    1812,  1816,  1820,  1824,  1828,  1832,  1836,  1840,  1844,  1848,
    1852,  1856,  1860,  1864,  1868,  1872,  1876,  1880,  1884,  1888,
    1892,  1896,  1900,  1904,  1908,  1912,  1916,  1920,  1924,  1928,
    1932,  1936,  1940,  1944,  1948,  1952,  1956,  1960,  1964,  1968,
    1972,  1976,  1980,  1984,  1988,  1992,  1996,  2000,  2004,  2008,
    2012,  2016,  2020,  2024,  2028,  2032,  2036,  2040,  2044,  2048,
    2052,  2056,  2060,  2064,  2068,  2072,  2076,  2080,  2084,  2088,
    2092,  2096,  2100,  2104,  2108,  2112,  2116,  2120,  2124,  2129,
    2133,  2137,  2142,  2146,  2150,  2155,  2160,  2164,  2168,  2172,
    2176,  2180,  2184,  2188,  2192,  2196,  2200,  2204,  2208,  2212,
    2216,  2220,  2224,  2228,  2232,  2236,  2240,  2244,  2248,  2252,
    2256,  2260,  2264,  2268,  2272,  2276,  2280,  2284,  2288,  2292,
    2296,  2300,  2304,  2308,  2312,  2316,  2320,  2324,  2328,  2332,
    2336,  2340,  2344,  2348,  2352,  2356,  2360,  2364,  2368,  2372,
    2376,  2380,  2384,  2388,  2392,  2396,  2404,  2411,  2418,  2425,
    2432,  2439,  2446,  2453,  2460,  2467,  2474,  2481,  2491,  2495,
    2499,  2503,  2507,  2511,  2515,  2519,  2524,  2529,  2534,  2538,
    2542,  2546,  2551,  2556,  2560,  2564,  2568,  2572,  2576,  2580,
    2584,  2588,  2592,  2596,  2601,  2605,  2610,  2614,  2618,  2622,
    2626,  2630,  2634,  2638,  2642,  2646,  2650,  2654,  2658,  2662,
    2666,  2670,  2674,  2678,  2682,  2686,  2690,  2694,  2698,  2702,
    2706,  2710,  2714,  2718,  2722,  2726,  2730,  2734,  2738,  2742,
    2746,  2750,  2754,  2758,  2762,  2766,  2770,  2774,  2778,  2782,
    2786,  2790,  2794,  2798,  2802,  2806,  2810,  2814,  2818,  2822,
    2826,  2830,  2834,  2838,  2842,  2846,  2850,  2854,  2858,  2862,
    2866,  2873,  2877,  2881,  2885,  2889,  2896,  2901,  2906,  2912
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
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  seclang_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 6398 "seclang-parser.cc" // lalr1.cc:1167
#line 2919 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
