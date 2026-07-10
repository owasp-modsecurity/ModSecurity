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


#ifdef __cplusplus
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <cctype>
#endif


#include "modsecurity/variable_value.h"


#ifndef HEADERS_MODSECURITY_COLLECTION_COLLECTION_H_
#define HEADERS_MODSECURITY_COLLECTION_COLLECTION_H_

#ifndef __cplusplus
typedef struct Variable_t Variables;
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace variables {
class KeyExclusions;
}
namespace collection {

class Collection {
 public:
    explicit Collection(const std::string &a) : m_name(a) { }
    virtual ~Collection() { }

    /*
     * Collection variable names are case-insensitive in SecLang (e.g.
     * `setvar:ip.counter` and `IP:COUNTER` refer to the same key). Normalise
     * the variable-name term of the composite LMDB key to lowercase so that
     * writes and reads performed with different casing hit the same entry.
     * The compartment values (collection key, web app id) are intentionally
     * left untouched since they may be opaque, case-sensitive identifiers.
     */
    static std::string normKey(const std::string &k) {
        std::string out;
        out.reserve(k.size());
        for (char c : k) {
            out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        return out;
    }

    virtual bool storeOrUpdateFirst(const std::string &key,
        const std::string &value) = 0;

    virtual bool updateFirst(const std::string &key,
        const std::string &value) = 0;

    virtual void del(const std::string& key) = 0;

    virtual void setExpiry(const std::string& key, int32_t expiry_seconds) = 0;

    virtual std::unique_ptr<std::string> resolveFirst(
        const std::string& var) = 0;

    virtual void resolveSingleMatch(const std::string& var,
        std::vector<const VariableValue *> *l) = 0;
    virtual void resolveMultiMatches(const std::string& var,
        std::vector<const VariableValue *> *l,
        variables::KeyExclusions &ke) = 0;
    virtual void resolveRegularExpression(const std::string& var,
        std::vector<const VariableValue *> *l,
        variables::KeyExclusions &ke) = 0;


    /* storeOrUpdateFirst */
    virtual bool storeOrUpdateFirst(const std::string &key,
        std::string compartment, const std::string &value) {
        std::string nkey = compartment + "::" + normKey(key);
        return storeOrUpdateFirst(nkey, value);
    }


    virtual bool storeOrUpdateFirst(const std::string &key,
        std::string compartment, std::string compartment2,
        const std::string &value) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(key);
        return storeOrUpdateFirst(nkey, value);
    }


    /* updateFirst */
    virtual bool updateFirst(const std::string &key, std::string compartment,
        const std::string &value) {
        std::string nkey = compartment + "::" + normKey(key);
        return updateFirst(nkey, value);
    }


    virtual bool updateFirst(const std::string &key, std::string compartment,
        std::string compartment2, const std::string &value) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(key);
        return updateFirst(nkey, value);
    }


    /* del */
    virtual void del(const std::string& key, std::string compartment) {
        std::string nkey = compartment + "::" + normKey(key);
        del(nkey);
    }


    virtual void del(const std::string& key, std::string compartment,
        std::string compartment2) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(key);
        del(nkey);
    }


    /* setExpiry */
    virtual void setExpiry(const std::string& key, std::string compartment,
        int32_t expiry_seconds) {
        std::string nkey = compartment + "::" + normKey(key);
        setExpiry(nkey, expiry_seconds);
    }


    virtual void setExpiry(const std::string& key, std::string compartment,
        std::string compartment2, int32_t expiry_seconds) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(key);
        setExpiry(nkey, expiry_seconds);
    }


    /* resolveFirst */
    virtual std::unique_ptr<std::string> resolveFirst(const std::string& var,
        std::string compartment) {
        std::string nkey = compartment + "::" + normKey(var);
        return resolveFirst(nkey);
    }


    virtual std::unique_ptr<std::string> resolveFirst(const std::string& var,
        std::string compartment, std::string compartment2) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(var);
        return resolveFirst(nkey);
    }


    /* resolveSingleMatch */
    virtual void resolveSingleMatch(const std::string& var,
        std::string compartment, std::vector<const VariableValue *> *l) {
        std::string nkey = compartment + "::" + normKey(var);
        resolveSingleMatch(nkey, l);
    }


    virtual void resolveSingleMatch(const std::string& var,
        std::string compartment, std::string compartment2,
        std::vector<const VariableValue *> *l) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(var);
        resolveSingleMatch(nkey, l);
    }


    /* resolveMultiMatches */
    virtual void resolveMultiMatches(const std::string& var,
        std::string compartment, std::vector<const VariableValue *> *l,
        variables::KeyExclusions &ke) {
        std::string nkey = compartment + "::" + normKey(var);
        resolveMultiMatches(nkey, l, ke);
    }


    virtual void resolveMultiMatches(const std::string& var,
        std::string compartment, std::string compartment2,
        std::vector<const VariableValue *> *l,
        variables::KeyExclusions &ke) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(var);
        resolveMultiMatches(nkey, l, ke);
    }


    /* resolveRegularExpression */
    virtual void resolveRegularExpression(const std::string& var,
        std::string compartment, std::vector<const VariableValue *> *l,
        variables::KeyExclusions &ke) {
        std::string nkey = compartment + "::" + normKey(var);
        resolveRegularExpression(nkey, l, ke);
    }


    virtual void resolveRegularExpression(const std::string& var,
        std::string compartment, std::string compartment2,
        std::vector<const VariableValue *> *l, variables::KeyExclusions &ke) {
        std::string nkey = compartment + "::" + compartment2 + "::" + normKey(var);
        resolveRegularExpression(nkey, l, ke);
    }

    std::string m_name;
};

}  // namespace collection
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_COLLECTION_COLLECTION_H_
