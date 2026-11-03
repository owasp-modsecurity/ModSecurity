/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include <string.h>
#include "modsecurity/rules_set.h"
#include "modsecurity/modsecurity.h"
#include "src/actions/transformations/transformation.h"

/**
 * for i in $(ls -l src/actions/transformations/\*.h | awk {'print $9'}); do echo "#include \"$i\""; done;
 *
 */
#include "src/actions/transformations/base64_decode.h"
#include "src/actions/transformations/base64_decode_ext.h"
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
#include "src/actions/transformations/remove_comments.h"
#include "src/actions/transformations/remove_comments_char.h"
#include "src/actions/transformations/remove_nulls.h"
#include "src/actions/transformations/remove_whitespace.h"
#include "src/actions/transformations/replace_comments.h"
#include "src/actions/transformations/replace_nulls.h"
#include "src/actions/transformations/sha1.h"
#include "src/actions/transformations/sql_hex_decode.h"
#include "src/actions/transformations/transformation.h"
#include "src/actions/transformations/trim.h"
#include "src/actions/transformations/trim_left.h"
#include "src/actions/transformations/trim_right.h"
#include "src/actions/transformations/upper_case.h"
#include "src/actions/transformations/url_decode.h"
#include "src/actions/transformations/url_decode_uni.h"
#include "src/actions/transformations/url_encode.h"
#include "src/actions/transformations/utf8_to_unicode.h"



/**
 * for i in $(ls -l src/operators/\*.h | awk {'print $9'}); do echo "#include \"$i\""; done;
 *
 */
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


using namespace modsecurity::actions::transformations;
using namespace modsecurity::operators;
using namespace modsecurity;

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <signal.h>
#include <string.h>
#include <memory>
#include <vector>

#ifndef __AFL_LOOP
#define __AFL_LOOP(x) (1)
#endif

inline void op_test(const std::string &opName, const std::string &s) {
    Operator *op = Operator::instantiate(opName, "");
    if (op == nullptr) {
        return;
    }
    op->init("", nullptr);
    op->evaluate(nullptr, nullptr, s);
    delete op;
}

int main(int argc, char** argv) {
    uint8_t buf[128];

#if 0
    std::string lastString;
#endif

    while (__AFL_LOOP(1000)) {
        ssize_t read_bytes;
        // (re-) initialize the library and read new input
        memset(buf, 0, 128);
        read_bytes = read(STDIN_FILENO, buf, 128);

        if (read_bytes <= 0) {
            continue;
        }

        std::string currentString =
            std::string(reinterpret_cast<const char *>(buf), read_bytes);
        const std::string& s = currentString;
#if 0
        std::string z = lastString;
#endif

        ModSecurity *ms = new ModSecurity();
        RulesSet *rules = new RulesSet();
        // Here it is possible to load a real transaction from a JSON.
        // like we do on the regression tests.
        Transaction *t = new Transaction(ms, rules, NULL);


        /**
        * Transformations, generated by:
        *
        * for i in $(grep "class " -Ri src/actions/transformations/\* | grep " :" | grep -v "InstantCache" | awk {'print $2'}); do echo $i *$(echo $i | awk '{print tolower($0)}') = new $i\(\"$i\"\)\; $(echo $i | awk '{print tolower($0)}')-\>evaluate\(s, NULL\)\; delete $(echo $i | awk '{print tolower($0)}')\;; done;
        *
        */
std::vector<std::string> transformationNames = {
    "Base64Decode", "Base64DecodeExt", "Base64Encode", "CmdLine",
    "CompressWhitespace", "CssDecode", "EscapeSeqDecode", "HexDecode",
    "HexEncode", "HtmlEntityDecode", "JsDecode", "Length", "LowerCase",
    "Md5", "None", "NormalisePath", "NormalisePathWin", "ParityEven7bit",
    "ParityOdd7bit", "ParityZero7bit", "RemoveComments", "RemoveCommentsChar",
    "RemoveNulls", "RemoveWhitespace", "ReplaceComments", "ReplaceNulls",
    "Sha1", "SqlHexDecode", "Transformation", "Trim", "TrimLeft",
    "TrimRight", "UpperCase", "UrlDecode", "UrlDecodeUni", "UrlEncode",
    "Utf8ToUnicode"
};
for (const auto &name : transformationNames) {
    std::unique_ptr<Transformation> transformation(Transformation::instantiate(name));
    if (transformation == nullptr) {
        continue;
    }
    std::string value = s;
    transformation->transform(value, t);
}


        /**
        * Operators, generated by:
        *
        * for i in $(grep "class " -Ri src/operators/\* | grep " :" | awk {'print $2'}); do echo $i *$(echo $i | awk '{print tolower($0)}') = new $i\(\"$i\", z, false\)\; $(echo $i | awk '{print tolower($0)}')-\>evaluate\(t, s\)\; delete $(echo $i | awk '{print tolower($0)}')\;; done;
        *
        */
op_test("BeginsWith", s);
op_test("Contains", s);
op_test("ContainsWord", s);
op_test("DetectSQLi", s);
op_test("DetectXSS", s);
op_test("EndsWith", s);
op_test("Eq", s);
//op_test("FuzzyHash", s);
op_test("Ge", s);
//op_test("GeoLookup", s);
//op_test("GsbLookup", s);
op_test("Gt", s);
//op_test("InspectFile", s);
//op_test("IpMatchF", s);
//op_test("IpMatchFromFile", s);
op_test("IpMatch", s);
op_test("Le", s);
op_test("Lt", s);
op_test("NoMatch", s);
//op_test("PmF", s);
//op_test("PmFromFile", s);
op_test("Pm", s);
op_test("Rbl", s);
op_test("Rsub", s);
op_test("Rx", s);
op_test("StrEq", s);
op_test("StrMatch", s);
op_test("UnconditionalMatch", s);
//op_test("ValidateByteRange", s);
//op_test("ValidateDTD", s);
//op_test("ValidateHash", s);
//op_test("ValidateSchema", s);
//op_test("ValidateUrlEncoding", s);
op_test("ValidateUtf8Encoding", s);
op_test("VerifyCC", s);
op_test("VerifyCPF", s);
op_test("VerifySSN", s);
op_test("VerifySVNR", s);
op_test("Within", s);


        /**
        * ModSec API
        *
        */
#if 0
    t->processConnection(s.c_str(), 123, s.c_str(), 123);
    t->processURI(s.c_str(), z.c_str(), z.c_str());
    t->addRequestHeader(s, z);
    t->addRequestHeader(s, s);
    t->addRequestHeader(z, z);
    t->addRequestHeader(z, s);
    t->processRequestHeaders();
    t->appendRequestBody((const unsigned char *)s.c_str(), s.length());
    t->processRequestBody();
    t->addResponseHeader(s, z);
    t->addResponseHeader(s, s);
    t->addResponseHeader(z, z);
    t->addResponseHeader(z, s);
    t->processResponseHeaders();
    t->appendResponseBody((const unsigned char *)s.c_str(), s.length());
    t->processResponseBody();
#endif


        delete t;
        delete rules;
        delete ms;
#if 0
        lastString = currentString;
#endif
    }
    return 0;
}

