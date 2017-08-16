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
#line 348 "seclang-parser.yy" // lalr1.cc:413

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
      case 95: // "Accuracy"
      case 96: // "Allow"
      case 97: // "Append"
      case 98: // "AuditLog"
      case 99: // "Block"
      case 100: // "Capture"
      case 101: // "Chain"
      case 102: // "ACTION_CTL_AUDIT_ENGINE"
      case 103: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 104: // "ACTION_CTL_BDY_JSON"
      case 105: // "ACTION_CTL_BDY_XML"
      case 106: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 107: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 108: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 111: // "Deny"
      case 112: // "DeprecateVar"
      case 113: // "Drop"
      case 114: // "Exec"
      case 115: // "ExpireVar"
      case 116: // "Id"
      case 117: // "InitCol"
      case 118: // "Log"
      case 119: // "LogData"
      case 120: // "Maturity"
      case 121: // "Msg"
      case 122: // "MultiMatch"
      case 123: // "NoAuditLog"
      case 124: // "NoLog"
      case 125: // "Pass"
      case 126: // "Pause"
      case 127: // "Phase"
      case 128: // "Prepend"
      case 129: // "Proxy"
      case 130: // "Redirect"
      case 131: // "Rev"
      case 132: // "SanatiseArg"
      case 133: // "SanatiseMatched"
      case 134: // "SanatiseMatchedBytes"
      case 135: // "SanatiseRequestHeader"
      case 136: // "SanatiseResponseHeader"
      case 137: // "SetEnv"
      case 138: // "SetRsc"
      case 139: // "SetSid"
      case 140: // "SetUID"
      case 141: // "Severity"
      case 142: // "Skip"
      case 143: // "SkipAfter"
      case 144: // "Status"
      case 145: // "Tag"
      case 146: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 147: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 148: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 151: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 152: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_LENGTH"
      case 154: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 155: // "ACTION_TRANSFORMATION_MD5"
      case 156: // "ACTION_TRANSFORMATION_NONE"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 159: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 168: // "ACTION_TRANSFORMATION_SHA1"
      case 169: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 170: // "ACTION_TRANSFORMATION_TRIM"
      case 171: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 174: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 175: // "Ver"
      case 176: // "xmlns"
      case 177: // "CONFIG_COMPONENT_SIG"
      case 178: // "CONFIG_DIR_AUDIT_DIR"
      case 179: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 180: // "CONFIG_DIR_AUDIT_ENG"
      case 181: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 182: // "CONFIG_DIR_AUDIT_LOG"
      case 183: // "CONFIG_DIR_AUDIT_LOG2"
      case 184: // "CONFIG_DIR_AUDIT_LOG_P"
      case 185: // "CONFIG_DIR_AUDIT_STS"
      case 186: // "CONFIG_DIR_AUDIT_TPE"
      case 187: // "CONFIG_DIR_DEBUG_LOG"
      case 188: // "CONFIG_DIR_DEBUG_LVL"
      case 189: // "CONFIG_DIR_GEO_DB"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 191: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 192: // "CONFIG_DIR_REQ_BODY"
      case 193: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 195: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 196: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 197: // "CONFIG_DIR_RES_BODY"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 199: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 200: // "CONFIG_DIR_RULE_ENG"
      case 201: // "CONFIG_DIR_SEC_ACTION"
      case 202: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 203: // "CONFIG_DIR_SEC_MARKER"
      case 204: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 205: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 206: // "CONFIG_SEC_HTTP_BLKEY"
      case 207: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 208: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 209: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 210: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 211: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 212: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 213: // "CONFIG_UPLOAD_DIR"
      case 214: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 215: // "CONFIG_UPLOAD_FILE_MODE"
      case 216: // "CONFIG_VALUE_ABORT"
      case 217: // "CONFIG_VALUE_DETC"
      case 218: // "CONFIG_VALUE_HTTPS"
      case 219: // "CONFIG_VALUE_OFF"
      case 220: // "CONFIG_VALUE_ON"
      case 221: // "CONFIG_VALUE_PARALLEL"
      case 222: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 223: // "CONFIG_VALUE_REJECT"
      case 224: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 225: // "CONFIG_VALUE_SERIAL"
      case 226: // "CONFIG_VALUE_WARN"
      case 227: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 228: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 229: // "CONGIG_DIR_SEC_ARG_SEP"
      case 230: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 231: // "CONGIG_DIR_SEC_DATA_DIR"
      case 232: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 233: // "CONGIG_DIR_SEC_TMP_DIR"
      case 234: // "DIRECTIVE"
      case 235: // "DIRECTIVE_SECRULESCRIPT"
      case 236: // "FREE_TEXT"
      case 237: // "OPERATOR"
      case 238: // "OPERATOR_BEGINS_WITH"
      case 239: // "OPERATOR_CONTAINS"
      case 240: // "OPERATOR_CONTAINS_WORD"
      case 241: // "OPERATOR_DETECT_SQLI"
      case 242: // "OPERATOR_DETECT_XSS"
      case 243: // "OPERATOR_ENDS_WITH"
      case 244: // "OPERATOR_EQ"
      case 245: // "OPERATOR_FUZZY_HASH"
      case 246: // "OPERATOR_GE"
      case 247: // "OPERATOR_GEOLOOKUP"
      case 248: // "OPERATOR_GSB_LOOKUP"
      case 249: // "OPERATOR_GT"
      case 250: // "OPERATOR_INSPECT_FILE"
      case 251: // "OPERATOR_IP_MATCH"
      case 252: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 253: // "OPERATOR_LE"
      case 254: // "OPERATOR_LT"
      case 255: // "OPERATOR_PM"
      case 256: // "OPERATOR_PM_FROM_FILE"
      case 257: // "OPERATOR_RBL"
      case 258: // "OPERATOR_RSUB"
      case 259: // "OPERATOR_RX"
      case 260: // "Operator RX (content only)"
      case 261: // "OPERATOR_STR_EQ"
      case 262: // "OPERATOR_STR_MATCH"
      case 263: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 264: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 265: // "OPERATOR_VALIDATE_DTD"
      case 266: // "OPERATOR_VALIDATE_HASH"
      case 267: // "OPERATOR_VALIDATE_SCHEMA"
      case 268: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 269: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 270: // "OPERATOR_VERIFY_CC"
      case 271: // "OPERATOR_VERIFY_CPF"
      case 272: // "OPERATOR_VERIFY_SSN"
      case 273: // "OPERATOR_WITHIN"
      case 274: // "OP_QUOTE"
      case 275: // "QUOTATION_MARK"
      case 276: // "RUN_TIME_VAR_BLD"
      case 277: // "RUN_TIME_VAR_DUR"
      case 278: // "RUN_TIME_VAR_HSV"
      case 279: // "RUN_TIME_VAR_REMOTE_USER"
      case 280: // "RUN_TIME_VAR_TIME"
      case 281: // "RUN_TIME_VAR_TIME_DAY"
      case 282: // "RUN_TIME_VAR_TIME_EPOCH"
      case 283: // "RUN_TIME_VAR_TIME_HOUR"
      case 284: // "RUN_TIME_VAR_TIME_MIN"
      case 285: // "RUN_TIME_VAR_TIME_MON"
      case 286: // "RUN_TIME_VAR_TIME_SEC"
      case 287: // "RUN_TIME_VAR_TIME_WDAY"
      case 288: // "RUN_TIME_VAR_TIME_YEAR"
      case 289: // "VARIABLE"
      case 290: // "Dictionary element"
      case 291: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 298: // op
      case 299: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 303: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 304: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 301: // variables
      case 302: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 296: // actions
      case 297: // actions_may_quoted
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
      case 95: // "Accuracy"
      case 96: // "Allow"
      case 97: // "Append"
      case 98: // "AuditLog"
      case 99: // "Block"
      case 100: // "Capture"
      case 101: // "Chain"
      case 102: // "ACTION_CTL_AUDIT_ENGINE"
      case 103: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 104: // "ACTION_CTL_BDY_JSON"
      case 105: // "ACTION_CTL_BDY_XML"
      case 106: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 107: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 108: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 111: // "Deny"
      case 112: // "DeprecateVar"
      case 113: // "Drop"
      case 114: // "Exec"
      case 115: // "ExpireVar"
      case 116: // "Id"
      case 117: // "InitCol"
      case 118: // "Log"
      case 119: // "LogData"
      case 120: // "Maturity"
      case 121: // "Msg"
      case 122: // "MultiMatch"
      case 123: // "NoAuditLog"
      case 124: // "NoLog"
      case 125: // "Pass"
      case 126: // "Pause"
      case 127: // "Phase"
      case 128: // "Prepend"
      case 129: // "Proxy"
      case 130: // "Redirect"
      case 131: // "Rev"
      case 132: // "SanatiseArg"
      case 133: // "SanatiseMatched"
      case 134: // "SanatiseMatchedBytes"
      case 135: // "SanatiseRequestHeader"
      case 136: // "SanatiseResponseHeader"
      case 137: // "SetEnv"
      case 138: // "SetRsc"
      case 139: // "SetSid"
      case 140: // "SetUID"
      case 141: // "Severity"
      case 142: // "Skip"
      case 143: // "SkipAfter"
      case 144: // "Status"
      case 145: // "Tag"
      case 146: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 147: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 148: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 151: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 152: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_LENGTH"
      case 154: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 155: // "ACTION_TRANSFORMATION_MD5"
      case 156: // "ACTION_TRANSFORMATION_NONE"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 159: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 168: // "ACTION_TRANSFORMATION_SHA1"
      case 169: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 170: // "ACTION_TRANSFORMATION_TRIM"
      case 171: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 174: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 175: // "Ver"
      case 176: // "xmlns"
      case 177: // "CONFIG_COMPONENT_SIG"
      case 178: // "CONFIG_DIR_AUDIT_DIR"
      case 179: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 180: // "CONFIG_DIR_AUDIT_ENG"
      case 181: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 182: // "CONFIG_DIR_AUDIT_LOG"
      case 183: // "CONFIG_DIR_AUDIT_LOG2"
      case 184: // "CONFIG_DIR_AUDIT_LOG_P"
      case 185: // "CONFIG_DIR_AUDIT_STS"
      case 186: // "CONFIG_DIR_AUDIT_TPE"
      case 187: // "CONFIG_DIR_DEBUG_LOG"
      case 188: // "CONFIG_DIR_DEBUG_LVL"
      case 189: // "CONFIG_DIR_GEO_DB"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 191: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 192: // "CONFIG_DIR_REQ_BODY"
      case 193: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 195: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 196: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 197: // "CONFIG_DIR_RES_BODY"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 199: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 200: // "CONFIG_DIR_RULE_ENG"
      case 201: // "CONFIG_DIR_SEC_ACTION"
      case 202: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 203: // "CONFIG_DIR_SEC_MARKER"
      case 204: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 205: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 206: // "CONFIG_SEC_HTTP_BLKEY"
      case 207: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 208: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 209: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 210: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 211: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 212: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 213: // "CONFIG_UPLOAD_DIR"
      case 214: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 215: // "CONFIG_UPLOAD_FILE_MODE"
      case 216: // "CONFIG_VALUE_ABORT"
      case 217: // "CONFIG_VALUE_DETC"
      case 218: // "CONFIG_VALUE_HTTPS"
      case 219: // "CONFIG_VALUE_OFF"
      case 220: // "CONFIG_VALUE_ON"
      case 221: // "CONFIG_VALUE_PARALLEL"
      case 222: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 223: // "CONFIG_VALUE_REJECT"
      case 224: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 225: // "CONFIG_VALUE_SERIAL"
      case 226: // "CONFIG_VALUE_WARN"
      case 227: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 228: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 229: // "CONGIG_DIR_SEC_ARG_SEP"
      case 230: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 231: // "CONGIG_DIR_SEC_DATA_DIR"
      case 232: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 233: // "CONGIG_DIR_SEC_TMP_DIR"
      case 234: // "DIRECTIVE"
      case 235: // "DIRECTIVE_SECRULESCRIPT"
      case 236: // "FREE_TEXT"
      case 237: // "OPERATOR"
      case 238: // "OPERATOR_BEGINS_WITH"
      case 239: // "OPERATOR_CONTAINS"
      case 240: // "OPERATOR_CONTAINS_WORD"
      case 241: // "OPERATOR_DETECT_SQLI"
      case 242: // "OPERATOR_DETECT_XSS"
      case 243: // "OPERATOR_ENDS_WITH"
      case 244: // "OPERATOR_EQ"
      case 245: // "OPERATOR_FUZZY_HASH"
      case 246: // "OPERATOR_GE"
      case 247: // "OPERATOR_GEOLOOKUP"
      case 248: // "OPERATOR_GSB_LOOKUP"
      case 249: // "OPERATOR_GT"
      case 250: // "OPERATOR_INSPECT_FILE"
      case 251: // "OPERATOR_IP_MATCH"
      case 252: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 253: // "OPERATOR_LE"
      case 254: // "OPERATOR_LT"
      case 255: // "OPERATOR_PM"
      case 256: // "OPERATOR_PM_FROM_FILE"
      case 257: // "OPERATOR_RBL"
      case 258: // "OPERATOR_RSUB"
      case 259: // "OPERATOR_RX"
      case 260: // "Operator RX (content only)"
      case 261: // "OPERATOR_STR_EQ"
      case 262: // "OPERATOR_STR_MATCH"
      case 263: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 264: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 265: // "OPERATOR_VALIDATE_DTD"
      case 266: // "OPERATOR_VALIDATE_HASH"
      case 267: // "OPERATOR_VALIDATE_SCHEMA"
      case 268: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 269: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 270: // "OPERATOR_VERIFY_CC"
      case 271: // "OPERATOR_VERIFY_CPF"
      case 272: // "OPERATOR_VERIFY_SSN"
      case 273: // "OPERATOR_WITHIN"
      case 274: // "OP_QUOTE"
      case 275: // "QUOTATION_MARK"
      case 276: // "RUN_TIME_VAR_BLD"
      case 277: // "RUN_TIME_VAR_DUR"
      case 278: // "RUN_TIME_VAR_HSV"
      case 279: // "RUN_TIME_VAR_REMOTE_USER"
      case 280: // "RUN_TIME_VAR_TIME"
      case 281: // "RUN_TIME_VAR_TIME_DAY"
      case 282: // "RUN_TIME_VAR_TIME_EPOCH"
      case 283: // "RUN_TIME_VAR_TIME_HOUR"
      case 284: // "RUN_TIME_VAR_TIME_MIN"
      case 285: // "RUN_TIME_VAR_TIME_MON"
      case 286: // "RUN_TIME_VAR_TIME_SEC"
      case 287: // "RUN_TIME_VAR_TIME_WDAY"
      case 288: // "RUN_TIME_VAR_TIME_YEAR"
      case 289: // "VARIABLE"
      case 290: // "Dictionary element"
      case 291: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 298: // op
      case 299: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 303: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 304: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 301: // variables
      case 302: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 296: // actions
      case 297: // actions_may_quoted
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
    #line 341 "/home/zimmerle/core-trustwave/ModSecurity/src/parser/seclang-parser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 863 "seclang-parser.cc" // lalr1.cc:741

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
      case 95: // "Accuracy"
      case 96: // "Allow"
      case 97: // "Append"
      case 98: // "AuditLog"
      case 99: // "Block"
      case 100: // "Capture"
      case 101: // "Chain"
      case 102: // "ACTION_CTL_AUDIT_ENGINE"
      case 103: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 104: // "ACTION_CTL_BDY_JSON"
      case 105: // "ACTION_CTL_BDY_XML"
      case 106: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 107: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 108: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 109: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 111: // "Deny"
      case 112: // "DeprecateVar"
      case 113: // "Drop"
      case 114: // "Exec"
      case 115: // "ExpireVar"
      case 116: // "Id"
      case 117: // "InitCol"
      case 118: // "Log"
      case 119: // "LogData"
      case 120: // "Maturity"
      case 121: // "Msg"
      case 122: // "MultiMatch"
      case 123: // "NoAuditLog"
      case 124: // "NoLog"
      case 125: // "Pass"
      case 126: // "Pause"
      case 127: // "Phase"
      case 128: // "Prepend"
      case 129: // "Proxy"
      case 130: // "Redirect"
      case 131: // "Rev"
      case 132: // "SanatiseArg"
      case 133: // "SanatiseMatched"
      case 134: // "SanatiseMatchedBytes"
      case 135: // "SanatiseRequestHeader"
      case 136: // "SanatiseResponseHeader"
      case 137: // "SetEnv"
      case 138: // "SetRsc"
      case 139: // "SetSid"
      case 140: // "SetUID"
      case 141: // "Severity"
      case 142: // "Skip"
      case 143: // "SkipAfter"
      case 144: // "Status"
      case 145: // "Tag"
      case 146: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 147: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 148: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 149: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 151: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 152: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 153: // "ACTION_TRANSFORMATION_LENGTH"
      case 154: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 155: // "ACTION_TRANSFORMATION_MD5"
      case 156: // "ACTION_TRANSFORMATION_NONE"
      case 157: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 159: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 160: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 166: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 168: // "ACTION_TRANSFORMATION_SHA1"
      case 169: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 170: // "ACTION_TRANSFORMATION_TRIM"
      case 171: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 172: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 174: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 175: // "Ver"
      case 176: // "xmlns"
      case 177: // "CONFIG_COMPONENT_SIG"
      case 178: // "CONFIG_DIR_AUDIT_DIR"
      case 179: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 180: // "CONFIG_DIR_AUDIT_ENG"
      case 181: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 182: // "CONFIG_DIR_AUDIT_LOG"
      case 183: // "CONFIG_DIR_AUDIT_LOG2"
      case 184: // "CONFIG_DIR_AUDIT_LOG_P"
      case 185: // "CONFIG_DIR_AUDIT_STS"
      case 186: // "CONFIG_DIR_AUDIT_TPE"
      case 187: // "CONFIG_DIR_DEBUG_LOG"
      case 188: // "CONFIG_DIR_DEBUG_LVL"
      case 189: // "CONFIG_DIR_GEO_DB"
      case 190: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 191: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 192: // "CONFIG_DIR_REQ_BODY"
      case 193: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 194: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 195: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 196: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 197: // "CONFIG_DIR_RES_BODY"
      case 198: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 199: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 200: // "CONFIG_DIR_RULE_ENG"
      case 201: // "CONFIG_DIR_SEC_ACTION"
      case 202: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 203: // "CONFIG_DIR_SEC_MARKER"
      case 204: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 205: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 206: // "CONFIG_SEC_HTTP_BLKEY"
      case 207: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 208: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 209: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 210: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 211: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 212: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 213: // "CONFIG_UPLOAD_DIR"
      case 214: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 215: // "CONFIG_UPLOAD_FILE_MODE"
      case 216: // "CONFIG_VALUE_ABORT"
      case 217: // "CONFIG_VALUE_DETC"
      case 218: // "CONFIG_VALUE_HTTPS"
      case 219: // "CONFIG_VALUE_OFF"
      case 220: // "CONFIG_VALUE_ON"
      case 221: // "CONFIG_VALUE_PARALLEL"
      case 222: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 223: // "CONFIG_VALUE_REJECT"
      case 224: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 225: // "CONFIG_VALUE_SERIAL"
      case 226: // "CONFIG_VALUE_WARN"
      case 227: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 228: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 229: // "CONGIG_DIR_SEC_ARG_SEP"
      case 230: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 231: // "CONGIG_DIR_SEC_DATA_DIR"
      case 232: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 233: // "CONGIG_DIR_SEC_TMP_DIR"
      case 234: // "DIRECTIVE"
      case 235: // "DIRECTIVE_SECRULESCRIPT"
      case 236: // "FREE_TEXT"
      case 237: // "OPERATOR"
      case 238: // "OPERATOR_BEGINS_WITH"
      case 239: // "OPERATOR_CONTAINS"
      case 240: // "OPERATOR_CONTAINS_WORD"
      case 241: // "OPERATOR_DETECT_SQLI"
      case 242: // "OPERATOR_DETECT_XSS"
      case 243: // "OPERATOR_ENDS_WITH"
      case 244: // "OPERATOR_EQ"
      case 245: // "OPERATOR_FUZZY_HASH"
      case 246: // "OPERATOR_GE"
      case 247: // "OPERATOR_GEOLOOKUP"
      case 248: // "OPERATOR_GSB_LOOKUP"
      case 249: // "OPERATOR_GT"
      case 250: // "OPERATOR_INSPECT_FILE"
      case 251: // "OPERATOR_IP_MATCH"
      case 252: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 253: // "OPERATOR_LE"
      case 254: // "OPERATOR_LT"
      case 255: // "OPERATOR_PM"
      case 256: // "OPERATOR_PM_FROM_FILE"
      case 257: // "OPERATOR_RBL"
      case 258: // "OPERATOR_RSUB"
      case 259: // "OPERATOR_RX"
      case 260: // "Operator RX (content only)"
      case 261: // "OPERATOR_STR_EQ"
      case 262: // "OPERATOR_STR_MATCH"
      case 263: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 264: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 265: // "OPERATOR_VALIDATE_DTD"
      case 266: // "OPERATOR_VALIDATE_HASH"
      case 267: // "OPERATOR_VALIDATE_SCHEMA"
      case 268: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 269: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 270: // "OPERATOR_VERIFY_CC"
      case 271: // "OPERATOR_VERIFY_CPF"
      case 272: // "OPERATOR_VERIFY_SSN"
      case 273: // "OPERATOR_WITHIN"
      case 274: // "OP_QUOTE"
      case 275: // "QUOTATION_MARK"
      case 276: // "RUN_TIME_VAR_BLD"
      case 277: // "RUN_TIME_VAR_DUR"
      case 278: // "RUN_TIME_VAR_HSV"
      case 279: // "RUN_TIME_VAR_REMOTE_USER"
      case 280: // "RUN_TIME_VAR_TIME"
      case 281: // "RUN_TIME_VAR_TIME_DAY"
      case 282: // "RUN_TIME_VAR_TIME_EPOCH"
      case 283: // "RUN_TIME_VAR_TIME_HOUR"
      case 284: // "RUN_TIME_VAR_TIME_MIN"
      case 285: // "RUN_TIME_VAR_TIME_MON"
      case 286: // "RUN_TIME_VAR_TIME_SEC"
      case 287: // "RUN_TIME_VAR_TIME_WDAY"
      case 288: // "RUN_TIME_VAR_TIME_YEAR"
      case 289: // "VARIABLE"
      case 290: // "Dictionary element"
      case 291: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 298: // op
      case 299: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 303: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 304: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 301: // variables
      case 302: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 296: // actions
      case 297: // actions_may_quoted
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
#line 675 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1198 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 688 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1206 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 694 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1214 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 700 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 704 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1230 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 708 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1238 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 714 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1246 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 720 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1254 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 726 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1262 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 732 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1270 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 738 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 745 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 749 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 753 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 759 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 763 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 767 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1328 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 772 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 777 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1346 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 782 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1354 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 786 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1362 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 793 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1370 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 797 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1378 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 804 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1388 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 810 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 820 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1412 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 829 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1426 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 839 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 848 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("!" + utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1452 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 860 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1460 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 864 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1468 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 868 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1476 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 872 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1484 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 876 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1492 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 880 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1501 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 885 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 890 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 894 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 898 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 903 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1543 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 907 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 911 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1560 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 916 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 921 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1578 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 926 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1587 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 931 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1595 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 935 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1603 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 939 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1611 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 943 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1619 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 947 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1627 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 951 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 955 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1643 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 959 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1651 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 963 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1659 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 967 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1667 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 971 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1675 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 975 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1683 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 979 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1691 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 983 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1699 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 987 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 991 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1715 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 995 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1723 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 999 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1731 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1003 "seclang-parser.yy" // lalr1.cc:859
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
#line 1746 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1018 "seclang-parser.yy" // lalr1.cc:859
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
#line 1774 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 1042 "seclang-parser.yy" // lalr1.cc:859
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
#line 1797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1061 "seclang-parser.yy" // lalr1.cc:859
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
#line 1816 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1076 "seclang-parser.yy" // lalr1.cc:859
    {
        /*

        TODO: implement the SecRuleScript directive

        */
      }
#line 1828 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1084 "seclang-parser.yy" // lalr1.cc:859
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
#line 1879 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1131 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1135 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1895 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1139 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1903 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1143 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1911 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1147 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1919 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1151 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1927 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1155 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1935 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1159 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1943 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1163 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1951 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1167 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1960 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1172 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 1969 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1177 "seclang-parser.yy" // lalr1.cc:859
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
#line 1986 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1190 "seclang-parser.yy" // lalr1.cc:859
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
#line 2003 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1203 "seclang-parser.yy" // lalr1.cc:859
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
#line 2033 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1230 "seclang-parser.yy" // lalr1.cc:859
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
#line 2049 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1242 "seclang-parser.yy" // lalr1.cc:859
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
#line 2072 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1262 "seclang-parser.yy" // lalr1.cc:859
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
#line 2103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1290 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2112 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1295 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2121 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1300 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2130 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1305 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2139 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1310 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2147 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1314 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2155 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1318 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2163 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1322 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2171 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1326 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2179 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1330 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2187 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1336 "seclang-parser.yy" // lalr1.cc:859
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
#line 2202 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1347 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2210 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1351 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2218 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1361 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2225 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 1364 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2234 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 1372 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2242 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 1376 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2250 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 1383 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2259 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 1388 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 1394 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1400 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2289 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1406 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2300 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1413 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1423 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1427 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1431 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2335 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1435 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2343 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1439 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1443 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1447 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1451 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1455 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1459 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1463 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1467 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1471 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1475 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2423 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1479 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1483 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1487 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2447 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1491 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1495 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2463 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1499 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2471 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1503 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2479 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1507 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1511 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2495 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1515 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1519 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2511 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1523 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2519 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1527 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1531 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1535 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2543 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1539 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1543 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2559 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1547 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2567 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1551 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1555 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1559 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1563 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1567 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1571 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1575 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2623 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1579 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2631 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1583 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2639 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1587 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1591 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2655 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1595 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2663 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1599 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2671 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1603 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1607 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1611 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2695 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1615 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1619 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2711 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1623 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2719 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1627 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2727 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1631 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2735 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1635 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2743 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1639 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2751 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1643 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2759 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1647 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2767 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1651 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2775 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1655 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2783 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1659 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2791 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1664 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2799 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1668 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2807 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1672 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 2815 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1677 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2823 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1681 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2831 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1685 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 2839 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1690 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2847 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1694 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2855 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1698 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 2863 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1703 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2871 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1707 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2879 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1711 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 2887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1718 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames());
      }
#line 2895 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1722 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames());
      }
#line 2903 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1726 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames());
      }
#line 2911 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1730 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames());
      }
#line 2919 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1734 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 2927 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1738 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames());
      }
#line 2935 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1742 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 2943 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1746 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 2951 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1750 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 2959 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1754 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 2967 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1758 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 2975 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1762 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 2983 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1766 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 2991 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1770 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 2999 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1774 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3007 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1778 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3015 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1782 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3023 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1786 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3031 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1790 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3039 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1794 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3047 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1798 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1802 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3063 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1806 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1810 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1814 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1818 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1822 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1826 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1830 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1834 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3127 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1838 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3135 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1842 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3143 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1846 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1850 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1854 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3167 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1858 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3175 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1862 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1866 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1870 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1874 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3207 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1878 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 1882 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 1886 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource());
      }
#line 3231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 1890 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 1894 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 1898 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 1902 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 1906 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 1910 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 1914 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 1918 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 1922 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 1926 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 1930 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 1934 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 1938 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3338 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 1946 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3349 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 1953 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3360 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 1960 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3371 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 1967 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3382 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 1974 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3393 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 1981 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3404 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 1988 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 1995 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3426 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2002 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3437 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2009 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3448 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2016 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3459 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2023 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2033 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2037 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2041 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2045 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2049 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2053 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3518 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2057 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3526 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2061 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2066 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3544 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2071 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3553 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2076 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3561 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2080 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2084 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3577 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2088 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3586 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2093 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3595 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2098 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3603 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2102 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3611 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2106 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3619 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2110 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3627 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2114 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3635 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2118 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3643 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2122 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3651 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2126 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3659 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2130 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3667 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2134 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3675 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2138 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3684 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2143 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 3692 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2147 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3701 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2152 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3709 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2156 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3717 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2160 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3725 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2164 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3733 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2168 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3741 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2172 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 3749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2176 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 3757 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2180 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3765 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2184 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 3773 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2188 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 3781 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2192 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 3789 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2196 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 3797 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2200 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 3805 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2204 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 3813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2208 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 3821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2212 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 3829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2216 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 3837 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2220 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 3845 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2224 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 3853 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2228 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 3861 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2232 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3869 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2236 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3877 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2240 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(yystack_[0].value.as< std::string > ()));
      }
#line 3885 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2244 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 3893 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2248 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 3901 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2252 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3909 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2256 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 3917 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2260 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3925 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2264 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3933 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2268 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 3941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2272 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 3949 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2276 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 3957 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2280 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 3965 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2284 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 3973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2288 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 3981 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2292 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 3989 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2296 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 3997 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2300 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4005 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2304 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4013 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2308 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4021 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2312 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4029 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2316 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4037 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2320 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4045 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2324 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2328 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2332 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4069 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2336 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4077 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2340 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4085 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2344 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4093 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2348 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4101 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2352 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4109 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2356 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4117 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2360 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4125 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2364 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2368 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4141 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2372 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4149 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2376 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2380 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2384 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4173 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2388 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4181 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2392 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4189 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2396 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2400 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4205 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2404 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4213 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2408 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2412 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4229 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4233 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -272;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     387,  -272,    63,  -272,  -272,  -272,  -209,  -272,  -272,  -272,
    -272,  -272,  -213,  -272,  -272,  -272,  -272,  -272,    65,  -272,
    -272,  -216,  -272,    68,  -272,   119,  -200,   660,   660,  -272,
    -272,  -272,  -272,  -212,  -272,    16,    16,    70,   124,  -272,
    -272,  -272,   126,  -272,  -272,  -272,  -272,  -272,  -272,    16,
     660,   446,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,   257,    58,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,    50,  -272,  -272,  -272,   128,   132,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,   748,  -272,
       6,  -272,  -272,  -272,  -272,   460,   460,    64,    66,    69,
      71,    73,    75,    77,    79,    81,   102,   104,   106,   108,
     110,   112,   114,  -272,  -272,   116,  -272,  -272,  -272,  -272,
     118,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
     120,   122,   125,   127,   129,   131,    98,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,     8,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
     456,  -272,  -272,  -272,  -271,   189,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,    -2,   748,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,    -3,   180,
      67,  -215,  -134,   -52,  -272,  -272,    30,    35,    40,   117,
    -272,   137,   154,   178,   187,   188,   190,   191,   192,   193,
     194,   195,   196,  -272,   197,   198,  -272,   199,   200,   201,
     202,  -272,  -272,   203,   204,   205,   206,   660,  -272,  -272,
     207,   208,   209,  -272,  -272,  -272,   460,   460,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,    83,     7,     6,     0,    11,    14,    12,
      13,    15,     0,    90,    89,    91,   103,   102,     0,    94,
      92,     0,    93,     0,    95,     0,     0,     0,     0,    75,
     112,   113,   114,     0,    86,     0,     0,     0,     0,    23,
      21,    22,     0,   104,   109,   110,   108,   111,   107,     0,
       0,     0,     4,    69,     5,    85,    84,     9,    10,     8,
      18,    17,    16,    80,    79,    96,    97,    82,    81,    98,
      99,    78,    76,    77,     0,     0,   263,   264,   265,   266,
     267,   268,   269,     0,   273,   274,   275,     0,     0,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   294,
     293,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   321,   322,   323,   324,   325,   332,   342,   348,
     335,   336,   346,   347,   352,   337,   334,   341,   351,   350,
     330,   329,   328,   355,   354,   345,   343,   356,   344,   333,
     331,   349,   338,   340,   339,   353,   326,   327,     0,    72,
      27,    29,    74,   100,   101,     0,     0,   125,   128,   131,
     134,   137,   140,   143,   146,   149,   152,   155,   158,   161,
     164,   167,   170,   201,   196,   173,   195,   197,   202,   203,
     182,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     224,   223,   227,   226,   225,   228,   230,   229,   231,   198,
     232,   233,   234,   236,   235,   237,   238,   239,   199,   200,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     185,   188,   191,   194,   176,   179,     0,   251,   250,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
      87,   115,   120,    88,    20,    19,    25,    24,   105,   106,
       0,    73,     1,     3,     0,   317,   282,   281,   280,   271,
     270,   272,   277,   276,   279,   278,     0,     0,   122,   121,
     123,   124,   126,   127,   129,   130,   132,   133,   135,   136,
     138,   139,   141,   142,   144,   145,   147,   148,   150,   151,
     153,   154,   156,   157,   159,   160,   162,   163,   165,   166,
     168,   169,   171,   172,   180,   181,   183,   184,   186,   187,
     189,   190,   192,   193,   174,   175,   177,   178,     0,     0,
       0,     0,     0,     0,    35,    36,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,     0,     0,    34,     0,     0,     0,
       0,    37,    38,     0,     0,     0,     0,    71,    30,   316,
       0,     0,     0,    26,    28,   116,     0,     0,   117,    33,
      31,    67,    52,    51,    53,    54,    40,    55,    48,    56,
      39,    58,    57,    59,    60,    62,    61,    63,    49,    64,
      65,    66,    41,    42,    43,    44,    45,    46,    47,    50,
      70,   318,   319,   320,   119,   118
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -272,  -272,   235,  -272,   -28,   267,  -272,    18,  -272,   -33,
     304,   223,   160
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    51,    52,    53,   159,   160,   377,   378,    54,   260,
     261,   262,   161
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     162,   287,   339,   263,   163,    60,    65,    66,    61,   287,
      57,    58,    62,   339,   164,    59,   270,    71,   379,    72,
      73,   391,   271,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   392,   244,   245,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   393,   244,   245,   386,   387,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   394,   244,   245,   279,
     280,   395,   385,   383,   281,   276,   396,   277,   278,   380,
     381,   382,    55,    56,    63,    64,   273,    67,    68,   264,
     265,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   389,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,    69,    70,   266,   267,   268,   269,   282,   283,   420,
     274,   284,   285,   397,   290,   291,   292,   293,   390,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   398,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,     1,   288,   289,
     399,     2,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   400,   330,   331,   332,   333,   334,
     335,   336,   337,   401,   402,   286,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   421,   422,   423,   272,   384,     0,     0,
       2,     0,     0,     0,     0,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   275,   244,   245,   340,
     338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,     0,   424,
     425,     0,     0,     0,    42,    43,    44,    45,    46,    47,
      48,    49,    50,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
       0,     0,     0,     0,     0,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    74,
       0,     0,     0,     0,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    74,     0,     0,
       0,     0,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      28,     3,     5,    36,   216,   218,   222,   223,   221,     3,
     219,   220,   225,     5,   226,   224,    49,   217,   289,   219,
     220,   236,    50,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   236,    87,    88,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,   236,    87,    88,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,   236,    87,    88,   219,
     220,   236,   275,   275,   224,   217,   236,   219,   220,    90,
      91,    92,   219,   220,   219,   220,    51,   219,   220,   219,
     220,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   222,   223,   219,   220,   219,   220,   219,   220,   377,
      93,   219,   220,   236,   290,   291,   290,   291,   340,   290,
     291,   290,   291,   290,   291,   290,   291,   290,   291,   290,
     291,   290,   291,   236,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,     0,   165,   166,
     236,     4,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   236,   290,   291,   290,   291,   290,
     291,   290,   291,   236,   236,   158,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,     0,   287,    -1,    -1,
       4,    -1,    -1,    -1,    -1,    -1,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,   289,    87,    88,    93,
     246,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   339,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,   386,
     387,    -1,    -1,    -1,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,   229,   230,   231,   232,   233,
     234,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
      -1,    -1,    -1,    -1,    -1,    -1,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,    89,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    89,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   275
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   293,   294,   295,   300,   219,   220,   219,   220,   224,
     218,   221,   225,   219,   220,   222,   223,   219,   220,   222,
     223,   217,   219,   220,    89,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   275,   296,
     297,   304,   296,   216,   226,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    87,    88,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     301,   302,   303,   301,   219,   220,   219,   220,   219,   220,
     301,   296,     0,   294,    93,   289,   217,   219,   220,   219,
     220,   224,   219,   220,   219,   220,   297,     3,   303,   303,
     290,   291,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   290,   291,   290,   291,   290,   291,
     290,   291,   290,   291,   290,   291,   290,   291,   302,     5,
      93,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   298,   299,   289,
      90,    91,    92,   275,   304,   275,     7,     8,   303,   260,
     299,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     296,   236,   236,   236,   303,   303
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   292,   293,   293,   293,   294,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   296,   297,   297,
     298,   298,   298,   298,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   301,   301,   302,   302,   302,
     302,   302,   302,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304
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
       2,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     4,     4,
       1,     2,     2,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     4,     4,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const seclang_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"",
  "\"CONFIG_CONTENT_INJECTION\"", "PIPE", "NEW_LINE", "VAR_COUNT",
  "VAR_EXCLUSION", "VARIABLE_ARGS", "VARIABLE_ARGS_POST",
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
  "\"NOT\"", "\"ACTION_CTL_RULE_ENGINE\"", "\"Accuracy\"", "\"Allow\"",
  "\"Append\"", "\"AuditLog\"", "\"Block\"", "\"Capture\"", "\"Chain\"",
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
  "\"CONFIG_SEC_HTTP_BLKEY\"", "\"CONFIG_SEC_REMOTE_RULES_FAIL_ACTION\"",
  "\"CONFIG_SEC_RULE_REMOVE_BY_ID\"",
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG\"",
  "\"CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID\"",
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
  "\"VARIABLE\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "variables_may_be_quoted", "var", "act", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   674,   674,   678,   679,   682,   687,   693,   699,   703,
     707,   713,   719,   725,   731,   737,   744,   748,   752,   758,
     762,   766,   771,   776,   781,   785,   792,   796,   803,   809,
     819,   828,   838,   847,   859,   863,   867,   871,   875,   879,
     884,   889,   893,   897,   902,   906,   910,   915,   920,   925,
     930,   934,   938,   942,   946,   950,   954,   958,   962,   966,
     970,   974,   978,   982,   986,   990,   994,   998,  1002,  1016,
    1017,  1041,  1060,  1075,  1083,  1130,  1134,  1138,  1142,  1146,
    1150,  1154,  1158,  1162,  1166,  1171,  1176,  1189,  1202,  1229,
    1241,  1261,  1289,  1294,  1299,  1304,  1309,  1313,  1317,  1321,
    1325,  1329,  1333,  1334,  1335,  1346,  1350,  1354,  1355,  1356,
    1357,  1358,  1359,  1360,  1363,  1371,  1375,  1382,  1387,  1393,
    1399,  1405,  1412,  1422,  1426,  1430,  1434,  1438,  1442,  1446,
    1450,  1454,  1458,  1462,  1466,  1470,  1474,  1478,  1482,  1486,
    1490,  1494,  1498,  1502,  1506,  1510,  1514,  1518,  1522,  1526,
    1530,  1534,  1538,  1542,  1546,  1550,  1554,  1558,  1562,  1566,
    1570,  1574,  1578,  1582,  1586,  1590,  1594,  1598,  1602,  1606,
    1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,
    1650,  1654,  1658,  1663,  1667,  1671,  1676,  1680,  1684,  1689,
    1693,  1697,  1702,  1706,  1710,  1717,  1721,  1725,  1729,  1733,
    1737,  1741,  1745,  1749,  1753,  1757,  1761,  1765,  1769,  1773,
    1777,  1781,  1785,  1789,  1793,  1797,  1801,  1805,  1809,  1813,
    1817,  1821,  1825,  1829,  1833,  1837,  1841,  1845,  1849,  1853,
    1857,  1861,  1865,  1869,  1873,  1877,  1881,  1885,  1889,  1893,
    1897,  1901,  1905,  1909,  1913,  1917,  1921,  1925,  1929,  1933,
    1937,  1945,  1952,  1959,  1966,  1973,  1980,  1987,  1994,  2001,
    2008,  2015,  2022,  2032,  2036,  2040,  2044,  2048,  2052,  2056,
    2060,  2065,  2070,  2075,  2079,  2083,  2087,  2092,  2097,  2101,
    2105,  2109,  2113,  2117,  2121,  2125,  2129,  2133,  2137,  2142,
    2146,  2151,  2155,  2159,  2163,  2167,  2171,  2175,  2179,  2183,
    2187,  2191,  2195,  2199,  2203,  2207,  2211,  2215,  2219,  2223,
    2227,  2231,  2235,  2239,  2243,  2247,  2251,  2255,  2259,  2263,
    2267,  2271,  2275,  2279,  2283,  2287,  2291,  2295,  2299,  2303,
    2307,  2311,  2315,  2319,  2323,  2327,  2331,  2335,  2339,  2343,
    2347,  2351,  2355,  2359,  2363,  2367,  2371,  2375,  2379,  2383,
    2387,  2391,  2395,  2399,  2403,  2407,  2411
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
#line 5149 "seclang-parser.cc" // lalr1.cc:1167
#line 2417 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
