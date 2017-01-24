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
#line 310 "seclang-parser.yy" // lalr1.cc:413

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
      case 67: // "Accuracy"
      case 68: // "Allow"
      case 69: // "Append"
      case 70: // "AuditLog"
      case 71: // "Block"
      case 72: // "Capture"
      case 73: // "Chain"
      case 74: // "ACTION_CTL_AUDIT_ENGINE"
      case 75: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 76: // "ACTION_CTL_BDY_JSON"
      case 77: // "ACTION_CTL_BDY_XML"
      case 78: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 79: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 80: // "ACTION_CTL_RULE_ENGINE"
      case 81: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 83: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 84: // "Deny"
      case 85: // "DeprecateVar"
      case 86: // "Drop"
      case 87: // "Exec"
      case 88: // "ExpireVar"
      case 89: // "Id"
      case 90: // "InitCol"
      case 91: // "Log"
      case 92: // "LogData"
      case 93: // "Maturity"
      case 94: // "Msg"
      case 95: // "MultiMatch"
      case 96: // "NoAuditLog"
      case 97: // "NoLog"
      case 98: // "Pass"
      case 99: // "Pause"
      case 100: // "Phase"
      case 101: // "Prepend"
      case 102: // "Proxy"
      case 103: // "Redirect"
      case 104: // "Rev"
      case 105: // "SanatiseArg"
      case 106: // "SanatiseMatched"
      case 107: // "SanatiseMatchedBytes"
      case 108: // "SanatiseRequestHeader"
      case 109: // "SanatiseResponseHeader"
      case 110: // "SetEnv"
      case 111: // "SetSrc"
      case 112: // "SetSid"
      case 113: // "SetUID"
      case 114: // "Severity"
      case 115: // "Skip"
      case 116: // "SkipAfter"
      case 117: // "Status"
      case 118: // "Tag"
      case 119: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 120: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 121: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 122: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 123: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 124: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 125: // "ACTION_TRANSFORMATION_LENGTH"
      case 126: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 127: // "ACTION_TRANSFORMATION_MD5"
      case 128: // "ACTION_TRANSFORMATION_NONE"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 130: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 131: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 137: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 139: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 140: // "ACTION_TRANSFORMATION_SHA1"
      case 141: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 142: // "ACTION_TRANSFORMATION_TRIM"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 144: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 145: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 146: // "Ver"
      case 147: // "xmlns"
      case 148: // "CONFIG_COMPONENT_SIG"
      case 149: // "CONFIG_DIR_AUDIT_DIR"
      case 150: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 151: // "CONFIG_DIR_AUDIT_ENG"
      case 152: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 153: // "CONFIG_DIR_AUDIT_LOG"
      case 154: // "CONFIG_DIR_AUDIT_LOG2"
      case 155: // "CONFIG_DIR_AUDIT_LOG_P"
      case 156: // "CONFIG_DIR_AUDIT_STS"
      case 157: // "CONFIG_DIR_AUDIT_TPE"
      case 158: // "CONFIG_DIR_DEBUG_LOG"
      case 159: // "CONFIG_DIR_DEBUG_LVL"
      case 160: // "CONFIG_DIR_GEO_DB"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 162: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 163: // "CONFIG_DIR_REQ_BODY"
      case 164: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 166: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 167: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 168: // "CONFIG_DIR_RES_BODY"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 170: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 171: // "CONFIG_DIR_RULE_ENG"
      case 172: // "CONFIG_DIR_SEC_ACTION"
      case 173: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 174: // "CONFIG_DIR_SEC_MARKER"
      case 175: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 176: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 177: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 178: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 179: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 180: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 181: // "CONFIG_UPLOAD_DIR"
      case 182: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 183: // "CONFIG_UPLOAD_FILE_MODE"
      case 184: // "CONFIG_VALUE_ABORT"
      case 185: // "CONFIG_VALUE_DETC"
      case 186: // "CONFIG_VALUE_HTTPS"
      case 187: // "CONFIG_VALUE_OFF"
      case 188: // "CONFIG_VALUE_ON"
      case 189: // "CONFIG_VALUE_PARALLEL"
      case 190: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 191: // "CONFIG_VALUE_REJECT"
      case 192: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 193: // "CONFIG_VALUE_SERIAL"
      case 194: // "CONFIG_VALUE_WARN"
      case 195: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 196: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 197: // "CONGIG_DIR_SEC_ARG_SEP"
      case 198: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 199: // "CONGIG_DIR_SEC_DATA_DIR"
      case 200: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 201: // "CONGIG_DIR_SEC_TMP_DIR"
      case 202: // "DIRECTIVE"
      case 203: // "DIRECTIVE_SECRULESCRIPT"
      case 204: // "FREE_TEXT"
      case 205: // "OPERATOR"
      case 206: // "OPERATOR_BEGINS_WITH"
      case 207: // "OPERATOR_CONTAINS"
      case 208: // "OPERATOR_CONTAINS_WORD"
      case 209: // "OPERATOR_DETECT_SQLI"
      case 210: // "OPERATOR_DETECT_XSS"
      case 211: // "OPERATOR_ENDS_WITH"
      case 212: // "OPERATOR_EQ"
      case 213: // "OPERATOR_FUZZY_HASH"
      case 214: // "OPERATOR_GE"
      case 215: // "OPERATOR_GEOLOOKUP"
      case 216: // "OPERATOR_GSB_LOOKUP"
      case 217: // "OPERATOR_GT"
      case 218: // "OPERATOR_INSPECT_FILE"
      case 219: // "OPERATOR_IP_MATCH"
      case 220: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 221: // "OPERATOR_LE"
      case 222: // "OPERATOR_LT"
      case 223: // "OPERATOR_PM"
      case 224: // "OPERATOR_PM_FROM_FILE"
      case 225: // "OPERATOR_RBL"
      case 226: // "OPERATOR_RSUB"
      case 227: // "OPERATOR_RX"
      case 228: // "Operator Rx"
      case 229: // "OPERATOR_STR_EQ"
      case 230: // "OPERATOR_STR_MATCH"
      case 231: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 232: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 233: // "OPERATOR_VALIDATE_DTD"
      case 234: // "OPERATOR_VALIDATE_HASH"
      case 235: // "OPERATOR_VALIDATE_SCHEMA"
      case 236: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 237: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 238: // "OPERATOR_VERIFY_CC"
      case 239: // "OPERATOR_VERIFY_CPF"
      case 240: // "OPERATOR_VERIFY_SSN"
      case 241: // "OPERATOR_WITHIN"
      case 242: // "OP_QUOTE"
      case 243: // "QUOTATION_MARK"
      case 244: // "RUN_TIME_VAR_BLD"
      case 245: // "RUN_TIME_VAR_DUR"
      case 246: // "RUN_TIME_VAR_ENV"
      case 247: // "RUN_TIME_VAR_HSV"
      case 248: // "RUN_TIME_VAR_REMOTE_USER"
      case 249: // "RUN_TIME_VAR_RULE"
      case 250: // "RUN_TIME_VAR_TIME"
      case 251: // "RUN_TIME_VAR_TIME_DAY"
      case 252: // "RUN_TIME_VAR_TIME_EPOCH"
      case 253: // "RUN_TIME_VAR_TIME_HOUR"
      case 254: // "RUN_TIME_VAR_TIME_MIN"
      case 255: // "RUN_TIME_VAR_TIME_MON"
      case 256: // "RUN_TIME_VAR_TIME_SEC"
      case 257: // "RUN_TIME_VAR_TIME_WDAY"
      case 258: // "RUN_TIME_VAR_TIME_YEAR"
      case 259: // "RUN_TIME_VAR_XML"
      case 260: // "VARIABLE"
      case 261: // "VARIABLE_COL"
      case 262: // "VARIABLE_STATUS"
      case 263: // "VARIABLE_TX"
      case 264: // "Dictionary element"
      case 265: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 272: // op
      case 273: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 276: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 277: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 275: // variables
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 270: // actions
      case 271: // actions_may_quoted
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
      case 67: // "Accuracy"
      case 68: // "Allow"
      case 69: // "Append"
      case 70: // "AuditLog"
      case 71: // "Block"
      case 72: // "Capture"
      case 73: // "Chain"
      case 74: // "ACTION_CTL_AUDIT_ENGINE"
      case 75: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 76: // "ACTION_CTL_BDY_JSON"
      case 77: // "ACTION_CTL_BDY_XML"
      case 78: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 79: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 80: // "ACTION_CTL_RULE_ENGINE"
      case 81: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 83: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 84: // "Deny"
      case 85: // "DeprecateVar"
      case 86: // "Drop"
      case 87: // "Exec"
      case 88: // "ExpireVar"
      case 89: // "Id"
      case 90: // "InitCol"
      case 91: // "Log"
      case 92: // "LogData"
      case 93: // "Maturity"
      case 94: // "Msg"
      case 95: // "MultiMatch"
      case 96: // "NoAuditLog"
      case 97: // "NoLog"
      case 98: // "Pass"
      case 99: // "Pause"
      case 100: // "Phase"
      case 101: // "Prepend"
      case 102: // "Proxy"
      case 103: // "Redirect"
      case 104: // "Rev"
      case 105: // "SanatiseArg"
      case 106: // "SanatiseMatched"
      case 107: // "SanatiseMatchedBytes"
      case 108: // "SanatiseRequestHeader"
      case 109: // "SanatiseResponseHeader"
      case 110: // "SetEnv"
      case 111: // "SetSrc"
      case 112: // "SetSid"
      case 113: // "SetUID"
      case 114: // "Severity"
      case 115: // "Skip"
      case 116: // "SkipAfter"
      case 117: // "Status"
      case 118: // "Tag"
      case 119: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 120: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 121: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 122: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 123: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 124: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 125: // "ACTION_TRANSFORMATION_LENGTH"
      case 126: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 127: // "ACTION_TRANSFORMATION_MD5"
      case 128: // "ACTION_TRANSFORMATION_NONE"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 130: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 131: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 137: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 139: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 140: // "ACTION_TRANSFORMATION_SHA1"
      case 141: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 142: // "ACTION_TRANSFORMATION_TRIM"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 144: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 145: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 146: // "Ver"
      case 147: // "xmlns"
      case 148: // "CONFIG_COMPONENT_SIG"
      case 149: // "CONFIG_DIR_AUDIT_DIR"
      case 150: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 151: // "CONFIG_DIR_AUDIT_ENG"
      case 152: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 153: // "CONFIG_DIR_AUDIT_LOG"
      case 154: // "CONFIG_DIR_AUDIT_LOG2"
      case 155: // "CONFIG_DIR_AUDIT_LOG_P"
      case 156: // "CONFIG_DIR_AUDIT_STS"
      case 157: // "CONFIG_DIR_AUDIT_TPE"
      case 158: // "CONFIG_DIR_DEBUG_LOG"
      case 159: // "CONFIG_DIR_DEBUG_LVL"
      case 160: // "CONFIG_DIR_GEO_DB"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 162: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 163: // "CONFIG_DIR_REQ_BODY"
      case 164: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 166: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 167: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 168: // "CONFIG_DIR_RES_BODY"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 170: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 171: // "CONFIG_DIR_RULE_ENG"
      case 172: // "CONFIG_DIR_SEC_ACTION"
      case 173: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 174: // "CONFIG_DIR_SEC_MARKER"
      case 175: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 176: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 177: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 178: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 179: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 180: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 181: // "CONFIG_UPLOAD_DIR"
      case 182: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 183: // "CONFIG_UPLOAD_FILE_MODE"
      case 184: // "CONFIG_VALUE_ABORT"
      case 185: // "CONFIG_VALUE_DETC"
      case 186: // "CONFIG_VALUE_HTTPS"
      case 187: // "CONFIG_VALUE_OFF"
      case 188: // "CONFIG_VALUE_ON"
      case 189: // "CONFIG_VALUE_PARALLEL"
      case 190: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 191: // "CONFIG_VALUE_REJECT"
      case 192: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 193: // "CONFIG_VALUE_SERIAL"
      case 194: // "CONFIG_VALUE_WARN"
      case 195: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 196: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 197: // "CONGIG_DIR_SEC_ARG_SEP"
      case 198: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 199: // "CONGIG_DIR_SEC_DATA_DIR"
      case 200: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 201: // "CONGIG_DIR_SEC_TMP_DIR"
      case 202: // "DIRECTIVE"
      case 203: // "DIRECTIVE_SECRULESCRIPT"
      case 204: // "FREE_TEXT"
      case 205: // "OPERATOR"
      case 206: // "OPERATOR_BEGINS_WITH"
      case 207: // "OPERATOR_CONTAINS"
      case 208: // "OPERATOR_CONTAINS_WORD"
      case 209: // "OPERATOR_DETECT_SQLI"
      case 210: // "OPERATOR_DETECT_XSS"
      case 211: // "OPERATOR_ENDS_WITH"
      case 212: // "OPERATOR_EQ"
      case 213: // "OPERATOR_FUZZY_HASH"
      case 214: // "OPERATOR_GE"
      case 215: // "OPERATOR_GEOLOOKUP"
      case 216: // "OPERATOR_GSB_LOOKUP"
      case 217: // "OPERATOR_GT"
      case 218: // "OPERATOR_INSPECT_FILE"
      case 219: // "OPERATOR_IP_MATCH"
      case 220: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 221: // "OPERATOR_LE"
      case 222: // "OPERATOR_LT"
      case 223: // "OPERATOR_PM"
      case 224: // "OPERATOR_PM_FROM_FILE"
      case 225: // "OPERATOR_RBL"
      case 226: // "OPERATOR_RSUB"
      case 227: // "OPERATOR_RX"
      case 228: // "Operator Rx"
      case 229: // "OPERATOR_STR_EQ"
      case 230: // "OPERATOR_STR_MATCH"
      case 231: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 232: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 233: // "OPERATOR_VALIDATE_DTD"
      case 234: // "OPERATOR_VALIDATE_HASH"
      case 235: // "OPERATOR_VALIDATE_SCHEMA"
      case 236: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 237: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 238: // "OPERATOR_VERIFY_CC"
      case 239: // "OPERATOR_VERIFY_CPF"
      case 240: // "OPERATOR_VERIFY_SSN"
      case 241: // "OPERATOR_WITHIN"
      case 242: // "OP_QUOTE"
      case 243: // "QUOTATION_MARK"
      case 244: // "RUN_TIME_VAR_BLD"
      case 245: // "RUN_TIME_VAR_DUR"
      case 246: // "RUN_TIME_VAR_ENV"
      case 247: // "RUN_TIME_VAR_HSV"
      case 248: // "RUN_TIME_VAR_REMOTE_USER"
      case 249: // "RUN_TIME_VAR_RULE"
      case 250: // "RUN_TIME_VAR_TIME"
      case 251: // "RUN_TIME_VAR_TIME_DAY"
      case 252: // "RUN_TIME_VAR_TIME_EPOCH"
      case 253: // "RUN_TIME_VAR_TIME_HOUR"
      case 254: // "RUN_TIME_VAR_TIME_MIN"
      case 255: // "RUN_TIME_VAR_TIME_MON"
      case 256: // "RUN_TIME_VAR_TIME_SEC"
      case 257: // "RUN_TIME_VAR_TIME_WDAY"
      case 258: // "RUN_TIME_VAR_TIME_YEAR"
      case 259: // "RUN_TIME_VAR_XML"
      case 260: // "VARIABLE"
      case 261: // "VARIABLE_COL"
      case 262: // "VARIABLE_STATUS"
      case 263: // "VARIABLE_TX"
      case 264: // "Dictionary element"
      case 265: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 272: // op
      case 273: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 276: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 277: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 275: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 270: // actions
      case 271: // actions_may_quoted
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
    #line 303 "/home/zimmerle/core/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
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
      case 67: // "Accuracy"
      case 68: // "Allow"
      case 69: // "Append"
      case 70: // "AuditLog"
      case 71: // "Block"
      case 72: // "Capture"
      case 73: // "Chain"
      case 74: // "ACTION_CTL_AUDIT_ENGINE"
      case 75: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 76: // "ACTION_CTL_BDY_JSON"
      case 77: // "ACTION_CTL_BDY_XML"
      case 78: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 79: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 80: // "ACTION_CTL_RULE_ENGINE"
      case 81: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 82: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 83: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 84: // "Deny"
      case 85: // "DeprecateVar"
      case 86: // "Drop"
      case 87: // "Exec"
      case 88: // "ExpireVar"
      case 89: // "Id"
      case 90: // "InitCol"
      case 91: // "Log"
      case 92: // "LogData"
      case 93: // "Maturity"
      case 94: // "Msg"
      case 95: // "MultiMatch"
      case 96: // "NoAuditLog"
      case 97: // "NoLog"
      case 98: // "Pass"
      case 99: // "Pause"
      case 100: // "Phase"
      case 101: // "Prepend"
      case 102: // "Proxy"
      case 103: // "Redirect"
      case 104: // "Rev"
      case 105: // "SanatiseArg"
      case 106: // "SanatiseMatched"
      case 107: // "SanatiseMatchedBytes"
      case 108: // "SanatiseRequestHeader"
      case 109: // "SanatiseResponseHeader"
      case 110: // "SetEnv"
      case 111: // "SetSrc"
      case 112: // "SetSid"
      case 113: // "SetUID"
      case 114: // "Severity"
      case 115: // "Skip"
      case 116: // "SkipAfter"
      case 117: // "Status"
      case 118: // "Tag"
      case 119: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 120: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 121: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 122: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 123: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 124: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 125: // "ACTION_TRANSFORMATION_LENGTH"
      case 126: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 127: // "ACTION_TRANSFORMATION_MD5"
      case 128: // "ACTION_TRANSFORMATION_NONE"
      case 129: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 130: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 131: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 132: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 133: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 134: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 135: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 136: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 137: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 138: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 139: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 140: // "ACTION_TRANSFORMATION_SHA1"
      case 141: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 142: // "ACTION_TRANSFORMATION_TRIM"
      case 143: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 144: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 145: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 146: // "Ver"
      case 147: // "xmlns"
      case 148: // "CONFIG_COMPONENT_SIG"
      case 149: // "CONFIG_DIR_AUDIT_DIR"
      case 150: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 151: // "CONFIG_DIR_AUDIT_ENG"
      case 152: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 153: // "CONFIG_DIR_AUDIT_LOG"
      case 154: // "CONFIG_DIR_AUDIT_LOG2"
      case 155: // "CONFIG_DIR_AUDIT_LOG_P"
      case 156: // "CONFIG_DIR_AUDIT_STS"
      case 157: // "CONFIG_DIR_AUDIT_TPE"
      case 158: // "CONFIG_DIR_DEBUG_LOG"
      case 159: // "CONFIG_DIR_DEBUG_LVL"
      case 160: // "CONFIG_DIR_GEO_DB"
      case 161: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 162: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 163: // "CONFIG_DIR_REQ_BODY"
      case 164: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 165: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 166: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 167: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 168: // "CONFIG_DIR_RES_BODY"
      case 169: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 170: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 171: // "CONFIG_DIR_RULE_ENG"
      case 172: // "CONFIG_DIR_SEC_ACTION"
      case 173: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 174: // "CONFIG_DIR_SEC_MARKER"
      case 175: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 176: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 177: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 178: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 179: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 180: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 181: // "CONFIG_UPLOAD_DIR"
      case 182: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 183: // "CONFIG_UPLOAD_FILE_MODE"
      case 184: // "CONFIG_VALUE_ABORT"
      case 185: // "CONFIG_VALUE_DETC"
      case 186: // "CONFIG_VALUE_HTTPS"
      case 187: // "CONFIG_VALUE_OFF"
      case 188: // "CONFIG_VALUE_ON"
      case 189: // "CONFIG_VALUE_PARALLEL"
      case 190: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 191: // "CONFIG_VALUE_REJECT"
      case 192: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 193: // "CONFIG_VALUE_SERIAL"
      case 194: // "CONFIG_VALUE_WARN"
      case 195: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 196: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 197: // "CONGIG_DIR_SEC_ARG_SEP"
      case 198: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 199: // "CONGIG_DIR_SEC_DATA_DIR"
      case 200: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 201: // "CONGIG_DIR_SEC_TMP_DIR"
      case 202: // "DIRECTIVE"
      case 203: // "DIRECTIVE_SECRULESCRIPT"
      case 204: // "FREE_TEXT"
      case 205: // "OPERATOR"
      case 206: // "OPERATOR_BEGINS_WITH"
      case 207: // "OPERATOR_CONTAINS"
      case 208: // "OPERATOR_CONTAINS_WORD"
      case 209: // "OPERATOR_DETECT_SQLI"
      case 210: // "OPERATOR_DETECT_XSS"
      case 211: // "OPERATOR_ENDS_WITH"
      case 212: // "OPERATOR_EQ"
      case 213: // "OPERATOR_FUZZY_HASH"
      case 214: // "OPERATOR_GE"
      case 215: // "OPERATOR_GEOLOOKUP"
      case 216: // "OPERATOR_GSB_LOOKUP"
      case 217: // "OPERATOR_GT"
      case 218: // "OPERATOR_INSPECT_FILE"
      case 219: // "OPERATOR_IP_MATCH"
      case 220: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 221: // "OPERATOR_LE"
      case 222: // "OPERATOR_LT"
      case 223: // "OPERATOR_PM"
      case 224: // "OPERATOR_PM_FROM_FILE"
      case 225: // "OPERATOR_RBL"
      case 226: // "OPERATOR_RSUB"
      case 227: // "OPERATOR_RX"
      case 228: // "Operator Rx"
      case 229: // "OPERATOR_STR_EQ"
      case 230: // "OPERATOR_STR_MATCH"
      case 231: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 232: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 233: // "OPERATOR_VALIDATE_DTD"
      case 234: // "OPERATOR_VALIDATE_HASH"
      case 235: // "OPERATOR_VALIDATE_SCHEMA"
      case 236: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 237: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 238: // "OPERATOR_VERIFY_CC"
      case 239: // "OPERATOR_VERIFY_CPF"
      case 240: // "OPERATOR_VERIFY_SSN"
      case 241: // "OPERATOR_WITHIN"
      case 242: // "OP_QUOTE"
      case 243: // "QUOTATION_MARK"
      case 244: // "RUN_TIME_VAR_BLD"
      case 245: // "RUN_TIME_VAR_DUR"
      case 246: // "RUN_TIME_VAR_ENV"
      case 247: // "RUN_TIME_VAR_HSV"
      case 248: // "RUN_TIME_VAR_REMOTE_USER"
      case 249: // "RUN_TIME_VAR_RULE"
      case 250: // "RUN_TIME_VAR_TIME"
      case 251: // "RUN_TIME_VAR_TIME_DAY"
      case 252: // "RUN_TIME_VAR_TIME_EPOCH"
      case 253: // "RUN_TIME_VAR_TIME_HOUR"
      case 254: // "RUN_TIME_VAR_TIME_MIN"
      case 255: // "RUN_TIME_VAR_TIME_MON"
      case 256: // "RUN_TIME_VAR_TIME_SEC"
      case 257: // "RUN_TIME_VAR_TIME_WDAY"
      case 258: // "RUN_TIME_VAR_TIME_YEAR"
      case 259: // "RUN_TIME_VAR_XML"
      case 260: // "VARIABLE"
      case 261: // "VARIABLE_COL"
      case 262: // "VARIABLE_STATUS"
      case 263: // "VARIABLE_TX"
      case 264: // "Dictionary element"
      case 265: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 272: // op
      case 273: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 276: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 277: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 275: // variables
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 270: // actions
      case 271: // actions_may_quoted
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
#line 608 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 621 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 627 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1217 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 633 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 637 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 641 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 647 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 653 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 659 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1265 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 665 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1273 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 671 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 680 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1292 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 684 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 688 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1308 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 694 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1316 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 698 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 702 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 707 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 712 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 717 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 721 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 728 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 732 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 739 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 745 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 755 "seclang-parser.yy" // lalr1.cc:859
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
#line 764 "seclang-parser.yy" // lalr1.cc:859
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
#line 774 "seclang-parser.yy" // lalr1.cc:859
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
#line 783 "seclang-parser.yy" // lalr1.cc:859
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
#line 795 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 799 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 803 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1481 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 807 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1489 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 811 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 815 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1506 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 820 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1515 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 825 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1523 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 829 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1531 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 833 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1540 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 838 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1548 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 842 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 846 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 851 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 856 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 861 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 866 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 870 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 874 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 878 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1624 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 882 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 886 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1640 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 890 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 894 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 898 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1664 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 902 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1672 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 906 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 910 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 914 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 918 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 922 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 926 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 930 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 934 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1736 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 938 "seclang-parser.yy" // lalr1.cc:859
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
#line 953 "seclang-parser.yy" // lalr1.cc:859
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
#line 977 "seclang-parser.yy" // lalr1.cc:859
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
#line 996 "seclang-parser.yy" // lalr1.cc:859
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
#line 1011 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1019 "seclang-parser.yy" // lalr1.cc:859
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
#line 1066 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1892 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1070 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1900 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1074 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1908 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1078 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1916 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1082 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1924 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1086 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1932 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1090 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1940 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1094 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1948 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1098 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1956 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1102 "seclang-parser.yy" // lalr1.cc:859
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
#line 1116 "seclang-parser.yy" // lalr1.cc:859
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
#line 1128 "seclang-parser.yy" // lalr1.cc:859
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
#line 1148 "seclang-parser.yy" // lalr1.cc:859
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
#line 1168 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2044 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1173 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1178 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2062 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1183 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1188 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1192 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1196 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1200 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1204 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1208 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1214 "seclang-parser.yy" // lalr1.cc:859
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
#line 1225 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1229 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1239 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1245 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2166 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1250 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2176 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1259 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2185 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1264 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2194 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1269 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2203 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1274 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::Args_NoDictElement());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2212 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1279 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsGetNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1284 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsPostNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1289 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestHeadersNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1294 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseContentType());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1299 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseHeadersNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1304 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ArgsCombinedSize());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2266 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1309 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::AuthType());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2275 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1314 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FilesCombinedSize());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2284 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1319 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FilesTmpNames());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1324 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FullRequest());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2302 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1329 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::FullRequestLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1334 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::InboundDataError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2320 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1339 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MatchedVar());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2329 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1344 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MatchedVarName());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2338 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1349 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartCrlfLFLines());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2347 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1354 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartDateAfter());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2356 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1359 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartFileLimitExceeded());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2365 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1364 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartHeaderFolding());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1369 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartInvalidHeaderFolding());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1374 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartInvalidQuoting());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1379 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartStrictError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2401 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1384 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::MultipartUnmatchedBoundary());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2410 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1389 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::OutboundDataError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2419 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1394 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::PathInfo());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2428 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1399 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::QueryString());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2437 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1404 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemoteAddr());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2446 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1409 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemoteHost());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1414 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RemotePort());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2464 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1419 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1424 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyErrorMsg());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2482 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1429 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessor());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2491 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1434 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessorError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2500 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1439 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ReqbodyProcessorErrorMsg());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2509 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1444 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBasename());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1449 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBody());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1454 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestBodyLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2536 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1459 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestFilename());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2545 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1464 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestLine());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2554 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1469 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestMethod());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2563 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1474 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestProtocol());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2572 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1479 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestURI());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2581 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1484 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::RequestURIRaw());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1489 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::Resource());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1494 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseBody());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1499 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseContentLength());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1504 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseProtocol());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2626 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1509 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ResponseStatus());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1514 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerAddr());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2644 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1519 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerName());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2653 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1524 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::ServerPort());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2662 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1529 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::SessionID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2671 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1534 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UniqueID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1539 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UrlEncodedError());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2689 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1544 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new Variables::UserID());
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 2698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1549 "seclang-parser.yy" // lalr1.cc:859
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
#line 2719 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1566 "seclang-parser.yy" // lalr1.cc:859
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
#line 2740 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1583 "seclang-parser.yy" // lalr1.cc:859
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
#line 2761 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1600 "seclang-parser.yy" // lalr1.cc:859
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
#line 2782 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1617 "seclang-parser.yy" // lalr1.cc:859
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
#line 2803 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1634 "seclang-parser.yy" // lalr1.cc:859
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
#line 2824 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1651 "seclang-parser.yy" // lalr1.cc:859
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
#line 2845 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1668 "seclang-parser.yy" // lalr1.cc:859
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
#line 2866 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1685 "seclang-parser.yy" // lalr1.cc:859
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
#line 2887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1702 "seclang-parser.yy" // lalr1.cc:859
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
#line 2908 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1719 "seclang-parser.yy" // lalr1.cc:859
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
#line 2929 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1736 "seclang-parser.yy" // lalr1.cc:859
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
#line 2950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1753 "seclang-parser.yy" // lalr1.cc:859
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
#line 2971 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1770 "seclang-parser.yy" // lalr1.cc:859
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
#line 2992 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1787 "seclang-parser.yy" // lalr1.cc:859
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
#line 3013 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1804 "seclang-parser.yy" // lalr1.cc:859
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
#line 3034 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1821 "seclang-parser.yy" // lalr1.cc:859
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
#line 3055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1838 "seclang-parser.yy" // lalr1.cc:859
    {
            std::string name(yystack_[0].value.as< std::string > ());
            std::unique_ptr<Variable> c(new XML(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3065 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1844 "seclang-parser.yy" // lalr1.cc:859
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
#line 3086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1864 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1868 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1872 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1876 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1880 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3126 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1884 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1888 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3142 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1892 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1897 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3160 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1902 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3169 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1907 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3177 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1911 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3185 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1915 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3193 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1919 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3202 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1924 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3211 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1929 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3219 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1933 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3227 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1937 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3236 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1942 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3245 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1947 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1952 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1956 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1960 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3278 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1964 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3286 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1968 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3294 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1972 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1977 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 3311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1981 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3320 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1986 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3328 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1990 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3336 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1994 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3344 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1998 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3352 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 2002 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3360 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 2006 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 3368 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 2010 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 3376 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 2014 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3384 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 2018 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 3392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 2022 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 3400 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 2026 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 3408 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 2030 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 3416 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 2034 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 3424 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 2038 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 3432 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 2042 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 3440 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 2046 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 3448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 2050 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 3456 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 2054 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 3464 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 2058 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 3472 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 2062 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 3480 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 2066 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3488 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 2070 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3496 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 2074 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetRSC", yystack_[1].location);
      }
#line 3504 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 2078 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3512 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 2082 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3520 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 2086 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3528 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 2090 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3536 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 2094 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3544 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 2098 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3552 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 2102 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3560 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 2106 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3568 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 2110 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3576 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 2114 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3584 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 2118 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 2122 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 2126 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3608 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 2130 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3616 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 2134 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3624 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2138 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3632 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2142 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3640 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2146 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3648 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2150 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 3656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2154 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 3664 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2158 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 3672 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2162 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 3680 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2166 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 3688 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2170 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 3696 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2174 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3704 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2178 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 3712 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2182 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3720 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2186 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 3728 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2190 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3736 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2194 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 3744 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2198 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3752 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2202 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3760 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2206 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3768 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2210 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 3776 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2214 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 3784 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2218 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 3792 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2222 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 3800 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2226 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 3808 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2230 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 3816 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2234 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 3824 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2238 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 3832 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 3836 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -257;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
       4,  -257,  -257,  -257,  -257,  -176,  -257,  -257,  -257,  -257,
    -257,  -179,  -257,  -257,  -257,  -257,  -257,  -182,  -257,  -257,
     -63,  -257,   -58,  -257,   -59,  -164,   330,   330,  -257,  -257,
    -257,  -181,  -257,   -54,   -52,  -257,  -257,  -257,   -50,  -257,
    -257,  -257,  -257,  -257,  -257,    27,   330,   188,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,   -64,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -170,  -257,  -257,  -257,
     -48,   -46,  -160,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
     416,  -257,    12,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -256,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,    85,  -257,
    -257,  -257,  -257,  -240,   -34,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,    -2,   416,  -257,  -257,    27,
    -116,  -178,  -172,   -78,  -257,  -257,   -61,   -60,   -57,   -56,
    -257,   -55,   -15,   -14,   -13,   -12,   -11,   -10,    -9,    -7,
      -6,     5,     6,  -257,     7,     8,  -257,     9,    10,    11,
      13,  -257,  -257,    14,    15,    16,    17,   330,  -257,  -257,
      18,    19,    20,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257
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
      93,    82,    81,    94,    95,    78,    76,    77,     0,   189,
     190,   191,   192,   193,   194,   195,     0,   199,   200,   201,
       0,     0,     0,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   220,   219,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   247,   248,   249,   250,
     251,   258,   266,   272,   261,   270,   271,   276,   262,   260,
     265,   275,   274,   256,   255,   254,   279,   278,   269,   267,
     280,   268,   259,   257,   273,   264,   263,   277,   252,   253,
       0,    72,    27,    29,    74,    96,    97,    20,    19,    25,
      24,   101,   102,   115,   121,   116,   112,   117,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     145,   144,   148,   147,   146,   149,   151,   150,   152,   118,
     153,   154,   155,   157,   156,   158,   159,   160,   119,   120,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   175,
     173,   174,   176,   177,   188,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   171,   170,   172,     0,   111,
      73,     1,     3,     0,   243,   197,   196,   198,   203,   202,
     205,   204,   208,   207,   206,     0,     0,   113,   114,     0,
       0,     0,     0,     0,    35,    36,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,     0,     0,    34,     0,     0,     0,
       0,    37,    38,     0,     0,     0,     0,    71,    30,   242,
       0,     0,     0,    26,    28,   110,    33,    31,    67,    52,
      51,    53,    54,    40,    55,    48,    56,    39,    58,    57,
      59,    60,    62,    61,    63,    49,    64,    65,    66,    41,
      42,    43,    44,    45,    46,    47,    50,    70,   244,   245,
     246
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -257,  -257,    98,  -257,   -27,    -4,  -257,  -110,  -257,  -257,
     -51,   -40
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,   151,   152,   297,   298,    50,   238,
     239,   153
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     154,   256,   243,   155,     1,    57,    58,    54,   257,   258,
      55,    51,    52,   156,    56,   256,    53,   245,   246,   240,
     299,    65,   247,    66,    67,   252,   308,   253,   254,   300,
     301,   302,   309,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   259,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   306,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   310,    59,    60,    61,
      62,    63,    64,   157,   158,   159,   160,   161,   162,   248,
     249,   250,   251,   311,   312,   242,   255,   313,   314,   315,
     307,   260,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,   241,   316,
     317,   318,   319,   320,   321,   322,   244,   323,   324,    38,
      39,    40,    41,    42,    43,    44,    45,    46,   305,   325,
     326,   327,   328,   329,   330,   331,   304,   332,   333,   334,
     335,   336,   338,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,     0,   235,   236,
     237,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    68,     0,     0,     0,     0,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    68,     0,
       0,     0,     0,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   150
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      27,     3,    66,   184,     0,   187,   188,   186,   264,   265,
     189,   187,   188,   194,   193,     3,   192,   187,   188,    46,
     260,   185,   192,   187,   188,   185,   204,   187,   188,    63,
      64,    65,   204,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     4,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   204,   190,   191,   187,
     188,   190,   191,   187,   188,   187,   188,   187,   188,   187,
     188,   187,   188,   204,   204,    47,   150,   204,   204,   204,
     260,    66,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,     0,   204,
     204,   204,   204,   204,   204,   204,   260,   204,   204,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   259,   204,
     204,   204,   204,   204,   204,   204,   256,   204,   204,   204,
     204,   204,   204,   204,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     297,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,    -1,   261,   262,
     263,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,    62,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    62,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   267,   268,   269,
     274,   187,   188,   192,   186,   189,   193,   187,   188,   190,
     191,   187,   188,   190,   191,   185,   187,   188,    62,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     243,   270,   271,   277,   270,   184,   194,   187,   188,   187,
     188,   187,   188,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   261,   262,   263,   275,   276,
     270,     0,   268,    66,   260,   187,   188,   192,   187,   188,
     187,   188,   185,   187,   188,   271,     3,   264,   265,     4,
      66,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   272,   273,   260,
      63,    64,    65,   243,   277,   276,   228,   273,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   270,   204,   204,
     204
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   266,   267,   267,   267,   268,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   270,   270,   271,   271,
     272,   272,   272,   272,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     275,   275,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277
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
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     4,     4,     4,     1,     1,     1,
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
  "VARIABLE_ARGS", "VARIABLE_ARGS_COMBINED_SIZE",
  "VARIABLE_ARGS_GET_NAMES", "\"Variable ARGS_NAMES\"",
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
  "\"VARIABLE_STATUS\"", "\"VARIABLE_TX\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "var", "act", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   607,   607,   611,   612,   615,   620,   626,   632,   636,
     640,   646,   652,   658,   664,   670,   679,   683,   687,   693,
     697,   701,   706,   711,   716,   720,   727,   731,   738,   744,
     754,   763,   773,   782,   794,   798,   802,   806,   810,   814,
     819,   824,   828,   832,   837,   841,   845,   850,   855,   860,
     865,   869,   873,   877,   881,   885,   889,   893,   897,   901,
     905,   909,   913,   917,   921,   925,   929,   933,   937,   951,
     952,   976,   995,  1010,  1018,  1065,  1069,  1073,  1077,  1081,
    1085,  1089,  1093,  1097,  1101,  1115,  1127,  1147,  1167,  1172,
    1177,  1182,  1187,  1191,  1195,  1199,  1203,  1207,  1211,  1212,
    1213,  1224,  1228,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1244,  1249,  1258,  1263,  1268,  1273,  1278,  1283,  1288,  1293,
    1298,  1303,  1308,  1313,  1318,  1323,  1328,  1333,  1338,  1343,
    1348,  1353,  1358,  1363,  1368,  1373,  1378,  1383,  1388,  1393,
    1398,  1403,  1408,  1413,  1418,  1423,  1428,  1433,  1438,  1443,
    1448,  1453,  1458,  1463,  1468,  1473,  1478,  1483,  1488,  1493,
    1498,  1503,  1508,  1513,  1518,  1523,  1528,  1533,  1538,  1543,
    1548,  1565,  1582,  1599,  1616,  1633,  1650,  1667,  1684,  1701,
    1718,  1735,  1752,  1769,  1786,  1803,  1820,  1837,  1843,  1863,
    1867,  1871,  1875,  1879,  1883,  1887,  1891,  1896,  1901,  1906,
    1910,  1914,  1918,  1923,  1928,  1932,  1936,  1941,  1946,  1951,
    1955,  1959,  1963,  1967,  1971,  1976,  1980,  1985,  1989,  1993,
    1997,  2001,  2005,  2009,  2013,  2017,  2021,  2025,  2029,  2033,
    2037,  2041,  2045,  2049,  2053,  2057,  2061,  2065,  2069,  2073,
    2077,  2081,  2085,  2089,  2093,  2097,  2101,  2105,  2109,  2113,
    2117,  2121,  2125,  2129,  2133,  2137,  2141,  2145,  2149,  2153,
    2157,  2161,  2165,  2169,  2173,  2177,  2181,  2185,  2189,  2193,
    2197,  2201,  2205,  2209,  2213,  2217,  2221,  2225,  2229,  2233,
    2237
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
#line 4623 "seclang-parser.cc" // lalr1.cc:1167
#line 2243 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
