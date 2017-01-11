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
#line 251 "seclang-parser.yy" // lalr1.cc:413

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
      case 6: // "Accuracy"
      case 7: // "Allow"
      case 8: // "Append"
      case 9: // "AuditLog"
      case 10: // "Block"
      case 11: // "Capture"
      case 12: // "Chain"
      case 13: // "ACTION_CTL_AUDIT_ENGINE"
      case 14: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 15: // "ACTION_CTL_BDY_JSON"
      case 16: // "ACTION_CTL_BDY_XML"
      case 17: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 18: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 19: // "ACTION_CTL_RULE_ENGINE"
      case 20: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 21: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 23: // "Deny"
      case 24: // "DeprecateVar"
      case 25: // "Drop"
      case 26: // "Exec"
      case 27: // "ExpireVar"
      case 28: // "Id"
      case 29: // "InitCol"
      case 30: // "Log"
      case 31: // "LogData"
      case 32: // "Maturity"
      case 33: // "Msg"
      case 34: // "MultiMatch"
      case 35: // "NoAuditLog"
      case 36: // "NoLog"
      case 37: // "Pass"
      case 38: // "Pause"
      case 39: // "Phase"
      case 40: // "Prepend"
      case 41: // "Proxy"
      case 42: // "Redirect"
      case 43: // "Rev"
      case 44: // "SanatiseArg"
      case 45: // "SanatiseMatched"
      case 46: // "SanatiseMatchedBytes"
      case 47: // "SanatiseRequestHeader"
      case 48: // "SanatiseResponseHeader"
      case 49: // "SetEnv"
      case 50: // "SetSrc"
      case 51: // "SetSid"
      case 52: // "SetUID"
      case 53: // "SetVar"
      case 54: // "Severity"
      case 55: // "Skip"
      case 56: // "SkipAfter"
      case 57: // "Status"
      case 58: // "Tag"
      case 59: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 60: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 61: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 62: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 63: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 64: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 65: // "ACTION_TRANSFORMATION_LENGTH"
      case 66: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 67: // "ACTION_TRANSFORMATION_MD5"
      case 68: // "ACTION_TRANSFORMATION_NONE"
      case 69: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 71: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 72: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 78: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 80: // "ACTION_TRANSFORMATION_SHA1"
      case 81: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 82: // "ACTION_TRANSFORMATION_TRIM"
      case 83: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 85: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 86: // "Ver"
      case 87: // "xmlns"
      case 88: // "CONFIG_COMPONENT_SIG"
      case 89: // "CONFIG_DIR_AUDIT_DIR"
      case 90: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 91: // "CONFIG_DIR_AUDIT_ENG"
      case 92: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 93: // "CONFIG_DIR_AUDIT_LOG"
      case 94: // "CONFIG_DIR_AUDIT_LOG2"
      case 95: // "CONFIG_DIR_AUDIT_LOG_P"
      case 96: // "CONFIG_DIR_AUDIT_STS"
      case 97: // "CONFIG_DIR_AUDIT_TPE"
      case 98: // "CONFIG_DIR_DEBUG_LOG"
      case 99: // "CONFIG_DIR_DEBUG_LVL"
      case 100: // "CONFIG_DIR_GEO_DB"
      case 101: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 103: // "CONFIG_DIR_REQ_BODY"
      case 104: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 105: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 107: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 108: // "CONFIG_DIR_RES_BODY"
      case 109: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 111: // "CONFIG_DIR_RULE_ENG"
      case 112: // "CONFIG_DIR_SEC_ACTION"
      case 113: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 114: // "CONFIG_DIR_SEC_MARKER"
      case 115: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 116: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 117: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 118: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 119: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 120: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 121: // "CONFIG_UPLOAD_DIR"
      case 122: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 123: // "CONFIG_UPLOAD_FILE_MODE"
      case 124: // "CONFIG_VALUE_ABORT"
      case 125: // "CONFIG_VALUE_DETC"
      case 126: // "CONFIG_VALUE_HTTPS"
      case 127: // "CONFIG_VALUE_OFF"
      case 128: // "CONFIG_VALUE_ON"
      case 129: // "CONFIG_VALUE_PARALLEL"
      case 130: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 131: // "CONFIG_VALUE_REJECT"
      case 132: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 133: // "CONFIG_VALUE_SERIAL"
      case 134: // "CONFIG_VALUE_WARN"
      case 135: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 136: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 137: // "CONGIG_DIR_SEC_ARG_SEP"
      case 138: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 139: // "CONGIG_DIR_SEC_DATA_DIR"
      case 140: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 141: // "CONGIG_DIR_SEC_TMP_DIR"
      case 142: // "DIRECTIVE"
      case 143: // "DIRECTIVE_SECRULESCRIPT"
      case 144: // "FREE_TEXT"
      case 145: // "NOT"
      case 146: // "OPERATOR"
      case 147: // "OPERATOR_BEGINS_WITH"
      case 148: // "OPERATOR_CONTAINS"
      case 149: // "OPERATOR_CONTAINS_WORD"
      case 150: // "OPERATOR_DETECT_SQLI"
      case 151: // "OPERATOR_DETECT_XSS"
      case 152: // "OPERATOR_ENDS_WITH"
      case 153: // "OPERATOR_EQ"
      case 154: // "OPERATOR_FUZZY_HASH"
      case 155: // "OPERATOR_GE"
      case 156: // "OPERATOR_GEOLOOKUP"
      case 157: // "OPERATOR_GSB_LOOKUP"
      case 158: // "OPERATOR_GT"
      case 159: // "OPERATOR_INSPECT_FILE"
      case 160: // "OPERATOR_IP_MATCH"
      case 161: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 162: // "OPERATOR_LE"
      case 163: // "OPERATOR_LT"
      case 164: // "OPERATOR_PM"
      case 165: // "OPERATOR_PM_FROM_FILE"
      case 166: // "OPERATOR_RBL"
      case 167: // "OPERATOR_RSUB"
      case 168: // "OPERATOR_RX"
      case 169: // "Operator Rx"
      case 170: // "OPERATOR_STR_EQ"
      case 171: // "OPERATOR_STR_MATCH"
      case 172: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 173: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 174: // "OPERATOR_VALIDATE_DTD"
      case 175: // "OPERATOR_VALIDATE_HASH"
      case 176: // "OPERATOR_VALIDATE_SCHEMA"
      case 177: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 178: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 179: // "OPERATOR_VERIFY_CC"
      case 180: // "OPERATOR_VERIFY_CPF"
      case 181: // "OPERATOR_VERIFY_SSN"
      case 182: // "OPERATOR_WITHIN"
      case 183: // "OP_QUOTE"
      case 184: // "QUOTATION_MARK"
      case 185: // "RUN_TIME_VAR_BLD"
      case 186: // "RUN_TIME_VAR_DUR"
      case 187: // "RUN_TIME_VAR_ENV"
      case 188: // "RUN_TIME_VAR_HSV"
      case 189: // "RUN_TIME_VAR_REMOTE_USER"
      case 190: // "RUN_TIME_VAR_RULE"
      case 191: // "RUN_TIME_VAR_TIME"
      case 192: // "RUN_TIME_VAR_TIME_DAY"
      case 193: // "RUN_TIME_VAR_TIME_EPOCH"
      case 194: // "RUN_TIME_VAR_TIME_HOUR"
      case 195: // "RUN_TIME_VAR_TIME_MIN"
      case 196: // "RUN_TIME_VAR_TIME_MON"
      case 197: // "RUN_TIME_VAR_TIME_SEC"
      case 198: // "RUN_TIME_VAR_TIME_WDAY"
      case 199: // "RUN_TIME_VAR_TIME_YEAR"
      case 200: // "RUN_TIME_VAR_XML"
      case 201: // "VARIABLE"
      case 202: // "VARIABLE_COL"
      case 203: // "VARIABLE_STATUS"
      case 204: // "VARIABLE_TX"
        value.move< std::string > (that.value);
        break;

      case 211: // op
      case 212: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 215: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 216: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 214: // variables
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 209: // actions
      case 210: // actions_may_quoted
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
      case 6: // "Accuracy"
      case 7: // "Allow"
      case 8: // "Append"
      case 9: // "AuditLog"
      case 10: // "Block"
      case 11: // "Capture"
      case 12: // "Chain"
      case 13: // "ACTION_CTL_AUDIT_ENGINE"
      case 14: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 15: // "ACTION_CTL_BDY_JSON"
      case 16: // "ACTION_CTL_BDY_XML"
      case 17: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 18: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 19: // "ACTION_CTL_RULE_ENGINE"
      case 20: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 21: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 23: // "Deny"
      case 24: // "DeprecateVar"
      case 25: // "Drop"
      case 26: // "Exec"
      case 27: // "ExpireVar"
      case 28: // "Id"
      case 29: // "InitCol"
      case 30: // "Log"
      case 31: // "LogData"
      case 32: // "Maturity"
      case 33: // "Msg"
      case 34: // "MultiMatch"
      case 35: // "NoAuditLog"
      case 36: // "NoLog"
      case 37: // "Pass"
      case 38: // "Pause"
      case 39: // "Phase"
      case 40: // "Prepend"
      case 41: // "Proxy"
      case 42: // "Redirect"
      case 43: // "Rev"
      case 44: // "SanatiseArg"
      case 45: // "SanatiseMatched"
      case 46: // "SanatiseMatchedBytes"
      case 47: // "SanatiseRequestHeader"
      case 48: // "SanatiseResponseHeader"
      case 49: // "SetEnv"
      case 50: // "SetSrc"
      case 51: // "SetSid"
      case 52: // "SetUID"
      case 53: // "SetVar"
      case 54: // "Severity"
      case 55: // "Skip"
      case 56: // "SkipAfter"
      case 57: // "Status"
      case 58: // "Tag"
      case 59: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 60: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 61: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 62: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 63: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 64: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 65: // "ACTION_TRANSFORMATION_LENGTH"
      case 66: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 67: // "ACTION_TRANSFORMATION_MD5"
      case 68: // "ACTION_TRANSFORMATION_NONE"
      case 69: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 71: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 72: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 78: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 80: // "ACTION_TRANSFORMATION_SHA1"
      case 81: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 82: // "ACTION_TRANSFORMATION_TRIM"
      case 83: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 85: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 86: // "Ver"
      case 87: // "xmlns"
      case 88: // "CONFIG_COMPONENT_SIG"
      case 89: // "CONFIG_DIR_AUDIT_DIR"
      case 90: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 91: // "CONFIG_DIR_AUDIT_ENG"
      case 92: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 93: // "CONFIG_DIR_AUDIT_LOG"
      case 94: // "CONFIG_DIR_AUDIT_LOG2"
      case 95: // "CONFIG_DIR_AUDIT_LOG_P"
      case 96: // "CONFIG_DIR_AUDIT_STS"
      case 97: // "CONFIG_DIR_AUDIT_TPE"
      case 98: // "CONFIG_DIR_DEBUG_LOG"
      case 99: // "CONFIG_DIR_DEBUG_LVL"
      case 100: // "CONFIG_DIR_GEO_DB"
      case 101: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 103: // "CONFIG_DIR_REQ_BODY"
      case 104: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 105: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 107: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 108: // "CONFIG_DIR_RES_BODY"
      case 109: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 111: // "CONFIG_DIR_RULE_ENG"
      case 112: // "CONFIG_DIR_SEC_ACTION"
      case 113: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 114: // "CONFIG_DIR_SEC_MARKER"
      case 115: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 116: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 117: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 118: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 119: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 120: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 121: // "CONFIG_UPLOAD_DIR"
      case 122: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 123: // "CONFIG_UPLOAD_FILE_MODE"
      case 124: // "CONFIG_VALUE_ABORT"
      case 125: // "CONFIG_VALUE_DETC"
      case 126: // "CONFIG_VALUE_HTTPS"
      case 127: // "CONFIG_VALUE_OFF"
      case 128: // "CONFIG_VALUE_ON"
      case 129: // "CONFIG_VALUE_PARALLEL"
      case 130: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 131: // "CONFIG_VALUE_REJECT"
      case 132: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 133: // "CONFIG_VALUE_SERIAL"
      case 134: // "CONFIG_VALUE_WARN"
      case 135: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 136: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 137: // "CONGIG_DIR_SEC_ARG_SEP"
      case 138: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 139: // "CONGIG_DIR_SEC_DATA_DIR"
      case 140: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 141: // "CONGIG_DIR_SEC_TMP_DIR"
      case 142: // "DIRECTIVE"
      case 143: // "DIRECTIVE_SECRULESCRIPT"
      case 144: // "FREE_TEXT"
      case 145: // "NOT"
      case 146: // "OPERATOR"
      case 147: // "OPERATOR_BEGINS_WITH"
      case 148: // "OPERATOR_CONTAINS"
      case 149: // "OPERATOR_CONTAINS_WORD"
      case 150: // "OPERATOR_DETECT_SQLI"
      case 151: // "OPERATOR_DETECT_XSS"
      case 152: // "OPERATOR_ENDS_WITH"
      case 153: // "OPERATOR_EQ"
      case 154: // "OPERATOR_FUZZY_HASH"
      case 155: // "OPERATOR_GE"
      case 156: // "OPERATOR_GEOLOOKUP"
      case 157: // "OPERATOR_GSB_LOOKUP"
      case 158: // "OPERATOR_GT"
      case 159: // "OPERATOR_INSPECT_FILE"
      case 160: // "OPERATOR_IP_MATCH"
      case 161: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 162: // "OPERATOR_LE"
      case 163: // "OPERATOR_LT"
      case 164: // "OPERATOR_PM"
      case 165: // "OPERATOR_PM_FROM_FILE"
      case 166: // "OPERATOR_RBL"
      case 167: // "OPERATOR_RSUB"
      case 168: // "OPERATOR_RX"
      case 169: // "Operator Rx"
      case 170: // "OPERATOR_STR_EQ"
      case 171: // "OPERATOR_STR_MATCH"
      case 172: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 173: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 174: // "OPERATOR_VALIDATE_DTD"
      case 175: // "OPERATOR_VALIDATE_HASH"
      case 176: // "OPERATOR_VALIDATE_SCHEMA"
      case 177: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 178: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 179: // "OPERATOR_VERIFY_CC"
      case 180: // "OPERATOR_VERIFY_CPF"
      case 181: // "OPERATOR_VERIFY_SSN"
      case 182: // "OPERATOR_WITHIN"
      case 183: // "OP_QUOTE"
      case 184: // "QUOTATION_MARK"
      case 185: // "RUN_TIME_VAR_BLD"
      case 186: // "RUN_TIME_VAR_DUR"
      case 187: // "RUN_TIME_VAR_ENV"
      case 188: // "RUN_TIME_VAR_HSV"
      case 189: // "RUN_TIME_VAR_REMOTE_USER"
      case 190: // "RUN_TIME_VAR_RULE"
      case 191: // "RUN_TIME_VAR_TIME"
      case 192: // "RUN_TIME_VAR_TIME_DAY"
      case 193: // "RUN_TIME_VAR_TIME_EPOCH"
      case 194: // "RUN_TIME_VAR_TIME_HOUR"
      case 195: // "RUN_TIME_VAR_TIME_MIN"
      case 196: // "RUN_TIME_VAR_TIME_MON"
      case 197: // "RUN_TIME_VAR_TIME_SEC"
      case 198: // "RUN_TIME_VAR_TIME_WDAY"
      case 199: // "RUN_TIME_VAR_TIME_YEAR"
      case 200: // "RUN_TIME_VAR_XML"
      case 201: // "VARIABLE"
      case 202: // "VARIABLE_COL"
      case 203: // "VARIABLE_STATUS"
      case 204: // "VARIABLE_TX"
        value.copy< std::string > (that.value);
        break;

      case 211: // op
      case 212: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 215: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 216: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 214: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 209: // actions
      case 210: // actions_may_quoted
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
    #line 244 "/home/zimmerle/core/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
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
      case 6: // "Accuracy"
      case 7: // "Allow"
      case 8: // "Append"
      case 9: // "AuditLog"
      case 10: // "Block"
      case 11: // "Capture"
      case 12: // "Chain"
      case 13: // "ACTION_CTL_AUDIT_ENGINE"
      case 14: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 15: // "ACTION_CTL_BDY_JSON"
      case 16: // "ACTION_CTL_BDY_XML"
      case 17: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 18: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 19: // "ACTION_CTL_RULE_ENGINE"
      case 20: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 21: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 22: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 23: // "Deny"
      case 24: // "DeprecateVar"
      case 25: // "Drop"
      case 26: // "Exec"
      case 27: // "ExpireVar"
      case 28: // "Id"
      case 29: // "InitCol"
      case 30: // "Log"
      case 31: // "LogData"
      case 32: // "Maturity"
      case 33: // "Msg"
      case 34: // "MultiMatch"
      case 35: // "NoAuditLog"
      case 36: // "NoLog"
      case 37: // "Pass"
      case 38: // "Pause"
      case 39: // "Phase"
      case 40: // "Prepend"
      case 41: // "Proxy"
      case 42: // "Redirect"
      case 43: // "Rev"
      case 44: // "SanatiseArg"
      case 45: // "SanatiseMatched"
      case 46: // "SanatiseMatchedBytes"
      case 47: // "SanatiseRequestHeader"
      case 48: // "SanatiseResponseHeader"
      case 49: // "SetEnv"
      case 50: // "SetSrc"
      case 51: // "SetSid"
      case 52: // "SetUID"
      case 53: // "SetVar"
      case 54: // "Severity"
      case 55: // "Skip"
      case 56: // "SkipAfter"
      case 57: // "Status"
      case 58: // "Tag"
      case 59: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 60: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 61: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 62: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 63: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 64: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 65: // "ACTION_TRANSFORMATION_LENGTH"
      case 66: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 67: // "ACTION_TRANSFORMATION_MD5"
      case 68: // "ACTION_TRANSFORMATION_NONE"
      case 69: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 70: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 71: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 72: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 73: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 74: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 75: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 76: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 77: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 78: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 79: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 80: // "ACTION_TRANSFORMATION_SHA1"
      case 81: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 82: // "ACTION_TRANSFORMATION_TRIM"
      case 83: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 84: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 85: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 86: // "Ver"
      case 87: // "xmlns"
      case 88: // "CONFIG_COMPONENT_SIG"
      case 89: // "CONFIG_DIR_AUDIT_DIR"
      case 90: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 91: // "CONFIG_DIR_AUDIT_ENG"
      case 92: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 93: // "CONFIG_DIR_AUDIT_LOG"
      case 94: // "CONFIG_DIR_AUDIT_LOG2"
      case 95: // "CONFIG_DIR_AUDIT_LOG_P"
      case 96: // "CONFIG_DIR_AUDIT_STS"
      case 97: // "CONFIG_DIR_AUDIT_TPE"
      case 98: // "CONFIG_DIR_DEBUG_LOG"
      case 99: // "CONFIG_DIR_DEBUG_LVL"
      case 100: // "CONFIG_DIR_GEO_DB"
      case 101: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 102: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 103: // "CONFIG_DIR_REQ_BODY"
      case 104: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 105: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 106: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 107: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 108: // "CONFIG_DIR_RES_BODY"
      case 109: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 110: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 111: // "CONFIG_DIR_RULE_ENG"
      case 112: // "CONFIG_DIR_SEC_ACTION"
      case 113: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 114: // "CONFIG_DIR_SEC_MARKER"
      case 115: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 116: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 117: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 118: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 119: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 120: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 121: // "CONFIG_UPLOAD_DIR"
      case 122: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 123: // "CONFIG_UPLOAD_FILE_MODE"
      case 124: // "CONFIG_VALUE_ABORT"
      case 125: // "CONFIG_VALUE_DETC"
      case 126: // "CONFIG_VALUE_HTTPS"
      case 127: // "CONFIG_VALUE_OFF"
      case 128: // "CONFIG_VALUE_ON"
      case 129: // "CONFIG_VALUE_PARALLEL"
      case 130: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 131: // "CONFIG_VALUE_REJECT"
      case 132: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 133: // "CONFIG_VALUE_SERIAL"
      case 134: // "CONFIG_VALUE_WARN"
      case 135: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 136: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 137: // "CONGIG_DIR_SEC_ARG_SEP"
      case 138: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 139: // "CONGIG_DIR_SEC_DATA_DIR"
      case 140: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 141: // "CONGIG_DIR_SEC_TMP_DIR"
      case 142: // "DIRECTIVE"
      case 143: // "DIRECTIVE_SECRULESCRIPT"
      case 144: // "FREE_TEXT"
      case 145: // "NOT"
      case 146: // "OPERATOR"
      case 147: // "OPERATOR_BEGINS_WITH"
      case 148: // "OPERATOR_CONTAINS"
      case 149: // "OPERATOR_CONTAINS_WORD"
      case 150: // "OPERATOR_DETECT_SQLI"
      case 151: // "OPERATOR_DETECT_XSS"
      case 152: // "OPERATOR_ENDS_WITH"
      case 153: // "OPERATOR_EQ"
      case 154: // "OPERATOR_FUZZY_HASH"
      case 155: // "OPERATOR_GE"
      case 156: // "OPERATOR_GEOLOOKUP"
      case 157: // "OPERATOR_GSB_LOOKUP"
      case 158: // "OPERATOR_GT"
      case 159: // "OPERATOR_INSPECT_FILE"
      case 160: // "OPERATOR_IP_MATCH"
      case 161: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 162: // "OPERATOR_LE"
      case 163: // "OPERATOR_LT"
      case 164: // "OPERATOR_PM"
      case 165: // "OPERATOR_PM_FROM_FILE"
      case 166: // "OPERATOR_RBL"
      case 167: // "OPERATOR_RSUB"
      case 168: // "OPERATOR_RX"
      case 169: // "Operator Rx"
      case 170: // "OPERATOR_STR_EQ"
      case 171: // "OPERATOR_STR_MATCH"
      case 172: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 173: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 174: // "OPERATOR_VALIDATE_DTD"
      case 175: // "OPERATOR_VALIDATE_HASH"
      case 176: // "OPERATOR_VALIDATE_SCHEMA"
      case 177: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 178: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 179: // "OPERATOR_VERIFY_CC"
      case 180: // "OPERATOR_VERIFY_CPF"
      case 181: // "OPERATOR_VERIFY_SSN"
      case 182: // "OPERATOR_WITHIN"
      case 183: // "OP_QUOTE"
      case 184: // "QUOTATION_MARK"
      case 185: // "RUN_TIME_VAR_BLD"
      case 186: // "RUN_TIME_VAR_DUR"
      case 187: // "RUN_TIME_VAR_ENV"
      case 188: // "RUN_TIME_VAR_HSV"
      case 189: // "RUN_TIME_VAR_REMOTE_USER"
      case 190: // "RUN_TIME_VAR_RULE"
      case 191: // "RUN_TIME_VAR_TIME"
      case 192: // "RUN_TIME_VAR_TIME_DAY"
      case 193: // "RUN_TIME_VAR_TIME_EPOCH"
      case 194: // "RUN_TIME_VAR_TIME_HOUR"
      case 195: // "RUN_TIME_VAR_TIME_MIN"
      case 196: // "RUN_TIME_VAR_TIME_MON"
      case 197: // "RUN_TIME_VAR_TIME_SEC"
      case 198: // "RUN_TIME_VAR_TIME_WDAY"
      case 199: // "RUN_TIME_VAR_TIME_YEAR"
      case 200: // "RUN_TIME_VAR_XML"
      case 201: // "VARIABLE"
      case 202: // "VARIABLE_COL"
      case 203: // "VARIABLE_STATUS"
      case 204: // "VARIABLE_TX"
        yylhs.value.build< std::string > ();
        break;

      case 211: // op
      case 212: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 215: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 216: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 214: // variables
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 209: // actions
      case 210: // actions_may_quoted
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
#line 488 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 501 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 507 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1217 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 513 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 517 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 521 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 527 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 533 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 539 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1265 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 545 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1273 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 551 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 560 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 564 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 568 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 574 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 578 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 582 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 587 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 592 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 597 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 601 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 608 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 612 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 619 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 625 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 635 "seclang-parser.yy" // lalr1.cc:859
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
#line 644 "seclang-parser.yy" // lalr1.cc:859
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
#line 654 "seclang-parser.yy" // lalr1.cc:859
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
#line 663 "seclang-parser.yy" // lalr1.cc:859
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
#line 675 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 679 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 683 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1481 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 687 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1489 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 691 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 695 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1506 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 700 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1515 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 705 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 709 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1531 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 713 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 718 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 722 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 726 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 731 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 736 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 741 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 746 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 750 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 754 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1624 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 762 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 766 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1640 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 770 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 774 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 778 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1664 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 782 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1672 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 786 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 790 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 794 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 798 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 802 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 806 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 810 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 814 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1736 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 818 "seclang-parser.yy" // lalr1.cc:859
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
#line 833 "seclang-parser.yy" // lalr1.cc:859
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
#line 857 "seclang-parser.yy" // lalr1.cc:859
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
#line 876 "seclang-parser.yy" // lalr1.cc:859
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
#line 891 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 899 "seclang-parser.yy" // lalr1.cc:859
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
#line 946 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1892 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 950 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1900 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 954 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1908 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1916 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 962 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1924 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 966 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1932 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1940 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 974 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1948 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 978 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1956 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 982 "seclang-parser.yy" // lalr1.cc:859
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
#line 996 "seclang-parser.yy" // lalr1.cc:859
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
#line 1008 "seclang-parser.yy" // lalr1.cc:859
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
#line 1028 "seclang-parser.yy" // lalr1.cc:859
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
#line 1048 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1053 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1058 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1063 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1068 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1072 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1076 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1080 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1084 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1088 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1094 "seclang-parser.yy" // lalr1.cc:859
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
#line 1105 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1109 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1119 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1125 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1130 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2176 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1139 "seclang-parser.yy" // lalr1.cc:859
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
#line 2197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1156 "seclang-parser.yy" // lalr1.cc:859
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
#line 2218 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1173 "seclang-parser.yy" // lalr1.cc:859
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
#line 2239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1190 "seclang-parser.yy" // lalr1.cc:859
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
#line 2260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1207 "seclang-parser.yy" // lalr1.cc:859
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
#line 2281 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1224 "seclang-parser.yy" // lalr1.cc:859
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
#line 2302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1241 "seclang-parser.yy" // lalr1.cc:859
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
#line 2323 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1258 "seclang-parser.yy" // lalr1.cc:859
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
#line 2344 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1275 "seclang-parser.yy" // lalr1.cc:859
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
#line 2365 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1292 "seclang-parser.yy" // lalr1.cc:859
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
#line 2386 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1309 "seclang-parser.yy" // lalr1.cc:859
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
#line 2407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1326 "seclang-parser.yy" // lalr1.cc:859
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
#line 2428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1343 "seclang-parser.yy" // lalr1.cc:859
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
#line 2449 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1360 "seclang-parser.yy" // lalr1.cc:859
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
#line 2470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1377 "seclang-parser.yy" // lalr1.cc:859
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
#line 2491 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1394 "seclang-parser.yy" // lalr1.cc:859
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
#line 2512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1411 "seclang-parser.yy" // lalr1.cc:859
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
#line 2533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1428 "seclang-parser.yy" // lalr1.cc:859
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
#line 2554 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1445 "seclang-parser.yy" // lalr1.cc:859
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
#line 2575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1462 "seclang-parser.yy" // lalr1.cc:859
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
#line 2596 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1482 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 2604 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1486 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 2612 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1490 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 2620 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1494 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 2628 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1498 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 2636 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1502 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 2644 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1506 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 2652 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1510 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2661 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1515 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2670 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1525 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 2687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1529 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 2695 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1533 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 2703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1537 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1542 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2721 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1547 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 2729 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1551 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 2737 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1555 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2746 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1560 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2755 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1565 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 2764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1570 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 2772 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1574 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 2780 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1578 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 2788 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1582 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 2796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1586 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 2804 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1590 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 2813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1595 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 2821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1599 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 2830 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1604 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 2838 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1608 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 2846 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1612 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 2854 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1616 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 2862 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1620 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 2870 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1624 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 2878 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1628 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 2886 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1632 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 2894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1636 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 2902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1640 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 2910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1644 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 2918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1648 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 2926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1652 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 2934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1656 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 2942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1660 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 2950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1664 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 2958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1668 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 2966 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1672 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 2974 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1676 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 2982 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1680 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 2990 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1684 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 2998 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1688 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1692 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetRSC", yystack_[1].location);
      }
#line 3014 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1696 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3022 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1700 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3030 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1704 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(yystack_[0].value.as< std::string > ()));
      }
#line 3038 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1708 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3046 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1712 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3054 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1716 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1720 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1724 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3078 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1728 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1732 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1736 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1740 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1744 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1748 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3126 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1752 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 3134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1756 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 3142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1760 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 3150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1764 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 3158 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1768 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 3166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1772 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 3174 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1776 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3182 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1780 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 3190 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1784 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3198 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1788 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1792 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3214 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1796 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1800 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1804 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3238 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1808 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1812 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 3254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1816 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 3262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1820 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1824 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1828 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 3286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1832 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 3294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1836 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 3302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1840 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 3310 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 3314 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -138;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     124,  -138,  -138,  -138,  -138,  -117,  -138,  -138,  -138,  -138,
    -138,  -120,  -138,  -138,  -138,  -138,  -138,  -123,  -138,  -138,
       0,  -138,     5,  -138,     4,  -111,    14,    14,  -138,  -138,
    -138,  -122,  -138,     9,    11,  -138,  -138,  -138,    13,  -138,
    -138,  -138,  -138,  -138,  -138,   -83,    14,   183,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,    -5,  -138,  -138,  -138,    15,
      57,     1,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
     321,  -138,   122,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,    -1,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,    -2,   321,
     -83,   262,  -137,  -136,  -126,  -138,  -138,    42,    43,    44,
      45,  -138,    46,    47,    48,    49,    50,    51,    52,    53,
      55,    56,    58,    59,  -138,    60,    61,  -138,    62,    63,
      64,    65,  -138,  -138,    66,   104,   105,   106,    14,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138
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
      93,    82,    81,    94,    95,    78,    76,    77,   132,   133,
     134,   135,   136,   137,   138,     0,   142,   143,   144,     0,
       0,     0,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   163,   162,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   197,   205,   211,   200,   209,   210,   215,   201,   199,
     204,   214,   213,   195,   194,   193,   218,   217,   208,   206,
     219,   207,   198,   196,   212,   203,   202,   216,   191,   192,
       0,    72,    27,    29,    74,    96,    97,    20,    19,    25,
      24,   101,   102,   118,   116,   117,   119,   120,   131,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   112,
     114,   113,   115,     0,   111,    73,     1,     3,   140,   139,
     141,   146,   145,   148,   147,   151,   150,   149,     0,     0,
       0,     0,     0,     0,     0,    35,    36,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,    34,     0,     0,
       0,     0,    37,    38,     0,     0,     0,     0,    71,    30,
      26,    28,   110,    33,    31,    67,    52,    51,    53,    54,
      40,    55,    48,    56,    39,    58,    57,    59,    60,    62,
      61,    63,    49,    64,    65,    66,    41,    42,    43,    44,
      45,    46,    47,    50,    70
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -138,  -138,    98,  -138,   -27,   101,  -138,    54,  -138,  -138,
      68,     2
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,   151,   152,   238,   239,    50,   183,
     184,   153
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     154,   199,   155,   200,    57,    58,    54,   245,   246,    55,
      51,    52,   156,    56,    65,    53,    66,    67,   247,   185,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   188,   189,     1,   199,   195,   190,   196,   197,
      59,    60,    61,    62,    63,    64,   157,   158,   159,   160,
     161,   162,   191,   192,   201,   187,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   240,   186,   193,   194,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   150,   260,
     261,   241,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   274,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,   271,   272,
     273,   198,     0,     0,     0,   244,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,   242,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   243,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      27,     3,   124,     4,   127,   128,   126,   144,   144,   129,
     127,   128,   134,   133,   125,   132,   127,   128,   144,    46,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   127,   128,     0,     3,   125,   132,   127,   128,
     130,   131,   127,   128,   130,   131,   127,   128,   127,   128,
     127,   128,   127,   128,   145,    47,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   184,     0,   127,   128,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   184,   144,
     144,   199,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   238,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   144,   144,
     144,   150,    -1,    -1,    -1,   201,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   200,    -1,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182
  };

  const unsigned char
  seclang_parser::yystos_[] =
  {
       0,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   206,   207,   208,
     213,   127,   128,   132,   126,   129,   133,   127,   128,   130,
     131,   127,   128,   130,   131,   125,   127,   128,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
     184,   209,   210,   216,   209,   124,   134,   127,   128,   127,
     128,   127,   128,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   214,   215,   209,     0,   207,   127,   128,
     132,   127,   128,   127,   128,   125,   127,   128,   210,     3,
       4,   145,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   211,   212,
     184,   216,   215,   169,   212,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   209
  };

  const unsigned char
  seclang_parser::yyr1_[] =
  {
       0,   205,   206,   206,   206,   207,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   209,   209,   210,   210,
     211,   211,   211,   211,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "PIPE", "NEW_LINE",
  "\"Accuracy\"", "\"Allow\"", "\"Append\"", "\"AuditLog\"", "\"Block\"",
  "\"Capture\"", "\"Chain\"", "\"ACTION_CTL_AUDIT_ENGINE\"",
  "\"ACTION_CTL_AUDIT_LOG_PARTS\"", "\"ACTION_CTL_BDY_JSON\"",
  "\"ACTION_CTL_BDY_XML\"", "\"ACTION_CTL_FORCE_REQ_BODY_VAR\"",
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
       0,   487,   487,   491,   492,   495,   500,   506,   512,   516,
     520,   526,   532,   538,   544,   550,   559,   563,   567,   573,
     577,   581,   586,   591,   596,   600,   607,   611,   618,   624,
     634,   643,   653,   662,   674,   678,   682,   686,   690,   694,
     699,   704,   708,   712,   717,   721,   725,   730,   735,   740,
     745,   749,   753,   757,   761,   765,   769,   773,   777,   781,
     785,   789,   793,   797,   801,   805,   809,   813,   817,   831,
     832,   856,   875,   890,   898,   945,   949,   953,   957,   961,
     965,   969,   973,   977,   981,   995,  1007,  1027,  1047,  1052,
    1057,  1062,  1067,  1071,  1075,  1079,  1083,  1087,  1091,  1092,
    1093,  1104,  1108,  1112,  1113,  1114,  1115,  1116,  1117,  1118,
    1124,  1129,  1138,  1155,  1172,  1189,  1206,  1223,  1240,  1257,
    1274,  1291,  1308,  1325,  1342,  1359,  1376,  1393,  1410,  1427,
    1444,  1461,  1481,  1485,  1489,  1493,  1497,  1501,  1505,  1509,
    1514,  1519,  1524,  1528,  1532,  1536,  1541,  1546,  1550,  1554,
    1559,  1564,  1569,  1573,  1577,  1581,  1585,  1589,  1594,  1598,
    1603,  1607,  1611,  1615,  1619,  1623,  1627,  1631,  1635,  1639,
    1643,  1647,  1651,  1655,  1659,  1663,  1667,  1671,  1675,  1679,
    1683,  1687,  1691,  1695,  1699,  1703,  1707,  1711,  1715,  1719,
    1723,  1727,  1731,  1735,  1739,  1743,  1747,  1751,  1755,  1759,
    1763,  1767,  1771,  1775,  1779,  1783,  1787,  1791,  1795,  1799,
    1803,  1807,  1811,  1815,  1819,  1823,  1827,  1831,  1835,  1839
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
#line 4007 "seclang-parser.cc" // lalr1.cc:1167
#line 1845 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
