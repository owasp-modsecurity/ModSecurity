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

#include <string.h>
#include <cstring>

#include <iostream>
#include <ctime>
#include <string>


#include "modsecurity/modsecurity.h"
#include "modsecurity/rules.h"
#include "src/operators/operator.h"
#include "src/actions/transformations/transformation.h"
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
#include "src/actions/transformations/url_decode.h"
#include "src/actions/transformations/url_decode_uni.h"
#include "src/actions/transformations/url_encode.h"
#include "src/actions/transformations/utf8_to_unicode.h"


#include "test/common/modsecurity_test.h"
#include "test/common/modsecurity_test_results.h"
#include "test/common/colors.h"
#include "test/unit/unit_test.h"
#include "src/utils/string.h"

#define IF_MATCH(b) \
    if (a.compare(2, std::strlen(#b), #b) == 0)


using modsecurity_test::UnitTest;
using modsecurity_test::ModSecurityTest;
using modsecurity_test::ModSecurityTestResults;
using modsecurity::actions::transformations::Transformation;
using modsecurity::operators::Operator;
using namespace modsecurity::actions::transformations;

std::string default_test_path = "test-cases/secrules-language-tests/operators";

void print_help() {
    std::cout << "Use ./unit /path/to/file" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


Transformation* t_instantiate(std::string a) {
    IF_MATCH(base64DecodeExt) { return new Base64DecodeExt(a); }
    IF_MATCH(base64Decode) { return new Base64Decode(a); }
    IF_MATCH(base64Encode) { return new Base64Encode(a); }
    IF_MATCH(cmd_line) { return new CmdLine(a); }
    IF_MATCH(compress_whitespace) { return new CompressWhitespace(a); }
    IF_MATCH(cssDecode) { return new CssDecode(a); }
    IF_MATCH(escapeSeqDecode) { return new EscapeSeqDecode(a); }
    IF_MATCH(hexDecode) { return new HexDecode(a); }
    IF_MATCH(hexEncode) { return new HexEncode(a); }
    IF_MATCH(htmlEntityDecode) { return new HtmlEntityDecode(a); }
    IF_MATCH(jsDecode) { return new JsDecode(a); }
    IF_MATCH(length) { return new Length(a); }
    IF_MATCH(lowercase) { return new LowerCase(a); }
    IF_MATCH(md5) { return new Md5(a); }
    IF_MATCH(none) { return new None(a); }
    IF_MATCH(normalizePathWin) { return new NormalisePathWin(a); }
    IF_MATCH(normalisePathWin) { return new NormalisePathWin(a); }
    IF_MATCH(normalizePath) { return new NormalisePath(a); }
    IF_MATCH(normalisePath) { return new NormalisePath(a); }
    IF_MATCH(parityEven7bit) { return new ParityEven7bit(a); }
    IF_MATCH(parityOdd7bit) { return new ParityOdd7bit(a); }
    IF_MATCH(parityZero7bit) { return new ParityZero7bit(a); }
    IF_MATCH(removeCommentsChar) { return new RemoveCommentsChar(a); }
    IF_MATCH(removeComments) { return new RemoveComments(a); }
    IF_MATCH(removeNulls) { return new RemoveNulls(a); }
    IF_MATCH(removeWhitespace) { return new RemoveWhitespace(a); }
    IF_MATCH(compressWhitespace) { return new CompressWhitespace(a); }
    IF_MATCH(replaceComments) { return new ReplaceComments(a); }
    IF_MATCH(replaceNulls) { return new ReplaceNulls(a); }
    IF_MATCH(sha1) { return new Sha1(a); }
    IF_MATCH(sqlHexDecode) { return new SqlHexDecode(a); }
    IF_MATCH(transformation) { return new Transformation(a); }
    IF_MATCH(trimLeft) { return new TrimLeft(a); }
    IF_MATCH(trimRight) { return new TrimRight(a); }
    IF_MATCH(trim) { return new Trim(a); }
    IF_MATCH(urlDecodeUni) { return new UrlDecodeUni(a); }
    IF_MATCH(urlDecode) { return new UrlDecode(a); }
    IF_MATCH(urlEncode) { return new UrlEncode(a); }
    IF_MATCH(utf8ToUnicode) { return new Utf8ToUnicode(a); }

    return new Transformation(a);
}


void perform_unit_test(ModSecurityTest<UnitTest> *test, UnitTest *t,
    ModSecurityTestResults<UnitTest>* res) {
    std::string error;

    if (test->m_automake_output) {
        std::cout << ":test-result: ";
    }

    if (t->type == "op") {
        Operator *op = Operator::instantiate("\"@" + t->name + \
                " " + t->param + "\"");
        op->init(t->filename, &error);
        int ret = op->evaluate(NULL, t->input);
        t->obtained = ret;
        if (ret != t->ret) {
            res->push_back(t);
            if (test->m_automake_output) {
                std::cout << "FAIL ";
            }
        } else if (test->m_automake_output) {
            std::cout << "PASS ";
        }
        delete op;
    } else if (t->type == "tfn") {
        Transformation *tfn = t_instantiate("t:" + t->name);
        std::string ret = tfn->evaluate(t->input, NULL);
        t->obtained = 1;
        t->obtainedOutput = ret;
        if (ret != t->output) {
            res->push_back(t);
            if (test->m_automake_output) {
                std::cout << "FAIL ";
            }
        } else if (test->m_automake_output) {
            std::cout << "PASS ";
        }
        delete tfn;
    } else {
        std::cerr << "Failed. Test type is unknown: << " << t->type;
        std::cerr << std::endl;
    }

    if (test->m_automake_output) {
        std::cout << t->name << " "
            << modsecurity::utils::string::toHexIfNeeded(t->input)
            << std::endl;
    }
}


int main(int argc, char **argv) {
    int total = 0;
    ModSecurityTest<UnitTest> test;
    ModSecurityTestResults<UnitTest> results;

    test.cmd_options(argc, argv);
    if (!test.m_automake_output) {
        std::cout << test.header();
    }

    test.load_tests();
    if (test.target == default_test_path) {
        test.load_tests("test-cases/secrules-language-tests/transformations");
    }

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *tests = a.second;

        total += tests->size();
        for (UnitTest *t : *tests) {
            ModSecurityTestResults<UnitTest> r;

            if (!test.m_automake_output) {
                std::cout << "  " << a.first << "...\t";
            }
            perform_unit_test(&test, t, &r);

            if (!test.m_automake_output) {
                if (r.size() == 0) {
                    std::cout << KGRN << r.size() << " tests failed.";
                } else {
                    std::cout << KRED << r.size() << " tests failed.";
                }
                std::cout << RESET << std::endl;
            }

            results.insert(results.end(), r.begin(), r.end());
        }
    }

    if (!test.m_automake_output) {
        std::cout << "Total >> "  << total << std::endl;
    }

    for (UnitTest *t : results) {
        std::cout << t->print() << std::endl;
    }

    if (!test.m_automake_output) {
        std::cout << std::endl;

        std::cout << "Ran a total of: " << total << " unit tests - ";
        if (results.size() == 0) {
            std::cout << KGRN << "All tests passed" << RESET << std::endl;
        } else {
            std::cout << KRED << results.size() << " failed.";
            std::cout << RESET << std::endl;
        }
    }

    for (std::pair<std::string, std::vector<UnitTest *> *> a : test) {
        std::vector<UnitTest *> *vec = a.second;
        for (int i = 0; i < vec->size(); i++) {
            delete vec->at(i);
        }
        delete vec;
    }
}


