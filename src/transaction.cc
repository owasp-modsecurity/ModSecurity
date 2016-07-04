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

#include "modsecurity/transaction.h"

#ifdef WITH_YAJL
#include <yajl/yajl_tree.h>
#include <yajl/yajl_gen.h>
#endif

#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

#include "actions/action.h"
#include "actions/deny.h"
#include "modsecurity/intervention.h"
#include "modsecurity/modsecurity.h"
#include "request_body_processor/multipart.h"
#include "request_body_processor/xml.h"
#include "request_body_processor/json.h"
#include "audit_log/audit_log.h"
#include "src/unique_id.h"
#include "src/utils.h"
#include "modsecurity/rule.h"
#include "src/actions/allow.h"

using modsecurity::actions::Action;
using modsecurity::RequestBodyProcessor::Multipart;
using modsecurity::RequestBodyProcessor::XML;

namespace modsecurity {

/**
 * @name    Transaction
 * @brief   Represents the inspection on an entire request.
 *
 * An instance of the Transaction class represents an entire request, on its
 * different phases.
 *
 * @param ms ModSecurity core instance.
 * @param rules Rules instance.
 *
 * Example Usage:
 * @code
 *
 * using ModSecurity::ModSecurity;
 * using ModSecurity::Rules;
 * using ModSecurity::Transaction;
 *
 * ModSecurity *modsec;
 * ModSecurity::Rules *rules;
 *
 * modsec = new ModSecurity();
 * rules = new Rules();
 * rules->loadFromUri(rules_file);
 *
 * Transaction *modsecTransaction = new Transaction(modsec, rules);
 * modsecTransaction->processConnection("127.0.0.1", 33333, "127.0.0.1", 8080);
 *
 * if (modsecTransaction->intervention()) {
 *     std::cout << "There is an intervention" << std::endl;
 * }
 *
 * ...
 *
 * delete modsecTransaction;
 *
 * @endcode
 *
 */
Transaction::Transaction(ModSecurity *ms, Rules *rules, void *logCbData)
    : m_clientIpAddress(""),
    m_serverIpAddress(""),
    m_clientPort(0),
    m_serverPort(0),
    m_uri(""),
    m_method(""),
    m_httpVersion(""),
    m_rules(rules),
    m_toBeSavedInAuditlogs(false),
    m_toNotBeSavedInAuditLogs(false),
    m_timeStamp(std::time(NULL)),
    m_httpCodeReturned(200),
    m_highestSeverityAction(255),
    m_ARGScombinedSize(0),
    m_ARGScombinedSizeStr(NULL),
    m_namesArgs(NULL),
    m_namesArgsPost(NULL),
    m_namesArgsGet(NULL),
    m_requestBodyType(UnknownFormat),
    m_requestBodyProcessor(UnknownFormat),
    m_requestHeadersNames(NULL),
    m_responseHeadersNames(NULL),
    m_responseContentType(NULL),
    m_marker(""),
    m_allowType(modsecurity::actions::NoneAllowType),
    m_skip_next(0),
    m_creationTimeStamp(cpu_seconds()),
    m_logCbData(logCbData),
    m_ms(ms),
    m_collections(ms->m_global_collection, ms->m_ip_collection,
        ms->m_session_collection, ms->m_user_collection,
        ms->m_resource_collection),
    m_json(new RequestBodyProcessor::JSON(this)),
    m_xml(new RequestBodyProcessor::XML(this)) {
    m_id = std::to_string(this->m_timeStamp) + \
        std::to_string(generate_transaction_unique_id());
    m_rules->incrementReferenceCount();

    m_collections.store("ARGS_COMBINED_SIZE", std::string("0"));
    m_ARGScombinedSizeStr = m_collections.resolveFirst("ARGS_COMBINED_SIZE");
    m_collections.store("ARGS_NAMES", std::string(""));
    this->m_namesArgs = m_collections.resolveFirst("ARGS_NAMES");
    m_collections.store("ARGS_POST_NAMES", std::string(""));
    this->m_namesArgsPost = m_collections.resolveFirst("ARGS_POST_NAMES");
    m_collections.store("ARGS_GET_NAMES", std::string(""));
    this->m_namesArgsGet = m_collections.resolveFirst("ARGS_GET_NAMES");
    m_collections.store("REQUEST_HEADERS_NAMES", std::string(""));
    this->m_requestHeadersNames = m_collections.resolveFirst(
        "REQUEST_HEADERS_NAMES");
    m_collections.store("RESPONSE_HEADERS_NAMES", std::string(""));
    this->m_responseHeadersNames = m_collections.resolveFirst(
        "RESPONSE_HEADERS_NAMES");
    m_collections.store("RESPONSE_CONTENT_TYPE", std::string(""));
    this->m_responseContentType = m_collections.resolveFirst(
        "RESPONSE_CONTENT_TYPE");

    m_collections.storeOrUpdateFirst("URLENCODED_ERROR", "0");

#ifndef NO_LOGS
    this->debug(4, "Initialising transaction");
#endif
}


Transaction::~Transaction() {
    m_responseBody.str(std::string());
    m_responseBody.clear();

    m_requestBody.str(std::string());
    m_requestBody.clear();

    for (auto *a : m_rulesMessages) {
        delete a;
    }
    m_rulesMessages.clear();

    m_rules->decrementReferenceCount();

    delete m_json;
    delete m_xml;
}


/**
 * @name    debug
 * @brief   Prints a message on the debug logs.
 *
 * Debug logs are important during the rules creation phase, this method can be
 * used to print message on this debug log.
 * 
 * @param level Debug level, current supported from 0 to 9.
 * @param message Message to be logged.
 *
 */
#ifndef NO_LOGS
void Transaction::debug(int level, std::string message) {
    if (m_rules == NULL) {
        return;
    }

    m_rules->debug(level, message);
}
#endif

/**
 * @name    processConnection
 * @brief   Perform the analysis on the connection.
 *
 * This method should be called at very beginning of a request process, it is
 * expected to be executed prior to the virtual host resolution, when the
 * connection arrives on the server.
 *
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity Transaction.
 * @param client Client's IP address in text format.
 * @param cPort Client's port
 * @param server Server's IP address in text format.
 * @param sPort Server's port
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processConnection(const char *client, int cPort,
    const char *server, int sPort) {
    this->m_clientIpAddress = client;
    this->m_serverIpAddress = server;
    this->m_clientPort = cPort;
    this->m_serverPort = sPort;
#ifndef NO_LOGS
    debug(4, "Transaction context created.");
    debug(4, "Starting phase CONNECTION. (SecRules 0)");
#endif

    this->m_collections.store("REMOTE_HOST", m_clientIpAddress);
    this->m_collections.store("UNIQUE_ID", m_id);
    this->m_collections.store("REMOTE_ADDR", m_clientIpAddress);
    this->m_collections.store("SERVER_ADDR", m_serverIpAddress);
    this->m_collections.store("SERVER_PORT",
        std::to_string(this->m_serverPort));
    this->m_collections.store("REMOTE_PORT",
        std::to_string(this->m_clientPort));
    this->m_rules->evaluate(ModSecurity::ConnectionPhase, this);
    return true;
}


bool Transaction::extractArguments(const std::string &orig,
    const std::string& buf) {
    char sep1 = '&';
    std::vector<std::string> key_value_sets = split(buf, sep1);

    for (std::string t : key_value_sets) {
        char sep2 = '=';
        int i = 0;
        size_t key_s = 0;
        size_t value_s = 0;
        int invalid = 0;
        int changed = 0;

        std::string key;
        std::string value;
        std::vector<std::string> key_value = split(t, sep2);
        for (auto& a : key_value) {
            if (i == 0) {
                key = a;
            } else if (i == 1) {
                value = a;
            } else {
                value = value + "=" + a;
            }
            i++;
        }

        key_s = (key.length() + 1);
        value_s = (value.length() + 1);
        unsigned char *key_c = (unsigned char *) malloc(sizeof(char) * key_s);
        unsigned char *value_c = (unsigned char *) malloc(sizeof(char) * value_s);

        memset(key_c, '\0', sizeof(char) * key_s);
        memset(value_c, '\0', sizeof(char) * value_s);

        memcpy(key_c, key.c_str(), key_s);
        memcpy(value_c, value.c_str(), value_s);

        key_s = urldecode_nonstrict_inplace(key_c, key_s, &invalid, &changed);
        value_s = urldecode_nonstrict_inplace(value_c, value_s, &invalid, &changed);

        if (invalid) {
            m_collections.storeOrUpdateFirst("URLENCODED_ERROR", "1");
        }

        addArgument(orig, std::string((char *)key_c, key_s-1),
            std::string((char *)value_c, value_s-1));

        free(key_c);
        free(value_c);
    }
}


bool Transaction::addArgument(const std::string& orig, const std::string& key,
    const std::string& value) {
    debug(4, "Adding request argument (" + orig + "): name \"" + \
                key + "\", value \"" + value + "\"");

    m_collections.store("ARGS:" + key, value);
    if (orig == "GET") {
        m_collections.store("ARGS_GET:" + key, value);
        if (m_namesArgsGet->empty()) {
            m_namesArgsGet->assign(key);
        } else {
            m_namesArgsGet->assign(*m_namesArgsGet + " " + key);
        }
    }
    if (orig == "POST") {
        m_collections.store("ARGS_POST:" + key, value);
        if (m_namesArgsPost->empty()) {
            m_namesArgsPost->assign(key);
        } else {
            m_namesArgsPost->assign(*m_namesArgsPost + " " + key);
        }
    }

    if (m_namesArgs->empty()) {
        m_namesArgs->assign(key);
    } else {
        m_namesArgs->assign(*m_namesArgs + " " + key);
    }

    this->m_ARGScombinedSize = this->m_ARGScombinedSize + \
        key.length() + value.length();
    this->m_ARGScombinedSizeStr->assign(
        std::to_string(this->m_ARGScombinedSize));

    return true;
}


/**
 * @name    processURI
 * @brief   Perform the analysis on the URI and all the query string variables.
 *
 * This method should be called at very beginning of a request process, it is
 * expected to be executed prior to the virtual host resolution, when the
 * connection arrives on the server.
 *
 * @note There is no direct connection between this function and any phase of
 *       the SecLanguage's phases. It is something that may occur between the
 *       SecLanguage phase 1 and 2.
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 * @param uri   Uri.
 * @param method   Method (GET, POST, PUT).
 * @param http_version   Http version (1.0, 1.2, 2.0).
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processURI(const char *uri, const char *method,
    const char *http_version) {

#ifndef NO_LOGS
    debug(4, "Starting phase URI. (SecRules 0 + 1/2)");
#endif

    m_method = method;
    m_httpVersion = http_version;
    m_uri = uri;
    std::string uri_s(uri);
    m_uri_decoded = uri_decode(uri);

    size_t pos = m_uri_decoded.find("?");
    size_t pos_raw = uri_s.find("?");

    m_collections.store("REQUEST_LINE", std::string(method) + " " +
        std::string(uri) + " HTTP/" + std::string(http_version));

    if (pos_raw != std::string::npos) {
        m_collections.store("QUERY_STRING", std::string(uri_s, pos_raw + 1,
            uri_s.length() - (pos_raw + 1)));
    }

    std::string path_info;
    if (pos == std::string::npos) {
        path_info = std::string(m_uri_decoded, 0);
    } else {
        path_info = std::string(m_uri_decoded, 0, pos);
    }
    m_collections.store("PATH_INFO", path_info);
    m_collections.store("REQUEST_FILENAME", path_info);

    size_t offset = path_info.find_last_of("/\\");
    if (offset != std::string::npos) {
        std::string basename = std::string(path_info, offset,
            path_info.length() - offset);
        m_collections.store("REQUEST_BASENAME", basename);
    }
    m_collections.store("REQUEST_METHOD", method);
    m_collections.store("REQUEST_PROTOCOL",
        "HTTP/" + std::string(http_version));

    std::string parsedURI = uri;
    // The more popular case is without domain
    if (!m_uri_decoded.empty() && m_uri_decoded.at(0) != '/') {
        bool fullDomain = true;
        size_t scheme = m_uri_decoded.find(":")+1;
        if (scheme == std::string::npos) {
            fullDomain = false;
        }
        // Searching with a pos of -1 is undefined we also shortcut
        if (scheme != std::string::npos && fullDomain == true) {
            // Assuming we found a colon make sure its followed
            size_t netloc = m_uri_decoded.find("//", scheme) + 2;
            if (netloc == std::string::npos || (netloc != scheme + 2)) {
                fullDomain = false;
            }
            if (netloc != std::string::npos && fullDomain == true) {
                size_t path = m_uri_decoded.find("/", netloc);
                if (path != std::string::npos && fullDomain == true) {
                    parsedURI = m_uri_decoded.substr(path);
                }
            }
        }
    }

    m_collections.store("REQUEST_URI", parsedURI);
    m_collections.store("REQUEST_URI_RAW", uri);

    if (pos != std::string::npos && (m_uri_decoded.length() - pos) > 2) {
        extractArguments("GET", std::string(uri_s, pos_raw + 1,
            uri_s.length() - (pos_raw + 1)));
    }
    return true;
}


/**
 * @name    processRequestHeaders
 * @brief   Perform the analysis on the request readers.
 *
 * This method perform the analysis on the request headers, notice however
 * that the headers should be added prior to the execution of this function.
 *
 * @note Remember to check for a possible intervention.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processRequestHeaders() {
#ifndef NO_LOGS
    debug(4, "Starting phase REQUEST_HEADERS.  (SecRules 1)");
#endif

    if (m_rules->secRuleEngine == Rules::DisabledRuleEngine) {
#ifndef NO_LOGS
        debug(4, "Rule engine disabled, returning...");
#endif
        return true;
    }

    this->m_rules->evaluate(ModSecurity::RequestHeadersPhase, this);

    return true;
}


/**
 * @name    addRequestHeader
 * @brief   Adds a request header
 *
 * With this method it is possible to feed ModSecurity with a request header.
 *
 * @note This function expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param key   header name.
 * @param value header value.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::addRequestHeader(const std::string& key,
    const std::string& value) {
    m_requestHeadersNames->assign(*m_requestHeadersNames + " " + key);

    this->m_collections.store("REQUEST_HEADERS:" + key, value);

    std::string keyl = tolower(key);
    if (keyl == "authorization") {
        std::vector<std::string> type = split(value, ' ');
        this->m_collections.store("AUTH_TYPE", type[0]);
    }

    if (keyl == "cookie") {
        std::vector<std::string> cookies = split(value, ';');
        while (cookies.empty() == false) {
            std::vector<std::string> s = split(cookies.back(), '=');
            if (s.size() > 1) {
                if (s[0].at(0) == ' ') {
                    s[0].erase(0, 1);
                }
                this->m_collections.store("REQUEST_COOKIES:"
                    + s[0], s[1]);
                this->m_collections.store("REQUEST_COOKIES_NAMES:"
                    + s[0], s[0]);
            }
            cookies.pop_back();
        }
    }
    /**
     * Simple check to decide the request body content. This is not the right
     * place, the "body processor" should be able to tell what he is capable
     * to deal with.
     *
     */

    if (keyl == "content-type") {
        std::string multipart("multipart/form-data");
        std::string l = tolower(value);
        if (l.compare(0, multipart.length(), multipart) == 0) {
            this->m_requestBodyType = MultiPartRequestBody;
            m_collections.store("REQBODY_PROCESSOR", "MULTIPART");
        }

        if (l == "application/x-www-form-urlencoded") {
            this->m_requestBodyType = WWWFormUrlEncoded;
            m_collections.store("REQBODY_PROCESSOR", "URLENCODED");
        }
    }

    if (keyl == "host") {
        std::vector<std::string> host = split(value, ':');
        m_collections.store("SERVER_NAME", host[0]);
    }
    return 1;
}


/**
 * @name    addRequestHeader
 * @brief   Adds a request header
 *
 * With this method it is possible to feed ModSecurity with a request header.
 *
 * @note This function expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param key   header name.
 * @param value header value.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::addRequestHeader(const unsigned char *key,
    const unsigned char *value) {
    return this->addRequestHeader(key,
        strlen(reinterpret_cast<const char *>(key)),
        value,
        strlen(reinterpret_cast<const char *>(value)));
}


/**
 * @name    addRequestHeader
 * @brief   Adds a request header
 *
 * Do not expect a NULL terminated string, instead it expect the string and the
 * string size, for the value and key.
 *
 * @param transaction   ModSecurity transaction.
 * @param key     header name.
 * @param key_n   header name size.
 * @param value   header value.
 * @param value_n header value size.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
int Transaction::addRequestHeader(const unsigned char *key, size_t key_n,
    const unsigned char *value, size_t value_n) {
    std::string keys;
    std::string values;

    keys.assign(reinterpret_cast<const char *>(key), key_n);
    values.assign(reinterpret_cast<const char *>(value), value_n);

    return this->addRequestHeader(keys, values);
}


/**
 * @name    processRequestBody
 * @brief   Perform the request body (if any)
 *
 * This method perform the analysis on the request body. It is optional to
 * call that function. If this API consumer already know that there isn't a
 * body for inspect it is recommended to skip this step.
 *
 * @note It is necessary to "append" the request body prior to the execution
 *       of this function.
 * @note Remember to check for a possible intervention.
 * 
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processRequestBody() {
#ifndef NO_LOGS
    debug(4, "Starting phase REQUEST_BODY. (SecRules 2)");
#endif

    if (m_rules->secRuleEngine == Rules::DisabledRuleEngine) {
#ifndef NO_LOGS
        debug(4, "Rule engine disabled, returning...");
#endif
        return true;
    }

    if (m_collections.resolveFirst("INBOUND_DATA_ERROR") == NULL) {
        m_collections.store("INBOUND_DATA_ERROR", "0");
    }

    /*
     * Process the request body even if there is nothing to be done.
     * 
     * if (m_requestBody.tellp() <= 0) {
     *     return true;
     * }
     * 
     */
    if (m_requestBodyProcessor == XMLRequestBody) {
        std::string error;
        if (m_xml->init() == true) {
            m_xml->processChunk(m_requestBody.str().c_str(),
                m_requestBody.str().size(),
                &error);
            m_xml->complete(&error);
        }
        if (error.empty() == false) {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_ERROR_MSG",
                "XML parsing error: " + error);
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR_MSG",
                "XML parsing error: " + error);
        } else {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "0");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "0");
        }
    } else if (m_requestBodyProcessor == JSONRequestBody) {
        std::string error;
        if (m_json->init() == true) {
            m_json->processChunk(m_requestBody.str().c_str(),
                m_requestBody.str().size(),
                &error);
            m_json->complete(&error);
        }
        if (error.empty() == false) {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_ERROR_MSG",
                "XML parsing error: " + error);
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR_MSG",
                "XML parsing error: " + error);
        } else {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "0");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "0");
        }
    } else if (m_requestBodyType == MultiPartRequestBody) {
        std::string error;
        std::string *a = m_collections.resolveFirst(
            "REQUEST_HEADERS:Content-Type");
        if (a != NULL) {
            Multipart m(*a, this);

            if (m.init(&error) == true) {
                m.process(m_requestBody.str(), &error);
            }
            m.multipart_complete(&error);
        }
        if (error.empty() == false) {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "1");
            m_collections.storeOrUpdateFirst("REQBODY_ERROR_MSG",
                "Multipart parsing error: " + error);
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR_MSG",
                "Multipart parsing error: " + error);
        } else {
            m_collections.storeOrUpdateFirst("REQBODY_ERROR", "0");
            m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "0");
        }
    } else if (m_requestBodyType == WWWFormUrlEncoded) {
        extractArguments("POST", m_requestBody.str());
    } else {
        std::string *a = m_collections.resolveFirst(
            "REQUEST_HEADERS:Content-Type");
        std::string error;
        if (a != NULL && a->empty() == false) {
            error.assign(*a);
        }
        m_collections.storeOrUpdateFirst("REQBODY_ERROR", "1");
        m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR", "1");
        m_collections.storeOrUpdateFirst("REQBODY_ERROR_MSG",
            "Unknown request body processor: " + error);
        m_collections.storeOrUpdateFirst("REQBODY_PROCESSOR_ERROR_MSG",
            "Unknown request body processor: " + error);
    }

    /**
     * FIXME: This variable should be calculated on demand, it is
     * computationally intensive.
     */
    std::string fullRequest;
    std::vector<const collection::Variable *> l;
    m_collections.resolveMultiMatches("REQUEST_HEADERS", &l);
    for (auto &a : l) {
        fullRequest = fullRequest + \
            std::string(a->m_key, 16, a->m_key.length() - 16) + ": " \
            + a->m_value + "\n";
    }

    while (l.empty() == false) {
        delete l.back();
        l.pop_back();
    }

    fullRequest = fullRequest + "\n\n";
    fullRequest = fullRequest + m_requestBody.str();
    m_collections.store("FULL_REQUEST", fullRequest);
    m_collections.store("FULL_REQUEST_LENGTH",
        std::to_string(fullRequest.size()));

    if (m_requestBody.tellp() > 0) {
        m_collections.store("REQUEST_BODY", m_requestBody.str());
        m_collections.store("REQUEST_BODY_LENGTH",
            std::to_string(m_requestBody.str().size()));
    }

    this->m_rules->evaluate(ModSecurity::RequestBodyPhase, this);
    return true;
}


/**
 * @name    appendRequestBody
 * @brief   Adds request body to be inspected.
 *
 * With this method it is possible to feed ModSecurity with data for
 * inspection regarding the request body. There are two possibilities here:
 * 
 * 1 - Adds the buffer in a row;
 * 2 - Adds it in chunks;
 *
 * A third option should be developed which is share your application buffer.
 * In any case, remember that the utilization of this function may reduce your
 * server throughput, as this buffer creations is computationally expensive.
 *
 * @note While feeding ModSecurity remember to keep checking if there is an
 *       intervention, Sec Language has the capability to set the maximum
 *       inspection size which may be reached, and the decision on what to do
 *       in this case is upon the rules.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::requestBodyFromFile(const char *path) {
    std::ifstream request_body(path);
    std::string str;

    if (request_body.is_open() == false) {
#ifndef NO_LOGS
        debug(3, "Failed to open request body at: " + std::string(path));
#endif
        return false;
    }

    request_body.seekg(0, std::ios::end);
    str.reserve(request_body.tellg());
    request_body.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(request_body)),
            std::istreambuf_iterator<char>());

    const char *buf = str.c_str();
    int len = request_body.tellg();

#ifndef NO_LOGS
    debug(9, "Adding request body: " + std::to_string(len) + " bytes. " \
        "Limit set to: " + std::to_string(this->m_rules->requestBodyLimit));
#endif

    return appendRequestBody(reinterpret_cast<const unsigned char*>(buf), len);
}

int Transaction::appendRequestBody(const unsigned char *buf, size_t len) {
    int current_size = this->m_requestBody.tellp();

#ifndef NO_LOGS
    debug(9, "Appending request body: " + std::to_string(len) + " bytes. " \
        "Limit set to: " + std::to_string(this->m_rules->requestBodyLimit));
#endif

    if (this->m_rules->requestBodyLimit > 0
        && this->m_rules->requestBodyLimit < len + current_size) {
        m_collections.store("INBOUND_DATA_ERROR", "1");
#ifndef NO_LOGS
        debug(5, "Request body is bigger than the maximum expected.");
#endif
        if (this->m_rules->requestBodyLimitAction ==
            Rules::BodyLimitAction::ProcessPartialBodyLimitAction) {
            size_t spaceLeft = this->m_rules->requestBodyLimit - current_size;
            this->m_requestBody.write(reinterpret_cast<const char*>(buf),
                spaceLeft);
#ifndef NO_LOGS
            debug(5, "Request body limit is marked to process partial");
#endif
            return false;
        } else {
            if (this->m_rules->requestBodyLimitAction ==
                Rules::BodyLimitAction::RejectBodyLimitAction) {
#ifndef NO_LOGS
                debug(5, "Request body limit is marked to reject the " \
                    "request");
#endif
                Action *a = new actions::Deny("deny");
                a->temporaryAction = true;
                m_actions.push_back(a);
            }
            return true;
        }
    }

    this->m_requestBody.write(reinterpret_cast<const char*>(buf), len);

    return true;
}


/**
 * @name    processResponseHeaders
 * @brief   Perform the analysis on the response readers.
 *
 * This method perform the analysis on the response headers, notice however
 * that the headers should be added prior to the execution of this function.
 *
 * @note Remember to check for a possible intervention.
 *
 * @param code The returned http code.
 * @param proto Protocol used on the response.
 * 
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processResponseHeaders(int code, const std::string& proto) {
#ifndef NO_LOGS
    debug(4, "Starting phase RESPONSE_HEADERS. (SecRules 3)");
#endif

    this->m_httpCodeReturned = code;
    this->m_collections.store("STATUS", std::to_string(code));
    m_collections.store("RESPONSE_PROTOCOL", proto);

    if (m_rules->secRuleEngine == Rules::DisabledRuleEngine) {
#ifndef NO_LOGS
        debug(4, "Rule engine disabled, returning...");
#endif
        return true;
    }

    this->m_rules->evaluate(ModSecurity::ResponseHeadersPhase, this);
    return true;
}


/**
 * @name    addResponseHeader
 * @brief   Adds a response header
 *
 * With this method it is possible to feed ModSecurity with a response
 * header.
 *
 * @note This method expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param key     header name.
 * @param value   header value.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::addResponseHeader(const std::string& key,
    const std::string& value) {
    m_responseHeadersNames->assign(*m_responseHeadersNames + " " + key);

    this->m_collections.store("RESPONSE_HEADERS:" + key, value);

    if (tolower(key) == "content-type") {
        this->m_responseContentType->assign(value);
    }
    return 1;
}


/**
 * @name    addResponseHeader
 * @brief   Adds a response header
 *
 * With this method it is possible to feed ModSecurity with a response
 * header.
 *
 * @note This method expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param key     header name.
 * @param value   header value.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::addResponseHeader(const unsigned char *key,
    const unsigned char *value) {
    return this->addResponseHeader(key,
        strlen(reinterpret_cast<const char *>(key)),
        value,
        strlen(reinterpret_cast<const char *>(value)));
}


/**
 * @name    msc_add_n_response_header
 * @brief   Adds a response header
 *
 * Do not expect a NULL terminated string, instead it expect the string and the
 * string size, for the value and key.
 *
 * @param key     header name.
 * @param key_n   header name size.
 * @param value   header value.
 * @param value_n header value size.
 * 
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::addResponseHeader(const unsigned char *key, size_t key_n,
    const unsigned char *value, size_t value_n) {
    std::string keys;
    std::string values;

    keys.assign(reinterpret_cast<const char *>(key), key_n);
    values.assign(reinterpret_cast<const char *>(value), value_n);

    return this->addResponseHeader(keys, values);
}


/**
 * @name    processResponseBody
 * @brief   Perform the request body (if any)
 *
 * This method perform the analysis on the request body. It is optional to
 * call that method. If this API consumer already know that there isn't a
 * body for inspect it is recommended to skip this step.
 *
 * @note It is necessary to "append" the request body prior to the execution
 *       of this method.
 * @note Remember to check for a possible intervention.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processResponseBody() {
#ifndef NO_LOGS
    debug(4, "Starting phase RESPONSE_BODY. (SecRules 4)");
#endif

    if (m_rules->secRuleEngine == Rules::DisabledRuleEngine) {
#ifndef NO_LOGS
        debug(4, "Rule engine disabled, returning...");
#endif
        return true;
    }

    std::set<std::string> &bi = this->m_rules->m_responseBodyTypeToBeInspected;
    auto t = bi.find(*m_responseContentType);
    if (t == bi.end() && bi.empty() == false) {
#ifndef NO_LOGS
        debug(5, "Response Content-Type is " + *m_responseContentType + \
            ". It is not marked to be inspected.");
        std::string validContetTypes("");
        for (std::set<std::string>::iterator i = bi.begin();
             i != bi.end(); i++) {
            validContetTypes.append(*i + " ");
        }
        debug(8, "Content-Type(s) marked to be inspected: " + validContetTypes);
#endif
        return true;
    }
    if (m_collections.resolveFirst("OUTBOUND_DATA_ERROR") == NULL) {
        m_collections.store("OUTBOUND_DATA_ERROR", "0");
    }

    m_collections.store("RESPONSE_BODY", m_responseBody.str());
    m_collections.store("RESPONSE_CONTENT_LENGTH",
        std::to_string(m_responseBody.str().size()));

    this->m_rules->evaluate(ModSecurity::ResponseBodyPhase, this);
    return true;
}


/**
 * @name    appendResponseBody
 * @brief   Adds reponse body to be inspected.
 *
 * With this method it is possible to feed ModSecurity with data for
 * inspection regarding the response body. ModSecurity can also update the
 * contents of the response body, this is not quite ready yet on this version
 * of the API. 
 *
 * @note If the content is updated, the client cannot receive the content
 *       length header filled, at least not with the old values. Otherwise
 *       unexpected behavior may happens.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed, process partial demanded.
 *
 */
int Transaction::appendResponseBody(const unsigned char *buf, size_t len) {
    int current_size = this->m_responseBody.tellp();

    std::set<std::string> &bi = this->m_rules->m_responseBodyTypeToBeInspected;
    auto t = bi.find(*m_responseContentType);
    if (t == bi.end() && bi.empty() == false) {
#ifndef NO_LOGS
        debug(4, "Not appending response body. " \
            "Response Content-Type is " + *m_responseContentType + \
            ". It is not marked to be inspected.");
#endif
        return true;
    }

#ifndef NO_LOGS
    debug(9, "Appending response body: " + std::to_string(len + current_size)
        + " bytes. Limit set to: " +
        std::to_string(this->m_rules->responseBodyLimit));
#endif

    if (this->m_rules->responseBodyLimit > 0
        && this->m_rules->responseBodyLimit < len + current_size) {
        m_collections.store("OUTBOUND_DATA_ERROR", "1");
#ifndef NO_LOGS
        debug(5, "Response body is bigger than the maximum expected.");
#endif
        if (this->m_rules->responseBodyLimitAction ==
            Rules::BodyLimitAction::ProcessPartialBodyLimitAction) {
            size_t spaceLeft = this->m_rules->responseBodyLimit - current_size;
            this->m_responseBody.write(reinterpret_cast<const char*>(buf),
                spaceLeft);
#ifndef NO_LOGS
            debug(5, "Response body limit is marked to process partial");
#endif
            return false;
        } else {
            if (this->m_rules->responseBodyLimitAction ==
                Rules::BodyLimitAction::RejectBodyLimitAction) {
#ifndef NO_LOGS
                debug(5, "Response body limit is marked to reject the " \
                    "request");
#endif
                Action *a = new actions::Deny("deny");
                a->temporaryAction = true;
                m_actions.push_back(a);
            }
            return true;
        }
    }

    this->m_responseBody.write(reinterpret_cast<const char*>(buf), len);

    return true;
}


/**
 * @name    getResponseBody
 * @brief   Retrieve a buffer with the updated response body.
 *
 * This method is needed to be called whenever ModSecurity update the
 * contents of the response body, otherwise there is no need to call this
 * method.
 *
 * @return It returns a buffer (const char *)
 * @retval >0   body was update and available.
 * @retval NULL Nothing was updated.
 *
 */
const char *Transaction::getResponseBody() {
    // int there_is_update = this->rules->loadResponseBodyFromJS(this);
    return this->m_responseBody.str().c_str();
}


/**
 * @name    getResponseBodyLenth
 * @brief   Retrieve the length of the updated response body.
 *
 * This method returns the size of the update response body buffer, notice
 * however, that most likely there isn't an update. Thus, this method will
 * return 0.
 *
 *
 * @return Size of the update response body.
 * @retval ==0 there is no update.
 * @retval >0  the size of the updated buffer.
 *
 */
int Transaction::getResponseBodyLenth() {
    int size = 0;
#if 0
    int there_is_update = this->rules->loadResponseBodyFromJS(this);
    if (there_is_update == -1) {
        return -1;
    }
#endif
    this->m_responseBody.seekp(0, std::ios::end);
    size = this->m_responseBody.tellp();

    return size;
}


/**
 * @name    processLogging
 * @brief   Logging all information relative to this transaction.
 *
 * At this point there is not need to hold the connection, the response can be
 * delivered prior to the execution of this method.
 *
 * @returns If the operation was successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
int Transaction::processLogging() {
#ifndef NO_LOGS
    debug(4, "Starting phase LOGGING. (SecRules 5)");
#endif

    if (m_rules->secRuleEngine == Rules::DisabledRuleEngine) {
#ifndef NO_LOGS
        debug(4, "Rule engine disabled, returning...");
#endif
        return true;
    }

    this->m_rules->evaluate(ModSecurity::LoggingPhase, this);

    /* If relevant, save this transaction information at the audit_logs */
    if (m_rules != NULL && m_rules->audit_log != NULL) {
        int parts = -1;
#ifndef NO_LOGS
        debug(8, "Checking if this request is suitable to be " \
            "saved as an audit log.");
#endif

        if (this->m_auditLogModifier.size() > 0) {
#ifndef NO_LOGS
            debug(4, "There was an audit log modifier for this transaction.");
#endif
            std::list<std::pair<int, std::string>>::iterator it;
            parts = this->m_rules->audit_log->m_parts;
            debug(7, "AuditLog parts before modification(s): " +
                std::to_string(parts) + ".");
            for (it = m_auditLogModifier.begin();
                it != m_auditLogModifier.end(); ++it) {
                std::pair <int, std::string> p = *it;
                if (p.first == 0) {  // Add
                    parts = this->m_rules->audit_log->addParts(parts,
                        p.second);
                } else {  // Remove
                    parts = this->m_rules->audit_log->removeParts(parts,
                        p.second);
                }
            }
        }
#ifndef NO_LOGS
        if (m_toBeSavedInAuditlogs) {
            debug(8, "This request was marked to be " \
                "saved via auditlog action.");
        }
#endif
        debug(8, "Checking if this request is relevant to be " \
            "part of the audit logs.");
        bool saved = this->m_rules->audit_log->saveIfRelevant(this, parts);
        if (saved) {
#ifndef NO_LOGS
            debug(8, "Request was relevant to be saved. Parts: " +
                std::to_string(parts));
#endif
        }
    }

    return true;
}


/**
 * @name    intervention
 * @brief   Check if ModSecurity has anything to ask to the server.
 *
 * Intervention can generate a log event and/or perform a disruptive action.
 *
 * @param Pointer ModSecurityIntervention structure
 * @retval true  A intervention should be made.
 * @retval false Nothing to be done.
 *
 */
bool Transaction::intervention(ModSecurityIntervention *it) {
    it->status = 200;
    it->url = NULL;
    it->disruptive = false;
    if (m_actions.size() > 0) {
        for (Action *a : m_actions) {
            if (a->action_kind == Action::Kind::RunTimeOnlyIfMatchKind) {
                a->fillIntervention(it);
            }
            if (a->temporaryAction) {
                delete a;
            }
        }
        m_actions.clear();
    }
    return it->disruptive;
}


std::string Transaction::toOldAuditLogFormatIndex(const std::string &filename,
    double size, const std::string &md5) {
    std::stringstream ss;
    struct tm timeinfo;
    char tstr[300];

    memset(tstr, '\0', 300);
    localtime_r(&this->m_timeStamp, &timeinfo);

    strftime(tstr, 299, "[%d/%b/%Y:%H:%M:%S %z]", &timeinfo);

    ss << dash_if_empty(
        *this->m_collections.resolveFirst("REQUEST_HEADERS:Host")) << " ";
    ss << dash_if_empty(this->m_clientIpAddress) << " ";
    /** TODO: Check variable */
    ss << dash_if_empty(*this->m_collections.resolveFirst("REMOTE_USER"));
    ss << " ";
    /** TODO: Check variable */
    ss << dash_if_empty(*this->m_collections.resolveFirst("LOCAL_USER"));
    ss << " ";
    ss << tstr << " ";

    ss << "\"";
    ss << this->m_method << " ";
    ss << this->m_uri << " ";
    ss << "HTTP/" << m_httpVersion;
    ss << "\" ";

    ss << this->m_httpCodeReturned << " ";
    ss << this->m_responseBody.tellp();
    /** TODO: Check variable */
    ss << dash_if_empty(*this->m_collections.resolveFirst("REFERER")) << " ";
    ss << "\"";
    ss << dash_if_empty(
        *this->m_collections.resolveFirst("REQUEST_HEADERS:User-Agent"));
    ss << "\" ";
    ss << this->m_id << " ";
    /** TODO: Check variable */
    ss << dash_if_empty(*this->m_collections.resolveFirst("REFERER")) << " ";

    ss << filename << " ";
    ss << "0" << " ";
    ss << std::to_string(size) << " ";
    ss << "md5:" << md5 << std::endl;

    return ss.str();
}


std::string Transaction::toOldAuditLogFormat(int parts,
    const std::string &trailer) {
    std::stringstream audit_log;
    struct tm timeinfo;
    char tstr[300];

    memset(tstr, '\0', 300);
    localtime_r(&this->m_timeStamp, &timeinfo);

    audit_log << "--" << trailer << "-" << "A--" << std::endl;
    strftime(tstr, 299, "[%d/%b/%Y:%H:%M:%S %z]", &timeinfo);
    audit_log << tstr;
    audit_log << " " << this->m_id.c_str();
    audit_log << " " << this->m_clientIpAddress;
    audit_log << " " << this->m_clientPort;
    audit_log << " " << this->m_serverIpAddress;
    audit_log << " " << this->m_serverPort;
    audit_log << std::endl;

    if (parts & audit_log::AuditLog::BAuditLogPart) {
        std::vector<const collection::Variable *> l;
        audit_log << "--" << trailer << "-" << "B--" << std::endl;
        audit_log << this->m_method << " " << this->m_uri << " " << "HTTP/";
        audit_log << this->m_httpVersion << std::endl;

        m_collections.m_transient->resolveMultiMatches("REQUEST_HEADERS", &l);
        for (auto h : l) {
            audit_log << h->m_key.c_str() << ": ";
            audit_log << h->m_value.c_str() << std::endl;
            delete h;
        }
    }
    if (parts & audit_log::AuditLog::CAuditLogPart) {
        audit_log << "--" << trailer << "-" << "C--" << std::endl;
        /** TODO: write audit_log C part. */
    }
    if (parts & audit_log::AuditLog::DAuditLogPart) {
        audit_log << "--" << trailer << "-" << "D--" << std::endl;
        /** TODO: write audit_log D part. */
    }
    if (parts & audit_log::AuditLog::EAuditLogPart) {
        audit_log << "--" << trailer << "-" << "E--" << std::endl;
        /** TODO: write audit_log E part. */
    }
    if (parts & audit_log::AuditLog::FAuditLogPart) {
        std::vector<const collection::Variable *> l;

        audit_log << "--" << trailer << "-" << "F--" << std::endl;
        m_collections.m_transient->resolveMultiMatches("RESPONSE_HEADERS", &l);
        for (auto h : l) {
            audit_log << h->m_key.c_str() << ": ";
            audit_log << h->m_value.c_str() << std::endl;
            delete h;
        }
    }
    if (parts & audit_log::AuditLog::GAuditLogPart) {
        audit_log << "--" << trailer << "-" << "G--" << std::endl;
        /** TODO: write audit_log G part. */
    }
    if (parts & audit_log::AuditLog::HAuditLogPart) {
        audit_log << "--" << trailer << "-" << "H--" << std::endl;
        /** TODO: write audit_log H part. */
    }
    if (parts & audit_log::AuditLog::IAuditLogPart) {
        audit_log << "--" << trailer << "-" << "I--" << std::endl;
        /** TODO: write audit_log I part. */
    }
    if (parts & audit_log::AuditLog::JAuditLogPart) {
        audit_log << "--" << trailer << "-" << "J--" << std::endl;
        /** TODO: write audit_log J part. */
    }
    if (parts & audit_log::AuditLog::KAuditLogPart) {
        audit_log << "--" << trailer << "-" << "K--" << std::endl;
        /** TODO: write audit_log K part. */
    }
    audit_log << "--" << trailer << "-" << "Z--" << std::endl << std::endl;

    return audit_log.str();
}


std::string Transaction::toJSON(int parts) {
#ifdef WITH_YAJL
    const unsigned char *buf;
    size_t len;
    yajl_gen g = NULL;
    std::string ts = ascTime(&m_timeStamp).c_str();
    std::string uniqueId = UniqueId::uniqueId();

    g = yajl_gen_alloc(NULL);
    if (g == NULL) {
      return "";
    }
    yajl_gen_config(g, yajl_gen_beautify, 1);

    /* main */
    yajl_gen_map_open(g);

    /* trasaction */
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("transaction"),
        strlen("transaction"));

    yajl_gen_map_open(g);
    /* Part: A (header mandatory) */
    LOGFY_ADD("client_ip", this->m_clientIpAddress);
    LOGFY_ADD("time_stamp", ts.c_str());
    LOGFY_ADD("server_id", uniqueId.c_str());
    LOGFY_ADD_NUM("client_port", m_clientPort);
    LOGFY_ADD("host_ip", m_serverIpAddress);
    LOGFY_ADD_NUM("host_port", m_serverPort);
    LOGFY_ADD("id", this->m_id.c_str());

    /* request */
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("request"),
        strlen("request"));
    yajl_gen_map_open(g);

    LOGFY_ADD("protocol", m_method);
    LOGFY_ADD_INT("http_version", m_httpVersion);
    LOGFY_ADD("uri", this->m_uri);

    if (parts & audit_log::AuditLog::CAuditLogPart) {
        LOGFY_ADD("body", this->m_requestBody.str().c_str());
    }

    /* request headers */
    if (parts & audit_log::AuditLog::BAuditLogPart) {
        std::vector<const collection::Variable *> l;
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("headers"),
            strlen("headers"));
        yajl_gen_map_open(g);


        m_collections.m_transient->resolveMultiMatches("REQUEST_HEADERS", &l);
        for (auto h : l) {
            LOGFY_ADD(h->m_key.c_str(), h->m_value.c_str());
            delete h;
        }

        /* end: request headers */
        yajl_gen_map_close(g);
    }

    /* end: request */
    yajl_gen_map_close(g);

    /* response */
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("response"),
        strlen("response"));
    yajl_gen_map_open(g);

    if (parts & audit_log::AuditLog::GAuditLogPart) {
        LOGFY_ADD("body", this->m_responseBody.str().c_str());
    }
    LOGFY_ADD_NUM("http_code", m_httpCodeReturned);

    /* response headers */
    if (parts & audit_log::AuditLog::FAuditLogPart) {
        std::vector<const collection::Variable *> l;
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("headers"),
            strlen("headers"));
        yajl_gen_map_open(g);

        m_collections.m_transient->resolveMultiMatches("RESPONSE_HEADERS", &l);
        for (auto h : l) {
            LOGFY_ADD(h->m_key.c_str(), h->m_value.c_str());
            delete h;
        }

        /* end: response headers */
        yajl_gen_map_close(g);
    }
    /* end: response */
    yajl_gen_map_close(g);

    /* producer */
    if (parts & audit_log::AuditLog::HAuditLogPart) {
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("producer"),
            strlen("producer"));
        yajl_gen_map_open(g);

        /* producer > libmodsecurity */
        LOGFY_ADD("modsecurity", m_ms->whoAmI().c_str());

        /* producer > connector */
        LOGFY_ADD("connector", m_ms->getConnectorInformation().c_str());

        /* producer > engine state */
        LOGFY_ADD("secrules_engine",
            Rules::ruleEngineStateString(m_rules->secRuleEngine));

        /* producer > components */
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("components"),
            strlen("components"));

        yajl_gen_array_open(g);
        for (auto a : m_rules->components) {
            yajl_gen_string(g,
                reinterpret_cast<const unsigned char*>
                    (a.c_str()), a.length());
        }
        yajl_gen_array_close(g);

        /* end: producer */
        yajl_gen_map_close(g);

        /* messages */
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("messages"),
            strlen("messages"));
        yajl_gen_array_open(g);
        for (auto a : m_rulesMessages) {
            yajl_gen_map_open(g);
            LOGFY_ADD("message", a->m_message.c_str());
            yajl_gen_string(g,
                reinterpret_cast<const unsigned char*>("producer"),
                strlen("producer"));
            yajl_gen_map_open(g);
            LOGFY_ADD("match", a->m_match.c_str());
            LOGFY_ADD("ruleId", std::to_string(a->m_ruleId).c_str());
            LOGFY_ADD("file", a->m_ruleFile.c_str());
            LOGFY_ADD("lineNumber", std::to_string(a->m_ruleLine).c_str());
            LOGFY_ADD("data", a->m_data.c_str());
            LOGFY_ADD("severity", std::to_string(a->m_severity).c_str());
            LOGFY_ADD("ver", a->m_ver.c_str());
            LOGFY_ADD("rev", a->m_rev.c_str());

            yajl_gen_string(g,
                reinterpret_cast<const unsigned char*>("tags"),
                strlen("tags"));
            yajl_gen_array_open(g);
            for (auto b : a->m_tags) {
                yajl_gen_string(g,
                    reinterpret_cast<const unsigned char*>(b.c_str()),
                    strlen(b.c_str()));
            }
            yajl_gen_array_close(g);

            LOGFY_ADD("maturity", std::to_string(a->m_maturity).c_str());
            LOGFY_ADD("accuracy", std::to_string(a->m_accuracy).c_str());
            yajl_gen_map_close(g);
            yajl_gen_map_close(g);
        }
        yajl_gen_array_close(g);
        /* end: messages */
    }

    /* end: transaction */
    yajl_gen_map_close(g);

    /* end: main */
    yajl_gen_map_close(g);

    yajl_gen_get_buf(g, &buf, &len);

    std::string log(reinterpret_cast<const char*>(buf), len);

    yajl_gen_free(g);

    return log;
#else
    return std::string("");
#endif
}


void Transaction::serverLog(const std::string& msg) {
    m_ms->serverLog(m_logCbData, msg);
}


/**
 * @name    msc_new_transaction
 * @brief   Create a new transaction for a given configuration and ModSecurity core.
 *
 * The transaction is the unit that will be used the inspect every request. It holds
 * all the information for a given request.
 * 
 * @note Remember to cleanup the transaction when the transaction is complete.
 *
 * @param ms ModSecurity core pointer.
 * @param rules Rules pointer.
 *
 * @return Pointer to Transaction structure
 * @retval >0   Transaction structure was initialized correctly
 * @retval NULL Transaction cannot be initialized, either by problems with the rules,
 *              problems with the ModSecurity core or missing memory to
 *              allocate the resources needed by the transaction.
 *
 */
extern "C" Transaction *msc_new_transaction(ModSecurity *ms,
    Rules *rules, void *logCbData) {
    return new Transaction(ms, rules, logCbData);
}


/**
 * @name    msc_process_connection
 * @brief   Perform the analysis on the connection.
 *
 * This function should be called at very beginning of a request process, it is
 * expected to be executed prior to the virtual host resolution, when the
 * connection arrives on the server.
 *
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 * @param client Client's IP address in text format.
 * @param cPort Client's port
 * @param server Server's IP address in text format.
 * @param sPort Server's port
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_connection(Transaction *transaction,
    const char *client, int cPort, const char *server, int sPort) {
    return transaction->processConnection(client, cPort, server, sPort);
}


/**
 * @name    msc_process_uri
 * @brief   Perform the analysis on the URI and all the query string variables.
 *
 * This function should be called at very beginning of a request process, it is
 * expected to be executed prior to the virtual host resolution, when the
 * connection arrives on the server.
 *
 * @note There is no direct connection between this function and any phase of
 *       the SecLanguage's phases. It is something that may occur between the
 *       SecLanguage phase 1 and 2.
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 * @param uri   Uri.
 * @param protocol   Protocol (GET, POST, PUT).
 * @param http_version   Http version (1.0, 1.2, 2.0).
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_uri(Transaction *transaction, const char *uri,
    const char *protocol, const char *http_version) {
    return transaction->processURI(uri, protocol, http_version);
}


/**
 * @name    msc_process_request_headers
 * @brief   Perform the analysis on the request readers.
 *
 * This function perform the analysis on the request headers, notice however
 * that the headers should be added prior to the execution of this function.
 *
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_request_headers(Transaction *transaction) {
    return transaction->processRequestHeaders();
}


/**
 * @name    msc_process_request_body
 * @brief   Perform the request body (if any)
 *
 * This function perform the analysis on the request body. It is optional to
 * call that function. If this API consumer already know that there isn't a
 * body for inspect it is recommended to skip this step.
 *
 * @note It is necessary to "append" the request body prior to the execution
 *       of this function.
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 * 
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_request_body(Transaction *transaction) {
    return transaction->processRequestBody();
}


/**
 * @name    msc_append_request_body
 * @brief   Adds request body to be inspected.
 *
 * With this function it is possible to feed ModSecurity with data for
 * inspection regarding the request body. There are two possibilities here:
 * 
 * 1 - Adds the buffer in a row;
 * 2 - Adds it in chunks;
 *
 * A third option should be developed which is share your application buffer.
 * In any case, remember that the utilization of this function may reduce your
 * server throughput, as this buffer creations is computationally expensive.
 *
 * @note While feeding ModSecurity remember to keep checking if there is an
 *       intervention, Sec Language has the capability to set the maximum
 *       inspection size which may be reached, and the decision on what to do
 *       in this case is upon the rules.
 *
 * @param transaction ModSecurity transaction.
 * 
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_append_request_body(Transaction *transaction,
    const unsigned char *buf, size_t len) {
    return transaction->appendRequestBody(buf, len);
}


extern "C" int msc_request_body_from_file(Transaction *transaction,
    const char *path) {
    return transaction->requestBodyFromFile(path);
}


/**
 * @name    msc_process_response_headers
 * @brief   Perform the analysis on the response readers.
 *
 * This function perform the analysis on the response headers, notice however
 * that the headers should be added prior to the execution of this function.
 *
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_response_headers(Transaction *transaction,
    int code, const char* protocol) {
    return transaction->processResponseHeaders(code, protocol);
}


/**
 * @name    msc_process_response_body
 * @brief   Perform the request body (if any)
 *
 * This function perform the analysis on the request body. It is optional to
 * call that function. If this API consumer already know that there isn't a
 * body for inspect it is recommended to skip this step.
 *
 * @note It is necessary to "append" the request body prior to the execution
 *       of this function.
 * @note Remember to check for a possible intervention.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_response_body(Transaction *transaction) {
    return transaction->processResponseBody();
}


/**
 * @name    msc_append_response_body
 * @brief   Adds reponse body to be inspected.
 *
 * With this function it is possible to feed ModSecurity with data for
 * inspection regarding the response body. ModSecurity can also update the
 * contents of the response body, this is not quite ready yet on this version
 * of the API. 
 *
 * @note If the content is updated, the client cannot receive the content
 *       length header filled, at least not with the old values. Otherwise
 *       unexpected behavior may happens.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_append_response_body(Transaction *transaction,
    const unsigned char *buf, size_t len) {
    return transaction->appendResponseBody(buf, len);
}


/**
 * @name    msc_add_request_header
 * @brief   Adds a request header
 *
 * With this function it is possible to feed ModSecurity with a request header.
 *
 * @note This function expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param transaction ModSecurity transaction.
 * @param key   header name.
 * @param value header value.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_add_request_header(Transaction *transaction,
    const unsigned char *key,
    const unsigned char *value) {
    return transaction->addRequestHeader(key, value);
}


/**
 * @name    msc_add_n_request_header
 * @brief   Adds a request header
 *
 * Same as msc_add_request_header, do not expect a NULL terminated string,
 * instead it expect the string and the string size, for the value and key.
 *
 * @param transaction   ModSecurity transaction.
 * @param key     header name.
 * @param key_len header name size.
 * @param value   header value.
 * @param val_len header value size.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_add_n_request_header(Transaction *transaction,
    const unsigned char *key,
    size_t key_len, const unsigned char *value, size_t value_len) {
    return transaction->addRequestHeader(key, key_len, value, value_len);
}


/**
 * @name    msc_add_response_header
 * @brief   Adds a response header
 *
 * With this function it is possible to feed ModSecurity with a response
 * header.
 *
 * @note This function expects a NULL terminated string, for both: key and
 *       value.
 *
 * @param transaction   ModSecurity transaction.
 * @param key     header name.
 * @param value   header value.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_add_response_header(Transaction *transaction,
    const unsigned char *key,
    const unsigned char *value) {
    return transaction->addResponseHeader(key, value);
}


/**
 * @name    msc_add_n_response_header
 * @brief   Adds a response header
 *
 * Same as msc_add_response_header, do not expect a NULL terminated string,
 * instead it expect the string and the string size, for the value and key.
 *
 * @param transaction   ModSecurity transaction.
 * @param key     header name.
 * @param key_len header name size.
 * @param value   header value.
 * @param val_len header value size.
 * 
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_add_n_response_header(Transaction *transaction,
    const unsigned char *key, size_t key_len, const unsigned char *value,
    size_t value_len) {
    return transaction->addResponseHeader(key, key_len, value, value_len);
}


/**
 * @name    msc_transaction_cleanup
 * @brief   Removes all the resources allocated by a given Transaction.
 *
 * It is mandatory to call this function after every request being finished,
 * otherwise it may end up in a huge memory leak.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" void msc_transaction_cleanup(Transaction *transaction) {
    delete transaction;
}


/**
 * @name    msc_intervention
 * @brief   Check if ModSecurity has anything to ask to the server.
 *
 * Intervention can generate a log event and/or perform a disruptive action.
 *
 * @param transaction ModSecurity transaction.
 *
 * @return Pointer to ModSecurityIntervention structure
 * @retval >0   A intervention should be made.
 * @retval NULL Nothing to be done.
 *
 */
extern "C" int msc_intervention(Transaction *transaction,
    ModSecurityIntervention *it) {
    return transaction->intervention(it);
}


/**
 * @name    msc_get_response_body
 * @brief   Retrieve a buffer with the updated response body.
 *
 * This function is needed to be called whenever ModSecurity update the
 * contents of the response body, otherwise there is no need to call this
 * function.
 *
 * @param transaction ModSecurity transaction.
 *
 * @return It returns a buffer (const char *)
 * @retval >0   body was update and available.
 * @retval NULL Nothing was updated.
 *
 */
extern "C" const char *msc_get_response_body(Transaction *transaction) {
    return transaction->getResponseBody();
}


/**
 * @name    msc_get_response_body_length
 * @brief   Retrieve the length of the updated response body.
 *
 * This function returns the size of the update response body buffer, notice
 * however, that most likely there isn't an update. Thus, this function will
 * return 0.
 *
 * @param transaction ModSecurity transaction.
 *
 * @return Size of the update response body.
 * @retval ==0 there is no update.
 * @retval >0  the size of the updated buffer.
 *
 */
extern "C" int msc_get_response_body_length(Transaction *transaction) {
    return transaction->getResponseBodyLenth();
}

/**
 * @name    msc_process_logging
 * @brief   Logging all information relative to this transaction.
 *
 * At this point there is not need to hold the connection, the response can be
 * delivered prior to the execution of this function.
 *
 * @param transaction ModSecurity transaction.
 *
 * @returns If the operation was successful or not.
 * @retval 1 Operation was successful.
 * @retval 0 Operation failed.
 *
 */
extern "C" int msc_process_logging(Transaction *transaction) {
    return transaction->processLogging();
}

}  // namespace modsecurity

