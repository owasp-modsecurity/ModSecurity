/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */


#include "src/actions/transformations/transformation.h"

#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/actions/action.h"

#include "src/actions/transformations/base64_decode_ext.h"
#include "src/actions/transformations/base64_decode.h"
#include "src/actions/transformations/base64_encode.h"
#include "src/actions/transformations/cmd_line.h"
#include "src/actions/transformations/compress_whitespace.h"
#include "src/actions/transformations/css_decode.h"
#include "src/actions/transformations/escape_seq_decode.h"
#include "src/actions/transformations/hex_decode.h"
#include "src/actions/transformations/hex_encode.h"
#include "src/actions/transformations/html_entity_decode.h"
#include "src/actions/transformations/js_decode.h"
#include "src/actions/transformations/length.h"
#include "src/actions/transformations/lower_case.h"
#include "src/actions/transformations/md5.h"
#include "src/actions/transformations/none.h"
#include "src/actions/transformations/normalise_path.h"
#include "src/actions/transformations/normalise_path_win.h"
#include "src/actions/transformations/parity_even_7bit.h"
#include "src/actions/transformations/parity_odd_7bit.h"
#include "src/actions/transformations/parity_zero_7bit.h"
#include "src/actions/transformations/remove_comments_char.h"
#include "src/actions/transformations/remove_comments.h"
#include "src/actions/transformations/remove_nulls.h"
#include "src/actions/transformations/remove_whitespace.h"
#include "src/actions/transformations/replace_comments.h"
#include "src/actions/transformations/replace_nulls.h"
#include "src/actions/transformations/sha1.h"
#include "src/actions/transformations/sql_hex_decode.h"
#include "src/actions/transformations/trim.h"
#include "src/actions/transformations/trim_left.h"
#include "src/actions/transformations/trim_right.h"
#include "src/actions/transformations/upper_case.h"
#include "src/actions/transformations/url_decode.h"
#include "src/actions/transformations/url_decode_uni.h"
#include "src/actions/transformations/url_encode.h"
#include "src/actions/transformations/utf8_to_unicode.h"


namespace modsecurity {
namespace actions {
namespace transformations {


class TransformationDoesNotExist: public std::exception {
 public:
    explicit TransformationDoesNotExist(const std::string& name)
        : m_transformation(name)
    { }

  virtual const char* what() const throw() {
    return std::string("Transformation not found: " + m_transformation + \
        ". Make sure that the new transformation is registered at: " + \
        "transformation.cc").c_str();
  }

 private:
    std::string m_transformation;
};


Transformation* Transformation::instantiate(
    const std::string &transformationName) {
    /**
     * 
     * FIXME: Once part of ModSecurity, the transformation needs to register
     *        here. That is necessary to load transformations from external
     *        resources such as Python and Lua, not to mention the
     *        unit/regression framework.
     *
     *        Today this registration is manual; as seen below, the idea is to
     *        have those automatically generated. To avoid transformations not
     *        to be listed.
     */

    std::string name(transformationName);
    name.erase(std::remove(name.begin(), name.end(), '_'), name.end());

    if (match(name, "t:base64DecodeExt")) { return new Base64DecodeExt(); }
    if (match(name, "t:base64Decode")) { return new Base64Decode(); }
    if (match(name, "t:base64Encode")) { return new Base64Encode(); }
    if (match(name, "t:cmdLine")) { return new CmdLine(); }
    if (match(name, "t:compressWhitespace")) {
        return new CompressWhitespace();
    }
    if (match(name, "t:cssDecode")) { return new CssDecode(); }
    if (match(name, "t:escapeSeqDecode")) { return new EscapeSeqDecode(); }
    if (match(name, "t:hexDecode")) { return new HexDecode(); }
    if (match(name, "t:hexEncode")) { return new HexEncode(); }
    if (match(name, "t:htmlEntityDecode")) { return new HtmlEntityDecode(); }
    if (match(name, "t:jsDecode")) { return new JsDecode(); }
    if (match(name, "t:length")) { return new Length(); }
    if (match(name, "t:lowercase")) { return new LowerCase(); }
    if (match(name, "t:md5")) { return new Md5(); }
    if (match(name, "t:none")) { return new None(); }
    if (match(name, "t:normalizePathWin")) { return new NormalisePathWin(); }
    if (match(name, "t:normalisePathWin")) { return new NormalisePathWin(); }
    if (match(name, "t:normalizePath")) { return new NormalisePath(); }
    if (match(name, "t:normalisePath")) { return new NormalisePath(); }
    if (match(name, "t:parityEven7bit")) { return new ParityEven7bit(); }
    if (match(name, "t:parityOdd7bit")) { return new ParityOdd7bit(); }
    if (match(name, "t:parityZero7bit")) { return new ParityZero7bit(); }
    if (match(name, "t:removeCommentsChar")) {
        return new RemoveCommentsChar();
    }
    if (match(name, "t:removeComments")) { return new RemoveComments(); }
    if (match(name, "t:removeNulls")) { return new RemoveNulls(); }
    if (match(name, "t:removeWhitespace")) { return new RemoveWhitespace(); }
    if (match(name, "t:compressWhitespace")) {
        return new CompressWhitespace();
    }
    if (match(name, "t:replaceComments")) { return new ReplaceComments(); }
    if (match(name, "t:replaceNulls")) { return new ReplaceNulls(); }
    if (match(name, "t:sha1")) { return new Sha1(); }
    if (match(name, "t:sqlHexDecode")) { return new SqlHexDecode(); }
    if (match(name, "t:trimLeft")) { return new TrimLeft(); }
    if (match(name, "t:trimRight")) { return new TrimRight(); }
    if (match(name, "t:trim")) { return new Trim(); }
    if (match(name, "t:uppercase")) { return new UpperCase(); }
    if (match(name, "t:urlDecodeUni")) { return new UrlDecodeUni(); }
    if (match(name, "t:urlDecode")) { return new UrlDecode(); }
    if (match(name, "t:urlEncode")) { return new UrlEncode(); }
    if (match(name, "t:utf8toUnicode")) { return new Utf8ToUnicode(); }

    throw TransformationDoesNotExist(name);

    return nullptr;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
