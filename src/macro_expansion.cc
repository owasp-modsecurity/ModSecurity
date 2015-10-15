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

#include "src/macro_expansion.h"
#include "modsecurity/assay.h"

namespace ModSecurity {

MacroExpansion::MacroExpansion() { }


std::string MacroExpansion::expandKeepOriginal(const std::string& input,
    Assay *assay) {
    std::string a = MacroExpansion::expand(input, assay);

    if (a != input) {
        return "\"" + a + "\" (Was: " + input + ")";
    }

    return input;
}


std::string MacroExpansion::expand(const std::string& input, Assay *assay) {
    std::string res;
    size_t pos = input.find("%{");

    if (pos != std::string::npos) {
        res = input;
    } else {
        return input;
    }

    while (pos != std::string::npos) {
        size_t start = pos;
        size_t end = res.find("}");
        if (end == std::string::npos) {
            return res;
        }
        std::string variable(res, start + 2, end - (start + 2));
        std::string *variableValue;
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            variableValue = assay->resolve_variable_first(variable);
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));
            variableValue = assay->resolve_variable_first(col, var);
        }

        res.erase(start, end - start + 2);
        if (variableValue != NULL) {
            res.insert(start, *variableValue);
        }

        pos = res.find("%{");
    }

    return res;
}

}  // namespace ModSecurity
