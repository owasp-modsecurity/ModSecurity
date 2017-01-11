// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file y.tab.h
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_SECLANG_PARSER_HH_INCLUDED
# define YY_YY_SECLANG_PARSER_HH_INCLUDED
// //                    "%code requires" blocks.
#line 10 "seclang-parser.yy" // lalr1.cc:377

#include <string>
#include <iterator>

namespace ModSecurity {
namespace Parser {
class Driver;
}
}


#include "src/actions/accuracy.h"
#include "src/actions/audit_log.h"
#include "src/actions/capture.h"
#include "src/actions/chain.h"
#include "src/actions/ctl/audit_log_parts.h"
#include "src/actions/ctl/request_body_access.h"
#include "src/actions/ctl/request_body_processor_json.h"
#include "src/actions/ctl/request_body_processor_xml.h"
#include "src/actions/ctl/rule_remove_by_id.h"
#include "src/actions/ctl/rule_remove_target_by_id.h"
#include "src/actions/ctl/rule_remove_target_by_tag.h"
#include "src/actions/data/status.h"
#include "src/actions/disruptive/allow.h"
#include "src/actions/disruptive/block.h"
#include "src/actions/disruptive/deny.h"
#include "src/actions/disruptive/pass.h"
#include "src/actions/disruptive/redirect.h"
#include "src/actions/init_col.h"
#include "src/actions/log_data.h"
#include "src/actions/log.h"
#include "src/actions/maturity.h"
#include "src/actions/msg.h"
#include "src/actions/multi_match.h"
#include "src/actions/no_audit_log.h"
#include "src/actions/no_log.h"
#include "src/actions/phase.h"
#include "src/actions/rev.h"
#include "src/actions/rule_id.h"
#include "src/actions/set_sid.h"
#include "src/actions/set_uid.h"
#include "src/actions/set_var.h"
#include "src/actions/severity.h"
#include "src/actions/skip_after.h"
#include "src/actions/skip.h"
#include "src/actions/tag.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/ver.h"
#include "src/actions/xmlns.h"

#include "src/actions/transformations/hex_encode.h"
#include "src/actions/transformations/parity_even_7bit.h"
#include "src/actions/transformations/utf8_to_unicode.h"
#include "src/actions/transformations/parity_zero_7bit.h"
#include "src/actions/transformations/sql_hex_decode.h"
#include "src/actions/transformations/replace_comments.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/url_decode.h"
#include "src/actions/transformations/lower_case.h"
#include "src/actions/transformations/hex_decode.h"
#include "src/actions/transformations/url_encode.h"
#include "src/actions/transformations/js_decode.h"
#include "src/actions/transformations/url_decode_uni.h"
#include "src/actions/transformations/parity_odd_7bit.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/transformations/trim_right.h"
#include "src/actions/transformations/escape_seq_decode.h"
#include "src/actions/transformations/base64_decode_ext.h"
#include "src/actions/transformations/base64_decode.h"
#include "src/actions/transformations/trim.h"
#include "src/actions/transformations/cmd_line.h"
#include "src/actions/transformations/replace_nulls.h"
#include "src/actions/transformations/md5.h"
#include "src/actions/transformations/length.h"
#include "src/actions/transformations/sha1.h"
#include "src/actions/transformations/compress_whitespace.h"
#include "src/actions/transformations/normalise_path_win.h"
#include "src/actions/transformations/remove_nulls.h"
#include "src/actions/transformations/remove_comments.h"
#include "src/actions/transformations/normalise_path.h"
#include "src/actions/transformations/html_entity_decode.h"
#include "src/actions/transformations/trim_left.h"
#include "src/actions/transformations/remove_comments_char.h"
#include "src/actions/transformations/base64_encode.h"
#include "src/actions/transformations/remove_whitespace.h"
#include "src/actions/transformations/css_decode.h"

#include "src/operators/begins_with.h"
#include "src/operators/contains.h"
#include "src/operators/contains_word.h"
#include "src/operators/detect_sqli.h"
#include "src/operators/detect_xss.h"
#include "src/operators/ends_with.h"
#include "src/operators/eq.h"
#include "src/operators/fuzzy_hash.h"
#include "src/operators/ge.h"
#include "src/operators/geo_lookup.h"
#include "src/operators/gsblookup.h"
#include "src/operators/gt.h"
#include "src/operators/inspect_file.h"
#include "src/operators/ip_match_f.h"
#include "src/operators/ip_match_from_file.h"
#include "src/operators/ip_match.h"
#include "src/operators/le.h"
#include "src/operators/lt.h"
#include "src/operators/no_match.h"
#include "src/operators/operator.h"
#include "src/operators/pm_f.h"
#include "src/operators/pm_from_file.h"
#include "src/operators/pm.h"
#include "src/operators/rbl.h"
#include "src/operators/rsub.h"
#include "src/operators/rx.h"
#include "src/operators/str_eq.h"
#include "src/operators/str_match.h"
#include "src/operators/unconditional_match.h"
#include "src/operators/validate_byte_range.h"
#include "src/operators/validate_dtd.h"
#include "src/operators/validate_hash.h"
#include "src/operators/validate_schema.h"
#include "src/operators/validate_url_encoding.h"
#include "src/operators/validate_utf8_encoding.h"
#include "src/operators/verify_cc.h"
#include "src/operators/verify_cpf.h"
#include "src/operators/verify_ssn.h"
#include "src/operators/within.h"


#include "modsecurity/audit_log.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_properties.h"
#include "modsecurity/rule.h"
#include "src/operators/operator.h"
#include "src/utils/geo_lookup.h"
#include "src/utils/string.h"
#include "src/utils/system.h"
#include "src/variables/xml.h"
#include "src/variables/duration.h"
#include "src/variables/env.h"
#include "src/variables/highest_severity.h"
#include "src/variables/modsec_build.h"
#include "src/variables/remote_user.h"
#include "src/variables/rule.h"
#include "src/variables/time.h"
#include "src/variables/time_day.h"
#include "src/variables/time_epoch.h"
#include "src/variables/time_hour.h"
#include "src/variables/time_min.h"
#include "src/variables/time_mon.h"
#include "src/variables/time_sec.h"
#include "src/variables/time_wday.h"
#include "src/variables/time_year.h"
#include "src/variables/tx.h"
#include "src/variables/variations/count.h"
#include "src/variables/variations/exclusion.h"


using modsecurity::ModSecurity;
using modsecurity::Rule;
using modsecurity::Utils::GeoLookup;
using modsecurity::Variables::Duration;
using modsecurity::Variables::Env;
using modsecurity::Variables::HighestSeverity;
using modsecurity::Variables::ModsecBuild;
using modsecurity::Variables::RemoteUser;
using modsecurity::Variables::TimeDay;
using modsecurity::Variables::TimeEpoch;
using modsecurity::Variables::TimeHour;
using modsecurity::Variables::TimeMin;
using modsecurity::Variables::TimeMon;
using modsecurity::Variables::TimeSec;
using modsecurity::Variables::TimeWDay;
using modsecurity::Variables::TimeYear;
using modsecurity::Variables::Time;
using modsecurity::Variables::Tx;
using modsecurity::Variables::Variable;
using modsecurity::Variables::Variations::Count;
using modsecurity::Variables::Variations::Exclusion;
using modsecurity::Variables::XML;

using namespace modsecurity;

using modsecurity::operators::Operator;



#define CHECK_VARIATION_DECL \
    Variable *var = NULL; \
    bool t = false;

#define CHECK_VARIATION(a) \
    if (var == NULL) { \
        if (name.at(0) == std::string(#a).at(0)) { \
            name.erase(0, 1); \
            t = true ; \
        } \
    } else { \
        t = false; \
    } \
    if (t)


#define ACTION_NOT_SUPPORTED(a, b) \
    driver.error(b, "Action: " + std::string(a) + " is not yet supported."); \
    YYERROR;


#define OPERATOR_NOT_SUPPORTED(a, b) \
    driver.error(b, "Operator: " + std::string(a) + " is not yet supported."); \
    YYERROR;


#define ACTION_INIT(a, b) \
    std::string error; \
    if (a->init(&error) == false) { \
        driver.error(b, error); \
        YYERROR; \
    }

#define OPERATOR_CONTAINER(a, b) \
    std::unique_ptr<Operator> c(b); \
    a = std::move(c);

#define ACTION_CONTAINER(a, b) \
    std::unique_ptr<actions::Action> c(b); \
    a = std::move(c);



#line 275 "seclang-parser.hh" // lalr1.cc:377

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 352 "seclang-parser.hh" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::move((T&)t));
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class seclang_parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // "Accuracy"
      // "Allow"
      // "Append"
      // "AuditLog"
      // "Block"
      // "Capture"
      // "Chain"
      // "ACTION_CTL_AUDIT_ENGINE"
      // "ACTION_CTL_AUDIT_LOG_PARTS"
      // "ACTION_CTL_BDY_JSON"
      // "ACTION_CTL_BDY_XML"
      // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      // "ACTION_CTL_REQUEST_BODY_ACCESS"
      // "ACTION_CTL_RULE_ENGINE"
      // "ACTION_CTL_RULE_REMOVE_BY_ID"
      // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      // "Deny"
      // "DeprecateVar"
      // "Drop"
      // "Exec"
      // "ExpireVar"
      // "Id"
      // "InitCol"
      // "Log"
      // "LogData"
      // "Maturity"
      // "Msg"
      // "MultiMatch"
      // "NoAuditLog"
      // "NoLog"
      // "Pass"
      // "Pause"
      // "Phase"
      // "Prepend"
      // "Proxy"
      // "Redirect"
      // "Rev"
      // "SanatiseArg"
      // "SanatiseMatched"
      // "SanatiseMatchedBytes"
      // "SanatiseRequestHeader"
      // "SanatiseResponseHeader"
      // "SetEnv"
      // "SetSrc"
      // "SetSid"
      // "SetUID"
      // "SetVar"
      // "Severity"
      // "Skip"
      // "SkipAfter"
      // "Status"
      // "Tag"
      // "ACTION_TRANSFORMATION_CMD_LINE"
      // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      // "ACTION_TRANSFORMATION_CSS_DECODE"
      // "ACTION_TRANSFORMATION_HEX_ENCODE"
      // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      // "ACTION_TRANSFORMATION_JS_DECODE"
      // "ACTION_TRANSFORMATION_LENGTH"
      // "ACTION_TRANSFORMATION_LOWERCASE"
      // "ACTION_TRANSFORMATION_MD5"
      // "ACTION_TRANSFORMATION_NONE"
      // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      // "ACTION_TRANSFORMATION_SHA1"
      // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      // "ACTION_TRANSFORMATION_TRIM"
      // "ACTION_TRANSFORMATION_URL_DECODE"
      // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      // "Ver"
      // "xmlns"
      // "CONFIG_COMPONENT_SIG"
      // "CONFIG_DIR_AUDIT_DIR"
      // "CONFIG_DIR_AUDIT_DIR_MOD"
      // "CONFIG_DIR_AUDIT_ENG"
      // "CONFIG_DIR_AUDIT_FLE_MOD"
      // "CONFIG_DIR_AUDIT_LOG"
      // "CONFIG_DIR_AUDIT_LOG2"
      // "CONFIG_DIR_AUDIT_LOG_P"
      // "CONFIG_DIR_AUDIT_STS"
      // "CONFIG_DIR_AUDIT_TPE"
      // "CONFIG_DIR_DEBUG_LOG"
      // "CONFIG_DIR_DEBUG_LVL"
      // "CONFIG_DIR_GEO_DB"
      // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      // "CONFIG_DIR_REQ_BODY"
      // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      // "CONFIG_DIR_REQ_BODY_LIMIT"
      // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      // "CONFIG_DIR_RES_BODY"
      // "CONFIG_DIR_RES_BODY_LIMIT"
      // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      // "CONFIG_DIR_RULE_ENG"
      // "CONFIG_DIR_SEC_ACTION"
      // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      // "CONFIG_DIR_SEC_MARKER"
      // "CONFIG_DIR_UNICODE_MAP_FILE"
      // "CONFIG_SEC_COLLECTION_TIMEOUT"
      // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      // "CONFIG_UPDLOAD_KEEP_FILES"
      // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      // "CONFIG_UPLOAD_DIR"
      // "CONFIG_UPLOAD_FILE_LIMIT"
      // "CONFIG_UPLOAD_FILE_MODE"
      // "CONFIG_VALUE_ABORT"
      // "CONFIG_VALUE_DETC"
      // "CONFIG_VALUE_HTTPS"
      // "CONFIG_VALUE_OFF"
      // "CONFIG_VALUE_ON"
      // "CONFIG_VALUE_PARALLEL"
      // "CONFIG_VALUE_PROCESS_PARTIAL"
      // "CONFIG_VALUE_REJECT"
      // "CONFIG_VALUE_RELEVANT_ONLY"
      // "CONFIG_VALUE_SERIAL"
      // "CONFIG_VALUE_WARN"
      // "CONFIG_XML_EXTERNAL_ENTITY"
      // "CONGIG_DIR_RESPONSE_BODY_MP"
      // "CONGIG_DIR_SEC_ARG_SEP"
      // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      // "CONGIG_DIR_SEC_DATA_DIR"
      // "CONGIG_DIR_SEC_STATUS_ENGINE"
      // "CONGIG_DIR_SEC_TMP_DIR"
      // "DIRECTIVE"
      // "DIRECTIVE_SECRULESCRIPT"
      // "FREE_TEXT"
      // "NOT"
      // "OPERATOR"
      // "OPERATOR_BEGINS_WITH"
      // "OPERATOR_CONTAINS"
      // "OPERATOR_CONTAINS_WORD"
      // "OPERATOR_DETECT_SQLI"
      // "OPERATOR_DETECT_XSS"
      // "OPERATOR_ENDS_WITH"
      // "OPERATOR_EQ"
      // "OPERATOR_FUZZY_HASH"
      // "OPERATOR_GE"
      // "OPERATOR_GEOLOOKUP"
      // "OPERATOR_GSB_LOOKUP"
      // "OPERATOR_GT"
      // "OPERATOR_INSPECT_FILE"
      // "OPERATOR_IP_MATCH"
      // "OPERATOR_IP_MATCH_FROM_FILE"
      // "OPERATOR_LE"
      // "OPERATOR_LT"
      // "OPERATOR_PM"
      // "OPERATOR_PM_FROM_FILE"
      // "OPERATOR_RBL"
      // "OPERATOR_RSUB"
      // "OPERATOR_RX"
      // "Operator Rx"
      // "OPERATOR_STR_EQ"
      // "OPERATOR_STR_MATCH"
      // "OPERATOR_UNCONDITIONAL_MATCH"
      // "OPERATOR_VALIDATE_BYTE_RANGE"
      // "OPERATOR_VALIDATE_DTD"
      // "OPERATOR_VALIDATE_HASH"
      // "OPERATOR_VALIDATE_SCHEMA"
      // "OPERATOR_VALIDATE_URL_ENCODING"
      // "OPERATOR_VALIDATE_UTF8_ENCODING"
      // "OPERATOR_VERIFY_CC"
      // "OPERATOR_VERIFY_CPF"
      // "OPERATOR_VERIFY_SSN"
      // "OPERATOR_WITHIN"
      // "OP_QUOTE"
      // "QUOTATION_MARK"
      // "RUN_TIME_VAR_BLD"
      // "RUN_TIME_VAR_DUR"
      // "RUN_TIME_VAR_ENV"
      // "RUN_TIME_VAR_HSV"
      // "RUN_TIME_VAR_REMOTE_USER"
      // "RUN_TIME_VAR_RULE"
      // "RUN_TIME_VAR_TIME"
      // "RUN_TIME_VAR_TIME_DAY"
      // "RUN_TIME_VAR_TIME_EPOCH"
      // "RUN_TIME_VAR_TIME_HOUR"
      // "RUN_TIME_VAR_TIME_MIN"
      // "RUN_TIME_VAR_TIME_MON"
      // "RUN_TIME_VAR_TIME_SEC"
      // "RUN_TIME_VAR_TIME_WDAY"
      // "RUN_TIME_VAR_TIME_YEAR"
      // "RUN_TIME_VAR_XML"
      // "VARIABLE"
      // "VARIABLE_COL"
      // "VARIABLE_STATUS"
      // "VARIABLE_TX"
      char dummy1[sizeof(std::string)];

      // op
      // op_before_init
      char dummy2[sizeof(std::unique_ptr<Operator>)];

      // var
      char dummy3[sizeof(std::unique_ptr<Variable>)];

      // act
      char dummy4[sizeof(std::unique_ptr<actions::Action>)];

      // variables
      char dummy5[sizeof(std::unique_ptr<std::vector<std::unique_ptr<Variable> > > )];

      // actions
      // actions_may_quoted
      char dummy6[sizeof(std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > > )];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_END = 0,
        TOK_COMMA = 258,
        TOK_PIPE = 259,
        TOK_NEW_LINE = 260,
        TOK_ACTION_ACCURACY = 261,
        TOK_ACTION_ALLOW = 262,
        TOK_ACTION_APPEND = 263,
        TOK_ACTION_AUDIT_LOG = 264,
        TOK_ACTION_BLOCK = 265,
        TOK_ACTION_CAPTURE = 266,
        TOK_ACTION_CHAIN = 267,
        TOK_ACTION_CTL_AUDIT_ENGINE = 268,
        TOK_ACTION_CTL_AUDIT_LOG_PARTS = 269,
        TOK_ACTION_CTL_BDY_JSON = 270,
        TOK_ACTION_CTL_BDY_XML = 271,
        TOK_ACTION_CTL_FORCE_REQ_BODY_VAR = 272,
        TOK_ACTION_CTL_REQUEST_BODY_ACCESS = 273,
        TOK_ACTION_CTL_RULE_ENGINE = 274,
        TOK_ACTION_CTL_RULE_REMOVE_BY_ID = 275,
        TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID = 276,
        TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG = 277,
        TOK_ACTION_DENY = 278,
        TOK_ACTION_DEPRECATE_VAR = 279,
        TOK_ACTION_DROP = 280,
        TOK_ACTION_EXEC = 281,
        TOK_ACTION_EXPIRE_VAR = 282,
        TOK_ACTION_ID = 283,
        TOK_ACTION_INITCOL = 284,
        TOK_ACTION_LOG = 285,
        TOK_ACTION_LOG_DATA = 286,
        TOK_ACTION_MATURITY = 287,
        TOK_ACTION_MSG = 288,
        TOK_ACTION_MULTI_MATCH = 289,
        TOK_ACTION_NO_AUDIT_LOG = 290,
        TOK_ACTION_NO_LOG = 291,
        TOK_ACTION_PASS = 292,
        TOK_ACTION_PAUSE = 293,
        TOK_ACTION_PHASE = 294,
        TOK_ACTION_PREPEND = 295,
        TOK_ACTION_PROXY = 296,
        TOK_ACTION_REDIRECT = 297,
        TOK_ACTION_REV = 298,
        TOK_ACTION_SANATISE_ARG = 299,
        TOK_ACTION_SANATISE_MATCHED = 300,
        TOK_ACTION_SANATISE_MATCHED_BYTES = 301,
        TOK_ACTION_SANATISE_REQUEST_HEADER = 302,
        TOK_ACTION_SANATISE_RESPONSE_HEADER = 303,
        TOK_ACTION_SETENV = 304,
        TOK_ACTION_SETRSC = 305,
        TOK_ACTION_SETSID = 306,
        TOK_ACTION_SETUID = 307,
        TOK_ACTION_SETVAR = 308,
        TOK_ACTION_SEVERITY = 309,
        TOK_ACTION_SKIP = 310,
        TOK_ACTION_SKIP_AFTER = 311,
        TOK_ACTION_STATUS = 312,
        TOK_ACTION_TAG = 313,
        TOK_ACTION_TRANSFORMATION_CMD_LINE = 314,
        TOK_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE = 315,
        TOK_ACTION_TRANSFORMATION_CSS_DECODE = 316,
        TOK_ACTION_TRANSFORMATION_HEX_ENCODE = 317,
        TOK_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE = 318,
        TOK_ACTION_TRANSFORMATION_JS_DECODE = 319,
        TOK_ACTION_TRANSFORMATION_LENGTH = 320,
        TOK_ACTION_TRANSFORMATION_LOWERCASE = 321,
        TOK_ACTION_TRANSFORMATION_MD5 = 322,
        TOK_ACTION_TRANSFORMATION_NONE = 323,
        TOK_ACTION_TRANSFORMATION_NORMALISE_PATH = 324,
        TOK_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN = 325,
        TOK_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT = 326,
        TOK_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT = 327,
        TOK_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT = 328,
        TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS = 329,
        TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR = 330,
        TOK_ACTION_TRANSFORMATION_REMOVE_NULLS = 331,
        TOK_ACTION_TRANSFORMATION_REMOVE_WHITESPACE = 332,
        TOK_ACTION_TRANSFORMATION_REPLACE_COMMENTS = 333,
        TOK_ACTION_TRANSFORMATION_REPLACE_NULLS = 334,
        TOK_ACTION_TRANSFORMATION_SHA1 = 335,
        TOK_ACTION_TRANSFORMATION_SQL_HEX_DECODE = 336,
        TOK_ACTION_TRANSFORMATION_TRIM = 337,
        TOK_ACTION_TRANSFORMATION_URL_DECODE = 338,
        TOK_ACTION_TRANSFORMATION_URL_DECODE_UNI = 339,
        TOK_ACTION_TRANSFORMATION_UTF8_TO_UNICODE = 340,
        TOK_ACTION_VER = 341,
        TOK_ACTION_XMLNS = 342,
        TOK_CONFIG_COMPONENT_SIG = 343,
        TOK_CONFIG_DIR_AUDIT_DIR = 344,
        TOK_CONFIG_DIR_AUDIT_DIR_MOD = 345,
        TOK_CONFIG_DIR_AUDIT_ENG = 346,
        TOK_CONFIG_DIR_AUDIT_FLE_MOD = 347,
        TOK_CONFIG_DIR_AUDIT_LOG = 348,
        TOK_CONFIG_DIR_AUDIT_LOG2 = 349,
        TOK_CONFIG_DIR_AUDIT_LOG_P = 350,
        TOK_CONFIG_DIR_AUDIT_STS = 351,
        TOK_CONFIG_DIR_AUDIT_TPE = 352,
        TOK_CONFIG_DIR_DEBUG_LOG = 353,
        TOK_CONFIG_DIR_DEBUG_LVL = 354,
        TOK_CONFIG_DIR_GEO_DB = 355,
        TOK_CONFIG_DIR_PCRE_MATCH_LIMIT = 356,
        TOK_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION = 357,
        TOK_CONFIG_DIR_REQ_BODY = 358,
        TOK_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT = 359,
        TOK_CONFIG_DIR_REQ_BODY_LIMIT = 360,
        TOK_CONFIG_DIR_REQ_BODY_LIMIT_ACTION = 361,
        TOK_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT = 362,
        TOK_CONFIG_DIR_RES_BODY = 363,
        TOK_CONFIG_DIR_RES_BODY_LIMIT = 364,
        TOK_CONFIG_DIR_RES_BODY_LIMIT_ACTION = 365,
        TOK_CONFIG_DIR_RULE_ENG = 366,
        TOK_CONFIG_DIR_SEC_ACTION = 367,
        TOK_CONFIG_DIR_SEC_DEFAULT_ACTION = 368,
        TOK_CONFIG_DIR_SEC_MARKER = 369,
        TOK_CONFIG_DIR_UNICODE_MAP_FILE = 370,
        TOK_CONFIG_SEC_COLLECTION_TIMEOUT = 371,
        TOK_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION = 372,
        TOK_CONFIG_SEC_RULE_REMOVE_BY_ID = 373,
        TOK_CONFIG_UPDLOAD_KEEP_FILES = 374,
        TOK_CONFIG_UPDLOAD_SAVE_TMP_FILES = 375,
        TOK_CONFIG_UPLOAD_DIR = 376,
        TOK_CONFIG_UPLOAD_FILE_LIMIT = 377,
        TOK_CONFIG_UPLOAD_FILE_MODE = 378,
        TOK_CONFIG_VALUE_ABORT = 379,
        TOK_CONFIG_VALUE_DETC = 380,
        TOK_CONFIG_VALUE_HTTPS = 381,
        TOK_CONFIG_VALUE_OFF = 382,
        TOK_CONFIG_VALUE_ON = 383,
        TOK_CONFIG_VALUE_PARALLEL = 384,
        TOK_CONFIG_VALUE_PROCESS_PARTIAL = 385,
        TOK_CONFIG_VALUE_REJECT = 386,
        TOK_CONFIG_VALUE_RELEVANT_ONLY = 387,
        TOK_CONFIG_VALUE_SERIAL = 388,
        TOK_CONFIG_VALUE_WARN = 389,
        TOK_CONFIG_XML_EXTERNAL_ENTITY = 390,
        TOK_CONGIG_DIR_RESPONSE_BODY_MP = 391,
        TOK_CONGIG_DIR_SEC_ARG_SEP = 392,
        TOK_CONGIG_DIR_SEC_COOKIE_FORMAT = 393,
        TOK_CONGIG_DIR_SEC_DATA_DIR = 394,
        TOK_CONGIG_DIR_SEC_STATUS_ENGINE = 395,
        TOK_CONGIG_DIR_SEC_TMP_DIR = 396,
        TOK_DIRECTIVE = 397,
        TOK_DIRECTIVE_SECRULESCRIPT = 398,
        TOK_FREE_TEXT = 399,
        TOK_NOT = 400,
        TOK_OPERATOR = 401,
        TOK_OPERATOR_BEGINS_WITH = 402,
        TOK_OPERATOR_CONTAINS = 403,
        TOK_OPERATOR_CONTAINS_WORD = 404,
        TOK_OPERATOR_DETECT_SQLI = 405,
        TOK_OPERATOR_DETECT_XSS = 406,
        TOK_OPERATOR_ENDS_WITH = 407,
        TOK_OPERATOR_EQ = 408,
        TOK_OPERATOR_FUZZY_HASH = 409,
        TOK_OPERATOR_GE = 410,
        TOK_OPERATOR_GEOLOOKUP = 411,
        TOK_OPERATOR_GSB_LOOKUP = 412,
        TOK_OPERATOR_GT = 413,
        TOK_OPERATOR_INSPECT_FILE = 414,
        TOK_OPERATOR_IP_MATCH = 415,
        TOK_OPERATOR_IP_MATCH_FROM_FILE = 416,
        TOK_OPERATOR_LE = 417,
        TOK_OPERATOR_LT = 418,
        TOK_OPERATOR_PM = 419,
        TOK_OPERATOR_PM_FROM_FILE = 420,
        TOK_OPERATOR_RBL = 421,
        TOK_OPERATOR_RSUB = 422,
        TOK_OPERATOR_RX = 423,
        TOK_OPERATOR_RX_CONTENT_ONLY = 424,
        TOK_OPERATOR_STR_EQ = 425,
        TOK_OPERATOR_STR_MATCH = 426,
        TOK_OPERATOR_UNCONDITIONAL_MATCH = 427,
        TOK_OPERATOR_VALIDATE_BYTE_RANGE = 428,
        TOK_OPERATOR_VALIDATE_DTD = 429,
        TOK_OPERATOR_VALIDATE_HASH = 430,
        TOK_OPERATOR_VALIDATE_SCHEMA = 431,
        TOK_OPERATOR_VALIDATE_URL_ENCODING = 432,
        TOK_OPERATOR_VALIDATE_UTF8_ENCODING = 433,
        TOK_OPERATOR_VERIFY_CC = 434,
        TOK_OPERATOR_VERIFY_CPF = 435,
        TOK_OPERATOR_VERIFY_SSN = 436,
        TOK_OPERATOR_WITHIN = 437,
        TOK_OP_QUOTE = 438,
        TOK_QUOTATION_MARK = 439,
        TOK_RUN_TIME_VAR_BLD = 440,
        TOK_RUN_TIME_VAR_DUR = 441,
        TOK_RUN_TIME_VAR_ENV = 442,
        TOK_RUN_TIME_VAR_HSV = 443,
        TOK_RUN_TIME_VAR_REMOTE_USER = 444,
        TOK_RUN_TIME_VAR_RULE = 445,
        TOK_RUN_TIME_VAR_TIME = 446,
        TOK_RUN_TIME_VAR_TIME_DAY = 447,
        TOK_RUN_TIME_VAR_TIME_EPOCH = 448,
        TOK_RUN_TIME_VAR_TIME_HOUR = 449,
        TOK_RUN_TIME_VAR_TIME_MIN = 450,
        TOK_RUN_TIME_VAR_TIME_MON = 451,
        TOK_RUN_TIME_VAR_TIME_SEC = 452,
        TOK_RUN_TIME_VAR_TIME_WDAY = 453,
        TOK_RUN_TIME_VAR_TIME_YEAR = 454,
        TOK_RUN_TIME_VAR_XML = 455,
        TOK_VARIABLE = 456,
        TOK_VARIABLE_COL = 457,
        TOK_VARIABLE_STATUS = 458,
        TOK_VARIABLE_TX = 459
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<Operator> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<Variable> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<actions::Action> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_PIPE (const location_type& l);

    static inline
    symbol_type
    make_NEW_LINE (const location_type& l);

    static inline
    symbol_type
    make_ACTION_ACCURACY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_ALLOW (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_APPEND (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_AUDIT_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_BLOCK (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CAPTURE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CHAIN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_AUDIT_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_AUDIT_LOG_PARTS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_BDY_JSON (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_BDY_XML (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_FORCE_REQ_BODY_VAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_REQUEST_BODY_ACCESS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_DENY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_DEPRECATE_VAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_DROP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_EXEC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_EXPIRE_VAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_INITCOL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_LOG_DATA (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_MATURITY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_MSG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_MULTI_MATCH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_NO_AUDIT_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_NO_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_PASS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_PAUSE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_PHASE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_PREPEND (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_PROXY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_REDIRECT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_REV (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANATISE_ARG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANATISE_MATCHED (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANATISE_MATCHED_BYTES (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANATISE_REQUEST_HEADER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANATISE_RESPONSE_HEADER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETENV (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETRSC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETSID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETUID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETVAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SEVERITY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SKIP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SKIP_AFTER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_STATUS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TAG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_CMD_LINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_CSS_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_HEX_ENCODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_JS_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_LENGTH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_LOWERCASE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_MD5 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_NONE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_NORMALISE_PATH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REMOVE_COMMENTS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REMOVE_NULLS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REMOVE_WHITESPACE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REPLACE_COMMENTS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_REPLACE_NULLS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_SHA1 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_SQL_HEX_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_TRIM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_URL_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_URL_DECODE_UNI (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_UTF8_TO_UNICODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_VER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_XMLNS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_COMPONENT_SIG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_DIR_MOD (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_ENG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_FLE_MOD (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_LOG2 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_LOG_P (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_STS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_TPE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_DEBUG_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_DEBUG_LVL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_GEO_DB (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_PCRE_MATCH_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_REQ_BODY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_REQ_BODY_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_RES_BODY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_RES_BODY_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_RES_BODY_LIMIT_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_RULE_ENG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_SEC_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_SEC_DEFAULT_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_SEC_MARKER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_UNICODE_MAP_FILE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_COLLECTION_TIMEOUT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_UPDLOAD_KEEP_FILES (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_UPDLOAD_SAVE_TMP_FILES (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_UPLOAD_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_UPLOAD_FILE_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_UPLOAD_FILE_MODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_ABORT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_DETC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_HTTPS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_OFF (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_ON (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_PARALLEL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_PROCESS_PARTIAL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_REJECT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_RELEVANT_ONLY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_SERIAL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_VALUE_WARN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_XML_EXTERNAL_ENTITY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_RESPONSE_BODY_MP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_ARG_SEP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_COOKIE_FORMAT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_DATA_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_STATUS_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_TMP_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DIRECTIVE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DIRECTIVE_SECRULESCRIPT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_FREE_TEXT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NOT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_BEGINS_WITH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_CONTAINS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_CONTAINS_WORD (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_DETECT_SQLI (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_DETECT_XSS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_ENDS_WITH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_EQ (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_FUZZY_HASH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GEOLOOKUP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GSB_LOOKUP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_INSPECT_FILE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_IP_MATCH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_IP_MATCH_FROM_FILE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_LE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_LT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_PM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_PM_FROM_FILE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RBL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RSUB (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RX (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RX_CONTENT_ONLY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_STR_EQ (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_STR_MATCH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_UNCONDITIONAL_MATCH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_BYTE_RANGE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_DTD (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_HASH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_SCHEMA (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_URL_ENCODING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_UTF8_ENCODING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_CC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_CPF (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_SSN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_WITHIN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OP_QUOTE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_QUOTATION_MARK (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_BLD (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_DUR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_ENV (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_HSV (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_REMOTE_USER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_RULE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_DAY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_EPOCH (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_HOUR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_MIN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_MON (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_SEC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_WDAY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_TIME_YEAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_XML (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_VARIABLE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_COL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_STATUS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_TX (const std::string& v, const location_type& l);


    /// Build a parser object.
    seclang_parser (modsecurity::Parser::Driver& driver_yyarg);
    virtual ~seclang_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    seclang_parser (const seclang_parser&);
    seclang_parser& operator= (const seclang_parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned short int yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 444,     ///< Last index in yytable_.
      yynnts_ = 12,  ///< Number of nonterminal symbols.
      yyfinal_ = 186, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 205  ///< Number of tokens.
    };


    // User arguments.
    modsecurity::Parser::Driver& driver;
  };

  // Symbol number corresponding to token number t.
  inline
  seclang_parser::token_number_type
  seclang_parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204
    };
    const unsigned int user_token_number_max_ = 459;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  seclang_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  seclang_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  seclang_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
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
        value.copy< std::string > (other.value);
        break;

      case 211: // op
      case 212: // op_before_init
        value.copy< std::unique_ptr<Operator> > (other.value);
        break;

      case 215: // var
        value.copy< std::unique_ptr<Variable> > (other.value);
        break;

      case 216: // act
        value.copy< std::unique_ptr<actions::Action> > (other.value);
        break;

      case 214: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (other.value);
        break;

      case 209: // actions
      case 210: // actions_may_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
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
        value.copy< std::string > (v);
        break;

      case 211: // op
      case 212: // op_before_init
        value.copy< std::unique_ptr<Operator> > (v);
        break;

      case 215: // var
        value.copy< std::unique_ptr<Variable> > (v);
        break;

      case 216: // act
        value.copy< std::unique_ptr<actions::Action> > (v);
        break;

      case 214: // variables
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (v);
        break;

      case 209: // actions
      case 210: // actions_may_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<Operator> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<Variable> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<actions::Action> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  seclang_parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  seclang_parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
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
        value.template destroy< std::string > ();
        break;

      case 211: // op
      case 212: // op_before_init
        value.template destroy< std::unique_ptr<Operator> > ();
        break;

      case 215: // var
        value.template destroy< std::unique_ptr<Variable> > ();
        break;

      case 216: // act
        value.template destroy< std::unique_ptr<actions::Action> > ();
        break;

      case 214: // variables
        value.template destroy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 209: // actions
      case 210: // actions_may_quoted
        value.template destroy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  seclang_parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  seclang_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
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
        value.move< std::string > (s.value);
        break;

      case 211: // op
      case 212: // op_before_init
        value.move< std::unique_ptr<Operator> > (s.value);
        break;

      case 215: // var
        value.move< std::unique_ptr<Variable> > (s.value);
        break;

      case 216: // act
        value.move< std::unique_ptr<actions::Action> > (s.value);
        break;

      case 214: // variables
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (s.value);
        break;

      case 209: // actions
      case 210: // actions_may_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  seclang_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  seclang_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  seclang_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  seclang_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  seclang_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  seclang_parser::by_type::type_get () const
  {
    return type;
  }

  inline
  seclang_parser::token_type
  seclang_parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  seclang_parser::symbol_type
  seclang_parser::make_END (const location_type& l)
  {
    return symbol_type (token::TOK_END, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::TOK_COMMA, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_PIPE (const location_type& l)
  {
    return symbol_type (token::TOK_PIPE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_NEW_LINE (const location_type& l)
  {
    return symbol_type (token::TOK_NEW_LINE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_ACCURACY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_ACCURACY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_ALLOW (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_ALLOW, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_APPEND (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_APPEND, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_AUDIT_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_AUDIT_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_BLOCK (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_BLOCK, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CAPTURE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CAPTURE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CHAIN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CHAIN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_AUDIT_ENGINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_AUDIT_ENGINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_AUDIT_LOG_PARTS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_AUDIT_LOG_PARTS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_BDY_JSON (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_BDY_JSON, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_BDY_XML (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_BDY_XML, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_FORCE_REQ_BODY_VAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_FORCE_REQ_BODY_VAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_REQUEST_BODY_ACCESS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_REQUEST_BODY_ACCESS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_ENGINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_ENGINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_REMOVE_BY_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_DENY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_DENY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_DEPRECATE_VAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_DEPRECATE_VAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_DROP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_DROP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_EXEC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_EXEC, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_EXPIRE_VAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_EXPIRE_VAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_INITCOL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_INITCOL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_LOG_DATA (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_LOG_DATA, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_MATURITY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_MATURITY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_MSG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_MSG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_MULTI_MATCH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_MULTI_MATCH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_NO_AUDIT_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_NO_AUDIT_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_NO_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_NO_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_PASS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_PASS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_PAUSE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_PAUSE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_PHASE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_PHASE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_PREPEND (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_PREPEND, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_PROXY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_PROXY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_REDIRECT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_REDIRECT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_REV (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_REV, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANATISE_ARG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANATISE_ARG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANATISE_MATCHED (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANATISE_MATCHED, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANATISE_MATCHED_BYTES (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANATISE_MATCHED_BYTES, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANATISE_REQUEST_HEADER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANATISE_REQUEST_HEADER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANATISE_RESPONSE_HEADER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANATISE_RESPONSE_HEADER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETENV (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETENV, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETRSC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETRSC, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETSID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETSID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETUID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETUID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETVAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETVAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SEVERITY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SEVERITY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SKIP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SKIP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SKIP_AFTER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SKIP_AFTER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_STATUS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_STATUS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TAG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TAG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_CMD_LINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_CMD_LINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_CSS_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_CSS_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_HEX_ENCODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_HEX_ENCODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_JS_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_JS_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_LENGTH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_LENGTH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_LOWERCASE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_LOWERCASE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_MD5 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_MD5, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_NONE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_NONE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_NORMALISE_PATH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_NORMALISE_PATH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REMOVE_COMMENTS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REMOVE_NULLS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REMOVE_NULLS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REMOVE_WHITESPACE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REMOVE_WHITESPACE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REPLACE_COMMENTS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REPLACE_COMMENTS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_REPLACE_NULLS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_REPLACE_NULLS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_SHA1 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_SHA1, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_SQL_HEX_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_SQL_HEX_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_TRIM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_TRIM, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_URL_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_URL_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_URL_DECODE_UNI (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_URL_DECODE_UNI, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_UTF8_TO_UNICODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_UTF8_TO_UNICODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_VER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_VER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_XMLNS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_XMLNS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_COMPONENT_SIG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_COMPONENT_SIG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_DIR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_DIR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_DIR_MOD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_DIR_MOD, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_ENG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_ENG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_FLE_MOD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_FLE_MOD, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_LOG2 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_LOG2, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_LOG_P (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_LOG_P, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_STS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_STS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_TPE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_TPE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_DEBUG_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_DEBUG_LOG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_DEBUG_LVL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_DEBUG_LVL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_GEO_DB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_GEO_DB, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_PCRE_MATCH_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_PCRE_MATCH_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_REQ_BODY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_REQ_BODY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_REQ_BODY_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_REQ_BODY_LIMIT_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_REQ_BODY_LIMIT_ACTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_RES_BODY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_RES_BODY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_RES_BODY_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_RES_BODY_LIMIT_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_RES_BODY_LIMIT_ACTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_RULE_ENG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_RULE_ENG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_SEC_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_SEC_ACTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_SEC_DEFAULT_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_SEC_DEFAULT_ACTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_SEC_MARKER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_SEC_MARKER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_UNICODE_MAP_FILE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_UNICODE_MAP_FILE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_COLLECTION_TIMEOUT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_COLLECTION_TIMEOUT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_REMOVE_BY_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_UPDLOAD_KEEP_FILES (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_UPDLOAD_KEEP_FILES, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_UPDLOAD_SAVE_TMP_FILES (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_UPDLOAD_SAVE_TMP_FILES, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_UPLOAD_DIR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_UPLOAD_DIR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_UPLOAD_FILE_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_UPLOAD_FILE_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_UPLOAD_FILE_MODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_UPLOAD_FILE_MODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_ABORT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_ABORT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_DETC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_DETC, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_HTTPS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_HTTPS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_OFF (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_OFF, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_ON (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_ON, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_PARALLEL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_PARALLEL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_PROCESS_PARTIAL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_PROCESS_PARTIAL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_REJECT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_REJECT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_RELEVANT_ONLY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_RELEVANT_ONLY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_SERIAL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_SERIAL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_VALUE_WARN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_VALUE_WARN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_XML_EXTERNAL_ENTITY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_XML_EXTERNAL_ENTITY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_RESPONSE_BODY_MP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_RESPONSE_BODY_MP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_SEC_ARG_SEP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_SEC_ARG_SEP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_SEC_COOKIE_FORMAT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_SEC_COOKIE_FORMAT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_SEC_DATA_DIR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_SEC_DATA_DIR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_SEC_STATUS_ENGINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_SEC_STATUS_ENGINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_SEC_TMP_DIR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_SEC_TMP_DIR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_DIRECTIVE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_DIRECTIVE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_DIRECTIVE_SECRULESCRIPT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_DIRECTIVE_SECRULESCRIPT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_FREE_TEXT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_FREE_TEXT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_NOT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_NOT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_BEGINS_WITH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_BEGINS_WITH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_CONTAINS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_CONTAINS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_CONTAINS_WORD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_CONTAINS_WORD, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_DETECT_SQLI (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_DETECT_SQLI, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_DETECT_XSS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_DETECT_XSS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_ENDS_WITH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_ENDS_WITH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_EQ (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_EQ, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_FUZZY_HASH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_FUZZY_HASH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GEOLOOKUP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GEOLOOKUP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GSB_LOOKUP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GSB_LOOKUP, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_INSPECT_FILE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_INSPECT_FILE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_IP_MATCH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_IP_MATCH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_IP_MATCH_FROM_FILE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_IP_MATCH_FROM_FILE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_LE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_LE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_LT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_LT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_PM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_PM, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_PM_FROM_FILE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_PM_FROM_FILE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RBL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RBL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RSUB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RSUB, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RX (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RX, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RX_CONTENT_ONLY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RX_CONTENT_ONLY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_STR_EQ (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_STR_EQ, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_STR_MATCH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_STR_MATCH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_UNCONDITIONAL_MATCH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_UNCONDITIONAL_MATCH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_BYTE_RANGE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_BYTE_RANGE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_DTD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_DTD, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_HASH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_HASH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_SCHEMA (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_SCHEMA, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_URL_ENCODING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_URL_ENCODING, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_UTF8_ENCODING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_UTF8_ENCODING, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_CC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_CC, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_CPF (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_CPF, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_SSN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_SSN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_WITHIN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_WITHIN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OP_QUOTE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_OP_QUOTE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_QUOTATION_MARK (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_QUOTATION_MARK, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_BLD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_BLD, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_DUR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_DUR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_ENV (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_ENV, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_HSV (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_HSV, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_REMOTE_USER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_REMOTE_USER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_RULE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_RULE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_DAY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_DAY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_EPOCH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_EPOCH, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_HOUR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_HOUR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_MIN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_MIN, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_MON (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_MON, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_SEC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_SEC, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_WDAY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_WDAY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_TIME_YEAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_TIME_YEAR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_XML (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_XML, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_COL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_COL, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_STATUS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_STATUS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_TX (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_TX, v, l);
  }



} // yy
#line 4516 "seclang-parser.hh" // lalr1.cc:377




#endif // !YY_YY_SECLANG_PARSER_HH_INCLUDED
