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

#include "actions/transformations/hex_decode.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "modsecurity/assay.h"
#include "actions/transformations/transformation.h"


namespace modsecurity {
namespace actions {
namespace transformations {


HexDecode::HexDecode(std::string action)
    : Transformation(action) {
    this->action_kind = 1;
}


std::string HexDecode::evaluate(std::string value,
    Assay *assay) {
    int len = value.length();
    std::string newString;

    for (int i=0; i< len; i+=2) {
        std::string byte = value.substr(i, 2);
        char chr =  static_cast<char>(static_cast<int>(strtol(byte.c_str(),
            NULL, 16)));
        newString.push_back(chr);
    }

    return newString;
}


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
