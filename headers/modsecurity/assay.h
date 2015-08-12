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
#include <list>
#include <iomanip>
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <vector>
#include <utility>
#endif

#include <stdlib.h>
#include <stddef.h>

#ifndef HEADERS_MODSECURITY_ASSAY_H_
#define HEADERS_MODSECURITY_ASSAY_H_

#ifndef __cplusplus
typedef struct Assay_t Assay;
typedef struct Rules_t Rules;
#endif

#include "modsecurity/intervention.h"

#define LOGFY_ADD(a, b) \
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>(a), strlen(a)); \
    if (b == NULL) { \
      yajl_gen_string(g, reinterpret_cast<const unsigned char*>(""), \
          strlen("")); \
    } else { \
      yajl_gen_string(g, reinterpret_cast<const unsigned char*>(b), \
          strlen(b)); \
    }


#define LOGFY_ADD_INT(a, b) \
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>(a), strlen(a)); \
    yajl_gen_number(g, reinterpret_cast<const char*>(b), strlen(b));

#define LOGFY_ADD_NUM(a, b) \
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>(a), strlen(a)); \
    yajl_gen_integer(g, b);

#ifdef __cplusplus

namespace ModSecurity {

class ModSecurity;
class Assay;
class Rules;
namespace actions {
class Action;
}
namespace operators {
class Operator;
}

class ModSecurityHeader {
 public:
    ModSecurityHeader(char *key, char *value)
        : _key(key),
        _value(value)
        { }

    char *_key;
    char *_value;
};


class ModSecurityCollectionsVariables :
    public std::unordered_map<std::string,
        std::unordered_map<std::string, std::string>> {
 public:
};


class ModSecurityStringVariables :
    public std::unordered_multimap<std::string, std::string> {
 public:
    void storeVariable(std::string key, std::string value) {
        this->emplace(key, value);
    }

    bool storeOrUpdateVariable(const std::string &key,
        const std::string &value) {
        if (updateFirstVariable(key, value) == false) {
            storeVariable(key, value);
        }
        return true;
    }


    bool updateFirstVariable(const std::string &key, const std::string &value) {
        auto range = this->equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            it->second = value;
            return true;
        }
        return false;
    }


    void deleteVariable(const std::string& key) {
        this->erase(key);
    }


    std::list<std::pair<std::string, std::string>>
        resolveVariable(const std::string& key) {
        std::list<std::pair<std::string, std::string>> l;
        std::pair<std::string, std::string> pair;

        auto range = this->equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            pair = std::make_pair(std::string(key), std::string(it->second));
            l.push_back(pair);
        }

        if (l.size() == 0) {
            for (auto& x : *this) {
                if ((x.first.substr(0, key.size() + 1).compare(key + ":") != 0)
                    && (x.first != key)) {
                    continue;
                }
                std::list<std::pair<std::string, std::string>> t;
                t = this->resolveVariable(x.first);
                for (std::pair<std::string, std::string> z : t) {
                    pair = std::make_pair(std::string(z.first),
                        std::string(z.second));
                    l.push_back(pair);
                }
            }
        }

        return l;
    }
};

/** @ingroup ModSecurity_CPP_API */
class Assay {
 public:
    Assay(ModSecurity *assay, Rules *rules);
    ~Assay();

    /** TODO: Should be an structure that fits an IP address */
    int processConnection(const char *client, int cPort,
        const char *server, int sPort);
    int processURI(const char *uri, const char *protocol,
        const char *http_version);

    /**
     * Types of request body that ModSecurity may give a special treatment
     * for the data.
     */
    enum RequestBodyType {
      /**
       *
       */
      UnknownFormat,
      /**
       *
       */
      MultiPartRequestBody,
      /**
       *
       */
      WWWFormUrlEncoded,
      /**
       *
       */
      JSONRequestBody,
      /**
       *
       */
      XMLRequestBody
    };

    int processRequestHeaders();
    int addRequestHeader(const std::string& key, const std::string& value);
    int addRequestHeader(const unsigned char *key, const unsigned char *value);
    int addRequestHeader(const unsigned char *key, size_t len_key,
        const unsigned char *value, size_t len_value);

    int processRequestBody();
    int appendRequestBody(const unsigned char *body, size_t size);

    int processResponseHeaders();
    int addResponseHeader(const std::string& key, const std::string& value);
    int addResponseHeader(const unsigned char *key, const unsigned char *value);
    int addResponseHeader(const unsigned char *key, size_t len_key,
        const unsigned char *value, size_t len_value);

    int processResponseBody();
    int appendResponseBody(const unsigned char *body, size_t size);

    int processLogging(int status_code);

    bool intervention(ModSecurityIntervention *it);

    void cleanup();

    void setCollection(const std::string& collectionName,
        const std::string& variableName,
        const std::string& targetValue);

    const char *getResponseBody();
    int getResponseBodyLenth();

    std::list<std::pair<std::string, std::string>>
        resolve_variable(const std::string& var);
    std::string* resolve_variable_first(const std::string& key);
    std::string* resolve_variable_first(const std::string& collectionName,
       const std::string& var);

    void store_variable(std::string, std::string value);
    bool update_variable_first(std::string var, const std::string &value);
    void delete_variable(std::string key);

    ModSecurityStringVariables m_variables_strings;
    std::unordered_map<std::string, ModSecurityStringVariables *> collections;

    void debug(int, std::string);
    void serverLog(const std::string& msg);
    std::vector<actions::Action *> actions;

    bool save_in_auditlog;
    bool do_not_save_in_auditlog;

    int httpCodeReturned;

    std::string to_json(int parts);
    std::string toOldAuditLogFormat(int parts, const std::string &trailer);
    std::string toOldAuditLogFormatIndex(const std::string &filename,
        double size, const std::string &md5);

    std::string id;
    time_t timeStamp;
    clock_t start;
    int highest_severity;

    Rules *m_rules;

    std::list<std::string> rulesMessages;
    std::list<std::string> ruleTags;

 private:
    std::ofstream myfile;
    ModSecurity *m_ms;

    const char *m_clientIpAddress;
    const char *m_serverIpAddress;
    int m_clientPort;
    int m_serverPort;
    const char *m_uri;
    std::string m_uri_decoded;
    const char *m_protocol;
    const char *m_httpVersion;

    std::string *m_namesArgs;
    std::string *m_namesArgsPost;
    std::string *m_namesArgsGet;
    std::string *m_requestHeadersNames;
    std::string *m_responseHeadersNames;
    double m_ARGScombinedSize;
    /** TODO: Support to save double in the storage. */
    std::string *m_ARGScombinedSizeStr;
    RequestBodyType m_requestBodyType;

    std::ostringstream m_requestBody;
    std::ostringstream m_responseBody;
    ModSecurityCollectionsVariables m_variables_collections;
};


#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup ModSecurity_C_API */
Assay *msc_new_assay(ModSecurity *ms, Rules *rules);

/** @ingroup ModSecurity_C_API */
int msc_process_connection(Assay *assay, const char *client, int cPort,
    const char *server, int sPort);

/** @ingroup ModSecurity_C_API */
int msc_process_request_headers(Assay *assay);

/** @ingroup ModSecurity_C_API */
int msc_add_request_header(Assay *assay, const unsigned char *key,
    const unsigned char *value);

/** @ingroup ModSecurity_C_API */
int msc_add_n_request_header(Assay *assay, const unsigned char *key,
    size_t len_key, const unsigned char *value, size_t len_value);

/** @ingroup ModSecurity_C_API */
int msc_process_request_body(Assay *assay);

/** @ingroup ModSecurity_C_API */
int msc_append_request_body(Assay *assay,
    const unsigned char *body, size_t size);

/** @ingroup ModSecurity_C_API */
int msc_process_response_headers(Assay *assay);
/** @ingroup ModSecurity_C_API */
int msc_add_response_header(Assay *assay, const unsigned char *key,
    const unsigned char *value);
/** @ingroup ModSecurity_C_API */
int msc_add_n_response_header(Assay *assay, const unsigned char *key,
    size_t len_key, const unsigned char *value, size_t len_value);

/** @ingroup ModSecurity_C_API */
int msc_process_response_body(Assay *assay);
/** @ingroup ModSecurity_C_API */
int msc_append_response_body(Assay *assay,
    const unsigned char *body, size_t size);

/** @ingroup ModSecurity_C_API */
int msc_process_uri(Assay *assay, const char *uri, const char *protocol,
    const char *http_version);

/** @ingroup ModSecurity_C_API */
const char *msc_get_response_body(Assay *assay);
/** @ingroup ModSecurity_C_API */
int msc_get_response_body_length(Assay *assay);

/** @ingroup ModSecurity_C_API */
void msc_assay_cleanup(Assay *assay);

/** @ingroup ModSecurity_C_API */
int msc_intervention(Assay *assay, ModSecurityIntervention *it);

/** @ingroup ModSecurity_C_API */
int msc_process_logging(Assay *assay, int code);

#ifdef __cplusplus
}
}  // namespace ModSecurity
#endif


#endif  // HEADERS_MODSECURITY_ASSAY_H_
