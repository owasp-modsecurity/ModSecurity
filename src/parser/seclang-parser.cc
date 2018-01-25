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
#line 357 "seclang-parser.yy" // lalr1.cc:413

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
      case 193: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 194: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 195: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 196: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 197: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 198: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 199: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_LENGTH"
      case 201: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 202: // "ACTION_TRANSFORMATION_MD5"
      case 203: // "ACTION_TRANSFORMATION_NONE"
      case 204: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 205: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 206: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 207: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 208: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 209: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 210: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 211: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 213: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_SHA1"
      case 216: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 217: // "ACTION_TRANSFORMATION_TRIM"
      case 218: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 219: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 220: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 221: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 222: // "Ver"
      case 223: // "xmlns"
      case 224: // "CONFIG_COMPONENT_SIG"
      case 225: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 226: // "CONFIG_SEC_WEB_APP_ID"
      case 227: // "CONFIG_SEC_SERVER_SIG"
      case 228: // "CONFIG_DIR_AUDIT_DIR"
      case 229: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 230: // "CONFIG_DIR_AUDIT_ENG"
      case 231: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 232: // "CONFIG_DIR_AUDIT_LOG"
      case 233: // "CONFIG_DIR_AUDIT_LOG2"
      case 234: // "CONFIG_DIR_AUDIT_LOG_P"
      case 235: // "CONFIG_DIR_AUDIT_STS"
      case 236: // "CONFIG_DIR_AUDIT_TPE"
      case 237: // "CONFIG_DIR_DEBUG_LOG"
      case 238: // "CONFIG_DIR_DEBUG_LVL"
      case 239: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 240: // "CONFIG_DIR_GEO_DB"
      case 241: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 242: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 243: // "CONFIG_DIR_REQ_BODY"
      case 244: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 245: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 246: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 247: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 248: // "CONFIG_DIR_RES_BODY"
      case 249: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 250: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 251: // "CONFIG_DIR_RULE_ENG"
      case 252: // "CONFIG_DIR_SEC_ACTION"
      case 253: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 254: // "CONFIG_DIR_SEC_MARKER"
      case 255: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 256: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 257: // "CONFIG_SEC_HTTP_BLKEY"
      case 258: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 259: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 260: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 261: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 262: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 263: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 265: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 266: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 267: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 268: // "CONFIG_UPLOAD_DIR"
      case 269: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 270: // "CONFIG_UPLOAD_FILE_MODE"
      case 271: // "CONFIG_VALUE_ABORT"
      case 272: // "CONFIG_VALUE_DETC"
      case 273: // "CONFIG_VALUE_HTTPS"
      case 274: // "CONFIG_VALUE_OFF"
      case 275: // "CONFIG_VALUE_ON"
      case 276: // "CONFIG_VALUE_PARALLEL"
      case 277: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 278: // "CONFIG_VALUE_REJECT"
      case 279: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 280: // "CONFIG_VALUE_SERIAL"
      case 281: // "CONFIG_VALUE_WARN"
      case 282: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 283: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 284: // "CONGIG_DIR_SEC_ARG_SEP"
      case 285: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 286: // "CONGIG_DIR_SEC_DATA_DIR"
      case 287: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 288: // "CONGIG_DIR_SEC_TMP_DIR"
      case 289: // "DIRECTIVE"
      case 290: // "DIRECTIVE_SECRULESCRIPT"
      case 291: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 292: // "QUOTATION_MARK"
      case 293: // "RUN_TIME_VAR_BLD"
      case 294: // "RUN_TIME_VAR_DUR"
      case 295: // "RUN_TIME_VAR_HSV"
      case 296: // "RUN_TIME_VAR_REMOTE_USER"
      case 297: // "RUN_TIME_VAR_TIME"
      case 298: // "RUN_TIME_VAR_TIME_DAY"
      case 299: // "RUN_TIME_VAR_TIME_EPOCH"
      case 300: // "RUN_TIME_VAR_TIME_HOUR"
      case 301: // "RUN_TIME_VAR_TIME_MIN"
      case 302: // "RUN_TIME_VAR_TIME_MON"
      case 303: // "RUN_TIME_VAR_TIME_SEC"
      case 304: // "RUN_TIME_VAR_TIME_WDAY"
      case 305: // "RUN_TIME_VAR_TIME_YEAR"
      case 306: // "SETVAR_VARIABLE_PART"
      case 307: // "SETVAR_CONTENT_PART"
      case 308: // "VARIABLE"
      case 309: // "Dictionary element"
      case 310: // "Dictionary element, selected by regexp"
      case 325: // setvar_variable
      case 326: // setvar_content
        value.move< std::string > (that.value);
        break;

      case 317: // op
      case 318: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 327: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 322: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 323: // act
      case 324: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 320: // variables
      case 321: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 315: // actions
      case 316: // actions_may_quoted
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
      case 193: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 194: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 195: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 196: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 197: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 198: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 199: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_LENGTH"
      case 201: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 202: // "ACTION_TRANSFORMATION_MD5"
      case 203: // "ACTION_TRANSFORMATION_NONE"
      case 204: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 205: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 206: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 207: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 208: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 209: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 210: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 211: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 213: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_SHA1"
      case 216: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 217: // "ACTION_TRANSFORMATION_TRIM"
      case 218: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 219: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 220: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 221: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 222: // "Ver"
      case 223: // "xmlns"
      case 224: // "CONFIG_COMPONENT_SIG"
      case 225: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 226: // "CONFIG_SEC_WEB_APP_ID"
      case 227: // "CONFIG_SEC_SERVER_SIG"
      case 228: // "CONFIG_DIR_AUDIT_DIR"
      case 229: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 230: // "CONFIG_DIR_AUDIT_ENG"
      case 231: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 232: // "CONFIG_DIR_AUDIT_LOG"
      case 233: // "CONFIG_DIR_AUDIT_LOG2"
      case 234: // "CONFIG_DIR_AUDIT_LOG_P"
      case 235: // "CONFIG_DIR_AUDIT_STS"
      case 236: // "CONFIG_DIR_AUDIT_TPE"
      case 237: // "CONFIG_DIR_DEBUG_LOG"
      case 238: // "CONFIG_DIR_DEBUG_LVL"
      case 239: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 240: // "CONFIG_DIR_GEO_DB"
      case 241: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 242: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 243: // "CONFIG_DIR_REQ_BODY"
      case 244: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 245: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 246: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 247: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 248: // "CONFIG_DIR_RES_BODY"
      case 249: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 250: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 251: // "CONFIG_DIR_RULE_ENG"
      case 252: // "CONFIG_DIR_SEC_ACTION"
      case 253: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 254: // "CONFIG_DIR_SEC_MARKER"
      case 255: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 256: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 257: // "CONFIG_SEC_HTTP_BLKEY"
      case 258: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 259: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 260: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 261: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 262: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 263: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 265: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 266: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 267: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 268: // "CONFIG_UPLOAD_DIR"
      case 269: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 270: // "CONFIG_UPLOAD_FILE_MODE"
      case 271: // "CONFIG_VALUE_ABORT"
      case 272: // "CONFIG_VALUE_DETC"
      case 273: // "CONFIG_VALUE_HTTPS"
      case 274: // "CONFIG_VALUE_OFF"
      case 275: // "CONFIG_VALUE_ON"
      case 276: // "CONFIG_VALUE_PARALLEL"
      case 277: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 278: // "CONFIG_VALUE_REJECT"
      case 279: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 280: // "CONFIG_VALUE_SERIAL"
      case 281: // "CONFIG_VALUE_WARN"
      case 282: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 283: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 284: // "CONGIG_DIR_SEC_ARG_SEP"
      case 285: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 286: // "CONGIG_DIR_SEC_DATA_DIR"
      case 287: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 288: // "CONGIG_DIR_SEC_TMP_DIR"
      case 289: // "DIRECTIVE"
      case 290: // "DIRECTIVE_SECRULESCRIPT"
      case 291: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 292: // "QUOTATION_MARK"
      case 293: // "RUN_TIME_VAR_BLD"
      case 294: // "RUN_TIME_VAR_DUR"
      case 295: // "RUN_TIME_VAR_HSV"
      case 296: // "RUN_TIME_VAR_REMOTE_USER"
      case 297: // "RUN_TIME_VAR_TIME"
      case 298: // "RUN_TIME_VAR_TIME_DAY"
      case 299: // "RUN_TIME_VAR_TIME_EPOCH"
      case 300: // "RUN_TIME_VAR_TIME_HOUR"
      case 301: // "RUN_TIME_VAR_TIME_MIN"
      case 302: // "RUN_TIME_VAR_TIME_MON"
      case 303: // "RUN_TIME_VAR_TIME_SEC"
      case 304: // "RUN_TIME_VAR_TIME_WDAY"
      case 305: // "RUN_TIME_VAR_TIME_YEAR"
      case 306: // "SETVAR_VARIABLE_PART"
      case 307: // "SETVAR_CONTENT_PART"
      case 308: // "VARIABLE"
      case 309: // "Dictionary element"
      case 310: // "Dictionary element, selected by regexp"
      case 325: // setvar_variable
      case 326: // setvar_content
        value.copy< std::string > (that.value);
        break;

      case 317: // op
      case 318: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 327: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 322: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 323: // act
      case 324: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 320: // variables
      case 321: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 315: // actions
      case 316: // actions_may_quoted
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
    #line 350 "/home/zimmerle/core-trustwave/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 821 "seclang-parser.cc" // lalr1.cc:741

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
      case 193: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 194: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 195: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 196: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 197: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 198: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 199: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 200: // "ACTION_TRANSFORMATION_LENGTH"
      case 201: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 202: // "ACTION_TRANSFORMATION_MD5"
      case 203: // "ACTION_TRANSFORMATION_NONE"
      case 204: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 205: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 206: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 207: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 208: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 209: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 210: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 211: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 212: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 213: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 214: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 215: // "ACTION_TRANSFORMATION_SHA1"
      case 216: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 217: // "ACTION_TRANSFORMATION_TRIM"
      case 218: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 219: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 220: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 221: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 222: // "Ver"
      case 223: // "xmlns"
      case 224: // "CONFIG_COMPONENT_SIG"
      case 225: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 226: // "CONFIG_SEC_WEB_APP_ID"
      case 227: // "CONFIG_SEC_SERVER_SIG"
      case 228: // "CONFIG_DIR_AUDIT_DIR"
      case 229: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 230: // "CONFIG_DIR_AUDIT_ENG"
      case 231: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 232: // "CONFIG_DIR_AUDIT_LOG"
      case 233: // "CONFIG_DIR_AUDIT_LOG2"
      case 234: // "CONFIG_DIR_AUDIT_LOG_P"
      case 235: // "CONFIG_DIR_AUDIT_STS"
      case 236: // "CONFIG_DIR_AUDIT_TPE"
      case 237: // "CONFIG_DIR_DEBUG_LOG"
      case 238: // "CONFIG_DIR_DEBUG_LVL"
      case 239: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 240: // "CONFIG_DIR_GEO_DB"
      case 241: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 242: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 243: // "CONFIG_DIR_REQ_BODY"
      case 244: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 245: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 246: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 247: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 248: // "CONFIG_DIR_RES_BODY"
      case 249: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 250: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 251: // "CONFIG_DIR_RULE_ENG"
      case 252: // "CONFIG_DIR_SEC_ACTION"
      case 253: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 254: // "CONFIG_DIR_SEC_MARKER"
      case 255: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 256: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 257: // "CONFIG_SEC_HTTP_BLKEY"
      case 258: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 259: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 260: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 261: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 262: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 263: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 264: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 265: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 266: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 267: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 268: // "CONFIG_UPLOAD_DIR"
      case 269: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 270: // "CONFIG_UPLOAD_FILE_MODE"
      case 271: // "CONFIG_VALUE_ABORT"
      case 272: // "CONFIG_VALUE_DETC"
      case 273: // "CONFIG_VALUE_HTTPS"
      case 274: // "CONFIG_VALUE_OFF"
      case 275: // "CONFIG_VALUE_ON"
      case 276: // "CONFIG_VALUE_PARALLEL"
      case 277: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 278: // "CONFIG_VALUE_REJECT"
      case 279: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 280: // "CONFIG_VALUE_SERIAL"
      case 281: // "CONFIG_VALUE_WARN"
      case 282: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 283: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 284: // "CONGIG_DIR_SEC_ARG_SEP"
      case 285: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 286: // "CONGIG_DIR_SEC_DATA_DIR"
      case 287: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 288: // "CONGIG_DIR_SEC_TMP_DIR"
      case 289: // "DIRECTIVE"
      case 290: // "DIRECTIVE_SECRULESCRIPT"
      case 291: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 292: // "QUOTATION_MARK"
      case 293: // "RUN_TIME_VAR_BLD"
      case 294: // "RUN_TIME_VAR_DUR"
      case 295: // "RUN_TIME_VAR_HSV"
      case 296: // "RUN_TIME_VAR_REMOTE_USER"
      case 297: // "RUN_TIME_VAR_TIME"
      case 298: // "RUN_TIME_VAR_TIME_DAY"
      case 299: // "RUN_TIME_VAR_TIME_EPOCH"
      case 300: // "RUN_TIME_VAR_TIME_HOUR"
      case 301: // "RUN_TIME_VAR_TIME_MIN"
      case 302: // "RUN_TIME_VAR_TIME_MON"
      case 303: // "RUN_TIME_VAR_TIME_SEC"
      case 304: // "RUN_TIME_VAR_TIME_WDAY"
      case 305: // "RUN_TIME_VAR_TIME_YEAR"
      case 306: // "SETVAR_VARIABLE_PART"
      case 307: // "SETVAR_CONTENT_PART"
      case 308: // "VARIABLE"
      case 309: // "Dictionary element"
      case 310: // "Dictionary element, selected by regexp"
      case 325: // setvar_variable
      case 326: // setvar_content
        yylhs.value.build< std::string > ();
        break;

      case 317: // op
      case 318: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 327: // run_time_string
        yylhs.value.build< std::unique_ptr<RunTimeString> > ();
        break;

      case 322: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 323: // act
      case 324: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 320: // variables
      case 321: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 315: // actions
      case 316: // actions_may_quoted
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
#line 713 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1135 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 726 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1143 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 732 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 738 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 742 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1167 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 746 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1175 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 764 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 770 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1207 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 775 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 780 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 786 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1232 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 793 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1240 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 797 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 801 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1256 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 807 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1264 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 811 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1272 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 815 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1281 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 820 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1290 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 825 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1299 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 830 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1307 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 834 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1315 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 841 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1323 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 845 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1331 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 852 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1341 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 858 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1352 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 868 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1365 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 877 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1379 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 887 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 896 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1406 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 909 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1414 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 913 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 917 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 921 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 925 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 929 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::InspectFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1454 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 933 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::FuzzyHash(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1462 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 937 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 941 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 945 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 950 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1495 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 954 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 963 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1521 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 968 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1530 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 973 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1539 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 978 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1547 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 982 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1555 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 986 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1563 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 990 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1571 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 994 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1579 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 998 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1587 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 1002 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1595 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 1006 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1603 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 1010 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1611 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 1014 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1619 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 1018 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1627 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 1022 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 1026 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1643 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 1030 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1651 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1034 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1659 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1038 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1667 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1042 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1675 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 69:
#line 1046 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 1683 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1050 "seclang-parser.yy" // lalr1.cc:859
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
#line 1698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1065 "seclang-parser.yy" // lalr1.cc:859
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
#line 1726 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1089 "seclang-parser.yy" // lalr1.cc:859
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
#line 1749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1108 "seclang-parser.yy" // lalr1.cc:859
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
#line 1768 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1123 "seclang-parser.yy" // lalr1.cc:859
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
#line 1796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1147 "seclang-parser.yy" // lalr1.cc:859
    {
        std::vector<actions::Action *> *actions = new std::vector<actions::Action *>();
        for (auto &i : *yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ().get()) {
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
#line 1847 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1194 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1855 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1198 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1863 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1202 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1871 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1206 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1879 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1210 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1214 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1895 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1218 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1903 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1222 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1911 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1226 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 1924 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1235 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1932 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1239 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secWebAppId.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 1941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1244 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 1950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1249 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1959 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1254 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 1968 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1259 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1977 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1264 "seclang-parser.yy" // lalr1.cc:859
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
#line 1994 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1277 "seclang-parser.yy" // lalr1.cc:859
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
#line 2011 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1290 "seclang-parser.yy" // lalr1.cc:859
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
#line 2028 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1303 "seclang-parser.yy" // lalr1.cc:859
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
#line 2045 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1316 "seclang-parser.yy" // lalr1.cc:859
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
#line 2062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1329 "seclang-parser.yy" // lalr1.cc:859
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
#line 2092 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1355 "seclang-parser.yy" // lalr1.cc:859
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
#line 2121 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1381 "seclang-parser.yy" // lalr1.cc:859
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
#line 2137 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1393 "seclang-parser.yy" // lalr1.cc:859
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
#line 2160 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1413 "seclang-parser.yy" // lalr1.cc:859
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
#line 2191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1441 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2200 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1446 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1451 "seclang-parser.yy" // lalr1.cc:859
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1460 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1465 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1469 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 1473 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1477 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1481 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1485 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1491 "seclang-parser.yy" // lalr1.cc:859
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
#line 2295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1503 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2305 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1509 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2313 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1513 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2321 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1523 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2328 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1526 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1534 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2345 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1538 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1545 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2362 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1550 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2372 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1556 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2382 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1562 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1568 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2403 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1575 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2414 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1585 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1589 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1593 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1597 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1601 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2454 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1605 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2462 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1609 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1613 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1617 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1621 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1625 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1629 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1633 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1637 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1641 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1645 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1649 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1653 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1657 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1661 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1665 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1669 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1673 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1677 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2606 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1681 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2614 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1685 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2622 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1689 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1693 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1697 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2646 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1701 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2654 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1705 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2662 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1709 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1713 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2678 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1717 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2686 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1721 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2694 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1725 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2702 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1729 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2710 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1733 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2718 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1737 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2726 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1741 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2734 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1745 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1749 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2750 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1753 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2758 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1757 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2766 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1761 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2774 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1765 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2782 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1769 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1773 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2798 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1777 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2806 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1781 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1785 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2822 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1789 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1793 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2838 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1797 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1801 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1805 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1809 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2870 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1813 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1817 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1821 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1825 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1829 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1833 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 2918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1838 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1842 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1846 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 2942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1851 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1855 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1859 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 2966 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1864 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2974 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1868 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2982 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1872 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 2990 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1877 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2998 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1881 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1885 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3014 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1890 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1894 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3030 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1898 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3038 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1902 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3046 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1906 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1910 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1915 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1919 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3078 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1923 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1928 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1932 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1936 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1941 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1946 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3126 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1950 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1954 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1958 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1962 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3158 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1966 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1970 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3174 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1974 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3182 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1978 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3190 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1982 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3198 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1986 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1990 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryQuoted());
      }
#line 3214 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1994 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartBoundaryWhiteSpace());
      }
#line 3222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1998 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 2002 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3238 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2006 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateBefore());
      }
#line 3246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2010 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2014 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2018 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2022 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidPart());
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2026 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2030 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartLFLine());
      }
#line 3294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2034 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartMissingSemicolon());
      }
#line 3302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2038 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2042 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2046 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2050 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3334 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2054 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2058 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2062 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3358 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2066 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3366 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2070 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2074 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3382 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2078 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3390 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2082 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3398 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2086 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3406 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2090 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3414 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2094 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2098 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2102 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2106 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2110 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3454 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2114 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3462 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2118 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2122 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2126 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2130 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2134 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2138 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2142 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2146 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2150 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2154 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2158 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2162 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2166 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2170 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2174 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::WebAppId());
      }
#line 3582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2178 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3593 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2186 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3604 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2193 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2200 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3626 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2207 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3637 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2214 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2221 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3659 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2228 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2235 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3681 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2242 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3692 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2249 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2256 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3714 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2263 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3725 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2273 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3733 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2277 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3741 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2281 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2285 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3757 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2289 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3765 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2293 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3773 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2297 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3781 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2301 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2306 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3799 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2311 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3808 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2316 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3816 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2320 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3824 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2324 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3832 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2328 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3841 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2333 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3850 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2338 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3858 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2342 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3866 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2346 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3874 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2350 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3882 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2354 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3890 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2358 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3898 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2362 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3906 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2366 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3914 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2370 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3922 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2374 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3930 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2378 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3939 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2383 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 3947 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2387 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3956 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2392 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3964 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2396 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3972 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2400 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 3980 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2404 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3988 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2408 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3996 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2412 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4004 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2416 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4012 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2420 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4020 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2424 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4028 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2428 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4036 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2432 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2436 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4052 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2440 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4060 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2444 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4068 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2448 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4076 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2452 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4084 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2456 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4092 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2460 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4100 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2464 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4108 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2468 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4116 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2472 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4124 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2476 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4132 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2480 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4140 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2484 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4148 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2488 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4156 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2492 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4164 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2496 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4172 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2500 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4180 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2504 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4188 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2508 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4196 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2512 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4204 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2516 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4212 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2520 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4220 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2524 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4228 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2528 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4236 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2532 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4244 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2536 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4252 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2540 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2544 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4268 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2548 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4276 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2552 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2556 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2560 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 2564 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 2568 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 2572 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 2576 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4332 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 2580 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4340 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 2584 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4348 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 2588 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4356 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 2592 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4364 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 2596 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4372 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 373:
#line 2600 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4380 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 374:
#line 2604 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4388 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 375:
#line 2608 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4396 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 2612 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 2616 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4412 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 2620 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4420 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 2624 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 2628 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4436 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 2632 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 2636 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 383:
#line 2643 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4460 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 384:
#line 2647 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4468 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 385:
#line 2651 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 386:
#line 2655 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4484 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 387:
#line 2659 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4492 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 388:
#line 2666 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 389:
#line 2671 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4508 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 390:
#line 2678 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4516 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 391:
#line 2683 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4524 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 392:
#line 2690 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 393:
#line 2695 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 394:
#line 2700 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4552 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 395:
#line 2706 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4562 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4566 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -306;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
    1350,  -306,  -252,  -306,   -14,  -306,  -306,  -306,  -306,  -306,
    -306,  -146,  -306,  -306,  -306,  -306,  -306,  -149,  -306,  -306,
    -306,  -306,  -306,  -306,  -136,  -306,  -306,  -133,  -306,  -128,
    -306,  -129,  -244,  1665,  1665,  -306,  -306,  -306,  -306,  -260,
    -306,  -306,  -306,    25,    25,    25,  1665,  -124,  -122,  -306,
    -306,  -306,  -120,  -306,  -306,  -306,  -306,  -306,  -306,    25,
    1665,  1491,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,   -97,  -132,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -138,  -306,  -306,  -306,  -118,
    -116,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    1098,  -306,  1098,  -306,  1098,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  1098,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  1098,  1098,  1098,  -306,  -306,  -306,  -306,  1098,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    1817,  -306,     2,  -306,  -306,  -306,  -306,  1394,  1394,  -148,
    -144,  -142,  -140,  -137,  -135,  -131,  -127,  -123,  -121,  -119,
    -117,  -115,  -113,  -111,  -109,  -306,  -107,  -105,  -103,   -99,
    -306,  -306,   -96,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,   -94,  -306,  -306,
    -306,  -306,  -306,   -92,  -306,  -306,  -306,   -90,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,   -85,
     -83,   -81,   -79,   -77,   -75,   417,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
       1,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,   802,  -306,  -306,  -306,  -287,  -287,  -306,   -86,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,    17,
    1817,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,    12,   506,   930,  1098,  1098,  1098,  -306,
    -306,  1098,  1098,  1098,  -306,  1098,  1098,  1098,  1098,  1098,
    1098,  1098,  1098,  1098,  1098,  1098,  1098,  1098,  1098,  1098,
    -306,  1098,  1098,  1098,  1098,  -306,  -306,  1098,  1098,  1098,
    1098,  1665,  -306,  1226,  -306,  -306,  -188,  -188,  -188,  -306,
    -306,  -306,  -306,  -306,  1394,  1394,  -306,  -306,  1226,  1226,
    1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,
    1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,
    1226,  1226,  1226,  1226,  1226,  1226,  1226,  1226,  -306,  -188,
    -306,  -306,  -306,  -306,  -306,  -306
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
      83,   108,   109,    80,    78,    79,     0,     0,   293,   294,
     295,   296,   297,   298,   299,     0,   303,   304,   305,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
       0,   324,     0,   325,     0,   327,   328,   329,   330,   331,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   341,
     342,     0,     0,     0,   347,   348,   349,   350,     0,   358,
     368,   374,   361,   362,   372,   373,   378,   363,   360,   367,
     377,   376,   356,   355,   354,   381,   380,   371,   369,   382,
     370,   359,   357,   375,   364,   366,   365,   379,   352,   353,
       0,    74,    29,    31,    76,   110,   111,     0,     0,   136,
     139,   142,   145,   148,   151,   154,   157,   160,   163,   166,
     169,   172,   175,   178,   181,   225,   214,   184,   211,   217,
     226,   227,   193,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   254,   253,
     257,   256,   255,   258,   260,   259,   261,   220,   262,   263,
     264,   266,   265,   196,   267,   268,   221,   224,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   279,   278,   199,
     202,   205,   208,   187,   190,     0,   281,   280,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,    95,
     126,   131,    96,    97,    98,    22,    21,    27,    26,   116,
     117,     0,    75,     1,     3,     0,   388,   346,   384,   312,
     311,   310,   301,   300,   302,   307,   306,   309,   308,   394,
     395,   322,   323,   326,   335,   343,   344,   345,   351,     0,
       0,   133,   132,   134,   135,   137,   138,   140,   141,   143,
     144,   146,   147,   149,   150,   152,   153,   155,   156,   158,
     159,   161,   162,   164,   165,   167,   168,   170,   171,   173,
     174,   176,   177,   179,   180,   212,   213,   182,   183,   209,
     210,   215,   216,   191,   192,   218,   219,   194,   195,   222,
     223,   197,   198,   200,   201,   203,   204,   206,   207,   185,
     186,   188,   189,     0,     0,     0,     0,     0,     0,    37,
      38,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,    39,    40,     0,     0,     0,
       0,    73,    32,    34,   383,   389,     0,     0,     0,   392,
     393,    28,    30,   127,     0,     0,   128,    33,    35,    69,
      54,    53,    55,    56,    42,    57,    50,    58,    41,    59,
      60,    61,    62,    63,    64,    65,    51,    66,    67,    68,
      43,    44,    45,    46,    47,    48,    49,    52,    72,   390,
     385,   386,   387,   130,   129,   391
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -306,  -306,    69,  -306,   -31,   -38,  -306,  -225,  -306,   -35,
    -102,   -43,  -143,  -306,   -59,  -305,  -106
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    61,    62,    63,   171,   172,   421,   422,    64,   279,
     280,   310,   173,   297,   298,   470,   311
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     281,   281,   281,   174,   295,   320,   312,   384,   313,   282,
     283,   175,   426,   427,   428,   284,   281,   314,   384,   296,
     320,   176,    65,    66,   291,   315,   316,   317,    83,   292,
      84,    85,   318,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   469,
     263,   264,   471,   472,    72,    67,    68,    73,    69,    70,
     294,    74,   319,    71,   321,   322,   302,   303,    75,    76,
     299,   304,   300,   301,    77,    78,    79,    80,    81,    82,
     285,   286,   287,   288,   289,   290,   305,   306,   307,   308,
     437,   323,   324,   383,   475,   325,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   432,   335,   336,
       0,     0,   337,   338,     0,   423,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,   296,
     361,   362,     0,   363,   364,   365,   366,   367,   368,   369,
     370,     0,   281,     0,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   424,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,   430,
     430,   430,   430,   430,   430,   430,     0,     0,     0,   438,
     439,   440,   441,     0,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   433,   460,   461,   462,   463,   431,
       0,   464,   465,   466,   467,     0,     0,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   473,   474,     0,     0,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   263,   264,   434,   435,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   263,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   263,   264,     0,
       0,     0,     0,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,     0,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   263,   264,     0,     0,     0,
       0,     0,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,     0,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   263,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   263,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     0,     0,     0,     2,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     4,   263,
     264,   293,     0,     0,     0,     2,     3,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       0,     0,    52,    53,    54,    55,    56,    57,    58,    59,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      43,    44,    45,    34,   101,     3,   112,     6,   114,    44,
      45,   271,    98,    99,   100,    46,    59,   123,     6,   306,
       3,   281,   274,   275,    59,   131,   132,   133,   272,    60,
     274,   275,   138,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,   307,
      95,    96,   427,   428,   273,   139,   140,   276,   274,   275,
      61,   280,   170,   279,   177,   178,   274,   275,   274,   275,
     272,   279,   274,   275,   277,   278,   274,   275,   277,   278,
     274,   275,   274,   275,   274,   275,   274,   275,   274,   275,
     385,   309,   310,   265,   469,   309,   310,   309,   310,   309,
     310,    -1,   309,   310,   309,   310,    -1,   320,   309,   310,
      -1,    -1,   309,   310,    -1,   291,   309,   310,   309,   310,
     309,   310,   309,   310,   309,   310,   309,   310,   309,   310,
     309,   310,   309,   310,   309,   310,   309,   310,    -1,   306,
     309,   310,    -1,   309,   310,   309,   310,   309,   310,   309,
     310,    -1,   265,    -1,   309,   310,   309,   310,   309,   310,
     309,   310,   309,   310,   309,   310,   295,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   311,   312,
     313,   314,   315,   316,   317,   318,    -1,    -1,    -1,   385,
     386,   387,   388,    -1,    -1,   391,   392,   393,    -1,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   292,   411,   412,   413,   414,   292,
      -1,   417,   418,   419,   420,    -1,    -1,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   384,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     423,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     421,   434,   435,    -1,    -1,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,    -1,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,    -1,    -1,    -1,     4,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,   138,    95,
      96,     0,    -1,    -1,    -1,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   282,   283,   284,   285,   286,   287,   288,
     289,   290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   138,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   312,   313,   314,   319,   274,   275,   139,   140,   274,
     275,   279,   273,   276,   280,   274,   275,   277,   278,   274,
     275,   277,   278,   272,   274,   275,    97,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     292,   315,   316,   323,   315,   271,   281,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    95,    96,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   320,
     321,   322,   320,   320,   315,   274,   275,   274,   275,   274,
     275,   320,   315,     0,   313,   101,   306,   324,   325,   272,
     274,   275,   274,   275,   279,   274,   275,   274,   275,   291,
     322,   327,   327,   327,   327,   327,   327,   327,   327,   316,
       3,   322,   322,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   309,   310,   309,   310,   309,   310,   309,
     310,   309,   310,   321,     6,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   317,   318,   327,   325,   325,    98,    99,   100,   291,
     322,   292,   323,   292,     8,     9,   322,   318,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   315,   307,
     326,   326,   326,   322,   322,   326
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   311,   312,   312,   312,   313,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   315,   315,
     316,   316,   317,   317,   317,   317,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   320,   320,   321,   321,
     321,   321,   321,   321,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   324,   324,   324,   324,   324,   325,   325,
     326,   326,   327,   327,   327,   327
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
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     3,     3,     1,     2,
       1,     2,     2,     2,     1,     1
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
  "\"RUN_TIME_VAR_TIME_YEAR\"", "\"SETVAR_VARIABLE_PART\"",
  "\"SETVAR_CONTENT_PART\"", "\"VARIABLE\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "variables_may_be_quoted", "var", "act",
  "setvar_action", "setvar_variable", "setvar_content", "run_time_string", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   712,   712,   716,   717,   720,   725,   731,   737,   741,
     745,   751,   757,   763,   769,   774,   779,   785,   792,   796,
     800,   806,   810,   814,   819,   824,   829,   833,   840,   844,
     851,   857,   867,   876,   886,   895,   908,   912,   916,   920,
     924,   928,   932,   936,   940,   944,   949,   953,   957,   962,
     967,   972,   977,   981,   985,   989,   993,   997,  1001,  1005,
    1009,  1013,  1017,  1021,  1025,  1029,  1033,  1037,  1041,  1045,
    1049,  1063,  1064,  1088,  1107,  1122,  1146,  1193,  1197,  1201,
    1205,  1209,  1213,  1217,  1221,  1225,  1234,  1238,  1243,  1248,
    1253,  1258,  1263,  1276,  1289,  1302,  1315,  1328,  1354,  1380,
    1392,  1412,  1440,  1445,  1450,  1459,  1464,  1468,  1472,  1476,
    1480,  1484,  1488,  1489,  1490,  1502,  1508,  1512,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1525,  1533,  1537,  1544,  1549,
    1555,  1561,  1567,  1574,  1584,  1588,  1592,  1596,  1600,  1604,
    1608,  1612,  1616,  1620,  1624,  1628,  1632,  1636,  1640,  1644,
    1648,  1652,  1656,  1660,  1664,  1668,  1672,  1676,  1680,  1684,
    1688,  1692,  1696,  1700,  1704,  1708,  1712,  1716,  1720,  1724,
    1728,  1732,  1736,  1740,  1744,  1748,  1752,  1756,  1760,  1764,
    1768,  1772,  1776,  1780,  1784,  1788,  1792,  1796,  1800,  1804,
    1808,  1812,  1816,  1820,  1824,  1828,  1832,  1837,  1841,  1845,
    1850,  1854,  1858,  1863,  1867,  1871,  1876,  1880,  1884,  1889,
    1893,  1897,  1901,  1905,  1909,  1914,  1918,  1922,  1927,  1931,
    1935,  1940,  1945,  1949,  1953,  1957,  1961,  1965,  1969,  1973,
    1977,  1981,  1985,  1989,  1993,  1997,  2001,  2005,  2009,  2013,
    2017,  2021,  2025,  2029,  2033,  2037,  2041,  2045,  2049,  2053,
    2057,  2061,  2065,  2069,  2073,  2077,  2081,  2085,  2089,  2093,
    2097,  2101,  2105,  2109,  2113,  2117,  2121,  2125,  2129,  2133,
    2137,  2141,  2145,  2149,  2153,  2157,  2161,  2165,  2169,  2173,
    2177,  2185,  2192,  2199,  2206,  2213,  2220,  2227,  2234,  2241,
    2248,  2255,  2262,  2272,  2276,  2280,  2284,  2288,  2292,  2296,
    2300,  2305,  2310,  2315,  2319,  2323,  2327,  2332,  2337,  2341,
    2345,  2349,  2353,  2357,  2361,  2365,  2369,  2373,  2377,  2382,
    2386,  2391,  2395,  2399,  2403,  2407,  2411,  2415,  2419,  2423,
    2427,  2431,  2435,  2439,  2443,  2447,  2451,  2455,  2459,  2463,
    2467,  2471,  2475,  2479,  2483,  2487,  2491,  2495,  2499,  2503,
    2507,  2511,  2515,  2519,  2523,  2527,  2531,  2535,  2539,  2543,
    2547,  2551,  2555,  2559,  2563,  2567,  2571,  2575,  2579,  2583,
    2587,  2591,  2595,  2599,  2603,  2607,  2611,  2615,  2619,  2623,
    2627,  2631,  2635,  2642,  2646,  2650,  2654,  2658,  2665,  2670,
    2677,  2682,  2689,  2694,  2699,  2705
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
#line 5744 "seclang-parser.cc" // lalr1.cc:1167
#line 2712 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
