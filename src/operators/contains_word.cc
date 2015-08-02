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

#include "operators/contains_word.h"

#include <string>

#include "operators/operator.h"

namespace ModSecurity {
namespace operators {

bool ContainsWord::evaluate(Assay *assay,
    std::string input) {
    /**
     * @todo Implement the operator ContainsWord in a performative way.
     */

    // FIXME: This is odd logic and should be removed in a future version
    if (this->param == "") {
        return 1;
    }
    // If our length is too long we will never match
    if (this->param.length() > input.length()) {
        return 0;
    }
    // If they are exact matches shortcut
    if (this->param == input) {
        return 1;
    }

    // std::regex r("\\b" + this->param + "\\b");
    // std::smatch m;
    // if (std::regex_search(input, m, r)) {
        // this won't find anything because 'spoons' is not
        // the word you're searching for
    //    return 1;
    // }

    return 0;
}


ContainsWord::ContainsWord(std::string op,
    std::string param, bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace ModSecurity
