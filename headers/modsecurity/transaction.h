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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#endif

#include <stdlib.h>
#include <stddef.h>

#ifndef HEADERS_MODSECURITY_TRANSACTION_H_
#define HEADERS_MODSECURITY_TRANSACTION_H_

#ifndef __cplusplus
typedef struct ModSecurity_t ModSecurity;
typedef struct Transaction_t Transaction;
typedef struct Rules_t Rules;
#endif

#include "modsecurity/intervention.h"
#include "modsecurity/collection/collections.h"
#include "modsecurity/collection/variable.h"
#include "modsecurity/collection/collection.h"

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
class Transaction;
class Rules;
class RuleMessage;
namespace actions {
class Action;
namespace disruptive {
enum AllowType : int;
}
}
namespace RequestBodyProcessor {
class XML;
class JSON;
}
namespace operators {
class Operator;
}


/** @ingroup ModSecurity_CPP_API */
class Transaction {
 public:
    Transaction(ModSecurity *transaction, Rules *rules, void *logCbData);
    ~Transaction();

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

    int processResponseHeaders(int code, const std::string& proto);
    int addResponseHeader(const std::string& key, const std::string& value);
    int addResponseHeader(const unsigned char *key, const unsigned char *value);
    int addResponseHeader(const unsigned char *key, size_t len_key,
        const unsigned char *value, size_t len_value);

    int processResponseBody();
    int appendResponseBody(const unsigned char *body, size_t size);

    int processLogging();

    bool intervention(ModSecurityIntervention *it);

    bool addArgument(const std::string& orig, const std::string& key,
        const std::string& value);
    bool extractArguments(const std::string &orig, const std::string& buf);

    const char *getResponseBody();
    int getResponseBodyLenth();

#ifndef NO_LOGS
    void debug(int, std::string);
#endif
    void serverLog(const std::string& msg);

    std::string toJSON(int parts);
    std::string toOldAuditLogFormat(int parts, const std::string &trailer);
    std::string toOldAuditLogFormatIndex(const std::string &filename,
        double size, const std::string &md5);

    /**
     * Filled during the class instantiation, this variable can be later
     * used to fill the SecRule variable `duration'. The variable `duration'
     * is dynamic calculated, it is always relative to the value found in
     * m_creationTimeStamp.
     *
     * @note There is space for performance improvement. This value don't
	 *       need to be filled if there is no rule using the variable
	 *       `duration'.
     */
    clock_t m_creationTimeStamp;

    /**
     * Holds the client IP address.
     */
    const char *m_clientIpAddress;

    /**
     * Holds the HTTP version: 1.2, 2.0, 3.0 and so on....
     */
    const char *m_httpVersion;

    /**
     * Holds the request method: GET, POST, HEAD ...
     */
    const char *m_method;

    /**
     * Holds the server IP Address
     */
    const char *m_serverIpAddress;

    /**
     * Holds the raw URI that was requested.
     */
    const char *m_uri;

    /**
     * Holds the URI that was requests (without the query string).
     */
    std::string m_uri_no_query_string_decoded;

    /**
     * Holds the combined size of all arguments, later used to fill the
     * variable  ARGS_COMBINED_SIZE.
     */
    double m_ARGScombinedSize;

    /**
     * Client tcp port.
     */
    int m_clientPort;

    /**
     * This variable is set by the action `severity' and later can be
     * consulted via the SecLanguage variable HIGHEST_SEVERITY.
     */
    int m_highestSeverityAction;

    /**
     * Holds the HTTP return code when it is known. If 0 nothing was
	 * set.
     */
    int m_httpCodeReturned;

    /**
     * Holds the server port.
     */
    int m_serverPort;

    /**
     * ModSecurity instance used to start this transaction. Basically used
     * to fill the server log whenever is needed.
     */
    ModSecurity *m_ms;

    /**
     * Holds the type of the request body, in case there is one.
     */
    RequestBodyType m_requestBodyType;

    /**
     * Holds the request body "processor"
     */
    RequestBodyType m_requestBodyProcessor;

    /**
     * Rules object utilized during this specific transaction.
     */
    Rules *m_rules;

    /**
     *
     */
    std::list<int > m_ruleRemoveById;

    /**
     *
     */
    std::list< std::pair<std::string, std::string> > m_ruleRemoveTargetByTag;

    /**
     *
     */
    std::list< std::pair<int, std::string> > m_ruleRemoveTargetById;

    /**
     *
     */
    int m_requestBodyAccess;

    /**
     * The list m_auditLogModifier contains modifications to the `auditlogs'
     * for this specific request, those modifications can happens via the
     * utilization of the action: `ctl:auditLogParts='
     *
     */
    std::list< std::pair<int, std::string> > m_auditLogModifier;

    /**
     * This variable holds all the messages asked to be save by the utilization
     * of the actions: `log_data' and `msg'. These should be included on the
     * auditlogs.
     */
    std::list<modsecurity::RuleMessage> m_rulesMessages;

    /**
     * Holds the request body, in case of any.
     */
    std::ostringstream m_requestBody;

    /**
     * Holds the response body, in case of any.
     */
    std::ostringstream m_responseBody;

    /**
     * Contains the unique ID of the transaction. Use by the variable
	 * `UNIQUE_ID'. This unique id is also saved as part of the AuditLog.
     */
    std::string m_id;

    /**
     * Holds the SecMarker name that this transaction should wait to perform
     * rules evaluation again.
     */
    std::string m_marker;

    /**
     * Holds the amount of rules that should be skipped. If bigger than 0 the
     * current rule should be skipped and the number needs to be decreased.
     */
    int m_skip_next;

    /**
     * If allow action was utilized, this variable holds the allow type.
     */
    modsecurity::actions::disruptive::AllowType m_allowType;

    /**
     * Holds the decode URI. Notice that m_uri holds the raw version
     * of the URI.
     */
    std::string m_uri_decoded;

    /**
     * Actions (disruptive?) that should be taken by the connector related to
     * that transaction.
     */
    std::vector<ModSecurityIntervention> m_actions;
    ModSecurityIntervention m_it;

    /**
     * Holds the creation time stamp, using std::time.
     *
     * TODO: m_timeStamp and m_creationTimeStamp may be merged into a single
     *       variable.
     */
    time_t m_timeStamp;


    /**
     * Holds all the collections related to that transaction.
     */
    collection::Collections m_collections;

    /**
     * Holds the whatever matched in the operation utilization.
     * That variable will be further used by the capture action.
     *
     */
    std::list<std::string> m_matched;

    RequestBodyProcessor::XML *m_xml;
    RequestBodyProcessor::JSON *m_json;

    std::string m_variableDuration;
    std::map<std::string, std::string> m_variableEnvs;
    std::string m_variableHighestSeverityAction;
    std::string m_variableRemoteUser;
    std::string m_variableTime;
    std::string m_variableTimeDay;
    std::string m_variableTimeEpoch;
    std::string m_variableTimeHour;
    std::string m_variableTimeMin;
    std::string m_variableTimeSec;
    std::string m_variableTimeWDay;
    std::string m_variableTimeYear;

 private:
    std::string *m_ARGScombinedSizeStr;
    std::string *m_namesArgs;
    std::string *m_namesArgsGet;
    std::string *m_namesArgsPost;
    std::string *m_requestHeadersNames;
    std::string *m_responseContentType;
    std::string *m_responseHeadersNames;

    /**
     * Pointer to the callback function that will be called to fill
     * the web server (connector) log.
     */
    void *m_logCbData;
};


#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup ModSecurity_C_API */
Transaction *msc_new_transaction(ModSecurity *ms,
    Rules *rules, void *logCbData);

/** @ingroup ModSecurity_C_API */
int msc_process_connection(Transaction *transaction,
    const char *client, int cPort, const char *server, int sPort);

/** @ingroup ModSecurity_C_API */
int msc_process_request_headers(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
int msc_add_request_header(Transaction *transaction, const unsigned char *key,
    const unsigned char *value);

/** @ingroup ModSecurity_C_API */
int msc_add_n_request_header(Transaction *transaction,
    const unsigned char *key, size_t len_key, const unsigned char *value,
    size_t len_value);

/** @ingroup ModSecurity_C_API */
int msc_process_request_body(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
int msc_append_request_body(Transaction *transaction,
    const unsigned char *body, size_t size);

/** @ingroup ModSecurity_C_API */
int msc_request_body_from_file(Transaction *transaction, const char *path);

/** @ingroup ModSecurity_C_API */
int msc_process_response_headers(Transaction *transaction, int code,
    const char* protocol);

/** @ingroup ModSecurity_C_API */
int msc_add_response_header(Transaction *transaction,
    const unsigned char *key, const unsigned char *value);

/** @ingroup ModSecurity_C_API */
int msc_add_n_response_header(Transaction *transaction,
    const unsigned char *key, size_t len_key, const unsigned char *value,
    size_t len_value);

/** @ingroup ModSecurity_C_API */
int msc_process_response_body(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
int msc_append_response_body(Transaction *transaction,
    const unsigned char *body, size_t size);

/** @ingroup ModSecurity_C_API */
int msc_process_uri(Transaction *transaction, const char *uri,
    const char *protocol, const char *http_version);

/** @ingroup ModSecurity_C_API */
const char *msc_get_response_body(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
int msc_get_response_body_length(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
void msc_transaction_cleanup(Transaction *transaction);

/** @ingroup ModSecurity_C_API */
int msc_intervention(Transaction *transaction, ModSecurityIntervention *it);

/** @ingroup ModSecurity_C_API */
int msc_process_logging(Transaction *transaction);

#ifdef __cplusplus
}
}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_TRANSACTION_H_
