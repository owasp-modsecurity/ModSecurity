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

#include "actions/set_var.h"

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "src/rule.h"
#include "src/macro_expansion.h"
#include "src/utils.h"

namespace modsecurity {
namespace actions {

SetVar::SetVar(std::string action)
    : Action(action, RunTimeOnlyIfMatchKind) {
}


bool SetVar::init(std::string *error) {
    size_t pos;

    if (action.at(0) == '\'' && action.size() > 3) {
        action.erase(0, 1);
        action.pop_back();
    }

    // Resolv operation
    operation = setToOne;
    pos = action.find("=");
    if (pos != std::string::npos) {
        operation = setOperation;
    }
    pos = action.find("=+");
    if (pos != std::string::npos) {
        operation = sumAndSetOperation;
    }
    pos = action.find("=-");
    if (pos != std::string::npos) {
        operation = substractAndSetOperation;
    }

    // Collection name
    pos = action.find(".");
    if (pos != std::string::npos) {
        collectionName = std::string(action, 0, pos);
        collectionName = toupper(collectionName);
    } else {
        error->assign("Missing the collection and/or variable name");
        return false;
    }

    // Variable name
    if (operation == setToOne) {
        variableName = std::string(action, pos + 1, action.length()
            - (pos + 1));
    } else {
        size_t pos2 = action.find("=");
        variableName = std::string(action, pos + 1, pos2 - (pos + 1));
        if (pos2 + 2 > action.length()) {
            error->assign("Something wrong with the input format");
            return false;
        }

        if (operation == setOperation) {
            predicate = std::string(action, pos2 + 1, action.length() - (pos2));
        } else {
            predicate = std::string(action, pos2 + 2, action.length()
                - (pos2 + 1));
        }
    }

    if (collectionName.empty() || variableName.empty()) {
        error->assign("Something wrong with the input format");
        return false;
    }

    return true;
}

void SetVar::dump() {
    std::cout << " Operation: " << std::to_string(operation) << std::endl;
    std::cout << "Collection: " << collectionName << std::endl;
    std::cout << "  Variable: " << variableName << std::endl;
    std::cout << " Predicate: " << predicate << std::endl;
}

bool SetVar::evaluate(Rule *rule, Assay *assay) {
    std::string targetValue;
    std::string variableNameExpanded = MacroExpansion::expand(variableName,
        assay);
    std::string resolvedPre = MacroExpansion::expand(predicate, assay);

    if (operation == setOperation) {
        targetValue = resolvedPre;
    } else if (operation == setToOne) {
        targetValue = std::string("1");
    } else {
        int pre = 0;
        int value = 0;

        try {
            pre = stoi(resolvedPre);
        } catch (...) {
            pre = 0;
        }

        try {
            std::string *resolvedValue =
                assay->m_collections.resolveFirst(collectionName,
                    variableNameExpanded);
            if (resolvedValue == NULL) {
                value = 0;
            } else {
                value = stoi(*resolvedValue);
            }
        } catch (...) {
            value = 0;
        }

        switch (operation) {
            case sumAndSetOperation:
                targetValue = std::to_string(value + pre);
                break;
            case substractAndSetOperation:
                targetValue = std::to_string(value - pre);
                break;
        }
    }

#ifndef NO_LOGS
    assay->debug(8, "Saving variable: " + collectionName + ":" + \
        variableNameExpanded + " with value: " + targetValue);
#endif
    assay->m_collections.storeOrUpdateFirst(collectionName,
        variableNameExpanded, targetValue);

    return true;
}

}  // namespace actions
}  // namespace ModSecurity
