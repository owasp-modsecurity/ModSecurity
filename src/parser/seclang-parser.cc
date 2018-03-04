// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

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

#line 37 "seclang-parser.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "seclang-parser.hh"

// User implementation prologue.

#line 51 "seclang-parser.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 358 "seclang-parser.yy" // lalr1.cc:408

#include "src/parser/driver.h"

#line 57 "seclang-parser.cc" // lalr1.cc:408


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
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 143 "seclang-parser.cc" // lalr1.cc:474

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
    : state (empty)
  {}

  inline
  seclang_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  seclang_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  seclang_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  seclang_parser::symbol_number_type
  seclang_parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
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
      case 142: // "Accuracy"
      case 143: // "Allow"
      case 144: // "Append"
      case 145: // "AuditLog"
      case 146: // "Block"
      case 147: // "Capture"
      case 148: // "Chain"
      case 149: // "ACTION_CTL_AUDIT_ENGINE"
      case 150: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 151: // "ACTION_CTL_BDY_JSON"
      case 152: // "ACTION_CTL_BDY_XML"
      case 153: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 154: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 155: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 156: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 158: // "Deny"
      case 159: // "DeprecateVar"
      case 160: // "Drop"
      case 161: // "Exec"
      case 162: // "ExpireVar"
      case 163: // "Id"
      case 164: // "InitCol"
      case 165: // "Log"
      case 166: // "LogData"
      case 167: // "Maturity"
      case 168: // "Msg"
      case 169: // "MultiMatch"
      case 170: // "NoAuditLog"
      case 171: // "NoLog"
      case 172: // "Pass"
      case 173: // "Pause"
      case 174: // "Phase"
      case 175: // "Prepend"
      case 176: // "Proxy"
      case 177: // "Redirect"
      case 178: // "Rev"
      case 179: // "SanatiseArg"
      case 180: // "SanatiseMatched"
      case 181: // "SanatiseMatchedBytes"
      case 182: // "SanatiseRequestHeader"
      case 183: // "SanatiseResponseHeader"
      case 184: // "SetEnv"
      case 185: // "SetRsc"
      case 186: // "SetSid"
      case 187: // "SetUID"
      case 188: // "Severity"
      case 189: // "Skip"
      case 190: // "SkipAfter"
      case 191: // "Status"
      case 192: // "Tag"
      case 193: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 196: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 197: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 198: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 199: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 201: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 202: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_LENGTH"
      case 204: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 205: // "ACTION_TRANSFORMATION_MD5"
      case 206: // "ACTION_TRANSFORMATION_NONE"
      case 207: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 209: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 210: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 216: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 218: // "ACTION_TRANSFORMATION_SHA1"
      case 219: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 220: // "ACTION_TRANSFORMATION_TRIM"
      case 221: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 222: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 224: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 225: // "Ver"
      case 226: // "xmlns"
      case 227: // "CONFIG_COMPONENT_SIG"
      case 228: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 229: // "CONFIG_SEC_WEB_APP_ID"
      case 230: // "CONFIG_SEC_SERVER_SIG"
      case 231: // "CONFIG_DIR_AUDIT_DIR"
      case 232: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 233: // "CONFIG_DIR_AUDIT_ENG"
      case 234: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 235: // "CONFIG_DIR_AUDIT_LOG"
      case 236: // "CONFIG_DIR_AUDIT_LOG2"
      case 237: // "CONFIG_DIR_AUDIT_LOG_P"
      case 238: // "CONFIG_DIR_AUDIT_STS"
      case 239: // "CONFIG_DIR_AUDIT_TPE"
      case 240: // "CONFIG_DIR_DEBUG_LOG"
      case 241: // "CONFIG_DIR_DEBUG_LVL"
      case 242: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 243: // "CONFIG_DIR_GEO_DB"
      case 244: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 245: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 246: // "CONFIG_DIR_REQ_BODY"
      case 247: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 248: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 249: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 250: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 251: // "CONFIG_DIR_RES_BODY"
      case 252: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 253: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 254: // "CONFIG_DIR_RULE_ENG"
      case 255: // "CONFIG_DIR_SEC_ACTION"
      case 256: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 257: // "CONFIG_DIR_SEC_MARKER"
      case 258: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 259: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 260: // "CONFIG_SEC_HTTP_BLKEY"
      case 261: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 262: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 263: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 265: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 266: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 267: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 268: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 269: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 270: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 271: // "CONFIG_UPLOAD_DIR"
      case 272: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 273: // "CONFIG_UPLOAD_FILE_MODE"
      case 274: // "CONFIG_VALUE_ABORT"
      case 275: // "CONFIG_VALUE_DETC"
      case 276: // "CONFIG_VALUE_HTTPS"
      case 277: // "CONFIG_VALUE_OFF"
      case 278: // "CONFIG_VALUE_ON"
      case 279: // "CONFIG_VALUE_PARALLEL"
      case 280: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 281: // "CONFIG_VALUE_REJECT"
      case 282: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 283: // "CONFIG_VALUE_SERIAL"
      case 284: // "CONFIG_VALUE_WARN"
      case 285: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 286: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 287: // "CONGIG_DIR_SEC_ARG_SEP"
      case 288: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 289: // "CONGIG_DIR_SEC_DATA_DIR"
      case 290: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 291: // "CONGIG_DIR_SEC_TMP_DIR"
      case 292: // "DIRECTIVE"
      case 293: // "DIRECTIVE_SECRULESCRIPT"
      case 294: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 295: // "QUOTATION_MARK"
      case 296: // "RUN_TIME_VAR_BLD"
      case 297: // "RUN_TIME_VAR_DUR"
      case 298: // "RUN_TIME_VAR_HSV"
      case 299: // "RUN_TIME_VAR_REMOTE_USER"
      case 300: // "RUN_TIME_VAR_TIME"
      case 301: // "RUN_TIME_VAR_TIME_DAY"
      case 302: // "RUN_TIME_VAR_TIME_EPOCH"
      case 303: // "RUN_TIME_VAR_TIME_HOUR"
      case 304: // "RUN_TIME_VAR_TIME_MIN"
      case 305: // "RUN_TIME_VAR_TIME_MON"
      case 306: // "RUN_TIME_VAR_TIME_SEC"
      case 307: // "RUN_TIME_VAR_TIME_WDAY"
      case 308: // "RUN_TIME_VAR_TIME_YEAR"
      case 309: // "VARIABLE"
      case 310: // "Dictionary element"
      case 311: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 318: // op
      case 319: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 326: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 323: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 324: // act
      case 325: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 321: // variables
      case 322: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 316: // actions
      case 317: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
  seclang_parser::stack_symbol_type&
  seclang_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 142: // "Accuracy"
      case 143: // "Allow"
      case 144: // "Append"
      case 145: // "AuditLog"
      case 146: // "Block"
      case 147: // "Capture"
      case 148: // "Chain"
      case 149: // "ACTION_CTL_AUDIT_ENGINE"
      case 150: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 151: // "ACTION_CTL_BDY_JSON"
      case 152: // "ACTION_CTL_BDY_XML"
      case 153: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 154: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 155: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 156: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 158: // "Deny"
      case 159: // "DeprecateVar"
      case 160: // "Drop"
      case 161: // "Exec"
      case 162: // "ExpireVar"
      case 163: // "Id"
      case 164: // "InitCol"
      case 165: // "Log"
      case 166: // "LogData"
      case 167: // "Maturity"
      case 168: // "Msg"
      case 169: // "MultiMatch"
      case 170: // "NoAuditLog"
      case 171: // "NoLog"
      case 172: // "Pass"
      case 173: // "Pause"
      case 174: // "Phase"
      case 175: // "Prepend"
      case 176: // "Proxy"
      case 177: // "Redirect"
      case 178: // "Rev"
      case 179: // "SanatiseArg"
      case 180: // "SanatiseMatched"
      case 181: // "SanatiseMatchedBytes"
      case 182: // "SanatiseRequestHeader"
      case 183: // "SanatiseResponseHeader"
      case 184: // "SetEnv"
      case 185: // "SetRsc"
      case 186: // "SetSid"
      case 187: // "SetUID"
      case 188: // "Severity"
      case 189: // "Skip"
      case 190: // "SkipAfter"
      case 191: // "Status"
      case 192: // "Tag"
      case 193: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 196: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 197: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 198: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 199: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 201: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 202: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_LENGTH"
      case 204: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 205: // "ACTION_TRANSFORMATION_MD5"
      case 206: // "ACTION_TRANSFORMATION_NONE"
      case 207: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 209: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 210: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 216: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 218: // "ACTION_TRANSFORMATION_SHA1"
      case 219: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 220: // "ACTION_TRANSFORMATION_TRIM"
      case 221: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 222: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 224: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 225: // "Ver"
      case 226: // "xmlns"
      case 227: // "CONFIG_COMPONENT_SIG"
      case 228: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 229: // "CONFIG_SEC_WEB_APP_ID"
      case 230: // "CONFIG_SEC_SERVER_SIG"
      case 231: // "CONFIG_DIR_AUDIT_DIR"
      case 232: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 233: // "CONFIG_DIR_AUDIT_ENG"
      case 234: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 235: // "CONFIG_DIR_AUDIT_LOG"
      case 236: // "CONFIG_DIR_AUDIT_LOG2"
      case 237: // "CONFIG_DIR_AUDIT_LOG_P"
      case 238: // "CONFIG_DIR_AUDIT_STS"
      case 239: // "CONFIG_DIR_AUDIT_TPE"
      case 240: // "CONFIG_DIR_DEBUG_LOG"
      case 241: // "CONFIG_DIR_DEBUG_LVL"
      case 242: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 243: // "CONFIG_DIR_GEO_DB"
      case 244: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 245: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 246: // "CONFIG_DIR_REQ_BODY"
      case 247: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 248: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 249: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 250: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 251: // "CONFIG_DIR_RES_BODY"
      case 252: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 253: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 254: // "CONFIG_DIR_RULE_ENG"
      case 255: // "CONFIG_DIR_SEC_ACTION"
      case 256: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 257: // "CONFIG_DIR_SEC_MARKER"
      case 258: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 259: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 260: // "CONFIG_SEC_HTTP_BLKEY"
      case 261: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 262: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 263: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 265: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 266: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 267: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 268: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 269: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 270: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 271: // "CONFIG_UPLOAD_DIR"
      case 272: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 273: // "CONFIG_UPLOAD_FILE_MODE"
      case 274: // "CONFIG_VALUE_ABORT"
      case 275: // "CONFIG_VALUE_DETC"
      case 276: // "CONFIG_VALUE_HTTPS"
      case 277: // "CONFIG_VALUE_OFF"
      case 278: // "CONFIG_VALUE_ON"
      case 279: // "CONFIG_VALUE_PARALLEL"
      case 280: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 281: // "CONFIG_VALUE_REJECT"
      case 282: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 283: // "CONFIG_VALUE_SERIAL"
      case 284: // "CONFIG_VALUE_WARN"
      case 285: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 286: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 287: // "CONGIG_DIR_SEC_ARG_SEP"
      case 288: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 289: // "CONGIG_DIR_SEC_DATA_DIR"
      case 290: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 291: // "CONGIG_DIR_SEC_TMP_DIR"
      case 292: // "DIRECTIVE"
      case 293: // "DIRECTIVE_SECRULESCRIPT"
      case 294: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 295: // "QUOTATION_MARK"
      case 296: // "RUN_TIME_VAR_BLD"
      case 297: // "RUN_TIME_VAR_DUR"
      case 298: // "RUN_TIME_VAR_HSV"
      case 299: // "RUN_TIME_VAR_REMOTE_USER"
      case 300: // "RUN_TIME_VAR_TIME"
      case 301: // "RUN_TIME_VAR_TIME_DAY"
      case 302: // "RUN_TIME_VAR_TIME_EPOCH"
      case 303: // "RUN_TIME_VAR_TIME_HOUR"
      case 304: // "RUN_TIME_VAR_TIME_MIN"
      case 305: // "RUN_TIME_VAR_TIME_MON"
      case 306: // "RUN_TIME_VAR_TIME_SEC"
      case 307: // "RUN_TIME_VAR_TIME_WDAY"
      case 308: // "RUN_TIME_VAR_TIME_YEAR"
      case 309: // "VARIABLE"
      case 310: // "Dictionary element"
      case 311: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 318: // op
      case 319: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 326: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 323: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 324: // act
      case 325: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 321: // variables
      case 322: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 316: // actions
      case 317: // actions_may_quoted
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
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

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
    #line 351 "/root/ModSecurity-v3-Mar/src/parser/seclang-parser.yy" // lalr1.cc:725
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 808 "seclang-parser.cc" // lalr1.cc:725

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
    if (yyempty)
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
        yyempty = false;
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

    // Discard the token being shifted.
    yyempty = true;

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
      case 142: // "Accuracy"
      case 143: // "Allow"
      case 144: // "Append"
      case 145: // "AuditLog"
      case 146: // "Block"
      case 147: // "Capture"
      case 148: // "Chain"
      case 149: // "ACTION_CTL_AUDIT_ENGINE"
      case 150: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 151: // "ACTION_CTL_BDY_JSON"
      case 152: // "ACTION_CTL_BDY_XML"
      case 153: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 154: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 155: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 156: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 157: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 158: // "Deny"
      case 159: // "DeprecateVar"
      case 160: // "Drop"
      case 161: // "Exec"
      case 162: // "ExpireVar"
      case 163: // "Id"
      case 164: // "InitCol"
      case 165: // "Log"
      case 166: // "LogData"
      case 167: // "Maturity"
      case 168: // "Msg"
      case 169: // "MultiMatch"
      case 170: // "NoAuditLog"
      case 171: // "NoLog"
      case 172: // "Pass"
      case 173: // "Pause"
      case 174: // "Phase"
      case 175: // "Prepend"
      case 176: // "Proxy"
      case 177: // "Redirect"
      case 178: // "Rev"
      case 179: // "SanatiseArg"
      case 180: // "SanatiseMatched"
      case 181: // "SanatiseMatchedBytes"
      case 182: // "SanatiseRequestHeader"
      case 183: // "SanatiseResponseHeader"
      case 184: // "SetEnv"
      case 185: // "SetRsc"
      case 186: // "SetSid"
      case 187: // "SetUID"
      case 188: // "Severity"
      case 189: // "Skip"
      case 190: // "SkipAfter"
      case 191: // "Status"
      case 192: // "Tag"
      case 193: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 194: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 195: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 196: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 197: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 198: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 199: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 200: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 201: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 202: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_LENGTH"
      case 204: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 205: // "ACTION_TRANSFORMATION_MD5"
      case 206: // "ACTION_TRANSFORMATION_NONE"
      case 207: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 208: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 209: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 210: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 211: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 213: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 214: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 216: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 217: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 218: // "ACTION_TRANSFORMATION_SHA1"
      case 219: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 220: // "ACTION_TRANSFORMATION_TRIM"
      case 221: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 222: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 223: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 224: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 225: // "Ver"
      case 226: // "xmlns"
      case 227: // "CONFIG_COMPONENT_SIG"
      case 228: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 229: // "CONFIG_SEC_WEB_APP_ID"
      case 230: // "CONFIG_SEC_SERVER_SIG"
      case 231: // "CONFIG_DIR_AUDIT_DIR"
      case 232: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 233: // "CONFIG_DIR_AUDIT_ENG"
      case 234: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 235: // "CONFIG_DIR_AUDIT_LOG"
      case 236: // "CONFIG_DIR_AUDIT_LOG2"
      case 237: // "CONFIG_DIR_AUDIT_LOG_P"
      case 238: // "CONFIG_DIR_AUDIT_STS"
      case 239: // "CONFIG_DIR_AUDIT_TPE"
      case 240: // "CONFIG_DIR_DEBUG_LOG"
      case 241: // "CONFIG_DIR_DEBUG_LVL"
      case 242: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 243: // "CONFIG_DIR_GEO_DB"
      case 244: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 245: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 246: // "CONFIG_DIR_REQ_BODY"
      case 247: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 248: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 249: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 250: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 251: // "CONFIG_DIR_RES_BODY"
      case 252: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 253: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 254: // "CONFIG_DIR_RULE_ENG"
      case 255: // "CONFIG_DIR_SEC_ACTION"
      case 256: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 257: // "CONFIG_DIR_SEC_MARKER"
      case 258: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 259: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 260: // "CONFIG_SEC_HTTP_BLKEY"
      case 261: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 262: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 263: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 265: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 266: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 267: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 268: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 269: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 270: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 271: // "CONFIG_UPLOAD_DIR"
      case 272: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 273: // "CONFIG_UPLOAD_FILE_MODE"
      case 274: // "CONFIG_VALUE_ABORT"
      case 275: // "CONFIG_VALUE_DETC"
      case 276: // "CONFIG_VALUE_HTTPS"
      case 277: // "CONFIG_VALUE_OFF"
      case 278: // "CONFIG_VALUE_ON"
      case 279: // "CONFIG_VALUE_PARALLEL"
      case 280: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 281: // "CONFIG_VALUE_REJECT"
      case 282: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 283: // "CONFIG_VALUE_SERIAL"
      case 284: // "CONFIG_VALUE_WARN"
      case 285: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 286: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 287: // "CONGIG_DIR_SEC_ARG_SEP"
      case 288: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 289: // "CONGIG_DIR_SEC_DATA_DIR"
      case 290: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 291: // "CONGIG_DIR_SEC_TMP_DIR"
      case 292: // "DIRECTIVE"
      case 293: // "DIRECTIVE_SECRULESCRIPT"
      case 294: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 295: // "QUOTATION_MARK"
      case 296: // "RUN_TIME_VAR_BLD"
      case 297: // "RUN_TIME_VAR_DUR"
      case 298: // "RUN_TIME_VAR_HSV"
      case 299: // "RUN_TIME_VAR_REMOTE_USER"
      case 300: // "RUN_TIME_VAR_TIME"
      case 301: // "RUN_TIME_VAR_TIME_DAY"
      case 302: // "RUN_TIME_VAR_TIME_EPOCH"
      case 303: // "RUN_TIME_VAR_TIME_HOUR"
      case 304: // "RUN_TIME_VAR_TIME_MIN"
      case 305: // "RUN_TIME_VAR_TIME_MON"
      case 306: // "RUN_TIME_VAR_TIME_SEC"
      case 307: // "RUN_TIME_VAR_TIME_WDAY"
      case 308: // "RUN_TIME_VAR_TIME_YEAR"
      case 309: // "VARIABLE"
      case 310: // "Dictionary element"
      case 311: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 318: // op
      case 319: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 326: // run_time_string
        yylhs.value.build< std::unique_ptr<RunTimeString> > ();
        break;

      case 323: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 324: // act
      case 325: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 321: // variables
      case 322: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 316: // actions
      case 317: // actions_may_quoted
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
#line 713 "seclang-parser.yy" // lalr1.cc:847
    {
        return 0;
      }
#line 1125 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 726 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1133 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 732 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1141 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 738 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1149 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 742 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1157 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 746 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1165 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 752 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1173 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 758 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1181 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 764 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1189 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 770 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1197 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 775 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1205 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 780 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1213 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 786 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1222 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 793 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1230 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 797 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1238 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 801 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1246 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 807 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1254 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 811 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1262 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 815 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1271 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 820 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1280 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 25:
#line 825 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1289 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 26:
#line 830 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1297 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 27:
#line 834 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1305 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 28:
#line 841 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1313 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 29:
#line 845 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1321 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 30:
#line 852 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1331 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 31:
#line 858 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1342 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 32:
#line 868 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1355 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 877 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1369 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 887 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1382 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 35:
#line 896 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1396 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 36:
#line 909 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1404 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 913 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1412 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 917 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1420 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 39:
#line 921 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1428 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 40:
#line 925 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1436 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 41:
#line 929 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::InspectFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1444 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 42:
#line 933 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::FuzzyHash(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1452 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 43:
#line 937 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1460 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 44:
#line 941 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1468 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 45:
#line 945 "seclang-parser.yy" // lalr1.cc:847
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1477 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 46:
#line 950 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1485 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 47:
#line 954 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1493 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 48:
#line 958 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCPF(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1501 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 49:
#line 962 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifySSN(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1509 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 50:
#line 966 "seclang-parser.yy" // lalr1.cc:847
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1518 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 51:
#line 971 "seclang-parser.yy" // lalr1.cc:847
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1527 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 52:
#line 976 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1535 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 53:
#line 980 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1543 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 54:
#line 984 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1551 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 55:
#line 988 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1559 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 56:
#line 992 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1567 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 57:
#line 996 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1575 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 58:
#line 1000 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 59:
#line 1004 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1591 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 60:
#line 1008 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1599 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 61:
#line 1012 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1607 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 62:
#line 1016 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1615 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 63:
#line 1020 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1623 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 64:
#line 1024 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1631 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 65:
#line 1028 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1639 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 66:
#line 1032 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1647 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 67:
#line 1036 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1655 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 68:
#line 1040 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1663 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 69:
#line 1044 "seclang-parser.yy" // lalr1.cc:847
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1671 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 70:
#line 1048 "seclang-parser.yy" // lalr1.cc:847
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
#line 1686 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 72:
#line 1063 "seclang-parser.yy" // lalr1.cc:847
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
#line 1714 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 73:
#line 1087 "seclang-parser.yy" // lalr1.cc:847
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
#line 1737 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 74:
#line 1106 "seclang-parser.yy" // lalr1.cc:847
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
#line 1756 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 75:
#line 1121 "seclang-parser.yy" // lalr1.cc:847
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
#line 1784 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 76:
#line 1145 "seclang-parser.yy" // lalr1.cc:847
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
#line 1844 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 77:
#line 1201 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1852 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 78:
#line 1205 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1860 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 79:
#line 1209 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1868 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 80:
#line 1213 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1876 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 81:
#line 1217 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1884 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 82:
#line 1221 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1892 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 83:
#line 1225 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1900 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 84:
#line 1229 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1908 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 85:
#line 1233 "seclang-parser.yy" // lalr1.cc:847
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 1921 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 86:
#line 1242 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1929 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 87:
#line 1246 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secWebAppId.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 1938 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 88:
#line 1251 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 1947 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 89:
#line 1256 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1956 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 90:
#line 1261 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 1965 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 91:
#line 1266 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1974 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 92:
#line 1271 "seclang-parser.yy" // lalr1.cc:847
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
#line 1991 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 93:
#line 1284 "seclang-parser.yy" // lalr1.cc:847
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
#line 2008 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 94:
#line 1297 "seclang-parser.yy" // lalr1.cc:847
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
#line 2025 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 95:
#line 1310 "seclang-parser.yy" // lalr1.cc:847
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
#line 2042 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 96:
#line 1323 "seclang-parser.yy" // lalr1.cc:847
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
#line 2059 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 97:
#line 1336 "seclang-parser.yy" // lalr1.cc:847
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
#line 2089 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 98:
#line 1362 "seclang-parser.yy" // lalr1.cc:847
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
#line 2118 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 99:
#line 1388 "seclang-parser.yy" // lalr1.cc:847
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
#line 2134 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 100:
#line 1400 "seclang-parser.yy" // lalr1.cc:847
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
#line 2157 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 101:
#line 1420 "seclang-parser.yy" // lalr1.cc:847
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
        if (GeoLookup::getInstance().setDataBase(file) == false) {
            std::stringstream ss;
            ss << "Failed to load the GeoDB from: ";
            ss << file;
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
#line 2188 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 102:
#line 1448 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2197 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 103:
#line 1453 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2206 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 104:
#line 1458 "seclang-parser.yy" // lalr1.cc:847
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2219 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 105:
#line 1467 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2228 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 106:
#line 1472 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2236 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 107:
#line 1476 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2244 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 108:
#line 1480 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2252 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 109:
#line 1484 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2260 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 110:
#line 1488 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2268 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 111:
#line 1492 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2276 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 114:
#line 1498 "seclang-parser.yy" // lalr1.cc:847
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
#line 2292 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 115:
#line 1510 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2302 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 116:
#line 1516 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2310 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 117:
#line 1520 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2318 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 124:
#line 1530 "seclang-parser.yy" // lalr1.cc:847
    {
      }
#line 2325 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 125:
#line 1533 "seclang-parser.yy" // lalr1.cc:847
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2334 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 126:
#line 1541 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2342 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 127:
#line 1545 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2350 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 128:
#line 1552 "seclang-parser.yy" // lalr1.cc:847
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2359 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 129:
#line 1557 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2369 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 130:
#line 1563 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2379 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 131:
#line 1569 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2389 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 132:
#line 1575 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2400 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 133:
#line 1582 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2411 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 134:
#line 1592 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2419 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 135:
#line 1596 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2427 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 136:
#line 1600 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2435 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 137:
#line 1604 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2443 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 138:
#line 1608 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2451 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 139:
#line 1612 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2459 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 140:
#line 1616 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2467 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 141:
#line 1620 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2475 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 142:
#line 1624 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2483 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 143:
#line 1628 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2491 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 144:
#line 1632 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2499 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 145:
#line 1636 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2507 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 146:
#line 1640 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2515 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 147:
#line 1644 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2523 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 148:
#line 1648 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2531 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 149:
#line 1652 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2539 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 150:
#line 1656 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2547 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 151:
#line 1660 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2555 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 152:
#line 1664 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2563 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 153:
#line 1668 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2571 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 154:
#line 1672 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2579 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 155:
#line 1676 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2587 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 156:
#line 1680 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2595 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 157:
#line 1684 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2603 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 158:
#line 1688 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2611 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 159:
#line 1692 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2619 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 160:
#line 1696 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2627 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 161:
#line 1700 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2635 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 162:
#line 1704 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2643 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 163:
#line 1708 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2651 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 164:
#line 1712 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2659 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 165:
#line 1716 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2667 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 166:
#line 1720 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2675 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 167:
#line 1724 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2683 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 168:
#line 1728 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2691 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 169:
#line 1732 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2699 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 170:
#line 1736 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2707 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 171:
#line 1740 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2715 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 172:
#line 1744 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2723 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 173:
#line 1748 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2731 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 174:
#line 1752 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2739 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 175:
#line 1756 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2747 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 176:
#line 1760 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2755 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 177:
#line 1764 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2763 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 178:
#line 1768 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2771 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 179:
#line 1772 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2779 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 180:
#line 1776 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2787 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 181:
#line 1780 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2795 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 182:
#line 1784 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2803 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 183:
#line 1788 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2811 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 184:
#line 1792 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2819 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 185:
#line 1796 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2827 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 186:
#line 1800 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2835 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 187:
#line 1804 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2843 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 188:
#line 1808 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2851 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 189:
#line 1812 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2859 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 190:
#line 1816 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2867 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 191:
#line 1820 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2875 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 192:
#line 1824 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2883 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 193:
#line 1828 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2891 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 194:
#line 1832 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 2899 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 195:
#line 1836 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2907 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 196:
#line 1840 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2915 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 197:
#line 1844 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 2923 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 198:
#line 1848 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 2931 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 199:
#line 1852 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2939 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 200:
#line 1856 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2947 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 201:
#line 1860 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 2955 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 202:
#line 1864 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 2963 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 203:
#line 1868 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2971 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 204:
#line 1872 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2979 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 205:
#line 1876 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 2987 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 206:
#line 1880 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 2995 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 207:
#line 1884 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3003 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 208:
#line 1888 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3011 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 209:
#line 1892 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::User_NoDictElement());
      }
#line 3019 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 210:
#line 1896 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3027 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 211:
#line 1900 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3035 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 212:
#line 1904 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3043 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 213:
#line 1908 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 3051 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 214:
#line 1912 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DynamicElement(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3059 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 215:
#line 1916 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3067 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 216:
#line 1920 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3075 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 217:
#line 1924 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3083 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 218:
#line 1928 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3091 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 219:
#line 1932 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3099 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 220:
#line 1936 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3107 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 221:
#line 1940 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3115 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 222:
#line 1944 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3123 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 223:
#line 1948 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3131 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 224:
#line 1953 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3139 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 225:
#line 1957 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3147 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 226:
#line 1961 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3155 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 227:
#line 1966 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3163 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 228:
#line 1970 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3171 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 229:
#line 1974 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3179 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 230:
#line 1979 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3187 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 231:
#line 1984 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3195 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 232:
#line 1988 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3203 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 233:
#line 1992 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3211 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 234:
#line 1996 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3219 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 235:
#line 2000 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3227 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 236:
#line 2004 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3235 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 237:
#line 2008 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3243 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 238:
#line 2012 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3251 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 239:
#line 2016 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3259 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 240:
#line 2020 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3267 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 241:
#line 2024 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3275 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 242:
#line 2028 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryQuoted());
      }
#line 3283 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 243:
#line 2032 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryWhiteSpace());
      }
#line 3291 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 244:
#line 2036 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3299 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 245:
#line 2040 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3307 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 246:
#line 2044 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateBefore());
      }
#line 3315 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 247:
#line 2048 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3323 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 248:
#line 2052 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3331 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 249:
#line 2056 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3339 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 250:
#line 2060 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidPart());
      }
#line 3347 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 251:
#line 2064 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3355 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 252:
#line 2068 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartLFLine());
      }
#line 3363 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 253:
#line 2072 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartMissingSemicolon());
      }
#line 3371 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 254:
#line 2076 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3379 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 255:
#line 2080 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3387 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 256:
#line 2084 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3395 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 257:
#line 2088 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3403 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 258:
#line 2092 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3411 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 259:
#line 2096 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3419 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 260:
#line 2100 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3427 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 261:
#line 2104 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3435 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 262:
#line 2108 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3443 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 263:
#line 2112 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3451 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 264:
#line 2116 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3459 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 265:
#line 2120 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3467 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 266:
#line 2124 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3475 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 267:
#line 2128 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3483 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 268:
#line 2132 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3491 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 269:
#line 2136 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3499 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 270:
#line 2140 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3507 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 271:
#line 2144 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3515 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 272:
#line 2148 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3523 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 273:
#line 2152 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3531 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 274:
#line 2156 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3539 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 275:
#line 2160 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3547 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 276:
#line 2164 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3555 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 277:
#line 2168 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3563 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 278:
#line 2172 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3571 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 279:
#line 2176 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3579 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 280:
#line 2180 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3587 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 281:
#line 2184 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3595 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 282:
#line 2188 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3603 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 283:
#line 2192 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3611 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 284:
#line 2196 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3619 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 285:
#line 2200 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3627 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 286:
#line 2204 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3635 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 287:
#line 2208 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3643 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 288:
#line 2212 "seclang-parser.yy" // lalr1.cc:847
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::WebAppId());
      }
#line 3651 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 289:
#line 2216 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3662 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 290:
#line 2224 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3673 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 291:
#line 2231 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3684 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 292:
#line 2238 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3695 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 293:
#line 2245 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3706 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 294:
#line 2252 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3717 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 295:
#line 2259 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3728 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 296:
#line 2266 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3739 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 297:
#line 2273 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3750 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 298:
#line 2280 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3761 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 299:
#line 2287 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3772 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 300:
#line 2294 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3783 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 301:
#line 2301 "seclang-parser.yy" // lalr1.cc:847
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3794 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 302:
#line 2311 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3802 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 303:
#line 2315 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3810 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 304:
#line 2319 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3818 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 305:
#line 2323 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3826 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 306:
#line 2327 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3834 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 307:
#line 2331 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3842 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 308:
#line 2335 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3850 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 309:
#line 2339 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3859 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 310:
#line 2344 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3868 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 311:
#line 2349 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3877 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 312:
#line 2354 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3885 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 313:
#line 2358 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3893 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 314:
#line 2362 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3901 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 315:
#line 2366 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3910 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 316:
#line 2371 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3919 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 317:
#line 2376 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3927 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 318:
#line 2380 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3935 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 319:
#line 2384 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3943 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 320:
#line 2388 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3951 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 321:
#line 2392 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3959 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 322:
#line 2396 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3967 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 323:
#line 2400 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3975 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 324:
#line 2404 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3983 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 325:
#line 2408 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3991 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 326:
#line 2412 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3999 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 327:
#line 2416 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4008 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 328:
#line 2421 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 4016 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 329:
#line 2425 "seclang-parser.yy" // lalr1.cc:847
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4025 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 330:
#line 2430 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 4033 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 331:
#line 2434 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4041 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 332:
#line 2438 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4049 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 333:
#line 2442 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 4057 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 334:
#line 2446 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 4065 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 335:
#line 2450 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4073 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 336:
#line 2454 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4081 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 337:
#line 2458 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4089 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 338:
#line 2462 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4097 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 339:
#line 2466 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4105 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 340:
#line 2470 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4113 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 341:
#line 2474 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4121 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 342:
#line 2478 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4129 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 343:
#line 2482 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4137 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 344:
#line 2486 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4145 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 345:
#line 2490 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4153 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 346:
#line 2494 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4161 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 347:
#line 2498 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4169 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 348:
#line 2502 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4177 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 349:
#line 2506 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4185 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 350:
#line 2510 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4193 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 351:
#line 2514 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4201 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 352:
#line 2518 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4209 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 353:
#line 2522 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4217 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 354:
#line 2526 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4225 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 355:
#line 2530 "seclang-parser.yy" // lalr1.cc:847
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4233 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 356:
#line 2534 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4241 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 357:
#line 2538 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4249 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 358:
#line 2542 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4257 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 359:
#line 2546 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4265 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 360:
#line 2550 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4273 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 361:
#line 2554 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4281 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 362:
#line 2558 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4289 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 363:
#line 2562 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4297 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 364:
#line 2566 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4305 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 365:
#line 2570 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4313 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 366:
#line 2574 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4321 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 367:
#line 2578 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Encode(yystack_[0].value.as< std::string > ()));
      }
#line 4329 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 368:
#line 2582 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Decode(yystack_[0].value.as< std::string > ()));
      }
#line 4337 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 369:
#line 2586 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64DecodeExt(yystack_[0].value.as< std::string > ()));
      }
#line 4345 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 370:
#line 2590 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4353 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 371:
#line 2594 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4361 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 372:
#line 2598 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4369 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 373:
#line 2602 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4377 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 374:
#line 2606 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4385 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 375:
#line 2610 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4393 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 376:
#line 2614 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4401 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 377:
#line 2618 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4409 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 378:
#line 2622 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4417 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 379:
#line 2626 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4425 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 380:
#line 2630 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4433 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 381:
#line 2634 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4441 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 382:
#line 2638 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4449 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 383:
#line 2642 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4457 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 384:
#line 2646 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4465 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 385:
#line 2650 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4473 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 386:
#line 2654 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4481 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 387:
#line 2658 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4489 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 388:
#line 2662 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4497 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 389:
#line 2666 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4505 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 390:
#line 2670 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4513 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 391:
#line 2674 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4521 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 392:
#line 2678 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4529 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 393:
#line 2682 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4537 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 394:
#line 2686 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4545 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 395:
#line 2693 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
      }
#line 4553 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 396:
#line 2697 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
      }
#line 4561 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 397:
#line 2701 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4569 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 398:
#line 2705 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4577 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 399:
#line 2709 "seclang-parser.yy" // lalr1.cc:847
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, std::move(yystack_[2].value.as< std::unique_ptr<Variable> > ()), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4585 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 400:
#line 2716 "seclang-parser.yy" // lalr1.cc:847
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4594 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 401:
#line 2721 "seclang-parser.yy" // lalr1.cc:847
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4603 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 402:
#line 2726 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4613 "seclang-parser.cc" // lalr1.cc:847
    break;

  case 403:
#line 2732 "seclang-parser.yy" // lalr1.cc:847
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4623 "seclang-parser.cc" // lalr1.cc:847
    break;


#line 4627 "seclang-parser.cc" // lalr1.cc:847
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
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
    if (!yyempty)
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
        if (!yyempty)
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
  seclang_parser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
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
       - The only way there can be no lookahead present (in yytoken) is
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
    if (yytoken != yyempty_)
      {
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


  const short int seclang_parser::yypact_ninf_ = -359;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
    2555,  -359,  -265,  -359,     3,  -359,  -359,  -359,  -359,  -359,
    -359,  -241,  -359,  -359,  -359,  -359,  -359,  -244,  -359,  -359,
    -359,  -359,  -359,  -359,  -231,  -359,  -359,  -136,  -359,  -131,
    -359,  -132,  -256,   -93,   -93,  -359,  -359,  -359,  -359,  -264,
    -359,  -359,  -359,  1383,  1383,  1383,   -93,  -127,  -125,  -359,
    -359,  -359,  -123,  -359,  -359,  -359,  -359,  -359,  -359,  1383,
     -93,  2691,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  2159,  -233,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -143,  -359,  -359,  -359,  -115,
    -113,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    2287,  -359,  2287,  -359,  2287,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  2287,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  2287,  2287,  2287,  -359,  -359,  -359,  -359,  2287,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  2868,  -359,    27,  -359,  -359,  -359,  -359,
    2586,  2586,  -144,  -142,  -140,  -138,  -135,  -133,  -130,  -128,
    -126,  -124,  -122,  -119,  -117,  -114,  -112,  -110,  -359,  -107,
    -104,  -102,  -100,  -359,  -359,   -98,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
     -96,  -359,  -359,  -359,  -359,  -359,   429,  -359,  -359,  -359,
     -94,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,   516,   603,   905,   992,  1079,   -92,   -90,  1472,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,     2,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  1860,  -359,  -359,  -359,  2586,
     -75,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  2458,  2458,  2458,  2458,  2458,  2458,
    2458,  2458,     0,  2868,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  2458,  -359,  -359,  -359,  -359,  2458,  -359,  -359,
    2458,  -359,  -359,  2458,  -359,  -359,  2458,  -359,  -359,  2458,
    -359,  -359,  -359,  -359,     5,  1561,  1988,  2287,  2287,  2287,
    -359,  -359,  2287,  2287,  2287,  -359,  2287,  2287,  2287,  2287,
    2287,  2287,  2287,  2287,  2287,  2287,  2287,  2287,  2287,  2287,
    2287,  -359,  2287,  2287,  2287,  2287,  -359,  -359,  2287,  2287,
    2287,  2287,   -93,  -359,  2458,  -359,  2287,  2287,  2287,  -359,
    -359,  -359,  -359,  -359,  2586,  2586,  -359,  -359,  2458,  2458,
    2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,
    2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,
    2458,  2458,  2458,  2458,  2458,  2458,  2458,  2458,  -359,  2458,
    2458,  2458,  -359,  -359
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   115,     0,    86,    85,    87,    88,     7,
       6,     0,    11,    14,    12,    13,    17,     0,   100,    99,
      90,   101,   113,   112,     0,   104,   102,     0,   103,     0,
     105,     0,     0,     0,     0,    77,   123,   124,   125,     0,
      92,    94,    93,     0,     0,     0,     0,     0,     0,    25,
      23,    24,     0,   114,   120,   121,   119,   122,   118,     0,
       0,     0,     4,    71,     5,    91,    89,    15,    16,     9,
      10,     8,    20,    19,    18,    82,    81,   106,   107,    84,
      83,   108,   109,    80,    78,    79,     0,     0,   302,   303,
     304,   305,   306,   307,   308,     0,   312,   313,   314,     0,
       0,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   333,     0,   334,     0,   336,   337,   338,   339,   340,
     341,   342,   343,     0,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,   356,   357,   358,   359,     0,   367,
     368,   369,   370,   380,   386,   373,   374,   384,   385,   390,
     375,   372,   379,   389,   388,   365,   364,   363,   393,   392,
     383,   381,   394,   382,   371,   366,   387,   376,   378,   377,
     391,   361,   362,     0,    74,    29,    31,    76,   110,   111,
       0,     0,   136,   139,   142,   145,   148,   151,   154,   157,
     160,   163,   166,   169,   172,   175,   178,   181,   234,   223,
     184,   220,   226,   235,   236,   193,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   263,   262,   266,   265,   264,   267,   269,   268,   270,
     229,   271,   272,   273,   275,   274,   197,   276,   277,   230,
     233,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     288,   287,   201,   205,   213,   217,   209,   187,   190,     0,
     290,   289,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,    95,   126,   131,    96,    97,    98,    22,
      21,    27,    26,   116,   117,     0,    75,     1,     3,     0,
     396,   355,   321,   320,   319,   310,   309,   311,   316,   315,
     318,   317,   402,   403,   331,   332,   335,   344,   352,   353,
     354,   360,     0,     0,   133,   132,   134,   135,   137,   138,
     140,   141,   143,   144,   146,   147,   149,   150,   152,   153,
     155,   156,   158,   159,   161,   162,   164,   165,   167,   168,
     170,   171,   173,   174,   176,   177,   179,   180,   221,   222,
     182,   183,   218,   219,   224,   225,   191,   192,   227,   228,
     195,   196,   194,   231,   232,   199,   200,   198,   203,   204,
     202,   211,   212,   210,   215,   216,   214,   207,   208,   206,
     185,   186,   188,   189,     0,     0,     0,     0,     0,     0,
      37,    38,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,    39,    40,     0,     0,
       0,     0,    73,    32,    34,   395,     0,     0,     0,   400,
     401,    28,    30,   127,     0,     0,   128,    33,    35,    69,
      54,    53,    55,    56,    42,    57,    50,    58,    41,    59,
      60,    61,    62,    63,    64,    65,    51,    66,    67,    68,
      43,    44,    45,    46,    47,    48,    49,    52,    72,   397,
     398,   399,   130,   129
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -359,  -359,   -55,  -359,   -29,  -139,  -359,  -358,  -359,   -30,
    -229,   -43,  -187,  -359,  -105
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    61,    62,    63,   174,   175,   432,   433,    64,   283,
     284,   313,   176,   301,   314
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     285,   285,   285,   323,    86,   177,   298,   315,   395,   316,
     178,   395,    65,    66,   286,   287,   285,   288,   317,    83,
     179,    84,    85,   436,   437,   438,   318,   319,   320,   295,
     323,   296,    72,   321,   322,    73,    69,    70,   447,    74,
     394,    71,   302,   300,   303,   304,    75,    76,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   305,   306,   442,   324,   325,   307,
       0,   372,    67,    68,    77,    78,    79,    80,    81,    82,
     289,   290,   291,   292,   293,   294,     0,   377,   380,   383,
     386,   389,   308,   309,   310,   311,   326,   327,   328,   329,
     330,   331,   332,   333,     0,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     434,   348,   349,   350,   351,     0,   352,   353,   354,   355,
     356,   357,   173,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   373,   374,   390,   391,
     392,   393,     0,     0,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   440,   440,   440,   440,   440,   440,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,   450,   451,   441,     0,   452,   453,   454,
     443,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,     0,   470,   471,   472,
     473,     0,     0,   474,   475,   476,   477,     0,     0,   440,
       0,   479,   480,   481,   440,     0,     0,   440,     0,     0,
     440,     0,     0,   440,     0,     0,   440,     0,     0,     0,
       0,     0,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,   483,   478,     0,   440,   440,   440,   440,   440,
     440,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     440,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     440,   440,   440,   440,   440,     0,   440,   440,   440,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,     0,   370,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,     0,   378,   379,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,     0,   384,   385,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,     0,   387,
     388,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   444,
     445,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,     0,     0,     0,
       0,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,     0,     0,     0,     0,     0,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,     0,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,     0,     0,     0,     0,
     299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,     1,     0,     0,     0,     2,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,     0,     0,     0,     0,     0,     0,     0,
       0,   297,     0,     4,     0,     2,     3,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      43,    44,    45,     3,    97,    34,    61,   112,     6,   114,
     274,     6,   277,   278,    44,    45,    59,    46,   123,   275,
     284,   277,   278,    98,    99,   100,   131,   132,   133,    59,
       3,    60,   276,   138,   173,   279,   277,   278,   396,   283,
     269,   282,   275,    86,   277,   278,   277,   278,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   277,   278,   323,   180,   181,   282,
      -1,   246,   139,   140,   280,   281,   277,   278,   280,   281,
     277,   278,   277,   278,   277,   278,    -1,   262,   263,   264,
     265,   266,   277,   278,   277,   278,   310,   311,   310,   311,
     310,   311,   310,   311,    -1,   310,   311,   310,   311,    -1,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     295,   310,   311,   310,   311,    -1,   310,   311,   310,   311,
     310,   311,   295,   310,   311,    -1,   310,   311,   310,   311,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     310,   311,    -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   299,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,   320,   321,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   396,   397,   398,   399,   295,    -1,   402,   403,   404,
     295,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,    -1,   422,   423,   424,
     425,    -1,    -1,   428,   429,   430,   431,    -1,    -1,   372,
      -1,   436,   437,   438,   377,    -1,    -1,   380,    -1,    -1,
     383,    -1,    -1,   386,    -1,    -1,   389,    -1,    -1,    -1,
      -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   434,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   444,   445,   432,    -1,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,    -1,   479,   480,   481,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   294,    -1,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,    -1,   310,
     311,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,    -1,   310,   311,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,    -1,   310,   311,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,
      -1,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,    -1,   310,   311,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,    -1,   310,   311,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   294,    -1,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,    -1,   310,
     311,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,     0,    -1,    -1,    -1,     4,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   294,    -1,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,    -1,   138,    -1,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     285,   286,   287,   288,   289,   290,   291,   292,   293,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   285,   286,   287,   288,
     289,   290,   291,   292,   293,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   138,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   313,   314,   315,   320,   277,   278,   139,   140,   277,
     278,   282,   276,   279,   283,   277,   278,   280,   281,   277,
     278,   280,   281,   275,   277,   278,    97,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   295,   316,   317,   324,   316,   274,   284,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   321,   322,   323,   321,   321,   316,   277,
     278,   277,   278,   277,   278,   321,   316,     0,   314,   101,
     323,   325,   275,   277,   278,   277,   278,   282,   277,   278,
     277,   278,   294,   323,   326,   326,   326,   326,   326,   326,
     326,   326,   317,     3,   323,   323,   310,   311,   310,   311,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     310,   311,   310,   311,   310,   311,   310,   311,   310,   311,
     310,   311,   326,   310,   311,   310,   311,   326,   310,   311,
     326,   310,   311,   326,   310,   311,   326,   310,   311,   326,
     310,   311,   310,   311,   322,     6,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   318,   319,   326,   323,    98,    99,   100,   294,
     323,   295,   324,   295,     8,     9,   323,   319,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   316,   326,
     326,   326,   323,   323
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   312,   313,   313,   313,   314,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   316,   316,
     317,   317,   318,   318,   318,   318,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   321,   321,   322,   322,
     322,   322,   322,   322,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   325,   325,   325,   325,   325,
     326,   326,   326,   326
  };

  const unsigned char
  seclang_parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     2,     2,
       2,     2,     2,     1,     1,     1,     2,     2,     3,     1,
       3,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     4,     3,     2,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     4,
       4,     1,     2,     2,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     2,     1,     2,     2,
       2,     1,     2,     2,     2,     1,     2,     2,     2,     1,
       2,     2,     2,     1,     2,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     3,     3,
       2,     2,     1,     1
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
  "\"MULTIPART_STRICT_ERROR\"", "\"MULTIPART_UNMATCHED_BOUNDARY\"",
  "\"OUTBOUND_DATA_ERROR\"", "\"PATH_INFO\"", "\"QUERY_STRING\"",
  "\"REMOTE_ADDR\"", "\"REMOTE_HOST\"", "\"REMOTE_PORT\"",
  "\"REQBODY_ERROR_MSG\"", "\"REQBODY_ERROR\"",
  "\"REQBODY_PROCESSOR_ERROR_MSG\"", "\"REQBODY_PROCESSOR_ERROR\"",
  "\"REQBODY_PROCESSOR\"", "\"REQUEST_BASENAME\"",
  "\"REQUEST_BODY_LENGTH\"", "\"REQUEST_BODY\"", "\"REQUEST_FILENAME\"",
  "VARIABLE_REQUEST_HEADERS_NAMES", "\"REQUEST_LINE\"",
  "\"REQUEST_METHOD\"", "\"REQUEST_PROTOCOL\"", "\"REQUEST_URI_RAW\"",
  "\"REQUEST_URI\"", "\"RESOURCE\"", "\"RESPONSE_BODY\"",
  "\"RESPONSE_CONTENT_LENGTH\"", "VARIABLE_RESPONSE_CONTENT_TYPE",
  "VARIABLE_RESPONSE_HEADERS_NAMES", "\"RESPONSE_PROTOCOL\"",
  "\"RESPONSE_STATUS\"", "\"SERVER_ADDR\"", "\"SERVER_NAME\"",
  "\"SERVER_PORT\"", "\"SESSIONID\"", "\"UNIQUE_ID\"",
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
  "\"CONFIG_COMPONENT_SIG\"", "\"CONFIG_SEC_ARGUMENT_SEPARATOR\"",
  "\"CONFIG_SEC_WEB_APP_ID\"", "\"CONFIG_SEC_SERVER_SIG\"",
  "\"CONFIG_DIR_AUDIT_DIR\"", "\"CONFIG_DIR_AUDIT_DIR_MOD\"",
  "\"CONFIG_DIR_AUDIT_ENG\"", "\"CONFIG_DIR_AUDIT_FLE_MOD\"",
  "\"CONFIG_DIR_AUDIT_LOG\"", "\"CONFIG_DIR_AUDIT_LOG2\"",
  "\"CONFIG_DIR_AUDIT_LOG_P\"", "\"CONFIG_DIR_AUDIT_STS\"",
  "\"CONFIG_DIR_AUDIT_TPE\"", "\"CONFIG_DIR_DEBUG_LOG\"",
  "\"CONFIG_DIR_DEBUG_LVL\"", "\"CONFIG_SEC_CACHE_TRANSFORMATIONS\"",
  "\"CONFIG_DIR_GEO_DB\"", "\"CONFIG_DIR_PCRE_MATCH_LIMIT\"",
  "\"CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION\"", "\"CONFIG_DIR_REQ_BODY\"",
  "\"CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT\"",
  "\"CONFIG_DIR_REQ_BODY_LIMIT\"", "\"CONFIG_DIR_REQ_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT\"", "\"CONFIG_DIR_RES_BODY\"",
  "\"CONFIG_DIR_RES_BODY_LIMIT\"", "\"CONFIG_DIR_RES_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_RULE_ENG\"", "\"CONFIG_DIR_SEC_ACTION\"",
  "\"CONFIG_DIR_SEC_DEFAULT_ACTION\"", "\"CONFIG_DIR_SEC_MARKER\"",
  "\"CONFIG_DIR_UNICODE_MAP_FILE\"", "\"CONFIG_SEC_COLLECTION_TIMEOUT\"",
  "\"CONFIG_SEC_HTTP_BLKEY\"", "\"CONFIG_SEC_REMOTE_RULES_FAIL_ACTION\"",
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
  "\"CONGIG_DIR_SEC_DATA_DIR\"", "\"CONGIG_DIR_SEC_STATUS_ENGINE\"",
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
       0,   712,   712,   716,   717,   720,   725,   731,   737,   741,
     745,   751,   757,   763,   769,   774,   779,   785,   792,   796,
     800,   806,   810,   814,   819,   824,   829,   833,   840,   844,
     851,   857,   867,   876,   886,   895,   908,   912,   916,   920,
     924,   928,   932,   936,   940,   944,   949,   953,   957,   961,
     965,   970,   975,   979,   983,   987,   991,   995,   999,  1003,
    1007,  1011,  1015,  1019,  1023,  1027,  1031,  1035,  1039,  1043,
    1047,  1061,  1062,  1086,  1105,  1120,  1144,  1200,  1204,  1208,
    1212,  1216,  1220,  1224,  1228,  1232,  1241,  1245,  1250,  1255,
    1260,  1265,  1270,  1283,  1296,  1309,  1322,  1335,  1361,  1387,
    1399,  1419,  1447,  1452,  1457,  1466,  1471,  1475,  1479,  1483,
    1487,  1491,  1495,  1496,  1497,  1509,  1515,  1519,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1532,  1540,  1544,  1551,  1556,
    1562,  1568,  1574,  1581,  1591,  1595,  1599,  1603,  1607,  1611,
    1615,  1619,  1623,  1627,  1631,  1635,  1639,  1643,  1647,  1651,
    1655,  1659,  1663,  1667,  1671,  1675,  1679,  1683,  1687,  1691,
    1695,  1699,  1703,  1707,  1711,  1715,  1719,  1723,  1727,  1731,
    1735,  1739,  1743,  1747,  1751,  1755,  1759,  1763,  1767,  1771,
    1775,  1779,  1783,  1787,  1791,  1795,  1799,  1803,  1807,  1811,
    1815,  1819,  1823,  1827,  1831,  1835,  1839,  1843,  1847,  1851,
    1855,  1859,  1863,  1867,  1871,  1875,  1879,  1883,  1887,  1891,
    1895,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1927,  1931,
    1935,  1939,  1943,  1947,  1952,  1956,  1960,  1965,  1969,  1973,
    1978,  1983,  1987,  1991,  1995,  1999,  2003,  2007,  2011,  2015,
    2019,  2023,  2027,  2031,  2035,  2039,  2043,  2047,  2051,  2055,
    2059,  2063,  2067,  2071,  2075,  2079,  2083,  2087,  2091,  2095,
    2099,  2103,  2107,  2111,  2115,  2119,  2123,  2127,  2131,  2135,
    2139,  2143,  2147,  2151,  2155,  2159,  2163,  2167,  2171,  2175,
    2179,  2183,  2187,  2191,  2195,  2199,  2203,  2207,  2211,  2215,
    2223,  2230,  2237,  2244,  2251,  2258,  2265,  2272,  2279,  2286,
    2293,  2300,  2310,  2314,  2318,  2322,  2326,  2330,  2334,  2338,
    2343,  2348,  2353,  2357,  2361,  2365,  2370,  2375,  2379,  2383,
    2387,  2391,  2395,  2399,  2403,  2407,  2411,  2415,  2420,  2424,
    2429,  2433,  2437,  2441,  2445,  2449,  2453,  2457,  2461,  2465,
    2469,  2473,  2477,  2481,  2485,  2489,  2493,  2497,  2501,  2505,
    2509,  2513,  2517,  2521,  2525,  2529,  2533,  2537,  2541,  2545,
    2549,  2553,  2557,  2561,  2565,  2569,  2573,  2577,  2581,  2585,
    2589,  2593,  2597,  2601,  2605,  2609,  2613,  2617,  2621,  2625,
    2629,  2633,  2637,  2641,  2645,  2649,  2653,  2657,  2661,  2665,
    2669,  2673,  2677,  2681,  2685,  2692,  2696,  2700,  2704,  2708,
    2715,  2720,  2725,  2731
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
#line 6023 "seclang-parser.cc" // lalr1.cc:1155
#line 2738 "seclang-parser.yy" // lalr1.cc:1156


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
