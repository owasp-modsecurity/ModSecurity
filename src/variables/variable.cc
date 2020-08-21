/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include "src/utils/string.h"


namespace modsecurity {
namespace variables {


Variable::Variable(const std::string &name)
    : m_keyWithCollection(new std::string("")),
    m_keyExclusion(),
    m_collectionName(new std::string("")),
    m_key(new std::string("")) {
    size_t a = name.find(":");
    if (a == std::string::npos) {
        a = name.find(".");
    }
    if (a != std::string::npos) {
        m_collectionName->assign(utils::string::toupper(std::string(name, 0, a)));
        m_key->assign(std::string(name, a + 1, name.size()));
        m_keyWithCollection->assign(*m_collectionName.get() + ":" + std::string(name, a + 1, name.size()));
    } else {
        m_keyWithCollection->assign(name);
        m_collectionName->assign(name);
    }
}


void Variable::addsKeyExclusion(Variable *v) {
    std::unique_ptr<KeyExclusion> r;
    VariableModificatorExclusion *ve = \
        dynamic_cast<VariableModificatorExclusion *>(v);
    VariableRegex *vr;

    if (!ve) {
        return;
    }

    vr = dynamic_cast<VariableRegex *>(ve->m_base.get());

    if (vr == NULL) {
        r.reset(new KeyExclusionString(*v->getVariableKey()));
    } else {
        r.reset(new KeyExclusionRegex(vr->m_regex));
    }

    m_keyExclusion.push_back(std::move(r));
}


std::string operator+(const std::string &a, Variable *v) {
    return a + *v->getVariableKeyWithCollection();
}


std::string operator+(const std::string &a, Variables *v) {
    std::string test;
    for (auto &b : *v) {
        if (test.empty()) {
            test = std::string("") + b;
        } else {
            test = test + "|" + b;
        }
    }

    return a + test;
}


}  // namespace variables
}  // namespace modsecurity
