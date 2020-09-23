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


#ifndef SRC_RULE_WITH_ACTIONS_PROPERTIES_H_
#define SRC_RULE_WITH_ACTIONS_PROPERTIES_H_


#include "modsecurity/modsecurity.h"
#include "src/actions/action_with_execution.h"
#include "src/actions/disruptive/disruptive_action.h"

namespace modsecurity {

namespace actions {
class SetVar;
class Tag;
namespace transformations {
class Transformation;
}
}


class RuleWithActionsProperties {
    /**
     * Properties that can be part of the SecDefaultActions.
     *
     */
 public:
    using ActionWithExecution = actions::ActionWithExecution;
    using ActionDisruptive = actions::disruptive::ActionDisruptive;
    using MatchActions = std::vector<std::shared_ptr<ActionWithExecution>>;
    using SetVar = actions::SetVar;
    using SetVars = std::vector<std::shared_ptr<SetVar>>;
    using Tag = actions::Tag;
    using Tags = std::vector<std::shared_ptr<Tag>>;
    using Transformation = actions::transformations::Transformation;
    using Transformations = std::vector<std::shared_ptr<Transformation>>;

    explicit RuleWithActionsProperties(Transformations *transformations = nullptr);
    ~RuleWithActionsProperties() {
        /* all the allocated resources are shared pointers. */
    }

    RuleWithActionsProperties(const RuleWithActionsProperties &o);
    RuleWithActionsProperties &operator=(const RuleWithActionsProperties &o);
    RuleWithActionsProperties(RuleWithActionsProperties &&o) = delete;


    void clear() {
        m_hasLog = false;
        m_hasNoLog = false;
        m_hasBlock = false;
        m_setVars.clear();
        m_tags.clear();
        m_disruptiveAction = nullptr;
        m_executeIfMatchActions.clear();
        m_transformations.clear();
    };

    void populate(const RuleWithActions *r) const;


    /* auditLog */
    bool hasAuditLog() const noexcept {
        return m_hasAuditLog;
    }
    void setAuditLog(bool b) {
        m_hasAuditLog = b;
    }


    /* log */
    bool hasLog() const noexcept {
        return m_hasLog;
    }
    void setLog(bool b) {
        m_hasLog = b;
    }


    /* MultiMatch */
    bool hasMultiMatch() const noexcept {
        return m_hasMultiMatch;
    }
    void setMultiMatch(bool b) {
        m_hasMultiMatch = b;
    }


    /* noAuditLog */
    bool hasNoAuditLog() const noexcept {
        return m_hasNoAuditLog;
    }
    void setNoAuditLog(bool b) {
        m_hasNoAuditLog = b;
    }


    /* noLog */
    bool hasNoLog() const noexcept {
        return m_hasNoLog;
    }
    void setNoLog(bool b) {
        m_hasNoLog = b;
    }


    /* block */
    bool hasBlock() const noexcept {
        return m_hasBlock;
    }
    void setBlock(bool b) {
        m_hasBlock = b;
    }


    /* transformations */
    const Transformations &getTransformations() const noexcept {
        return m_transformations;
    }
    void addTransformation(std::shared_ptr<Transformation> t) {
        m_transformations.push_back(t);
    }


    /* tags */
    const Tags &getTags() const noexcept {
        return m_tags;
    }
    void setTags(Tags tags) noexcept {
        m_tags.insert(m_tags.end(), tags.begin(), tags.end());
    }
    void addTag(std::shared_ptr<Tag> t) {
        m_tags.push_back(t);
    }
    bool hasTags() const noexcept {
        return !m_tags.empty();
    }
    void clearTags() noexcept {
        m_tags.clear();
    }


    /* vars */
    const SetVars &getSetVars() const noexcept {
        return m_setVars;
    }
    void addSetVar(std::shared_ptr<SetVar> t) {
        m_setVars.push_back(t);
    }


    /* other match actions */
    const MatchActions &getGenericMatchActions() const noexcept {
        return m_executeIfMatchActions;
    }
    void addGenericMatchAction(std::shared_ptr<ActionWithExecution> a) {
        m_executeIfMatchActions.push_back(a);
    }


    /* disruptive action */
    const std::shared_ptr<ActionDisruptive> &getDisruptiveAction() const noexcept {
        return m_disruptiveAction;
    }
    inline void setDisruptiveAction(std::shared_ptr<ActionDisruptive> d) noexcept {
        m_disruptiveAction = d;
    }
    inline bool hasDisruptiveAction() const noexcept {
        return m_disruptiveAction != nullptr;
    }


 private:
    void inline copyActionsWithRunTimeStrings(const RuleWithActionsProperties &o);

    /**
     * FIXME: log, noLog, AuditLog and noAuditLog are save to compute whenver log
     * is necessary or not. This can be pre-computed while the actions are
     * encountered amoung the rule action list.
     *
     */
    bool m_hasAuditLog:1;
    bool m_hasBlock:1;
    bool m_hasLog:1;
    bool m_hasMultiMatch:1;
    bool m_hasNoAuditLog:1;
    bool m_hasNoLog:1;

    MatchActions m_executeIfMatchActions;
    SetVars m_setVars;
    std::shared_ptr<ActionDisruptive> m_disruptiveAction;
    Tags m_tags;
    Transformations m_transformations;
};

}  // namespace modsecurity


#endif  // SRC_RULE_WITH_ACTIONS_PROPERTIES_H_