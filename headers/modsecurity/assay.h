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
typedef struct ModSecurity_t ModSecurity;
typedef struct Assay_t Assay;
typedef struct Rules_t Rules;
#endif

#include "modsecurity/intervention.h"
#include "modsecurity/transaction/variable.h"
#include "modsecurity/transaction/variables.h"
#include "modsecurity/transaction/collections.h"

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

namespace modsecurity {

class ModSecurity;
class Assay;
class Rules;
class Collections;
namespace actions {
class Action;
}
namespace operators {
class Operator;
}


/** @ingroup ModSecurity_CPP_API */
class Assay {
 public:
    Assay(ModSecurity *assay, Rules *rules, void *logCbData);
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
    int requestBodyFromFile(const char *path);

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

    const char *getResponseBody();
    int getResponseBodyLenth();

    transaction::Collections m_collections;
#ifndef NO_LOGS
    void debug(int, std::string);
#endif
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

    std::list< std::pair<int, std::string> > auditLogModifier;
    std::string m_marker;

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
    void *m_logCbData;
};


#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup ModSecurity_C_API */
Assay *msc_new_assay(ModSecurity *ms, Rules *rules, void *logCbData);

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
int msc_request_body_from_file(Assay *assay, const char *path);

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
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_ASSAY_H_
