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
      case 96: // "Accuracy"
      case 97: // "Allow"
      case 98: // "Append"
      case 99: // "AuditLog"
      case 100: // "Block"
      case 101: // "Capture"
      case 102: // "Chain"
      case 103: // "ACTION_CTL_AUDIT_ENGINE"
      case 104: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 105: // "ACTION_CTL_BDY_JSON"
      case 106: // "ACTION_CTL_BDY_XML"
      case 107: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 108: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 109: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 111: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 112: // "Deny"
      case 113: // "DeprecateVar"
      case 114: // "Drop"
      case 115: // "Exec"
      case 116: // "ExpireVar"
      case 117: // "Id"
      case 118: // "InitCol"
      case 119: // "Log"
      case 120: // "LogData"
      case 121: // "Maturity"
      case 122: // "Msg"
      case 123: // "MultiMatch"
      case 124: // "NoAuditLog"
      case 125: // "NoLog"
      case 126: // "Pass"
      case 127: // "Pause"
      case 128: // "Phase"
      case 129: // "Prepend"
      case 130: // "Proxy"
      case 131: // "Redirect"
      case 132: // "Rev"
      case 133: // "SanatiseArg"
      case 134: // "SanatiseMatched"
      case 135: // "SanatiseMatchedBytes"
      case 136: // "SanatiseRequestHeader"
      case 137: // "SanatiseResponseHeader"
      case 138: // "SetEnv"
      case 139: // "SetRsc"
      case 140: // "SetSid"
      case 141: // "SetUID"
      case 142: // "Severity"
      case 143: // "Skip"
      case 144: // "SkipAfter"
      case 145: // "Status"
      case 146: // "Tag"
      case 147: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 148: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 149: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 151: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 152: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 153: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_LENGTH"
      case 155: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 156: // "ACTION_TRANSFORMATION_MD5"
      case 157: // "ACTION_TRANSFORMATION_NONE"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 159: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 160: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_SHA1"
      case 170: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 171: // "ACTION_TRANSFORMATION_TRIM"
      case 172: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 174: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 175: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 176: // "Ver"
      case 177: // "xmlns"
      case 178: // "CONFIG_COMPONENT_SIG"
      case 179: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 180: // "CONFIG_SEC_WEB_APP_ID"
      case 181: // "CONFIG_SEC_SERVER_SIG"
      case 182: // "CONFIG_DIR_AUDIT_DIR"
      case 183: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 184: // "CONFIG_DIR_AUDIT_ENG"
      case 185: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 186: // "CONFIG_DIR_AUDIT_LOG"
      case 187: // "CONFIG_DIR_AUDIT_LOG2"
      case 188: // "CONFIG_DIR_AUDIT_LOG_P"
      case 189: // "CONFIG_DIR_AUDIT_STS"
      case 190: // "CONFIG_DIR_AUDIT_TPE"
      case 191: // "CONFIG_DIR_DEBUG_LOG"
      case 192: // "CONFIG_DIR_DEBUG_LVL"
      case 193: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 194: // "CONFIG_DIR_GEO_DB"
      case 195: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 196: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 197: // "CONFIG_DIR_REQ_BODY"
      case 198: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 199: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 200: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 201: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 202: // "CONFIG_DIR_RES_BODY"
      case 203: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 204: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_RULE_ENG"
      case 206: // "CONFIG_DIR_SEC_ACTION"
      case 207: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 208: // "CONFIG_DIR_SEC_MARKER"
      case 209: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 210: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 211: // "CONFIG_SEC_HTTP_BLKEY"
      case 212: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 213: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 214: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 215: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 216: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 217: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 218: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 219: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 220: // "CONFIG_UPLOAD_DIR"
      case 221: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 222: // "CONFIG_UPLOAD_FILE_MODE"
      case 223: // "CONFIG_VALUE_ABORT"
      case 224: // "CONFIG_VALUE_DETC"
      case 225: // "CONFIG_VALUE_HTTPS"
      case 226: // "CONFIG_VALUE_OFF"
      case 227: // "CONFIG_VALUE_ON"
      case 228: // "CONFIG_VALUE_PARALLEL"
      case 229: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 230: // "CONFIG_VALUE_REJECT"
      case 231: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 232: // "CONFIG_VALUE_SERIAL"
      case 233: // "CONFIG_VALUE_WARN"
      case 234: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 235: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 236: // "CONGIG_DIR_SEC_ARG_SEP"
      case 237: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 238: // "CONGIG_DIR_SEC_DATA_DIR"
      case 239: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 240: // "CONGIG_DIR_SEC_TMP_DIR"
      case 241: // "DIRECTIVE"
      case 242: // "DIRECTIVE_SECRULESCRIPT"
      case 243: // "FREE_TEXT"
      case 244: // "OPERATOR"
      case 245: // "OPERATOR_BEGINS_WITH"
      case 246: // "OPERATOR_CONTAINS"
      case 247: // "OPERATOR_CONTAINS_WORD"
      case 248: // "OPERATOR_DETECT_SQLI"
      case 249: // "OPERATOR_DETECT_XSS"
      case 250: // "OPERATOR_ENDS_WITH"
      case 251: // "OPERATOR_EQ"
      case 252: // "OPERATOR_FUZZY_HASH"
      case 253: // "OPERATOR_GE"
      case 254: // "OPERATOR_GEOLOOKUP"
      case 255: // "OPERATOR_GSB_LOOKUP"
      case 256: // "OPERATOR_GT"
      case 257: // "OPERATOR_INSPECT_FILE"
      case 258: // "OPERATOR_IP_MATCH"
      case 259: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 260: // "OPERATOR_LE"
      case 261: // "OPERATOR_LT"
      case 262: // "OPERATOR_PM"
      case 263: // "OPERATOR_PM_FROM_FILE"
      case 264: // "OPERATOR_RBL"
      case 265: // "OPERATOR_RSUB"
      case 266: // "OPERATOR_RX"
      case 267: // "Operator RX (content only)"
      case 268: // "OPERATOR_STR_EQ"
      case 269: // "OPERATOR_STR_MATCH"
      case 270: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 271: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 272: // "OPERATOR_VALIDATE_DTD"
      case 273: // "OPERATOR_VALIDATE_HASH"
      case 274: // "OPERATOR_VALIDATE_SCHEMA"
      case 275: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 276: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 277: // "OPERATOR_VERIFY_CC"
      case 278: // "OPERATOR_VERIFY_CPF"
      case 279: // "OPERATOR_VERIFY_SSN"
      case 280: // "OPERATOR_WITHIN"
      case 281: // "OP_QUOTE"
      case 282: // "QUOTATION_MARK"
      case 283: // "RUN_TIME_VAR_BLD"
      case 284: // "RUN_TIME_VAR_DUR"
      case 285: // "RUN_TIME_VAR_HSV"
      case 286: // "RUN_TIME_VAR_REMOTE_USER"
      case 287: // "RUN_TIME_VAR_TIME"
      case 288: // "RUN_TIME_VAR_TIME_DAY"
      case 289: // "RUN_TIME_VAR_TIME_EPOCH"
      case 290: // "RUN_TIME_VAR_TIME_HOUR"
      case 291: // "RUN_TIME_VAR_TIME_MIN"
      case 292: // "RUN_TIME_VAR_TIME_MON"
      case 293: // "RUN_TIME_VAR_TIME_SEC"
      case 294: // "RUN_TIME_VAR_TIME_WDAY"
      case 295: // "RUN_TIME_VAR_TIME_YEAR"
      case 296: // "VARIABLE"
      case 297: // "Dictionary element"
      case 298: // "Dictionary element, selected by regexp"
        value.move< std::string > (that.value);
        break;

      case 305: // op
      case 306: // op_before_init
        value.move< std::unique_ptr<Operator> > (that.value);
        break;

      case 310: // var
        value.move< std::unique_ptr<Variable> > (that.value);
        break;

      case 311: // act
        value.move< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 308: // variables
      case 309: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 303: // actions
      case 304: // actions_may_quoted
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
      case 96: // "Accuracy"
      case 97: // "Allow"
      case 98: // "Append"
      case 99: // "AuditLog"
      case 100: // "Block"
      case 101: // "Capture"
      case 102: // "Chain"
      case 103: // "ACTION_CTL_AUDIT_ENGINE"
      case 104: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 105: // "ACTION_CTL_BDY_JSON"
      case 106: // "ACTION_CTL_BDY_XML"
      case 107: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 108: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 109: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 111: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 112: // "Deny"
      case 113: // "DeprecateVar"
      case 114: // "Drop"
      case 115: // "Exec"
      case 116: // "ExpireVar"
      case 117: // "Id"
      case 118: // "InitCol"
      case 119: // "Log"
      case 120: // "LogData"
      case 121: // "Maturity"
      case 122: // "Msg"
      case 123: // "MultiMatch"
      case 124: // "NoAuditLog"
      case 125: // "NoLog"
      case 126: // "Pass"
      case 127: // "Pause"
      case 128: // "Phase"
      case 129: // "Prepend"
      case 130: // "Proxy"
      case 131: // "Redirect"
      case 132: // "Rev"
      case 133: // "SanatiseArg"
      case 134: // "SanatiseMatched"
      case 135: // "SanatiseMatchedBytes"
      case 136: // "SanatiseRequestHeader"
      case 137: // "SanatiseResponseHeader"
      case 138: // "SetEnv"
      case 139: // "SetRsc"
      case 140: // "SetSid"
      case 141: // "SetUID"
      case 142: // "Severity"
      case 143: // "Skip"
      case 144: // "SkipAfter"
      case 145: // "Status"
      case 146: // "Tag"
      case 147: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 148: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 149: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 151: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 152: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 153: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_LENGTH"
      case 155: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 156: // "ACTION_TRANSFORMATION_MD5"
      case 157: // "ACTION_TRANSFORMATION_NONE"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 159: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 160: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_SHA1"
      case 170: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 171: // "ACTION_TRANSFORMATION_TRIM"
      case 172: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 174: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 175: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 176: // "Ver"
      case 177: // "xmlns"
      case 178: // "CONFIG_COMPONENT_SIG"
      case 179: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 180: // "CONFIG_SEC_WEB_APP_ID"
      case 181: // "CONFIG_SEC_SERVER_SIG"
      case 182: // "CONFIG_DIR_AUDIT_DIR"
      case 183: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 184: // "CONFIG_DIR_AUDIT_ENG"
      case 185: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 186: // "CONFIG_DIR_AUDIT_LOG"
      case 187: // "CONFIG_DIR_AUDIT_LOG2"
      case 188: // "CONFIG_DIR_AUDIT_LOG_P"
      case 189: // "CONFIG_DIR_AUDIT_STS"
      case 190: // "CONFIG_DIR_AUDIT_TPE"
      case 191: // "CONFIG_DIR_DEBUG_LOG"
      case 192: // "CONFIG_DIR_DEBUG_LVL"
      case 193: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 194: // "CONFIG_DIR_GEO_DB"
      case 195: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 196: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 197: // "CONFIG_DIR_REQ_BODY"
      case 198: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 199: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 200: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 201: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 202: // "CONFIG_DIR_RES_BODY"
      case 203: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 204: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_RULE_ENG"
      case 206: // "CONFIG_DIR_SEC_ACTION"
      case 207: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 208: // "CONFIG_DIR_SEC_MARKER"
      case 209: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 210: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 211: // "CONFIG_SEC_HTTP_BLKEY"
      case 212: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 213: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 214: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 215: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 216: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 217: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 218: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 219: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 220: // "CONFIG_UPLOAD_DIR"
      case 221: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 222: // "CONFIG_UPLOAD_FILE_MODE"
      case 223: // "CONFIG_VALUE_ABORT"
      case 224: // "CONFIG_VALUE_DETC"
      case 225: // "CONFIG_VALUE_HTTPS"
      case 226: // "CONFIG_VALUE_OFF"
      case 227: // "CONFIG_VALUE_ON"
      case 228: // "CONFIG_VALUE_PARALLEL"
      case 229: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 230: // "CONFIG_VALUE_REJECT"
      case 231: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 232: // "CONFIG_VALUE_SERIAL"
      case 233: // "CONFIG_VALUE_WARN"
      case 234: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 235: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 236: // "CONGIG_DIR_SEC_ARG_SEP"
      case 237: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 238: // "CONGIG_DIR_SEC_DATA_DIR"
      case 239: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 240: // "CONGIG_DIR_SEC_TMP_DIR"
      case 241: // "DIRECTIVE"
      case 242: // "DIRECTIVE_SECRULESCRIPT"
      case 243: // "FREE_TEXT"
      case 244: // "OPERATOR"
      case 245: // "OPERATOR_BEGINS_WITH"
      case 246: // "OPERATOR_CONTAINS"
      case 247: // "OPERATOR_CONTAINS_WORD"
      case 248: // "OPERATOR_DETECT_SQLI"
      case 249: // "OPERATOR_DETECT_XSS"
      case 250: // "OPERATOR_ENDS_WITH"
      case 251: // "OPERATOR_EQ"
      case 252: // "OPERATOR_FUZZY_HASH"
      case 253: // "OPERATOR_GE"
      case 254: // "OPERATOR_GEOLOOKUP"
      case 255: // "OPERATOR_GSB_LOOKUP"
      case 256: // "OPERATOR_GT"
      case 257: // "OPERATOR_INSPECT_FILE"
      case 258: // "OPERATOR_IP_MATCH"
      case 259: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 260: // "OPERATOR_LE"
      case 261: // "OPERATOR_LT"
      case 262: // "OPERATOR_PM"
      case 263: // "OPERATOR_PM_FROM_FILE"
      case 264: // "OPERATOR_RBL"
      case 265: // "OPERATOR_RSUB"
      case 266: // "OPERATOR_RX"
      case 267: // "Operator RX (content only)"
      case 268: // "OPERATOR_STR_EQ"
      case 269: // "OPERATOR_STR_MATCH"
      case 270: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 271: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 272: // "OPERATOR_VALIDATE_DTD"
      case 273: // "OPERATOR_VALIDATE_HASH"
      case 274: // "OPERATOR_VALIDATE_SCHEMA"
      case 275: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 276: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 277: // "OPERATOR_VERIFY_CC"
      case 278: // "OPERATOR_VERIFY_CPF"
      case 279: // "OPERATOR_VERIFY_SSN"
      case 280: // "OPERATOR_WITHIN"
      case 281: // "OP_QUOTE"
      case 282: // "QUOTATION_MARK"
      case 283: // "RUN_TIME_VAR_BLD"
      case 284: // "RUN_TIME_VAR_DUR"
      case 285: // "RUN_TIME_VAR_HSV"
      case 286: // "RUN_TIME_VAR_REMOTE_USER"
      case 287: // "RUN_TIME_VAR_TIME"
      case 288: // "RUN_TIME_VAR_TIME_DAY"
      case 289: // "RUN_TIME_VAR_TIME_EPOCH"
      case 290: // "RUN_TIME_VAR_TIME_HOUR"
      case 291: // "RUN_TIME_VAR_TIME_MIN"
      case 292: // "RUN_TIME_VAR_TIME_MON"
      case 293: // "RUN_TIME_VAR_TIME_SEC"
      case 294: // "RUN_TIME_VAR_TIME_WDAY"
      case 295: // "RUN_TIME_VAR_TIME_YEAR"
      case 296: // "VARIABLE"
      case 297: // "Dictionary element"
      case 298: // "Dictionary element, selected by regexp"
        value.copy< std::string > (that.value);
        break;

      case 305: // op
      case 306: // op_before_init
        value.copy< std::unique_ptr<Operator> > (that.value);
        break;

      case 310: // var
        value.copy< std::unique_ptr<Variable> > (that.value);
        break;

      case 311: // act
        value.copy< std::unique_ptr<actions::Action> > (that.value);
        break;

      case 308: // variables
      case 309: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (that.value);
        break;

      case 303: // actions
      case 304: // actions_may_quoted
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

#line 875 "seclang-parser.cc" // lalr1.cc:741

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
      case 96: // "Accuracy"
      case 97: // "Allow"
      case 98: // "Append"
      case 99: // "AuditLog"
      case 100: // "Block"
      case 101: // "Capture"
      case 102: // "Chain"
      case 103: // "ACTION_CTL_AUDIT_ENGINE"
      case 104: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 105: // "ACTION_CTL_BDY_JSON"
      case 106: // "ACTION_CTL_BDY_XML"
      case 107: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 108: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 109: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 110: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 111: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 112: // "Deny"
      case 113: // "DeprecateVar"
      case 114: // "Drop"
      case 115: // "Exec"
      case 116: // "ExpireVar"
      case 117: // "Id"
      case 118: // "InitCol"
      case 119: // "Log"
      case 120: // "LogData"
      case 121: // "Maturity"
      case 122: // "Msg"
      case 123: // "MultiMatch"
      case 124: // "NoAuditLog"
      case 125: // "NoLog"
      case 126: // "Pass"
      case 127: // "Pause"
      case 128: // "Phase"
      case 129: // "Prepend"
      case 130: // "Proxy"
      case 131: // "Redirect"
      case 132: // "Rev"
      case 133: // "SanatiseArg"
      case 134: // "SanatiseMatched"
      case 135: // "SanatiseMatchedBytes"
      case 136: // "SanatiseRequestHeader"
      case 137: // "SanatiseResponseHeader"
      case 138: // "SetEnv"
      case 139: // "SetRsc"
      case 140: // "SetSid"
      case 141: // "SetUID"
      case 142: // "Severity"
      case 143: // "Skip"
      case 144: // "SkipAfter"
      case 145: // "Status"
      case 146: // "Tag"
      case 147: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 148: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 149: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 150: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 151: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 152: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 153: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 154: // "ACTION_TRANSFORMATION_LENGTH"
      case 155: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 156: // "ACTION_TRANSFORMATION_MD5"
      case 157: // "ACTION_TRANSFORMATION_NONE"
      case 158: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 159: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 160: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 161: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 162: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 163: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 164: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 165: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 166: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 167: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 168: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 169: // "ACTION_TRANSFORMATION_SHA1"
      case 170: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 171: // "ACTION_TRANSFORMATION_TRIM"
      case 172: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 173: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 174: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 175: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 176: // "Ver"
      case 177: // "xmlns"
      case 178: // "CONFIG_COMPONENT_SIG"
      case 179: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 180: // "CONFIG_SEC_WEB_APP_ID"
      case 181: // "CONFIG_SEC_SERVER_SIG"
      case 182: // "CONFIG_DIR_AUDIT_DIR"
      case 183: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 184: // "CONFIG_DIR_AUDIT_ENG"
      case 185: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 186: // "CONFIG_DIR_AUDIT_LOG"
      case 187: // "CONFIG_DIR_AUDIT_LOG2"
      case 188: // "CONFIG_DIR_AUDIT_LOG_P"
      case 189: // "CONFIG_DIR_AUDIT_STS"
      case 190: // "CONFIG_DIR_AUDIT_TPE"
      case 191: // "CONFIG_DIR_DEBUG_LOG"
      case 192: // "CONFIG_DIR_DEBUG_LVL"
      case 193: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 194: // "CONFIG_DIR_GEO_DB"
      case 195: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 196: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 197: // "CONFIG_DIR_REQ_BODY"
      case 198: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 199: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 200: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 201: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 202: // "CONFIG_DIR_RES_BODY"
      case 203: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 204: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 205: // "CONFIG_DIR_RULE_ENG"
      case 206: // "CONFIG_DIR_SEC_ACTION"
      case 207: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 208: // "CONFIG_DIR_SEC_MARKER"
      case 209: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 210: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 211: // "CONFIG_SEC_HTTP_BLKEY"
      case 212: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 213: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 214: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 215: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 216: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 217: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 218: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 219: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 220: // "CONFIG_UPLOAD_DIR"
      case 221: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 222: // "CONFIG_UPLOAD_FILE_MODE"
      case 223: // "CONFIG_VALUE_ABORT"
      case 224: // "CONFIG_VALUE_DETC"
      case 225: // "CONFIG_VALUE_HTTPS"
      case 226: // "CONFIG_VALUE_OFF"
      case 227: // "CONFIG_VALUE_ON"
      case 228: // "CONFIG_VALUE_PARALLEL"
      case 229: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 230: // "CONFIG_VALUE_REJECT"
      case 231: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 232: // "CONFIG_VALUE_SERIAL"
      case 233: // "CONFIG_VALUE_WARN"
      case 234: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 235: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 236: // "CONGIG_DIR_SEC_ARG_SEP"
      case 237: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 238: // "CONGIG_DIR_SEC_DATA_DIR"
      case 239: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 240: // "CONGIG_DIR_SEC_TMP_DIR"
      case 241: // "DIRECTIVE"
      case 242: // "DIRECTIVE_SECRULESCRIPT"
      case 243: // "FREE_TEXT"
      case 244: // "OPERATOR"
      case 245: // "OPERATOR_BEGINS_WITH"
      case 246: // "OPERATOR_CONTAINS"
      case 247: // "OPERATOR_CONTAINS_WORD"
      case 248: // "OPERATOR_DETECT_SQLI"
      case 249: // "OPERATOR_DETECT_XSS"
      case 250: // "OPERATOR_ENDS_WITH"
      case 251: // "OPERATOR_EQ"
      case 252: // "OPERATOR_FUZZY_HASH"
      case 253: // "OPERATOR_GE"
      case 254: // "OPERATOR_GEOLOOKUP"
      case 255: // "OPERATOR_GSB_LOOKUP"
      case 256: // "OPERATOR_GT"
      case 257: // "OPERATOR_INSPECT_FILE"
      case 258: // "OPERATOR_IP_MATCH"
      case 259: // "OPERATOR_IP_MATCH_FROM_FILE"
      case 260: // "OPERATOR_LE"
      case 261: // "OPERATOR_LT"
      case 262: // "OPERATOR_PM"
      case 263: // "OPERATOR_PM_FROM_FILE"
      case 264: // "OPERATOR_RBL"
      case 265: // "OPERATOR_RSUB"
      case 266: // "OPERATOR_RX"
      case 267: // "Operator RX (content only)"
      case 268: // "OPERATOR_STR_EQ"
      case 269: // "OPERATOR_STR_MATCH"
      case 270: // "OPERATOR_UNCONDITIONAL_MATCH"
      case 271: // "OPERATOR_VALIDATE_BYTE_RANGE"
      case 272: // "OPERATOR_VALIDATE_DTD"
      case 273: // "OPERATOR_VALIDATE_HASH"
      case 274: // "OPERATOR_VALIDATE_SCHEMA"
      case 275: // "OPERATOR_VALIDATE_URL_ENCODING"
      case 276: // "OPERATOR_VALIDATE_UTF8_ENCODING"
      case 277: // "OPERATOR_VERIFY_CC"
      case 278: // "OPERATOR_VERIFY_CPF"
      case 279: // "OPERATOR_VERIFY_SSN"
      case 280: // "OPERATOR_WITHIN"
      case 281: // "OP_QUOTE"
      case 282: // "QUOTATION_MARK"
      case 283: // "RUN_TIME_VAR_BLD"
      case 284: // "RUN_TIME_VAR_DUR"
      case 285: // "RUN_TIME_VAR_HSV"
      case 286: // "RUN_TIME_VAR_REMOTE_USER"
      case 287: // "RUN_TIME_VAR_TIME"
      case 288: // "RUN_TIME_VAR_TIME_DAY"
      case 289: // "RUN_TIME_VAR_TIME_EPOCH"
      case 290: // "RUN_TIME_VAR_TIME_HOUR"
      case 291: // "RUN_TIME_VAR_TIME_MIN"
      case 292: // "RUN_TIME_VAR_TIME_MON"
      case 293: // "RUN_TIME_VAR_TIME_SEC"
      case 294: // "RUN_TIME_VAR_TIME_WDAY"
      case 295: // "RUN_TIME_VAR_TIME_YEAR"
      case 296: // "VARIABLE"
      case 297: // "Dictionary element"
      case 298: // "Dictionary element, selected by regexp"
        yylhs.value.build< std::string > ();
        break;

      case 305: // op
      case 306: // op_before_init
        yylhs.value.build< std::unique_ptr<Operator> > ();
        break;

      case 310: // var
        yylhs.value.build< std::unique_ptr<Variable> > ();
        break;

      case 311: // act
        yylhs.value.build< std::unique_ptr<actions::Action> > ();
        break;

      case 308: // variables
      case 309: // variables_may_be_quoted
        yylhs.value.build< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 303: // actions
      case 304: // actions_may_quoted
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
#line 682 "seclang-parser.yy" // lalr1.cc:859
    {
        return 0;
      }
#line 1216 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 695 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDirMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1224 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 7:
#line 701 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStorageDir(yystack_[0].value.as< std::string > ());
      }
#line 1232 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 707 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::RelevantOnlyAuditLogStatus);
      }
#line 1240 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 9:
#line 711 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OffAuditLogStatus);
      }
#line 1248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 715 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setStatus(modsecurity::audit_log::AuditLog::OnAuditLogStatus);
      }
#line 1256 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 11:
#line 721 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFileMode(strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8));
      }
#line 1264 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 727 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath2(yystack_[0].value.as< std::string > ());
      }
#line 1272 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 13:
#line 733 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setParts(yystack_[0].value.as< std::string > ());
      }
#line 1280 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 739 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setFilePath1(yystack_[0].value.as< std::string > ());
      }
#line 1288 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 15:
#line 745 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string relevant_status(yystack_[0].value.as< std::string > ());
        driver.m_auditLog->setRelevantStatus(relevant_status);
      }
#line 1297 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 752 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::SerialAuditLogType);
      }
#line 1305 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 17:
#line 756 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::ParallelAuditLogType);
      }
#line 1313 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 760 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_auditLog->setType(modsecurity::audit_log::AuditLog::HttpsAuditLogType);
      }
#line 1321 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 19:
#line 766 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1329 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 770 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadKeepFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 21:
#line 774 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileLimit.m_set = true;
        driver.m_uploadFileLimit.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 10);
      }
#line 1346 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 779 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadFileMode.m_set = true;
        driver.m_uploadFileMode.m_value = strtol(yystack_[0].value.as< std::string > ().c_str(), NULL, 8);
      }
#line 1355 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 23:
#line 784 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_uploadDirectory.m_set = true;
        driver.m_uploadDirectory.m_value = yystack_[0].value.as< std::string > ();
      }
#line 1364 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 789 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1372 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 793 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_tmpSaveUploadedFiles = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1380 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 800 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1388 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 804 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1396 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 811 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[3].location)
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ());
      }
#line 1406 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 817 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<actions::Action>>> b(new std::vector<std::unique_ptr<actions::Action>>());
        ACTION_INIT(yystack_[0].value.as< std::unique_ptr<actions::Action> > (), yystack_[1].location)
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<actions::Action> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > () = std::move(b);
      }
#line 1417 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 827 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1430 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 836 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<Operator> > () = std::move(yystack_[0].value.as< std::unique_ptr<Operator> > ());
        yylhs.value.as< std::unique_ptr<Operator> > ()->m_negation = true;
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1444 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 846 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[1].location, error);
            YYERROR;
        }
      }
#line 1457 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 855 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx("!" + utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ())));
        std::string error;
        if (yylhs.value.as< std::unique_ptr<Operator> > ()->init(driver.ref.back(), &error) == false) {
            driver.error(yystack_[2].location, error);
            YYERROR;
        }
      }
#line 1470 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 867 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::UnconditionalMatch());
      }
#line 1478 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 871 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectSQLi());
      }
#line 1486 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 875 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::DetectXSS());
      }
#line 1494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 879 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUrlEncoding());
      }
#line 1502 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 883 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateUtf8Encoding());
      }
#line 1510 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 887 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::InspectFile($1); */
        OPERATOR_NOT_SUPPORTED("InspectFile", yystack_[2].location);
      }
#line 1519 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 892 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::FuzzyHash(); */
        OPERATOR_NOT_SUPPORTED("FuzzyHash", yystack_[2].location);
      }
#line 1528 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 897 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateByteRange(yystack_[0].value.as< std::string > ()));
      }
#line 1536 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 901 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateDTD(yystack_[0].value.as< std::string > ()));
      }
#line 1544 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 905 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::ValidateHash($1); */
        OPERATOR_NOT_SUPPORTED("ValidateHash", yystack_[2].location);
      }
#line 1553 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 910 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ValidateSchema(yystack_[0].value.as< std::string > ()));
      }
#line 1561 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 914 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::VerifyCC(yystack_[0].value.as< std::string > ()));
      }
#line 1569 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 918 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifyCPF($1); */
        OPERATOR_NOT_SUPPORTED("VerifyCPF", yystack_[2].location);
      }
#line 1578 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 923 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::VerifySSN($1); */
        OPERATOR_NOT_SUPPORTED("VerifySSN", yystack_[2].location);
      }
#line 1587 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 928 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::GsbLookup($1); */
        OPERATOR_NOT_SUPPORTED("GsbLookup", yystack_[2].location);
      }
#line 1596 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 933 "seclang-parser.yy" // lalr1.cc:859
    {
        /* $$ = new operators::Rsub($1); */
        OPERATOR_NOT_SUPPORTED("Rsub", yystack_[2].location);
      }
#line 1605 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 938 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Within(yystack_[0].value.as< std::string > ()));
      }
#line 1613 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 942 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::ContainsWord(yystack_[0].value.as< std::string > ()));
      }
#line 1621 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 946 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Contains(yystack_[0].value.as< std::string > ()));
      }
#line 1629 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 950 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::EndsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1637 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 954 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Eq(yystack_[0].value.as< std::string > ()));
      }
#line 1645 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 958 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Ge(yystack_[0].value.as< std::string > ()));
      }
#line 1653 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 56:
#line 962 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Gt(yystack_[0].value.as< std::string > ()));
      }
#line 1661 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 57:
#line 966 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatchF(yystack_[0].value.as< std::string > ()));
      }
#line 1669 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 58:
#line 970 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::IpMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1677 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 59:
#line 974 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Le(yystack_[0].value.as< std::string > ()));
      }
#line 1685 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 60:
#line 978 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Lt(yystack_[0].value.as< std::string > ()));
      }
#line 1693 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 61:
#line 982 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::PmFromFile(yystack_[0].value.as< std::string > ()));
      }
#line 1701 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 986 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Pm(yystack_[0].value.as< std::string > ()));
      }
#line 1709 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 990 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rbl(yystack_[0].value.as< std::string > ()));
      }
#line 1717 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 994 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::Rx(yystack_[0].value.as< std::string > ()));
      }
#line 1725 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 998 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrEq(yystack_[0].value.as< std::string > ()));
      }
#line 1733 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 1002 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::StrMatch(yystack_[0].value.as< std::string > ()));
      }
#line 1741 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 1006 "seclang-parser.yy" // lalr1.cc:859
    {
        OPERATOR_CONTAINER(yylhs.value.as< std::unique_ptr<Operator> > (), new operators::BeginsWith(yystack_[0].value.as< std::string > ()));
      }
#line 1749 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 68:
#line 1010 "seclang-parser.yy" // lalr1.cc:859
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
#line 1764 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 70:
#line 1025 "seclang-parser.yy" // lalr1.cc:859
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
#line 1792 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 71:
#line 1049 "seclang-parser.yy" // lalr1.cc:859
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
#line 1815 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 72:
#line 1068 "seclang-parser.yy" // lalr1.cc:859
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
#line 1834 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 73:
#line 1083 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "SecRuleScript is not yet supported.");
        YYERROR;
      }
#line 1843 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 74:
#line 1088 "seclang-parser.yy" // lalr1.cc:859
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
#line 1894 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 75:
#line 1135 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.addSecMarker(modsecurity::utils::string::removeBracketsIfNeeded(yystack_[0].value.as< std::string > ()));
      }
#line 1902 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 76:
#line 1139 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DisabledRuleEngine;
      }
#line 1910 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 77:
#line 1143 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::EnabledRuleEngine;
      }
#line 1918 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 78:
#line 1147 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRuleEngine = modsecurity::Rules::DetectionOnlyRuleEngine;
      }
#line 1926 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 79:
#line 1151 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1934 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 80:
#line 1155 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secRequestBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1942 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 81:
#line 1159 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 1950 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 82:
#line 1163 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secResponseBodyAccess = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 1958 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 83:
#line 1167 "seclang-parser.yy" // lalr1.cc:859
    {
        if (yystack_[0].value.as< std::string > ().length() != 1) {
          driver.error(yystack_[1].location, "Argument separator should be set to a single character.");
          YYERROR;
        }
        driver.m_secArgumentSeparator.m_value = yystack_[0].value.as< std::string > ();
        driver.m_secArgumentSeparator.m_set = true;
      }
#line 1971 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 84:
#line 1176 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_components.push_back(yystack_[0].value.as< std::string > ());
      }
#line 1979 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 85:
#line 1180 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecWebAppId is not supported.");
        YYERROR;
      }
#line 1988 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 86:
#line 1185 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecServerSignature is not supported.");
        YYERROR;
      }
#line 1997 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 87:
#line 1190 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2006 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 88:
#line 1195 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[1].location, "SecCacheTransformations is not supported.");
        YYERROR;
      }
#line 2015 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 89:
#line 1200 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.error(yystack_[2].location, "ContentInjection is not yet supported.");
        YYERROR;
      }
#line 2024 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 1205 "seclang-parser.yy" // lalr1.cc:859
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
#line 2041 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 1218 "seclang-parser.yy" // lalr1.cc:859
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
#line 2058 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 1231 "seclang-parser.yy" // lalr1.cc:859
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
#line 2075 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 1244 "seclang-parser.yy" // lalr1.cc:859
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
#line 2105 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 1270 "seclang-parser.yy" // lalr1.cc:859
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
#line 2134 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 1296 "seclang-parser.yy" // lalr1.cc:859
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
#line 2150 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 1308 "seclang-parser.yy" // lalr1.cc:859
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
#line 2173 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 1328 "seclang-parser.yy" // lalr1.cc:859
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
#line 2204 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 1356 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimit.m_set = true;
        driver.m_requestBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2213 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 1361 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyNoFilesLimit.m_set = true;
        driver.m_requestBodyNoFilesLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2222 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 1366 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyInMemoryLimit.m_set = true;
        driver.m_requestBodyInMemoryLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 1371 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimit.m_set = true;
        driver.m_responseBodyLimit.m_value = atoi(yystack_[0].value.as< std::string > ().c_str());
      }
#line 2240 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 102:
#line 1376 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2248 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 1380 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_requestBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2256 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 1384 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::ProcessPartialBodyLimitAction;
      }
#line 2264 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 1388 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyLimitAction = modsecurity::Rules::BodyLimitAction::RejectBodyLimitAction;
      }
#line 2272 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 1392 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::AbortOnFailedRemoteRulesAction;
      }
#line 2280 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 1396 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_remoteRulesActionOnFailed = Rules::OnFailedRemoteRulesAction::WarnOnFailedRemoteRulesAction;
      }
#line 2288 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 1402 "seclang-parser.yy" // lalr1.cc:859
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
#line 2304 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 1414 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_responseBodyTypeToBeInspected.m_set = true;
        driver.m_responseBodyTypeToBeInspected.m_clear = true;
        driver.m_responseBodyTypeToBeInspected.m_value.clear();
      }
#line 2314 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 1420 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::FalseConfigBoolean;
      }
#line 2322 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 1424 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_secXMLExternalEntity = modsecurity::RulesProperties::TrueConfigBoolean;
      }
#line 2330 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 1434 "seclang-parser.yy" // lalr1.cc:859
    {
      }
#line 2337 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 1437 "seclang-parser.yy" // lalr1.cc:859
    {
        driver.m_httpblKey.m_set = true;
        driver.m_httpblKey.m_value = yystack_[0].value.as< std::string > ();
      }
#line 2346 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 1445 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[0].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2354 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 1449 "seclang-parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[1].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2362 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 1456 "seclang-parser.yy" // lalr1.cc:859
    {
        yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[2].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2371 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 125:
#line 1461 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2381 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 126:
#line 1467 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ()->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(yystack_[3].value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ());
      }
#line 2391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 127:
#line 1473 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        b->push_back(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ()));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2401 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 128:
#line 1479 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorExclusion(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2412 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 129:
#line 1486 "seclang-parser.yy" // lalr1.cc:859
    {
        std::unique_ptr<std::vector<std::unique_ptr<Variable>>> b(new std::vector<std::unique_ptr<Variable>>());
        std::unique_ptr<Variable> c(new VariableModificatorCount(std::move(yystack_[0].value.as< std::unique_ptr<Variable> > ())));
        b->push_back(std::move(c));
        yylhs.value.as< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > () = std::move(b);
      }
#line 2423 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 130:
#line 1496 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 1500 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 1504 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Args_NoDictElement());
      }
#line 2447 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 1508 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2455 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 1512 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2463 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 1516 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPost_NoDictElement());
      }
#line 2471 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 1520 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2479 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 1524 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2487 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 1528 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGet_NoDictElement());
      }
#line 2495 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 1532 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2503 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 1536 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2511 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 1540 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesSizes_NoDictElement());
      }
#line 2519 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 1544 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 1548 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2535 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 1552 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesNames_NoDictElement());
      }
#line 2543 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 1556 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2551 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 1560 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2559 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 1564 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpContent_NoDictElement());
      }
#line 2567 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 1568 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2575 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 1572 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2583 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 1576 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartFileName_NoDictElement());
      }
#line 2591 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 1580 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2599 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 1584 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2607 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 1588 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultiPartName_NoDictElement());
      }
#line 2615 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 1592 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2623 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 1596 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2631 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 1600 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarsNames_NoDictElement());
      }
#line 2639 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 1604 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 1608 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2655 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 1612 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVars_NoDictElement());
      }
#line 2663 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 1616 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2671 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 1620 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2679 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 1624 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Files_NoDictElement());
      }
#line 2687 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 1628 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2695 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 1632 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2703 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 1636 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookies_NoDictElement());
      }
#line 2711 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 1640 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2719 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 1644 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2727 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 1648 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeaders_NoDictElement());
      }
#line 2735 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 1652 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2743 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 170:
#line 1656 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2751 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 171:
#line 1660 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeaders_NoDictElement());
      }
#line 2759 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 1664 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2767 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 1668 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2775 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 1672 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Geo_NoDictElement());
      }
#line 2783 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 1676 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2791 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 1680 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2799 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 1684 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestCookiesNames_NoDictElement());
      }
#line 2807 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 1688 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2815 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 179:
#line 1692 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2823 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 180:
#line 1696 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Rule_NoDictElement());
      }
#line 2831 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 181:
#line 1700 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2839 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 182:
#line 1704 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV:" + yystack_[0].value.as< std::string > ()));
      }
#line 2847 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 1708 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Env("ENV"));
      }
#line 2855 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 1712 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2863 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 1716 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML("XML:" + yystack_[0].value.as< std::string > ()));
      }
#line 2871 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 1720 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::XML_NoDictElement());
      }
#line 2879 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 1724 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2887 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 1728 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2895 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 189:
#line 1732 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesTmpNames_NoDictElement());
      }
#line 2903 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 190:
#line 1737 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2911 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 191:
#line 1741 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2919 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 1745 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Ip_NoDictElement());
      }
#line 2927 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 1750 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2935 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 1754 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2943 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 1758 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Global_NoDictElement());
      }
#line 2951 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 1763 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2959 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 1767 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2967 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 1771 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Tx_NoDictElement());
      }
#line 2975 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 1776 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElement(yystack_[0].value.as< std::string > ()));
      }
#line 2983 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 1780 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_DictElementRegexp(yystack_[0].value.as< std::string > ()));
      }
#line 2991 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 1784 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Session_NoDictElement());
      }
#line 2999 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 1791 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsNames());
      }
#line 3007 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 1795 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsGetNames());
      }
#line 3015 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 1799 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsPostNames());
      }
#line 3023 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 205:
#line 1803 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestHeadersNames());
      }
#line 3031 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 1807 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentType());
      }
#line 3039 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 1811 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseHeadersNames());
      }
#line 3047 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 1815 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ArgsCombinedSize());
      }
#line 3055 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 1819 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::AuthType());
      }
#line 3063 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 1823 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FilesCombinedSize());
      }
#line 3071 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 1827 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequest());
      }
#line 3079 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 1831 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::FullRequestLength());
      }
#line 3087 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 1835 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::InboundDataError());
      }
#line 3095 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 214:
#line 1839 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVar());
      }
#line 3103 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 215:
#line 1843 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MatchedVarName());
      }
#line 3111 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 216:
#line 1847 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartCrlfLFLines());
      }
#line 3119 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 217:
#line 1851 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartDateAfter());
      }
#line 3127 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 1855 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartFileLimitExceeded());
      }
#line 3135 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 1859 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartHeaderFolding());
      }
#line 3143 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 1863 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidHeaderFolding());
      }
#line 3151 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 1867 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartInvalidQuoting());
      }
#line 3159 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 222:
#line 1871 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartStrictError());
      }
#line 3167 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 223:
#line 1875 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::MultipartUnmatchedBoundary());
      }
#line 3175 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 224:
#line 1879 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::OutboundDataError());
      }
#line 3183 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 225:
#line 1883 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::PathInfo());
      }
#line 3191 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 226:
#line 1887 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::QueryString());
      }
#line 3199 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 227:
#line 1891 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteAddr());
      }
#line 3207 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 228:
#line 1895 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemoteHost());
      }
#line 3215 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 229:
#line 1899 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RemotePort());
      }
#line 3223 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 230:
#line 1903 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyError());
      }
#line 3231 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 231:
#line 1907 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyErrorMsg());
      }
#line 3239 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 232:
#line 1911 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessor());
      }
#line 3247 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 233:
#line 1915 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorError());
      }
#line 3255 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 234:
#line 1919 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ReqbodyProcessorErrorMsg());
      }
#line 3263 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 1923 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBasename());
      }
#line 3271 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 1927 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBody());
      }
#line 3279 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 1931 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestBodyLength());
      }
#line 3287 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 1935 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestFilename());
      }
#line 3295 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 1939 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestLine());
      }
#line 3303 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 1943 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestMethod());
      }
#line 3311 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 1947 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestProtocol());
      }
#line 3319 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 1951 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURI());
      }
#line 3327 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 1955 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::RequestURIRaw());
      }
#line 3335 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 1959 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Resource());
      }
#line 3343 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 1963 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseBody());
      }
#line 3351 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 1967 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseContentLength());
      }
#line 3359 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 1971 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseProtocol());
      }
#line 3367 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 1975 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ResponseStatus());
      }
#line 3375 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 1979 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerAddr());
      }
#line 3383 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 1983 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerName());
      }
#line 3391 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 1987 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::ServerPort());
      }
#line 3399 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 1991 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::SessionID());
      }
#line 3407 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 1995 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UniqueID());
      }
#line 3415 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 1999 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UrlEncodedError());
      }
#line 3423 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 2003 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::UserID());
      }
#line 3431 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 2007 "seclang-parser.yy" // lalr1.cc:859
    {
        VARIABLE_CONTAINER(yylhs.value.as< std::unique_ptr<Variable> > (), new Variables::Status());
      }
#line 3439 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 2011 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Duration(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3450 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 2019 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new ModsecBuild(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3461 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 2026 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new HighestSeverity(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3472 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 2033 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new RemoteUser(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3483 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 2040 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new Time(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3494 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 2047 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3505 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 2054 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeEpoch(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3516 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 2061 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeHour(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3527 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 2068 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMin(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3538 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 2075 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeMon(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3549 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 2082 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
            std::unique_ptr<Variable> c(new TimeSec(name));
            yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3560 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 2089 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeWDay(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3571 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 2096 "seclang-parser.yy" // lalr1.cc:859
    {
        std::string name(yystack_[0].value.as< std::string > ());
        char z = name.at(0);
        std::unique_ptr<Variable> c(new TimeYear(name));
        yylhs.value.as< std::unique_ptr<Variable> > () = std::move(c);
      }
#line 3582 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 2106 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Accuracy(yystack_[0].value.as< std::string > ()));
      }
#line 3590 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 271:
#line 2110 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Allow(yystack_[0].value.as< std::string > ()));
      }
#line 3598 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 272:
#line 2114 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Append", yystack_[1].location);
      }
#line 3606 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 273:
#line 2118 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::AuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3614 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 274:
#line 2122 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Block(yystack_[0].value.as< std::string > ()));
      }
#line 3622 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 2126 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Capture(yystack_[0].value.as< std::string > ()));
      }
#line 3630 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 2130 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Chain(yystack_[0].value.as< std::string > ()));
      }
#line 3638 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 277:
#line 2134 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3647 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 278:
#line 2139 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3656 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 279:
#line 2144 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlAuditEngine", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3665 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 2149 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::AuditLogParts(yystack_[0].value.as< std::string > ()));
      }
#line 3673 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 2153 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorJSON(yystack_[0].value.as< std::string > ()));
      }
#line 3681 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 282:
#line 2157 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyProcessorXML(yystack_[0].value.as< std::string > ()));
      }
#line 3689 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 283:
#line 2161 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3698 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 284:
#line 2166 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("CtlForceReequestBody", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[1].value.as< std::string > ()));
      }
#line 3707 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 285:
#line 2171 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "true"));
      }
#line 3715 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 2175 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RequestBodyAccess(yystack_[1].value.as< std::string > () + "false"));
      }
#line 3723 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 2179 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=on"));
      }
#line 3731 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 2183 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=off"));
      }
#line 3739 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 2187 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleEngine("ctl:RuleEngine=detectiononly"));
      }
#line 3747 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 290:
#line 2191 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveById(yystack_[0].value.as< std::string > ()));
      }
#line 3755 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 291:
#line 2195 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetById(yystack_[0].value.as< std::string > ()));
      }
#line 3763 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 292:
#line 2199 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::ctl::RuleRemoveTargetByTag(yystack_[0].value.as< std::string > ()));
      }
#line 3771 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 293:
#line 2203 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Deny(yystack_[0].value.as< std::string > ()));
      }
#line 3779 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 2207 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("DeprecateVar", yystack_[1].location);
      }
#line 3787 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 2211 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("Drop", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3796 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 2216 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Exec", yystack_[1].location);
      }
#line 3804 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 2220 "seclang-parser.yy" // lalr1.cc:859
    {
        //ACTION_NOT_SUPPORTED("ExpireVar", @0);
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Action(yystack_[0].value.as< std::string > ()));
      }
#line 3813 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 298:
#line 2225 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::RuleId(yystack_[0].value.as< std::string > ()));
      }
#line 3821 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 299:
#line 2229 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::InitCol(yystack_[0].value.as< std::string > ()));
      }
#line 3829 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 300:
#line 2233 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::LogData(yystack_[0].value.as< std::string > ()));
      }
#line 3837 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 301:
#line 2237 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Log(yystack_[0].value.as< std::string > ()));
      }
#line 3845 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 2241 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Maturity(yystack_[0].value.as< std::string > ()));
      }
#line 3853 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 2245 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Msg(yystack_[0].value.as< std::string > ()));
      }
#line 3861 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 2249 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::MultiMatch(yystack_[0].value.as< std::string > ()));
      }
#line 3869 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 2253 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoAuditLog(yystack_[0].value.as< std::string > ()));
      }
#line 3877 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 2257 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::NoLog(yystack_[0].value.as< std::string > ()));
      }
#line 3885 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 2261 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Pass(yystack_[0].value.as< std::string > ()));
      }
#line 3893 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 2265 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Pause", yystack_[1].location);
      }
#line 3901 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 2269 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Phase(yystack_[0].value.as< std::string > ()));
      }
#line 3909 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 2273 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Prepend", yystack_[1].location);
      }
#line 3917 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 311:
#line 2277 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("Proxy", yystack_[1].location);
      }
#line 3925 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 2281 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::disruptive::Redirect(yystack_[0].value.as< std::string > ()));
      }
#line 3933 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 2285 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Rev(yystack_[0].value.as< std::string > ()));
      }
#line 3941 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 2289 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseArg", yystack_[1].location);
      }
#line 3949 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 2293 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatched", yystack_[1].location);
      }
#line 3957 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 2297 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseMatchedBytes", yystack_[1].location);
      }
#line 3965 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 317:
#line 2301 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseRequestHeader", yystack_[1].location);
      }
#line 3973 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 318:
#line 2305 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SanatiseResponseHeader", yystack_[1].location);
      }
#line 3981 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 2309 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_NOT_SUPPORTED("SetEnv", yystack_[1].location);
      }
#line 3989 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 320:
#line 2313 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetRSC(yystack_[0].value.as< std::string > ()));
      }
#line 3997 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 2317 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetSID(yystack_[0].value.as< std::string > ()));
      }
#line 4005 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 2321 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetUID(yystack_[0].value.as< std::string > ()));
      }
#line 4013 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 2325 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::unsetOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4021 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 2329 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setToOneOperation, yystack_[0].value.as< std::string > ()));
      }
#line 4029 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 2333 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::setOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4037 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 2337 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::sumAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4045 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 327:
#line 2341 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SetVar(actions::SetVarOperation::substractAndSetOperation, yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()));
      }
#line 4053 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 2345 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Severity(yystack_[0].value.as< std::string > ()));
      }
#line 4061 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 2349 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Skip(yystack_[0].value.as< std::string > ()));
      }
#line 4069 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 2353 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::SkipAfter(yystack_[0].value.as< std::string > ()));
      }
#line 4077 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 2357 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::data::Status(yystack_[0].value.as< std::string > ()));
      }
#line 4085 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 2361 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Tag(yystack_[0].value.as< std::string > ()));
      }
#line 4093 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 333:
#line 2365 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::Ver(yystack_[0].value.as< std::string > ()));
      }
#line 4101 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 334:
#line 2369 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::XmlNS(yystack_[0].value.as< std::string > ()));
      }
#line 4109 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 335:
#line 2373 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityZero7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4117 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 336:
#line 2377 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityOdd7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4125 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 2381 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ParityEven7bit(yystack_[0].value.as< std::string > ()));
      }
#line 4133 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 2385 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::SqlHexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4141 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 2389 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CmdLine(yystack_[0].value.as< std::string > ()));
      }
#line 4149 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 2393 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Sha1(yystack_[0].value.as< std::string > ()));
      }
#line 4157 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 2397 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Md5(yystack_[0].value.as< std::string > ()));
      }
#line 4165 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 2401 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexEncode(yystack_[0].value.as< std::string > ()));
      }
#line 4173 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 2405 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HexDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4181 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 2409 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::LowerCase(yystack_[0].value.as< std::string > ()));
      }
#line 4189 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 345:
#line 2413 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UpperCase(yystack_[0].value.as< std::string > ()));
      }
#line 4197 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 346:
#line 2417 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecodeUni(yystack_[0].value.as< std::string > ()));
      }
#line 4205 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 347:
#line 2421 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::UrlDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4213 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 348:
#line 2425 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::None(yystack_[0].value.as< std::string > ()));
      }
#line 4221 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 2429 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CompressWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4229 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 2433 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveWhitespace(yystack_[0].value.as< std::string > ()));
      }
#line 4237 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 2437 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4245 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 2441 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveNulls(yystack_[0].value.as< std::string > ()));
      }
#line 4253 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 2445 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::HtmlEntityDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4261 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 2449 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::JsDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4269 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 355:
#line 2453 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::CssDecode(yystack_[0].value.as< std::string > ()));
      }
#line 4277 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 356:
#line 2457 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Trim(yystack_[0].value.as< std::string > ()));
      }
#line 4285 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 2461 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePathWin(yystack_[0].value.as< std::string > ()));
      }
#line 4293 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 2465 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::NormalisePath(yystack_[0].value.as< std::string > ()));
      }
#line 4301 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 2469 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Length(yystack_[0].value.as< std::string > ()));
      }
#line 4309 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 2473 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::Utf8ToUnicode(yystack_[0].value.as< std::string > ()));
      }
#line 4317 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 2477 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveCommentsChar(yystack_[0].value.as< std::string > ()));
      }
#line 4325 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 2481 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::RemoveComments(yystack_[0].value.as< std::string > ()));
      }
#line 4333 "seclang-parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 2485 "seclang-parser.yy" // lalr1.cc:859
    {
        ACTION_CONTAINER(yylhs.value.as< std::unique_ptr<actions::Action> > (), new actions::transformations::ReplaceComments(yystack_[0].value.as< std::string > ()));
      }
#line 4341 "seclang-parser.cc" // lalr1.cc:859
    break;


#line 4345 "seclang-parser.cc" // lalr1.cc:859
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


  const short int seclang_parser::yypact_ninf_ = -284;

  const signed char seclang_parser::yytable_ninf_ = -1;

  const short int
  seclang_parser::yypact_[] =
  {
     395,  -284,  -205,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -211,  -284,  -284,  -284,  -284,  -284,  -220,  -284,  -284,  -284,
    -284,  -284,  -284,    57,  -284,  -284,  -223,  -284,    59,  -284,
      58,    47,   559,   559,  -284,  -284,  -284,  -284,  -219,  -284,
    -284,    17,    17,   559,    63,    65,  -284,  -284,  -284,    67,
    -284,  -284,  -284,  -284,  -284,  -284,    17,   559,   618,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,   261,    55,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -208,  -284,  -284,  -284,    69,    71,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,   771,  -284,     6,  -284,  -284,
    -284,  -284,   467,   467,    52,    56,    60,    62,    64,    66,
      70,    72,    74,    76,    78,    80,    82,   104,   106,   108,
    -284,  -284,   110,  -284,  -284,  -284,  -284,   112,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,   114,   116,   118,
     120,   158,   160,    99,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,     5,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,   174,  -284,
    -284,  -284,  -283,   184,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,    -2,   771,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,    -4,   181,    68,  -139,
     -57,    29,  -284,  -284,   113,   122,   123,   138,  -284,   153,
     216,   217,   218,   219,   220,   311,   315,   316,   317,   318,
     319,  -284,   320,   321,  -284,   322,   323,   324,   325,  -284,
    -284,   326,   327,   328,   378,   559,  -284,  -284,   381,   382,
     383,  -284,  -284,  -284,   467,   467,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284
  };

  const unsigned short int
  seclang_parser::yydefact_[] =
  {
       0,     2,     0,   111,    84,    83,    85,    86,     7,     6,
       0,    11,    14,    12,    13,    15,     0,    96,    95,    88,
      97,   109,   108,     0,   100,    98,     0,    99,     0,   101,
       0,     0,     0,     0,    75,   119,   120,   121,     0,    90,
      91,     0,     0,     0,     0,     0,    23,    21,    22,     0,
     110,   116,   117,   115,   118,   114,     0,     0,     0,     4,
      69,     5,    89,    87,     9,    10,     8,    18,    17,    16,
      80,    79,   102,   103,    82,    81,   104,   105,    78,    76,
      77,     0,     0,   270,   271,   272,   273,   274,   275,   276,
       0,   280,   281,   282,     0,     0,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   301,   300,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   328,
     329,   330,   331,   332,   339,   349,   355,   342,   343,   353,
     354,   359,   344,   341,   348,   358,   357,   337,   336,   335,
     362,   361,   352,   350,   363,   351,   340,   338,   356,   345,
     347,   346,   360,   333,   334,     0,    72,    27,    29,    74,
     106,   107,     0,     0,   132,   135,   138,   141,   144,   147,
     150,   153,   156,   159,   162,   165,   168,   171,   174,   177,
     208,   203,   180,   202,   204,   209,   210,   189,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   231,   230,   234,
     233,   232,   235,   237,   236,   238,   205,   239,   240,   241,
     243,   242,   244,   245,   246,   206,   207,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   192,   195,   198,
     201,   183,   186,     0,   258,   257,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,    92,   122,   127,
      93,    94,    20,    19,    25,    24,   112,   113,     0,    73,
       1,     3,     0,   324,   289,   288,   287,   278,   277,   279,
     284,   283,   286,   285,     0,     0,   129,   128,   130,   131,
     133,   134,   136,   137,   139,   140,   142,   143,   145,   146,
     148,   149,   151,   152,   154,   155,   157,   158,   160,   161,
     163,   164,   166,   167,   169,   170,   172,   173,   175,   176,
     178,   179,   187,   188,   190,   191,   193,   194,   196,   197,
     199,   200,   181,   182,   184,   185,     0,     0,     0,     0,
       0,     0,    35,    36,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,    34,     0,     0,     0,     0,    37,
      38,     0,     0,     0,     0,    71,    30,   323,     0,     0,
       0,    26,    28,   123,     0,     0,   124,    33,    31,    67,
      52,    51,    53,    54,    40,    55,    48,    56,    39,    58,
      57,    59,    60,    62,    61,    63,    49,    64,    65,    66,
      41,    42,    43,    44,    45,    46,    47,    50,    70,   325,
     326,   327,   126,   125
  };

  const short int
  seclang_parser::yypgoto_[] =
  {
    -284,  -284,   293,  -284,   -33,   462,  -284,   280,  -284,   -39,
     385,   225,   344
  };

  const short int
  seclang_parser::yydefgoto_[] =
  {
      -1,    58,    59,    60,   166,   167,   385,   386,    61,   267,
     268,   269,   168
  };

  const unsigned short int
  seclang_parser::yytable_[] =
  {
     169,   295,   347,   270,   170,    67,    72,    73,    68,   295,
     271,   347,    69,   387,   171,    64,    65,   278,   287,   288,
      66,    62,    63,   289,   279,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   399,   251,   252,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   400,   251,   252,   394,
     395,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   348,   251,
     252,    78,   401,    79,    80,   388,   389,   390,   393,   284,
     391,   285,   286,    70,    71,    74,    75,    76,    77,   272,
     273,   274,   275,   276,   277,   290,   291,   292,   293,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   397,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   298,
     299,   281,   428,   300,   301,   282,   402,   302,   303,   304,
     305,   306,   307,   308,   309,   403,   404,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   405,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     1,   406,   296,   297,     2,
       3,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   342,   343,   344,   345,   407,
     408,   409,   410,   411,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   412,   251,   252,   283,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   396,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,   280,   432,
     433,   427,     2,     3,   429,   430,   431,   294,   398,    49,
      50,    51,    52,    53,    54,    55,    56,    57,   346,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    81,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164
  };

  const short int
  seclang_parser::yycheck_[] =
  {
      33,     3,     6,    42,   223,   225,   229,   230,   228,     3,
      43,     6,   232,   296,   233,   226,   227,    56,   226,   227,
     231,   226,   227,   231,    57,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   243,    88,    89,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   243,    88,    89,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    94,    88,
      89,   224,   243,   226,   227,    91,    92,    93,   282,   224,
     282,   226,   227,   226,   227,   226,   227,   229,   230,   226,
     227,   226,   227,   226,   227,   226,   227,   226,   227,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   297,
     298,    58,   385,   297,   298,    94,   243,   297,   298,   297,
     298,   297,   298,   297,   298,   243,   243,   297,   298,   297,
     298,   297,   298,   297,   298,   297,   298,   297,   298,   297,
     298,   243,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,     0,   243,   172,   173,     4,
       5,   297,   298,   297,   298,   297,   298,   297,   298,   297,
     298,   297,   298,   297,   298,   297,   298,   297,   298,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   297,   298,   297,   298,   243,
     243,   243,   243,   243,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   243,    88,    89,   296,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   347,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,     0,   394,
     395,   243,     4,     5,   243,   243,   243,   165,   348,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   253,   295,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   282,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   234,   235,   236,   237,   238,   239,   240,   241,
     242,    90,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177
  };

  const unsigned short int
  seclang_parser::yystos_[] =
  {
       0,     0,     4,     5,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   300,   301,
     302,   307,   226,   227,   226,   227,   231,   225,   228,   232,
     226,   227,   229,   230,   226,   227,   229,   230,   224,   226,
     227,    90,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   282,   303,   304,   311,   303,
     223,   233,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    88,    89,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   308,   309,   310,
     308,   303,   226,   227,   226,   227,   226,   227,   308,   303,
       0,   301,    94,   296,   224,   226,   227,   226,   227,   231,
     226,   227,   226,   227,   304,     3,   310,   310,   297,   298,
     297,   298,   297,   298,   297,   298,   297,   298,   297,   298,
     297,   298,   297,   298,   297,   298,   297,   298,   297,   298,
     297,   298,   297,   298,   297,   298,   297,   298,   297,   298,
     297,   298,   297,   298,   297,   298,   297,   298,   297,   298,
     297,   298,   297,   298,   297,   298,   309,     6,    94,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   305,   306,   296,    91,    92,
      93,   282,   311,   282,     8,     9,   310,   267,   306,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   303,   243,
     243,   243,   310,   310
  };

  const unsigned short int
  seclang_parser::yyr1_[] =
  {
       0,   299,   300,   300,   300,   301,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   303,   303,   304,   304,
     305,   305,   305,   305,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   308,   308,   309,   309,   309,   309,   309,   309,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311
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
       2,     2,     2,     1,     1,     1,     1,     2,     1,     2,
       1,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     4,     1,     2,     2,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
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
  "\"VARIABLE\"", "\"Dictionary element\"",
  "\"Dictionary element, selected by regexp\"", "$accept", "input", "line",
  "audit_log", "actions", "actions_may_quoted", "op", "op_before_init",
  "expression", "variables", "variables_may_be_quoted", "var", "act", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  seclang_parser::yyrline_[] =
  {
       0,   681,   681,   685,   686,   689,   694,   700,   706,   710,
     714,   720,   726,   732,   738,   744,   751,   755,   759,   765,
     769,   773,   778,   783,   788,   792,   799,   803,   810,   816,
     826,   835,   845,   854,   866,   870,   874,   878,   882,   886,
     891,   896,   900,   904,   909,   913,   917,   922,   927,   932,
     937,   941,   945,   949,   953,   957,   961,   965,   969,   973,
     977,   981,   985,   989,   993,   997,  1001,  1005,  1009,  1023,
    1024,  1048,  1067,  1082,  1087,  1134,  1138,  1142,  1146,  1150,
    1154,  1158,  1162,  1166,  1175,  1179,  1184,  1189,  1194,  1199,
    1204,  1217,  1230,  1243,  1269,  1295,  1307,  1327,  1355,  1360,
    1365,  1370,  1375,  1379,  1383,  1387,  1391,  1395,  1399,  1400,
    1401,  1413,  1419,  1423,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1436,  1444,  1448,  1455,  1460,  1466,  1472,  1478,  1485,
    1495,  1499,  1503,  1507,  1511,  1515,  1519,  1523,  1527,  1531,
    1535,  1539,  1543,  1547,  1551,  1555,  1559,  1563,  1567,  1571,
    1575,  1579,  1583,  1587,  1591,  1595,  1599,  1603,  1607,  1611,
    1615,  1619,  1623,  1627,  1631,  1635,  1639,  1643,  1647,  1651,
    1655,  1659,  1663,  1667,  1671,  1675,  1679,  1683,  1687,  1691,
    1695,  1699,  1703,  1707,  1711,  1715,  1719,  1723,  1727,  1731,
    1736,  1740,  1744,  1749,  1753,  1757,  1762,  1766,  1770,  1775,
    1779,  1783,  1790,  1794,  1798,  1802,  1806,  1810,  1814,  1818,
    1822,  1826,  1830,  1834,  1838,  1842,  1846,  1850,  1854,  1858,
    1862,  1866,  1870,  1874,  1878,  1882,  1886,  1890,  1894,  1898,
    1902,  1906,  1910,  1914,  1918,  1922,  1926,  1930,  1934,  1938,
    1942,  1946,  1950,  1954,  1958,  1962,  1966,  1970,  1974,  1978,
    1982,  1986,  1990,  1994,  1998,  2002,  2006,  2010,  2018,  2025,
    2032,  2039,  2046,  2053,  2060,  2067,  2074,  2081,  2088,  2095,
    2105,  2109,  2113,  2117,  2121,  2125,  2129,  2133,  2138,  2143,
    2148,  2152,  2156,  2160,  2165,  2170,  2174,  2178,  2182,  2186,
    2190,  2194,  2198,  2202,  2206,  2210,  2215,  2219,  2224,  2228,
    2232,  2236,  2240,  2244,  2248,  2252,  2256,  2260,  2264,  2268,
    2272,  2276,  2280,  2284,  2288,  2292,  2296,  2300,  2304,  2308,
    2312,  2316,  2320,  2324,  2328,  2332,  2336,  2340,  2344,  2348,
    2352,  2356,  2360,  2364,  2368,  2372,  2376,  2380,  2384,  2388,
    2392,  2396,  2400,  2404,  2408,  2412,  2416,  2420,  2424,  2428,
    2432,  2436,  2440,  2444,  2448,  2452,  2456,  2460,  2464,  2468,
    2472,  2476,  2480,  2484
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
#line 5273 "seclang-parser.cc" // lalr1.cc:1167
#line 2490 "seclang-parser.yy" // lalr1.cc:1168


void yy::seclang_parser::error (const location_type& l, const std::string& m) {
    driver.error (l, m);
}
