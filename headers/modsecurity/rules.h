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

#ifdef __cplusplus
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#endif


#ifndef HEADERS_MODSECURITY_RULES_H_
#define HEADERS_MODSECURITY_RULES_H_

#ifndef __cplusplus
typedef struct Rules_t Rules;
typedef struct Assay_t Assay;
#endif

#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"
#include "modsecurity/rules_properties.h"

#ifdef __cplusplus

namespace ModSecurity {
class Rule;
class AuditLog;
namespace Parser {
class Driver;
}

/** @ingroup ModSecurity_CPP_API */
class Rules : public RulesProperties  {
 public:
    Rules()
        : RulesProperties(NULL) { }

    Rules(DebugLog *customLog)
        : RulesProperties(customLog) { }

    ~Rules();

    void incrementReferenceCount(void);
    void decrementReferenceCount(void);

    bool loadFromUri(const char *uri);
    bool loadRemote(char *key, char *uri);
    bool load(const char *rules);
    bool load(const char *rules, const std::string &ref);

    void dump();

    int merge(Parser::Driver *driver);
    int merge(Rules *rules);

    int evaluate(int phase, Assay *assay);
    std::string getParserError();

    void debug(int level, std::string message);

    std::ostringstream parserError;

    DebugLog *debugLog;
 private:
    int m_referenceCount;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

Rules *msc_create_rules_set();
void msc_rules_dump(Rules *rules);
int msc_rules_merge(Rules *rules_dst, Rules *rules_from);
int msc_rules_add_remote(Rules *rules, char *key, char *uri,
    const char **error);
int msc_rules_add_file(Rules *rules, char *file, const char **error);
int msc_rules_add(Rules *rules, const char *plain_rules, const char **error);
int msc_rules_cleanup(Rules *rules);

#ifdef __cplusplus
}
}  // namespace ModSecurity
#endif

#endif  // HEADERS_MODSECURITY_RULES_H_
