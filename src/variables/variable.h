/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2023 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
    explicit n ## _DictElementRegexp(const std::string &regex) \
        : VariableRegex(#N, regex) { } \
\
    void evaluate(Transaction *transaction, \
        RuleWithActions *rule, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .resolveRegularExpression(&m_r, l, \
            m_keyExclusion); \
    } \
};


#define VAR_WITH_DICT_ELEMENT(n, N, e) \
class n ## _DictElement : public VariableDictElement { \
 public: \
    explicit n ## _DictElement(const std::string &dictElement) \
        : VariableDictElement(#N, dictElement) { } \
\
    void evaluate(Transaction *transaction, \
        RuleWithActions *rule, \
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
        RuleWithActions *rule, \
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
        RuleWithActions *rule, \
        std::vector<const VariableValue *> *l) override { \
        transaction-> e .evaluate(l); \
    } \
};


namespace modsecurity {

class Transaction;
namespace variables {

class KeyExclusion {
 public:
    KeyExclusion() { }
    virtual bool match(const std::string &a) = 0;
    virtual ~KeyExclusion() { }
};


// FIXME: use pre built regex.
class KeyExclusionRegex : public KeyExclusion {
 public:
    explicit KeyExclusionRegex(const Utils::Regex &re)
        : m_re(re.pattern, true) { }
    explicit KeyExclusionRegex(const std::string &re)
        : m_re(re, true) { }

    ~KeyExclusionRegex() override { }

    bool match(const std::string &a) override {
        return m_re.searchAll(a).size() > 0;
    }

    Utils::Regex m_re;
};


class KeyExclusionString : public KeyExclusion {
 public:
    explicit KeyExclusionString(const std::string &a)
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
    KeyExclusions() {
    }

    bool toOmit(std::string a) const {
        for (auto &z : *this) {
            if (z->match(a)) {
                return true;
            }
        }
        return false;
    }

    bool toOmit(std::string a) { // cppcheck-suppress passedByValue
        return static_cast<const KeyExclusions&>(*this).toOmit(a);
    }
};


class VariableMonkeyResolution {
 public:
    VariableMonkeyResolution () { }
    static inline bool comp(const std::string &a, const std::string &b) {
        return a.size() == b.size()
             && std::equal(a.begin(), a.end(), b.begin(),
            [](char aa, char bb) {
            return toupper(aa) == bb;
        });
    }

    static void stringMatchResolveMulti(Transaction *t,
        const std::string &variable,
        std::vector<const VariableValue *> *l) {
        size_t collection_delimiter_offset = variable.find(".");
        if (collection_delimiter_offset == std::string::npos) {
            collection_delimiter_offset = variable.find(":");
        }
        std::string col; // collection name excluding individual variable specification
        std::string var; // variable within the collection
        if (collection_delimiter_offset == std::string::npos) {
            col = variable;
        } else {
            col = std::string(variable, 0, collection_delimiter_offset);
            var = std::string(variable, collection_delimiter_offset + 1,
                variable.length() - (collection_delimiter_offset + 1));
	}

        // First check if the request is for a collection of type AnchoredSetVariable
        AnchoredSetVariable* anchoredSetVariable = NULL;
        if (comp(col, "ARGS")) {
            anchoredSetVariable = &t->m_variableArgs;
        } else if (comp(col, "ARGS_RAW")) {
            anchoredSetVariable = &t->m_variableArgsRaw;
        } else if (comp(col, "ARGS_GET")) {
            anchoredSetVariable = &t->m_variableArgsGet;
        } else if (comp(col, "ARGS_GET_RAW")) {
            anchoredSetVariable = &t->m_variableArgsGetRaw;
        } else if (comp(col, "ARGS_POST")) {
            anchoredSetVariable = &t->m_variableArgsPost;
        } else if (comp(col, "ARGS_POST_RAW")) {
            anchoredSetVariable = &t->m_variableArgsPostRaw;
        } else if (comp(col, "FILES_SIZES")) {
            anchoredSetVariable = &t->m_variableFilesSizes;
        } else if (comp(col, "FILES_NAMES")) {
            anchoredSetVariable = &t->m_variableFilesNames;
        } else if (comp(col, "FILES_TMP_CONTENT")) {
            anchoredSetVariable = &t->m_variableFilesTmpContent;
        } else if (comp(col, "MULTIPART_FILENAME")) {
            anchoredSetVariable = &t->m_variableMultipartFileName;
        } else if (comp(col, "MULTIPART_NAME")) {
            anchoredSetVariable = &t->m_variableMultipartName;
        } else if (comp(col, "MATCHED_VARS_NAMES")) {
            anchoredSetVariable = &t->m_variableMatchedVarsNames;
        } else if (comp(col, "MATCHED_VARS")) {
            anchoredSetVariable = &t->m_variableMatchedVars;
        } else if (comp(col, "FILES")) {
            anchoredSetVariable = &t->m_variableFiles;
        } else if (comp(col, "REQUEST_COOKIES")) {
            anchoredSetVariable = &t->m_variableRequestCookies;
        } else if (comp(col, "REQUEST_HEADERS")) {
            anchoredSetVariable = &t->m_variableRequestHeaders;
        } else if (comp(variable, "REQUEST_HEADERS_NAMES")) {
            anchoredSetVariable = &t->m_variableRequestHeadersNames;
        } else if (comp(col, "RESPONSE_HEADERS")) {
            anchoredSetVariable = &t->m_variableResponseHeaders;
        } else if (comp(variable, "RESPONSE_HEADERS_NAMES")) {
            anchoredSetVariable = &t->m_variableResponseHeadersNames;
        } else if (comp(col, "GEO")) {
            anchoredSetVariable = &t->m_variableGeo;
        } else if (comp(col, "REQUEST_COOKIES_NAMES")) {
            anchoredSetVariable = &t->m_variableRequestCookiesNames;
        } else if (comp(col, "MULTIPART_PART_HEADERS")) {
            anchoredSetVariable = &t->m_variableMultipartPartHeaders;
        } else if (comp(col, "FILES_TMPNAMES")) {
            anchoredSetVariable = &t->m_variableFilesTmpNames;
        }
        if (anchoredSetVariable != NULL) {
            if (collection_delimiter_offset == std::string::npos) {
                anchoredSetVariable->resolve(l);
            } else {
                anchoredSetVariable->resolve(var, l);
            }
            return;
        }

        // Next check for collection of type AnchoredSetVariableTranslationProxy
        AnchoredSetVariableTranslationProxy* anchoredSetVariableTranslationProxy = NULL;
        if (comp(col, "ARGS_NAMES")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsNames;
        } else if (comp(col, "ARGS_NAMES_RAW")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsNamesRaw;
        } else if (comp(col, "ARGS_GET_NAMES")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsGetNames;
        } else if (comp(col, "ARGS_GET_NAMES_RAW")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsGetNamesRaw;
        } else if (comp(col, "ARGS_POST_NAMES")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsPostNames;
        } else if (comp(col, "ARGS_POST_NAMES_RAW")) {
            anchoredSetVariableTranslationProxy = &t->m_variableArgsPostNamesRaw;
        }
        if (anchoredSetVariableTranslationProxy != NULL) {
            if (collection_delimiter_offset
