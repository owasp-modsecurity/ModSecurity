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
#line 346 "seclang-parser.yy" // lalr1.cc:413

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
      case 93: // "Accuracy"
      case 94: // "Allow"
      case 95: // "Append"
      case 96: // "AuditLog"
      case 97: // "Block"
      case 98: // "Capture"
      case 99: // "Chain"
      case 100: // "ACTION_CTL_AUDIT_ENGINE"
      case 101: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 102: // "ACTION_CTL_BDY_JSON"
      case 103: // "ACTION_CTL_BDY_XML"
      case 104: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 105: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 106: // "ACTION_CTL_RULE_ENGINE"
      case 107: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 108: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 110: // "Deny"
      case 111: // "DeprecateVar"
      case 112: // "Drop"
      case 113: // "Exec"
      case 114: // "ExpireVar"
      case 115: // "Id"
      case 116: // "InitCol"
      case 117: // "Log"
      case 118: // "LogData"
      case 119: // "Maturity"
      case 120: // "Msg"
      case 121: // "MultiMatch"
      case 122: // "NoAuditLog"
      case 123: // "NoLog"
      case 124: // "Pass"
      case 125: // "Pause"
      case 126: // "Phase"
      case 127: // "Prepend"
      case 128: // "Proxy"
      case 129: // "Redirect"
      case 130: // "Rev"
      case 131: // "SanatiseArg"
      case 132: // "SanatiseMatched"
      case 133: // "SanatiseMatchedBytes"
      case 134: // "SanatiseRequestHeader"
      case 135: // "SanatiseResponseHeader"
      case 136: // "SetEnv"
      case 137: // "SetSrc"
      case 138: // "SetSid"
      case 139: // "SetUID"
      case 140: // "Severity"
      case 141: // "Skip"
      case 142: // "SkipAfter"
      case 143: // "Status"
      case 144: // "Tag"
      case 145: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 146: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 147: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 148: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 151: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 152: // "ACTION_TRANSFORMATION_LENGTH"
      case 153: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 154: // "ACTION_TRANSFORMATION_MD5"
      case 155: // "ACTION_TRANSFORMATION_NONE"
      case 156: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 158: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 159: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 165: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 167: // "ACTION_TRANSFORMATION_SHA1"
      case 168: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 169: // "ACTION_TRANSFORMATION_TRIM"
      case 170: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 171: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 173: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 174: // "Ver"
      case 175: // "xmlns"
      case 176: // "CONFIG_COMPONENT_SIG"
      case 177: // "CONFIG_DIR_AUDIT_DIR"
      case 178: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 179: // "CONFIG_DIR_AUDIT_ENG"
      case 180: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 181: // "CONFIG_DIR_AUDIT_LOG"
      case 182: // "CONFIG_DIR_AUDIT_LOG2"
      case 183: // "CONFIG_DIR_AUDIT_LOG_P"
      case 184: // "CONFIG_DIR_AUDIT_STS"
      case 185: // "CONFIG_DIR_AUDIT_TPE"
      case 186: // "CONFIG_DIR_DEBUG_LOG"
      case 187: // "CONFIG_DIR_DEBUG_LVL"
      case 188: // "CONFIG_DIR_GEO_DB"
      case 189: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 191: // "CONFIG_DIR_REQ_BODY"
      case 192: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 193: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 195: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 196: // "CONFIG_DIR_RES_BODY"
      case 197: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 199: // "CONFIG_DIR_RULE_ENG"
      case 200: // "CONFIG_DIR_SEC_ACTION"
      case 201: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 202: // "CONFIG_DIR_SEC_MARKER"
      case 203: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 204: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 205: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 206: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 207: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 208: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 209: // "CONFIG_UPLOAD_DIR"
      case 210: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 211: // "CONFIG_UPLOAD_FILE_MODE"
      case 212: // "CONFIG_VALUE_ABORT"
      case 213: // "CONFIG_VALUE_DETC"
      case 214: // "CONFIG_VALUE_HTTPS"
      case 215: // "CONFIG_VALUE_OFF"
      case 216: // "CONFIG_VALUE_ON"
      case 217: // "CONFIG_VALUE_PARALLEL"
      case 218: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 219: // "CONFIG_VALUE_REJECT"
      case 220: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 221: // "CONFIG_VALUE_SERIAL"
      case 222: // "CONFIG_VALUE_WARN"
      case 223: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 224: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 225: // "CONGIG_DIR_SEC_ARG_SEP"
      case 226: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 227: // "CONGIG_DIR_SEC_DATA_DIR"
      case 228: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 229: // "CONGIG_DIR_SEC_TMP_DIR"
      case 230: // "DIRECTIVE"
      case 231: // "DIRECTIVE_SECRULESCRIPT"
      case 232: // "FREE_TEXT"
      case 233: // "OPERATOR"
      case 234: // "OPERATOR_BEGINS_WITH"
      case 235: // "OPERATOR_CONTAINS"
      case 236: // "OPERATOR_CONTAINS_WORD"
      case 237: // "OPERATOR_DETECT_SQLI"
      case 238: // "OPERATOR_DETECT_XSS"
      case 239: // "OPERATOR_ENDS_WITH"
      case 240: // "OPERATOR_EQ"
      case 241: // "OPERATOR_FUZZY_HASH"
      case 242: // "OPERATOR_GE"
      case 243: // "OPERATOR_GEOLOOKUP"
      case 244: // "OPERATOR_GSB_LOOKUP"
      case 245: // "OPERATOR_GT"
      case 246: // "OPERATOR_INSPECT_FILE"
      case 247: // "OPERATOR_IP_MATCH"
      case 248: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 249: // "OPERATOR_LE"
      case 250: // "OPERATOR_LT"
      case 251: // "OPERATOR_PM"
      case 252: // "OPERATOR_PM_FROM_FILE"
      case 253: // "OPERATOR_RBL"
      case 254: // "OPERATOR_RSUB"
      case 255: // "OPERATOR_RX"
      case 256: // "Operator RX (content only)"
      case 257: // "OPERATOR_STR_EQ"
      case 258: // "OPERATOR_STR_MATCH"
      case 259: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 260: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 261: // "OPERATOR_VALIDATE_DTD"
      case 262: // "OPERATOR_VALIDATE_HASH"
      case 263: // "OPERATOR_VALIDATE_SCHEMA"
      case 264: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 265: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 266: // "OPERATOR_VERIFY_CC"
      case 267: // "OPERATOR_VERIFY_CPF"
      case 268: // "OPERATOR_VERIFY_SSN"
      case 269: // "OPERATOR_WITHIN"
      case 270: // "OP_QUOTE"
      case 271: // "QUOTATION_MARK"
      case 272: // "RUN_TIME_VAR_BLD"
      case 273: // "RUN_TIME_VAR_DUR"
      case 274: // "RUN_TIME_VAR_HSV"
      case 275: // "RUN_TIME_VAR_REMOTE_USER"
      case 276: // "RUN_TIME_VAR_TIME"
      case 277: // "RUN_TIME_VAR_TIME_DAY"
      case 278: // "RUN_TIME_VAR_TIME_EPOCH"
      case 279: // "RUN_TIME_VAR_TIME_HOUR"
      case 280: // "RUN_TIME_VAR_TIME_MIN"
      case 281: // "RUN_TIME_VAR_TIME_MON"
      case 282: // "RUN_TIME_VAR_TIME_SEC"
      case 283: // "RUN_TIME_VAR_TIME_WDAY"
      case 284: // "RUN_TIME_VAR_TIME_YEAR"
      case 285: // "VARIABLE"
      case 286: // "Dictionary element"
      case 287: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 294: // op
      case 295: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 299: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 300: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 297: // variables
      case 298: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 292: // actions
      case 293: // actions_may_quoted
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
      case 93: // "Accuracy"
      case 94: // "Allow"
      case 95: // "Append"
      case 96: // "AuditLog"
      case 97: // "Block"
      case 98: // "Capture"
      case 99: // "Chain"
      case 100: // "ACTION_CTL_AUDIT_ENGINE"
      case 101: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 102: // "ACTION_CTL_BDY_JSON"
      case 103: // "ACTION_CTL_BDY_XML"
      case 104: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 105: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 106: // "ACTION_CTL_RULE_ENGINE"
      case 107: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 108: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 110: // "Deny"
      case 111: // "DeprecateVar"
      case 112: // "Drop"
      case 113: // "Exec"
      case 114: // "ExpireVar"
      case 115: // "Id"
      case 116: // "InitCol"
      case 117: // "Log"
      case 118: // "LogData"
      case 119: // "Maturity"
      case 120: // "Msg"
      case 121: // "MultiMatch"
      case 122: // "NoAuditLog"
      case 123: // "NoLog"
      case 124: // "Pass"
      case 125: // "Pause"
      case 126: // "Phase"
      case 127: // "Prepend"
      case 128: // "Proxy"
      case 129: // "Redirect"
      case 130: // "Rev"
      case 131: // "SanatiseArg"
      case 132: // "SanatiseMatched"
      case 133: // "SanatiseMatchedBytes"
      case 134: // "SanatiseRequestHeader"
      case 135: // "SanatiseResponseHeader"
      case 136: // "SetEnv"
      case 137: // "SetSrc"
      case 138: // "SetSid"
      case 139: // "SetUID"
      case 140: // "Severity"
      case 141: // "Skip"
      case 142: // "SkipAfter"
      case 143: // "Status"
      case 144: // "Tag"
      case 145: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 146: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 147: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 148: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 151: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 152: // "ACTION_TRANSFORMATION_LENGTH"
      case 153: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 154: // "ACTION_TRANSFORMATION_MD5"
      case 155: // "ACTION_TRANSFORMATION_NONE"
      case 156: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 158: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 159: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 165: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 167: // "ACTION_TRANSFORMATION_SHA1"
      case 168: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 169: // "ACTION_TRANSFORMATION_TRIM"
      case 170: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 171: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 173: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 174: // "Ver"
      case 175: // "xmlns"
      case 176: // "CONFIG_COMPONENT_SIG"
      case 177: // "CONFIG_DIR_AUDIT_DIR"
      case 178: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 179: // "CONFIG_DIR_AUDIT_ENG"
      case 180: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 181: // "CONFIG_DIR_AUDIT_LOG"
      case 182: // "CONFIG_DIR_AUDIT_LOG2"
      case 183: // "CONFIG_DIR_AUDIT_LOG_P"
      case 184: // "CONFIG_DIR_AUDIT_STS"
      case 185: // "CONFIG_DIR_AUDIT_TPE"
      case 186: // "CONFIG_DIR_DEBUG_LOG"
      case 187: // "CONFIG_DIR_DEBUG_LVL"
      case 188: // "CONFIG_DIR_GEO_DB"
      case 189: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 191: // "CONFIG_DIR_REQ_BODY"
      case 192: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 193: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 195: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 196: // "CONFIG_DIR_RES_BODY"
      case 197: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 199: // "CONFIG_DIR_RULE_ENG"
      case 200: // "CONFIG_DIR_SEC_ACTION"
      case 201: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 202: // "CONFIG_DIR_SEC_MARKER"
      case 203: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 204: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 205: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 206: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 207: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 208: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 209: // "CONFIG_UPLOAD_DIR"
      case 210: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 211: // "CONFIG_UPLOAD_FILE_MODE"
      case 212: // "CONFIG_VALUE_ABORT"
      case 213: // "CONFIG_VALUE_DETC"
      case 214: // "CONFIG_VALUE_HTTPS"
      case 215: // "CONFIG_VALUE_OFF"
      case 216: // "CONFIG_VALUE_ON"
      case 217: // "CONFIG_VALUE_PARALLEL"
      case 218: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 219: // "CONFIG_VALUE_REJECT"
      case 220: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 221: // "CONFIG_VALUE_SERIAL"
      case 222: // "CONFIG_VALUE_WARN"
      case 223: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 224: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 225: // "CONGIG_DIR_SEC_ARG_SEP"
      case 226: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 227: // "CONGIG_DIR_SEC_DATA_DIR"
      case 228: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 229: // "CONGIG_DIR_SEC_TMP_DIR"
      case 230: // "DIRECTIVE"
      case 231: // "DIRECTIVE_SECRULESCRIPT"
      case 232: // "FREE_TEXT"
      case 233: // "OPERATOR"
      case 234: // "OPERATOR_BEGINS_WITH"
      case 235: // "OPERATOR_CONTAINS"
      case 236: // "OPERATOR_CONTAINS_WORD"
      case 237: // "OPERATOR_DETECT_SQLI"
      case 238: // "OPERATOR_DETECT_XSS"
      case 239: // "OPERATOR_ENDS_WITH"
      case 240: // "OPERATOR_EQ"
      case 241: // "OPERATOR_FUZZY_HASH"
      case 242: // "OPERATOR_GE"
      case 243: // "OPERATOR_GEOLOOKUP"
      case 244: // "OPERATOR_GSB_LOOKUP"
      case 245: // "OPERATOR_GT"
      case 246: // "OPERATOR_INSPECT_FILE"
      case 247: // "OPERATOR_IP_MATCH"
      case 248: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 249: // "OPERATOR_LE"
      case 250: // "OPERATOR_LT"
      case 251: // "OPERATOR_PM"
      case 252: // "OPERATOR_PM_FROM_FILE"
      case 253: // "OPERATOR_RBL"
      case 254: // "OPERATOR_RSUB"
      case 255: // "OPERATOR_RX"
      case 256: // "Operator RX (content only)"
      case 257: // "OPERATOR_STR_EQ"
      case 258: // "OPERATOR_STR_MATCH"
      case 259: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 260: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 261: // "OPERATOR_VALIDATE_DTD"
      case 262: // "OPERATOR_VALIDATE_HASH"
      case 263: // "OPERATOR_VALIDATE_SCHEMA"
      case 264: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 265: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 266: // "OPERATOR_VERIFY_CC"
      case 267: // "OPERATOR_VERIFY_CPF"
      case 268: // "OPERATOR_VERIFY_SSN"
      case 269: // "OPERATOR_WITHIN"
      case 270: // "OP_QUOTE"
      case 271: // "QUOTATION_MARK"
      case 272: // "RUN_TIME_VAR_BLD"
      case 273: // "RUN_TIME_VAR_DUR"
      case 274: // "RUN_TIME_VAR_HSV"
      case 275: // "RUN_TIME_VAR_REMOTE_USER"
      case 276: // "RUN_TIME_VAR_TIME"
      case 277: // "RUN_TIME_VAR_TIME_DAY"
      case 278: // "RUN_TIME_VAR_TIME_EPOCH"
      case 279: // "RUN_TIME_VAR_TIME_HOUR"
      case 280: // "RUN_TIME_VAR_TIME_MIN"
      case 281: // "RUN_TIME_VAR_TIME_MON"
      case 282: // "RUN_TIME_VAR_TIME_SEC"
      case 283: // "RUN_TIME_VAR_TIME_WDAY"
      case 284: // "RUN_TIME_VAR_TIME_YEAR"
      case 285: // "VARIABLE"
      case 286: // "Dictionary element"
      case 287: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 294: // op
      case 295: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 299: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 300: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 297: // variables
      case 298: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 292: // actions
      case 293: // actions_may_quoted
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
    #line 339 "/root/ModSec/ModSecurity-v3/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 859 "seclang-parser.cc" // lalr1.cc:741

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
      case 93: // "Accuracy"
      case 94: // "Allow"
      case 95: // "Append"
      case 96: // "AuditLog"
      case 97: // "Block"
      case 98: // "Capture"
      case 99: // "Chain"
      case 100: // "ACTION_CTL_AUDIT_ENGINE"
      case 101: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 102: // "ACTION_CTL_BDY_JSON"
      case 103: // "ACTION_CTL_BDY_XML"
      case 104: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 105: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 106: // "ACTION_CTL_RULE_ENGINE"
      case 107: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 108: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 110: // "Deny"
      case 111: // "DeprecateVar"
      case 112: // "Drop"
      case 113: // "Exec"
      case 114: // "ExpireVar"
      case 115: // "Id"
      case 116: // "InitCol"
      case 117: // "Log"
      case 118: // "LogData"
      case 119: // "Maturity"
      case 120: // "Msg"
      case 121: // "MultiMatch"
      case 122: // "NoAuditLog"
      case 123: // "NoLog"
      case 124: // "Pass"
      case 125: // "Pause"
      case 126: // "Phase"
      case 127: // "Prepend"
      case 128: // "Proxy"
      case 129: // "Redirect"
      case 130: // "Rev"
      case 131: // "SanatiseArg"
      case 132: // "SanatiseMatched"
      case 133: // "SanatiseMatchedBytes"
      case 134: // "SanatiseRequestHeader"
      case 135: // "SanatiseResponseHeader"
      case 136: // "SetEnv"
      case 137: // "SetSrc"
      case 138: // "SetSid"
      case 139: // "SetUID"
      case 140: // "Severity"
      case 141: // "Skip"
      case 142: // "SkipAfter"
      case 143: // "Status"
      case 144: // "Tag"
      case 145: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 146: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 147: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 148: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 151: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 152: // "ACTION_TRANSFORMATION_LENGTH"
      case 153: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 154: // "ACTION_TRANSFORMATION_MD5"
      case 155: // "ACTION_TRANSFORMATION_NONE"
      case 156: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 158: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 159: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 165: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 167: // "ACTION_TRANSFORMATION_SHA1"
      case 168: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 169: // "ACTION_TRANSFORMATION_TRIM"
      case 170: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 171: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 173: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 174: // "Ver"
      case 175: // "xmlns"
      case 176: // "CONFIG_COMPONENT_SIG"
      case 177: // "CONFIG_DIR_AUDIT_DIR"
      case 178: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 179: // "CONFIG_DIR_AUDIT_ENG"
      case 180: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 181: // "CONFIG_DIR_AUDIT_LOG"
      case 182: // "CONFIG_DIR_AUDIT_LOG2"
      case 183: // "CONFIG_DIR_AUDIT_LOG_P"
      case 184: // "CONFIG_DIR_AUDIT_STS"
      case 185: // "CONFIG_DIR_AUDIT_TPE"
      case 186: // "CONFIG_DIR_DEBUG_LOG"
      case 187: // "CONFIG_DIR_DEBUG_LVL"
      case 188: // "CONFIG_DIR_GEO_DB"
      case 189: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 191: // "CONFIG_DIR_REQ_BODY"
      case 192: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 193: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 195: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 196: // "CONFIG_DIR_RES_BODY"
      case 197: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 199: // "CONFIG_DIR_RULE_ENG"
      case 200: // "CONFIG_DIR_SEC_ACTION"
      case 201: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 202: // "CONFIG_DIR_SEC_MARKER"
      case 203: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 204: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 205: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 206: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 207: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 208: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 209: // "CONFIG_UPLOAD_DIR"
      case 210: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 211: // "CONFIG_UPLOAD_FILE_MODE"
      case 212: // "CONFIG_VALUE_ABORT"
      case 213: // "CONFIG_VALUE_DETC"
      case 214: // "CONFIG_VALUE_HTTPS"
      case 215: // "CONFIG_VALUE_OFF"
      case 216: // "CONFIG_VALUE_ON"
      case 217: // "CONFIG_VALUE_PARALLEL"
      case 218: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 219: // "CONFIG_VALUE_REJECT"
      case 220: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 221: // "CONFIG_VALUE_SERIAL"
      case 222: // "CONFIG_VALUE_WARN"
      case 223: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 224: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 225: // "CONGIG_DIR_SEC_ARG_SEP"
      case 226: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 227: // "CONGIG_DIR_SEC_DATA_DIR"
      case 228: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 229: // "CONGIG_DIR_SEC_TMP_DIR"
      case 230: // "DIRECTIVE"
      case 231: // "DIRECTIVE_SECRULESCRIPT"
      case 232: // "FREE_TEXT"
      case 233: // "OPERATOR"
      case 234: // "OPERATOR_BEGINS_WITH"
      case 235: // "OPERATOR_CONTAINS"
      case 236: // "OPERATOR_CONTAINS_WORD"
      case 237: // "OPERATOR_DETECT_SQLI"
      case 238: // "OPERATOR_DETECT_XSS"
      case 239: // "OPERATOR_ENDS_WITH"
      case 240: // "OPERATOR_EQ"
      case 241: // "OPERATOR_FUZZY_HASH"
      case 242: // "OPERATOR_GE"
      case 243: // "OPERATOR_GEOLOOKUP"
      case 244: // "OPERATOR_GSB_LOOKUP"
      case 245: // "OPERATOR_GT"
      case 246: // "OPERATOR_INSPECT_FILE"
      case 247: // "OPERATOR_IP_MATCH"
      case 248: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 249: // "OPERATOR_LE"
      case 250: // "OPERATOR_LT"
      case 251: // "OPERATOR_PM"
      case 252: // "OPERATOR_PM_FROM_FILE"
      case 253: // "OPERATOR_RBL"
      case 254: // "OPERATOR_RSUB"
      case 255: // "OPERATOR_RX"
      case 256: // "Operator RX (content only)"
      case 257: // "OPERATOR_STR_EQ"
      case 258: // "OPERATOR_STR_MATCH"
      case 259: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 260: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 261: // "OPERATOR_VALIDATE_DTD"
      case 262: // "OPERATOR_VALIDATE_HASH"
      case 263: // "OPERATOR_VALIDATE_SCHEMA"
      case 264: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 265: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 266: // "OPERATOR_VERIFY_CC"
      case 267: // "OPERATOR_VERIFY_CPF"
      case 268: // "OPERATOR_VERIFY_SSN"
      case 269: // "OPERATOR_WITHIN"
      case 270: // "OP_QUOTE"
      case 271: // "QUOTATION_MARK"
      case 272: // "RUN_TIME_VAR_BLD"
      case 273: // "RUN_TIME_VAR_DUR"
      case 274: // "RUN_TIME_VAR_HSV"
      case 275: // "RUN_TIME_VAR_REMOTE_USER"
      case 276: // "RUN_TIME_VAR_TIME"
      case 277: // "RUN_TIME_VAR_TIME_DAY"
      case 278: // "RUN_TIME_VAR_TIME_EPOCH"
      case 279: // "RUN_TIME_VAR_TIME_HOUR"
      case 280: // "RUN_TIME_VAR_TIME_MIN"
      case 281: // "RUN_TIME_VAR_TIME_MON"
      case 282: // "RUN_TIME_VAR_TIME_SEC"
      case 283: // "RUN_TIME_VAR_TIME_WDAY"
      case 284: // "RUN_TIME_VAR_TIME_YEAR"
      case 285: // "VARIABLE"
      case 286: // "Dictionary element"
      case 287: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 294: // op
      case 295: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 299: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 300: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 297: // variables
      case 298: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 292: // actions
      case 293: // actions_may_quoted
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
#line 668 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1192 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 681 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1200 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 687 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1208 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 693 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1216 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 697 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1224 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 701 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1232 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 707 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1240 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 713 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 719 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1256 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 725 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1264 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 731 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        relevant_status.pop_back();
        relevant_status.erase(0, 1);
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1275 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 740 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1283 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 744 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1291 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 748 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1299 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 754 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1307 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 758 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1315 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 762 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1324 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 767 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 772 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1342 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 777 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1350 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 781 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1358 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 788 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1366 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 792 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1374 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 799 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1384 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 805 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1395 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 815 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1408 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 824 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1422 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 834 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1435 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 843 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("!" + utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 855 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1456 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 859 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1464 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 863 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1472 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 867 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1480 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 871 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1488 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 875 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 880 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1506 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 885 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1514 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 889 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1522 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 893 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1531 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 898 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1539 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 902 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1547 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 906 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1556 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 911 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1565 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 916 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1574 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 921 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 926 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 930 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 934 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 938 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 942 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1623 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 946 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1631 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 950 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1639 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 954 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1655 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 962 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1663 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 966 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1671 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 974 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 978 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1695 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 982 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 986 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1711 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 990 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1719 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 994 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1727 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 998 "seclang-parser.yy" // lalr1.cc:859
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
#line 1742 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1013 "seclang-parser.yy" // lalr1.cc:859
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
#line 1770 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 1037 "seclang-parser.yy" // lalr1.cc:859
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
#line 1793 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1056 "seclang-parser.yy" // lalr1.cc:859
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
#line 1812 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1071 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1824 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1079 "seclang-parser.yy" // lalr1.cc:859
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
#line 1875 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1126 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1883 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1130 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1891 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1134 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1899 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1138 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1907 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1142 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1915 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1146 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1923 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1150 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1931 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1154 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1939 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1158 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1947 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1162 "seclang-parser.yy" // lalr1.cc:859
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
#line 1964 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1176 "seclang-parser.yy" // lalr1.cc:859
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
#line 1980 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1188 "seclang-parser.yy" // lalr1.cc:859
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
#line 2003 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1208 "seclang-parser.yy" // lalr1.cc:859
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
#line 2034 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1236 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2043 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1241 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2052 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1246 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1251 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2070 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1256 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2078 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1260 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2086 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1264 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2094 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1268 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2102 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1272 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2110 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1276 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2118 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1282 "seclang-parser.yy" // lalr1.cc:859
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
#line 2133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1293 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2141 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1297 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2149 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 1307 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2156 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1313 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2164 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1317 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2172 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1324 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2181 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1329 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1335 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1341 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2211 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1347 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1354 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1364 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1368 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1372 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2257 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1376 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2265 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1380 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2273 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1384 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2281 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1388 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2289 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1392 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2297 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1396 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2305 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1400 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2313 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1404 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2321 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1408 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2329 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1412 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1416 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2345 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1420 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2353 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1424 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2361 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1428 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2369 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1432 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2377 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1436 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2385 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1440 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1444 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2401 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1448 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2409 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1452 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2417 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1456 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2425 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1460 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2433 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1464 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2441 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1468 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2449 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1472 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1476 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2465 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1480 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2473 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1484 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2481 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1488 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2489 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1492 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2497 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1496 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2505 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1500 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2513 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1504 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2521 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1508 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2529 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1512 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2537 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1516 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2545 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2553 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1524 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2561 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1528 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1532 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2577 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1536 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2585 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1540 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2593 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1544 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2601 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1548 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2609 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1552 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1556 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2625 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1560 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2633 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1564 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2641 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1568 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2649 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1572 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2657 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1576 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1580 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2673 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1584 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2681 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1588 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2689 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1592 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2697 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1596 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2705 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1600 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2713 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1605 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2721 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1609 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2729 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1613 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 2737 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1618 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2745 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1622 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2753 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1626 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 2761 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1631 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2769 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1635 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2777 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1639 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 2785 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1644 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2793 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1648 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2801 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1652 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 2809 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1659 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames());
      }
#line 2817 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1663 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames());
      }
#line 2825 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1667 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames());
      }
#line 2833 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1671 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames());
      }
#line 2841 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1675 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 2849 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1679 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames());
      }
#line 2857 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1683 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 2865 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1687 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 2873 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1691 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 2881 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1695 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 2889 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1699 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 2897 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1703 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 2905 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1707 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 2913 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1711 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 2921 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1715 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 2929 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1719 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 2937 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1723 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 2945 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1727 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 2953 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1731 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 2961 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1735 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 2969 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1739 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 2977 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1743 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 2985 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1747 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 2993 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1751 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3001 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1755 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3009 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1759 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3017 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1763 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3025 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1767 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3033 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1771 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3041 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1775 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3049 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1779 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3057 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1783 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3065 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1787 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3073 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1791 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3081 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1795 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3089 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1799 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3097 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1803 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3105 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1807 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3113 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1811 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3121 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1815 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3129 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3137 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1823 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3145 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1827 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource());
      }
#line 3153 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1831 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3161 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1835 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3169 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1839 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3177 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 1843 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3185 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 1847 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3193 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 1851 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3201 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 1855 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3209 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 1859 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3217 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 1863 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 1867 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3233 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 1871 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3241 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 1875 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3249 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 1879 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3260 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 1887 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 1894 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3282 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 1901 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 1908 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3304 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 1915 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3315 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 1922 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3326 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 1929 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 1936 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3348 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 1943 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 1950 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3370 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 1957 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3381 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 1964 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3392 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 1974 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3400 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 1978 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3408 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 1982 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3416 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 1986 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3424 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 1990 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3432 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 1994 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3440 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 1998 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2002 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2007 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3466 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2012 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3475 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2017 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3483 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2021 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3491 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2025 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3499 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2029 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3508 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2034 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3517 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2039 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3525 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2043 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3533 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2047 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3542 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2052 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2057 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlRuleEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3560 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2062 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3568 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2066 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3576 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2070 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3584 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2074 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3592 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2078 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3600 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2082 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3609 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2087 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 3617 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2091 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3626 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2096 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3634 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2100 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3642 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2104 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3650 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2108 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3658 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2112 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3666 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2116 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 3674 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2120 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 3682 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2124 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3690 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2128 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 3698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2132 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 3706 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2136 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 3714 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2140 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 3722 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2144 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 3730 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2148 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 3738 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2152 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 3746 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2156 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 3754 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2160 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 3762 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2164 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 3770 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2168 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 3778 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2172 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 3786 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2176 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3794 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2180 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3802 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2184 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetRSC", yystack_[1].location);
      }
#line 3810 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2188 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3818 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2192 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3826 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2196 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3834 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2200 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3842 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2204 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3850 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2208 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3858 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2212 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3866 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2216 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3874 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2220 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3882 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2224 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3890 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2228 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3898 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2232 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3906 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2236 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3914 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2240 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3922 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2244 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3930 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2248 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3938 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2252 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 3946 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2256 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3954 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2260 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 3962 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2264 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 3970 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2268 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 3978 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2272 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 3986 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2276 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 3994 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2280 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4002 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2284 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4010 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2288 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4018 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2292 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4026 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2296 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4034 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2300 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4042 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2304 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4050 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2308 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4058 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2312 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4066 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2316 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4074 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2320 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4082 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2324 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4090 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2328 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4098 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2332 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4106 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2336 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4114 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2340 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4122 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2344 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4130 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2348 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4138 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2352 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4146 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2356 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4154 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4158 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -280;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     367,  -280,  -280,  -280,  -280,  -205,  -280,  -280,  -280,  -280,
    -280,  -208,  -280,  -280,  -280,  -280,  -280,  -211,  -280,  -280,
      61,  -280,    66,  -280,   119,  -199,   652,   652,  -280,  -280,
    -280,  -210,  -280,    68,   124,  -280,  -280,  -280,   127,  -280,
    -280,  -280,  -280,  -280,  -280,    14,   652,   423,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,   294,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,    51,  -280,  -280,  -280,
     129,   131,    59,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,   831,  -280,    15,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,   455,   455,  -279,    62,    64,
      67,    69,    71,    73,    75,    77,    79,    95,    97,   101,
     103,   105,   107,  -280,  -280,   109,  -280,  -280,  -280,  -280,
     111,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
     113,   115,   117,   120,   122,   125,    96,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
     594,   177,  -280,  -280,  -280,  -280,  -186,   187,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,    -2,   831,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
      -1,    65,    31,    36,    41,  -280,  -280,   153,   173,   181,
     182,  -280,   183,   184,   185,   186,   188,   189,   190,   192,
     193,   194,   195,   196,  -280,   197,   198,  -280,   199,   200,
     201,   202,  -280,  -280,   203,   204,   205,   206,   652,  -280,
     178,  -280,   207,   208,   209,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
     455,   455,  -280,  -280,  -280,  -280,  -280,  -280
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
      93,    82,    81,    94,    95,    78,    76,    77,     0,   258,
     259,   260,   261,   262,   263,   264,     0,   268,   269,   270,
       0,     0,     0,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   289,   288,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   316,   317,   318,   319,
     320,   327,   337,   343,   330,   331,   341,   342,   347,   332,
     329,   336,   346,   345,   325,   324,   323,   350,   349,   340,
     338,   351,   339,   328,   326,   344,   333,   335,   334,   348,
     321,   322,     0,    72,    27,    29,    74,    96,    97,    20,
      19,    25,    24,   101,   102,     0,     0,   120,   123,   126,
     129,   132,   135,   138,   141,   144,   147,   150,   153,   156,
     159,   162,   165,   196,   191,   168,   190,   192,   197,   198,
     177,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     219,   218,   222,   221,   220,   223,   225,   224,   226,   193,
     227,   228,   229,   231,   230,   232,   233,   234,   194,   195,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     180,   183,   186,   189,   171,   174,     0,   246,   245,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
       0,   110,   115,    73,     1,     3,     0,   312,   266,   265,
     267,   272,   271,   274,   273,   277,   276,   275,     0,     0,
     117,   116,   118,   119,   121,   122,   124,   125,   127,   128,
     130,   131,   133,   134,   136,   137,   139,   140,   142,   143,
     145,   146,   148,   149,   151,   152,   154,   155,   157,   158,
     160,   161,   163,   164,   166,   167,   175,   176,   178,   179,
     181,   182,   184,   185,   187,   188,   169,   170,   172,   173,
       0,     0,     0,     0,     0,    35,    36,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,    34,     0,     0,
       0,     0,    37,    38,     0,     0,     0,     0,    71,    30,
       0,   311,     0,     0,     0,    26,    28,   111,    33,    31,
      67,    52,    51,    53,    54,    40,    55,    48,    56,    39,
      58,    57,    59,    60,    62,    61,    63,    49,    64,    65,
      66,    41,    42,    43,    44,    45,    46,    47,    50,    70,
       0,     0,   112,   313,   314,   315,   114,   113
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -280,  -280,   305,  -280,   -27,   258,  -280,    88,  -280,  -280,
     336,   170,   163
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,   153,   154,   368,   369,    50,   260,
     261,   262,   155
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     156,   279,   157,   370,    57,    58,    54,   282,   283,    55,
      51,    52,   158,    56,    65,    53,    66,    67,   279,   263,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   371,
     244,   245,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   370,   244,   245,   410,   411,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   380,   244,   245,   268,   269,   381,   375,
     377,   270,   275,   382,   276,   277,   372,   373,   374,    59,
      60,    61,    62,   159,   160,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   378,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   280,   281,    63,    64,   161,
     162,   409,   163,   164,   271,   272,   273,   274,   284,   285,
     286,   287,   265,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,     1,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   302,   303,   304,   305,   383,   266,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   384,   324,   325,   326,   327,
     278,   328,   329,   385,   386,   387,   388,   389,   390,   379,
     391,   392,   393,   264,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   413,
     414,   415,   376,     0,     0,     0,     0,     0,     0,     0,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     412,   244,   245,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,   267,
     416,   417,   330,     0,     0,     0,     0,     0,     0,     0,
      38,    39,    40,    41,    42,    43,    44,    45,    46,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,    45,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
      68,     0,     0,     0,     0,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,   152,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      27,     3,   212,     4,   215,   216,   214,   286,   287,   217,
     215,   216,   222,   221,   213,   220,   215,   216,     3,    46,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,   285,
      86,    87,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,     4,    86,    87,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,   232,    86,    87,   215,   216,   232,   271,
     271,   220,   213,   232,   215,   216,    89,    90,    91,   218,
     219,   215,   216,   215,   216,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   165,   166,   218,   219,   215,
     216,   368,   215,   216,   215,   216,   215,   216,   286,   287,
     286,   287,    47,   286,   287,   286,   287,   286,   287,   286,
     287,   286,   287,   286,   287,   286,   287,     0,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   286,   287,   286,   287,   232,    92,   286,   287,   286,
     287,   286,   287,   286,   287,   286,   287,   286,   287,   286,
     287,   286,   287,   286,   287,   232,   286,   287,   286,   287,
     152,   286,   287,   232,   232,   232,   232,   232,   232,   331,
     232,   232,   232,     0,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     370,    86,    87,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   285,
     410,   411,   246,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,   224,   225,   226,
     227,   228,   229,   230,   231,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      88,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,   271,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   289,   290,   291,
     296,   215,   216,   220,   214,   217,   221,   215,   216,   218,
     219,   215,   216,   218,   219,   213,   215,   216,    88,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   271,   292,   293,   300,   292,   212,   222,   215,
     216,   215,   216,   215,   216,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    86,    87,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     297,   298,   299,   292,     0,   290,    92,   285,   215,   216,
     220,   215,   216,   215,   216,   213,   215,   216,   293,     3,
     299,   299,   286,   287,   286,   287,   286,   287,   286,   287,
     286,   287,   286,   287,   286,   287,   286,   287,   286,   287,
     286,   287,   286,   287,   286,   287,   286,   287,   286,   287,
     286,   287,   286,   287,   286,   287,   286,   287,   286,   287,
     286,   287,   286,   287,   286,   287,   286,   287,   286,   287,
     298,    92,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   294,   295,
       4,   285,    89,    90,    91,   271,   300,   271,   256,   295,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   292,
       6,     7,   299,   232,   232,   232,   299,   299
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   288,   289,   289,   289,   290,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     294,   294,   294,   294,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     297,   297,   298,   298,   298,   298,   298,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300
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
       1,     3,     3,     4,     4,     1,     2,     2,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     4,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "PIPE", "NEW_LINE",
  "VAR_COUNT", "VAR_EXCLUSION", "VARIABLE_ARGS", "VARIABLE_ARGS_POST",
  "VARIABLE_ARGS_GET", "VARIABLE_FILES_SIZES", "VARIABLE_FILES_NAMES",
  "VARIABLE_FILES_TMP_CONTENT", "VARIABLE_MULTIPART_FILENAME",
  "VARIABLE_MULTIPART_NAME", "VARIABLE_MATCHED_VARS_NAMES",
  "VARIABLE_MATCHED_VARS", "VARIABLE_FILES", "VARIABLE_REQUEST_COOKIES",
  "VARIABLE_REQUEST_HEADERS", "VARIABLE_RESPONSE_HEADERS", "VARIABLE_GEO",
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
  "\"NOT\"", "\"Accuracy\"", "\"Allow\"", "\"Append\"", "\"AuditLog\"",
  "\"Block\"", "\"Capture\"", "\"Chain\"", "\"ACTION_CTL_AUDIT_ENGINE\"",
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
  "\"SetSrc\"", "\"SetSid\"", "\"SetUID\"", "\"Severity\"", "\"Skip\"",
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
  "\"VARIABLE\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "variables_may_be_quoted", "var", "act", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   667,   667,   671,   672,   675,   680,   686,   692,   696,
     700,   706,   712,   718,   724,   730,   739,   743,   747,   753,
     757,   761,   766,   771,   776,   780,   787,   791,   798,   804,
     814,   823,   833,   842,   854,   858,   862,   866,   870,   874,
     879,   884,   888,   892,   897,   901,   905,   910,   915,   920,
     925,   929,   933,   937,   941,   945,   949,   953,   957,   961,
     965,   969,   973,   977,   981,   985,   989,   993,   997,  1011,
    1012,  1036,  1055,  1070,  1078,  1125,  1129,  1133,  1137,  1141,
    1145,  1149,  1153,  1157,  1161,  1175,  1187,  1207,  1235,  1240,
    1245,  1250,  1255,  1259,  1263,  1267,  1271,  1275,  1279,  1280,
    1281,  1292,  1296,  1300,  1301,  1302,  1303,  1304,  1305,  1306,
    1312,  1316,  1323,  1328,  1334,  1340,  1346,  1353,  1363,  1367,
    1371,  1375,  1379,  1383,  1387,  1391,  1395,  1399,  1403,  1407,
    1411,  1415,  1419,  1423,  1427,  1431,  1435,  1439,  1443,  1447,
    1451,  1455,  1459,  1463,  1467,  1471,  1475,  1479,  1483,  1487,
    1491,  1495,  1499,  1503,  1507,  1511,  1515,  1519,  1523,  1527,
    1531,  1535,  1539,  1543,  1547,  1551,  1555,  1559,  1563,  1567,
    1571,  1575,  1579,  1583,  1587,  1591,  1595,  1599,  1604,  1608,
    1612,  1617,  1621,  1625,  1630,  1634,  1638,  1643,  1647,  1651,
    1658,  1662,  1666,  1670,  1674,  1678,  1682,  1686,  1690,  1694,
    1698,  1702,  1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,
    1738,  1742,  1746,  1750,  1754,  1758,  1762,  1766,  1770,  1774,
    1778,  1782,  1786,  1790,  1794,  1798,  1802,  1806,  1810,  1814,
    1818,  1822,  1826,  1830,  1834,  1838,  1842,  1846,  1850,  1854,
    1858,  1862,  1866,  1870,  1874,  1878,  1886,  1893,  1900,  1907,
    1914,  1921,  1928,  1935,  1942,  1949,  1956,  1963,  1973,  1977,
    1981,  1985,  1989,  1993,  1997,  2001,  2006,  2011,  2016,  2020,
    2024,  2028,  2033,  2038,  2042,  2046,  2051,  2056,  2061,  2065,
    2069,  2073,  2077,  2081,  2086,  2090,  2095,  2099,  2103,  2107,
    2111,  2115,  2119,  2123,  2127,  2131,  2135,  2139,  2143,  2147,
    2151,  2155,  2159,  2163,  2167,  2171,  2175,  2179,  2183,  2187,
    2191,  2195,  2199,  2203,  2207,  2211,  2215,  2219,  2223,  2227,
    2231,  2235,  2239,  2243,  2247,  2251,  2255,  2259,  2263,  2267,
    2271,  2275,  2279,  2283,  2287,  2291,  2295,  2299,  2303,  2307,
    2311,  2315,  2319,  2323,  2327,  2331,  2335,  2339,  2343,  2347,
    2351,  2355
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
#line 5081 "seclang-parser.cc" // lalr1.cc:1167
#line 2361 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
