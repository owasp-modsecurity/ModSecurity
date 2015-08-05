/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "actions/transformations/transformation.h"

#include <cstring>

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "actions/action.h"
#include "actions/transformations/base64_decode_ext.h"
#include "actions/transformations/base64_decode.h"
#include "actions/transformations/cmd_line.h"
#include "actions/transformations/compress_whitespace.h"
#include "actions/transformations/css_decode.h"
#include "actions/transformations/escape_seq_decode.h"
#include "actions/transformations/hex_decode.h"
#include "actions/transformations/hex_encode.h"
#include "actions/transformations/html_entity_decode.h"
#include "actions/transformations/js_decode.h"
#include "actions/transformations/length.h"
#include "actions/transformations/lower_case.h"
#include "actions/transformations/md5.h"
#include "actions/transformations/none.h"
#include "actions/transformations/normalise_path.h"
#include "actions/transformations/normalise_path_win.h"
#include "actions/transformations/parity_even_7bit.h"
#include "actions/transformations/parity_odd_7bit.h"
#include "actions/transformations/parity_zero_7bit.h"
#include "actions/transformations/remove_comments_char.h"
#include "actions/transformations/remove_comments.h"
#include "actions/transformations/remove_nulls.h"
#include "actions/transformations/remove_whitespace.h"
#include "actions/transformations/replace_comments.h"
#include "actions/transformations/replace_nulls.h"
#include "actions/transformations/sha1.h"
#include "actions/transformations/sql_hex_decode.h"
#include "actions/transformations/trim.h"
#include "actions/transformations/trim_left.h"
#include "actions/transformations/trim_right.h"
#include "actions/transformations/url_decode.h"
#include "actions/transformations/url_decode_uni.h"
#include "actions/transformations/url_encode.h"
#include "actions/transformations/utf8_to_unicode.h"

#define IF_MATCH(b) \
    if (a.compare(2, std::strlen(#b), #b) == 0)


namespace ModSecurity {
namespace actions {
namespace transformations {


std::string Transformation::evaluate(std::string value,
    Assay *assay) {
    return value;
}


Transformation* Transformation::instantiate(std::string a) {
    IF_MATCH(base64_decode_ext) { return new Base64DecodeExt(a); }
    IF_MATCH(base64_decode) { return new Base64Decode(a); }
    IF_MATCH(cmd_line) { return new CmdLine(a); }
    IF_MATCH(compress_whitespace) { return new CompressWhitespace(a); }
    IF_MATCH(cssDecode) { return new CssDecode(a); }
    IF_MATCH(escape_seq_decode) { return new EscapeSeqDecode(a); }
    IF_MATCH(hex_decode) { return new HexDecode(a); }
    IF_MATCH(hex_encode) { return new HexEncode(a); }
    IF_MATCH(htmlEntityDecode) { return new HtmlEntityDecode(a); }
    IF_MATCH(jsDecode) { return new JsDecode(a); }
    IF_MATCH(length) { return new Length(a); }
    IF_MATCH(lowercase) { return new LowerCase(a); }
    IF_MATCH(md5) { return new Md5(a); }
    IF_MATCH(none) { return new None(a); }
    IF_MATCH(normalise_path) { return new NormalisePath(a); }
    IF_MATCH(normalisePathWin) { return new NormalisePathWin(a); }
    IF_MATCH(parity_even_7bit) { return new ParityEven7bit(a); }
    IF_MATCH(parity_odd_7bit) { return new ParityOdd7bit(a); }
    IF_MATCH(parity_zero_7bit) { return new ParityZero7bit(a); }
    IF_MATCH(remove_comments_char) { return new RemoveCommentsChar(a); }
    IF_MATCH(remove_comments) { return new RemoveComments(a); }
    IF_MATCH(remove_nulls) { return new RemoveNulls(a); }
    IF_MATCH(remove_whitespace) { return new RemoveWhitespace(a); }
    IF_MATCH(replace_comments) { return new ReplaceComments(a); }
    IF_MATCH(replace_nulls) { return new ReplaceNulls(a); }
    IF_MATCH(sha1) { return new Sha1(a); }
    IF_MATCH(sql_hex_decode) { return new SqlHexDecode(a); }
    IF_MATCH(transformation) { return new Transformation(a); }
    IF_MATCH(trim) { return new Trim(a); }
    IF_MATCH(trim_left) { return new TrimLeft(a); }
    IF_MATCH(trim_right) { return new TrimRight(a); }
    IF_MATCH(url_decode) { return new UrlDecode(a); }
    IF_MATCH(url_decode_uni) { return new UrlDecodeUni(a); }
    IF_MATCH(url_encode) { return new UrlEncode(a); }
    IF_MATCH(utf8_to_unicode) { return new Utf8Unicode(a); }

    return new Transformation(a);
}

}  // namespace transformations
}  // namespace actions
}  // namespace ModSecurity
