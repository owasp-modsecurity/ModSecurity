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

#include "modsecurity/rule.h"

#include <stdio.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "src/actions/set_var.h"
#include "src/actions/tag.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {


RuleWithActionsProperties::RuleWithActionsProperties(Transformations *transformations) :
    m_hasAuditLog(false),
    m_hasBlock(false),
    m_hasLog(false),
    m_hasMultiMatch(false),
    m_hasNoAuditLog(false),
    m_hasNoLog(false),
    m_executeIfMatchActions(),
    m_setVars(),
    m_disruptiveAction(nullptr),
    m_tags(),
    m_transformations(transformations != nullptr ? *transformations : Transformations())
{ }



RuleWithActionsProperties::RuleWithActionsProperties(const RuleWithActionsProperties &o) :
    m_hasAuditLog(o.m_hasAuditLog),
    m_hasBlock(o.m_hasBlock),
    m_hasLog(o.m_hasLog),
    m_hasMultiMatch(o.m_hasMultiMatch),
    m_hasNoAuditLog(o.m_hasNoAuditLog),
    m_hasNoLog(o.m_hasNoAuditLog),
    m_executeIfMatchActions(),
    m_setVars(),
    m_disruptiveAction(o.m_disruptiveAction),
    m_tags(),
    m_transformations(o.m_transformations)
{
    copyActionsWithRunTimeStrings(o);
}


RuleWithActionsProperties &RuleWithActionsProperties::operator=(const RuleWithActionsProperties &o) {
    m_hasAuditLog = o.m_hasAuditLog;
    m_hasBlock = o.m_hasBlock;
    m_hasLog = o.m_hasLog;
    m_hasMultiMatch = o.m_hasMultiMatch;
    m_hasNoAuditLog = o.m_hasNoAuditLog;
    m_hasNoLog = o.m_hasNoAuditLog;
    m_disruptiveAction = o.m_disruptiveAction;
    m_transformations = o.m_transformations;
    copyActionsWithRunTimeStrings(o);

    return *this;
}


void inline RuleWithActionsProperties::copyActionsWithRunTimeStrings(const RuleWithActionsProperties &o) {
    for (auto &i : o.m_executeIfMatchActions) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (!arts) {
            m_executeIfMatchActions.push_back(i);
            continue;
        }
        std::shared_ptr<actions::ActionWithExecution> z(dynamic_cast<actions::ActionWithExecution *>(arts->clone()));
        actions::ActionWithRunTimeString *aa = dynamic_cast<actions::ActionWithRunTimeString *>(z.get());
        aa->populate(nullptr);
        m_executeIfMatchActions.push_back(z);
    }
    for (auto &i : o.m_setVars) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (!arts) {
            m_setVars.push_back(i);
            continue;
        }
        std::shared_ptr<actions::SetVar> z(dynamic_cast<actions::SetVar *>(arts->clone()));
        actions::ActionWithRunTimeString *aa = dynamic_cast<actions::ActionWithRunTimeString *>(z.get());
        aa->populate(nullptr);
        m_setVars.push_back(z);
    }
    for (auto &i : o.m_tags) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (!arts) {
            m_tags.push_back(i);
            continue;
        }
        std::shared_ptr<actions::Tag> z(dynamic_cast<actions::Tag *>(arts->clone()));
        actions::Tag *aa = dynamic_cast<actions::Tag *>(z.get());
        aa->populate(nullptr);
        m_tags.push_back(z);
    }
}


void RuleWithActionsProperties::populate(const RuleWithActions *r) const {
    /**
     *
     * ActionWithRunTimeString needs to be aware of the Rule that it
     * belongs to. It is necessary to resolve some variables
     * (e.g. Rule); Clone and associate are mandatory.
     *
     */
    for (auto &i : m_executeIfMatchActions) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (arts != nullptr) {
            arts->populate(r);
        }
    }
    for (auto &i : m_setVars) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (arts != nullptr) {
            arts->populate(r);
        }
    }
    for (auto &i : m_tags) {
        actions::ActionWithRunTimeString *arts = dynamic_cast<actions::ActionWithRunTimeString *>(i.get());
        if (arts != nullptr) {
            arts->populate(r);
        }
    }
}


}  // namespace modsecurity
