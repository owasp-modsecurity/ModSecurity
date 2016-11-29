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
#include "modsecurity/transaction.h"
#include "modsecurity/collection/variable.h"
#include "src/variables/rule.h"
#include "src/variables/tx.h"
#include "src/variables/highest_severity.h"
#include "src/utils/string.h"


namespace modsecurity {

MacroExpansion::MacroExpansion() { }


std::string MacroExpansion::expandKeepOriginal(const std::string& input,
    Transaction *transaction) {
    std::string a = MacroExpansion::expand(input, transaction);

    if (a != input) {
        return "\"" + a + "\" (Was: " + input + ")";
    }

    return input;
}


std::string MacroExpansion::expand(const std::string& input,
    Transaction *transaction) {
    return expand(input, NULL, transaction);
}


std::string MacroExpansion::expand(const std::string& input,
     modsecurity::Rule *rule, Transaction *transaction) {
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
        std::string *variableValue = NULL;
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            variableValue = transaction->m_collections.resolveFirst(variable);
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));

            if (utils::string::toupper(col) == "RULE") {
                variableValue = transaction->m_collections.resolveFirst(
                    "RULE:" + var);
            } else {
                variableValue = transaction->m_collections.resolveFirst(col,
                    var);
            }
        }

        if (variableValue) {
            transaction->debug(6, "Resolving: " + variable + " to: " +
                *variableValue);
        } else {
            transaction->debug(6, "Resolving: " + variable + " to: NULL");
        }
        res.erase(start, end - start + 1);
        if (res[start] == '%') {
            res.erase(start, 1);
        }

        if (variableValue != NULL) {
            res.insert(start, *variableValue);
        }

        pos = res.find("%{");
    }

    return res;
}

}  // namespace modsecurity
