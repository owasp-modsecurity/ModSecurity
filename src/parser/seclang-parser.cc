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
#line 252 "seclang-parser.yy" // lalr1.cc:413

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
      case 7: // "Accuracy"
      case 8: // "Allow"
      case 9: // "Append"
      case 10: // "AuditLog"
      case 11: // "Block"
      case 12: // "Capture"
      case 13: // "Chain"
      case 14: // "ACTION_CTL_AUDIT_ENGINE"
      case 15: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 16: // "ACTION_CTL_BDY_JSON"
      case 17: // "ACTION_CTL_BDY_XML"
      case 18: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 19: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 20: // "ACTION_CTL_RULE_ENGINE"
      case 21: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 23: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 24: // "Deny"
      case 25: // "DeprecateVar"
      case 26: // "Drop"
      case 27: // "Exec"
      case 28: // "ExpireVar"
      case 29: // "Id"
      case 30: // "InitCol"
      case 31: // "Log"
      case 32: // "LogData"
      case 33: // "Maturity"
      case 34: // "Msg"
      case 35: // "MultiMatch"
      case 36: // "NoAuditLog"
      case 37: // "NoLog"
      case 38: // "Pass"
      case 39: // "Pause"
      case 40: // "Phase"
      case 41: // "Prepend"
      case 42: // "Proxy"
      case 43: // "Redirect"
      case 44: // "Rev"
      case 45: // "SanatiseArg"
      case 46: // "SanatiseMatched"
      case 47: // "SanatiseMatchedBytes"
      case 48: // "SanatiseRequestHeader"
      case 49: // "SanatiseResponseHeader"
      case 50: // "SetEnv"
      case 51: // "SetSrc"
      case 52: // "SetSid"
      case 53: // "SetUID"
      case 54: // "SetVar"
      case 55: // "Severity"
      case 56: // "Skip"
      case 57: // "SkipAfter"
      case 58: // "Status"
      case 59: // "Tag"
      case 60: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 61: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 62: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 63: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 64: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 65: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 66: // "ACTION_TRANSFORMATION_LENGTH"
      case 67: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 68: // "ACTION_TRANSFORMATION_MD5"
      case 69: // "ACTION_TRANSFORMATION_NONE"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 71: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 72: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 78: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 80: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 81: // "ACTION_TRANSFORMATION_SHA1"
      case 82: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 83: // "ACTION_TRANSFORMATION_TRIM"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 85: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 86: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 87: // "Ver"
      case 88: // "xmlns"
      case 89: // "CONFIG_COMPONENT_SIG"
      case 90: // "CONFIG_DIR_AUDIT_DIR"
      case 91: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 92: // "CONFIG_DIR_AUDIT_ENG"
      case 93: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 94: // "CONFIG_DIR_AUDIT_LOG"
      case 95: // "CONFIG_DIR_AUDIT_LOG2"
      case 96: // "CONFIG_DIR_AUDIT_LOG_P"
      case 97: // "CONFIG_DIR_AUDIT_STS"
      case 98: // "CONFIG_DIR_AUDIT_TPE"
      case 99: // "CONFIG_DIR_DEBUG_LOG"
      case 100: // "CONFIG_DIR_DEBUG_LVL"
      case 101: // "CONFIG_DIR_GEO_DB"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 103: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 104: // "CONFIG_DIR_REQ_BODY"
      case 105: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 107: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 108: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 109: // "CONFIG_DIR_RES_BODY"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 111: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 112: // "CONFIG_DIR_RULE_ENG"
      case 113: // "CONFIG_DIR_SEC_ACTION"
      case 114: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 115: // "CONFIG_DIR_SEC_MARKER"
      case 116: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 117: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 118: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 119: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 120: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 121: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 122: // "CONFIG_UPLOAD_DIR"
      case 123: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 124: // "CONFIG_UPLOAD_FILE_MODE"
      case 125: // "CONFIG_VALUE_ABORT"
      case 126: // "CONFIG_VALUE_DETC"
      case 127: // "CONFIG_VALUE_HTTPS"
      case 128: // "CONFIG_VALUE_OFF"
      case 129: // "CONFIG_VALUE_ON"
      case 130: // "CONFIG_VALUE_PARALLEL"
      case 131: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 132: // "CONFIG_VALUE_REJECT"
      case 133: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 134: // "CONFIG_VALUE_SERIAL"
      case 135: // "CONFIG_VALUE_WARN"
      case 136: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 137: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 138: // "CONGIG_DIR_SEC_ARG_SEP"
      case 139: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 140: // "CONGIG_DIR_SEC_DATA_DIR"
      case 141: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 142: // "CONGIG_DIR_SEC_TMP_DIR"
      case 143: // "DIRECTIVE"
      case 144: // "DIRECTIVE_SECRULESCRIPT"
      case 145: // "FREE_TEXT"
      case 146: // "NOT"
      case 147: // "OPERATOR"
      case 148: // "OPERATOR_BEGINS_WITH"
      case 149: // "OPERATOR_CONTAINS"
      case 150: // "OPERATOR_CONTAINS_WORD"
      case 151: // "OPERATOR_DETECT_SQLI"
      case 152: // "OPERATOR_DETECT_XSS"
      case 153: // "OPERATOR_ENDS_WITH"
      case 154: // "OPERATOR_EQ"
      case 155: // "OPERATOR_FUZZY_HASH"
      case 156: // "OPERATOR_GE"
      case 157: // "OPERATOR_GEOLOOKUP"
      case 158: // "OPERATOR_GSB_LOOKUP"
      case 159: // "OPERATOR_GT"
      case 160: // "OPERATOR_INSPECT_FILE"
      case 161: // "OPERATOR_IP_MATCH"
      case 162: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 163: // "OPERATOR_LE"
      case 164: // "OPERATOR_LT"
      case 165: // "OPERATOR_PM"
      case 166: // "OPERATOR_PM_FROM_FILE"
      case 167: // "OPERATOR_RBL"
      case 168: // "OPERATOR_RSUB"
      case 169: // "OPERATOR_RX"
      case 170: // "Operator Rx"
      case 171: // "OPERATOR_STR_EQ"
      case 172: // "OPERATOR_STR_MATCH"
      case 173: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 174: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 175: // "OPERATOR_VALIDATE_DTD"
      case 176: // "OPERATOR_VALIDATE_HASH"
      case 177: // "OPERATOR_VALIDATE_SCHEMA"
      case 178: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 179: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 180: // "OPERATOR_VERIFY_CC"
      case 181: // "OPERATOR_VERIFY_CPF"
      case 182: // "OPERATOR_VERIFY_SSN"
      case 183: // "OPERATOR_WITHIN"
      case 184: // "OP_QUOTE"
      case 185: // "QUOTATION_MARK"
      case 186: // "RUN_TIME_VAR_BLD"
      case 187: // "RUN_TIME_VAR_DUR"
      case 188: // "RUN_TIME_VAR_ENV"
      case 189: // "RUN_TIME_VAR_HSV"
      case 190: // "RUN_TIME_VAR_REMOTE_USER"
      case 191: // "RUN_TIME_VAR_RULE"
      case 192: // "RUN_TIME_VAR_TIME"
      case 193: // "RUN_TIME_VAR_TIME_DAY"
      case 194: // "RUN_TIME_VAR_TIME_EPOCH"
      case 195: // "RUN_TIME_VAR_TIME_HOUR"
      case 196: // "RUN_TIME_VAR_TIME_MIN"
      case 197: // "RUN_TIME_VAR_TIME_MON"
      case 198: // "RUN_TIME_VAR_TIME_SEC"
      case 199: // "RUN_TIME_VAR_TIME_WDAY"
      case 200: // "RUN_TIME_VAR_TIME_YEAR"
      case 201: // "RUN_TIME_VAR_XML"
      case 202: // "VARIABLE"
      case 203: // "VARIABLE_COL"
      case 204: // "VARIABLE_STATUS"
      case 205: // "VARIABLE_TX"
        value.move< std::string > (that.value);
        break;

      case 212: // op
      case 213: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 216: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 217: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 215: // variables
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 210: // actions
      case 211: // actions_may_quoted
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
      case 7: // "Accuracy"
      case 8: // "Allow"
      case 9: // "Append"
      case 10: // "AuditLog"
      case 11: // "Block"
      case 12: // "Capture"
      case 13: // "Chain"
      case 14: // "ACTION_CTL_AUDIT_ENGINE"
      case 15: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 16: // "ACTION_CTL_BDY_JSON"
      case 17: // "ACTION_CTL_BDY_XML"
      case 18: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 19: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 20: // "ACTION_CTL_RULE_ENGINE"
      case 21: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 23: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 24: // "Deny"
      case 25: // "DeprecateVar"
      case 26: // "Drop"
      case 27: // "Exec"
      case 28: // "ExpireVar"
      case 29: // "Id"
      case 30: // "InitCol"
      case 31: // "Log"
      case 32: // "LogData"
      case 33: // "Maturity"
      case 34: // "Msg"
      case 35: // "MultiMatch"
      case 36: // "NoAuditLog"
      case 37: // "NoLog"
      case 38: // "Pass"
      case 39: // "Pause"
      case 40: // "Phase"
      case 41: // "Prepend"
      case 42: // "Proxy"
      case 43: // "Redirect"
      case 44: // "Rev"
      case 45: // "SanatiseArg"
      case 46: // "SanatiseMatched"
      case 47: // "SanatiseMatchedBytes"
      case 48: // "SanatiseRequestHeader"
      case 49: // "SanatiseResponseHeader"
      case 50: // "SetEnv"
      case 51: // "SetSrc"
      case 52: // "SetSid"
      case 53: // "SetUID"
      case 54: // "SetVar"
      case 55: // "Severity"
      case 56: // "Skip"
      case 57: // "SkipAfter"
      case 58: // "Status"
      case 59: // "Tag"
      case 60: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 61: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 62: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 63: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 64: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 65: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 66: // "ACTION_TRANSFORMATION_LENGTH"
      case 67: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 68: // "ACTION_TRANSFORMATION_MD5"
      case 69: // "ACTION_TRANSFORMATION_NONE"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 71: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 72: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 78: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 80: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 81: // "ACTION_TRANSFORMATION_SHA1"
      case 82: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 83: // "ACTION_TRANSFORMATION_TRIM"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 85: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 86: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 87: // "Ver"
      case 88: // "xmlns"
      case 89: // "CONFIG_COMPONENT_SIG"
      case 90: // "CONFIG_DIR_AUDIT_DIR"
      case 91: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 92: // "CONFIG_DIR_AUDIT_ENG"
      case 93: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 94: // "CONFIG_DIR_AUDIT_LOG"
      case 95: // "CONFIG_DIR_AUDIT_LOG2"
      case 96: // "CONFIG_DIR_AUDIT_LOG_P"
      case 97: // "CONFIG_DIR_AUDIT_STS"
      case 98: // "CONFIG_DIR_AUDIT_TPE"
      case 99: // "CONFIG_DIR_DEBUG_LOG"
      case 100: // "CONFIG_DIR_DEBUG_LVL"
      case 101: // "CONFIG_DIR_GEO_DB"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 103: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 104: // "CONFIG_DIR_REQ_BODY"
      case 105: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 107: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 108: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 109: // "CONFIG_DIR_RES_BODY"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 111: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 112: // "CONFIG_DIR_RULE_ENG"
      case 113: // "CONFIG_DIR_SEC_ACTION"
      case 114: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 115: // "CONFIG_DIR_SEC_MARKER"
      case 116: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 117: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 118: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 119: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 120: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 121: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 122: // "CONFIG_UPLOAD_DIR"
      case 123: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 124: // "CONFIG_UPLOAD_FILE_MODE"
      case 125: // "CONFIG_VALUE_ABORT"
      case 126: // "CONFIG_VALUE_DETC"
      case 127: // "CONFIG_VALUE_HTTPS"
      case 128: // "CONFIG_VALUE_OFF"
      case 129: // "CONFIG_VALUE_ON"
      case 130: // "CONFIG_VALUE_PARALLEL"
      case 131: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 132: // "CONFIG_VALUE_REJECT"
      case 133: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 134: // "CONFIG_VALUE_SERIAL"
      case 135: // "CONFIG_VALUE_WARN"
      case 136: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 137: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 138: // "CONGIG_DIR_SEC_ARG_SEP"
      case 139: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 140: // "CONGIG_DIR_SEC_DATA_DIR"
      case 141: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 142: // "CONGIG_DIR_SEC_TMP_DIR"
      case 143: // "DIRECTIVE"
      case 144: // "DIRECTIVE_SECRULESCRIPT"
      case 145: // "FREE_TEXT"
      case 146: // "NOT"
      case 147: // "OPERATOR"
      case 148: // "OPERATOR_BEGINS_WITH"
      case 149: // "OPERATOR_CONTAINS"
      case 150: // "OPERATOR_CONTAINS_WORD"
      case 151: // "OPERATOR_DETECT_SQLI"
      case 152: // "OPERATOR_DETECT_XSS"
      case 153: // "OPERATOR_ENDS_WITH"
      case 154: // "OPERATOR_EQ"
      case 155: // "OPERATOR_FUZZY_HASH"
      case 156: // "OPERATOR_GE"
      case 157: // "OPERATOR_GEOLOOKUP"
      case 158: // "OPERATOR_GSB_LOOKUP"
      case 159: // "OPERATOR_GT"
      case 160: // "OPERATOR_INSPECT_FILE"
      case 161: // "OPERATOR_IP_MATCH"
      case 162: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 163: // "OPERATOR_LE"
      case 164: // "OPERATOR_LT"
      case 165: // "OPERATOR_PM"
      case 166: // "OPERATOR_PM_FROM_FILE"
      case 167: // "OPERATOR_RBL"
      case 168: // "OPERATOR_RSUB"
      case 169: // "OPERATOR_RX"
      case 170: // "Operator Rx"
      case 171: // "OPERATOR_STR_EQ"
      case 172: // "OPERATOR_STR_MATCH"
      case 173: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 174: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 175: // "OPERATOR_VALIDATE_DTD"
      case 176: // "OPERATOR_VALIDATE_HASH"
      case 177: // "OPERATOR_VALIDATE_SCHEMA"
      case 178: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 179: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 180: // "OPERATOR_VERIFY_CC"
      case 181: // "OPERATOR_VERIFY_CPF"
      case 182: // "OPERATOR_VERIFY_SSN"
      case 183: // "OPERATOR_WITHIN"
      case 184: // "OP_QUOTE"
      case 185: // "QUOTATION_MARK"
      case 186: // "RUN_TIME_VAR_BLD"
      case 187: // "RUN_TIME_VAR_DUR"
      case 188: // "RUN_TIME_VAR_ENV"
      case 189: // "RUN_TIME_VAR_HSV"
      case 190: // "RUN_TIME_VAR_REMOTE_USER"
      case 191: // "RUN_TIME_VAR_RULE"
      case 192: // "RUN_TIME_VAR_TIME"
      case 193: // "RUN_TIME_VAR_TIME_DAY"
      case 194: // "RUN_TIME_VAR_TIME_EPOCH"
      case 195: // "RUN_TIME_VAR_TIME_HOUR"
      case 196: // "RUN_TIME_VAR_TIME_MIN"
      case 197: // "RUN_TIME_VAR_TIME_MON"
      case 198: // "RUN_TIME_VAR_TIME_SEC"
      case 199: // "RUN_TIME_VAR_TIME_WDAY"
      case 200: // "RUN_TIME_VAR_TIME_YEAR"
      case 201: // "RUN_TIME_VAR_XML"
      case 202: // "VARIABLE"
      case 203: // "VARIABLE_COL"
      case 204: // "VARIABLE_STATUS"
      case 205: // "VARIABLE_TX"
        value.copy< std::string > (that.value);
        break;

      case 212: // op
      case 213: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 216: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 217: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 215: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 210: // actions
      case 211: // actions_may_quoted
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
    #line 245 "/home/zimmerle/core/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 865 "seclang-parser.cc" // lalr1.cc:741

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
      case 7: // "Accuracy"
      case 8: // "Allow"
      case 9: // "Append"
      case 10: // "AuditLog"
      case 11: // "Block"
      case 12: // "Capture"
      case 13: // "Chain"
      case 14: // "ACTION_CTL_AUDIT_ENGINE"
      case 15: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 16: // "ACTION_CTL_BDY_JSON"
      case 17: // "ACTION_CTL_BDY_XML"
      case 18: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 19: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 20: // "ACTION_CTL_RULE_ENGINE"
      case 21: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 23: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 24: // "Deny"
      case 25: // "DeprecateVar"
      case 26: // "Drop"
      case 27: // "Exec"
      case 28: // "ExpireVar"
      case 29: // "Id"
      case 30: // "InitCol"
      case 31: // "Log"
      case 32: // "LogData"
      case 33: // "Maturity"
      case 34: // "Msg"
      case 35: // "MultiMatch"
      case 36: // "NoAuditLog"
      case 37: // "NoLog"
      case 38: // "Pass"
      case 39: // "Pause"
      case 40: // "Phase"
      case 41: // "Prepend"
      case 42: // "Proxy"
      case 43: // "Redirect"
      case 44: // "Rev"
      case 45: // "SanatiseArg"
      case 46: // "SanatiseMatched"
      case 47: // "SanatiseMatchedBytes"
      case 48: // "SanatiseRequestHeader"
      case 49: // "SanatiseResponseHeader"
      case 50: // "SetEnv"
      case 51: // "SetSrc"
      case 52: // "SetSid"
      case 53: // "SetUID"
      case 54: // "SetVar"
      case 55: // "Severity"
      case 56: // "Skip"
      case 57: // "SkipAfter"
      case 58: // "Status"
      case 59: // "Tag"
      case 60: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 61: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 62: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 63: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 64: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 65: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 66: // "ACTION_TRANSFORMATION_LENGTH"
      case 67: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 68: // "ACTION_TRANSFORMATION_MD5"
      case 69: // "ACTION_TRANSFORMATION_NONE"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 71: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 72: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 78: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 80: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 81: // "ACTION_TRANSFORMATION_SHA1"
      case 82: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 83: // "ACTION_TRANSFORMATION_TRIM"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 85: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 86: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 87: // "Ver"
      case 88: // "xmlns"
      case 89: // "CONFIG_COMPONENT_SIG"
      case 90: // "CONFIG_DIR_AUDIT_DIR"
      case 91: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 92: // "CONFIG_DIR_AUDIT_ENG"
      case 93: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 94: // "CONFIG_DIR_AUDIT_LOG"
      case 95: // "CONFIG_DIR_AUDIT_LOG2"
      case 96: // "CONFIG_DIR_AUDIT_LOG_P"
      case 97: // "CONFIG_DIR_AUDIT_STS"
      case 98: // "CONFIG_DIR_AUDIT_TPE"
      case 99: // "CONFIG_DIR_DEBUG_LOG"
      case 100: // "CONFIG_DIR_DEBUG_LVL"
      case 101: // "CONFIG_DIR_GEO_DB"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 103: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 104: // "CONFIG_DIR_REQ_BODY"
      case 105: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 107: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 108: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 109: // "CONFIG_DIR_RES_BODY"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 111: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 112: // "CONFIG_DIR_RULE_ENG"
      case 113: // "CONFIG_DIR_SEC_ACTION"
      case 114: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 115: // "CONFIG_DIR_SEC_MARKER"
      case 116: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 117: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 118: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 119: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 120: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 121: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 122: // "CONFIG_UPLOAD_DIR"
      case 123: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 124: // "CONFIG_UPLOAD_FILE_MODE"
      case 125: // "CONFIG_VALUE_ABORT"
      case 126: // "CONFIG_VALUE_DETC"
      case 127: // "CONFIG_VALUE_HTTPS"
      case 128: // "CONFIG_VALUE_OFF"
      case 129: // "CONFIG_VALUE_ON"
      case 130: // "CONFIG_VALUE_PARALLEL"
      case 131: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 132: // "CONFIG_VALUE_REJECT"
      case 133: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 134: // "CONFIG_VALUE_SERIAL"
      case 135: // "CONFIG_VALUE_WARN"
      case 136: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 137: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 138: // "CONGIG_DIR_SEC_ARG_SEP"
      case 139: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 140: // "CONGIG_DIR_SEC_DATA_DIR"
      case 141: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 142: // "CONGIG_DIR_SEC_TMP_DIR"
      case 143: // "DIRECTIVE"
      case 144: // "DIRECTIVE_SECRULESCRIPT"
      case 145: // "FREE_TEXT"
      case 146: // "NOT"
      case 147: // "OPERATOR"
      case 148: // "OPERATOR_BEGINS_WITH"
      case 149: // "OPERATOR_CONTAINS"
      case 150: // "OPERATOR_CONTAINS_WORD"
      case 151: // "OPERATOR_DETECT_SQLI"
      case 152: // "OPERATOR_DETECT_XSS"
      case 153: // "OPERATOR_ENDS_WITH"
      case 154: // "OPERATOR_EQ"
      case 155: // "OPERATOR_FUZZY_HASH"
      case 156: // "OPERATOR_GE"
      case 157: // "OPERATOR_GEOLOOKUP"
      case 158: // "OPERATOR_GSB_LOOKUP"
      case 159: // "OPERATOR_GT"
      case 160: // "OPERATOR_INSPECT_FILE"
      case 161: // "OPERATOR_IP_MATCH"
      case 162: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 163: // "OPERATOR_LE"
      case 164: // "OPERATOR_LT"
      case 165: // "OPERATOR_PM"
      case 166: // "OPERATOR_PM_FROM_FILE"
      case 167: // "OPERATOR_RBL"
      case 168: // "OPERATOR_RSUB"
      case 169: // "OPERATOR_RX"
      case 170: // "Operator Rx"
      case 171: // "OPERATOR_STR_EQ"
      case 172: // "OPERATOR_STR_MATCH"
      case 173: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 174: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 175: // "OPERATOR_VALIDATE_DTD"
      case 176: // "OPERATOR_VALIDATE_HASH"
      case 177: // "OPERATOR_VALIDATE_SCHEMA"
      case 178: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 179: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 180: // "OPERATOR_VERIFY_CC"
      case 181: // "OPERATOR_VERIFY_CPF"
      case 182: // "OPERATOR_VERIFY_SSN"
      case 183: // "OPERATOR_WITHIN"
      case 184: // "OP_QUOTE"
      case 185: // "QUOTATION_MARK"
      case 186: // "RUN_TIME_VAR_BLD"
      case 187: // "RUN_TIME_VAR_DUR"
      case 188: // "RUN_TIME_VAR_ENV"
      case 189: // "RUN_TIME_VAR_HSV"
      case 190: // "RUN_TIME_VAR_REMOTE_USER"
      case 191: // "RUN_TIME_VAR_RULE"
      case 192: // "RUN_TIME_VAR_TIME"
      case 193: // "RUN_TIME_VAR_TIME_DAY"
      case 194: // "RUN_TIME_VAR_TIME_EPOCH"
      case 195: // "RUN_TIME_VAR_TIME_HOUR"
      case 196: // "RUN_TIME_VAR_TIME_MIN"
      case 197: // "RUN_TIME_VAR_TIME_MON"
      case 198: // "RUN_TIME_VAR_TIME_SEC"
      case 199: // "RUN_TIME_VAR_TIME_WDAY"
      case 200: // "RUN_TIME_VAR_TIME_YEAR"
      case 201: // "RUN_TIME_VAR_XML"
      case 202: // "VARIABLE"
      case 203: // "VARIABLE_COL"
      case 204: // "VARIABLE_STATUS"
      case 205: // "VARIABLE_TX"
        yylhs.value.build< std::string > ();
        break;

      case 212: // op
      case 213: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 216: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 217: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 215: // variables
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 210: // actions
      case 211: // actions_may_quoted
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
#line 490 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 503 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 509 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1217 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 515 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 519 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 523 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 529 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 535 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 541 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1265 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 547 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1273 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 553 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 562 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 566 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 570 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 576 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 580 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 584 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 589 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 594 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 599 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 603 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 610 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 614 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 621 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 627 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 637 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1417 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 646 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 656 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 665 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("!" + utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 677 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 681 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 685 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1481 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 689 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1489 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 693 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 697 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1506 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 702 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1515 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 707 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 711 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1531 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 715 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 720 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 724 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 728 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 733 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 738 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 743 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 748 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 756 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 760 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1624 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 764 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 768 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1640 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 772 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 776 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 780 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1664 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 784 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1672 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 788 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 792 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 796 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 800 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 804 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 808 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 812 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 816 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1736 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 820 "seclang-parser.yy" // lalr1.cc:859
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
#line 1751 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 835 "seclang-parser.yy" // lalr1.cc:859
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
#line 1779 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 859 "seclang-parser.yy" // lalr1.cc:859
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
#line 1802 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 878 "seclang-parser.yy" // lalr1.cc:859
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
#line 1821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 893 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 901 "seclang-parser.yy" // lalr1.cc:859
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
#line 1884 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 948 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1892 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 952 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1900 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 956 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1908 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 960 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1916 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 964 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1924 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 968 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1932 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 972 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1940 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 976 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1948 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 980 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1956 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 984 "seclang-parser.yy" // lalr1.cc:859
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
#line 1973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 998 "seclang-parser.yy" // lalr1.cc:859
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
#line 1989 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1010 "seclang-parser.yy" // lalr1.cc:859
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
#line 2012 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1030 "seclang-parser.yy" // lalr1.cc:859
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
#line 2035 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1050 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1055 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1060 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1065 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1070 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1074 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1078 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1082 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1086 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1090 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1096 "seclang-parser.yy" // lalr1.cc:859
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
#line 2134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1107 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1111 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1121 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1127 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1132 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2176 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1141 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2185 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1146 "seclang-parser.yy" // lalr1.cc:859
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
#line 2206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1163 "seclang-parser.yy" // lalr1.cc:859
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
#line 2227 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1180 "seclang-parser.yy" // lalr1.cc:859
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
#line 2248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1197 "seclang-parser.yy" // lalr1.cc:859
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
#line 2269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1214 "seclang-parser.yy" // lalr1.cc:859
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
#line 2290 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1231 "seclang-parser.yy" // lalr1.cc:859
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
#line 2311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1248 "seclang-parser.yy" // lalr1.cc:859
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
#line 2332 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1265 "seclang-parser.yy" // lalr1.cc:859
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
#line 2353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1282 "seclang-parser.yy" // lalr1.cc:859
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
#line 2374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1299 "seclang-parser.yy" // lalr1.cc:859
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
#line 2395 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1316 "seclang-parser.yy" // lalr1.cc:859
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
#line 2416 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1333 "seclang-parser.yy" // lalr1.cc:859
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
#line 2437 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1350 "seclang-parser.yy" // lalr1.cc:859
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
#line 2458 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1367 "seclang-parser.yy" // lalr1.cc:859
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
#line 2479 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1384 "seclang-parser.yy" // lalr1.cc:859
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
#line 2500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1401 "seclang-parser.yy" // lalr1.cc:859
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
#line 2521 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1418 "seclang-parser.yy" // lalr1.cc:859
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
#line 2542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1435 "seclang-parser.yy" // lalr1.cc:859
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
#line 2563 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1452 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        if (z == '&') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Count(new XML(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else if (z == '!') {
            name.erase(0, 1);
            std::unique_ptr<Variable> c(new Exclusion(new XML(name)));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        } else {
            std::unique_ptr<Variable> c(new XML(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
        }
      }
#line 2584 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1469 "seclang-parser.yy" // lalr1.cc:859
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
#line 2605 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1489 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 2613 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1493 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 2621 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1497 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 2629 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1501 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 2637 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1505 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 2645 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1509 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 2653 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1513 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 2661 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1517 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1522 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1527 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1532 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 2696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1536 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 2704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1540 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 2712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1544 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2721 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1549 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2730 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1554 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 2738 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1558 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 2746 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1562 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2755 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1567 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1572 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2773 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1577 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 2781 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1581 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 2789 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1585 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 2797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1589 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 2805 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1593 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 2813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1597 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 2822 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1602 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 2830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1606 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 2839 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1611 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 2847 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1615 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 2855 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1619 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 2863 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1623 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 2871 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1627 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 2879 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1631 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 2887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1635 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 2895 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1639 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 2903 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1643 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 2911 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1647 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 2919 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1651 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 2927 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1655 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 2935 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1659 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 2943 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1663 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 2951 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1667 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 2959 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1671 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 2967 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1675 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 2975 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1679 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 2983 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1683 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 2991 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1687 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 2999 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1691 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3007 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1695 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3015 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1699 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetRSC", yystack_[1].location);
      }
#line 3023 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1703 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3031 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1707 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3039 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1711 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(yystack_[0].value.as< std::string > ()));
      }
#line 3047 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1715 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1719 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3063 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1723 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1727 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1731 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1735 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1739 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1743 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1747 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1751 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3127 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1755 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3135 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1759 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 3143 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1763 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 3151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1767 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 3159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1771 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 3167 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1775 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 3175 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1779 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 3183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1783 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1787 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 3199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1791 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3207 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1795 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1799 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1803 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1807 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1811 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1815 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 3263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1823 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 3271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1827 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 3279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1831 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 3287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1835 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 3295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1839 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 3303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1843 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 3311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1847 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 3319 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 3323 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -141;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     161,  -141,  -141,  -141,  -141,  -120,  -141,  -141,  -141,  -141,
    -141,  -124,  -141,  -141,  -141,  -141,  -141,   -69,  -141,  -141,
     -66,  -141,   -61,  -141,    22,  -112,    64,    64,  -141,  -141,
    -141,  -123,  -141,    27,    29,  -141,  -141,  -141,    31,  -141,
    -141,  -141,  -141,  -141,  -141,    -5,    64,   286,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,   -71,  -141,  -141,  -141,    34,
      36,   -65,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
     424,  -141,     1,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,    65,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,    17,
     424,    -5,  -127,  -140,  -138,  -134,  -141,  -141,  -130,   -75,
      21,    23,  -141,    24,    25,    26,    28,    30,    32,    33,
      35,    56,    58,    59,    60,  -141,    61,    62,  -141,    63,
     142,   143,   144,  -141,  -141,   145,   146,   147,   148,    64,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,  -141
  };

  const unsigned char
  seclang_parser::yydefact_[] =
  {
       0,     2,    83,     7,     6,     0,    11,    14,    12,    13,
      15,     0,    86,    85,    87,    99,    98,     0,    90,    88,
       0,    89,     0,    91,     0,     0,     0,     0,    75,   108,
     109,     0,    84,     0,     0,    23,    21,    22,     0,   100,
     105,   106,   104,   107,   103,     0,     0,     0,     4,    69,
       5,     9,    10,     8,    18,    17,    16,    80,    79,    92,
      93,    82,    81,    94,    95,    78,    76,    77,   133,   134,
     135,   136,   137,   138,   139,     0,   143,   144,   145,     0,
       0,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   164,   163,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   198,   206,   212,   201,   210,   211,   216,   202,   200,
     205,   215,   214,   196,   195,   194,   219,   218,   209,   207,
     220,   208,   199,   197,   213,   204,   203,   217,   192,   193,
       0,    72,    27,    29,    74,    96,    97,    20,    19,    25,
      24,   101,   102,   112,   119,   117,   118,   120,   121,   132,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     113,   115,   114,   116,     0,   111,    73,     1,     3,   141,
     140,   142,   147,   146,   149,   148,   152,   151,   150,     0,
       0,     0,     0,     0,     0,     0,    35,    36,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,     0,     0,    34,     0,
       0,     0,     0,    37,    38,     0,     0,     0,     0,    71,
      30,    26,    28,   110,    33,    31,    67,    52,    51,    53,
      54,    40,    55,    48,    56,    39,    58,    57,    59,    60,
      62,    61,    63,    49,    64,    65,    66,    41,    42,    43,
      44,    45,    46,    47,    50,    70
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -141,  -141,   -29,  -141,   -27,   156,  -141,   -35,  -141,  -141,
     -25,   -28
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,   151,   152,   239,   240,    50,   184,
     185,   153
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     154,   163,   155,    54,   200,   246,    55,   247,    51,    52,
      56,   248,   156,    53,    65,   249,    66,    67,   188,   186,
     200,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   244,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   189,   190,    57,
      58,   196,   191,   197,   198,    59,    60,    61,    62,   201,
     250,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    63,    64,   157,   158,   159,   160,   161,
     162,     1,   192,   193,   194,   195,   251,   245,   252,   253,
     254,   255,   242,   256,     0,   257,   243,   258,   259,     0,
     260,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   261,   241,   262,   263,   264,   265,   266,   267,     0,
       0,   202,   275,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   150,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,   187,   268,   269,   270,
     271,   272,   273,   274,     0,     0,     0,    38,    39,    40,
      41,    42,    43,    44,    45,    46,   199,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      27,     6,   125,   127,     3,   145,   130,   145,   128,   129,
     134,   145,   135,   133,   126,   145,   128,   129,    47,    46,
       3,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   128,   129,   128,
     129,   126,   133,   128,   129,   131,   132,   128,   129,     4,
     145,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   131,   132,   128,   129,   128,   129,   128,
     129,     0,   128,   129,   128,   129,   145,   202,   145,   145,
     145,   145,   200,   145,    -1,   145,   201,   145,   145,    -1,
     145,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   145,   185,   145,   145,   145,   145,   145,   145,    -1,
      -1,   146,   239,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   185,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,     0,   145,   145,   145,
     145,   145,   145,   145,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88
  };

  const unsigned char
  seclang_parser::yystos_[] =
  {
       0,     0,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   207,   208,   209,
     214,   128,   129,   133,   127,   130,   134,   128,   129,   131,
     132,   128,   129,   131,   132,   126,   128,   129,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
     185,   210,   211,   217,   210,   125,   135,   128,   129,   128,
     129,   128,   129,     6,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   215,   216,   210,     0,   208,   128,
     129,   133,   128,   129,   128,   129,   126,   128,   129,   211,
       3,     4,   146,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   212,
     213,   185,   217,   216,   170,   213,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   210
  };

  const unsigned char
  seclang_parser::yyr1_[] =
  {
       0,   206,   207,   207,   207,   208,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   210,   210,   211,   211,
     212,   212,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     215,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217
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
       2,     2,     2,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "PIPE", "NEW_LINE",
  "\"Variable ARGS_NAMES\"", "\"Accuracy\"", "\"Allow\"", "\"Append\"",
  "\"AuditLog\"", "\"Block\"", "\"Capture\"", "\"Chain\"",
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
  "\"SetSrc\"", "\"SetSid\"", "\"SetUID\"", "\"SetVar\"", "\"Severity\"",
  "\"Skip\"", "\"SkipAfter\"", "\"Status\"", "\"Tag\"",
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
  "\"DIRECTIVE_SECRULESCRIPT\"", "\"FREE_TEXT\"", "\"NOT\"",
  "\"OPERATOR\"", "\"OPERATOR_BEGINS_WITH\"", "\"OPERATOR_CONTAINS\"",
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
       0,   489,   489,   493,   494,   497,   502,   508,   514,   518,
     522,   528,   534,   540,   546,   552,   561,   565,   569,   575,
     579,   583,   588,   593,   598,   602,   609,   613,   620,   626,
     636,   645,   655,   664,   676,   680,   684,   688,   692,   696,
     701,   706,   710,   714,   719,   723,   727,   732,   737,   742,
     747,   751,   755,   759,   763,   767,   771,   775,   779,   783,
     787,   791,   795,   799,   803,   807,   811,   815,   819,   833,
     834,   858,   877,   892,   900,   947,   951,   955,   959,   963,
     967,   971,   975,   979,   983,   997,  1009,  1029,  1049,  1054,
    1059,  1064,  1069,  1073,  1077,  1081,  1085,  1089,  1093,  1094,
    1095,  1106,  1110,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1126,  1131,  1140,  1145,  1162,  1179,  1196,  1213,  1230,  1247,
    1264,  1281,  1298,  1315,  1332,  1349,  1366,  1383,  1400,  1417,
    1434,  1451,  1468,  1488,  1492,  1496,  1500,  1504,  1508,  1512,
    1516,  1521,  1526,  1531,  1535,  1539,  1543,  1548,  1553,  1557,
    1561,  1566,  1571,  1576,  1580,  1584,  1588,  1592,  1596,  1601,
    1605,  1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,  1642,
    1646,  1650,  1654,  1658,  1662,  1666,  1670,  1674,  1678,  1682,
    1686,  1690,  1694,  1698,  1702,  1706,  1710,  1714,  1718,  1722,
    1726,  1730,  1734,  1738,  1742,  1746,  1750,  1754,  1758,  1762,
    1766,  1770,  1774,  1778,  1782,  1786,  1790,  1794,  1798,  1802,
    1806,  1810,  1814,  1818,  1822,  1826,  1830,  1834,  1838,  1842,
    1846
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
#line 4034 "seclang-parser.cc" // lalr1.cc:1167
#line 1852 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
