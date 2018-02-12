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
      case 151: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 152: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 153: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 154: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 155: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 156: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 158: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 159: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 160: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 161: // "ACTION_TRANSFORMATION_LENGTH"
      case 162: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 163: // "ACTION_TRANSFORMATION_MD5"
      case 164: // "ACTION_TRANSFORMATION_NONE"
      case 165: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 166: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 167: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 168: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 169: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 172: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 174: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 175: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 176: // "ACTION_TRANSFORMATION_SHA1"
      case 177: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 178: // "ACTION_TRANSFORMATION_TRIM"
      case 179: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 180: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 181: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 182: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 183: // "Ver"
      case 184: // "xmlns"
      case 185: // "CONFIG_COMPONENT_SIG"
      case 186: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 187: // "CONFIG_SEC_WEB_APP_ID"
      case 188: // "CONFIG_SEC_SERVER_SIG"
      case 189: // "CONFIG_DIR_AUDIT_DIR"
      case 190: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 191: // "CONFIG_DIR_AUDIT_ENG"
      case 192: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 193: // "CONFIG_DIR_AUDIT_LOG"
      case 194: // "CONFIG_DIR_AUDIT_LOG2"
      case 195: // "CONFIG_DIR_AUDIT_LOG_P"
      case 196: // "CONFIG_DIR_AUDIT_STS"
      case 197: // "CONFIG_DIR_AUDIT_TPE"
      case 198: // "CONFIG_DIR_DEBUG_LOG"
      case 199: // "CONFIG_DIR_DEBUG_LVL"
      case 200: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 201: // "CONFIG_DIR_GEO_DB"
      case 202: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 203: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 204: // "CONFIG_DIR_REQ_BODY"
      case 205: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 206: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 207: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 209: // "CONFIG_DIR_RES_BODY"
      case 210: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 211: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 212: // "CONFIG_DIR_RULE_ENG"
      case 213: // "CONFIG_DIR_SEC_ACTION"
      case 214: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 215: // "CONFIG_DIR_SEC_MARKER"
      case 216: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 217: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 218: // "CONFIG_SEC_HTTP_BLKEY"
      case 219: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 220: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 221: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 223: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 224: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 225: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 226: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 227: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 228: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 229: // "CONFIG_UPLOAD_DIR"
      case 230: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 231: // "CONFIG_UPLOAD_FILE_MODE"
      case 232: // "CONFIG_VALUE_ABORT"
      case 233: // "CONFIG_VALUE_DETC"
      case 234: // "CONFIG_VALUE_HTTPS"
      case 235: // "CONFIG_VALUE_OFF"
      case 236: // "CONFIG_VALUE_ON"
      case 237: // "CONFIG_VALUE_PARALLEL"
      case 238: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 239: // "CONFIG_VALUE_REJECT"
      case 240: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 241: // "CONFIG_VALUE_SERIAL"
      case 242: // "CONFIG_VALUE_WARN"
      case 243: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 244: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 245: // "CONGIG_DIR_SEC_ARG_SEP"
      case 246: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 247: // "CONGIG_DIR_SEC_DATA_DIR"
      case 248: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 249: // "CONGIG_DIR_SEC_TMP_DIR"
      case 250: // "DIRECTIVE"
      case 251: // "DIRECTIVE_SECRULESCRIPT"
      case 252: // "FREE_TEXT"
      case 253: // "OPERATOR"
      case 254: // "OPERATOR_BEGINS_WITH"
      case 255: // "OPERATOR_CONTAINS"
      case 256: // "OPERATOR_CONTAINS_WORD"
      case 257: // "OPERATOR_DETECT_SQLI"
      case 258: // "OPERATOR_DETECT_XSS"
      case 259: // "OPERATOR_ENDS_WITH"
      case 260: // "OPERATOR_EQ"
      case 261: // "OPERATOR_FUZZY_HASH"
      case 262: // "OPERATOR_GE"
      case 263: // "OPERATOR_GEOLOOKUP"
      case 264: // "OPERATOR_GSB_LOOKUP"
      case 265: // "OPERATOR_GT"
      case 266: // "OPERATOR_INSPECT_FILE"
      case 267: // "OPERATOR_IP_MATCH"
      case 268: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 269: // "OPERATOR_LE"
      case 270: // "OPERATOR_LT"
      case 271: // "OPERATOR_PM"
      case 272: // "OPERATOR_PM_FROM_FILE"
      case 273: // "OPERATOR_RBL"
      case 274: // "OPERATOR_RSUB"
      case 275: // "OPERATOR_RX"
      case 276: // "Operator RX (content only)"
      case 277: // "OPERATOR_STR_EQ"
      case 278: // "OPERATOR_STR_MATCH"
      case 279: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 280: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 281: // "OPERATOR_VALIDATE_DTD"
      case 282: // "OPERATOR_VALIDATE_HASH"
      case 283: // "OPERATOR_VALIDATE_SCHEMA"
      case 284: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 285: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 286: // "OPERATOR_VERIFY_CC"
      case 287: // "OPERATOR_VERIFY_CPF"
      case 288: // "OPERATOR_VERIFY_SSN"
      case 289: // "OPERATOR_WITHIN"
      case 290: // "OP_QUOTE"
      case 291: // "QUOTATION_MARK"
      case 292: // "RUN_TIME_VAR_BLD"
      case 293: // "RUN_TIME_VAR_DUR"
      case 294: // "RUN_TIME_VAR_HSV"
      case 295: // "RUN_TIME_VAR_REMOTE_USER"
      case 296: // "RUN_TIME_VAR_TIME"
      case 297: // "RUN_TIME_VAR_TIME_DAY"
      case 298: // "RUN_TIME_VAR_TIME_EPOCH"
      case 299: // "RUN_TIME_VAR_TIME_HOUR"
      case 300: // "RUN_TIME_VAR_TIME_MIN"
      case 301: // "RUN_TIME_VAR_TIME_MON"
      case 302: // "RUN_TIME_VAR_TIME_SEC"
      case 303: // "RUN_TIME_VAR_TIME_WDAY"
      case 304: // "RUN_TIME_VAR_TIME_YEAR"
      case 305: // "SETVAR_VARIABLE_PART"
      case 306: // "SETVAR_CONTENT_PART"
      case 307: // "VARIABLE"
      case 308: // "Dictionary element"
      case 309: // "Dictionary element, selected by regexp"
      case 324: // setvar_variable
      case 325: // setvar_content
        value.move< std::string > (that.value);
        break;

      case 316: // op
      case 317: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 321: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 322: // act
      case 323: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 319: // variables
      case 320: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 314: // actions
      case 315: // actions_may_quoted
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
      case 151: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 152: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 153: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 154: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 155: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 156: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 158: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 159: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 160: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 161: // "ACTION_TRANSFORMATION_LENGTH"
      case 162: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 163: // "ACTION_TRANSFORMATION_MD5"
      case 164: // "ACTION_TRANSFORMATION_NONE"
      case 165: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 166: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 167: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 168: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 169: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 172: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 174: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 175: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 176: // "ACTION_TRANSFORMATION_SHA1"
      case 177: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 178: // "ACTION_TRANSFORMATION_TRIM"
      case 179: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 180: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 181: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 182: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 183: // "Ver"
      case 184: // "xmlns"
      case 185: // "CONFIG_COMPONENT_SIG"
      case 186: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 187: // "CONFIG_SEC_WEB_APP_ID"
      case 188: // "CONFIG_SEC_SERVER_SIG"
      case 189: // "CONFIG_DIR_AUDIT_DIR"
      case 190: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 191: // "CONFIG_DIR_AUDIT_ENG"
      case 192: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 193: // "CONFIG_DIR_AUDIT_LOG"
      case 194: // "CONFIG_DIR_AUDIT_LOG2"
      case 195: // "CONFIG_DIR_AUDIT_LOG_P"
      case 196: // "CONFIG_DIR_AUDIT_STS"
      case 197: // "CONFIG_DIR_AUDIT_TPE"
      case 198: // "CONFIG_DIR_DEBUG_LOG"
      case 199: // "CONFIG_DIR_DEBUG_LVL"
      case 200: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 201: // "CONFIG_DIR_GEO_DB"
      case 202: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 203: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 204: // "CONFIG_DIR_REQ_BODY"
      case 205: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 206: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 207: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 209: // "CONFIG_DIR_RES_BODY"
      case 210: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 211: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 212: // "CONFIG_DIR_RULE_ENG"
      case 213: // "CONFIG_DIR_SEC_ACTION"
      case 214: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 215: // "CONFIG_DIR_SEC_MARKER"
      case 216: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 217: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 218: // "CONFIG_SEC_HTTP_BLKEY"
      case 219: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 220: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 221: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 223: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 224: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 225: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 226: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 227: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 228: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 229: // "CONFIG_UPLOAD_DIR"
      case 230: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 231: // "CONFIG_UPLOAD_FILE_MODE"
      case 232: // "CONFIG_VALUE_ABORT"
      case 233: // "CONFIG_VALUE_DETC"
      case 234: // "CONFIG_VALUE_HTTPS"
      case 235: // "CONFIG_VALUE_OFF"
      case 236: // "CONFIG_VALUE_ON"
      case 237: // "CONFIG_VALUE_PARALLEL"
      case 238: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 239: // "CONFIG_VALUE_REJECT"
      case 240: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 241: // "CONFIG_VALUE_SERIAL"
      case 242: // "CONFIG_VALUE_WARN"
      case 243: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 244: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 245: // "CONGIG_DIR_SEC_ARG_SEP"
      case 246: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 247: // "CONGIG_DIR_SEC_DATA_DIR"
      case 248: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 249: // "CONGIG_DIR_SEC_TMP_DIR"
      case 250: // "DIRECTIVE"
      case 251: // "DIRECTIVE_SECRULESCRIPT"
      case 252: // "FREE_TEXT"
      case 253: // "OPERATOR"
      case 254: // "OPERATOR_BEGINS_WITH"
      case 255: // "OPERATOR_CONTAINS"
      case 256: // "OPERATOR_CONTAINS_WORD"
      case 257: // "OPERATOR_DETECT_SQLI"
      case 258: // "OPERATOR_DETECT_XSS"
      case 259: // "OPERATOR_ENDS_WITH"
      case 260: // "OPERATOR_EQ"
      case 261: // "OPERATOR_FUZZY_HASH"
      case 262: // "OPERATOR_GE"
      case 263: // "OPERATOR_GEOLOOKUP"
      case 264: // "OPERATOR_GSB_LOOKUP"
      case 265: // "OPERATOR_GT"
      case 266: // "OPERATOR_INSPECT_FILE"
      case 267: // "OPERATOR_IP_MATCH"
      case 268: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 269: // "OPERATOR_LE"
      case 270: // "OPERATOR_LT"
      case 271: // "OPERATOR_PM"
      case 272: // "OPERATOR_PM_FROM_FILE"
      case 273: // "OPERATOR_RBL"
      case 274: // "OPERATOR_RSUB"
      case 275: // "OPERATOR_RX"
      case 276: // "Operator RX (content only)"
      case 277: // "OPERATOR_STR_EQ"
      case 278: // "OPERATOR_STR_MATCH"
      case 279: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 280: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 281: // "OPERATOR_VALIDATE_DTD"
      case 282: // "OPERATOR_VALIDATE_HASH"
      case 283: // "OPERATOR_VALIDATE_SCHEMA"
      case 284: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 285: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 286: // "OPERATOR_VERIFY_CC"
      case 287: // "OPERATOR_VERIFY_CPF"
      case 288: // "OPERATOR_VERIFY_SSN"
      case 289: // "OPERATOR_WITHIN"
      case 290: // "OP_QUOTE"
      case 291: // "QUOTATION_MARK"
      case 292: // "RUN_TIME_VAR_BLD"
      case 293: // "RUN_TIME_VAR_DUR"
      case 294: // "RUN_TIME_VAR_HSV"
      case 295: // "RUN_TIME_VAR_REMOTE_USER"
      case 296: // "RUN_TIME_VAR_TIME"
      case 297: // "RUN_TIME_VAR_TIME_DAY"
      case 298: // "RUN_TIME_VAR_TIME_EPOCH"
      case 299: // "RUN_TIME_VAR_TIME_HOUR"
      case 300: // "RUN_TIME_VAR_TIME_MIN"
      case 301: // "RUN_TIME_VAR_TIME_MON"
      case 302: // "RUN_TIME_VAR_TIME_SEC"
      case 303: // "RUN_TIME_VAR_TIME_WDAY"
      case 304: // "RUN_TIME_VAR_TIME_YEAR"
      case 305: // "SETVAR_VARIABLE_PART"
      case 306: // "SETVAR_CONTENT_PART"
      case 307: // "VARIABLE"
      case 308: // "Dictionary element"
      case 309: // "Dictionary element, selected by regexp"
      case 324: // setvar_variable
      case 325: // setvar_content
        value.copy< std::string > (that.value);
        break;

      case 316: // op
      case 317: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 321: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 322: // act
      case 323: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 319: // variables
      case 320: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 314: // actions
      case 315: // actions_may_quoted
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
    #line 344 "/home/vhora/ModSecurity-v3/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 895 "seclang-parser.cc" // lalr1.cc:741

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
      case 151: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 152: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 153: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 154: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 155: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 156: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 158: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 159: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 160: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 161: // "ACTION_TRANSFORMATION_LENGTH"
      case 162: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 163: // "ACTION_TRANSFORMATION_MD5"
      case 164: // "ACTION_TRANSFORMATION_NONE"
      case 165: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 166: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 167: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 168: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 169: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 170: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 172: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 173: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 174: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 175: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 176: // "ACTION_TRANSFORMATION_SHA1"
      case 177: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 178: // "ACTION_TRANSFORMATION_TRIM"
      case 179: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 180: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 181: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 182: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 183: // "Ver"
      case 184: // "xmlns"
      case 185: // "CONFIG_COMPONENT_SIG"
      case 186: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 187: // "CONFIG_SEC_WEB_APP_ID"
      case 188: // "CONFIG_SEC_SERVER_SIG"
      case 189: // "CONFIG_DIR_AUDIT_DIR"
      case 190: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 191: // "CONFIG_DIR_AUDIT_ENG"
      case 192: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 193: // "CONFIG_DIR_AUDIT_LOG"
      case 194: // "CONFIG_DIR_AUDIT_LOG2"
      case 195: // "CONFIG_DIR_AUDIT_LOG_P"
      case 196: // "CONFIG_DIR_AUDIT_STS"
      case 197: // "CONFIG_DIR_AUDIT_TPE"
      case 198: // "CONFIG_DIR_DEBUG_LOG"
      case 199: // "CONFIG_DIR_DEBUG_LVL"
      case 200: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 201: // "CONFIG_DIR_GEO_DB"
      case 202: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 203: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 204: // "CONFIG_DIR_REQ_BODY"
      case 205: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 206: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 207: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 209: // "CONFIG_DIR_RES_BODY"
      case 210: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 211: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 212: // "CONFIG_DIR_RULE_ENG"
      case 213: // "CONFIG_DIR_SEC_ACTION"
      case 214: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 215: // "CONFIG_DIR_SEC_MARKER"
      case 216: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 217: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 218: // "CONFIG_SEC_HTTP_BLKEY"
      case 219: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 220: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 221: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 222: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 223: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 224: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 225: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 226: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 227: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 228: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 229: // "CONFIG_UPLOAD_DIR"
      case 230: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 231: // "CONFIG_UPLOAD_FILE_MODE"
      case 232: // "CONFIG_VALUE_ABORT"
      case 233: // "CONFIG_VALUE_DETC"
      case 234: // "CONFIG_VALUE_HTTPS"
      case 235: // "CONFIG_VALUE_OFF"
      case 236: // "CONFIG_VALUE_ON"
      case 237: // "CONFIG_VALUE_PARALLEL"
      case 238: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 239: // "CONFIG_VALUE_REJECT"
      case 240: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 241: // "CONFIG_VALUE_SERIAL"
      case 242: // "CONFIG_VALUE_WARN"
      case 243: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 244: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 245: // "CONGIG_DIR_SEC_ARG_SEP"
      case 246: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 247: // "CONGIG_DIR_SEC_DATA_DIR"
      case 248: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 249: // "CONGIG_DIR_SEC_TMP_DIR"
      case 250: // "DIRECTIVE"
      case 251: // "DIRECTIVE_SECRULESCRIPT"
      case 252: // "FREE_TEXT"
      case 253: // "OPERATOR"
      case 254: // "OPERATOR_BEGINS_WITH"
      case 255: // "OPERATOR_CONTAINS"
      case 256: // "OPERATOR_CONTAINS_WORD"
      case 257: // "OPERATOR_DETECT_SQLI"
      case 258: // "OPERATOR_DETECT_XSS"
      case 259: // "OPERATOR_ENDS_WITH"
      case 260: // "OPERATOR_EQ"
      case 261: // "OPERATOR_FUZZY_HASH"
      case 262: // "OPERATOR_GE"
      case 263: // "OPERATOR_GEOLOOKUP"
      case 264: // "OPERATOR_GSB_LOOKUP"
      case 265: // "OPERATOR_GT"
      case 266: // "OPERATOR_INSPECT_FILE"
      case 267: // "OPERATOR_IP_MATCH"
      case 268: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 269: // "OPERATOR_LE"
      case 270: // "OPERATOR_LT"
      case 271: // "OPERATOR_PM"
      case 272: // "OPERATOR_PM_FROM_FILE"
      case 273: // "OPERATOR_RBL"
      case 274: // "OPERATOR_RSUB"
      case 275: // "OPERATOR_RX"
      case 276: // "Operator RX (content only)"
      case 277: // "OPERATOR_STR_EQ"
      case 278: // "OPERATOR_STR_MATCH"
      case 279: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 280: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 281: // "OPERATOR_VALIDATE_DTD"
      case 282: // "OPERATOR_VALIDATE_HASH"
      case 283: // "OPERATOR_VALIDATE_SCHEMA"
      case 284: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 285: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 286: // "OPERATOR_VERIFY_CC"
      case 287: // "OPERATOR_VERIFY_CPF"
      case 288: // "OPERATOR_VERIFY_SSN"
      case 289: // "OPERATOR_WITHIN"
      case 290: // "OP_QUOTE"
      case 291: // "QUOTATION_MARK"
      case 292: // "RUN_TIME_VAR_BLD"
      case 293: // "RUN_TIME_VAR_DUR"
      case 294: // "RUN_TIME_VAR_HSV"
      case 295: // "RUN_TIME_VAR_REMOTE_USER"
      case 296: // "RUN_TIME_VAR_TIME"
      case 297: // "RUN_TIME_VAR_TIME_DAY"
      case 298: // "RUN_TIME_VAR_TIME_EPOCH"
      case 299: // "RUN_TIME_VAR_TIME_HOUR"
      case 300: // "RUN_TIME_VAR_TIME_MIN"
      case 301: // "RUN_TIME_VAR_TIME_MON"
      case 302: // "RUN_TIME_VAR_TIME_SEC"
      case 303: // "RUN_TIME_VAR_TIME_WDAY"
      case 304: // "RUN_TIME_VAR_TIME_YEAR"
      case 305: // "SETVAR_VARIABLE_PART"
      case 306: // "SETVAR_CONTENT_PART"
      case 307: // "VARIABLE"
      case 308: // "Dictionary element"
      case 309: // "Dictionary element, selected by regexp"
      case 324: // setvar_variable
      case 325: // setvar_content
        yylhs.value.build< std::string > ();
        break;

      case 316: // op
      case 317: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 321: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 322: // act
      case 323: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 319: // variables
      case 320: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 314: // actions
      case 315: // actions_may_quoted
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
#line 1246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 714 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 720 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 726 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 730 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 734 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 740 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 746 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 763 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 768 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1334 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 774 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1343 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 781 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 785 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 789 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 795 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 799 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 803 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 808 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1401 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 813 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1410 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 818 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1418 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 822 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1426 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 829 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1434 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 833 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1442 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 840 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 846 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1463 "seclang-parser.cc" // lalr1.cc:859
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
#line 1476 "seclang-parser.cc" // lalr1.cc:859
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
#line 1490 "seclang-parser.cc" // lalr1.cc:859
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
#line 1503 "seclang-parser.cc" // lalr1.cc:859
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
#line 1516 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 896 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1524 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 900 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1532 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 904 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 908 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 912 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1556 "seclang-parser.cc" // lalr1.cc:859
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
#line 1569 "seclang-parser.cc" // lalr1.cc:859
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
#line 1582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 934 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 938 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 942 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 947 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 951 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1623 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 955 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 960 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 965 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1650 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1659 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 975 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1667 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 979 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1675 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 983 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1683 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 987 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1691 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 991 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1699 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 995 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 999 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1715 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 1003 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1723 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 1007 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1731 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 1011 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1739 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 1015 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1747 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 1019 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1755 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 1023 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1763 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 1027 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1771 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1031 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1779 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1035 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1787 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1039 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1795 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 69:
#line 1043 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1803 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1047 "seclang-parser.yy" // lalr1.cc:859
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
#line 1818 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1062 "seclang-parser.yy" // lalr1.cc:859
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
#line 1846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1086 "seclang-parser.yy" // lalr1.cc:859
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
#line 1869 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1105 "seclang-parser.yy" // lalr1.cc:859
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
#line 1888 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1120 "seclang-parser.yy" // lalr1.cc:859
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
#line 1916 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1144 "seclang-parser.yy" // lalr1.cc:859
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
#line 1967 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1191 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1975 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1195 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1983 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1199 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1991 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1203 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1999 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1207 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2007 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1211 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2015 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1215 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2023 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1219 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2031 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1223 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 2044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1232 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 2052 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1236 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secWebAppId.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secWebAppId.m_set = true;
      }
#line 2061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1241 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1246 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1251 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2088 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1256 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2097 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1261 "seclang-parser.yy" // lalr1.cc:859
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
#line 2114 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1274 "seclang-parser.yy" // lalr1.cc:859
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
#line 2131 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1287 "seclang-parser.yy" // lalr1.cc:859
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
#line 2148 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1300 "seclang-parser.yy" // lalr1.cc:859
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
#line 2165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1313 "seclang-parser.yy" // lalr1.cc:859
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
#line 2182 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1326 "seclang-parser.yy" // lalr1.cc:859
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
#line 2212 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1352 "seclang-parser.yy" // lalr1.cc:859
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
#line 2241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1378 "seclang-parser.yy" // lalr1.cc:859
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
#line 2257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1390 "seclang-parser.yy" // lalr1.cc:859
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
#line 2280 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1410 "seclang-parser.yy" // lalr1.cc:859
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
#line 2311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1438 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2320 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1443 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2329 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1448 "seclang-parser.yy" // lalr1.cc:859
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1457 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1462 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1466 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 1470 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1474 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1478 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1482 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1488 "seclang-parser.yy" // lalr1.cc:859
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
#line 2415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1500 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2425 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1506 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2433 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1510 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2441 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1523 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1531 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1535 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1542 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2482 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1547 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2492 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1553 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1559 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1565 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1572 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1582 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1586 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1590 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1594 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1598 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1602 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1606 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1610 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1614 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2606 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1618 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2614 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1622 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2622 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1626 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1630 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1634 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2646 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1638 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2654 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1642 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2662 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1646 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1650 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2678 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1654 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2686 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1658 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2694 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1662 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2702 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1666 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2710 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1670 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2718 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1674 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2726 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1678 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2734 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1682 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1686 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2750 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1690 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2758 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1694 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2766 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1698 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2774 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1702 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2782 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1706 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2790 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1710 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2798 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1714 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2806 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1718 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1722 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2822 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1726 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1730 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2838 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1734 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1738 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1742 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1746 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2870 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1750 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1754 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1758 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1762 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1766 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1770 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1774 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1778 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1782 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1786 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1790 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1794 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2966 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1798 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2974 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1802 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2982 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1806 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2990 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1810 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2998 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1814 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1818 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 3014 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1822 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1826 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3030 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1830 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 3038 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1835 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3046 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1839 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1843 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 3062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1848 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1852 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3078 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1856 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 3086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1861 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1865 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1869 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 3110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1874 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1878 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3126 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1882 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1887 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1891 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1895 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3158 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1899 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1903 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3174 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1907 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3182 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1912 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3190 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1916 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3198 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1920 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1925 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3214 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1929 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1933 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1938 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3238 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1943 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1947 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1951 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1955 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1959 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1963 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1967 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1971 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1975 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1979 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1983 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1987 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3334 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1991 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1995 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 1999 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3358 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2003 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3366 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2007 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2011 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3382 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2015 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3390 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2019 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3398 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2023 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3406 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2027 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3414 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2031 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2035 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2039 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2043 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2047 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3454 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2051 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3462 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2055 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2059 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2063 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2067 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2071 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2075 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2079 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2083 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2087 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2091 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2095 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2099 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3558 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2103 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2107 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2111 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2115 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2119 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2123 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3606 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2127 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3614 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2131 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3622 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2135 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2139 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2143 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3646 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2147 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::WebAppId());
      }
#line 3654 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2151 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2159 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3676 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2166 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2173 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2180 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3709 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2187 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2194 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3731 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2201 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2208 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3753 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2215 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2222 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3775 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2229 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3786 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2236 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2246 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3805 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2250 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2254 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2258 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2262 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3837 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2266 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3845 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2270 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3853 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2274 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2279 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3871 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2284 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3880 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2289 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3888 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2293 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3896 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2297 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3904 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2301 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3913 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2306 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3922 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2311 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3930 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2315 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3938 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2319 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3946 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2323 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3954 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2327 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3962 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2331 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3970 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2335 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3978 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2339 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3986 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2343 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3994 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2347 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 4002 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2351 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4011 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2356 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 4019 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2360 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 4028 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2365 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 4036 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2369 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 4044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2373 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 4052 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2377 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 4060 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2381 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 4068 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2385 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 4076 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2389 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4084 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2393 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4092 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2397 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4100 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2401 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4108 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2405 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4116 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2409 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4124 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2413 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4132 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2417 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4140 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2421 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 4148 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2425 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4156 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2429 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4164 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2433 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4172 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2437 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4180 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2441 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4188 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2445 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4196 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2449 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4204 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2453 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(yystack_[0].value.as< std::string > ()));
      }
#line 4212 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2457 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 4220 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2461 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 4228 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2465 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4236 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2469 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4244 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2473 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4252 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2477 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2481 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4268 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2485 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 4276 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2489 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2493 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2497 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2501 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2505 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2509 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2513 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Encode(yystack_[0].value.as< std::string > ()));
      }
#line 4332 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2517 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64Decode(yystack_[0].value.as< std::string > ()));
      }
#line 4340 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2521 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Base64DecodeExt(yystack_[0].value.as< std::string > ()));
      }
#line 4348 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2525 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4356 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2529 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4364 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2533 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4372 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2537 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4380 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2541 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4388 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2545 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4396 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2549 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2553 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4412 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2557 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4420 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 2561 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 2565 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4436 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 2569 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 2573 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 2577 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4460 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 2581 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4468 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 2585 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 2589 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4484 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 2593 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4492 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 373:
#line 2597 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 374:
#line 2601 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4508 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 375:
#line 2605 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4516 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 2609 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4524 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 2613 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4532 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 2617 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 2621 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 2628 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 2632 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4564 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 2636 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4572 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 383:
#line 2640 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4580 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 384:
#line 2644 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4588 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 385:
#line 2651 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4596 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 386:
#line 2656 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4604 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 387:
#line 2663 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4612 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 388:
#line 2668 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4620 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4624 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -281;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     600,  -281,  -213,  -281,   193,  -281,  -281,  -281,  -281,  -281,
    -281,  -219,  -281,  -281,  -281,  -281,  -281,  -228,  -281,  -281,
    -281,  -281,  -281,  -281,    57,  -281,  -281,    56,  -281,    61,
    -281,    60,  -225,   828,   828,  -281,  -281,  -281,  -281,  -227,
    -281,  -281,  -281,    19,    19,    19,   828,    65,    67,  -281,
    -281,  -281,    69,  -281,  -281,  -281,  -281,  -281,  -281,    19,
     828,   667,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,   266,    43,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -216,  -281,  -281,  -281,    72,
     127,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,   922,  -281,     4,  -281,  -281,  -281,  -281,
     480,   480,    58,    62,    64,    73,    75,    77,    79,    81,
      83,    99,   101,   103,   105,   107,   109,   111,  -281,   113,
     115,   117,   119,  -281,  -281,   121,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,   160,  -281,  -281,  -281,  -281,  -281,
     162,  -281,  -281,  -281,   164,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,   166,   264,   267,   269,
     271,   273,   102,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,     8,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,   178,  -281,
    -281,  -281,  -280,  -280,  -281,   188,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,    -2,   922,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
      -4,   185,    71,  -145,   -62,    25,  -281,  -281,    31,    34,
      36,    54,  -281,   112,   122,   124,   128,   141,   179,   224,
     322,   331,   332,   333,   334,  -281,   335,   336,  -281,   337,
     338,   339,   340,  -281,  -281,   341,   342,   343,   344,   828,
    -281,  -281,  -281,     3,     3,     3,  -281,  -281,  -281,   480,
     480,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,     3,  -281,  -281,  -281,  -281,  -281,
    -281
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
     316,   318,   317,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   341,   342,   343,   344,   345,   352,
     353,   354,   355,   365,   371,   358,   359,   369,   370,   375,
     360,   357,   364,   374,   373,   350,   349,   348,   378,   377,
     368,   366,   379,   367,   356,   351,   372,   361,   363,   362,
     376,   346,   347,     0,    74,    29,    31,    76,   110,   111,
       0,     0,   136,   139,   142,   145,   148,   151,   154,   157,
     160,   163,   166,   169,   172,   175,   178,   181,   225,   214,
     184,   211,   217,   226,   227,   193,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   248,   247,   251,   250,   249,
     252,   254,   253,   255,   220,   256,   257,   258,   260,   259,
     196,   261,   262,   221,   224,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   273,   272,   199,   202,   205,   208,
     187,   190,     0,   275,   274,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,    95,   126,   131,    96,
      97,    98,    22,    21,    27,    26,   116,   117,     0,    75,
       1,     3,     0,   385,   340,   381,   306,   305,   304,   295,
     294,   296,   301,   300,   303,   302,     0,     0,   133,   132,
     134,   135,   137,   138,   140,   141,   143,   144,   146,   147,
     149,   150,   152,   153,   155,   156,   158,   159,   161,   162,
     164,   165,   167,   168,   170,   171,   173,   174,   176,   177,
     179,   180,   212,   213,   182,   183,   209,   210,   215,   216,
     191,   192,   218,   219,   194,   195,   222,   223,   197,   198,
     200,   201,   203,   204,   206,   207,   185,   186,   188,   189,
       0,     0,     0,     0,     0,     0,    37,    38,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,     0,    36,     0,
       0,     0,     0,    39,    40,     0,     0,     0,     0,    73,
      32,   380,   386,     0,     0,     0,    28,    30,   127,     0,
       0,   128,    35,    33,    69,    54,    53,    55,    56,    42,
      57,    50,    58,    41,    60,    59,    61,    62,    64,    63,
      65,    51,    66,    67,    68,    43,    44,    45,    46,    47,
      48,    49,    52,    72,   387,   382,   383,   384,   130,   129,
     388
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -281,  -281,   304,  -281,   -34,   206,  -281,   225,  -281,   -41,
     345,   197,   291,  -281,    76,  -130
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    61,    62,    63,   174,   175,   409,   410,    64,   276,
     277,   278,   176,   294,   295,   455
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     177,   307,   371,   279,   280,   178,    72,   307,    83,    73,
      84,    85,   281,    74,   371,   179,    69,    70,   288,   299,
     300,    71,    65,    66,   301,   293,   289,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   424,   260,   261,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     425,   260,   261,   419,   420,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   372,   260,   261,   296,   426,   297,   298,
     413,   414,   415,   427,   456,   457,   428,   418,   429,   416,
      67,    68,    75,    76,    77,    78,    79,    80,    81,    82,
     282,   283,   284,   285,   286,   287,   430,   302,   303,   454,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   460,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   422,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   292,   304,   305,   431,   291,   310,   311,   411,   412,
     312,   313,   314,   315,   432,   453,   433,   308,   309,   306,
     434,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   435,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   436,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   352,   353,
     354,   355,   356,   357,   358,   359,   437,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   421,   260,
     261,   293,   360,   361,   438,   362,   363,   364,   365,   366,
     367,   368,   369,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   423,   417,     0,
       1,     0,     0,     0,     2,     3,     0,   370,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,     2,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     0,     0,     0,     0,     0,
       0,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    86,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    86,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      34,     3,     6,    44,    45,   232,   234,     3,   233,   237,
     235,   236,    46,   241,     6,   242,   235,   236,    59,   235,
     236,   240,   235,   236,   240,   305,    60,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,   252,    89,    90,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
     252,    89,    90,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    95,    89,    90,   233,   252,   235,   236,
      92,    93,    94,   252,   414,   415,   252,   291,   252,   291,
      97,    98,   235,   236,   238,   239,   235,   236,   238,   239,
     235,   236,   235,   236,   235,   236,   252,   235,   236,   306,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   454,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,    95,   235,   236,   252,    61,   308,   309,   292,   293,
     308,   309,   308,   309,   252,   409,   252,   180,   181,   173,
     252,   308,   309,   308,   309,   308,   309,   308,   309,   308,
     309,   308,   309,   252,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   308,   309,   308,
     309,   308,   309,   308,   309,   308,   309,   308,   309,   308,
     309,   308,   309,   308,   309,   308,   309,   308,   309,   308,
     309,   252,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   308,   309,
     308,   309,   308,   309,   308,   309,   252,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,   371,    89,
      90,   305,   308,   309,   252,   308,   309,   308,   309,   308,
     309,   308,   309,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   372,   307,    -1,
       0,    -1,    -1,    -1,     4,     5,    -1,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   419,   420,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,
      -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     243,   244,   245,   246,   247,   248,   249,   250,   251,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,    96,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   311,   312,   313,   318,   235,   236,    97,    98,   235,
     236,   240,   234,   237,   241,   235,   236,   238,   239,   235,
     236,   238,   239,   233,   235,   236,    91,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   291,   314,   315,   322,   314,   232,   242,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      89,    90,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   319,   320,   321,   319,
     319,   314,   235,   236,   235,   236,   235,   236,   319,   314,
       0,   312,    95,   305,   323,   324,   233,   235,   236,   235,
     236,   240,   235,   236,   235,   236,   315,     3,   321,   321,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     308,   309,   308,   309,   308,   309,   308,   309,   308,   309,
     320,     6,    95,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   316,
     317,   324,   324,    92,    93,    94,   291,   322,   291,     8,
       9,   321,   276,   317,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   314,   306,   325,   325,   325,   321,   321,
     325
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   310,   311,   311,   311,   312,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   314,   314,
     315,   315,   316,   316,   316,   316,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   319,   319,   320,   320,
     320,   320,   320,   320,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     323,   323,   323,   323,   323,   324,   324,   325,   325
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     3,     3,     1,     2,     1,     2
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
  "\"DIRECTIVE_SECRULESCRIPT\"", "\"FREE_TEXT\"", "\"OPERATOR\"",
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
  "setvar_variable", "setvar_content", YY_NULLPTR
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
    1006,  1010,  1014,  1018,  1022,  1026,  1030,  1034,  1038,  1042,
    1046,  1060,  1061,  1085,  1104,  1119,  1143,  1190,  1194,  1198,
    1202,  1206,  1210,  1214,  1218,  1222,  1231,  1235,  1240,  1245,
    1250,  1255,  1260,  1273,  1286,  1299,  1312,  1325,  1351,  1377,
    1389,  1409,  1437,  1442,  1447,  1456,  1461,  1465,  1469,  1473,
    1477,  1481,  1485,  1486,  1487,  1499,  1505,  1509,  1513,  1514,
    1515,  1516,  1517,  1518,  1519,  1522,  1530,  1534,  1541,  1546,
    1552,  1558,  1564,  1571,  1581,  1585,  1589,  1593,  1597,  1601,
    1605,  1609,  1613,  1617,  1621,  1625,  1629,  1633,  1637,  1641,
    1645,  1649,  1653,  1657,  1661,  1665,  1669,  1673,  1677,  1681,
    1685,  1689,  1693,  1697,  1701,  1705,  1709,  1713,  1717,  1721,
    1725,  1729,  1733,  1737,  1741,  1745,  1749,  1753,  1757,  1761,
    1765,  1769,  1773,  1777,  1781,  1785,  1789,  1793,  1797,  1801,
    1805,  1809,  1813,  1817,  1821,  1825,  1829,  1834,  1838,  1842,
    1847,  1851,  1855,  1860,  1864,  1868,  1873,  1877,  1881,  1886,
    1890,  1894,  1898,  1902,  1906,  1911,  1915,  1919,  1924,  1928,
    1932,  1937,  1942,  1946,  1950,  1954,  1958,  1962,  1966,  1970,
    1974,  1978,  1982,  1986,  1990,  1994,  1998,  2002,  2006,  2010,
    2014,  2018,  2022,  2026,  2030,  2034,  2038,  2042,  2046,  2050,
    2054,  2058,  2062,  2066,  2070,  2074,  2078,  2082,  2086,  2090,
    2094,  2098,  2102,  2106,  2110,  2114,  2118,  2122,  2126,  2130,
    2134,  2138,  2142,  2146,  2150,  2158,  2165,  2172,  2179,  2186,
    2193,  2200,  2207,  2214,  2221,  2228,  2235,  2245,  2249,  2253,
    2257,  2261,  2265,  2269,  2273,  2278,  2283,  2288,  2292,  2296,
    2300,  2305,  2310,  2314,  2318,  2322,  2326,  2330,  2334,  2338,
    2342,  2346,  2350,  2355,  2359,  2364,  2368,  2372,  2376,  2380,
    2384,  2388,  2392,  2396,  2400,  2404,  2408,  2412,  2416,  2420,
    2424,  2428,  2432,  2436,  2440,  2444,  2448,  2452,  2456,  2460,
    2464,  2468,  2472,  2476,  2480,  2484,  2488,  2492,  2496,  2500,
    2504,  2508,  2512,  2516,  2520,  2524,  2528,  2532,  2536,  2540,
    2544,  2548,  2552,  2556,  2560,  2564,  2568,  2572,  2576,  2580,
    2584,  2588,  2592,  2596,  2600,  2604,  2608,  2612,  2616,  2620,
    2627,  2631,  2635,  2639,  2643,  2650,  2655,  2662,  2667
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
#line 5609 "seclang-parser.cc" // lalr1.cc:1167
#line 2673 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
