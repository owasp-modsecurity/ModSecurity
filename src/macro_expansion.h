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

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"

#ifndef SRC_MACRO_EXPANSION_H_
#define SRC_MACRO_EXPANSION_H_

namespace ModSecurity {


class MacroExpansion {
 public:
    MacroExpansion();

    static std::string expand(const std::string& input, Assay *assay);
    static std::string expandKeepOriginal(const std::string& input, Assay *assay);
};


}  // namespace ModSecurity



#endif  // SRC_MACRO_EXPANSION_H_
