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

#include "src/variables/variable.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "modsecurity/transaction.h"
#include "src/variables/variations/exclusion.h"
#include "src/utils/msc_string.h"


using modsecurity::utils::String;
using modsecurity::Variables::Variations::Exclusion;


namespace modsecurity {
namespace Variables {


Variable::Variable(std::string name)
    : m_name(name),
    m_collectionName(""),
    m_isExclusion(false),
    m_isCount(false) {
    if (m_name.find(":") != std::string::npos) {
        std::string col = String::toupper(
            std::string(m_name, 0, m_name.find(":")));
        std::string name = std::string(m_name, m_name.find(":") + 1,
            m_name.size());
        if (col == "TX" || col == "IP" || col == "GLOBAL"
            || col == "RESOURCE" || col == "SESSION") {
            m_collectionName = col;
        }
        if ((name.at(0) == '\\') || (name.at(0) == '/')) {
            m_type = RegularExpression;
        } else {
            m_type = SingleMatch;
        }
    } else {
        m_type = MultipleMatches;
    }

    if (String::tolower(m_name) == "tx") {
        m_collectionName = "TX";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "ip") {
        m_collectionName = "IP";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "global") {
        m_collectionName = "GLOBAL";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "resource") {
        m_collectionName = "RESOURCE";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "session") {
        m_collectionName = "SESSION";
        m_type = MultipleMatches;
    } else if (m_name.find(".") != std::string::npos) {
        m_kind = CollectionVarible;
        m_collectionName = std::string(m_name, 0, m_name.find("."));
    } else {
        m_kind = DirectVariable;
    }
}


Variable::Variable(std::string name, VariableKind kind)
    : m_name(name),
    m_collectionName(""),
    m_kind(kind),
    m_isExclusion(false),
    m_isCount(false) {
    if (m_name.find(":") != std::string::npos) {
        std::string col = String::toupper(
            std::string(m_name, 0, m_name.find(":")));
        std::string name = std::string(m_name, m_name.find(":") + 1,
            m_name.size());
        if (col == "TX" || col == "IP" || col == "GLOBAL"
            || col == "RESOURCE" || col == "SESSION") {
            m_collectionName = col;
        }
        if ((name.at(0) == '\\') || (name.at(0) == '/')) {
            m_type = RegularExpression;
        } else {
            m_type = SingleMatch;
        }
    } else {
        m_type = MultipleMatches;
    }

    if (String::tolower(m_name) == "tx") {
        m_collectionName = "TX";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "ip") {
        m_collectionName = "IP";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "global") {
        m_collectionName = "GLOBAL";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "resource") {
        m_collectionName = "RESOURCE";
        m_type = MultipleMatches;
    } else if (String::tolower(m_name) == "session") {
        m_collectionName = "SESSION";
        m_type = MultipleMatches;
    } else if (m_name.find(".") != std::string::npos) {
        m_collectionName = std::string(m_name, 0, m_name.find("."));
    }
}


std::vector<const collection::Variable *> *
    Variable::evaluate(Transaction *transaction) {
    std::vector<const collection::Variable *> *l = NULL;
    l = new std::vector<const collection::Variable *>();
    evaluate(transaction, l);

    return l;
}

void Variable::evaluateInternal(Transaction *transaction,
    std::vector<const collection::Variable *> *l) {
    if (m_collectionName.empty() == false) {
        if (m_kind == CollectionVarible && m_type == MultipleMatches) {
            transaction->m_collections.resolveMultiMatches(m_name,
                m_collectionName, l);
        } else if (m_type == RegularExpression) {
            transaction->m_collections.resolveRegularExpression(m_name,
                m_collectionName, l);
        } else {
            transaction->m_collections.resolveSingleMatch(m_name,
                m_collectionName, l);
        }
    } else {
        if (m_kind == CollectionVarible && m_type == MultipleMatches) {
            transaction->m_collections.resolveMultiMatches(m_name, l);
        } else if (m_type == RegularExpression) {
            transaction->m_collections.resolveRegularExpression(m_name, l);
        } else {
            transaction->m_collections.resolveSingleMatch(m_name, l);
        }
    }
}


void Variable::evaluate(Transaction *transaction,
    std::vector<const collection::Variable *> *l) {
    evaluateInternal(transaction, l);
}


std::string Variable::to_s(
    std::vector<Variable *> *variables) {
    std::string ret;
    std::string except("");
    for (int i = 0; i < variables->size() ; i++) {
        std::string name = variables->at(i)->m_name;
        Exclusion *e =  dynamic_cast<Exclusion *>(variables->at(i));
        if (e != NULL) {
            if (except.empty()) {
                except = except + name;
            } else {
                except = except + "|" + name;
            }
            continue;
        }

        if (i == 0) {
            ret = ret + name;
        } else {
            ret = ret + "|" + name;
        }
    }

    if (except.empty() == false) {
        ret = ret + ", except for: " + except;
    }
    return ret;
}


}  // namespace Variables
}  // namespace modsecurity
