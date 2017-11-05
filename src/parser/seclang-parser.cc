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
#line 350 "seclang-parser.yy" // lalr1.cc:413

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
      case 99: // "Accuracy"
      case 100: // "Allow"
      case 101: // "Append"
      case 102: // "AuditLog"
      case 103: // "Block"
      case 104: // "Capture"
      case 105: // "Chain"
      case 106: // "ACTION_CTL_AUDIT_ENGINE"
      case 107: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 108: // "ACTION_CTL_BDY_JSON"
      case 109: // "ACTION_CTL_BDY_XML"
      case 110: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 111: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 112: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 113: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 115: // "Deny"
      case 116: // "DeprecateVar"
      case 117: // "Drop"
      case 118: // "Exec"
      case 119: // "ExpireVar"
      case 120: // "Id"
      case 121: // "InitCol"
      case 122: // "Log"
      case 123: // "LogData"
      case 124: // "Maturity"
      case 125: // "Msg"
      case 126: // "MultiMatch"
      case 127: // "NoAuditLog"
      case 128: // "NoLog"
      case 129: // "Pass"
      case 130: // "Pause"
      case 131: // "Phase"
      case 132: // "Prepend"
      case 133: // "Proxy"
      case 134: // "Redirect"
      case 135: // "Rev"
      case 136: // "SanatiseArg"
      case 137: // "SanatiseMatched"
      case 138: // "SanatiseMatchedBytes"
      case 139: // "SanatiseRequestHeader"
      case 140: // "SanatiseResponseHeader"
      case 141: // "SetEnv"
      case 142: // "SetRsc"
      case 143: // "SetSid"
      case 144: // "SetUID"
      case 145: // "Severity"
      case 146: // "Skip"
      case 147: // "SkipAfter"
      case 148: // "Status"
      case 149: // "Tag"
      case 150: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 151: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 152: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 155: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 156: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_LENGTH"
      case 158: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 159: // "ACTION_TRANSFORMATION_MD5"
      case 160: // "ACTION_TRANSFORMATION_NONE"
      case 161: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 163: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 164: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 170: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 172: // "ACTION_TRANSFORMATION_SHA1"
      case 173: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 174: // "ACTION_TRANSFORMATION_TRIM"
      case 175: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 176: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 178: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 179: // "Ver"
      case 180: // "xmlns"
      case 181: // "CONFIG_COMPONENT_SIG"
      case 182: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 183: // "CONFIG_SEC_WEB_APP_ID"
      case 184: // "CONFIG_SEC_SERVER_SIG"
      case 185: // "CONFIG_DIR_AUDIT_DIR"
      case 186: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 187: // "CONFIG_DIR_AUDIT_ENG"
      case 188: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 189: // "CONFIG_DIR_AUDIT_LOG"
      case 190: // "CONFIG_DIR_AUDIT_LOG2"
      case 191: // "CONFIG_DIR_AUDIT_LOG_P"
      case 192: // "CONFIG_DIR_AUDIT_STS"
      case 193: // "CONFIG_DIR_AUDIT_TPE"
      case 194: // "CONFIG_DIR_DEBUG_LOG"
      case 195: // "CONFIG_DIR_DEBUG_LVL"
      case 196: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 197: // "CONFIG_DIR_GEO_DB"
      case 198: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 200: // "CONFIG_DIR_REQ_BODY"
      case 201: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 202: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 204: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 205: // "CONFIG_DIR_RES_BODY"
      case 206: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_RULE_ENG"
      case 209: // "CONFIG_DIR_SEC_ACTION"
      case 210: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 211: // "CONFIG_DIR_SEC_MARKER"
      case 212: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 213: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 214: // "CONFIG_SEC_HTTP_BLKEY"
      case 215: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 216: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 218: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 219: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 220: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 221: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 222: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 223: // "CONFIG_UPLOAD_DIR"
      case 224: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 225: // "CONFIG_UPLOAD_FILE_MODE"
      case 226: // "CONFIG_VALUE_ABORT"
      case 227: // "CONFIG_VALUE_DETC"
      case 228: // "CONFIG_VALUE_HTTPS"
      case 229: // "CONFIG_VALUE_OFF"
      case 230: // "CONFIG_VALUE_ON"
      case 231: // "CONFIG_VALUE_PARALLEL"
      case 232: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 233: // "CONFIG_VALUE_REJECT"
      case 234: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 235: // "CONFIG_VALUE_SERIAL"
      case 236: // "CONFIG_VALUE_WARN"
      case 237: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 238: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 239: // "CONGIG_DIR_SEC_ARG_SEP"
      case 240: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 241: // "CONGIG_DIR_SEC_DATA_DIR"
      case 242: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 243: // "CONGIG_DIR_SEC_TMP_DIR"
      case 244: // "DIRECTIVE"
      case 245: // "DIRECTIVE_SECRULESCRIPT"
      case 246: // "FREE_TEXT"
      case 247: // "OPERATOR"
      case 248: // "OPERATOR_BEGINS_WITH"
      case 249: // "OPERATOR_CONTAINS"
      case 250: // "OPERATOR_CONTAINS_WORD"
      case 251: // "OPERATOR_DETECT_SQLI"
      case 252: // "OPERATOR_DETECT_XSS"
      case 253: // "OPERATOR_ENDS_WITH"
      case 254: // "OPERATOR_EQ"
      case 255: // "OPERATOR_FUZZY_HASH"
      case 256: // "OPERATOR_GE"
      case 257: // "OPERATOR_GEOLOOKUP"
      case 258: // "OPERATOR_GSB_LOOKUP"
      case 259: // "OPERATOR_GT"
      case 260: // "OPERATOR_INSPECT_FILE"
      case 261: // "OPERATOR_IP_MATCH"
      case 262: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 263: // "OPERATOR_LE"
      case 264: // "OPERATOR_LT"
      case 265: // "OPERATOR_PM"
      case 266: // "OPERATOR_PM_FROM_FILE"
      case 267: // "OPERATOR_RBL"
      case 268: // "OPERATOR_RSUB"
      case 269: // "OPERATOR_RX"
      case 270: // "Operator RX (content only)"
      case 271: // "OPERATOR_STR_EQ"
      case 272: // "OPERATOR_STR_MATCH"
      case 273: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 274: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 275: // "OPERATOR_VALIDATE_DTD"
      case 276: // "OPERATOR_VALIDATE_HASH"
      case 277: // "OPERATOR_VALIDATE_SCHEMA"
      case 278: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 279: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 280: // "OPERATOR_VERIFY_CC"
      case 281: // "OPERATOR_VERIFY_CPF"
      case 282: // "OPERATOR_VERIFY_SSN"
      case 283: // "OPERATOR_WITHIN"
      case 284: // "OP_QUOTE"
      case 285: // "QUOTATION_MARK"
      case 286: // "RUN_TIME_VAR_BLD"
      case 287: // "RUN_TIME_VAR_DUR"
      case 288: // "RUN_TIME_VAR_HSV"
      case 289: // "RUN_TIME_VAR_REMOTE_USER"
      case 290: // "RUN_TIME_VAR_TIME"
      case 291: // "RUN_TIME_VAR_TIME_DAY"
      case 292: // "RUN_TIME_VAR_TIME_EPOCH"
      case 293: // "RUN_TIME_VAR_TIME_HOUR"
      case 294: // "RUN_TIME_VAR_TIME_MIN"
      case 295: // "RUN_TIME_VAR_TIME_MON"
      case 296: // "RUN_TIME_VAR_TIME_SEC"
      case 297: // "RUN_TIME_VAR_TIME_WDAY"
      case 298: // "RUN_TIME_VAR_TIME_YEAR"
      case 299: // "SETVAR_VARIABLE_PART"
      case 300: // "SETVAR_CONTENT_PART"
      case 301: // "VARIABLE"
      case 302: // "Dictionary element"
      case 303: // "Dictionary element, selected by regexp"
      case 318: // setvar_variable
      case 319: // setvar_content
        value.move< std::string > (that.value);
        break;

      case 310: // op
      case 311: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 315: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 316: // act
      case 317: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 313: // variables
      case 314: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 308: // actions
      case 309: // actions_may_quoted
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
      case 99: // "Accuracy"
      case 100: // "Allow"
      case 101: // "Append"
      case 102: // "AuditLog"
      case 103: // "Block"
      case 104: // "Capture"
      case 105: // "Chain"
      case 106: // "ACTION_CTL_AUDIT_ENGINE"
      case 107: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 108: // "ACTION_CTL_BDY_JSON"
      case 109: // "ACTION_CTL_BDY_XML"
      case 110: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 111: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 112: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 113: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 115: // "Deny"
      case 116: // "DeprecateVar"
      case 117: // "Drop"
      case 118: // "Exec"
      case 119: // "ExpireVar"
      case 120: // "Id"
      case 121: // "InitCol"
      case 122: // "Log"
      case 123: // "LogData"
      case 124: // "Maturity"
      case 125: // "Msg"
      case 126: // "MultiMatch"
      case 127: // "NoAuditLog"
      case 128: // "NoLog"
      case 129: // "Pass"
      case 130: // "Pause"
      case 131: // "Phase"
      case 132: // "Prepend"
      case 133: // "Proxy"
      case 134: // "Redirect"
      case 135: // "Rev"
      case 136: // "SanatiseArg"
      case 137: // "SanatiseMatched"
      case 138: // "SanatiseMatchedBytes"
      case 139: // "SanatiseRequestHeader"
      case 140: // "SanatiseResponseHeader"
      case 141: // "SetEnv"
      case 142: // "SetRsc"
      case 143: // "SetSid"
      case 144: // "SetUID"
      case 145: // "Severity"
      case 146: // "Skip"
      case 147: // "SkipAfter"
      case 148: // "Status"
      case 149: // "Tag"
      case 150: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 151: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 152: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 155: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 156: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_LENGTH"
      case 158: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 159: // "ACTION_TRANSFORMATION_MD5"
      case 160: // "ACTION_TRANSFORMATION_NONE"
      case 161: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 163: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 164: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 170: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 172: // "ACTION_TRANSFORMATION_SHA1"
      case 173: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 174: // "ACTION_TRANSFORMATION_TRIM"
      case 175: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 176: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 178: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 179: // "Ver"
      case 180: // "xmlns"
      case 181: // "CONFIG_COMPONENT_SIG"
      case 182: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 183: // "CONFIG_SEC_WEB_APP_ID"
      case 184: // "CONFIG_SEC_SERVER_SIG"
      case 185: // "CONFIG_DIR_AUDIT_DIR"
      case 186: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 187: // "CONFIG_DIR_AUDIT_ENG"
      case 188: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 189: // "CONFIG_DIR_AUDIT_LOG"
      case 190: // "CONFIG_DIR_AUDIT_LOG2"
      case 191: // "CONFIG_DIR_AUDIT_LOG_P"
      case 192: // "CONFIG_DIR_AUDIT_STS"
      case 193: // "CONFIG_DIR_AUDIT_TPE"
      case 194: // "CONFIG_DIR_DEBUG_LOG"
      case 195: // "CONFIG_DIR_DEBUG_LVL"
      case 196: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 197: // "CONFIG_DIR_GEO_DB"
      case 198: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 200: // "CONFIG_DIR_REQ_BODY"
      case 201: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 202: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 204: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 205: // "CONFIG_DIR_RES_BODY"
      case 206: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_RULE_ENG"
      case 209: // "CONFIG_DIR_SEC_ACTION"
      case 210: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 211: // "CONFIG_DIR_SEC_MARKER"
      case 212: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 213: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 214: // "CONFIG_SEC_HTTP_BLKEY"
      case 215: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 216: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 218: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 219: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 220: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 221: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 222: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 223: // "CONFIG_UPLOAD_DIR"
      case 224: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 225: // "CONFIG_UPLOAD_FILE_MODE"
      case 226: // "CONFIG_VALUE_ABORT"
      case 227: // "CONFIG_VALUE_DETC"
      case 228: // "CONFIG_VALUE_HTTPS"
      case 229: // "CONFIG_VALUE_OFF"
      case 230: // "CONFIG_VALUE_ON"
      case 231: // "CONFIG_VALUE_PARALLEL"
      case 232: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 233: // "CONFIG_VALUE_REJECT"
      case 234: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 235: // "CONFIG_VALUE_SERIAL"
      case 236: // "CONFIG_VALUE_WARN"
      case 237: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 238: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 239: // "CONGIG_DIR_SEC_ARG_SEP"
      case 240: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 241: // "CONGIG_DIR_SEC_DATA_DIR"
      case 242: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 243: // "CONGIG_DIR_SEC_TMP_DIR"
      case 244: // "DIRECTIVE"
      case 245: // "DIRECTIVE_SECRULESCRIPT"
      case 246: // "FREE_TEXT"
      case 247: // "OPERATOR"
      case 248: // "OPERATOR_BEGINS_WITH"
      case 249: // "OPERATOR_CONTAINS"
      case 250: // "OPERATOR_CONTAINS_WORD"
      case 251: // "OPERATOR_DETECT_SQLI"
      case 252: // "OPERATOR_DETECT_XSS"
      case 253: // "OPERATOR_ENDS_WITH"
      case 254: // "OPERATOR_EQ"
      case 255: // "OPERATOR_FUZZY_HASH"
      case 256: // "OPERATOR_GE"
      case 257: // "OPERATOR_GEOLOOKUP"
      case 258: // "OPERATOR_GSB_LOOKUP"
      case 259: // "OPERATOR_GT"
      case 260: // "OPERATOR_INSPECT_FILE"
      case 261: // "OPERATOR_IP_MATCH"
      case 262: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 263: // "OPERATOR_LE"
      case 264: // "OPERATOR_LT"
      case 265: // "OPERATOR_PM"
      case 266: // "OPERATOR_PM_FROM_FILE"
      case 267: // "OPERATOR_RBL"
      case 268: // "OPERATOR_RSUB"
      case 269: // "OPERATOR_RX"
      case 270: // "Operator RX (content only)"
      case 271: // "OPERATOR_STR_EQ"
      case 272: // "OPERATOR_STR_MATCH"
      case 273: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 274: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 275: // "OPERATOR_VALIDATE_DTD"
      case 276: // "OPERATOR_VALIDATE_HASH"
      case 277: // "OPERATOR_VALIDATE_SCHEMA"
      case 278: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 279: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 280: // "OPERATOR_VERIFY_CC"
      case 281: // "OPERATOR_VERIFY_CPF"
      case 282: // "OPERATOR_VERIFY_SSN"
      case 283: // "OPERATOR_WITHIN"
      case 284: // "OP_QUOTE"
      case 285: // "QUOTATION_MARK"
      case 286: // "RUN_TIME_VAR_BLD"
      case 287: // "RUN_TIME_VAR_DUR"
      case 288: // "RUN_TIME_VAR_HSV"
      case 289: // "RUN_TIME_VAR_REMOTE_USER"
      case 290: // "RUN_TIME_VAR_TIME"
      case 291: // "RUN_TIME_VAR_TIME_DAY"
      case 292: // "RUN_TIME_VAR_TIME_EPOCH"
      case 293: // "RUN_TIME_VAR_TIME_HOUR"
      case 294: // "RUN_TIME_VAR_TIME_MIN"
      case 295: // "RUN_TIME_VAR_TIME_MON"
      case 296: // "RUN_TIME_VAR_TIME_SEC"
      case 297: // "RUN_TIME_VAR_TIME_WDAY"
      case 298: // "RUN_TIME_VAR_TIME_YEAR"
      case 299: // "SETVAR_VARIABLE_PART"
      case 300: // "SETVAR_CONTENT_PART"
      case 301: // "VARIABLE"
      case 302: // "Dictionary element"
      case 303: // "Dictionary element, selected by regexp"
      case 318: // setvar_variable
      case 319: // setvar_content
        value.copy< std::string > (that.value);
        break;

      case 310: // op
      case 311: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 315: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 316: // act
      case 317: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 313: // variables
      case 314: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 308: // actions
      case 309: // actions_may_quoted
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
    #line 343 "/home/zimmerle/core-trustwave/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 885 "seclang-parser.cc" // lalr1.cc:741

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
      case 99: // "Accuracy"
      case 100: // "Allow"
      case 101: // "Append"
      case 102: // "AuditLog"
      case 103: // "Block"
      case 104: // "Capture"
      case 105: // "Chain"
      case 106: // "ACTION_CTL_AUDIT_ENGINE"
      case 107: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 108: // "ACTION_CTL_BDY_JSON"
      case 109: // "ACTION_CTL_BDY_XML"
      case 110: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 111: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 112: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 113: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 114: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 115: // "Deny"
      case 116: // "DeprecateVar"
      case 117: // "Drop"
      case 118: // "Exec"
      case 119: // "ExpireVar"
      case 120: // "Id"
      case 121: // "InitCol"
      case 122: // "Log"
      case 123: // "LogData"
      case 124: // "Maturity"
      case 125: // "Msg"
      case 126: // "MultiMatch"
      case 127: // "NoAuditLog"
      case 128: // "NoLog"
      case 129: // "Pass"
      case 130: // "Pause"
      case 131: // "Phase"
      case 132: // "Prepend"
      case 133: // "Proxy"
      case 134: // "Redirect"
      case 135: // "Rev"
      case 136: // "SanatiseArg"
      case 137: // "SanatiseMatched"
      case 138: // "SanatiseMatchedBytes"
      case 139: // "SanatiseRequestHeader"
      case 140: // "SanatiseResponseHeader"
      case 141: // "SetEnv"
      case 142: // "SetRsc"
      case 143: // "SetSid"
      case 144: // "SetUID"
      case 145: // "Severity"
      case 146: // "Skip"
      case 147: // "SkipAfter"
      case 148: // "Status"
      case 149: // "Tag"
      case 150: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 151: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 152: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 154: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 155: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 156: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 157: // "ACTION_TRANSFORMATION_LENGTH"
      case 158: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 159: // "ACTION_TRANSFORMATION_MD5"
      case 160: // "ACTION_TRANSFORMATION_NONE"
      case 161: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 162: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 163: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 164: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 165: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 168: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 170: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 171: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 172: // "ACTION_TRANSFORMATION_SHA1"
      case 173: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 174: // "ACTION_TRANSFORMATION_TRIM"
      case 175: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 176: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 177: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 178: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 179: // "Ver"
      case 180: // "xmlns"
      case 181: // "CONFIG_COMPONENT_SIG"
      case 182: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 183: // "CONFIG_SEC_WEB_APP_ID"
      case 184: // "CONFIG_SEC_SERVER_SIG"
      case 185: // "CONFIG_DIR_AUDIT_DIR"
      case 186: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 187: // "CONFIG_DIR_AUDIT_ENG"
      case 188: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 189: // "CONFIG_DIR_AUDIT_LOG"
      case 190: // "CONFIG_DIR_AUDIT_LOG2"
      case 191: // "CONFIG_DIR_AUDIT_LOG_P"
      case 192: // "CONFIG_DIR_AUDIT_STS"
      case 193: // "CONFIG_DIR_AUDIT_TPE"
      case 194: // "CONFIG_DIR_DEBUG_LOG"
      case 195: // "CONFIG_DIR_DEBUG_LVL"
      case 196: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 197: // "CONFIG_DIR_GEO_DB"
      case 198: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 199: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 200: // "CONFIG_DIR_REQ_BODY"
      case 201: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 202: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 203: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 204: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 205: // "CONFIG_DIR_RES_BODY"
      case 206: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 207: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 208: // "CONFIG_DIR_RULE_ENG"
      case 209: // "CONFIG_DIR_SEC_ACTION"
      case 210: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 211: // "CONFIG_DIR_SEC_MARKER"
      case 212: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 213: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 214: // "CONFIG_SEC_HTTP_BLKEY"
      case 215: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 216: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 217: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 218: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 219: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 220: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 221: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 222: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 223: // "CONFIG_UPLOAD_DIR"
      case 224: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 225: // "CONFIG_UPLOAD_FILE_MODE"
      case 226: // "CONFIG_VALUE_ABORT"
      case 227: // "CONFIG_VALUE_DETC"
      case 228: // "CONFIG_VALUE_HTTPS"
      case 229: // "CONFIG_VALUE_OFF"
      case 230: // "CONFIG_VALUE_ON"
      case 231: // "CONFIG_VALUE_PARALLEL"
      case 232: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 233: // "CONFIG_VALUE_REJECT"
      case 234: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 235: // "CONFIG_VALUE_SERIAL"
      case 236: // "CONFIG_VALUE_WARN"
      case 237: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 238: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 239: // "CONGIG_DIR_SEC_ARG_SEP"
      case 240: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 241: // "CONGIG_DIR_SEC_DATA_DIR"
      case 242: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 243: // "CONGIG_DIR_SEC_TMP_DIR"
      case 244: // "DIRECTIVE"
      case 245: // "DIRECTIVE_SECRULESCRIPT"
      case 246: // "FREE_TEXT"
      case 247: // "OPERATOR"
      case 248: // "OPERATOR_BEGINS_WITH"
      case 249: // "OPERATOR_CONTAINS"
      case 250: // "OPERATOR_CONTAINS_WORD"
      case 251: // "OPERATOR_DETECT_SQLI"
      case 252: // "OPERATOR_DETECT_XSS"
      case 253: // "OPERATOR_ENDS_WITH"
      case 254: // "OPERATOR_EQ"
      case 255: // "OPERATOR_FUZZY_HASH"
      case 256: // "OPERATOR_GE"
      case 257: // "OPERATOR_GEOLOOKUP"
      case 258: // "OPERATOR_GSB_LOOKUP"
      case 259: // "OPERATOR_GT"
      case 260: // "OPERATOR_INSPECT_FILE"
      case 261: // "OPERATOR_IP_MATCH"
      case 262: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 263: // "OPERATOR_LE"
      case 264: // "OPERATOR_LT"
      case 265: // "OPERATOR_PM"
      case 266: // "OPERATOR_PM_FROM_FILE"
      case 267: // "OPERATOR_RBL"
      case 268: // "OPERATOR_RSUB"
      case 269: // "OPERATOR_RX"
      case 270: // "Operator RX (content only)"
      case 271: // "OPERATOR_STR_EQ"
      case 272: // "OPERATOR_STR_MATCH"
      case 273: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 274: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 275: // "OPERATOR_VALIDATE_DTD"
      case 276: // "OPERATOR_VALIDATE_HASH"
      case 277: // "OPERATOR_VALIDATE_SCHEMA"
      case 278: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 279: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 280: // "OPERATOR_VERIFY_CC"
      case 281: // "OPERATOR_VERIFY_CPF"
      case 282: // "OPERATOR_VERIFY_SSN"
      case 283: // "OPERATOR_WITHIN"
      case 284: // "OP_QUOTE"
      case 285: // "QUOTATION_MARK"
      case 286: // "RUN_TIME_VAR_BLD"
      case 287: // "RUN_TIME_VAR_DUR"
      case 288: // "RUN_TIME_VAR_HSV"
      case 289: // "RUN_TIME_VAR_REMOTE_USER"
      case 290: // "RUN_TIME_VAR_TIME"
      case 291: // "RUN_TIME_VAR_TIME_DAY"
      case 292: // "RUN_TIME_VAR_TIME_EPOCH"
      case 293: // "RUN_TIME_VAR_TIME_HOUR"
      case 294: // "RUN_TIME_VAR_TIME_MIN"
      case 295: // "RUN_TIME_VAR_TIME_MON"
      case 296: // "RUN_TIME_VAR_TIME_SEC"
      case 297: // "RUN_TIME_VAR_TIME_WDAY"
      case 298: // "RUN_TIME_VAR_TIME_YEAR"
      case 299: // "SETVAR_VARIABLE_PART"
      case 300: // "SETVAR_CONTENT_PART"
      case 301: // "VARIABLE"
      case 302: // "Dictionary element"
      case 303: // "Dictionary element, selected by regexp"
      case 318: // setvar_variable
      case 319: // setvar_content
        yylhs.value.build< std::string > ();
        break;

      case 310: // op
      case 311: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 315: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 316: // act
      case 317: // setvar_action
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 313: // variables
      case 314: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 308: // actions
      case 309: // actions_may_quoted
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
#line 694 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 707 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 713 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 719 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 723 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 727 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 733 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 739 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 745 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 751 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 756 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::JSONAuditLogFormat);
      }
#line 1311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 761 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFormat(modsecurity::audit_log::AuditLog::NativeAuditLogFormat);
      }
#line 1319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 767 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1328 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 774 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1336 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 778 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1344 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 782 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1352 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 788 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1360 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 792 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1368 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 796 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1377 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 801 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 806 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1395 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 811 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1403 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 815 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1411 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 822 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1419 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 826 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1427 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 833 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1437 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 839 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 849 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1461 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 858 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1475 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 868 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1488 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 877 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("Rx", utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()), true));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1501 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 889 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 893 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 897 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 901 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 905 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1541 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 909 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::InspectFile(yystack_[0].value.as< std::string > ()));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1554 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 918 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::FuzzyHash(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1567 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 927 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 931 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 935 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 940 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 944 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 948 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 953 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1626 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 963 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1644 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 968 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1652 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 972 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1660 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 976 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1668 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 980 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1676 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 984 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1684 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 988 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1692 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 992 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1700 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 996 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1708 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 1000 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1716 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 1004 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1724 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 1008 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1732 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 1012 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1740 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 1016 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1748 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 1020 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1756 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1024 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1028 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1772 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1032 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1780 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 69:
#line 1036 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1788 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1040 "seclang-parser.yy" // lalr1.cc:859
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
#line 1803 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1055 "seclang-parser.yy" // lalr1.cc:859
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
#line 1831 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1079 "seclang-parser.yy" // lalr1.cc:859
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
#line 1854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1098 "seclang-parser.yy" // lalr1.cc:859
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
#line 1873 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1113 "seclang-parser.yy" // lalr1.cc:859
    {
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
        std::string err;
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
#line 1901 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1137 "seclang-parser.yy" // lalr1.cc:859
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
#line 1952 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1184 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1960 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1188 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1968 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1192 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1976 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1196 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1984 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1200 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1992 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1204 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2000 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1208 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2008 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1212 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2016 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1216 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 2029 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1225 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 2037 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1229 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecWebAppId is not supported.");
        YYERROR;
      }
#line 2046 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1234 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 2055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1239 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2064 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1244 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2073 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1249 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2082 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1254 "seclang-parser.yy" // lalr1.cc:859
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
#line 2099 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1267 "seclang-parser.yy" // lalr1.cc:859
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
#line 2116 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1280 "seclang-parser.yy" // lalr1.cc:859
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
#line 2133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1293 "seclang-parser.yy" // lalr1.cc:859
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
#line 2163 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1319 "seclang-parser.yy" // lalr1.cc:859
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
#line 2192 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1345 "seclang-parser.yy" // lalr1.cc:859
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
#line 2208 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1357 "seclang-parser.yy" // lalr1.cc:859
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
#line 2231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1377 "seclang-parser.yy" // lalr1.cc:859
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
#line 2262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1405 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1410 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2280 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1415 "seclang-parser.yy" // lalr1.cc:859
    {
        std::stringstream ss;
        ss << "As of ModSecurity version 3.0, SecRequestBodyInMemoryLimit is no longer ";
        ss << "supported. Instead, you can use your web server configurations to control ";
        ss << "those values. ModSecurity will follow the web server decision.";
        driver.error(yystack_[1].location, ss.str());
        YYERROR;
      }
#line 2293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1424 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1429 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1433 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1437 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1441 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2334 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 1445 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1449 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1455 "seclang-parser.yy" // lalr1.cc:859
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
#line 2366 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1467 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2376 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1473 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2384 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1477 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1487 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1490 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2408 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1498 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2416 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1502 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2424 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1509 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2433 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1514 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2443 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2453 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1526 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2463 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1532 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2474 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1539 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2485 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1549 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2493 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1553 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2501 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1557 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1561 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1565 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1569 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1573 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2541 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1577 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2549 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1581 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2557 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1585 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1589 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2573 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1593 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2581 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1597 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2589 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1601 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2597 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1605 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2605 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1609 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2613 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1613 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2621 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1617 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2629 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1621 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2637 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1625 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2645 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1629 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2653 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1633 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2661 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1637 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2669 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1641 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2677 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1645 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2685 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1649 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2693 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1653 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2701 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1657 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2709 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1661 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2717 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1665 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2725 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1669 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2733 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1673 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2741 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1677 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1681 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2757 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1685 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2765 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1689 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2773 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1693 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2781 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1697 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2789 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1701 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1705 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2805 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1709 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1713 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1717 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1721 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2837 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1725 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2845 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1729 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2853 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1733 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2861 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1737 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2869 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1741 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2877 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1745 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2885 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1749 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2893 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1753 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2901 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1757 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2909 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1761 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2917 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1765 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2925 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1769 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2933 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1773 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1777 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2949 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1781 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2957 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1785 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2965 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1789 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1793 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2981 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1797 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource_NoDictElement());
      }
#line 2989 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1802 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2997 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1806 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3005 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1810 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 3013 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1815 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3021 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3029 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1823 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 3037 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1828 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3045 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1832 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1836 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 3061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1841 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3069 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1845 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3077 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1849 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 3085 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1854 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3093 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1858 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3101 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1862 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames_NoDictElement());
      }
#line 3109 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1866 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3117 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1870 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3125 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1874 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames_NoDictElement());
      }
#line 3133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1879 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3141 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1883 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3149 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1887 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames_NoDictElement());
      }
#line 3157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1892 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1896 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3173 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1900 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames_NoDictElement());
      }
#line 3181 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1905 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3189 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1910 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 3197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1914 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 3205 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1918 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames_NoDictElement());
      }
#line 3213 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1922 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1926 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3229 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1930 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3237 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1934 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3245 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1938 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3253 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1942 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3261 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1946 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1950 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3277 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1954 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3285 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1958 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1962 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3301 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1966 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3309 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1970 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3317 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 1974 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3325 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 1978 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 1982 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3341 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 1986 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3349 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 1990 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3357 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 1994 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3365 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 1998 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3373 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2002 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3381 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2006 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3389 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2010 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3397 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2014 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3405 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2018 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3413 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2022 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3421 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2026 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3429 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2030 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3437 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2034 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3445 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2038 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3453 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2042 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3461 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2046 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3469 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2050 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3477 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2054 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3485 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2058 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3493 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2062 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3501 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2066 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2070 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2074 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2078 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2082 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3541 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2086 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3549 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2090 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3557 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2094 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2098 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3573 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2102 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3581 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2106 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3589 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2110 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3597 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2114 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2122 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3619 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2129 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2136 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2143 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3652 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2150 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3663 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2157 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3674 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2164 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3685 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2171 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2178 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2185 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3718 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2192 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3729 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2199 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3740 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2209 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3748 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2213 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3756 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2217 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2221 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3772 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2225 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3780 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2229 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3788 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2233 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2237 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3805 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2242 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3814 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2247 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3823 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2252 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3831 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2256 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3839 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2260 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3847 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2264 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3856 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2269 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3865 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2274 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3873 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2278 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3881 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2282 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3889 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2286 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3897 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2290 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3905 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2294 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3913 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2298 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3921 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2302 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3929 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2306 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3937 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2310 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3945 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2314 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3954 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2319 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Exec(yystack_[0].value.as< std::string > ()));
      }
#line 3962 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2323 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3971 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2328 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3979 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2332 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3987 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2336 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3995 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2340 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 4003 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2344 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 4011 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2348 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 4019 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2352 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 4027 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2356 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 4035 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2360 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 4043 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2364 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 4051 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2368 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 4059 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2372 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 4067 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2376 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 4075 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2380 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 4083 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2384 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 4091 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2388 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 4099 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2392 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 4107 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2396 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 4115 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2400 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 4123 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2404 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 4131 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2408 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 4139 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2412 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 4147 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2416 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(yystack_[0].value.as< std::string > ()));
      }
#line 4155 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2420 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 4163 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2424 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 4171 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2428 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<actions::Action> > () = std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ());
      }
#line 4179 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2432 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4187 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2436 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4195 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2440 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4203 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2444 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4211 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2448 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 4219 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2452 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4227 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2456 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4235 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2460 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4243 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2464 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4251 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2468 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4259 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2472 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4267 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2476 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4275 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2480 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4283 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2484 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4291 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2488 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4299 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2492 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4307 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2496 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4315 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2500 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4323 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2504 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4331 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2508 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4339 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2512 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4347 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2516 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4355 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2520 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4363 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2524 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4371 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2528 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4379 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2532 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4387 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 2536 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4395 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 2540 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4403 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 2544 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4411 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 2548 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4419 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 2552 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4427 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 2556 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4435 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 2560 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4443 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 2564 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4451 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 2568 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4459 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 373:
#line 2572 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4467 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 374:
#line 2579 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4475 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 375:
#line 2583 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4483 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 2587 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4491 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 2591 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4499 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 2595 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4507 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 2602 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4515 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 2607 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 2614 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
      }
#line 4531 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 2619 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + yystack_[0].value.as< std::string > ();
      }
#line 4539 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4543 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -287;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     588,  -287,  -208,  -287,   -90,  -287,  -287,  -287,  -287,  -287,
    -287,  -214,  -287,  -287,  -287,  -287,  -287,  -223,  -287,  -287,
    -287,  -287,  -287,  -287,    57,  -287,  -287,    56,  -287,    61,
    -287,    60,    44,   809,   809,  -287,  -287,  -287,  -287,  -222,
    -287,  -287,    17,    17,   809,    65,    67,  -287,  -287,  -287,
      69,  -287,  -287,  -287,  -287,  -287,  -287,    17,   809,   653,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,   258,    55,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -211,  -287,  -287,  -287,    71,    88,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,   900,  -287,
       6,  -287,  -287,  -287,  -287,   470,   470,    18,    20,    22,
      24,    26,    28,    30,    32,    34,    36,    38,    40,    42,
      45,    47,    51,  -287,    53,    58,    62,    64,  -287,  -287,
      72,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,    74,
    -287,  -287,  -287,  -287,  -287,    76,  -287,  -287,  -287,    78,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
      80,    96,    98,   100,   102,   104,    99,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,     5,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,   174,  -287,  -287,  -287,  -286,  -286,  -287,   187,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,    -2,
     900,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,    -4,   181,   362,  -142,   -60,    29,  -287,
    -287,   105,   111,   116,   117,  -287,   123,   124,   127,   138,
     162,   163,   164,   165,   166,   167,   168,   169,  -287,   170,
     171,  -287,   172,   173,   175,   212,  -287,  -287,   213,   214,
     215,   216,   809,  -287,  -287,  -287,   -28,   -28,   -28,  -287,
    -287,  -287,   470,   470,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,   -28,  -287,  -287,
    -287,  -287,  -287,  -287
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   113,     0,    86,    85,    87,    88,     7,
       6,     0,    11,    14,    12,    13,    17,     0,    98,    97,
      90,    99,   111,   110,     0,   102,   100,     0,   101,     0,
     103,     0,     0,     0,     0,    77,   121,   122,   123,     0,
      92,    93,     0,     0,     0,     0,     0,    25,    23,    24,
       0,   112,   118,   119,   117,   120,   116,     0,     0,     0,
       4,    71,     5,    91,    89,    15,    16,     9,    10,     8,
      20,    19,    18,    82,    81,   104,   105,    84,    83,   106,
     107,    80,    78,    79,     0,     0,   284,   285,   286,   287,
     288,   289,   290,     0,   294,   295,   296,     0,     0,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   315,
     314,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   338,   339,   340,   341,   342,   349,   359,   365,
     352,   353,   363,   364,   369,   354,   351,   358,   368,   367,
     347,   346,   345,   372,   371,   362,   360,   373,   361,   350,
     348,   366,   355,   357,   356,   370,   343,   344,     0,    74,
      29,    31,    76,   108,   109,     0,     0,   134,   137,   140,
     143,   146,   149,   152,   155,   158,   161,   164,   167,   170,
     173,   176,   179,   223,   212,   182,   209,   215,   224,   225,
     191,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     246,   245,   249,   248,   247,   250,   252,   251,   253,   218,
     254,   255,   256,   258,   257,   194,   259,   260,   219,   222,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     197,   200,   203,   206,   185,   188,     0,   272,   271,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
      94,   124,   129,    95,    96,    22,    21,    27,    26,   114,
     115,     0,    75,     1,     3,     0,   379,   337,   375,   303,
     302,   301,   292,   291,   293,   298,   297,   300,   299,     0,
       0,   131,   130,   132,   133,   135,   136,   138,   139,   141,
     142,   144,   145,   147,   148,   150,   151,   153,   154,   156,
     157,   159,   160,   162,   163,   165,   166,   168,   169,   171,
     172,   174,   175,   177,   178,   210,   211,   180,   181,   207,
     208,   213,   214,   189,   190,   216,   217,   192,   193,   220,
     221,   195,   196,   198,   199,   201,   202,   204,   205,   183,
     184,   186,   187,     0,     0,     0,     0,     0,     0,    37,
      38,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
       0,    36,     0,     0,     0,     0,    39,    40,     0,     0,
       0,     0,    73,    32,   374,   380,     0,     0,     0,    28,
      30,   125,     0,     0,   126,    35,    33,    69,    54,    53,
      55,    56,    42,    57,    50,    58,    41,    60,    59,    61,
      62,    64,    63,    65,    51,    66,    67,    68,    43,    44,
      45,    46,    47,    48,    49,    52,    72,   381,   376,   377,
     378,   128,   127,   382
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -287,  -287,   260,  -287,   -34,   178,  -287,   101,  -287,   -40,
     207,   196,   120,  -287,    73,  -131
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    59,    60,    61,   169,   170,   402,   403,    62,   270,
     271,   272,   171,   287,   288,   448
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     172,   300,   364,   273,   173,    70,    65,    66,    71,   300,
     274,   364,    72,   286,   174,    67,    68,   281,   292,   293,
      69,    63,    64,   294,   282,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   417,   254,   255,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   418,   254,   255,   412,
     413,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   365,   254,
     255,    81,   447,    82,    83,   419,   449,   450,   406,   407,
     408,   411,   289,   409,   290,   291,    73,    74,    75,    76,
      77,    78,    79,    80,   275,   276,   277,   278,   279,   280,
     295,   296,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   453,   297,   298,   284,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   299,   329,   330,   331,
     332,   420,   285,   333,   334,   335,   336,   421,   404,   405,
     337,   338,   422,   423,   339,   340,   341,   342,   446,   424,
     425,   301,   302,   426,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   427,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     410,   440,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   441,   442,
     443,   444,   445,   363,     0,     0,   416,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   286,   254,   255,
     414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   415,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,     0,     0,     0,     0,
       0,     0,     0,   283,     0,     0,     0,     2,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     0,
       0,     0,     0,     0,     0,     0,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    84,
       0,     0,     0,     0,     0,     0,     0,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      84,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      34,     3,     6,    43,   226,   228,    96,    97,   231,     3,
      44,     6,   235,   299,   236,   229,   230,    57,   229,   230,
     234,   229,   230,   234,    58,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   246,    88,    89,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   246,    88,    89,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    94,    88,
      89,   227,   300,   229,   230,   246,   407,   408,    91,    92,
      93,   285,   227,   285,   229,   230,   229,   230,   232,   233,
     229,   230,   232,   233,   229,   230,   229,   230,   229,   230,
     229,   230,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   447,   229,   230,    59,
     302,   303,   302,   303,   302,   303,   302,   303,   302,   303,
     302,   303,   302,   303,   302,   303,   302,   303,   302,   303,
     302,   303,   302,   303,   302,   303,   168,   302,   303,   302,
     303,   246,    94,   302,   303,   302,   303,   246,   285,   286,
     302,   303,   246,   246,   302,   303,   302,   303,   402,   246,
     246,   175,   176,   246,   302,   303,   302,   303,   302,   303,
     302,   303,   302,   303,   246,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   302,   303,
     302,   303,   302,   303,   302,   303,   302,   303,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     300,   246,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   246,   246,
     246,   246,   246,   256,    -1,    -1,   365,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,   299,    88,    89,
     364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     0,    -1,    -1,    -1,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,    95,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   305,
     306,   307,   312,   229,   230,    96,    97,   229,   230,   234,
     228,   231,   235,   229,   230,   232,   233,   229,   230,   232,
     233,   227,   229,   230,    90,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   285,   308,
     309,   316,   308,   226,   236,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    88,    89,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     313,   314,   315,   313,   308,   229,   230,   229,   230,   229,
     230,   313,   308,     0,   306,    94,   299,   317,   318,   227,
     229,   230,   229,   230,   234,   229,   230,   229,   230,   309,
       3,   315,   315,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   302,   303,   302,   303,   302,   303,   302,
     303,   302,   303,   314,     6,    94,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   310,   311,   318,   318,    91,    92,    93,   285,
     316,   285,     8,     9,   315,   270,   311,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   308,   300,   319,   319,
     319,   315,   315,   319
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   304,   305,   305,   305,   306,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   308,   308,
     309,   309,   310,   310,   310,   310,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   313,   313,   314,   314,   314,   314,
     314,   314,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   317,   317,   317,   317,   317,   318,
     318,   319,   319
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
       1,     2,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     4,     1,
       2,     2,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     1,
       2,     1,     2
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
  "\"URLENCODED_ERROR\"", "\"USERID\"", "\"VARIABLE_STATUS\"",
  "\"VARIABLE_IP\"", "\"VARIABLE_GLOBAL\"", "\"VARIABLE_TX\"",
  "\"VARIABLE_SESSION\"", "\"VARIABLE_USER\"", "\"RUN_TIME_VAR_ENV\"",
  "\"RUN_TIME_VAR_XML\"", "\"SetVar\"", "SETVAR_OPERATION_EQUALS",
  "SETVAR_OPERATION_EQUALS_PLUS", "SETVAR_OPERATION_EQUALS_MINUS",
  "\"NOT\"", "CONFIG_DIR_AUDIT_LOG_FMT", "JSON", "NATIVE",
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
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG\"",
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
       0,   693,   693,   697,   698,   701,   706,   712,   718,   722,
     726,   732,   738,   744,   750,   755,   760,   766,   773,   777,
     781,   787,   791,   795,   800,   805,   810,   814,   821,   825,
     832,   838,   848,   857,   867,   876,   888,   892,   896,   900,
     904,   908,   917,   926,   930,   934,   939,   943,   947,   952,
     957,   962,   967,   971,   975,   979,   983,   987,   991,   995,
     999,  1003,  1007,  1011,  1015,  1019,  1023,  1027,  1031,  1035,
    1039,  1053,  1054,  1078,  1097,  1112,  1136,  1183,  1187,  1191,
    1195,  1199,  1203,  1207,  1211,  1215,  1224,  1228,  1233,  1238,
    1243,  1248,  1253,  1266,  1279,  1292,  1318,  1344,  1356,  1376,
    1404,  1409,  1414,  1423,  1428,  1432,  1436,  1440,  1444,  1448,
    1452,  1453,  1454,  1466,  1472,  1476,  1480,  1481,  1482,  1483,
    1484,  1485,  1486,  1489,  1497,  1501,  1508,  1513,  1519,  1525,
    1531,  1538,  1548,  1552,  1556,  1560,  1564,  1568,  1572,  1576,
    1580,  1584,  1588,  1592,  1596,  1600,  1604,  1608,  1612,  1616,
    1620,  1624,  1628,  1632,  1636,  1640,  1644,  1648,  1652,  1656,
    1660,  1664,  1668,  1672,  1676,  1680,  1684,  1688,  1692,  1696,
    1700,  1704,  1708,  1712,  1716,  1720,  1724,  1728,  1732,  1736,
    1740,  1744,  1748,  1752,  1756,  1760,  1764,  1768,  1772,  1776,
    1780,  1784,  1788,  1792,  1796,  1801,  1805,  1809,  1814,  1818,
    1822,  1827,  1831,  1835,  1840,  1844,  1848,  1853,  1857,  1861,
    1865,  1869,  1873,  1878,  1882,  1886,  1891,  1895,  1899,  1904,
    1909,  1913,  1917,  1921,  1925,  1929,  1933,  1937,  1941,  1945,
    1949,  1953,  1957,  1961,  1965,  1969,  1973,  1977,  1981,  1985,
    1989,  1993,  1997,  2001,  2005,  2009,  2013,  2017,  2021,  2025,
    2029,  2033,  2037,  2041,  2045,  2049,  2053,  2057,  2061,  2065,
    2069,  2073,  2077,  2081,  2085,  2089,  2093,  2097,  2101,  2105,
    2109,  2113,  2121,  2128,  2135,  2142,  2149,  2156,  2163,  2170,
    2177,  2184,  2191,  2198,  2208,  2212,  2216,  2220,  2224,  2228,
    2232,  2236,  2241,  2246,  2251,  2255,  2259,  2263,  2268,  2273,
    2277,  2281,  2285,  2289,  2293,  2297,  2301,  2305,  2309,  2313,
    2318,  2322,  2327,  2331,  2335,  2339,  2343,  2347,  2351,  2355,
    2359,  2363,  2367,  2371,  2375,  2379,  2383,  2387,  2391,  2395,
    2399,  2403,  2407,  2411,  2415,  2419,  2423,  2427,  2431,  2435,
    2439,  2443,  2447,  2451,  2455,  2459,  2463,  2467,  2471,  2475,
    2479,  2483,  2487,  2491,  2495,  2499,  2503,  2507,  2511,  2515,
    2519,  2523,  2527,  2531,  2535,  2539,  2543,  2547,  2551,  2555,
    2559,  2563,  2567,  2571,  2578,  2582,  2586,  2590,  2594,  2601,
    2606,  2613,  2618
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
#line 5516 "seclang-parser.cc" // lalr1.cc:1167
#line 2624 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
