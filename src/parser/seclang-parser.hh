// A Bison parser, made by GNU Bison 3.1.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

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
#line 10 "seclang-parser.yy" // lalr1.cc:380

#include <string>
#include <iterator>

namespace ModSecurity {
namespace Parser {
class Driver;
}
}

#include "src/rule_script.h"

#include "src/actions/accuracy.h"
#include "src/actions/audit_log.h"
#include "src/actions/block.h"
#include "src/actions/capture.h"
#include "src/actions/chain.h"
#include "src/actions/ctl/audit_log_parts.h"
#include "src/actions/ctl/request_body_access.h"
#include "src/actions/ctl/rule_engine.h"
#include "src/actions/ctl/request_body_processor_json.h"
#include "src/actions/ctl/request_body_processor_xml.h"
#include "src/actions/ctl/request_body_processor_urlencoded.h"
#include "src/actions/ctl/rule_remove_by_id.h"
#include "src/actions/ctl/rule_remove_by_tag.h"
#include "src/actions/ctl/rule_remove_target_by_id.h"
#include "src/actions/ctl/rule_remove_target_by_tag.h"
#include "src/actions/data/status.h"
#include "src/actions/disruptive/allow.h"
#include "src/actions/disruptive/deny.h"
#include "src/actions/disruptive/pass.h"
#include "src/actions/disruptive/redirect.h"
#include "src/actions/init_col.h"
#include "src/actions/exec.h"
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
#include "src/actions/set_env.h"
#include "src/actions/set_rsc.h"
#include "src/actions/set_sid.h"
#include "src/actions/set_uid.h"
#include "src/actions/set_var.h"
#include "src/actions/severity.h"
#include "src/actions/skip_after.h"
#include "src/actions/skip.h"
#include "src/actions/tag.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/transformations/url_decode_uni.h"
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
#include "src/actions/transformations/upper_case.h"
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
#include "src/variables/args_combined_size.h"
#include "src/variables/args_get.h"
#include "src/variables/args_get_names.h"
#include "src/variables/args.h"
#include "src/variables/args_names.h"
#include "src/variables/args_post.h"
#include "src/variables/args_post_names.h"
#include "src/variables/auth_type.h"
#include "src/variables/duration.h"
#include "src/variables/env.h"
#include "src/variables/files_combined_size.h"
#include "src/variables/files.h"
#include "src/variables/files_names.h"
#include "src/variables/files_sizes.h"
#include "src/variables/files_tmp_content.h"
#include "src/variables/files_tmp_names.h"
#include "src/variables/full_request.h"
#include "src/variables/full_request_length.h"
#include "src/variables/geo.h"
#include "src/variables/highest_severity.h"
#include "src/variables/inbound_data_error.h"
#include "src/variables/matched_var.h"
#include "src/variables/matched_var_name.h"
#include "src/variables/matched_vars.h"
#include "src/variables/matched_vars_names.h"
#include "src/variables/modsec_build.h"
#include "src/variables/multipart_boundary_quoted.h"
#include "src/variables/multipart_boundary_whitespace.h"
#include "src/variables/multipart_crlf_lf_lines.h"
#include "src/variables/multipart_data_after.h"
#include "src/variables/multipart_data_before.h"
#include "src/variables/multipart_file_limit_exceeded.h"
#include "src/variables/multipart_file_name.h"
#include "src/variables/multipart_header_folding.h"
#include "src/variables/multipart_invalid_header_folding.h"
#include "src/variables/multipart_invalid_part.h"
#include "src/variables/multipart_invalid_quoting.h"
#include "src/variables/multipart_lf_line.h"
#include "src/variables/multipart_missing_semicolon.h"
#include "src/variables/multipart_name.h"
#include "src/variables/multipart_strict_error.h"
#include "src/variables/multipart_unmatched_boundary.h"
#include "src/variables/outbound_data_error.h"
#include "src/variables/path_info.h"
#include "src/variables/query_string.h"
#include "src/variables/remote_addr.h"
#include "src/variables/remote_host.h"
#include "src/variables/remote_port.h"
#include "src/variables/remote_user.h"
#include "src/variables/reqbody_error.h"
#include "src/variables/reqbody_error_msg.h"
#include "src/variables/reqbody_processor_error.h"
#include "src/variables/reqbody_processor_error_msg.h"
#include "src/variables/reqbody_processor.h"
#include "src/variables/request_base_name.h"
#include "src/variables/request_body.h"
#include "src/variables/request_body_length.h"
#include "src/variables/request_cookies.h"
#include "src/variables/request_cookies_names.h"
#include "src/variables/request_file_name.h"
#include "src/variables/request_headers.h"
#include "src/variables/request_headers_names.h"
#include "src/variables/request_line.h"
#include "src/variables/request_method.h"
#include "src/variables/request_protocol.h"
#include "src/variables/request_uri.h"
#include "src/variables/request_uri_raw.h"
#include "src/variables/resource.h"
#include "src/variables/response_body.h"
#include "src/variables/response_content_length.h"
#include "src/variables/response_content_type.h"
#include "src/variables/response_headers.h"
#include "src/variables/response_headers_names.h"
#include "src/variables/response_protocol.h"
#include "src/variables/response_status.h"
#include "src/variables/rule.h"
#include "src/variables/server_addr.h"
#include "src/variables/server_name.h"
#include "src/variables/server_port.h"
#include "src/variables/session_id.h"
#include "src/variables/web_app_id.h"
#include "src/variables/time_day.h"
#include "src/variables/time_epoch.h"
#include "src/variables/time.h"
#include "src/variables/time_hour.h"
#include "src/variables/time_min.h"
#include "src/variables/time_mon.h"
#include "src/variables/time_sec.h"
#include "src/variables/time_wday.h"
#include "src/variables/time_year.h"
#include "src/variables/tx.h"
#include "src/variables/unique_id.h"
#include "src/variables/url_encoded_error.h"
#include "src/variables/user.h"
#include "src/variables/user_id.h"
#include "src/variables/variable.h"
#include "src/variables/xml.h"
#include "src/variables/ip.h"
#include "src/variables/global.h"
#include "src/variables/session.h"
#include "src/variables/status.h"

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
using modsecurity::Variables::Tx_DictElement;
using modsecurity::Variables::Tx_NoDictElement;
using modsecurity::Variables::Tx_DictElementRegexp;

using modsecurity::Variables::Ip_DictElement;
using modsecurity::Variables::Ip_NoDictElement;
using modsecurity::Variables::Ip_DictElementRegexp;

using modsecurity::Variables::Global_DictElement;
using modsecurity::Variables::Global_NoDictElement;
using modsecurity::Variables::Global_DictElementRegexp;

using modsecurity::Variables::Session_DictElement;
using modsecurity::Variables::Session_NoDictElement;
using modsecurity::Variables::Session_DictElementRegexp;



using modsecurity::Variables::Variable;
using modsecurity::Variables::VariableModificatorExclusion;
using modsecurity::Variables::VariableModificatorCount;
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

#define VARIABLE_CONTAINER(a, b) \
    std::unique_ptr<Variable> c(b); \
    a = std::move(c);


#line 385 "seclang-parser.hh" // lalr1.cc:380

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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 469 "seclang-parser.hh" // lalr1.cc:380



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
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
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
      return *new (yyas_<T> ()) T ();
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
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (yytypeid_);
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
      // "ACTION_CTL_BDY_URLENCODED"
      // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      // "ACTION_CTL_REQUEST_BODY_ACCESS"
      // "ACTION_CTL_RULE_REMOVE_BY_ID"
      // "ACTION_CTL_RULE_REMOVE_BY_TAG"
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
      // "SanitiseArg"
      // "SanitiseMatched"
      // "SanitiseMatchedBytes"
      // "SanitiseRequestHeader"
      // "SanitiseResponseHeader"
      // "SetEnv"
      // "SetRsc"
      // "SetSid"
      // "SetUID"
      // "Severity"
      // "Skip"
      // "SkipAfter"
      // "Status"
      // "Tag"
      // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      // "ACTION_TRANSFORMATION_CMD_LINE"
      // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      // "ACTION_TRANSFORMATION_CSS_DECODE"
      // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      // "ACTION_TRANSFORMATION_HEX_ENCODE"
      // "ACTION_TRANSFORMATION_HEX_DECODE"
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
      // "ACTION_TRANSFORMATION_TRIM_LEFT"
      // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      // "ACTION_TRANSFORMATION_UPPERCASE"
      // "ACTION_TRANSFORMATION_URL_ENCODE"
      // "ACTION_TRANSFORMATION_URL_DECODE"
      // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      // "Ver"
      // "xmlns"
      // "CONFIG_COMPONENT_SIG"
      // "CONFIG_CONN_ENGINE"
      // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      // "CONFIG_SEC_WEB_APP_ID"
      // "CONFIG_SEC_SERVER_SIG"
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
      // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      // "CONFIG_SEC_HASH_ENGINE"
      // "CONFIG_SEC_HASH_KEY"
      // "CONFIG_SEC_HASH_PARAM"
      // "CONFIG_SEC_HASH_METHOD_RX"
      // "CONFIG_SEC_HASH_METHOD_PM"
      // "CONFIG_SEC_CHROOT_DIR"
      // "CONFIG_DIR_GEO_DB"
      // "CONFIG_DIR_GSB_DB"
      // "CONFIG_SEC_GUARDIAN_LOG"
      // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      // "CONFIG_SEC_SENSOR_ID"
      // "CONFIG_DIR_REQ_BODY"
      // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      // "CONFIG_DIR_REQ_BODY_LIMIT"
      // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      // "CONFIG_DIR_RES_BODY"
      // "CONFIG_DIR_RES_BODY_LIMIT"
      // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      // "CONFIG_SEC_RULE_INHERITANCE"
      // "CONFIG_SEC_RULE_PERF_TIME"
      // "CONFIG_DIR_RULE_ENG"
      // "CONFIG_DIR_SEC_ACTION"
      // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      // "CONFIG_DIR_SEC_MARKER"
      // "CONFIG_DIR_UNICODE_MAP_FILE"
      // "CONFIG_DIR_UNICODE_CODE_PAGE"
      // "CONFIG_SEC_COLLECTION_TIMEOUT"
      // "CONFIG_SEC_HTTP_BLKEY"
      // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
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
      // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      // "CONGIG_DIR_SEC_DATA_DIR"
      // "CONGIG_DIR_SEC_STATUS_ENGINE"
      // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      // "CONGIG_DIR_SEC_TMP_DIR"
      // "DIRECTIVE"
      // "DIRECTIVE_SECRULESCRIPT"
      // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      // "QUOTATION_MARK"
      // "RUN_TIME_VAR_BLD"
      // "RUN_TIME_VAR_DUR"
      // "RUN_TIME_VAR_HSV"
      // "RUN_TIME_VAR_REMOTE_USER"
      // "RUN_TIME_VAR_TIME"
      // "RUN_TIME_VAR_TIME_DAY"
      // "RUN_TIME_VAR_TIME_EPOCH"
      // "RUN_TIME_VAR_TIME_HOUR"
      // "RUN_TIME_VAR_TIME_MIN"
      // "RUN_TIME_VAR_TIME_MON"
      // "RUN_TIME_VAR_TIME_SEC"
      // "RUN_TIME_VAR_TIME_WDAY"
      // "RUN_TIME_VAR_TIME_YEAR"
      // "VARIABLE"
      // "Dictionary element"
      // "Dictionary element, selected by regexp"
      char dummy1[sizeof(std::string)];

      // op
      // op_before_init
      char dummy2[sizeof(std::unique_ptr<Operator>)];

      // run_time_string
      char dummy3[sizeof(std::unique_ptr<RunTimeString>)];

      // var
      char dummy4[sizeof(std::unique_ptr<Variable>)];

      // act
      // setvar_action
      char dummy5[sizeof(std::unique_ptr<actions::Action>)];

      // variables
      // variables_pre_process
      // variables_may_be_quoted
      char dummy6[sizeof(std::unique_ptr<std::vector<std::unique_ptr<Variable> > > )];

      // actions
      // actions_may_quoted
      char dummy7[sizeof(std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > > )];
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
        TOK_CONFIG_CONTENT_INJECTION = 259,
        TOK_CONGIG_DIR_RESPONSE_BODY_MP_CLEAR = 260,
        TOK_PIPE = 261,
        TOK_NEW_LINE = 262,
        TOK_VAR_COUNT = 263,
        TOK_VAR_EXCLUSION = 264,
        TOK_VARIABLE_ARGS = 265,
        TOK_VARIABLE_ARGS_POST = 266,
        TOK_VARIABLE_ARGS_GET = 267,
        TOK_VARIABLE_FILES_SIZES = 268,
        TOK_VARIABLE_FILES_NAMES = 269,
        TOK_VARIABLE_FILES_TMP_CONTENT = 270,
        TOK_VARIABLE_MULTIPART_FILENAME = 271,
        TOK_VARIABLE_MULTIPART_NAME = 272,
        TOK_VARIABLE_MATCHED_VARS_NAMES = 273,
        TOK_VARIABLE_MATCHED_VARS = 274,
        TOK_VARIABLE_FILES = 275,
        TOK_VARIABLE_REQUEST_COOKIES = 276,
        TOK_VARIABLE_REQUEST_HEADERS = 277,
        TOK_VARIABLE_RESPONSE_HEADERS = 278,
        TOK_VARIABLE_GEO = 279,
        TOK_VARIABLE_REQUEST_COOKIES_NAMES = 280,
        TOK_VARIABLE_ARGS_COMBINED_SIZE = 281,
        TOK_VARIABLE_ARGS_GET_NAMES = 282,
        TOK_VARIABLE_RULE = 283,
        TOK_VARIABLE_ARGS_NAMES = 284,
        TOK_VARIABLE_ARGS_POST_NAMES = 285,
        TOK_VARIABLE_AUTH_TYPE = 286,
        TOK_VARIABLE_FILES_COMBINED_SIZE = 287,
        TOK_VARIABLE_FILES_TMP_NAMES = 288,
        TOK_VARIABLE_FULL_REQUEST = 289,
        TOK_VARIABLE_FULL_REQUEST_LENGTH = 290,
        TOK_VARIABLE_INBOUND_DATA_ERROR = 291,
        TOK_VARIABLE_MATCHED_VAR = 292,
        TOK_VARIABLE_MATCHED_VAR_NAME = 293,
        TOK_VARIABLE_MULTIPART_BOUNDARY_QUOTED = 294,
        TOK_VARIABLE_MULTIPART_BOUNDARY_WHITESPACE = 295,
        TOK_VARIABLE_MULTIPART_CRLF_LF_LINES = 296,
        TOK_VARIABLE_MULTIPART_DATA_AFTER = 297,
        TOK_VARIABLE_MULTIPART_DATA_BEFORE = 298,
        TOK_VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED = 299,
        TOK_VARIABLE_MULTIPART_HEADER_FOLDING = 300,
        TOK_VARIABLE_MULTIPART_INVALID_HEADER_FOLDING = 301,
        TOK_VARIABLE_MULTIPART_INVALID_PART = 302,
        TOK_VARIABLE_MULTIPART_INVALID_QUOTING = 303,
        TOK_VARIABLE_MULTIPART_LF_LINE = 304,
        TOK_VARIABLE_MULTIPART_MISSING_SEMICOLON = 305,
        TOK_VARIABLE_MULTIPART_SEMICOLON_MISSING = 306,
        TOK_VARIABLE_MULTIPART_STRICT_ERROR = 307,
        TOK_VARIABLE_MULTIPART_UNMATCHED_BOUNDARY = 308,
        TOK_VARIABLE_OUTBOUND_DATA_ERROR = 309,
        TOK_VARIABLE_PATH_INFO = 310,
        TOK_VARIABLE_QUERY_STRING = 311,
        TOK_VARIABLE_REMOTE_ADDR = 312,
        TOK_VARIABLE_REMOTE_HOST = 313,
        TOK_VARIABLE_REMOTE_PORT = 314,
        TOK_VARIABLE_REQBODY_ERROR_MSG = 315,
        TOK_VARIABLE_REQBODY_ERROR = 316,
        TOK_VARIABLE_REQBODY_PROCESSOR_ERROR_MSG = 317,
        TOK_VARIABLE_REQBODY_PROCESSOR_ERROR = 318,
        TOK_VARIABLE_REQBODY_PROCESSOR = 319,
        TOK_VARIABLE_REQUEST_BASENAME = 320,
        TOK_VARIABLE_REQUEST_BODY_LENGTH = 321,
        TOK_VARIABLE_REQUEST_BODY = 322,
        TOK_VARIABLE_REQUEST_FILE_NAME = 323,
        TOK_VARIABLE_REQUEST_HEADERS_NAMES = 324,
        TOK_VARIABLE_REQUEST_LINE = 325,
        TOK_VARIABLE_REQUEST_METHOD = 326,
        TOK_VARIABLE_REQUEST_PROTOCOL = 327,
        TOK_VARIABLE_REQUEST_URI_RAW = 328,
        TOK_VARIABLE_REQUEST_URI = 329,
        TOK_VARIABLE_RESOURCE = 330,
        TOK_VARIABLE_RESPONSE_BODY = 331,
        TOK_VARIABLE_RESPONSE_CONTENT_LENGTH = 332,
        TOK_VARIABLE_RESPONSE_CONTENT_TYPE = 333,
        TOK_VARIABLE_RESPONSE_HEADERS_NAMES = 334,
        TOK_VARIABLE_RESPONSE_PROTOCOL = 335,
        TOK_VARIABLE_RESPONSE_STATUS = 336,
        TOK_VARIABLE_SERVER_ADDR = 337,
        TOK_VARIABLE_SERVER_NAME = 338,
        TOK_VARIABLE_SERVER_PORT = 339,
        TOK_VARIABLE_SESSION_ID = 340,
        TOK_VARIABLE_UNIQUE_ID = 341,
        TOK_VARIABLE_URL_ENCODED_ERROR = 342,
        TOK_VARIABLE_USER_ID = 343,
        TOK_VARIABLE_WEB_APP_ID = 344,
        TOK_VARIABLE_STATUS = 345,
        TOK_VARIABLE_STATUS_LINE = 346,
        TOK_VARIABLE_IP = 347,
        TOK_VARIABLE_GLOBAL = 348,
        TOK_VARIABLE_TX = 349,
        TOK_VARIABLE_SESSION = 350,
        TOK_VARIABLE_USER = 351,
        TOK_RUN_TIME_VAR_ENV = 352,
        TOK_RUN_TIME_VAR_XML = 353,
        TOK_ACTION_SETVAR = 354,
        TOK_SETVAR_OPERATION_EQUALS = 355,
        TOK_SETVAR_OPERATION_EQUALS_PLUS = 356,
        TOK_SETVAR_OPERATION_EQUALS_MINUS = 357,
        TOK_NOT = 358,
        TOK_OPERATOR_BEGINS_WITH = 359,
        TOK_OPERATOR_CONTAINS = 360,
        TOK_OPERATOR_CONTAINS_WORD = 361,
        TOK_OPERATOR_DETECT_SQLI = 362,
        TOK_OPERATOR_DETECT_XSS = 363,
        TOK_OPERATOR_ENDS_WITH = 364,
        TOK_OPERATOR_EQ = 365,
        TOK_OPERATOR_FUZZY_HASH = 366,
        TOK_OPERATOR_GEOLOOKUP = 367,
        TOK_OPERATOR_GE = 368,
        TOK_OPERATOR_GSB_LOOKUP = 369,
        TOK_OPERATOR_GT = 370,
        TOK_OPERATOR_INSPECT_FILE = 371,
        TOK_OPERATOR_IP_MATCH_FROM_FILE = 372,
        TOK_OPERATOR_IP_MATCH = 373,
        TOK_OPERATOR_LE = 374,
        TOK_OPERATOR_LT = 375,
        TOK_OPERATOR_PM_FROM_FILE = 376,
        TOK_OPERATOR_PM = 377,
        TOK_OPERATOR_RBL = 378,
        TOK_OPERATOR_RSUB = 379,
        TOK_OPERATOR_RX_CONTENT_ONLY = 380,
        TOK_OPERATOR_RX = 381,
        TOK_OPERATOR_STR_EQ = 382,
        TOK_OPERATOR_STR_MATCH = 383,
        TOK_OPERATOR_UNCONDITIONAL_MATCH = 384,
        TOK_OPERATOR_VALIDATE_BYTE_RANGE = 385,
        TOK_OPERATOR_VALIDATE_DTD = 386,
        TOK_OPERATOR_VALIDATE_HASH = 387,
        TOK_OPERATOR_VALIDATE_SCHEMA = 388,
        TOK_OPERATOR_VALIDATE_URL_ENCODING = 389,
        TOK_OPERATOR_VALIDATE_UTF8_ENCODING = 390,
        TOK_OPERATOR_VERIFY_CC = 391,
        TOK_OPERATOR_VERIFY_CPF = 392,
        TOK_OPERATOR_VERIFY_SSN = 393,
        TOK_OPERATOR_WITHIN = 394,
        TOK_CONFIG_DIR_AUDIT_LOG_FMT = 395,
        TOK_JSON = 396,
        TOK_NATIVE = 397,
        TOK_ACTION_CTL_RULE_ENGINE = 398,
        TOK_ACTION_ACCURACY = 399,
        TOK_ACTION_ALLOW = 400,
        TOK_ACTION_APPEND = 401,
        TOK_ACTION_AUDIT_LOG = 402,
        TOK_ACTION_BLOCK = 403,
        TOK_ACTION_CAPTURE = 404,
        TOK_ACTION_CHAIN = 405,
        TOK_ACTION_CTL_AUDIT_ENGINE = 406,
        TOK_ACTION_CTL_AUDIT_LOG_PARTS = 407,
        TOK_ACTION_CTL_BDY_JSON = 408,
        TOK_ACTION_CTL_BDY_XML = 409,
        TOK_ACTION_CTL_BDY_URLENCODED = 410,
        TOK_ACTION_CTL_FORCE_REQ_BODY_VAR = 411,
        TOK_ACTION_CTL_REQUEST_BODY_ACCESS = 412,
        TOK_ACTION_CTL_RULE_REMOVE_BY_ID = 413,
        TOK_ACTION_CTL_RULE_REMOVE_BY_TAG = 414,
        TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_ID = 415,
        TOK_ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG = 416,
        TOK_ACTION_DENY = 417,
        TOK_ACTION_DEPRECATE_VAR = 418,
        TOK_ACTION_DROP = 419,
        TOK_ACTION_EXEC = 420,
        TOK_ACTION_EXPIRE_VAR = 421,
        TOK_ACTION_ID = 422,
        TOK_ACTION_INITCOL = 423,
        TOK_ACTION_LOG = 424,
        TOK_ACTION_LOG_DATA = 425,
        TOK_ACTION_MATURITY = 426,
        TOK_ACTION_MSG = 427,
        TOK_ACTION_MULTI_MATCH = 428,
        TOK_ACTION_NO_AUDIT_LOG = 429,
        TOK_ACTION_NO_LOG = 430,
        TOK_ACTION_PASS = 431,
        TOK_ACTION_PAUSE = 432,
        TOK_ACTION_PHASE = 433,
        TOK_ACTION_PREPEND = 434,
        TOK_ACTION_PROXY = 435,
        TOK_ACTION_REDIRECT = 436,
        TOK_ACTION_REV = 437,
        TOK_ACTION_SANITISE_ARG = 438,
        TOK_ACTION_SANITISE_MATCHED = 439,
        TOK_ACTION_SANITISE_MATCHED_BYTES = 440,
        TOK_ACTION_SANITISE_REQUEST_HEADER = 441,
        TOK_ACTION_SANITISE_RESPONSE_HEADER = 442,
        TOK_ACTION_SETENV = 443,
        TOK_ACTION_SETRSC = 444,
        TOK_ACTION_SETSID = 445,
        TOK_ACTION_SETUID = 446,
        TOK_ACTION_SEVERITY = 447,
        TOK_ACTION_SKIP = 448,
        TOK_ACTION_SKIP_AFTER = 449,
        TOK_ACTION_STATUS = 450,
        TOK_ACTION_TAG = 451,
        TOK_ACTION_TRANSFORMATION_BASE_64_ENCODE = 452,
        TOK_ACTION_TRANSFORMATION_BASE_64_DECODE = 453,
        TOK_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT = 454,
        TOK_ACTION_TRANSFORMATION_CMD_LINE = 455,
        TOK_ACTION_TRANSFORMATION_COMPRESS_WHITESPACE = 456,
        TOK_ACTION_TRANSFORMATION_CSS_DECODE = 457,
        TOK_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE = 458,
        TOK_ACTION_TRANSFORMATION_HEX_ENCODE = 459,
        TOK_ACTION_TRANSFORMATION_HEX_DECODE = 460,
        TOK_ACTION_TRANSFORMATION_HTML_ENTITY_DECODE = 461,
        TOK_ACTION_TRANSFORMATION_JS_DECODE = 462,
        TOK_ACTION_TRANSFORMATION_LENGTH = 463,
        TOK_ACTION_TRANSFORMATION_LOWERCASE = 464,
        TOK_ACTION_TRANSFORMATION_MD5 = 465,
        TOK_ACTION_TRANSFORMATION_NONE = 466,
        TOK_ACTION_TRANSFORMATION_NORMALISE_PATH = 467,
        TOK_ACTION_TRANSFORMATION_NORMALISE_PATH_WIN = 468,
        TOK_ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT = 469,
        TOK_ACTION_TRANSFORMATION_PARITY_ODD_7_BIT = 470,
        TOK_ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT = 471,
        TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS = 472,
        TOK_ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR = 473,
        TOK_ACTION_TRANSFORMATION_REMOVE_NULLS = 474,
        TOK_ACTION_TRANSFORMATION_REMOVE_WHITESPACE = 475,
        TOK_ACTION_TRANSFORMATION_REPLACE_COMMENTS = 476,
        TOK_ACTION_TRANSFORMATION_REPLACE_NULLS = 477,
        TOK_ACTION_TRANSFORMATION_SHA1 = 478,
        TOK_ACTION_TRANSFORMATION_SQL_HEX_DECODE = 479,
        TOK_ACTION_TRANSFORMATION_TRIM = 480,
        TOK_ACTION_TRANSFORMATION_TRIM_LEFT = 481,
        TOK_ACTION_TRANSFORMATION_TRIM_RIGHT = 482,
        TOK_ACTION_TRANSFORMATION_UPPERCASE = 483,
        TOK_ACTION_TRANSFORMATION_URL_ENCODE = 484,
        TOK_ACTION_TRANSFORMATION_URL_DECODE = 485,
        TOK_ACTION_TRANSFORMATION_URL_DECODE_UNI = 486,
        TOK_ACTION_TRANSFORMATION_UTF8_TO_UNICODE = 487,
        TOK_ACTION_VER = 488,
        TOK_ACTION_XMLNS = 489,
        TOK_CONFIG_COMPONENT_SIG = 490,
        TOK_CONFIG_CONN_ENGINE = 491,
        TOK_CONFIG_SEC_ARGUMENT_SEPARATOR = 492,
        TOK_CONFIG_SEC_WEB_APP_ID = 493,
        TOK_CONFIG_SEC_SERVER_SIG = 494,
        TOK_CONFIG_DIR_AUDIT_DIR = 495,
        TOK_CONFIG_DIR_AUDIT_DIR_MOD = 496,
        TOK_CONFIG_DIR_AUDIT_ENG = 497,
        TOK_CONFIG_DIR_AUDIT_FLE_MOD = 498,
        TOK_CONFIG_DIR_AUDIT_LOG = 499,
        TOK_CONFIG_DIR_AUDIT_LOG2 = 500,
        TOK_CONFIG_DIR_AUDIT_LOG_P = 501,
        TOK_CONFIG_DIR_AUDIT_STS = 502,
        TOK_CONFIG_DIR_AUDIT_TPE = 503,
        TOK_CONFIG_DIR_DEBUG_LOG = 504,
        TOK_CONFIG_DIR_DEBUG_LVL = 505,
        TOK_CONFIG_SEC_CACHE_TRANSFORMATIONS = 506,
        TOK_CONFIG_SEC_DISABLE_BACKEND_COMPRESS = 507,
        TOK_CONFIG_SEC_HASH_ENGINE = 508,
        TOK_CONFIG_SEC_HASH_KEY = 509,
        TOK_CONFIG_SEC_HASH_PARAM = 510,
        TOK_CONFIG_SEC_HASH_METHOD_RX = 511,
        TOK_CONFIG_SEC_HASH_METHOD_PM = 512,
        TOK_CONFIG_SEC_CHROOT_DIR = 513,
        TOK_CONFIG_DIR_GEO_DB = 514,
        TOK_CONFIG_DIR_GSB_DB = 515,
        TOK_CONFIG_SEC_GUARDIAN_LOG = 516,
        TOK_CONFIG_DIR_PCRE_MATCH_LIMIT = 517,
        TOK_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION = 518,
        TOK_CONFIG_SEC_CONN_R_STATE_LIMIT = 519,
        TOK_CONFIG_SEC_CONN_W_STATE_LIMIT = 520,
        TOK_CONFIG_SEC_SENSOR_ID = 521,
        TOK_CONFIG_DIR_REQ_BODY = 522,
        TOK_CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT = 523,
        TOK_CONFIG_DIR_REQ_BODY_LIMIT = 524,
        TOK_CONFIG_DIR_REQ_BODY_LIMIT_ACTION = 525,
        TOK_CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT = 526,
        TOK_CONFIG_DIR_RES_BODY = 527,
        TOK_CONFIG_DIR_RES_BODY_LIMIT = 528,
        TOK_CONFIG_DIR_RES_BODY_LIMIT_ACTION = 529,
        TOK_CONFIG_SEC_RULE_INHERITANCE = 530,
        TOK_CONFIG_SEC_RULE_PERF_TIME = 531,
        TOK_CONFIG_DIR_RULE_ENG = 532,
        TOK_CONFIG_DIR_SEC_ACTION = 533,
        TOK_CONFIG_DIR_SEC_DEFAULT_ACTION = 534,
        TOK_CONFIG_DIR_SEC_MARKER = 535,
        TOK_CONFIG_DIR_UNICODE_MAP_FILE = 536,
        TOK_CONFIG_DIR_UNICODE_CODE_PAGE = 537,
        TOK_CONFIG_SEC_COLLECTION_TIMEOUT = 538,
        TOK_CONFIG_SEC_HTTP_BLKEY = 539,
        TOK_CONFIG_SEC_INTERCEPT_ON_ERROR = 540,
        TOK_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION = 541,
        TOK_CONFIG_SEC_RULE_REMOVE_BY_ID = 542,
        TOK_CONFIG_SEC_RULE_REMOVE_BY_MSG = 543,
        TOK_CONFIG_SEC_RULE_REMOVE_BY_TAG = 544,
        TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG = 545,
        TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG = 546,
        TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID = 547,
        TOK_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID = 548,
        TOK_CONFIG_UPDLOAD_KEEP_FILES = 549,
        TOK_CONFIG_UPDLOAD_SAVE_TMP_FILES = 550,
        TOK_CONFIG_UPLOAD_DIR = 551,
        TOK_CONFIG_UPLOAD_FILE_LIMIT = 552,
        TOK_CONFIG_UPLOAD_FILE_MODE = 553,
        TOK_CONFIG_VALUE_ABORT = 554,
        TOK_CONFIG_VALUE_DETC = 555,
        TOK_CONFIG_VALUE_HTTPS = 556,
        TOK_CONFIG_VALUE_OFF = 557,
        TOK_CONFIG_VALUE_ON = 558,
        TOK_CONFIG_VALUE_PARALLEL = 559,
        TOK_CONFIG_VALUE_PROCESS_PARTIAL = 560,
        TOK_CONFIG_VALUE_REJECT = 561,
        TOK_CONFIG_VALUE_RELEVANT_ONLY = 562,
        TOK_CONFIG_VALUE_SERIAL = 563,
        TOK_CONFIG_VALUE_WARN = 564,
        TOK_CONFIG_XML_EXTERNAL_ENTITY = 565,
        TOK_CONGIG_DIR_RESPONSE_BODY_MP = 566,
        TOK_CONGIG_DIR_SEC_ARG_SEP = 567,
        TOK_CONGIG_DIR_SEC_COOKIE_FORMAT = 568,
        TOK_CONFIG_SEC_COOKIEV0_SEPARATOR = 569,
        TOK_CONGIG_DIR_SEC_DATA_DIR = 570,
        TOK_CONGIG_DIR_SEC_STATUS_ENGINE = 571,
        TOK_CONFIG_SEC_STREAM_IN_BODY_INSPECTION = 572,
        TOK_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION = 573,
        TOK_CONGIG_DIR_SEC_TMP_DIR = 574,
        TOK_DIRECTIVE = 575,
        TOK_DIRECTIVE_SECRULESCRIPT = 576,
        TOK_FREE_TEXT_QUOTE_MACRO_EXPANSION = 577,
        TOK_QUOTATION_MARK = 578,
        TOK_RUN_TIME_VAR_BLD = 579,
        TOK_RUN_TIME_VAR_DUR = 580,
        TOK_RUN_TIME_VAR_HSV = 581,
        TOK_RUN_TIME_VAR_REMOTE_USER = 582,
        TOK_RUN_TIME_VAR_TIME = 583,
        TOK_RUN_TIME_VAR_TIME_DAY = 584,
        TOK_RUN_TIME_VAR_TIME_EPOCH = 585,
        TOK_RUN_TIME_VAR_TIME_HOUR = 586,
        TOK_RUN_TIME_VAR_TIME_MIN = 587,
        TOK_RUN_TIME_VAR_TIME_MON = 588,
        TOK_RUN_TIME_VAR_TIME_SEC = 589,
        TOK_RUN_TIME_VAR_TIME_WDAY = 590,
        TOK_RUN_TIME_VAR_TIME_YEAR = 591,
        TOK_VARIABLE = 592,
        TOK_DICT_ELEMENT = 593,
        TOK_DICT_ELEMENT_REGEXP = 594
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned short token_number_type;

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

  basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<Operator>& v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<RunTimeString>& v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<Variable>& v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<actions::Action>& v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<Variable> > > & v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > > & v, const location_type& l);


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
    make_CONFIG_CONTENT_INJECTION (const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_RESPONSE_BODY_MP_CLEAR (const location_type& l);

    static inline
    symbol_type
    make_PIPE (const location_type& l);

    static inline
    symbol_type
    make_NEW_LINE (const location_type& l);

    static inline
    symbol_type
    make_VAR_COUNT (const location_type& l);

    static inline
    symbol_type
    make_VAR_EXCLUSION (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_POST (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_GET (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES_SIZES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES_TMP_CONTENT (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_FILENAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_NAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MATCHED_VARS_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MATCHED_VARS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_COOKIES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_HEADERS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_HEADERS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_GEO (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_COOKIES_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_COMBINED_SIZE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_GET_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RULE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_ARGS_POST_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_AUTH_TYPE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES_COMBINED_SIZE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FILES_TMP_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FULL_REQUEST (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_FULL_REQUEST_LENGTH (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_INBOUND_DATA_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MATCHED_VAR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MATCHED_VAR_NAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_BOUNDARY_QUOTED (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_BOUNDARY_WHITESPACE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_CRLF_LF_LINES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_DATA_AFTER (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_DATA_BEFORE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_HEADER_FOLDING (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_INVALID_HEADER_FOLDING (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_INVALID_PART (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_INVALID_QUOTING (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_LF_LINE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_MISSING_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_SEMICOLON_MISSING (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_STRICT_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_MULTIPART_UNMATCHED_BOUNDARY (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_OUTBOUND_DATA_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_PATH_INFO (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_QUERY_STRING (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REMOTE_ADDR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REMOTE_HOST (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REMOTE_PORT (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQBODY_ERROR_MSG (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQBODY_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQBODY_PROCESSOR_ERROR_MSG (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQBODY_PROCESSOR_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQBODY_PROCESSOR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_BASENAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_BODY_LENGTH (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_BODY (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_FILE_NAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_HEADERS_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_LINE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_METHOD (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_PROTOCOL (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_URI_RAW (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_REQUEST_URI (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESOURCE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_BODY (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_CONTENT_LENGTH (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_CONTENT_TYPE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_HEADERS_NAMES (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_PROTOCOL (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_RESPONSE_STATUS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_SERVER_ADDR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_SERVER_NAME (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_SERVER_PORT (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_SESSION_ID (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_UNIQUE_ID (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_URL_ENCODED_ERROR (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_USER_ID (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_WEB_APP_ID (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_STATUS (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_STATUS_LINE (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_IP (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_GLOBAL (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_TX (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_SESSION (const location_type& l);

    static inline
    symbol_type
    make_VARIABLE_USER (const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_ENV (const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_XML (const location_type& l);

    static inline
    symbol_type
    make_ACTION_SETVAR (const location_type& l);

    static inline
    symbol_type
    make_SETVAR_OPERATION_EQUALS (const location_type& l);

    static inline
    symbol_type
    make_SETVAR_OPERATION_EQUALS_PLUS (const location_type& l);

    static inline
    symbol_type
    make_SETVAR_OPERATION_EQUALS_MINUS (const location_type& l);

    static inline
    symbol_type
    make_NOT (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_BEGINS_WITH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_CONTAINS (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_CONTAINS_WORD (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_DETECT_SQLI (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_DETECT_XSS (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_ENDS_WITH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_EQ (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_FUZZY_HASH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GEOLOOKUP (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GSB_LOOKUP (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_GT (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_INSPECT_FILE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_IP_MATCH_FROM_FILE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_IP_MATCH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_LE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_LT (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_PM_FROM_FILE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_PM (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RBL (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RSUB (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RX_CONTENT_ONLY (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_RX (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_STR_EQ (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_STR_MATCH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_UNCONDITIONAL_MATCH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_BYTE_RANGE (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_DTD (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_HASH (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_SCHEMA (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_URL_ENCODING (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VALIDATE_UTF8_ENCODING (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_CC (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_CPF (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_VERIFY_SSN (const location_type& l);

    static inline
    symbol_type
    make_OPERATOR_WITHIN (const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_AUDIT_LOG_FMT (const location_type& l);

    static inline
    symbol_type
    make_JSON (const location_type& l);

    static inline
    symbol_type
    make_NATIVE (const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_ENGINE (const location_type& l);

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
    make_ACTION_CTL_BDY_URLENCODED (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_FORCE_REQ_BODY_VAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_REQUEST_BODY_ACCESS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_CTL_RULE_REMOVE_BY_TAG (const std::string& v, const location_type& l);

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
    make_ACTION_SANITISE_ARG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANITISE_MATCHED (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANITISE_MATCHED_BYTES (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANITISE_REQUEST_HEADER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_SANITISE_RESPONSE_HEADER (const std::string& v, const location_type& l);

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
    make_ACTION_TRANSFORMATION_BASE_64_ENCODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_BASE_64_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT (const std::string& v, const location_type& l);

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
    make_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_HEX_ENCODE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_HEX_DECODE (const std::string& v, const location_type& l);

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
    make_ACTION_TRANSFORMATION_TRIM_LEFT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_TRIM_RIGHT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_UPPERCASE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ACTION_TRANSFORMATION_URL_ENCODE (const std::string& v, const location_type& l);

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
    make_CONFIG_CONN_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_ARGUMENT_SEPARATOR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_WEB_APP_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_SERVER_SIG (const std::string& v, const location_type& l);

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
    make_CONFIG_SEC_CACHE_TRANSFORMATIONS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_DISABLE_BACKEND_COMPRESS (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HASH_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HASH_KEY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HASH_PARAM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HASH_METHOD_RX (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HASH_METHOD_PM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_CHROOT_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_GEO_DB (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_GSB_DB (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_GUARDIAN_LOG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_PCRE_MATCH_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_CONN_R_STATE_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_CONN_W_STATE_LIMIT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_SENSOR_ID (const std::string& v, const location_type& l);

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
    make_CONFIG_SEC_RULE_INHERITANCE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_PERF_TIME (const std::string& v, const location_type& l);

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
    make_CONFIG_DIR_UNICODE_CODE_PAGE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_COLLECTION_TIMEOUT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_HTTP_BLKEY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_INTERCEPT_ON_ERROR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_REMOTE_RULES_FAIL_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_REMOVE_BY_MSG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_REMOVE_BY_TAG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID (const std::string& v, const location_type& l);

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
    make_CONFIG_SEC_COOKIEV0_SEPARATOR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_DATA_DIR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONGIG_DIR_SEC_STATUS_ENGINE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_STREAM_IN_BODY_INSPECTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION (const std::string& v, const location_type& l);

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
    make_FREE_TEXT_QUOTE_MACRO_EXPANSION (const std::string& v, const location_type& l);

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
    make_RUN_TIME_VAR_HSV (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RUN_TIME_VAR_REMOTE_USER (const std::string& v, const location_type& l);

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
    make_VARIABLE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DICT_ELEMENT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DICT_ELEMENT_REGEXP (const std::string& v, const location_type& l);


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

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned short yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned short yytable_[];

  static const short yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned short yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned short yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short yyrline_[];
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
      /// Copy construct (for efficiency).
      stack_symbol_type (const stack_symbol_type& that);
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
    void yypop_ (unsigned n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 3295,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyfinal_ = 336, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 340  ///< Number of tokens.
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
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339
    };
    const unsigned user_token_number_max_ = 594;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
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
  seclang_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
    , location ()
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    switch (other.type_get ())
    {
      case 144: // "Accuracy"
      case 145: // "Allow"
      case 146: // "Append"
      case 147: // "AuditLog"
      case 148: // "Block"
      case 149: // "Capture"
      case 150: // "Chain"
      case 151: // "ACTION_CTL_AUDIT_ENGINE"
      case 152: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 153: // "ACTION_CTL_BDY_JSON"
      case 154: // "ACTION_CTL_BDY_XML"
      case 155: // "ACTION_CTL_BDY_URLENCODED"
      case 156: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 157: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 158: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 160: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 162: // "Deny"
      case 163: // "DeprecateVar"
      case 164: // "Drop"
      case 165: // "Exec"
      case 166: // "ExpireVar"
      case 167: // "Id"
      case 168: // "InitCol"
      case 169: // "Log"
      case 170: // "LogData"
      case 171: // "Maturity"
      case 172: // "Msg"
      case 173: // "MultiMatch"
      case 174: // "NoAuditLog"
      case 175: // "NoLog"
      case 176: // "Pass"
      case 177: // "Pause"
      case 178: // "Phase"
      case 179: // "Prepend"
      case 180: // "Proxy"
      case 181: // "Redirect"
      case 182: // "Rev"
      case 183: // "SanitiseArg"
      case 184: // "SanitiseMatched"
      case 185: // "SanitiseMatchedBytes"
      case 186: // "SanitiseRequestHeader"
      case 187: // "SanitiseResponseHeader"
      case 188: // "SetEnv"
      case 189: // "SetRsc"
      case 190: // "SetSid"
      case 191: // "SetUID"
      case 192: // "Severity"
      case 193: // "Skip"
      case 194: // "SkipAfter"
      case 195: // "Status"
      case 196: // "Tag"
      case 197: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 200: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 201: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 202: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 204: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 206: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 207: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 208: // "ACTION_TRANSFORMATION_LENGTH"
      case 209: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 210: // "ACTION_TRANSFORMATION_MD5"
      case 211: // "ACTION_TRANSFORMATION_NONE"
      case 212: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 214: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 215: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 221: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 223: // "ACTION_TRANSFORMATION_SHA1"
      case 224: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 225: // "ACTION_TRANSFORMATION_TRIM"
      case 226: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 227: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 228: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 229: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 230: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 232: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 233: // "Ver"
      case 234: // "xmlns"
      case 235: // "CONFIG_COMPONENT_SIG"
      case 236: // "CONFIG_CONN_ENGINE"
      case 237: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 238: // "CONFIG_SEC_WEB_APP_ID"
      case 239: // "CONFIG_SEC_SERVER_SIG"
      case 240: // "CONFIG_DIR_AUDIT_DIR"
      case 241: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 242: // "CONFIG_DIR_AUDIT_ENG"
      case 243: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 244: // "CONFIG_DIR_AUDIT_LOG"
      case 245: // "CONFIG_DIR_AUDIT_LOG2"
      case 246: // "CONFIG_DIR_AUDIT_LOG_P"
      case 247: // "CONFIG_DIR_AUDIT_STS"
      case 248: // "CONFIG_DIR_AUDIT_TPE"
      case 249: // "CONFIG_DIR_DEBUG_LOG"
      case 250: // "CONFIG_DIR_DEBUG_LVL"
      case 251: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 252: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 253: // "CONFIG_SEC_HASH_ENGINE"
      case 254: // "CONFIG_SEC_HASH_KEY"
      case 255: // "CONFIG_SEC_HASH_PARAM"
      case 256: // "CONFIG_SEC_HASH_METHOD_RX"
      case 257: // "CONFIG_SEC_HASH_METHOD_PM"
      case 258: // "CONFIG_SEC_CHROOT_DIR"
      case 259: // "CONFIG_DIR_GEO_DB"
      case 260: // "CONFIG_DIR_GSB_DB"
      case 261: // "CONFIG_SEC_GUARDIAN_LOG"
      case 262: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 264: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 265: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 266: // "CONFIG_SEC_SENSOR_ID"
      case 267: // "CONFIG_DIR_REQ_BODY"
      case 268: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 270: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 271: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 272: // "CONFIG_DIR_RES_BODY"
      case 273: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 275: // "CONFIG_SEC_RULE_INHERITANCE"
      case 276: // "CONFIG_SEC_RULE_PERF_TIME"
      case 277: // "CONFIG_DIR_RULE_ENG"
      case 278: // "CONFIG_DIR_SEC_ACTION"
      case 279: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 280: // "CONFIG_DIR_SEC_MARKER"
      case 281: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 282: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 283: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 284: // "CONFIG_SEC_HTTP_BLKEY"
      case 285: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 286: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 287: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 288: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 290: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 291: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 293: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 294: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 295: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 296: // "CONFIG_UPLOAD_DIR"
      case 297: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 298: // "CONFIG_UPLOAD_FILE_MODE"
      case 299: // "CONFIG_VALUE_ABORT"
      case 300: // "CONFIG_VALUE_DETC"
      case 301: // "CONFIG_VALUE_HTTPS"
      case 302: // "CONFIG_VALUE_OFF"
      case 303: // "CONFIG_VALUE_ON"
      case 304: // "CONFIG_VALUE_PARALLEL"
      case 305: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 306: // "CONFIG_VALUE_REJECT"
      case 307: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 308: // "CONFIG_VALUE_SERIAL"
      case 309: // "CONFIG_VALUE_WARN"
      case 310: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 311: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 312: // "CONGIG_DIR_SEC_ARG_SEP"
      case 313: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 314: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 315: // "CONGIG_DIR_SEC_DATA_DIR"
      case 316: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 317: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 318: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 319: // "CONGIG_DIR_SEC_TMP_DIR"
      case 320: // "DIRECTIVE"
      case 321: // "DIRECTIVE_SECRULESCRIPT"
      case 322: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 323: // "QUOTATION_MARK"
      case 324: // "RUN_TIME_VAR_BLD"
      case 325: // "RUN_TIME_VAR_DUR"
      case 326: // "RUN_TIME_VAR_HSV"
      case 327: // "RUN_TIME_VAR_REMOTE_USER"
      case 328: // "RUN_TIME_VAR_TIME"
      case 329: // "RUN_TIME_VAR_TIME_DAY"
      case 330: // "RUN_TIME_VAR_TIME_EPOCH"
      case 331: // "RUN_TIME_VAR_TIME_HOUR"
      case 332: // "RUN_TIME_VAR_TIME_MIN"
      case 333: // "RUN_TIME_VAR_TIME_MON"
      case 334: // "RUN_TIME_VAR_TIME_SEC"
      case 335: // "RUN_TIME_VAR_TIME_WDAY"
      case 336: // "RUN_TIME_VAR_TIME_YEAR"
      case 337: // "VARIABLE"
      case 338: // "Dictionary element"
      case 339: // "Dictionary element, selected by regexp"
        value.copy< std::string > (other.value);
        break;

      case 346: // op
      case 347: // op_before_init
        value.copy< std::unique_ptr<Operator> > (other.value);
        break;

      case 355: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (other.value);
        break;

      case 352: // var
        value.copy< std::unique_ptr<Variable> > (other.value);
        break;

      case 353: // act
      case 354: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (other.value);
        break;

      case 349: // variables
      case 350: // variables_pre_process
      case 351: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (other.value);
        break;

      case 344: // actions
      case 345: // actions_may_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > (other.value);
        break;

      default:
        break;
    }

  }

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
    switch (this->type_get ())
    {
      case 144: // "Accuracy"
      case 145: // "Allow"
      case 146: // "Append"
      case 147: // "AuditLog"
      case 148: // "Block"
      case 149: // "Capture"
      case 150: // "Chain"
      case 151: // "ACTION_CTL_AUDIT_ENGINE"
      case 152: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 153: // "ACTION_CTL_BDY_JSON"
      case 154: // "ACTION_CTL_BDY_XML"
      case 155: // "ACTION_CTL_BDY_URLENCODED"
      case 156: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 157: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 158: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 160: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 162: // "Deny"
      case 163: // "DeprecateVar"
      case 164: // "Drop"
      case 165: // "Exec"
      case 166: // "ExpireVar"
      case 167: // "Id"
      case 168: // "InitCol"
      case 169: // "Log"
      case 170: // "LogData"
      case 171: // "Maturity"
      case 172: // "Msg"
      case 173: // "MultiMatch"
      case 174: // "NoAuditLog"
      case 175: // "NoLog"
      case 176: // "Pass"
      case 177: // "Pause"
      case 178: // "Phase"
      case 179: // "Prepend"
      case 180: // "Proxy"
      case 181: // "Redirect"
      case 182: // "Rev"
      case 183: // "SanitiseArg"
      case 184: // "SanitiseMatched"
      case 185: // "SanitiseMatchedBytes"
      case 186: // "SanitiseRequestHeader"
      case 187: // "SanitiseResponseHeader"
      case 188: // "SetEnv"
      case 189: // "SetRsc"
      case 190: // "SetSid"
      case 191: // "SetUID"
      case 192: // "Severity"
      case 193: // "Skip"
      case 194: // "SkipAfter"
      case 195: // "Status"
      case 196: // "Tag"
      case 197: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 200: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 201: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 202: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 204: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 206: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 207: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 208: // "ACTION_TRANSFORMATION_LENGTH"
      case 209: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 210: // "ACTION_TRANSFORMATION_MD5"
      case 211: // "ACTION_TRANSFORMATION_NONE"
      case 212: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 214: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 215: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 221: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 223: // "ACTION_TRANSFORMATION_SHA1"
      case 224: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 225: // "ACTION_TRANSFORMATION_TRIM"
      case 226: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 227: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 228: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 229: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 230: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 232: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 233: // "Ver"
      case 234: // "xmlns"
      case 235: // "CONFIG_COMPONENT_SIG"
      case 236: // "CONFIG_CONN_ENGINE"
      case 237: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 238: // "CONFIG_SEC_WEB_APP_ID"
      case 239: // "CONFIG_SEC_SERVER_SIG"
      case 240: // "CONFIG_DIR_AUDIT_DIR"
      case 241: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 242: // "CONFIG_DIR_AUDIT_ENG"
      case 243: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 244: // "CONFIG_DIR_AUDIT_LOG"
      case 245: // "CONFIG_DIR_AUDIT_LOG2"
      case 246: // "CONFIG_DIR_AUDIT_LOG_P"
      case 247: // "CONFIG_DIR_AUDIT_STS"
      case 248: // "CONFIG_DIR_AUDIT_TPE"
      case 249: // "CONFIG_DIR_DEBUG_LOG"
      case 250: // "CONFIG_DIR_DEBUG_LVL"
      case 251: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 252: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 253: // "CONFIG_SEC_HASH_ENGINE"
      case 254: // "CONFIG_SEC_HASH_KEY"
      case 255: // "CONFIG_SEC_HASH_PARAM"
      case 256: // "CONFIG_SEC_HASH_METHOD_RX"
      case 257: // "CONFIG_SEC_HASH_METHOD_PM"
      case 258: // "CONFIG_SEC_CHROOT_DIR"
      case 259: // "CONFIG_DIR_GEO_DB"
      case 260: // "CONFIG_DIR_GSB_DB"
      case 261: // "CONFIG_SEC_GUARDIAN_LOG"
      case 262: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 264: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 265: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 266: // "CONFIG_SEC_SENSOR_ID"
      case 267: // "CONFIG_DIR_REQ_BODY"
      case 268: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 270: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 271: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 272: // "CONFIG_DIR_RES_BODY"
      case 273: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 275: // "CONFIG_SEC_RULE_INHERITANCE"
      case 276: // "CONFIG_SEC_RULE_PERF_TIME"
      case 277: // "CONFIG_DIR_RULE_ENG"
      case 278: // "CONFIG_DIR_SEC_ACTION"
      case 279: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 280: // "CONFIG_DIR_SEC_MARKER"
      case 281: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 282: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 283: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 284: // "CONFIG_SEC_HTTP_BLKEY"
      case 285: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 286: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 287: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 288: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 290: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 291: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 293: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 294: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 295: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 296: // "CONFIG_UPLOAD_DIR"
      case 297: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 298: // "CONFIG_UPLOAD_FILE_MODE"
      case 299: // "CONFIG_VALUE_ABORT"
      case 300: // "CONFIG_VALUE_DETC"
      case 301: // "CONFIG_VALUE_HTTPS"
      case 302: // "CONFIG_VALUE_OFF"
      case 303: // "CONFIG_VALUE_ON"
      case 304: // "CONFIG_VALUE_PARALLEL"
      case 305: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 306: // "CONFIG_VALUE_REJECT"
      case 307: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 308: // "CONFIG_VALUE_SERIAL"
      case 309: // "CONFIG_VALUE_WARN"
      case 310: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 311: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 312: // "CONGIG_DIR_SEC_ARG_SEP"
      case 313: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 314: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 315: // "CONGIG_DIR_SEC_DATA_DIR"
      case 316: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 317: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 318: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 319: // "CONGIG_DIR_SEC_TMP_DIR"
      case 320: // "DIRECTIVE"
      case 321: // "DIRECTIVE_SECRULESCRIPT"
      case 322: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 323: // "QUOTATION_MARK"
      case 324: // "RUN_TIME_VAR_BLD"
      case 325: // "RUN_TIME_VAR_DUR"
      case 326: // "RUN_TIME_VAR_HSV"
      case 327: // "RUN_TIME_VAR_REMOTE_USER"
      case 328: // "RUN_TIME_VAR_TIME"
      case 329: // "RUN_TIME_VAR_TIME_DAY"
      case 330: // "RUN_TIME_VAR_TIME_EPOCH"
      case 331: // "RUN_TIME_VAR_TIME_HOUR"
      case 332: // "RUN_TIME_VAR_TIME_MIN"
      case 333: // "RUN_TIME_VAR_TIME_MON"
      case 334: // "RUN_TIME_VAR_TIME_SEC"
      case 335: // "RUN_TIME_VAR_TIME_WDAY"
      case 336: // "RUN_TIME_VAR_TIME_YEAR"
      case 337: // "VARIABLE"
      case 338: // "Dictionary element"
      case 339: // "Dictionary element, selected by regexp"
        value.copy< std::string > (v);
        break;

      case 346: // op
      case 347: // op_before_init
        value.copy< std::unique_ptr<Operator> > (v);
        break;

      case 355: // run_time_string
        value.copy< std::unique_ptr<RunTimeString> > (v);
        break;

      case 352: // var
        value.copy< std::unique_ptr<Variable> > (v);
        break;

      case 353: // act
      case 354: // setvar_action
        value.copy< std::unique_ptr<actions::Action> > (v);
        break;

      case 349: // variables
      case 350: // variables_pre_process
      case 351: // variables_may_be_quoted
        value.copy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (v);
        break;

      case 344: // actions
      case 345: // actions_may_quoted
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
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<Operator>& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<RunTimeString>& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<Variable>& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<actions::Action>& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<Variable> > > & v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  seclang_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > > & v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  seclang_parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
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
      case 144: // "Accuracy"
      case 145: // "Allow"
      case 146: // "Append"
      case 147: // "AuditLog"
      case 148: // "Block"
      case 149: // "Capture"
      case 150: // "Chain"
      case 151: // "ACTION_CTL_AUDIT_ENGINE"
      case 152: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 153: // "ACTION_CTL_BDY_JSON"
      case 154: // "ACTION_CTL_BDY_XML"
      case 155: // "ACTION_CTL_BDY_URLENCODED"
      case 156: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 157: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 158: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 160: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 162: // "Deny"
      case 163: // "DeprecateVar"
      case 164: // "Drop"
      case 165: // "Exec"
      case 166: // "ExpireVar"
      case 167: // "Id"
      case 168: // "InitCol"
      case 169: // "Log"
      case 170: // "LogData"
      case 171: // "Maturity"
      case 172: // "Msg"
      case 173: // "MultiMatch"
      case 174: // "NoAuditLog"
      case 175: // "NoLog"
      case 176: // "Pass"
      case 177: // "Pause"
      case 178: // "Phase"
      case 179: // "Prepend"
      case 180: // "Proxy"
      case 181: // "Redirect"
      case 182: // "Rev"
      case 183: // "SanitiseArg"
      case 184: // "SanitiseMatched"
      case 185: // "SanitiseMatchedBytes"
      case 186: // "SanitiseRequestHeader"
      case 187: // "SanitiseResponseHeader"
      case 188: // "SetEnv"
      case 189: // "SetRsc"
      case 190: // "SetSid"
      case 191: // "SetUID"
      case 192: // "Severity"
      case 193: // "Skip"
      case 194: // "SkipAfter"
      case 195: // "Status"
      case 196: // "Tag"
      case 197: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 200: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 201: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 202: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 204: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 206: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 207: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 208: // "ACTION_TRANSFORMATION_LENGTH"
      case 209: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 210: // "ACTION_TRANSFORMATION_MD5"
      case 211: // "ACTION_TRANSFORMATION_NONE"
      case 212: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 214: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 215: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 221: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 223: // "ACTION_TRANSFORMATION_SHA1"
      case 224: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 225: // "ACTION_TRANSFORMATION_TRIM"
      case 226: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 227: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 228: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 229: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 230: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 232: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 233: // "Ver"
      case 234: // "xmlns"
      case 235: // "CONFIG_COMPONENT_SIG"
      case 236: // "CONFIG_CONN_ENGINE"
      case 237: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 238: // "CONFIG_SEC_WEB_APP_ID"
      case 239: // "CONFIG_SEC_SERVER_SIG"
      case 240: // "CONFIG_DIR_AUDIT_DIR"
      case 241: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 242: // "CONFIG_DIR_AUDIT_ENG"
      case 243: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 244: // "CONFIG_DIR_AUDIT_LOG"
      case 245: // "CONFIG_DIR_AUDIT_LOG2"
      case 246: // "CONFIG_DIR_AUDIT_LOG_P"
      case 247: // "CONFIG_DIR_AUDIT_STS"
      case 248: // "CONFIG_DIR_AUDIT_TPE"
      case 249: // "CONFIG_DIR_DEBUG_LOG"
      case 250: // "CONFIG_DIR_DEBUG_LVL"
      case 251: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 252: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 253: // "CONFIG_SEC_HASH_ENGINE"
      case 254: // "CONFIG_SEC_HASH_KEY"
      case 255: // "CONFIG_SEC_HASH_PARAM"
      case 256: // "CONFIG_SEC_HASH_METHOD_RX"
      case 257: // "CONFIG_SEC_HASH_METHOD_PM"
      case 258: // "CONFIG_SEC_CHROOT_DIR"
      case 259: // "CONFIG_DIR_GEO_DB"
      case 260: // "CONFIG_DIR_GSB_DB"
      case 261: // "CONFIG_SEC_GUARDIAN_LOG"
      case 262: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 264: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 265: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 266: // "CONFIG_SEC_SENSOR_ID"
      case 267: // "CONFIG_DIR_REQ_BODY"
      case 268: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 270: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 271: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 272: // "CONFIG_DIR_RES_BODY"
      case 273: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 275: // "CONFIG_SEC_RULE_INHERITANCE"
      case 276: // "CONFIG_SEC_RULE_PERF_TIME"
      case 277: // "CONFIG_DIR_RULE_ENG"
      case 278: // "CONFIG_DIR_SEC_ACTION"
      case 279: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 280: // "CONFIG_DIR_SEC_MARKER"
      case 281: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 282: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 283: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 284: // "CONFIG_SEC_HTTP_BLKEY"
      case 285: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 286: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 287: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 288: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 290: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 291: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 293: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 294: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 295: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 296: // "CONFIG_UPLOAD_DIR"
      case 297: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 298: // "CONFIG_UPLOAD_FILE_MODE"
      case 299: // "CONFIG_VALUE_ABORT"
      case 300: // "CONFIG_VALUE_DETC"
      case 301: // "CONFIG_VALUE_HTTPS"
      case 302: // "CONFIG_VALUE_OFF"
      case 303: // "CONFIG_VALUE_ON"
      case 304: // "CONFIG_VALUE_PARALLEL"
      case 305: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 306: // "CONFIG_VALUE_REJECT"
      case 307: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 308: // "CONFIG_VALUE_SERIAL"
      case 309: // "CONFIG_VALUE_WARN"
      case 310: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 311: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 312: // "CONGIG_DIR_SEC_ARG_SEP"
      case 313: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 314: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 315: // "CONGIG_DIR_SEC_DATA_DIR"
      case 316: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 317: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 318: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 319: // "CONGIG_DIR_SEC_TMP_DIR"
      case 320: // "DIRECTIVE"
      case 321: // "DIRECTIVE_SECRULESCRIPT"
      case 322: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 323: // "QUOTATION_MARK"
      case 324: // "RUN_TIME_VAR_BLD"
      case 325: // "RUN_TIME_VAR_DUR"
      case 326: // "RUN_TIME_VAR_HSV"
      case 327: // "RUN_TIME_VAR_REMOTE_USER"
      case 328: // "RUN_TIME_VAR_TIME"
      case 329: // "RUN_TIME_VAR_TIME_DAY"
      case 330: // "RUN_TIME_VAR_TIME_EPOCH"
      case 331: // "RUN_TIME_VAR_TIME_HOUR"
      case 332: // "RUN_TIME_VAR_TIME_MIN"
      case 333: // "RUN_TIME_VAR_TIME_MON"
      case 334: // "RUN_TIME_VAR_TIME_SEC"
      case 335: // "RUN_TIME_VAR_TIME_WDAY"
      case 336: // "RUN_TIME_VAR_TIME_YEAR"
      case 337: // "VARIABLE"
      case 338: // "Dictionary element"
      case 339: // "Dictionary element, selected by regexp"
        value.template destroy< std::string > ();
        break;

      case 346: // op
      case 347: // op_before_init
        value.template destroy< std::unique_ptr<Operator> > ();
        break;

      case 355: // run_time_string
        value.template destroy< std::unique_ptr<RunTimeString> > ();
        break;

      case 352: // var
        value.template destroy< std::unique_ptr<Variable> > ();
        break;

      case 353: // act
      case 354: // setvar_action
        value.template destroy< std::unique_ptr<actions::Action> > ();
        break;

      case 349: // variables
      case 350: // variables_pre_process
      case 351: // variables_may_be_quoted
        value.template destroy< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > ();
        break;

      case 344: // actions
      case 345: // actions_may_quoted
        value.template destroy< std::unique_ptr<std::vector<std::unique_ptr<actions::Action> > >  > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  bool
  seclang_parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  seclang_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 144: // "Accuracy"
      case 145: // "Allow"
      case 146: // "Append"
      case 147: // "AuditLog"
      case 148: // "Block"
      case 149: // "Capture"
      case 150: // "Chain"
      case 151: // "ACTION_CTL_AUDIT_ENGINE"
      case 152: // "ACTION_CTL_AUDIT_LOG_PARTS"
      case 153: // "ACTION_CTL_BDY_JSON"
      case 154: // "ACTION_CTL_BDY_XML"
      case 155: // "ACTION_CTL_BDY_URLENCODED"
      case 156: // "ACTION_CTL_FORCE_REQ_BODY_VAR"
      case 157: // "ACTION_CTL_REQUEST_BODY_ACCESS"
      case 158: // "ACTION_CTL_RULE_REMOVE_BY_ID"
      case 159: // "ACTION_CTL_RULE_REMOVE_BY_TAG"
      case 160: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_ID"
      case 161: // "ACTION_CTL_RULE_REMOVE_TARGET_BY_TAG"
      case 162: // "Deny"
      case 163: // "DeprecateVar"
      case 164: // "Drop"
      case 165: // "Exec"
      case 166: // "ExpireVar"
      case 167: // "Id"
      case 168: // "InitCol"
      case 169: // "Log"
      case 170: // "LogData"
      case 171: // "Maturity"
      case 172: // "Msg"
      case 173: // "MultiMatch"
      case 174: // "NoAuditLog"
      case 175: // "NoLog"
      case 176: // "Pass"
      case 177: // "Pause"
      case 178: // "Phase"
      case 179: // "Prepend"
      case 180: // "Proxy"
      case 181: // "Redirect"
      case 182: // "Rev"
      case 183: // "SanitiseArg"
      case 184: // "SanitiseMatched"
      case 185: // "SanitiseMatchedBytes"
      case 186: // "SanitiseRequestHeader"
      case 187: // "SanitiseResponseHeader"
      case 188: // "SetEnv"
      case 189: // "SetRsc"
      case 190: // "SetSid"
      case 191: // "SetUID"
      case 192: // "Severity"
      case 193: // "Skip"
      case 194: // "SkipAfter"
      case 195: // "Status"
      case 196: // "Tag"
      case 197: // "ACTION_TRANSFORMATION_BASE_64_ENCODE"
      case 198: // "ACTION_TRANSFORMATION_BASE_64_DECODE"
      case 199: // "ACTION_TRANSFORMATION_BASE_64_DECODE_EXT"
      case 200: // "ACTION_TRANSFORMATION_CMD_LINE"
      case 201: // "ACTION_TRANSFORMATION_COMPRESS_WHITESPACE"
      case 202: // "ACTION_TRANSFORMATION_CSS_DECODE"
      case 203: // "ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE"
      case 204: // "ACTION_TRANSFORMATION_HEX_ENCODE"
      case 205: // "ACTION_TRANSFORMATION_HEX_DECODE"
      case 206: // "ACTION_TRANSFORMATION_HTML_ENTITY_DECODE"
      case 207: // "ACTION_TRANSFORMATION_JS_DECODE"
      case 208: // "ACTION_TRANSFORMATION_LENGTH"
      case 209: // "ACTION_TRANSFORMATION_LOWERCASE"
      case 210: // "ACTION_TRANSFORMATION_MD5"
      case 211: // "ACTION_TRANSFORMATION_NONE"
      case 212: // "ACTION_TRANSFORMATION_NORMALISE_PATH"
      case 213: // "ACTION_TRANSFORMATION_NORMALISE_PATH_WIN"
      case 214: // "ACTION_TRANSFORMATION_PARITY_EVEN_7_BIT"
      case 215: // "ACTION_TRANSFORMATION_PARITY_ODD_7_BIT"
      case 216: // "ACTION_TRANSFORMATION_PARITY_ZERO_7_BIT"
      case 217: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS"
      case 218: // "ACTION_TRANSFORMATION_REMOVE_COMMENTS_CHAR"
      case 219: // "ACTION_TRANSFORMATION_REMOVE_NULLS"
      case 220: // "ACTION_TRANSFORMATION_REMOVE_WHITESPACE"
      case 221: // "ACTION_TRANSFORMATION_REPLACE_COMMENTS"
      case 222: // "ACTION_TRANSFORMATION_REPLACE_NULLS"
      case 223: // "ACTION_TRANSFORMATION_SHA1"
      case 224: // "ACTION_TRANSFORMATION_SQL_HEX_DECODE"
      case 225: // "ACTION_TRANSFORMATION_TRIM"
      case 226: // "ACTION_TRANSFORMATION_TRIM_LEFT"
      case 227: // "ACTION_TRANSFORMATION_TRIM_RIGHT"
      case 228: // "ACTION_TRANSFORMATION_UPPERCASE"
      case 229: // "ACTION_TRANSFORMATION_URL_ENCODE"
      case 230: // "ACTION_TRANSFORMATION_URL_DECODE"
      case 231: // "ACTION_TRANSFORMATION_URL_DECODE_UNI"
      case 232: // "ACTION_TRANSFORMATION_UTF8_TO_UNICODE"
      case 233: // "Ver"
      case 234: // "xmlns"
      case 235: // "CONFIG_COMPONENT_SIG"
      case 236: // "CONFIG_CONN_ENGINE"
      case 237: // "CONFIG_SEC_ARGUMENT_SEPARATOR"
      case 238: // "CONFIG_SEC_WEB_APP_ID"
      case 239: // "CONFIG_SEC_SERVER_SIG"
      case 240: // "CONFIG_DIR_AUDIT_DIR"
      case 241: // "CONFIG_DIR_AUDIT_DIR_MOD"
      case 242: // "CONFIG_DIR_AUDIT_ENG"
      case 243: // "CONFIG_DIR_AUDIT_FLE_MOD"
      case 244: // "CONFIG_DIR_AUDIT_LOG"
      case 245: // "CONFIG_DIR_AUDIT_LOG2"
      case 246: // "CONFIG_DIR_AUDIT_LOG_P"
      case 247: // "CONFIG_DIR_AUDIT_STS"
      case 248: // "CONFIG_DIR_AUDIT_TPE"
      case 249: // "CONFIG_DIR_DEBUG_LOG"
      case 250: // "CONFIG_DIR_DEBUG_LVL"
      case 251: // "CONFIG_SEC_CACHE_TRANSFORMATIONS"
      case 252: // "CONFIG_SEC_DISABLE_BACKEND_COMPRESS"
      case 253: // "CONFIG_SEC_HASH_ENGINE"
      case 254: // "CONFIG_SEC_HASH_KEY"
      case 255: // "CONFIG_SEC_HASH_PARAM"
      case 256: // "CONFIG_SEC_HASH_METHOD_RX"
      case 257: // "CONFIG_SEC_HASH_METHOD_PM"
      case 258: // "CONFIG_SEC_CHROOT_DIR"
      case 259: // "CONFIG_DIR_GEO_DB"
      case 260: // "CONFIG_DIR_GSB_DB"
      case 261: // "CONFIG_SEC_GUARDIAN_LOG"
      case 262: // "CONFIG_DIR_PCRE_MATCH_LIMIT"
      case 263: // "CONFIG_DIR_PCRE_MATCH_LIMIT_RECURSION"
      case 264: // "CONFIG_SEC_CONN_R_STATE_LIMIT"
      case 265: // "CONFIG_SEC_CONN_W_STATE_LIMIT"
      case 266: // "CONFIG_SEC_SENSOR_ID"
      case 267: // "CONFIG_DIR_REQ_BODY"
      case 268: // "CONFIG_DIR_REQ_BODY_IN_MEMORY_LIMIT"
      case 269: // "CONFIG_DIR_REQ_BODY_LIMIT"
      case 270: // "CONFIG_DIR_REQ_BODY_LIMIT_ACTION"
      case 271: // "CONFIG_DIR_REQ_BODY_NO_FILES_LIMIT"
      case 272: // "CONFIG_DIR_RES_BODY"
      case 273: // "CONFIG_DIR_RES_BODY_LIMIT"
      case 274: // "CONFIG_DIR_RES_BODY_LIMIT_ACTION"
      case 275: // "CONFIG_SEC_RULE_INHERITANCE"
      case 276: // "CONFIG_SEC_RULE_PERF_TIME"
      case 277: // "CONFIG_DIR_RULE_ENG"
      case 278: // "CONFIG_DIR_SEC_ACTION"
      case 279: // "CONFIG_DIR_SEC_DEFAULT_ACTION"
      case 280: // "CONFIG_DIR_SEC_MARKER"
      case 281: // "CONFIG_DIR_UNICODE_MAP_FILE"
      case 282: // "CONFIG_DIR_UNICODE_CODE_PAGE"
      case 283: // "CONFIG_SEC_COLLECTION_TIMEOUT"
      case 284: // "CONFIG_SEC_HTTP_BLKEY"
      case 285: // "CONFIG_SEC_INTERCEPT_ON_ERROR"
      case 286: // "CONFIG_SEC_REMOTE_RULES_FAIL_ACTION"
      case 287: // "CONFIG_SEC_RULE_REMOVE_BY_ID"
      case 288: // "CONFIG_SEC_RULE_REMOVE_BY_MSG"
      case 289: // "CONFIG_SEC_RULE_REMOVE_BY_TAG"
      case 290: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG"
      case 291: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG"
      case 292: // "CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID"
      case 293: // "CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID"
      case 294: // "CONFIG_UPDLOAD_KEEP_FILES"
      case 295: // "CONFIG_UPDLOAD_SAVE_TMP_FILES"
      case 296: // "CONFIG_UPLOAD_DIR"
      case 297: // "CONFIG_UPLOAD_FILE_LIMIT"
      case 298: // "CONFIG_UPLOAD_FILE_MODE"
      case 299: // "CONFIG_VALUE_ABORT"
      case 300: // "CONFIG_VALUE_DETC"
      case 301: // "CONFIG_VALUE_HTTPS"
      case 302: // "CONFIG_VALUE_OFF"
      case 303: // "CONFIG_VALUE_ON"
      case 304: // "CONFIG_VALUE_PARALLEL"
      case 305: // "CONFIG_VALUE_PROCESS_PARTIAL"
      case 306: // "CONFIG_VALUE_REJECT"
      case 307: // "CONFIG_VALUE_RELEVANT_ONLY"
      case 308: // "CONFIG_VALUE_SERIAL"
      case 309: // "CONFIG_VALUE_WARN"
      case 310: // "CONFIG_XML_EXTERNAL_ENTITY"
      case 311: // "CONGIG_DIR_RESPONSE_BODY_MP"
      case 312: // "CONGIG_DIR_SEC_ARG_SEP"
      case 313: // "CONGIG_DIR_SEC_COOKIE_FORMAT"
      case 314: // "CONFIG_SEC_COOKIEV0_SEPARATOR"
      case 315: // "CONGIG_DIR_SEC_DATA_DIR"
      case 316: // "CONGIG_DIR_SEC_STATUS_ENGINE"
      case 317: // "CONFIG_SEC_STREAM_IN_BODY_INSPECTION"
      case 318: // "CONFIG_SEC_STREAM_OUT_BODY_INSPECTION"
      case 319: // "CONGIG_DIR_SEC_TMP_DIR"
      case 320: // "DIRECTIVE"
      case 321: // "DIRECTIVE_SECRULESCRIPT"
      case 322: // "FREE_TEXT_QUOTE_MACRO_EXPANSION"
      case 323: // "QUOTATION_MARK"
      case 324: // "RUN_TIME_VAR_BLD"
      case 325: // "RUN_TIME_VAR_DUR"
      case 326: // "RUN_TIME_VAR_HSV"
      case 327: // "RUN_TIME_VAR_REMOTE_USER"
      case 328: // "RUN_TIME_VAR_TIME"
      case 329: // "RUN_TIME_VAR_TIME_DAY"
      case 330: // "RUN_TIME_VAR_TIME_EPOCH"
      case 331: // "RUN_TIME_VAR_TIME_HOUR"
      case 332: // "RUN_TIME_VAR_TIME_MIN"
      case 333: // "RUN_TIME_VAR_TIME_MON"
      case 334: // "RUN_TIME_VAR_TIME_SEC"
      case 335: // "RUN_TIME_VAR_TIME_WDAY"
      case 336: // "RUN_TIME_VAR_TIME_YEAR"
      case 337: // "VARIABLE"
      case 338: // "Dictionary element"
      case 339: // "Dictionary element, selected by regexp"
        value.move< std::string > (s.value);
        break;

      case 346: // op
      case 347: // op_before_init
        value.move< std::unique_ptr<Operator> > (s.value);
        break;

      case 355: // run_time_string
        value.move< std::unique_ptr<RunTimeString> > (s.value);
        break;

      case 352: // var
        value.move< std::unique_ptr<Variable> > (s.value);
        break;

      case 353: // act
      case 354: // setvar_action
        value.move< std::unique_ptr<actions::Action> > (s.value);
        break;

      case 349: // variables
      case 350: // variables_pre_process
      case 351: // variables_may_be_quoted
        value.move< std::unique_ptr<std::vector<std::unique_ptr<Variable> > >  > (s.value);
        break;

      case 344: // actions
      case 345: // actions_may_quoted
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
    const unsigned short
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
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594
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
  seclang_parser::make_CONFIG_CONTENT_INJECTION (const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_CONTENT_INJECTION, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONGIG_DIR_RESPONSE_BODY_MP_CLEAR (const location_type& l)
  {
    return symbol_type (token::TOK_CONGIG_DIR_RESPONSE_BODY_MP_CLEAR, l);
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
  seclang_parser::make_VAR_COUNT (const location_type& l)
  {
    return symbol_type (token::TOK_VAR_COUNT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VAR_EXCLUSION (const location_type& l)
  {
    return symbol_type (token::TOK_VAR_EXCLUSION, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_POST (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_POST, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_GET (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_GET, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES_SIZES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES_SIZES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES_TMP_CONTENT (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES_TMP_CONTENT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_FILENAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_FILENAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_NAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_NAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MATCHED_VARS_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MATCHED_VARS_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MATCHED_VARS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MATCHED_VARS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_COOKIES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_COOKIES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_HEADERS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_HEADERS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_HEADERS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_HEADERS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_GEO (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_GEO, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_COOKIES_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_COOKIES_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_COMBINED_SIZE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_COMBINED_SIZE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_GET_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_GET_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RULE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RULE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_ARGS_POST_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_ARGS_POST_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_AUTH_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_AUTH_TYPE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES_COMBINED_SIZE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES_COMBINED_SIZE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FILES_TMP_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FILES_TMP_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FULL_REQUEST (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FULL_REQUEST, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_FULL_REQUEST_LENGTH (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_FULL_REQUEST_LENGTH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_INBOUND_DATA_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_INBOUND_DATA_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MATCHED_VAR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MATCHED_VAR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MATCHED_VAR_NAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MATCHED_VAR_NAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_BOUNDARY_QUOTED (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_BOUNDARY_QUOTED, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_BOUNDARY_WHITESPACE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_BOUNDARY_WHITESPACE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_CRLF_LF_LINES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_CRLF_LF_LINES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_DATA_AFTER (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_DATA_AFTER, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_DATA_BEFORE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_DATA_BEFORE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_FILE_LIMIT_EXCEEDED, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_HEADER_FOLDING (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_HEADER_FOLDING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_INVALID_HEADER_FOLDING (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_INVALID_HEADER_FOLDING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_INVALID_PART (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_INVALID_PART, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_INVALID_QUOTING (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_INVALID_QUOTING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_LF_LINE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_LF_LINE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_MISSING_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_MISSING_SEMICOLON, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_SEMICOLON_MISSING (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_SEMICOLON_MISSING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_STRICT_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_STRICT_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_MULTIPART_UNMATCHED_BOUNDARY (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_MULTIPART_UNMATCHED_BOUNDARY, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_OUTBOUND_DATA_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_OUTBOUND_DATA_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_PATH_INFO (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_PATH_INFO, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_QUERY_STRING (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_QUERY_STRING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REMOTE_ADDR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REMOTE_ADDR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REMOTE_HOST (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REMOTE_HOST, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REMOTE_PORT (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REMOTE_PORT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQBODY_ERROR_MSG (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQBODY_ERROR_MSG, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQBODY_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQBODY_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQBODY_PROCESSOR_ERROR_MSG (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQBODY_PROCESSOR_ERROR_MSG, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQBODY_PROCESSOR_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQBODY_PROCESSOR_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQBODY_PROCESSOR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQBODY_PROCESSOR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_BASENAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_BASENAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_BODY_LENGTH (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_BODY_LENGTH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_BODY (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_BODY, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_FILE_NAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_FILE_NAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_HEADERS_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_HEADERS_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_LINE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_LINE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_METHOD (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_METHOD, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_PROTOCOL (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_PROTOCOL, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_URI_RAW (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_URI_RAW, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_REQUEST_URI (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_REQUEST_URI, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESOURCE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESOURCE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_BODY (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_BODY, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_CONTENT_LENGTH (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_CONTENT_LENGTH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_CONTENT_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_CONTENT_TYPE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_HEADERS_NAMES (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_HEADERS_NAMES, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_PROTOCOL (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_PROTOCOL, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_RESPONSE_STATUS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_RESPONSE_STATUS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_SERVER_ADDR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_SERVER_ADDR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_SERVER_NAME (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_SERVER_NAME, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_SERVER_PORT (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_SERVER_PORT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_SESSION_ID (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_SESSION_ID, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_UNIQUE_ID (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_UNIQUE_ID, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_URL_ENCODED_ERROR (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_URL_ENCODED_ERROR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_USER_ID (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_USER_ID, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_WEB_APP_ID (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_WEB_APP_ID, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_STATUS (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_STATUS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_STATUS_LINE (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_STATUS_LINE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_IP (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_IP, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_GLOBAL (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_GLOBAL, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_TX (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_TX, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_SESSION (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_SESSION, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_VARIABLE_USER (const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE_USER, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_ENV (const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_ENV, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_RUN_TIME_VAR_XML (const location_type& l)
  {
    return symbol_type (token::TOK_RUN_TIME_VAR_XML, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SETVAR (const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SETVAR, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_SETVAR_OPERATION_EQUALS (const location_type& l)
  {
    return symbol_type (token::TOK_SETVAR_OPERATION_EQUALS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_SETVAR_OPERATION_EQUALS_PLUS (const location_type& l)
  {
    return symbol_type (token::TOK_SETVAR_OPERATION_EQUALS_PLUS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_SETVAR_OPERATION_EQUALS_MINUS (const location_type& l)
  {
    return symbol_type (token::TOK_SETVAR_OPERATION_EQUALS_MINUS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_NOT (const location_type& l)
  {
    return symbol_type (token::TOK_NOT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_BEGINS_WITH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_BEGINS_WITH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_CONTAINS (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_CONTAINS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_CONTAINS_WORD (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_CONTAINS_WORD, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_DETECT_SQLI (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_DETECT_SQLI, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_DETECT_XSS (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_DETECT_XSS, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_ENDS_WITH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_ENDS_WITH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_EQ (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_EQ, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_FUZZY_HASH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_FUZZY_HASH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GEOLOOKUP (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GEOLOOKUP, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GSB_LOOKUP (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GSB_LOOKUP, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_GT (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_GT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_INSPECT_FILE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_INSPECT_FILE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_IP_MATCH_FROM_FILE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_IP_MATCH_FROM_FILE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_IP_MATCH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_IP_MATCH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_LE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_LE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_LT (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_LT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_PM_FROM_FILE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_PM_FROM_FILE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_PM (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_PM, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RBL (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RBL, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RSUB (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RSUB, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RX_CONTENT_ONLY (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RX_CONTENT_ONLY, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_RX (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_RX, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_STR_EQ (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_STR_EQ, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_STR_MATCH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_STR_MATCH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_UNCONDITIONAL_MATCH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_UNCONDITIONAL_MATCH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_BYTE_RANGE (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_BYTE_RANGE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_DTD (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_DTD, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_HASH (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_HASH, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_SCHEMA (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_SCHEMA, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_URL_ENCODING (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_URL_ENCODING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VALIDATE_UTF8_ENCODING (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VALIDATE_UTF8_ENCODING, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_CC (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_CC, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_CPF (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_CPF, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_VERIFY_SSN (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_VERIFY_SSN, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_OPERATOR_WITHIN (const location_type& l)
  {
    return symbol_type (token::TOK_OPERATOR_WITHIN, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_AUDIT_LOG_FMT (const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_AUDIT_LOG_FMT, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_JSON (const location_type& l)
  {
    return symbol_type (token::TOK_JSON, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_NATIVE (const location_type& l)
  {
    return symbol_type (token::TOK_NATIVE, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_ENGINE (const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_ENGINE, l);
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
  seclang_parser::make_ACTION_CTL_BDY_URLENCODED (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_BDY_URLENCODED, v, l);
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
  seclang_parser::make_ACTION_CTL_RULE_REMOVE_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_REMOVE_BY_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_CTL_RULE_REMOVE_BY_TAG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_CTL_RULE_REMOVE_BY_TAG, v, l);
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
  seclang_parser::make_ACTION_SANITISE_ARG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANITISE_ARG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANITISE_MATCHED (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANITISE_MATCHED, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANITISE_MATCHED_BYTES (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANITISE_MATCHED_BYTES, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANITISE_REQUEST_HEADER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANITISE_REQUEST_HEADER, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_SANITISE_RESPONSE_HEADER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_SANITISE_RESPONSE_HEADER, v, l);
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
  seclang_parser::make_ACTION_TRANSFORMATION_BASE_64_ENCODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_BASE_64_ENCODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_BASE_64_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_BASE_64_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_BASE_64_DECODE_EXT, v, l);
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
  seclang_parser::make_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_ESCAPE_SEQ_DECODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_HEX_ENCODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_HEX_ENCODE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_HEX_DECODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_HEX_DECODE, v, l);
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
  seclang_parser::make_ACTION_TRANSFORMATION_TRIM_LEFT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_TRIM_LEFT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_TRIM_RIGHT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_TRIM_RIGHT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_UPPERCASE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_UPPERCASE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_ACTION_TRANSFORMATION_URL_ENCODE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_ACTION_TRANSFORMATION_URL_ENCODE, v, l);
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
  seclang_parser::make_CONFIG_CONN_ENGINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_CONN_ENGINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_ARGUMENT_SEPARATOR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_ARGUMENT_SEPARATOR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_WEB_APP_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_WEB_APP_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_SERVER_SIG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_SERVER_SIG, v, l);
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
  seclang_parser::make_CONFIG_SEC_CACHE_TRANSFORMATIONS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_CACHE_TRANSFORMATIONS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_DISABLE_BACKEND_COMPRESS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_DISABLE_BACKEND_COMPRESS, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HASH_ENGINE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HASH_ENGINE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HASH_KEY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HASH_KEY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HASH_PARAM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HASH_PARAM, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HASH_METHOD_RX (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HASH_METHOD_RX, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HASH_METHOD_PM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HASH_METHOD_PM, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_CHROOT_DIR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_CHROOT_DIR, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_GEO_DB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_GEO_DB, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_DIR_GSB_DB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_GSB_DB, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_GUARDIAN_LOG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_GUARDIAN_LOG, v, l);
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
  seclang_parser::make_CONFIG_SEC_CONN_R_STATE_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_CONN_R_STATE_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_CONN_W_STATE_LIMIT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_CONN_W_STATE_LIMIT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_SENSOR_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_SENSOR_ID, v, l);
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
  seclang_parser::make_CONFIG_SEC_RULE_INHERITANCE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_INHERITANCE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_PERF_TIME (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_PERF_TIME, v, l);
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
  seclang_parser::make_CONFIG_DIR_UNICODE_CODE_PAGE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_DIR_UNICODE_CODE_PAGE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_COLLECTION_TIMEOUT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_COLLECTION_TIMEOUT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_HTTP_BLKEY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_HTTP_BLKEY, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_INTERCEPT_ON_ERROR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_INTERCEPT_ON_ERROR, v, l);
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
  seclang_parser::make_CONFIG_SEC_RULE_REMOVE_BY_MSG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_REMOVE_BY_MSG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_REMOVE_BY_TAG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_REMOVE_BY_TAG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_TAG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_MSG, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_UPDATE_TARGET_BY_ID, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_RULE_UPDATE_ACTION_BY_ID, v, l);
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
  seclang_parser::make_CONFIG_SEC_COOKIEV0_SEPARATOR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_COOKIEV0_SEPARATOR, v, l);
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
  seclang_parser::make_CONFIG_SEC_STREAM_IN_BODY_INSPECTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_STREAM_IN_BODY_INSPECTION, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CONFIG_SEC_STREAM_OUT_BODY_INSPECTION, v, l);
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
  seclang_parser::make_FREE_TEXT_QUOTE_MACRO_EXPANSION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_FREE_TEXT_QUOTE_MACRO_EXPANSION, v, l);
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
  seclang_parser::make_VARIABLE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARIABLE, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_DICT_ELEMENT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_DICT_ELEMENT, v, l);
  }

  seclang_parser::symbol_type
  seclang_parser::make_DICT_ELEMENT_REGEXP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_DICT_ELEMENT_REGEXP, v, l);
  }



} // yy
#line 6170 "seclang-parser.hh" // lalr1.cc:380




#endif // !YY_YY_SECLANG_PARSER_HH_INCLUDED
