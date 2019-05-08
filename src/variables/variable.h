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

#include <exception>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <deque>

#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/utils/string.h"
#include "src/utils/regex.h"
#include "src/variables/variable_run_time_resolution.h"
#include "src/rule_with_actions.h"

#ifndef SRC_VARIABLES_VARIABLE_H_
#define SRC_VARIABLES_VARIABLE_H_

#define DEFINE_VARIABLE(n, N, e) \
    VAR_DIRECT(n, N, e)

#define DEFINE_VARIABLE_DICT(n, N, e) \
    VAR_WITH_DICT_ELEMENT(n, N, e) \
    \
    VAR_WITHOUT_DICT_ELEMENT(n, N, e) \
    \
    VAR_WITH_REGEX(n, N, e) \



#define VAR_WITH_REGEX(n, N, e) \
class n ## _DictElementRegexp : public VariableRegex { \
 public: \
    explicit n ## _DictElementRegexp(std::string regex) \
        : VariableRegex(#N, regex) { } \
\
    void evaluate(Transaction *transaction, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .resolveRegularExpression(&m_r, l, \
            m_keyExclusion); \
    } \
};


#define VAR_WITH_DICT_ELEMENT(n, N, e) \
class n ## _DictElement : public VariableDictElement { \
 public: \
    explicit n ## _DictElement(std::string dictElement) \
        : VariableDictElement(#N, dictElement) { } \
\
    void evaluate(Transaction *transaction, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .resolve(m_dictElement, l); \
    } \
};


#define VAR_WITHOUT_DICT_ELEMENT(n, N, e) \
class n ## _NoDictElement : public Variable { \
 public: \
    explicit n ## _NoDictElement() \
        : Variable(#N) { } \
\
    void evaluate(Transaction *transaction, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .resolve(l, m_keyExclusion); \
    } \
};


#define VAR_DIRECT(n, N, e) \
class n : public Variable { \
 public: \
    n() \
        : Variable(#N) { } \
    \
    void evaluate(Transaction *transaction, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .evaluate(l); \
    } \
};


namespace modsecurity {

class Transaction;
namespace variables {

class KeyExclusion {
 public:
    virtual bool match(const std::string &a) = 0;
    virtual ~KeyExclusion() { }
};


// FIXME: use pre built regex.
class KeyExclusionRegex : public KeyExclusion {
 public:
    explicit KeyExclusionRegex(Utils::Regex re)
        : m_re(re.pattern) { }
    explicit KeyExclusionRegex(std::string re)
        : m_re(re) { }

    ~KeyExclusionRegex() override { }

    bool match(const std::string &a) override {
        return m_re.searchAll(a).size() > 0;
    }

    Utils::Regex m_re;
};


class KeyExclusionString : public KeyExclusion {
 public:
    KeyExclusionString(std::string &a)
        : m_key(utils::string::toupper(a)) { }

    ~KeyExclusionString() override { }

    bool match(const std::string &a) override {
        return a.size() == m_key.size() && std::equal(a.begin(), a.end(),
            m_key.begin(),
            [](char aa, char bb) {
                return static_cast<char>(toupper(aa)) == static_cast<char>(bb);
            });
    }

    std::string m_key;
};


class KeyExclusions : public std::deque<std::unique_ptr<KeyExclusion>> {
 public:
    bool toOmit(std::string a) {
        for (auto &z : *this) {
            if (z->match(a)) {
                return true;
            }
        }
        return false;
    }
};


class Variable : public VariableRunTimeResolution {
 public:
    explicit Variable(std::string _name);
    explicit Variable(Variable *_name);
    virtual ~Variable() { }


    virtual void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) = 0;


    void ruleInit(const RuleWithActions *r) {
        m_rule = r;
    }

    bool inline belongsToCollection(Variable *var) {
        return m_collectionName.size() == var->m_collectionName.size()
             && std::equal(m_collectionName.begin(), m_collectionName.end(),
                           var->m_collectionName.begin(),
            [](char aa, char bb) {
            return toupper(aa) == bb;
        });
    }


    void addsKeyExclusion(Variable *v);


    bool operator==(const Variable& b) {
        return m_collectionName == b.m_collectionName &&
            m_name == b.m_name &&
            *m_fullName == *b.m_fullName;
    }


    std::string& operator+=(const char * p) {  return m_name; }


    std::string m_name;
    std::string m_collectionName;
    std::shared_ptr<std::string> m_fullName;
    KeyExclusions m_keyExclusion;

 protected:
    const RuleWithActions *m_rule;
};

class VariableDictElement : public Variable {
 public:
    VariableDictElement(std::string name, std::string dict_element)
        :  m_dictElement(dict_element), Variable(name + ":" + dict_element) { }

    std::string m_dictElement;
};


class VariableRegex : public Variable {
 public:
    VariableRegex(std::string name, std::string regex)
        :  m_r(regex),
        m_regex(regex),
        Variable(name + ":" + "regex(" + regex + ")") { }

    // FIXME: no need for that.
    std::string m_regex;
    Utils::Regex m_r;
};

class Variables : public std::vector<Variable *> {
 public:
    bool contains(Variable *v) {
        return std::find_if(begin(), end(),
            [v](Variable *m) -> bool { return *v == *m; }) != end();
    };


    bool contains(const std::string &v) {
        return std::find_if(begin(), end(),
            [v](Variable *m) -> bool {
                VariableRegex *r = dynamic_cast<VariableRegex *>(m);
                if (r) {
                    return r->m_r.searchAll(v).size() > 0;
                }
                return v == *m->m_fullName.get();
            }) != end();
    };


    std::string getVariableNames(std::string sep = ",") {
        std::string names;
        for (auto a : *this) {
            if (names.length() > 0) {
                names = names + sep + *a->m_fullName;
            } else {
                names = *a->m_fullName;
            }
        }
        return names;
    }


    void ruleInit(const RuleWithActions *r) {
        for (auto a : *this) {
            a->ruleInit(r);
        }
    }
};


class VariableModificatorExclusion : public Variable {
 public:
    explicit VariableModificatorExclusion(std::unique_ptr<Variable> var)
        : m_base(std::move(var)), Variable(var.get()) { }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) {
        m_base->evaluate(t, l);
    }

    std::unique_ptr<Variable> m_base;
};


class VariableModificatorCount : public Variable {
 public:
    explicit VariableModificatorCount(std::unique_ptr<Variable> var)
        : Variable(var.get()),
          m_base(nullptr) {
            m_base.reset(var.release());
        }

    void evaluate(Transaction *t,
        std::vector<const VariableValue *> *l) {
        std::vector<const VariableValue *> reslIn;
        VariableValue *val = NULL;
        int count = 0;

        m_base->evaluate(t, &reslIn);

        for (const VariableValue *a : reslIn) {
            count++;
            delete a;
            a = NULL;
        }
        reslIn.clear();

        std::string *res = new std::string(std::to_string(count));
        val = new VariableValue(m_fullName.get(), res);
        delete res;

        l->push_back(val);
        return;
    }

    std::unique_ptr<Variable> m_base;
};


std::string operator+(std::string a, modsecurity::variables::Variable *v);
std::string operator+(std::string a, modsecurity::variables::Variables *v);


}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_VARIABLE_H_
