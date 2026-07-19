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

#include <iostream>
#include <string>

#include "src/utils/regex.h"

using modsecurity::Utils::Regex;

namespace {

int g_failures = 0;

void check(const std::string &label, const Regex &re, const std::string &input,
    bool expectMatch) {
    const bool matched = re.search(input) > 0;
    if (matched != expectMatch) {
        std::cerr << "FAIL: " << label << " - expected "
            << (expectMatch ? "match" : "no match") << ", got "
            << (matched ? "match" : "no match") << std::endl;
        g_failures++;
    } else {
        std::cout << "PASS: " << label << std::endl;
    }
}

}  // namespace

/*
 * Regression coverage for the Regex(pattern, ignoreCase, multiLine) parameter
 * added to support --enable-regex-dollar-endonly (see README.md and
 * https://github.com/owasp-modsecurity/ModSecurity/issues/3295). Every other
 * Regex call site in the codebase relies on the multiLine=true default and is
 * unaffected by this parameter.
 */
int main() {
    const std::string pattern = "^hello.*world$";
    const std::string multiLineInput = "test\nhello world\nmore";

    /* multiLine=true (default): PCRE2_MULTILINE / PCRE_MULTILINE, so ^/$
     * anchor at internal line breaks and the pattern matches the middle
     * line on its own. */
    check("multiLine=true matches across internal line breaks",
        Regex(pattern, false, true), multiLineInput, true);

    /* multiLine=false: PCRE2_DOLLAR_ENDONLY / PCRE_DOLLAR_ENDONLY, so ^/$
     * anchor only at the start/end of the whole subject, matching
     * ModSecurity v2's @rx behavior. */
    check("multiLine=false does not match across internal line breaks",
        Regex(pattern, false, false), multiLineInput, false);

    check("multiLine=false still matches a single-line subject",
        Regex(pattern, false, false), "hello world", true);

    /* DOLLAR_ENDONLY specifically makes '$' match only at the true end of
     * the subject, not just before a trailing newline. */
    check("multiLine=false rejects a trailing newline before '$'",
        Regex("123$", false, false), "123\n", false);
    check("multiLine=false matches '$' at the true end of the subject",
        Regex("123$", false, false), "123", true);

    if (g_failures == 0) {
        std::cout << "All Regex multiLine tests passed." << std::endl;
    } else {
        std::cout << g_failures << " Regex multiLine test(s) failed." << std::endl;
    }

    return g_failures;
}
