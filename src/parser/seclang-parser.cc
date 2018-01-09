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
#line 351 "seclang-parser.yy" // lalr1.cc:413

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
      case 100: // "Accuracy"
      case 101: // "Allow"
      case 102: // "Append"
      case 103: // "AuditLog"
      case 104: // "Block"
      case 105: // "Capture"
      case 106: // "Chain"
      case 107: // "ACTION_CTL_AUDIT_ENGINE"
      case 108: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 109: // "ACTION_CTL_BDY_JSON"
      case 110: // "ACTION_CTL_BDY_XML"
      case 111: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 112: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 113: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 115: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 116: // "Deny"
      case 117: // "DeprecateVar"
      case 118: // "Drop"
      case 119: // "Exec"
      case 120: // "ExpireVar"
      case 121: // "Id"
      case 122: // "InitCol"
      case 123: // "Log"
      case 124: // "LogData"
      case 125: // "Maturity"
      case 126: // "Msg"
      case 127: // "MultiMatch"
      case 128: // "NoAuditLog"
      case 129: // "NoLog"
      case 130: // "Pass"
      case 131: // "Pause"
      case 132: // "Phase"
      case 133: // "Prepend"
      case 134: // "Proxy"
      case 135: // "Redirect"
      case 136: // "Rev"
      case 137: // "SanatiseArg"
      case 138: // "SanatiseMatched"
      case 139: // "SanatiseMatchedBytes"
      case 140: // "SanatiseRequestHeader"
      case 141: // "SanatiseResponseHeader"
      case 142: // "SetEnv"
      case 143: // "SetRsc"
      case 144: // "SetSid"
      case 145: // "SetUID"
      case 146: // "Severity"
      case 147: // "Skip"
      case 148: // "SkipAfter"
      case 149: // "Status"
      case 150: // "Tag"
      case 151: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 152: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 153: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 155: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 156: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 157: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 158: // "ACTION_TRANSFORMATION_LENGTH"
      case 159: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 160: // "ACTION_TRANSFORMATION_MD5"
      case 161: // "ACTION_TRANSFORMATION_NONE"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 163: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 164: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 172: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_SHA1"
      case 174: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 175: // "ACTION_TRANSFORMATION_TRIM"
      case 176: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 178: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 179: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 180: // "Ver"
      case 181: // "xmlns"
      case 182: // "CONFIG_COMPONENT_SIG"
      case 183: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 184: // "CONFIG_SEC_WEB_APP_ID"
      case 185: // "CONFIG_SEC_SERVER_SIG"
      case 186: // "CONFIG_DIR_AUDIT_DIR"
      case 187: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 188: // "CONFIG_DIR_AUDIT_ENG"
      case 189: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 190: // "CONFIG_DIR_AUDIT_LOG"
      case 191: // "CONFIG_DIR_AUDIT_LOG2"
      case 192: // "CONFIG_DIR_AUDIT_LOG_P"
      case 193: // "CONFIG_DIR_AUDIT_STS"
      case 194: // "CONFIG_DIR_AUDIT_TPE"
      case 195: // "CONFIG_DIR_DEBUG_LOG"
      case 196: // "CONFIG_DIR_DEBUG_LVL"
      case 197: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 198: // "CONFIG_DIR_GEO_DB"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 200: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 201: // "CONFIG_DIR_REQ_BODY"
      case 202: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 204: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 206: // "CONFIG_DIR_RES_BODY"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 208: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 209: // "CONFIG_DIR_RULE_ENG"
      case 210: // "CONFIG_DIR_SEC_ACTION"
      case 211: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 212: // "CONFIG_DIR_SEC_MARKER"
      case 213: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 214: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 215: // "CONFIG_SEC_HTTP_BLKEY"
      case 216: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 218: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 219: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 220: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 221: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 223: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 224: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 225: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 226: // "CONFIG_UPLOAD_DIR"
      case 227: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 228: // "CONFIG_UPLOAD_FILE_MODE"
      case 229: // "CONFIG_VALUE_ABORT"
      case 230: // "CONFIG_VALUE_DETC"
      case 231: // "CONFIG_VALUE_HTTPS"
      case 232: // "CONFIG_VALUE_OFF"
      case 233: // "CONFIG_VALUE_ON"
      case 234: // "CONFIG_VALUE_PARALLEL"
      case 235: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 236: // "CONFIG_VALUE_REJECT"
      case 237: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 238: // "CONFIG_VALUE_SERIAL"
      case 239: // "CONFIG_VALUE_WARN"
      case 240: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 241: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 242: // "CONGIG_DIR_SEC_ARG_SEP"
      case 243: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 244: // "CONGIG_DIR_SEC_DATA_DIR"
      case 245: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 246: // "CONGIG_DIR_SEC_TMP_DIR"
      case 247: // "DIRECTIVE"
      case 248: // "DIRECTIVE_SECRULESCRIPT"
      case 249: // "FREE_TEXT"
      case 250: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 251: // "OPERATOR"
      case 252: // "OPERATOR_BEGINS_WITH"
      case 253: // "OPERATOR_CONTAINS"
      case 254: // "OPERATOR_CONTAINS_WORD"
      case 255: // "OPERATOR_DETECT_SQLI"
      case 256: // "OPERATOR_DETECT_XSS"
      case 257: // "OPERATOR_ENDS_WITH"
      case 258: // "OPERATOR_EQ"
      case 259: // "OPERATOR_FUZZY_HASH"
      case 260: // "OPERATOR_GE"
      case 261: // "OPERATOR_GEOLOOKUP"
      case 262: // "OPERATOR_GSB_LOOKUP"
      case 263: // "OPERATOR_GT"
      case 264: // "OPERATOR_INSPECT_FILE"
      case 265: // "OPERATOR_IP_MATCH"
      case 266: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 267: // "OPERATOR_LE"
      case 268: // "OPERATOR_LT"
      case 269: // "OPERATOR_PM"
      case 270: // "OPERATOR_PM_FROM_FILE"
      case 271: // "OPERATOR_RBL"
      case 272: // "OPERATOR_RSUB"
      case 273: // "OPERATOR_RX"
      case 274: // "Operator RX (content only)"
      case 275: // "OPERATOR_STR_EQ"
      case 276: // "OPERATOR_STR_MATCH"
      case 277: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 278: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 279: // "OPERATOR_VALIDATE_DTD"
      case 280: // "OPERATOR_VALIDATE_HASH"
      case 281: // "OPERATOR_VALIDATE_SCHEMA"
      case 282: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 283: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 284: // "OPERATOR_VERIFY_CC"
      case 285: // "OPERATOR_VERIFY_CPF"
      case 286: // "OPERATOR_VERIFY_SSN"
      case 287: // "OPERATOR_WITHIN"
      case 288: // "OP_QUOTE"
      case 289: // "QUOTATION_MARK"
      case 290: // "RUN_TIME_VAR_BLD"
      case 291: // "RUN_TIME_VAR_DUR"
      case 292: // "RUN_TIME_VAR_HSV"
      case 293: // "RUN_TIME_VAR_REMOTE_USER"
      case 294: // "RUN_TIME_VAR_TIME"
      case 295: // "RUN_TIME_VAR_TIME_DAY"
      case 296: // "RUN_TIME_VAR_TIME_EPOCH"
      case 297: // "RUN_TIME_VAR_TIME_HOUR"
      case 298: // "RUN_TIME_VAR_TIME_MIN"
      case 299: // "RUN_TIME_VAR_TIME_MON"
      case 300: // "RUN_TIME_VAR_TIME_SEC"
      case 301: // "RUN_TIME_VAR_TIME_WDAY"
      case 302: // "RUN_TIME_VAR_TIME_YEAR"
      case 303: // "SETVAR_VARIABLE_PART"
      case 304: // "SETVAR_CONTENT_PART"
      case 305: // "VARIABLE"
      case 306: // "Dictionary element"
      case 307: // "Dictionary element, selected by regexp"
      case 322: // setvar_variable
      case 323: // setvar_content
        value.move< std::string > (that.value);
        break;

      case 314: // op
      case 315: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 324: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 319: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 320: // act
      case 321: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 317: // variables
      case 318: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 312: // actions
      case 313: // actions_may_quoted
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
      case 100: // "Accuracy"
      case 101: // "Allow"
      case 102: // "Append"
      case 103: // "AuditLog"
      case 104: // "Block"
      case 105: // "Capture"
      case 106: // "Chain"
      case 107: // "ACTION_CTL_AUDIT_ENGINE"
      case 108: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 109: // "ACTION_CTL_BDY_JSON"
      case 110: // "ACTION_CTL_BDY_XML"
      case 111: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 112: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 113: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 115: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 116: // "Deny"
      case 117: // "DeprecateVar"
      case 118: // "Drop"
      case 119: // "Exec"
      case 120: // "ExpireVar"
      case 121: // "Id"
      case 122: // "InitCol"
      case 123: // "Log"
      case 124: // "LogData"
      case 125: // "Maturity"
      case 126: // "Msg"
      case 127: // "MultiMatch"
      case 128: // "NoAuditLog"
      case 129: // "NoLog"
      case 130: // "Pass"
      case 131: // "Pause"
      case 132: // "Phase"
      case 133: // "Prepend"
      case 134: // "Proxy"
      case 135: // "Redirect"
      case 136: // "Rev"
      case 137: // "SanatiseArg"
      case 138: // "SanatiseMatched"
      case 139: // "SanatiseMatchedBytes"
      case 140: // "SanatiseRequestHeader"
      case 141: // "SanatiseResponseHeader"
      case 142: // "SetEnv"
      case 143: // "SetRsc"
      case 144: // "SetSid"
      case 145: // "SetUID"
      case 146: // "Severity"
      case 147: // "Skip"
      case 148: // "SkipAfter"
      case 149: // "Status"
      case 150: // "Tag"
      case 151: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 152: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 153: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 155: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 156: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 157: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 158: // "ACTION_TRANSFORMATION_LENGTH"
      case 159: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 160: // "ACTION_TRANSFORMATION_MD5"
      case 161: // "ACTION_TRANSFORMATION_NONE"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 163: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 164: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 172: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_SHA1"
      case 174: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 175: // "ACTION_TRANSFORMATION_TRIM"
      case 176: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 178: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 179: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 180: // "Ver"
      case 181: // "xmlns"
      case 182: // "CONFIG_COMPONENT_SIG"
      case 183: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 184: // "CONFIG_SEC_WEB_APP_ID"
      case 185: // "CONFIG_SEC_SERVER_SIG"
      case 186: // "CONFIG_DIR_AUDIT_DIR"
      case 187: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 188: // "CONFIG_DIR_AUDIT_ENG"
      case 189: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 190: // "CONFIG_DIR_AUDIT_LOG"
      case 191: // "CONFIG_DIR_AUDIT_LOG2"
      case 192: // "CONFIG_DIR_AUDIT_LOG_P"
      case 193: // "CONFIG_DIR_AUDIT_STS"
      case 194: // "CONFIG_DIR_AUDIT_TPE"
      case 195: // "CONFIG_DIR_DEBUG_LOG"
      case 196: // "CONFIG_DIR_DEBUG_LVL"
      case 197: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 198: // "CONFIG_DIR_GEO_DB"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 200: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 201: // "CONFIG_DIR_REQ_BODY"
      case 202: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 204: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 206: // "CONFIG_DIR_RES_BODY"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 208: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 209: // "CONFIG_DIR_RULE_ENG"
      case 210: // "CONFIG_DIR_SEC_ACTION"
      case 211: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 212: // "CONFIG_DIR_SEC_MARKER"
      case 213: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 214: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 215: // "CONFIG_SEC_HTTP_BLKEY"
      case 216: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 218: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 219: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 220: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 221: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 223: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 224: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 225: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 226: // "CONFIG_UPLOAD_DIR"
      case 227: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 228: // "CONFIG_UPLOAD_FILE_MODE"
      case 229: // "CONFIG_VALUE_ABORT"
      case 230: // "CONFIG_VALUE_DETC"
      case 231: // "CONFIG_VALUE_HTTPS"
      case 232: // "CONFIG_VALUE_OFF"
      case 233: // "CONFIG_VALUE_ON"
      case 234: // "CONFIG_VALUE_PARALLEL"
      case 235: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 236: // "CONFIG_VALUE_REJECT"
      case 237: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 238: // "CONFIG_VALUE_SERIAL"
      case 239: // "CONFIG_VALUE_WARN"
      case 240: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 241: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 242: // "CONGIG_DIR_SEC_ARG_SEP"
      case 243: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 244: // "CONGIG_DIR_SEC_DATA_DIR"
      case 245: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 246: // "CONGIG_DIR_SEC_TMP_DIR"
      case 247: // "DIRECTIVE"
      case 248: // "DIRECTIVE_SECRULESCRIPT"
      case 249: // "FREE_TEXT"
      case 250: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 251: // "OPERATOR"
      case 252: // "OPERATOR_BEGINS_WITH"
      case 253: // "OPERATOR_CONTAINS"
      case 254: // "OPERATOR_CONTAINS_WORD"
      case 255: // "OPERATOR_DETECT_SQLI"
      case 256: // "OPERATOR_DETECT_XSS"
      case 257: // "OPERATOR_ENDS_WITH"
      case 258: // "OPERATOR_EQ"
      case 259: // "OPERATOR_FUZZY_HASH"
      case 260: // "OPERATOR_GE"
      case 261: // "OPERATOR_GEOLOOKUP"
      case 262: // "OPERATOR_GSB_LOOKUP"
      case 263: // "OPERATOR_GT"
      case 264: // "OPERATOR_INSPECT_FILE"
      case 265: // "OPERATOR_IP_MATCH"
      case 266: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 267: // "OPERATOR_LE"
      case 268: // "OPERATOR_LT"
      case 269: // "OPERATOR_PM"
      case 270: // "OPERATOR_PM_FROM_FILE"
      case 271: // "OPERATOR_RBL"
      case 272: // "OPERATOR_RSUB"
      case 273: // "OPERATOR_RX"
      case 274: // "Operator RX (content only)"
      case 275: // "OPERATOR_STR_EQ"
      case 276: // "OPERATOR_STR_MATCH"
      case 277: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 278: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 279: // "OPERATOR_VALIDATE_DTD"
      case 280: // "OPERATOR_VALIDATE_HASH"
      case 281: // "OPERATOR_VALIDATE_SCHEMA"
      case 282: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 283: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 284: // "OPERATOR_VERIFY_CC"
      case 285: // "OPERATOR_VERIFY_CPF"
      case 286: // "OPERATOR_VERIFY_SSN"
      case 287: // "OPERATOR_WITHIN"
      case 288: // "OP_QUOTE"
      case 289: // "QUOTATION_MARK"
      case 290: // "RUN_TIME_VAR_BLD"
      case 291: // "RUN_TIME_VAR_DUR"
      case 292: // "RUN_TIME_VAR_HSV"
      case 293: // "RUN_TIME_VAR_REMOTE_USER"
      case 294: // "RUN_TIME_VAR_TIME"
      case 295: // "RUN_TIME_VAR_TIME_DAY"
      case 296: // "RUN_TIME_VAR_TIME_EPOCH"
      case 297: // "RUN_TIME_VAR_TIME_HOUR"
      case 298: // "RUN_TIME_VAR_TIME_MIN"
      case 299: // "RUN_TIME_VAR_TIME_MON"
      case 300: // "RUN_TIME_VAR_TIME_SEC"
      case 301: // "RUN_TIME_VAR_TIME_WDAY"
      case 302: // "RUN_TIME_VAR_TIME_YEAR"
      case 303: // "SETVAR_VARIABLE_PART"
      case 304: // "SETVAR_CONTENT_PART"
      case 305: // "VARIABLE"
      case 306: // "Dictionary element"
      case 307: // "Dictionary element, selected by regexp"
      case 322: // setvar_variable
      case 323: // setvar_content
        value.copy< std::string > (that.value);
        break;

      case 314: // op
      case 315: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 324: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (that.value);
        break;

      case 319: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 320: // act
      case 321: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 317: // variables
      case 318: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 312: // actions
      case 313: // actions_may_quoted
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
    #line 344 "/home/zimmerle/core-trustwave/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 899 "seclang-parser.cc" // lalr1.cc:741

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
      case 100: // "Accuracy"
      case 101: // "Allow"
      case 102: // "Append"
      case 103: // "AuditLog"
      case 104: // "Block"
      case 105: // "Capture"
      case 106: // "Chain"
      case 107: // "ACTION_CTL_AUDIT_ENGINE"
      case 108: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 109: // "ACTION_CTL_BDY_JSON"
      case 110: // "ACTION_CTL_BDY_XML"
      case 111: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 112: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 113: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 115: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 116: // "Deny"
      case 117: // "DeprecateVar"
      case 118: // "Drop"
      case 119: // "Exec"
      case 120: // "ExpireVar"
      case 121: // "Id"
      case 122: // "InitCol"
      case 123: // "Log"
      case 124: // "LogData"
      case 125: // "Maturity"
      case 126: // "Msg"
      case 127: // "MultiMatch"
      case 128: // "NoAuditLog"
      case 129: // "NoLog"
      case 130: // "Pass"
      case 131: // "Pause"
      case 132: // "Phase"
      case 133: // "Prepend"
      case 134: // "Proxy"
      case 135: // "Redirect"
      case 136: // "Rev"
      case 137: // "SanatiseArg"
      case 138: // "SanatiseMatched"
      case 139: // "SanatiseMatchedBytes"
      case 140: // "SanatiseRequestHeader"
      case 141: // "SanatiseResponseHeader"
      case 142: // "SetEnv"
      case 143: // "SetRsc"
      case 144: // "SetSid"
      case 145: // "SetUID"
      case 146: // "Severity"
      case 147: // "Skip"
      case 148: // "SkipAfter"
      case 149: // "Status"
      case 150: // "Tag"
      case 151: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 152: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 153: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 155: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 156: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 157: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 158: // "ACTION_TRANSFORMATION_LENGTH"
      case 159: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 160: // "ACTION_TRANSFORMATION_MD5"
      case 161: // "ACTION_TRANSFORMATION_NONE"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 163: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 164: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 172: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_SHA1"
      case 174: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 175: // "ACTION_TRANSFORMATION_TRIM"
      case 176: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 178: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 179: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 180: // "Ver"
      case 181: // "xmlns"
      case 182: // "CONFIG_COMPONENT_SIG"
      case 183: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 184: // "CONFIG_SEC_WEB_APP_ID"
      case 185: // "CONFIG_SEC_SERVER_SIG"
      case 186: // "CONFIG_DIR_AUDIT_DIR"
      case 187: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 188: // "CONFIG_DIR_AUDIT_ENG"
      case 189: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 190: // "CONFIG_DIR_AUDIT_LOG"
      case 191: // "CONFIG_DIR_AUDIT_LOG2"
      case 192: // "CONFIG_DIR_AUDIT_LOG_P"
      case 193: // "CONFIG_DIR_AUDIT_STS"
      case 194: // "CONFIG_DIR_AUDIT_TPE"
      case 195: // "CONFIG_DIR_DEBUG_LOG"
      case 196: // "CONFIG_DIR_DEBUG_LVL"
      case 197: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 198: // "CONFIG_DIR_GEO_DB"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 200: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 201: // "CONFIG_DIR_REQ_BODY"
      case 202: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 204: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 206: // "CONFIG_DIR_RES_BODY"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 208: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 209: // "CONFIG_DIR_RULE_ENG"
      case 210: // "CONFIG_DIR_SEC_ACTION"
      case 211: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 212: // "CONFIG_DIR_SEC_MARKER"
      case 213: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 214: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 215: // "CONFIG_SEC_HTTP_BLKEY"
      case 216: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 218: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 219: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 220: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 221: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 223: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 224: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 225: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 226: // "CONFIG_UPLOAD_DIR"
      case 227: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 228: // "CONFIG_UPLOAD_FILE_MODE"
      case 229: // "CONFIG_VALUE_ABORT"
      case 230: // "CONFIG_VALUE_DETC"
      case 231: // "CONFIG_VALUE_HTTPS"
      case 232: // "CONFIG_VALUE_OFF"
      case 233: // "CONFIG_VALUE_ON"
      case 234: // "CONFIG_VALUE_PARALLEL"
      case 235: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 236: // "CONFIG_VALUE_REJECT"
      case 237: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 238: // "CONFIG_VALUE_SERIAL"
      case 239: // "CONFIG_VALUE_WARN"
      case 240: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 241: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 242: // "CONGIG_DIR_SEC_ARG_SEP"
      case 243: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 244: // "CONGIG_DIR_SEC_DATA_DIR"
      case 245: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 246: // "CONGIG_DIR_SEC_TMP_DIR"
      case 247: // "DIRECTIVE"
      case 248: // "DIRECTIVE_SECRULESCRIPT"
      case 249: // "FREE_TEXT"
      case 250: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 251: // "OPERATOR"
      case 252: // "OPERATOR_BEGINS_WITH"
      case 253: // "OPERATOR_CONTAINS"
      case 254: // "OPERATOR_CONTAINS_WORD"
      case 255: // "OPERATOR_DETECT_SQLI"
      case 256: // "OPERATOR_DETECT_XSS"
      case 257: // "OPERATOR_ENDS_WITH"
      case 258: // "OPERATOR_EQ"
      case 259: // "OPERATOR_FUZZY_HASH"
      case 260: // "OPERATOR_GE"
      case 261: // "OPERATOR_GEOLOOKUP"
      case 262: // "OPERATOR_GSB_LOOKUP"
      case 263: // "OPERATOR_GT"
      case 264: // "OPERATOR_INSPECT_FILE"
      case 265: // "OPERATOR_IP_MATCH"
      case 266: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 267: // "OPERATOR_LE"
      case 268: // "OPERATOR_LT"
      case 269: // "OPERATOR_PM"
      case 270: // "OPERATOR_PM_FROM_FILE"
      case 271: // "OPERATOR_RBL"
      case 272: // "OPERATOR_RSUB"
      case 273: // "OPERATOR_RX"
      case 274: // "Operator RX (content only)"
      case 275: // "OPERATOR_STR_EQ"
      case 276: // "OPERATOR_STR_MATCH"
      case 277: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 278: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 279: // "OPERATOR_VALIDATE_DTD"
      case 280: // "OPERATOR_VALIDATE_HASH"
      case 281: // "OPERATOR_VALIDATE_SCHEMA"
      case 282: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 283: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 284: // "OPERATOR_VERIFY_CC"
      case 285: // "OPERATOR_VERIFY_CPF"
      case 286: // "OPERATOR_VERIFY_SSN"
      case 287: // "OPERATOR_WITHIN"
      case 288: // "OP_QUOTE"
      case 289: // "QUOTATION_MARK"
      case 290: // "RUN_TIME_VAR_BLD"
      case 291: // "RUN_TIME_VAR_DUR"
      case 292: // "RUN_TIME_VAR_HSV"
      case 293: // "RUN_TIME_VAR_REMOTE_USER"
      case 294: // "RUN_TIME_VAR_TIME"
      case 295: // "RUN_TIME_VAR_TIME_DAY"
      case 296: // "RUN_TIME_VAR_TIME_EPOCH"
      case 297: // "RUN_TIME_VAR_TIME_HOUR"
      case 298: // "RUN_TIME_VAR_TIME_MIN"
      case 299: // "RUN_TIME_VAR_TIME_MON"
      case 300: // "RUN_TIME_VAR_TIME_SEC"
      case 301: // "RUN_TIME_VAR_TIME_WDAY"
      case 302: // "RUN_TIME_VAR_TIME_YEAR"
      case 303: // "SETVAR_VARIABLE_PART"
      case 304: // "SETVAR_CONTENT_PART"
      case 305: // "VARIABLE"
      case 306: // "Dictionary element"
      case 307: // "Dictionary element, selected by regexp"
      case 322: // setvar_variable
      case 323: // setvar_content
        yylhs.value.build< std::string > ();
        break;

      case 314: // op
      case 315: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 324: // run_time_string
        yylhs.value.build< std::unique_ptr<RunTimeString> > ();
        break;

      case 319: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 320: // act
      case 321: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 317: // variables
      case 318: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 312: // actions
      case 313: // actions_may_quoted
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
#line 701 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1252 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 714 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 720 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1268 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 726 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1276 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 730 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 734 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 740 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 746 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 763 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1332 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 768 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1340 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 774 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1349 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 781 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1357 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 785 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1365 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 789 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1373 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 795 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1381 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 799 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1389 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 803 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1398 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 808 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 813 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1416 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 818 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1424 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 822 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1432 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 829 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1440 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 833 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 840 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1458 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 846 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1469 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 856 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1482 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 865 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1496 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 875 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 884 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("Rx", utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()), true));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1522 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 896 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1530 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 900 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1538 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 904 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1546 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 908 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1554 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 912 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1562 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 916 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::InspectFile(yystack_[0].value.as< std::string > ()));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 925 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::FuzzyHash(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1588 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 934 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1596 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 938 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1604 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 942 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1613 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 947 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1621 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 951 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1629 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 955 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 960 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 965 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 975 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1673 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 979 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1681 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 983 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1689 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 987 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1697 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 991 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1705 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 995 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1713 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 999 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1721 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 1003 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1729 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 1007 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1737 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 1011 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1745 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 1015 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1753 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 1019 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1761 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 1023 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1769 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 1027 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1777 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1031 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1040 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1798 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1044 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1806 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 69:
#line 1048 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1052 "seclang-parser.yy" // lalr1.cc:859
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
#line 1829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1067 "seclang-parser.yy" // lalr1.cc:859
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
#line 1857 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1091 "seclang-parser.yy" // lalr1.cc:859
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
#line 1880 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1110 "seclang-parser.yy" // lalr1.cc:859
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
#line 1899 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1125 "seclang-parser.yy" // lalr1.cc:859
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
#line 1927 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1149 "seclang-parser.yy" // lalr1.cc:859
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
#line 1978 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1196 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1986 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1200 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1994 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1204 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 2002 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1208 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 2010 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1212 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2018 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1216 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2026 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1220 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2034 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1224 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2042 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1228 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 2055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1237 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 2063 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1241 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secWebAppId.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 2072 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1246 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2081 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1251 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2090 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1256 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2099 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1261 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2108 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1266 "seclang-parser.yy" // lalr1.cc:859
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
#line 2125 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1279 "seclang-parser.yy" // lalr1.cc:859
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
#line 2142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1292 "seclang-parser.yy" // lalr1.cc:859
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
#line 2159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1305 "seclang-parser.yy" // lalr1.cc:859
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
#line 2176 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1318 "seclang-parser.yy" // lalr1.cc:859
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
#line 2193 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1331 "seclang-parser.yy" // lalr1.cc:859
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
#line 2223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1357 "seclang-parser.yy" // lalr1.cc:859
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
#line 2252 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1383 "seclang-parser.yy" // lalr1.cc:859
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
#line 2268 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1395 "seclang-parser.yy" // lalr1.cc:859
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
#line 2291 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1415 "seclang-parser.yy" // lalr1.cc:859
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
#line 2322 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1443 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2331 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1448 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2340 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1453 "seclang-parser.yy" // lalr1.cc:859
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1462 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2362 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1467 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2370 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1471 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2378 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 1475 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1479 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2394 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1483 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2402 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1487 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2410 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1493 "seclang-parser.yy" // lalr1.cc:859
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
#line 2426 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1505 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2436 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1511 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1515 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1525 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2459 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1528 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2468 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1536 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1540 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2484 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1547 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2493 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1552 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1558 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2513 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1564 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1570 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1577 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2545 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1587 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2553 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1591 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2561 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1595 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1599 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2577 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1603 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2585 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1607 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2593 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1611 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2601 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1615 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2609 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1619 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1623 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2625 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1627 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2633 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1631 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1635 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2649 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1639 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2657 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1643 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1647 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2673 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1651 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2681 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1655 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2689 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1659 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2697 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1663 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2705 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1667 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2713 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1671 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2721 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1675 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2729 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1679 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2737 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1683 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2745 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1687 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2753 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1691 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2761 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1695 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2769 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1699 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2777 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1703 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2785 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1707 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2793 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1711 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2801 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1715 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2809 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1719 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2817 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1723 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2825 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1727 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1731 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2841 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1735 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2849 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1739 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2857 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1743 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2865 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1747 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2873 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1751 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2881 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1755 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2889 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1759 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2897 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1763 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2905 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1767 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2913 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1771 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2921 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1775 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2929 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1779 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2937 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1783 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2945 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1787 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2953 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1791 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2961 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1795 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2969 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1799 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2977 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1803 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2985 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1807 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2993 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1811 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 3001 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1815 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3009 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3017 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1823 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 3025 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1827 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3033 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1831 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3041 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1835 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 3049 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1840 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3057 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1844 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3065 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1848 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 3073 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1853 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3081 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1857 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3089 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1861 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 3097 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1866 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3105 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1870 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3113 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1874 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 3121 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1879 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3129 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1883 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3137 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1887 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3145 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1892 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3153 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1896 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3161 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1900 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3169 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1904 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3177 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1908 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3185 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1912 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3193 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1917 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1921 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1925 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3217 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1930 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1934 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1938 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1943 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1948 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1952 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3265 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1956 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3273 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1960 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3281 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1964 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3289 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1968 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3297 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1972 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3305 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1976 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3313 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1980 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3321 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1984 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3329 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1988 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1992 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3345 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1996 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 2000 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3361 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 2004 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3369 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2008 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3377 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2012 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3385 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2016 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2020 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3401 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2024 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3409 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2028 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3417 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2032 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3425 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2036 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3433 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2040 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3441 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2044 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3449 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2048 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2052 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2056 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2060 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3481 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2064 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3489 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2068 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2072 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3505 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2076 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3513 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2080 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3521 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2084 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3529 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2088 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3537 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2092 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3545 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2096 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3553 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2100 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3561 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2104 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2108 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3577 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2112 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3585 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2116 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3593 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2120 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3601 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2124 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3609 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2128 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2132 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3625 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2136 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3633 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2140 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2144 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3649 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2148 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3657 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2152 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::WebAppId());
      }
#line 3665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2156 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3676 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2164 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2171 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2178 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3709 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2185 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2192 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3731 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2199 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2206 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3753 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2213 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2220 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3775 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2227 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3786 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2234 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2241 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3808 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2251 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3816 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2255 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3824 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2259 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3832 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2263 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3840 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2267 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3848 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2271 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3856 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2275 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3864 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2279 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3873 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2284 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3882 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2289 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3891 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2294 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3899 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2298 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3907 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2302 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3915 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2306 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3924 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2311 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3933 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2316 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2320 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3949 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2324 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3957 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2328 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3965 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2332 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2336 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3981 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2340 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3989 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2344 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3997 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2348 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 4005 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2352 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 4013 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2356 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2361 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 4030 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2365 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4039 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2370 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 4047 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2374 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[1].value.as< std::string > (), std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2378 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4063 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2382 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 4071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2386 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 4079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2390 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2394 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2398 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2402 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2406 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2410 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4127 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2414 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4135 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2418 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4143 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2422 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2426 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2430 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4167 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2434 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4175 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2438 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2442 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2446 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2450 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4207 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2454 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2458 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2462 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2466 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2470 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2474 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2478 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2482 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2486 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2490 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(std::move(yystack_[0].value.as< std::unique_ptr<RunTimeString> > ())));
      }
#line 4287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2494 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2498 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2502 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2506 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2510 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2514 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4335 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2518 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4343 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2522 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2526 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2530 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2534 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2538 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2542 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2546 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2550 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2554 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2558 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4423 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2562 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 2566 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 2570 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4447 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 2574 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 2578 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4463 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 2582 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4471 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 2586 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4479 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 2590 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 2594 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4495 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 2598 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 373:
#line 2602 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4511 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 374:
#line 2606 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4519 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 375:
#line 2610 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 2614 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 2621 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4543 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 2625 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 2629 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4559 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 2633 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4567 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 2637 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 2644 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 383:
#line 2649 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 384:
#line 2656 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 385:
#line 2661 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 386:
#line 2668 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 387:
#line 2673 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[1].value.as< std::unique_ptr<RunTimeString> > ()->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(yystack_[1].value.as< std::unique_ptr<RunTimeString> > ());
      }
#line 4625 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 388:
#line 2678 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendText(yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 389:
#line 2684 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<RunTimeString> r(new RunTimeString());
        r->appendVar(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<RunTimeString> > () = std::move(r);
      }
#line 4645 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4649 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -274;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     999,  -274,  -208,  -274,    25,  -274,  -274,  -274,  -274,  -274,
    -274,  -223,  -274,  -274,  -274,  -274,  -274,  -219,  -274,  -274,
    -274,  -274,  -274,  -274,  -108,  -274,  -274,  -109,  -274,  -104,
    -274,  -105,  -204,  1224,  1224,  -274,  -274,  -274,  -274,  -221,
    -274,  -274,  -274,    24,    24,    24,  1224,  -100,    -9,  -274,
    -274,  -274,    -7,  -274,  -274,  -274,  -274,  -274,  -274,    24,
    1224,  1066,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,   -84,  -115,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -210,  -274,  -274,  -274,     1,
       3,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
     504,  -274,   504,  -274,   504,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,   504,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,   504,   504,   504,  -274,  -274,  -274,  -274,   504,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    1315,  -274,    10,  -274,  -274,  -274,  -274,   878,   878,   -69,
     -64,   -62,   -59,   -57,   -55,   -53,   -51,   -49,   -47,   -35,
     -33,   -31,   -28,   -26,   -24,  -274,   -22,   -16,   -14,   -12,
    -274,  -274,   -10,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,    -8,  -274,  -274,  -274,  -274,  -274,    -6,  -274,  -274,
    -274,    -4,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,    -1,     2,     4,    21,    23,    26,   128,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,    14,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,   584,  -274,  -274,  -274,  -273,
    -273,  -274,    27,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,   585,   585,   585,   585,   585,
     585,   585,   585,     0,  1315,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,    -2,   423,    84,
    -137,  -133,   -19,  -274,  -274,   -17,    12,    28,    37,  -274,
      39,    55,    58,    63,    82,    85,   125,   126,   127,   129,
     130,   131,  -274,   132,   133,  -274,   134,   135,   139,   140,
    -274,  -274,   141,   142,   143,   144,  1224,  -274,  -274,  -274,
     -83,   -83,   -83,  -274,  -274,  -274,  -274,  -274,   878,   878,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,   -83,  -274,  -274,  -274,  -274,  -274,  -274
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
      83,   108,   109,    80,    78,    79,     0,     0,   287,   288,
     289,   290,   291,   292,   293,     0,   297,   298,   299,     0,
       0,   307,   308,   309,   310,   311,   312,   313,   314,   315,
       0,   318,     0,   319,     0,   321,   322,   323,   324,   325,
     326,   327,   328,     0,   330,   331,   332,   333,   334,   335,
     336,     0,     0,     0,   341,   342,   343,   344,     0,   352,
     362,   368,   355,   356,   366,   367,   372,   357,   354,   361,
     371,   370,   350,   349,   348,   375,   374,   365,   363,   376,
     364,   353,   351,   369,   358,   360,   359,   373,   346,   347,
       0,    74,    29,    31,    76,   110,   111,     0,     0,   136,
     139,   142,   145,   148,   151,   154,   157,   160,   163,   166,
     169,   172,   175,   178,   181,   225,   214,   184,   211,   217,
     226,   227,   193,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   248,   247,   251,   250,   249,   252,   254,   253,
     255,   220,   256,   257,   258,   260,   259,   196,   261,   262,
     221,   224,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   273,   272,   199,   202,   205,   208,   187,   190,     0,
     275,   274,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,    95,   126,   131,    96,    97,    98,    22,
      21,    27,    26,   116,   117,     0,    75,     1,     3,     0,
     382,   340,   378,   306,   305,   304,   295,   294,   296,   301,
     300,   303,   302,   388,   389,   316,   317,   320,   329,   337,
     338,   339,   345,     0,     0,   133,   132,   134,   135,   137,
     138,   140,   141,   143,   144,   146,   147,   149,   150,   152,
     153,   155,   156,   158,   159,   161,   162,   164,   165,   167,
     168,   170,   171,   173,   174,   176,   177,   179,   180,   212,
     213,   182,   183,   209,   210,   215,   216,   191,   192,   218,
     219,   194,   195,   222,   223,   197,   198,   200,   201,   203,
     204,   206,   207,   185,   186,   188,   189,     0,     0,     0,
       0,     0,     0,    37,    38,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,     0,    36,     0,     0,     0,     0,
      39,    40,     0,     0,     0,     0,    73,    32,   377,   383,
       0,     0,     0,   386,   387,    28,    30,   127,     0,     0,
     128,    35,    33,    69,    54,    53,    55,    56,    42,    57,
      50,    58,    41,    60,    59,    61,    62,    64,    63,    65,
      51,    66,    67,    68,    43,    44,    45,    46,    47,    48,
      49,    52,    72,   384,   379,   380,   381,   130,   129,   385
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -274,  -274,   166,  -274,   -29,   207,  -274,    15,  -274,   -38,
     136,   -43,    86,  -274,    83,  -193,   108
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    61,    62,    63,   171,   172,   416,   417,    64,   273,
     274,   304,   173,   291,   292,   464,   305
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     275,   275,   275,   314,   378,   174,   276,   277,   175,    69,
      70,   289,    72,   314,    71,    73,   275,   278,   176,    74,
     378,   285,   296,   297,    65,    66,    83,   298,    84,    85,
     290,   286,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   433,   257,   258,   293,   434,   294,   295,   420,
     421,   422,    67,    68,    75,    76,    77,    78,    79,    80,
      81,    82,   279,   280,   315,   316,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   275,   257,   258,   290,
     306,   463,   307,   281,   282,   283,   284,   288,   465,   466,
     435,   308,   436,   299,   300,   301,   302,   317,   318,   309,
     310,   311,   319,   320,   321,   322,   312,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   437,   424,   424,   424,   424,   424,   424,   424,   424,
     469,   337,   338,   339,   340,   341,   342,   438,   343,   344,
     345,   346,   347,   348,   349,   350,   439,   427,   440,   425,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   441,   365,   366,   442,   367,   368,
     369,   370,   443,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   371,   372,   373,
     374,   444,   375,   376,   445,   430,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   431,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   418,   419,   446,   447,   448,   313,   449,   450,
     451,   452,   453,   454,   455,   467,   468,   462,   456,   457,
     458,   459,   460,   461,   432,   377,     0,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   428,   429,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,     0,   257,   258,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,     0,   257,   258,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,     0,   257,   258,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   423,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,     0,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,     0,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,     2,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   287,     0,     0,     0,
       2,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     0,     0,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,    59,    60,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    86,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    86,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      43,    44,    45,     3,     6,    34,    44,    45,   229,   232,
     233,    95,   231,     3,   237,   234,    59,    46,   239,   238,
       6,    59,   232,   233,   232,   233,   230,   237,   232,   233,
     303,    60,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   249,    89,    90,   230,   249,   232,   233,    92,
      93,    94,    97,    98,   232,   233,   235,   236,   232,   233,
     235,   236,   232,   233,   177,   178,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   259,    89,    90,   303,
     112,   304,   114,   232,   233,   232,   233,    61,   421,   422,
     249,   123,   249,   232,   233,   232,   233,   306,   307,   131,
     132,   133,   306,   307,   306,   307,   138,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   249,   305,   306,   307,   308,   309,   310,   311,   312,
     463,   306,   307,   306,   307,   306,   307,   249,   306,   307,
     306,   307,   306,   307,   306,   307,   249,   289,   249,   289,
     306,   307,   306,   307,   306,   307,   306,   307,   306,   307,
     306,   307,   306,   307,   249,   306,   307,   249,   306,   307,
     306,   307,   249,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   306,   307,   306,
     307,   249,   306,   307,   249,   378,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   289,   290,   249,   249,   249,   170,   249,   249,
     249,   249,   249,   249,   249,   428,   429,   416,   249,   249,
     249,   249,   249,   249,   379,   259,    -1,    -1,    -1,    -1,
     314,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   250,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    -1,    -1,    -1,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
      -1,    -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,    -1,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   240,   241,   242,   243,
     244,   245,   246,   247,   248,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   289
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,    96,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   309,   310,   311,   316,   232,   233,    97,    98,   232,
     233,   237,   231,   234,   238,   232,   233,   235,   236,   232,
     233,   235,   236,   230,   232,   233,    91,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     289,   312,   313,   320,   312,   229,   239,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    89,    90,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   317,   318,   319,   317,   317,   312,   232,
     233,   232,   233,   232,   233,   317,   312,     0,   310,    95,
     303,   321,   322,   230,   232,   233,   232,   233,   237,   232,
     233,   232,   233,   250,   319,   324,   324,   324,   324,   324,
     324,   324,   324,   313,     3,   319,   319,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   306,   307,   306,
     307,   306,   307,   306,   307,   306,   307,   318,     6,    95,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   314,   315,   322,   322,
      92,    93,    94,   250,   319,   289,   320,   289,     8,     9,
     319,   274,   315,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   312,   304,   323,   323,   323,   319,   319,   323
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   308,   309,   309,   309,   310,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   312,
     313,   313,   314,   314,   314,   314,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   317,   317,   318,   318,
     318,   318,   318,   318,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   321,   321,   321,
     321,   321,   322,   322,   323,   323,   324,   324,   324,   324
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
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       3,     3,     1,     2,     1,     2,     2,     2,     1,     1
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
  "\"MULTIPART_CRLF_LF_LINES\"", "\"MULTIPART_DATA_AFTER\"",
  "\"MULTIPART_FILE_LIMIT_EXCEEDED\"", "\"MULTIPART_HEADER_FOLDING\"",
  "\"MULTIPART_INVALID_HEADER_FOLDING\"", "\"MULTIPART_INVALID_QUOTING\"",
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
  "SETVAR_OPERATION_EQUALS_MINUS", "\"NOT\"", "CONFIG_DIR_AUDIT_LOG_FMT",
  "JSON", "NATIVE", "\"ACTION_CTL_RULE_ENGINE\"", "\"Accuracy\"",
  "\"Allow\"", "\"Append\"", "\"AuditLog\"", "\"Block\"", "\"Capture\"",
  "\"Chain\"", "\"ACTION_CTL_AUDIT_ENGINE\"",
  "\"ACTION_CTL_AUDIT_LOG_PARTS\"", "\"ACTION_CTL_BDY_JSON\"",
  "\"ACTION_CTL_BDY_XML\"", "\"ACTION_CTL_FORCE_REQ_BODY_VAR\"",
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
  "\"DIRECTIVE_SECRULESCRIPT\"", "\"FREE_TEXT\"",
  "\"FREE_TEXT_QUOTE_MACRO_EXPANSION\"", "\"OPERATOR\"",
  "\"OPERATOR_BEGINS_WITH\"", "\"OPERATOR_CONTAINS\"",
  "\"OPERATOR_CONTAINS_WORD\"", "\"OPERATOR_DETECT_SQLI\"",
  "\"OPERATOR_DETECT_XSS\"", "\"OPERATOR_ENDS_WITH\"", "\"OPERATOR_EQ\"",
  "\"OPERATOR_FUZZY_HASH\"", "\"OPERATOR_GE\"", "\"OPERATOR_GEOLOOKUP\"",
  "\"OPERATOR_GSB_LOOKUP\"", "\"OPERATOR_GT\"",
  "\"OPERATOR_INSPECT_FILE\"", "\"OPERATOR_IP_MATCH\"",
  "\"OPERATOR_IP_MATCH_FROM_FILE\"", "\"OPERATOR_LE\"", "\"OPERATOR_LT\"",
  "\"OPERATOR_PM\"", "\"OPERATOR_PM_FROM_FILE\"", "\"OPERATOR_RBL\"",
  "\"OPERATOR_RSUB\"", "\"OPERATOR_RX\"", "\"Operator RX (content only)\"",
  "\"OPERATOR_STR_EQ\"", "\"OPERATOR_STR_MATCH\"",
  "\"OPERATOR_UNCONDITIONAL_MATCH\"", "\"OPERATOR_VALIDATE_BYTE_RANGE\"",
  "\"OPERATOR_VALIDATE_DTD\"", "\"OPERATOR_VALIDATE_HASH\"",
  "\"OPERATOR_VALIDATE_SCHEMA\"", "\"OPERATOR_VALIDATE_URL_ENCODING\"",
  "\"OPERATOR_VALIDATE_UTF8_ENCODING\"", "\"OPERATOR_VERIFY_CC\"",
  "\"OPERATOR_VERIFY_CPF\"", "\"OPERATOR_VERIFY_SSN\"",
  "\"OPERATOR_WITHIN\"", "\"OP_QUOTE\"", "\"QUOTATION_MARK\"",
  "\"RUN_TIME_VAR_BLD\"", "\"RUN_TIME_VAR_DUR\"", "\"RUN_TIME_VAR_HSV\"",
  "\"RUN_TIME_VAR_REMOTE_USER\"", "\"RUN_TIME_VAR_TIME\"",
  "\"RUN_TIME_VAR_TIME_DAY\"", "\"RUN_TIME_VAR_TIME_EPOCH\"",
  "\"RUN_TIME_VAR_TIME_HOUR\"", "\"RUN_TIME_VAR_TIME_MIN\"",
  "\"RUN_TIME_VAR_TIME_MON\"", "\"RUN_TIME_VAR_TIME_SEC\"",
  "\"RUN_TIME_VAR_TIME_WDAY\"", "\"RUN_TIME_VAR_TIME_YEAR\"",
  "\"SETVAR_VARIABLE_PART\"", "\"SETVAR_CONTENT_PART\"", "\"VARIABLE\"",
  "\"Dictionary element\"", "\"Dictionary element, selected by regexp\"",
  "$accept", "input", "line", "audit_log", "actions", "actions_may_quoted",
  "op", "op_before_init", "expression", "variables",
  "variables_may_be_quoted", "var", "act", "setvar_action",
  "setvar_variable", "setvar_content", "run_time_string", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   700,   700,   704,   705,   708,   713,   719,   725,   729,
     733,   739,   745,   751,   757,   762,   767,   773,   780,   784,
     788,   794,   798,   802,   807,   812,   817,   821,   828,   832,
     839,   845,   855,   864,   874,   883,   895,   899,   903,   907,
     911,   915,   924,   933,   937,   941,   946,   950,   954,   959,
     964,   969,   974,   978,   982,   986,   990,   994,   998,  1002,
    1006,  1010,  1014,  1018,  1022,  1026,  1030,  1039,  1043,  1047,
    1051,  1065,  1066,  1090,  1109,  1124,  1148,  1195,  1199,  1203,
    1207,  1211,  1215,  1219,  1223,  1227,  1236,  1240,  1245,  1250,
    1255,  1260,  1265,  1278,  1291,  1304,  1317,  1330,  1356,  1382,
    1394,  1414,  1442,  1447,  1452,  1461,  1466,  1470,  1474,  1478,
    1482,  1486,  1490,  1491,  1492,  1504,  1510,  1514,  1518,  1519,
    1520,  1521,  1522,  1523,  1524,  1527,  1535,  1539,  1546,  1551,
    1557,  1563,  1569,  1576,  1586,  1590,  1594,  1598,  1602,  1606,
    1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,
    1650,  1654,  1658,  1662,  1666,  1670,  1674,  1678,  1682,  1686,
    1690,  1694,  1698,  1702,  1706,  1710,  1714,  1718,  1722,  1726,
    1730,  1734,  1738,  1742,  1746,  1750,  1754,  1758,  1762,  1766,
    1770,  1774,  1778,  1782,  1786,  1790,  1794,  1798,  1802,  1806,
    1810,  1814,  1818,  1822,  1826,  1830,  1834,  1839,  1843,  1847,
    1852,  1856,  1860,  1865,  1869,  1873,  1878,  1882,  1886,  1891,
    1895,  1899,  1903,  1907,  1911,  1916,  1920,  1924,  1929,  1933,
    1937,  1942,  1947,  1951,  1955,  1959,  1963,  1967,  1971,  1975,
    1979,  1983,  1987,  1991,  1995,  1999,  2003,  2007,  2011,  2015,
    2019,  2023,  2027,  2031,  2035,  2039,  2043,  2047,  2051,  2055,
    2059,  2063,  2067,  2071,  2075,  2079,  2083,  2087,  2091,  2095,
    2099,  2103,  2107,  2111,  2115,  2119,  2123,  2127,  2131,  2135,
    2139,  2143,  2147,  2151,  2155,  2163,  2170,  2177,  2184,  2191,
    2198,  2205,  2212,  2219,  2226,  2233,  2240,  2250,  2254,  2258,
    2262,  2266,  2270,  2274,  2278,  2283,  2288,  2293,  2297,  2301,
    2305,  2310,  2315,  2319,  2323,  2327,  2331,  2335,  2339,  2343,
    2347,  2351,  2355,  2360,  2364,  2369,  2373,  2377,  2381,  2385,
    2389,  2393,  2397,  2401,  2405,  2409,  2413,  2417,  2421,  2425,
    2429,  2433,  2437,  2441,  2445,  2449,  2453,  2457,  2461,  2465,
    2469,  2473,  2477,  2481,  2485,  2489,  2493,  2497,  2501,  2505,
    2509,  2513,  2517,  2521,  2525,  2529,  2533,  2537,  2541,  2545,
    2549,  2553,  2557,  2561,  2565,  2569,  2573,  2577,  2581,  2585,
    2589,  2593,  2597,  2601,  2605,  2609,  2613,  2620,  2624,  2628,
    2632,  2636,  2643,  2648,  2655,  2660,  2667,  2672,  2677,  2683
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
#line 5712 "seclang-parser.cc" // lalr1.cc:1167
#line 2690 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
