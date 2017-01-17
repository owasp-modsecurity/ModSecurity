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
#line 309 "seclang-parser.yy" // lalr1.cc:413

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
      case 66: // "Accuracy"
      case 67: // "Allow"
      case 68: // "Append"
      case 69: // "AuditLog"
      case 70: // "Block"
      case 71: // "Capture"
      case 72: // "Chain"
      case 73: // "ACTION_CTL_AUDIT_ENGINE"
      case 74: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 75: // "ACTION_CTL_BDY_JSON"
      case 76: // "ACTION_CTL_BDY_XML"
      case 77: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 78: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 79: // "ACTION_CTL_RULE_ENGINE"
      case 80: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 81: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 83: // "Deny"
      case 84: // "DeprecateVar"
      case 85: // "Drop"
      case 86: // "Exec"
      case 87: // "ExpireVar"
      case 88: // "Id"
      case 89: // "InitCol"
      case 90: // "Log"
      case 91: // "LogData"
      case 92: // "Maturity"
      case 93: // "Msg"
      case 94: // "MultiMatch"
      case 95: // "NoAuditLog"
      case 96: // "NoLog"
      case 97: // "Pass"
      case 98: // "Pause"
      case 99: // "Phase"
      case 100: // "Prepend"
      case 101: // "Proxy"
      case 102: // "Redirect"
      case 103: // "Rev"
      case 104: // "SanatiseArg"
      case 105: // "SanatiseMatched"
      case 106: // "SanatiseMatchedBytes"
      case 107: // "SanatiseRequestHeader"
      case 108: // "SanatiseResponseHeader"
      case 109: // "SetEnv"
      case 110: // "SetSrc"
      case 111: // "SetSid"
      case 112: // "SetUID"
      case 113: // "Severity"
      case 114: // "Skip"
      case 115: // "SkipAfter"
      case 116: // "Status"
      case 117: // "Tag"
      case 118: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 119: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 120: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 121: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 122: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 123: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 124: // "ACTION_TRANSFORMATION_LENGTH"
      case 125: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 126: // "ACTION_TRANSFORMATION_MD5"
      case 127: // "ACTION_TRANSFORMATION_NONE"
      case 128: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 130: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 131: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 137: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 139: // "ACTION_TRANSFORMATION_SHA1"
      case 140: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 141: // "ACTION_TRANSFORMATION_TRIM"
      case 142: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 144: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 145: // "Ver"
      case 146: // "xmlns"
      case 147: // "CONFIG_COMPONENT_SIG"
      case 148: // "CONFIG_DIR_AUDIT_DIR"
      case 149: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 150: // "CONFIG_DIR_AUDIT_ENG"
      case 151: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 152: // "CONFIG_DIR_AUDIT_LOG"
      case 153: // "CONFIG_DIR_AUDIT_LOG2"
      case 154: // "CONFIG_DIR_AUDIT_LOG_P"
      case 155: // "CONFIG_DIR_AUDIT_STS"
      case 156: // "CONFIG_DIR_AUDIT_TPE"
      case 157: // "CONFIG_DIR_DEBUG_LOG"
      case 158: // "CONFIG_DIR_DEBUG_LVL"
      case 159: // "CONFIG_DIR_GEO_DB"
      case 160: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 162: // "CONFIG_DIR_REQ_BODY"
      case 163: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 164: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 166: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 167: // "CONFIG_DIR_RES_BODY"
      case 168: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 170: // "CONFIG_DIR_RULE_ENG"
      case 171: // "CONFIG_DIR_SEC_ACTION"
      case 172: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 173: // "CONFIG_DIR_SEC_MARKER"
      case 174: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 175: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 176: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 177: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 178: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 179: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 180: // "CONFIG_UPLOAD_DIR"
      case 181: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 182: // "CONFIG_UPLOAD_FILE_MODE"
      case 183: // "CONFIG_VALUE_ABORT"
      case 184: // "CONFIG_VALUE_DETC"
      case 185: // "CONFIG_VALUE_HTTPS"
      case 186: // "CONFIG_VALUE_OFF"
      case 187: // "CONFIG_VALUE_ON"
      case 188: // "CONFIG_VALUE_PARALLEL"
      case 189: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 190: // "CONFIG_VALUE_REJECT"
      case 191: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 192: // "CONFIG_VALUE_SERIAL"
      case 193: // "CONFIG_VALUE_WARN"
      case 194: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 195: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 196: // "CONGIG_DIR_SEC_ARG_SEP"
      case 197: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 198: // "CONGIG_DIR_SEC_DATA_DIR"
      case 199: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 200: // "CONGIG_DIR_SEC_TMP_DIR"
      case 201: // "DIRECTIVE"
      case 202: // "DIRECTIVE_SECRULESCRIPT"
      case 203: // "FREE_TEXT"
      case 204: // "OPERATOR"
      case 205: // "OPERATOR_BEGINS_WITH"
      case 206: // "OPERATOR_CONTAINS"
      case 207: // "OPERATOR_CONTAINS_WORD"
      case 208: // "OPERATOR_DETECT_SQLI"
      case 209: // "OPERATOR_DETECT_XSS"
      case 210: // "OPERATOR_ENDS_WITH"
      case 211: // "OPERATOR_EQ"
      case 212: // "OPERATOR_FUZZY_HASH"
      case 213: // "OPERATOR_GE"
      case 214: // "OPERATOR_GEOLOOKUP"
      case 215: // "OPERATOR_GSB_LOOKUP"
      case 216: // "OPERATOR_GT"
      case 217: // "OPERATOR_INSPECT_FILE"
      case 218: // "OPERATOR_IP_MATCH"
      case 219: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 220: // "OPERATOR_LE"
      case 221: // "OPERATOR_LT"
      case 222: // "OPERATOR_PM"
      case 223: // "OPERATOR_PM_FROM_FILE"
      case 224: // "OPERATOR_RBL"
      case 225: // "OPERATOR_RSUB"
      case 226: // "OPERATOR_RX"
      case 227: // "Operator Rx"
      case 228: // "OPERATOR_STR_EQ"
      case 229: // "OPERATOR_STR_MATCH"
      case 230: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 231: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 232: // "OPERATOR_VALIDATE_DTD"
      case 233: // "OPERATOR_VALIDATE_HASH"
      case 234: // "OPERATOR_VALIDATE_SCHEMA"
      case 235: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 236: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 237: // "OPERATOR_VERIFY_CC"
      case 238: // "OPERATOR_VERIFY_CPF"
      case 239: // "OPERATOR_VERIFY_SSN"
      case 240: // "OPERATOR_WITHIN"
      case 241: // "OP_QUOTE"
      case 242: // "QUOTATION_MARK"
      case 243: // "RUN_TIME_VAR_BLD"
      case 244: // "RUN_TIME_VAR_DUR"
      case 245: // "RUN_TIME_VAR_ENV"
      case 246: // "RUN_TIME_VAR_HSV"
      case 247: // "RUN_TIME_VAR_REMOTE_USER"
      case 248: // "RUN_TIME_VAR_RULE"
      case 249: // "RUN_TIME_VAR_TIME"
      case 250: // "RUN_TIME_VAR_TIME_DAY"
      case 251: // "RUN_TIME_VAR_TIME_EPOCH"
      case 252: // "RUN_TIME_VAR_TIME_HOUR"
      case 253: // "RUN_TIME_VAR_TIME_MIN"
      case 254: // "RUN_TIME_VAR_TIME_MON"
      case 255: // "RUN_TIME_VAR_TIME_SEC"
      case 256: // "RUN_TIME_VAR_TIME_WDAY"
      case 257: // "RUN_TIME_VAR_TIME_YEAR"
      case 258: // "RUN_TIME_VAR_XML"
      case 259: // "VARIABLE"
      case 260: // "VARIABLE_COL"
      case 261: // "VARIABLE_STATUS"
      case 262: // "VARIABLE_TX"
        value.move< std::string > (that.value);
        break;

      case 269: // op
      case 270: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 273: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 274: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 272: // variables
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 267: // actions
      case 268: // actions_may_quoted
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
      case 66: // "Accuracy"
      case 67: // "Allow"
      case 68: // "Append"
      case 69: // "AuditLog"
      case 70: // "Block"
      case 71: // "Capture"
      case 72: // "Chain"
      case 73: // "ACTION_CTL_AUDIT_ENGINE"
      case 74: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 75: // "ACTION_CTL_BDY_JSON"
      case 76: // "ACTION_CTL_BDY_XML"
      case 77: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 78: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 79: // "ACTION_CTL_RULE_ENGINE"
      case 80: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 81: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 83: // "Deny"
      case 84: // "DeprecateVar"
      case 85: // "Drop"
      case 86: // "Exec"
      case 87: // "ExpireVar"
      case 88: // "Id"
      case 89: // "InitCol"
      case 90: // "Log"
      case 91: // "LogData"
      case 92: // "Maturity"
      case 93: // "Msg"
      case 94: // "MultiMatch"
      case 95: // "NoAuditLog"
      case 96: // "NoLog"
      case 97: // "Pass"
      case 98: // "Pause"
      case 99: // "Phase"
      case 100: // "Prepend"
      case 101: // "Proxy"
      case 102: // "Redirect"
      case 103: // "Rev"
      case 104: // "SanatiseArg"
      case 105: // "SanatiseMatched"
      case 106: // "SanatiseMatchedBytes"
      case 107: // "SanatiseRequestHeader"
      case 108: // "SanatiseResponseHeader"
      case 109: // "SetEnv"
      case 110: // "SetSrc"
      case 111: // "SetSid"
      case 112: // "SetUID"
      case 113: // "Severity"
      case 114: // "Skip"
      case 115: // "SkipAfter"
      case 116: // "Status"
      case 117: // "Tag"
      case 118: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 119: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 120: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 121: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 122: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 123: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 124: // "ACTION_TRANSFORMATION_LENGTH"
      case 125: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 126: // "ACTION_TRANSFORMATION_MD5"
      case 127: // "ACTION_TRANSFORMATION_NONE"
      case 128: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 130: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 131: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 137: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 139: // "ACTION_TRANSFORMATION_SHA1"
      case 140: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 141: // "ACTION_TRANSFORMATION_TRIM"
      case 142: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 144: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 145: // "Ver"
      case 146: // "xmlns"
      case 147: // "CONFIG_COMPONENT_SIG"
      case 148: // "CONFIG_DIR_AUDIT_DIR"
      case 149: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 150: // "CONFIG_DIR_AUDIT_ENG"
      case 151: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 152: // "CONFIG_DIR_AUDIT_LOG"
      case 153: // "CONFIG_DIR_AUDIT_LOG2"
      case 154: // "CONFIG_DIR_AUDIT_LOG_P"
      case 155: // "CONFIG_DIR_AUDIT_STS"
      case 156: // "CONFIG_DIR_AUDIT_TPE"
      case 157: // "CONFIG_DIR_DEBUG_LOG"
      case 158: // "CONFIG_DIR_DEBUG_LVL"
      case 159: // "CONFIG_DIR_GEO_DB"
      case 160: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 162: // "CONFIG_DIR_REQ_BODY"
      case 163: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 164: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 166: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 167: // "CONFIG_DIR_RES_BODY"
      case 168: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 170: // "CONFIG_DIR_RULE_ENG"
      case 171: // "CONFIG_DIR_SEC_ACTION"
      case 172: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 173: // "CONFIG_DIR_SEC_MARKER"
      case 174: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 175: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 176: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 177: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 178: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 179: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 180: // "CONFIG_UPLOAD_DIR"
      case 181: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 182: // "CONFIG_UPLOAD_FILE_MODE"
      case 183: // "CONFIG_VALUE_ABORT"
      case 184: // "CONFIG_VALUE_DETC"
      case 185: // "CONFIG_VALUE_HTTPS"
      case 186: // "CONFIG_VALUE_OFF"
      case 187: // "CONFIG_VALUE_ON"
      case 188: // "CONFIG_VALUE_PARALLEL"
      case 189: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 190: // "CONFIG_VALUE_REJECT"
      case 191: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 192: // "CONFIG_VALUE_SERIAL"
      case 193: // "CONFIG_VALUE_WARN"
      case 194: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 195: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 196: // "CONGIG_DIR_SEC_ARG_SEP"
      case 197: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 198: // "CONGIG_DIR_SEC_DATA_DIR"
      case 199: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 200: // "CONGIG_DIR_SEC_TMP_DIR"
      case 201: // "DIRECTIVE"
      case 202: // "DIRECTIVE_SECRULESCRIPT"
      case 203: // "FREE_TEXT"
      case 204: // "OPERATOR"
      case 205: // "OPERATOR_BEGINS_WITH"
      case 206: // "OPERATOR_CONTAINS"
      case 207: // "OPERATOR_CONTAINS_WORD"
      case 208: // "OPERATOR_DETECT_SQLI"
      case 209: // "OPERATOR_DETECT_XSS"
      case 210: // "OPERATOR_ENDS_WITH"
      case 211: // "OPERATOR_EQ"
      case 212: // "OPERATOR_FUZZY_HASH"
      case 213: // "OPERATOR_GE"
      case 214: // "OPERATOR_GEOLOOKUP"
      case 215: // "OPERATOR_GSB_LOOKUP"
      case 216: // "OPERATOR_GT"
      case 217: // "OPERATOR_INSPECT_FILE"
      case 218: // "OPERATOR_IP_MATCH"
      case 219: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 220: // "OPERATOR_LE"
      case 221: // "OPERATOR_LT"
      case 222: // "OPERATOR_PM"
      case 223: // "OPERATOR_PM_FROM_FILE"
      case 224: // "OPERATOR_RBL"
      case 225: // "OPERATOR_RSUB"
      case 226: // "OPERATOR_RX"
      case 227: // "Operator Rx"
      case 228: // "OPERATOR_STR_EQ"
      case 229: // "OPERATOR_STR_MATCH"
      case 230: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 231: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 232: // "OPERATOR_VALIDATE_DTD"
      case 233: // "OPERATOR_VALIDATE_HASH"
      case 234: // "OPERATOR_VALIDATE_SCHEMA"
      case 235: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 236: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 237: // "OPERATOR_VERIFY_CC"
      case 238: // "OPERATOR_VERIFY_CPF"
      case 239: // "OPERATOR_VERIFY_SSN"
      case 240: // "OPERATOR_WITHIN"
      case 241: // "OP_QUOTE"
      case 242: // "QUOTATION_MARK"
      case 243: // "RUN_TIME_VAR_BLD"
      case 244: // "RUN_TIME_VAR_DUR"
      case 245: // "RUN_TIME_VAR_ENV"
      case 246: // "RUN_TIME_VAR_HSV"
      case 247: // "RUN_TIME_VAR_REMOTE_USER"
      case 248: // "RUN_TIME_VAR_RULE"
      case 249: // "RUN_TIME_VAR_TIME"
      case 250: // "RUN_TIME_VAR_TIME_DAY"
      case 251: // "RUN_TIME_VAR_TIME_EPOCH"
      case 252: // "RUN_TIME_VAR_TIME_HOUR"
      case 253: // "RUN_TIME_VAR_TIME_MIN"
      case 254: // "RUN_TIME_VAR_TIME_MON"
      case 255: // "RUN_TIME_VAR_TIME_SEC"
      case 256: // "RUN_TIME_VAR_TIME_WDAY"
      case 257: // "RUN_TIME_VAR_TIME_YEAR"
      case 258: // "RUN_TIME_VAR_XML"
      case 259: // "VARIABLE"
      case 260: // "VARIABLE_COL"
      case 261: // "VARIABLE_STATUS"
      case 262: // "VARIABLE_TX"
        value.copy< std::string > (that.value);
        break;

      case 269: // op
      case 270: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 273: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 274: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 272: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 267: // actions
      case 268: // actions_may_quoted
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
    #line 302 "/home/zimmerle/core/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 861 "seclang-parser.cc" // lalr1.cc:741

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
      case 66: // "Accuracy"
      case 67: // "Allow"
      case 68: // "Append"
      case 69: // "AuditLog"
      case 70: // "Block"
      case 71: // "Capture"
      case 72: // "Chain"
      case 73: // "ACTION_CTL_AUDIT_ENGINE"
      case 74: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 75: // "ACTION_CTL_BDY_JSON"
      case 76: // "ACTION_CTL_BDY_XML"
      case 77: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 78: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 79: // "ACTION_CTL_RULE_ENGINE"
      case 80: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 81: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 83: // "Deny"
      case 84: // "DeprecateVar"
      case 85: // "Drop"
      case 86: // "Exec"
      case 87: // "ExpireVar"
      case 88: // "Id"
      case 89: // "InitCol"
      case 90: // "Log"
      case 91: // "LogData"
      case 92: // "Maturity"
      case 93: // "Msg"
      case 94: // "MultiMatch"
      case 95: // "NoAuditLog"
      case 96: // "NoLog"
      case 97: // "Pass"
      case 98: // "Pause"
      case 99: // "Phase"
      case 100: // "Prepend"
      case 101: // "Proxy"
      case 102: // "Redirect"
      case 103: // "Rev"
      case 104: // "SanatiseArg"
      case 105: // "SanatiseMatched"
      case 106: // "SanatiseMatchedBytes"
      case 107: // "SanatiseRequestHeader"
      case 108: // "SanatiseResponseHeader"
      case 109: // "SetEnv"
      case 110: // "SetSrc"
      case 111: // "SetSid"
      case 112: // "SetUID"
      case 113: // "Severity"
      case 114: // "Skip"
      case 115: // "SkipAfter"
      case 116: // "Status"
      case 117: // "Tag"
      case 118: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 119: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 120: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 121: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 122: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 123: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 124: // "ACTION_TRANSFORMATION_LENGTH"
      case 125: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 126: // "ACTION_TRANSFORMATION_MD5"
      case 127: // "ACTION_TRANSFORMATION_NONE"
      case 128: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 130: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 131: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 137: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 139: // "ACTION_TRANSFORMATION_SHA1"
      case 140: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 141: // "ACTION_TRANSFORMATION_TRIM"
      case 142: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 144: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 145: // "Ver"
      case 146: // "xmlns"
      case 147: // "CONFIG_COMPONENT_SIG"
      case 148: // "CONFIG_DIR_AUDIT_DIR"
      case 149: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 150: // "CONFIG_DIR_AUDIT_ENG"
      case 151: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 152: // "CONFIG_DIR_AUDIT_LOG"
      case 153: // "CONFIG_DIR_AUDIT_LOG2"
      case 154: // "CONFIG_DIR_AUDIT_LOG_P"
      case 155: // "CONFIG_DIR_AUDIT_STS"
      case 156: // "CONFIG_DIR_AUDIT_TPE"
      case 157: // "CONFIG_DIR_DEBUG_LOG"
      case 158: // "CONFIG_DIR_DEBUG_LVL"
      case 159: // "CONFIG_DIR_GEO_DB"
      case 160: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 162: // "CONFIG_DIR_REQ_BODY"
      case 163: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 164: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 166: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 167: // "CONFIG_DIR_RES_BODY"
      case 168: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 170: // "CONFIG_DIR_RULE_ENG"
      case 171: // "CONFIG_DIR_SEC_ACTION"
      case 172: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 173: // "CONFIG_DIR_SEC_MARKER"
      case 174: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 175: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 176: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 177: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 178: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 179: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 180: // "CONFIG_UPLOAD_DIR"
      case 181: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 182: // "CONFIG_UPLOAD_FILE_MODE"
      case 183: // "CONFIG_VALUE_ABORT"
      case 184: // "CONFIG_VALUE_DETC"
      case 185: // "CONFIG_VALUE_HTTPS"
      case 186: // "CONFIG_VALUE_OFF"
      case 187: // "CONFIG_VALUE_ON"
      case 188: // "CONFIG_VALUE_PARALLEL"
      case 189: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 190: // "CONFIG_VALUE_REJECT"
      case 191: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 192: // "CONFIG_VALUE_SERIAL"
      case 193: // "CONFIG_VALUE_WARN"
      case 194: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 195: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 196: // "CONGIG_DIR_SEC_ARG_SEP"
      case 197: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 198: // "CONGIG_DIR_SEC_DATA_DIR"
      case 199: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 200: // "CONGIG_DIR_SEC_TMP_DIR"
      case 201: // "DIRECTIVE"
      case 202: // "DIRECTIVE_SECRULESCRIPT"
      case 203: // "FREE_TEXT"
      case 204: // "OPERATOR"
      case 205: // "OPERATOR_BEGINS_WITH"
      case 206: // "OPERATOR_CONTAINS"
      case 207: // "OPERATOR_CONTAINS_WORD"
      case 208: // "OPERATOR_DETECT_SQLI"
      case 209: // "OPERATOR_DETECT_XSS"
      case 210: // "OPERATOR_ENDS_WITH"
      case 211: // "OPERATOR_EQ"
      case 212: // "OPERATOR_FUZZY_HASH"
      case 213: // "OPERATOR_GE"
      case 214: // "OPERATOR_GEOLOOKUP"
      case 215: // "OPERATOR_GSB_LOOKUP"
      case 216: // "OPERATOR_GT"
      case 217: // "OPERATOR_INSPECT_FILE"
      case 218: // "OPERATOR_IP_MATCH"
      case 219: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 220: // "OPERATOR_LE"
      case 221: // "OPERATOR_LT"
      case 222: // "OPERATOR_PM"
      case 223: // "OPERATOR_PM_FROM_FILE"
      case 224: // "OPERATOR_RBL"
      case 225: // "OPERATOR_RSUB"
      case 226: // "OPERATOR_RX"
      case 227: // "Operator Rx"
      case 228: // "OPERATOR_STR_EQ"
      case 229: // "OPERATOR_STR_MATCH"
      case 230: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 231: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 232: // "OPERATOR_VALIDATE_DTD"
      case 233: // "OPERATOR_VALIDATE_HASH"
      case 234: // "OPERATOR_VALIDATE_SCHEMA"
      case 235: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 236: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 237: // "OPERATOR_VERIFY_CC"
      case 238: // "OPERATOR_VERIFY_CPF"
      case 239: // "OPERATOR_VERIFY_SSN"
      case 240: // "OPERATOR_WITHIN"
      case 241: // "OP_QUOTE"
      case 242: // "QUOTATION_MARK"
      case 243: // "RUN_TIME_VAR_BLD"
      case 244: // "RUN_TIME_VAR_DUR"
      case 245: // "RUN_TIME_VAR_ENV"
      case 246: // "RUN_TIME_VAR_HSV"
      case 247: // "RUN_TIME_VAR_REMOTE_USER"
      case 248: // "RUN_TIME_VAR_RULE"
      case 249: // "RUN_TIME_VAR_TIME"
      case 250: // "RUN_TIME_VAR_TIME_DAY"
      case 251: // "RUN_TIME_VAR_TIME_EPOCH"
      case 252: // "RUN_TIME_VAR_TIME_HOUR"
      case 253: // "RUN_TIME_VAR_TIME_MIN"
      case 254: // "RUN_TIME_VAR_TIME_MON"
      case 255: // "RUN_TIME_VAR_TIME_SEC"
      case 256: // "RUN_TIME_VAR_TIME_WDAY"
      case 257: // "RUN_TIME_VAR_TIME_YEAR"
      case 258: // "RUN_TIME_VAR_XML"
      case 259: // "VARIABLE"
      case 260: // "VARIABLE_COL"
      case 261: // "VARIABLE_STATUS"
      case 262: // "VARIABLE_TX"
        yylhs.value.build< std::string > ();
        break;

      case 269: // op
      case 270: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 273: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 274: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 272: // variables
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 267: // actions
      case 268: // actions_may_quoted
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
#line 604 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1195 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 617 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1203 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 623 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1211 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 629 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1219 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 633 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1227 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 637 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1235 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 643 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1243 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 649 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1251 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 655 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1259 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 661 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1267 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 667 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 676 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 680 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 684 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 690 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1310 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 694 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1318 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 698 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 703 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1336 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 708 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1345 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 713 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 717 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1361 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 724 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1369 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 728 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1377 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 735 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1387 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 741 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1398 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 751 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1411 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 760 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1425 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 770 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1438 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 779 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("!" + utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1451 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 791 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1459 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 795 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1467 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 799 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1475 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 803 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1483 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 807 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1491 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 811 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 816 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 821 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 825 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 829 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1534 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 834 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 838 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1550 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 842 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1559 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 847 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1568 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 852 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1577 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 857 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1586 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 862 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1594 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 866 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1602 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 870 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1610 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 874 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1618 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 878 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1626 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 882 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1634 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 886 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1642 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 890 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1650 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 894 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1658 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 898 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1666 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 902 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1674 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 906 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1682 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 910 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1690 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 914 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 918 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1706 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 922 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1714 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 926 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1722 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 930 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1730 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 934 "seclang-parser.yy" // lalr1.cc:859
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
#line 1745 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 949 "seclang-parser.yy" // lalr1.cc:859
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
#line 1773 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 973 "seclang-parser.yy" // lalr1.cc:859
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
#line 1796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 992 "seclang-parser.yy" // lalr1.cc:859
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
#line 1815 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1007 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1827 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1015 "seclang-parser.yy" // lalr1.cc:859
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
#line 1878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1062 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1066 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1070 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1074 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1078 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1082 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1086 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1090 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1094 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1098 "seclang-parser.yy" // lalr1.cc:859
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
#line 1967 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1112 "seclang-parser.yy" // lalr1.cc:859
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
#line 1983 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1124 "seclang-parser.yy" // lalr1.cc:859
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
#line 2006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1144 "seclang-parser.yy" // lalr1.cc:859
    {
#ifdef WITH_GEOIP
        std::string file = modsecurity::utils::find_resource(yystack_[0].value.as< std::string > (),
            driver.ref.back());
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
#line 2029 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1164 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2038 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1169 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2047 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1174 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2056 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1179 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2065 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1184 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2073 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1188 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2081 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1192 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2089 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1196 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2097 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1200 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2105 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1204 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2113 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1210 "seclang-parser.yy" // lalr1.cc:859
    {
        std::istringstream buf(yystack_[0].value.as< std::string > ());
        std::istream_iterator<std::string> beg(buf), end;
        std::set<std::string> tokens(beg, end);
        for (std::set<std::string>::iterator it=tokens.begin();
            it!=tokens.end(); ++it)
        {
            driver.m_responseBodyTypeToBeInspected.insert(*it);
        }
      }
#line 2128 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1221 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2136 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1225 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2144 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1235 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1241 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2160 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1246 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2170 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1255 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2179 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1260 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsGetNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2188 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1265 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsPostNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1270 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestHeadersNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1275 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseContentType());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1280 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseHeadersNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2224 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1285 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsCombinedSize());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1290 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::AuthType());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2242 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1295 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FilesCombinedSize());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2251 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1300 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FilesTmpNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1305 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FullRequest());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1310 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FullRequestLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1315 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::InboundDataError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1320 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MatchedVar());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2296 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1325 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MatchedVarName());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2305 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1330 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartCrlfLFLines());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2314 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1335 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartDateAfter());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2323 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1340 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartFileLimitExceeded());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2332 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1345 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartHeaderFolding());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2341 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1350 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartInvalidHeaderFolding());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1355 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartInvalidQuoting());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1360 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartStrictError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2368 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1365 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartUnmatchedBoundary());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2377 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1370 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::OutboundDataError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1375 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::PathInfo());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2395 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1380 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::QueryString());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1385 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemoteAddr());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2413 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1390 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemoteHost());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1395 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemotePort());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1400 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2440 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1405 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyErrorMsg());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2449 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1410 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessor());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2458 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1415 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessorError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2467 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1420 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessorErrorMsg());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1425 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBasename());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2485 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1430 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBody());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1435 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBodyLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1440 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestFilename());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1445 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestLine());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2521 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1450 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestMethod());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2530 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1455 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestProtocol());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2539 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1460 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestURI());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1465 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestURIRaw());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2557 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1470 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::Resource());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2566 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1475 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseBody());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1480 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseContentLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2584 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1485 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseProtocol());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2593 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1490 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseStatus());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2602 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1495 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerAddr());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2611 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1500 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerName());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2620 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1505 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerPort());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2629 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1510 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::SessionID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1515 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UniqueID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UrlEncodedError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1525 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UserID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1530 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Variable(name, Variable::VariableKind::DirectVariable)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Variable(name, Variable::VariableKind::DirectVariable)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Variable(name, Variable::VariableKind::DirectVariable));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2686 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1547 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Variable(name, Variable::VariableKind::CollectionVarible)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Variable(name, Variable::VariableKind::CollectionVarible)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Variable(name, Variable::VariableKind::CollectionVarible));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1564 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Tx(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Tx(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Tx(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1581 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Duration(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Duration(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Duration(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1598 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Env(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Env(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Env(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2770 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1615 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new ModsecBuild(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new ModsecBuild(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new ModsecBuild(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2791 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1632 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new HighestSeverity(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new HighestSeverity(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new HighestSeverity(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2812 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1649 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new RemoteUser(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new RemoteUser(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new RemoteUser(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1666 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Time(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Time(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Time(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1683 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeDay(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeDay(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeDay(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2875 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1700 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeEpoch(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeEpoch(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeEpoch(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2896 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1717 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeHour(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeHour(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeHour(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2917 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1734 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeMin(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeMin(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeMin(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2938 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1751 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeMon(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeMon(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeMon(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2959 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1768 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeSec(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeSec(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2980 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1785 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeWDay(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeWDay(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeWDay(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 3001 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1802 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new TimeYear(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new TimeYear(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new TimeYear(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 3022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
            std::string name(yystack_[0].value.as< std::string > ());
            std::unique_ptr<Variable> c(new XML(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3032 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1825 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new Variable(name, Variable::VariableKind::DirectVariable)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new Variable(name, Variable::VariableKind::DirectVariable)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new Variable(name, Variable::VariableKind::DirectVariable));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 3053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1845 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1849 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3069 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1853 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3077 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1857 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3085 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1861 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3093 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1865 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3101 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1869 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3109 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1873 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1878 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3127 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1883 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3136 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1888 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3144 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1892 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3152 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1896 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3160 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1900 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3169 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1905 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3178 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1910 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3186 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1914 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3194 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1918 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3203 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1923 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3212 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1928 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1933 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3229 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1937 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3237 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1941 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3245 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1945 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3253 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1949 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3261 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1953 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1958 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1962 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1967 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1971 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1975 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1979 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1983 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1987 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 3335 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1991 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 3343 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1995 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1999 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 3359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 2003 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 3367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 2007 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 3375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 2011 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 3383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 2015 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 3391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 2019 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 3399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 2023 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 3407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 2027 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 3415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 2031 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 3423 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 2035 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 3431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 2039 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 3439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 2043 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 3447 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 2047 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 2051 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3463 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 2055 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetRSC", yystack_[1].location);
      }
#line 3471 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2059 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3479 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2063 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2067 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3495 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2071 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2075 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3511 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2079 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3519 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2083 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2087 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2091 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3543 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2095 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2099 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3559 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2103 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3567 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2107 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2111 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2115 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2119 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2123 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2127 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2131 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 3623 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2135 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 3631 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2139 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 3639 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2143 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 3647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2147 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 3655 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2151 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 3663 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2155 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3671 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2159 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 3679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2163 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2167 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3695 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2171 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2175 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3711 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2179 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3719 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2183 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3727 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2187 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3735 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2191 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 3743 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2195 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 3751 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2199 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 3759 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2203 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 3767 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2207 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 3775 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2211 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 3783 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2215 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 3791 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2219 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 3799 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 3803 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -252;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
       2,  -252,  -252,  -252,  -252,  -182,  -252,  -252,  -252,  -252,
    -252,  -178,  -252,  -252,  -252,  -252,  -252,  -169,  -252,  -252,
    -160,  -252,   -59,  -252,   -60,  -163,   327,   327,  -252,  -252,
    -252,  -180,  -252,   -55,   -53,  -252,  -252,  -252,   -51,  -252,
    -252,  -252,  -252,  -252,  -252,    25,   327,   185,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,   -54,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -171,  -252,  -252,  -252,
     -49,   -47,  -159,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
     413,  -252,     3,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,    83,  -252,  -252,
    -252,  -252,  -251,    62,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,    -2,   413,    25,  -117,  -191,  -181,
    -177,  -252,  -252,   -62,   -61,   -58,   -57,  -252,   -56,   -17,
     -16,   -15,   -14,   -13,   -12,   -11,   -10,    -9,    -8,     4,
    -252,     5,     6,  -252,     7,     8,     9,    10,  -252,  -252,
      11,    12,    13,    14,   327,  -252,  -252,    15,    16,    17,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252,
    -252,  -252,  -252,  -252,  -252,  -252,  -252,  -252
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,    83,     7,     6,     0,    11,    14,    12,    13,
      15,     0,    86,    85,    87,    99,    98,     0,    90,    88,
       0,    89,     0,    91,     0,     0,     0,     0,    75,   108,
     109,     0,    84,     0,     0,    23,    21,    22,     0,   100,
     105,   106,   104,   107,   103,     0,     0,     0,     4,    69,
       5,     9,    10,     8,    18,    17,    16,    80,    79,    92,
      93,    82,    81,    94,    95,    78,    76,    77,     0,   186,
     187,   188,   189,   190,   191,   192,     0,   196,   197,   198,
       0,     0,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   217,   216,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   244,   245,   246,   247,
     248,   255,   263,   269,   258,   267,   268,   273,   259,   257,
     262,   272,   271,   253,   252,   251,   276,   275,   266,   264,
     277,   265,   256,   254,   270,   261,   260,   274,   249,   250,
       0,    72,    27,    29,    74,    96,    97,    20,    19,    25,
      24,   101,   102,   118,   113,   112,   114,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   142,
     141,   145,   144,   143,   146,   148,   147,   149,   115,   150,
     151,   152,   154,   153,   155,   156,   157,   116,   117,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   172,   170,
     171,   173,   174,   185,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   168,   167,   169,     0,   111,    73,
       1,     3,     0,   240,   194,   193,   195,   200,   199,   202,
     201,   205,   204,   203,     0,     0,     0,     0,     0,     0,
       0,    35,    36,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     0,     0,    34,     0,     0,     0,     0,    37,    38,
       0,     0,     0,     0,    71,    30,   239,     0,     0,     0,
      26,    28,   110,    33,    31,    67,    52,    51,    53,    54,
      40,    55,    48,    56,    39,    58,    57,    59,    60,    62,
      61,    63,    49,    64,    65,    66,    41,    42,    43,    44,
      45,    46,    47,    50,    70,   241,   242,   243
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -252,  -252,    39,  -252,   -27,    -7,  -252,  -113,  -252,  -252,
     -50,   -34
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,   151,   152,   294,   295,    50,   237,
     238,   153
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     154,   255,     1,   155,    51,    52,   255,    54,   296,    53,
      55,   242,   305,   156,    56,   244,   245,    57,    58,   239,
     246,    65,   306,    66,    67,   251,   307,   252,   253,    59,
      60,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   241,   256,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     303,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   297,   298,   299,    61,    62,    63,
      64,   157,   158,   159,   160,   161,   162,   247,   248,   249,
     250,   308,   309,   254,   304,   310,   311,   312,   257,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,   240,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    38,    39,    40,    41,
      42,    43,    44,    45,    46,   243,   302,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   335,   336,
     337,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   334,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,     0,   234,   235,   236,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    68,     0,
       0,     0,     0,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    68,     0,     0,     0,     0,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   150
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      27,     3,     0,   183,   186,   187,     3,   185,   259,   191,
     188,    65,   203,   193,   192,   186,   187,   186,   187,    46,
     191,   184,   203,   186,   187,   184,   203,   186,   187,   189,
     190,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    47,     4,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,    62,    63,    64,   186,   187,   189,
     190,   186,   187,   186,   187,   186,   187,   186,   187,   186,
     187,   203,   203,   150,   257,   203,   203,   203,    65,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   259,   256,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,   201,   202,    61,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    61,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   242
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   264,   265,   266,
     271,   186,   187,   191,   185,   188,   192,   186,   187,   189,
     190,   186,   187,   189,   190,   184,   186,   187,    61,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     242,   267,   268,   274,   267,   183,   193,   186,   187,   186,
     187,   186,   187,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   260,   261,   262,   272,   273,   267,
       0,   265,    65,   259,   186,   187,   191,   186,   187,   186,
     187,   184,   186,   187,   268,     3,     4,    65,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   269,   270,   259,    62,    63,    64,
     242,   274,   273,   227,   270,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   267,   203,   203,   203
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   263,   264,   264,   264,   265,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   267,   267,   268,   268,
     269,   269,   269,   269,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     272,   272,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274
  };

  const unsigned char
  seclang_parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     1,     1,     1,     2,     2,     3,     1,     3,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       4,     3,     2,     2,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "PIPE", "NEW_LINE",
  "VARIABLE_ARGS_COMBINED_SIZE", "VARIABLE_ARGS_GET_NAMES",
  "\"Variable ARGS_NAMES\"", "VARIABLE_ARGS_POST_NAMES", "\"AUTH_TYPE\"",
  "\"FILES_COMBINED_SIZE\"", "\"FILES_TMPNAMES\"", "\"FULL_REQUEST\"",
  "\"FULL_REQUEST_LENGTH\"", "\"INBOUND_DATA_ERROR\"", "\"MATCHED_VAR\"",
  "\"MATCHED_VAR_NAME\"", "\"MULTIPART_CRLF_LF_LINES\"",
  "\"MULTIPART_DATA_AFTER\"", "\"MULTIPART_FILE_LIMIT_EXCEEDED\"",
  "\"MULTIPART_HEADER_FOLDING\"", "\"MULTIPART_INVALID_HEADER_FOLDING\"",
  "\"MULTIPART_INVALID_QUOTING\"", "\"MULTIPART_STRICT_ERROR\"",
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
  "\"URLENCODED_ERROR\"", "\"USERID\"", "\"SetVar\"",
  "SETVAR_OPERATION_EQUALS", "SETVAR_OPERATION_EQUALS_PLUS",
  "SETVAR_OPERATION_EQUALS_MINUS", "\"NOT\"", "\"Accuracy\"", "\"Allow\"",
  "\"Append\"", "\"AuditLog\"", "\"Block\"", "\"Capture\"", "\"Chain\"",
  "\"ACTION_CTL_AUDIT_ENGINE\"", "\"ACTION_CTL_AUDIT_LOG_PARTS\"",
  "\"ACTION_CTL_BDY_JSON\"", "\"ACTION_CTL_BDY_XML\"",
  "\"ACTION_CTL_FORCE_REQ_BODY_VAR\"",
  "\"ACTION_CTL_REQUEST_BODY_ACCESS\"", "\"ACTION_CTL_RULE_ENGINE\"",
  "\"ACTION_CTL_RULE_REMOVE_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_ID\"",
  "\"ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG\"", "\"Deny\"",
  "\"DeprecateVar\"", "\"Drop\"", "\"Exec\"", "\"ExpireVar\"", "\"Id\"",
  "\"InitCol\"", "\"Log\"", "\"LogData\"", "\"Maturity\"", "\"Msg\"",
  "\"MultiMatch\"", "\"NoAuditLog\"", "\"NoLog\"", "\"Pass\"", "\"Pause\"",
  "\"Phase\"", "\"Prepend\"", "\"Proxy\"", "\"Redirect\"", "\"Rev\"",
  "\"SanatiseArg\"", "\"SanatiseMatched\"", "\"SanatiseMatchedBytes\"",
  "\"SanatiseRequestHeader\"", "\"SanatiseResponseHeader\"", "\"SetEnv\"",
  "\"SetSrc\"", "\"SetSid\"", "\"SetUID\"", "\"Severity\"", "\"Skip\"",
  "\"SkipAfter\"", "\"Status\"", "\"Tag\"",
  "\"ACTION_TRANSFORMATION_CMD_LINE\"",
  "\"ACTION_TRANSFORMATION_COMPRESS_WHITESPACE\"",
  "\"ACTION_TRANSFORMATION_CSS_DECODE\"",
  "\"ACTION_TRANSFORMATION_HEX_ENCODE\"",
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
  "\"ACTION_TRANSFORMATION_TRIM\"", "\"ACTION_TRANSFORMATION_URL_DECODE\"",
  "\"ACTION_TRANSFORMATION_URL_DECODE_UNI\"",
  "\"ACTION_TRANSFORMATION_UTF8_TO_UNICODE\"", "\"Ver\"", "\"xmlns\"",
  "\"CONFIG_COMPONENT_SIG\"", "\"CONFIG_DIR_AUDIT_DIR\"",
  "\"CONFIG_DIR_AUDIT_DIR_MOD\"", "\"CONFIG_DIR_AUDIT_ENG\"",
  "\"CONFIG_DIR_AUDIT_FLE_MOD\"", "\"CONFIG_DIR_AUDIT_LOG\"",
  "\"CONFIG_DIR_AUDIT_LOG2\"", "\"CONFIG_DIR_AUDIT_LOG_P\"",
  "\"CONFIG_DIR_AUDIT_STS\"", "\"CONFIG_DIR_AUDIT_TPE\"",
  "\"CONFIG_DIR_DEBUG_LOG\"", "\"CONFIG_DIR_DEBUG_LVL\"",
  "\"CONFIG_DIR_GEO_DB\"", "\"CONFIG_DIR_PCRE_MATCH_LIMIT\"",
  "\"CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION\"", "\"CONFIG_DIR_REQ_BODY\"",
  "\"CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT\"",
  "\"CONFIG_DIR_REQ_BODY_LIMIT\"", "\"CONFIG_DIR_REQ_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT\"", "\"CONFIG_DIR_RES_BODY\"",
  "\"CONFIG_DIR_RES_BODY_LIMIT\"", "\"CONFIG_DIR_RES_BODY_LIMIT_ACTION\"",
  "\"CONFIG_DIR_RULE_ENG\"", "\"CONFIG_DIR_SEC_ACTION\"",
  "\"CONFIG_DIR_SEC_DEFAULT_ACTION\"", "\"CONFIG_DIR_SEC_MARKER\"",
  "\"CONFIG_DIR_UNICODE_MAP_FILE\"", "\"CONFIG_SEC_COLLECTION_TIMEOUT\"",
  "\"CONFIG_SEC_REMOTE_RULES_FAIL_ACTION\"",
  "\"CONFIG_SEC_RULE_REMOVE_BY_ID\"", "\"CONFIG_UPDLOAD_KEEP_FILES\"",
  "\"CONFIG_UPDLOAD_SAVE_TMP_FILES\"", "\"CONFIG_UPLOAD_DIR\"",
  "\"CONFIG_UPLOAD_FILE_LIMIT\"", "\"CONFIG_UPLOAD_FILE_MODE\"",
  "\"CONFIG_VALUE_ABORT\"", "\"CONFIG_VALUE_DETC\"",
  "\"CONFIG_VALUE_HTTPS\"", "\"CONFIG_VALUE_OFF\"", "\"CONFIG_VALUE_ON\"",
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
  "\"OPERATOR_RSUB\"", "\"OPERATOR_RX\"", "\"Operator Rx\"",
  "\"OPERATOR_STR_EQ\"", "\"OPERATOR_STR_MATCH\"",
  "\"OPERATOR_UNCONDITIONAL_MATCH\"", "\"OPERATOR_VALIDATE_BYTE_RANGE\"",
  "\"OPERATOR_VALIDATE_DTD\"", "\"OPERATOR_VALIDATE_HASH\"",
  "\"OPERATOR_VALIDATE_SCHEMA\"", "\"OPERATOR_VALIDATE_URL_ENCODING\"",
  "\"OPERATOR_VALIDATE_UTF8_ENCODING\"", "\"OPERATOR_VERIFY_CC\"",
  "\"OPERATOR_VERIFY_CPF\"", "\"OPERATOR_VERIFY_SSN\"",
  "\"OPERATOR_WITHIN\"", "\"OP_QUOTE\"", "\"QUOTATION_MARK\"",
  "\"RUN_TIME_VAR_BLD\"", "\"RUN_TIME_VAR_DUR\"", "\"RUN_TIME_VAR_ENV\"",
  "\"RUN_TIME_VAR_HSV\"", "\"RUN_TIME_VAR_REMOTE_USER\"",
  "\"RUN_TIME_VAR_RULE\"", "\"RUN_TIME_VAR_TIME\"",
  "\"RUN_TIME_VAR_TIME_DAY\"", "\"RUN_TIME_VAR_TIME_EPOCH\"",
  "\"RUN_TIME_VAR_TIME_HOUR\"", "\"RUN_TIME_VAR_TIME_MIN\"",
  "\"RUN_TIME_VAR_TIME_MON\"", "\"RUN_TIME_VAR_TIME_SEC\"",
  "\"RUN_TIME_VAR_TIME_WDAY\"", "\"RUN_TIME_VAR_TIME_YEAR\"",
  "\"RUN_TIME_VAR_XML\"", "\"VARIABLE\"", "\"VARIABLE_COL\"",
  "\"VARIABLE_STATUS\"", "\"VARIABLE_TX\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "var", "act", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   603,   603,   607,   608,   611,   616,   622,   628,   632,
     636,   642,   648,   654,   660,   666,   675,   679,   683,   689,
     693,   697,   702,   707,   712,   716,   723,   727,   734,   740,
     750,   759,   769,   778,   790,   794,   798,   802,   806,   810,
     815,   820,   824,   828,   833,   837,   841,   846,   851,   856,
     861,   865,   869,   873,   877,   881,   885,   889,   893,   897,
     901,   905,   909,   913,   917,   921,   925,   929,   933,   947,
     948,   972,   991,  1006,  1014,  1061,  1065,  1069,  1073,  1077,
    1081,  1085,  1089,  1093,  1097,  1111,  1123,  1143,  1163,  1168,
    1173,  1178,  1183,  1187,  1191,  1195,  1199,  1203,  1207,  1208,
    1209,  1220,  1224,  1228,  1229,  1230,  1231,  1232,  1233,  1234,
    1240,  1245,  1254,  1259,  1264,  1269,  1274,  1279,  1284,  1289,
    1294,  1299,  1304,  1309,  1314,  1319,  1324,  1329,  1334,  1339,
    1344,  1349,  1354,  1359,  1364,  1369,  1374,  1379,  1384,  1389,
    1394,  1399,  1404,  1409,  1414,  1419,  1424,  1429,  1434,  1439,
    1444,  1449,  1454,  1459,  1464,  1469,  1474,  1479,  1484,  1489,
    1494,  1499,  1504,  1509,  1514,  1519,  1524,  1529,  1546,  1563,
    1580,  1597,  1614,  1631,  1648,  1665,  1682,  1699,  1716,  1733,
    1750,  1767,  1784,  1801,  1818,  1824,  1844,  1848,  1852,  1856,
    1860,  1864,  1868,  1872,  1877,  1882,  1887,  1891,  1895,  1899,
    1904,  1909,  1913,  1917,  1922,  1927,  1932,  1936,  1940,  1944,
    1948,  1952,  1957,  1961,  1966,  1970,  1974,  1978,  1982,  1986,
    1990,  1994,  1998,  2002,  2006,  2010,  2014,  2018,  2022,  2026,
    2030,  2034,  2038,  2042,  2046,  2050,  2054,  2058,  2062,  2066,
    2070,  2074,  2078,  2082,  2086,  2090,  2094,  2098,  2102,  2106,
    2110,  2114,  2118,  2122,  2126,  2130,  2134,  2138,  2142,  2146,
    2150,  2154,  2158,  2162,  2166,  2170,  2174,  2178,  2182,  2186,
    2190,  2194,  2198,  2202,  2206,  2210,  2214,  2218
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
#line 4580 "seclang-parser.cc" // lalr1.cc:1167
#line 2224 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
