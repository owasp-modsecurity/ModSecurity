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

#include "request_body_processor/json.h"

#include <list>
#include <iostream>
#include <string>


namespace modsecurity {
namespace RequestBodyProcessor {

/**
 * yajl callback functions
 * For more information on the function signatures and order, check
 * http://lloyd.github.com/yajl/yajl-1.0.12/structyajl__callbacks.html
 */

/**
 * Callback for hash key values; we use those to define the variable names
 * under ARGS. Whenever we reach a new key, we update the current key value.
 */
int JSON::yajl_map_key(void *ctx, const unsigned char *key, size_t length) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);
    std::string safe_key;

    /**
     * yajl does not provide us with null-terminated strings, but
     * rather expects us to copy the data from the key up to the
     * length informed; we create a standalone null-termined copy
     * in safe_key
     */
    safe_key.assign((const char *)key, length);

    tthis->debug(9, "New JSON hash key '" + safe_key + "'");

    /**
     * TODO: How do we free the previously string value stored here?
     */
    tthis->m_data.current_key = safe_key;

    return 1;
}

/**
 * Callback for null values
 *
 */
int JSON::yajl_null(void *ctx) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);

    return tthis->addArgument("");
}

/**
 * Callback for boolean values
 */
int JSON::yajl_boolean(void *ctx, int value) {
    JSON *tthis =  reinterpret_cast<JSON *>(ctx);

    if (value) {
        return tthis->addArgument("true");
    }

    return tthis->addArgument("false");
}

/**
 * Callback for string values
 */
int JSON::yajl_string(void *ctx, const unsigned char *value, size_t length) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);
    std::string v = std::string((const char*)value, length);

    return tthis->addArgument(v);
}

/**
 * Callback for numbers; YAJL can use separate callbacks for integers/longs and
 * float/double values, but since we are not interested in using the numeric
 * values here, we use a generic handler which uses numeric strings
 */
int JSON::yajl_number(void *ctx, const char *value, size_t length) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);
    std::string v = std::string((const char*)value, length);

    return tthis->addArgument(v);
}

/**
 * Callback for a new hash, which indicates a new subtree, labeled as the
 * current argument name, is being created
 */
int JSON::yajl_start_map(void *ctx) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);

    /**
     * If we do not have a current_key, this is a top-level hash, so we do not
     * need to do anything
     */
    if (tthis->m_data.current_key.empty() == true) {
        return true;
    }

    /**
     * Check if we are already inside a hash context, and append or create the
     * current key name accordingly
     */
    if (tthis->m_data.prefix.empty() == false) {
        tthis->m_data.prefix.append("." + tthis->m_data.current_key);
    } else {
        tthis->m_data.prefix.assign(tthis->m_data.current_key);
    }

    tthis->debug(9, "New JSON hash context (prefix '" + \
        tthis->m_data.prefix + "')");

    return 1;
}

/**
 * Callback for end hash, meaning the current subtree is being closed, and that
 * we should go back to the parent variable label
 */
int JSON::yajl_end_map(void *ctx) {
    JSON *tthis = reinterpret_cast<JSON *>(ctx);
    size_t sep_pos = std::string::npos;

    /**
     * If we have no prefix, then this is the end of a top-level hash and
     * we don't do anything
     */
    if (tthis->m_data.prefix.empty() == true) {
        return true;
    }

    /**
     * Current prefix might or not include a separator character; top-level
     * hash keys do not have separators in the variable name
     */
    sep_pos = tthis->m_data.prefix.find(".");

    if (sep_pos != std::string::npos) {
        std::string tmp = tthis->m_data.prefix;
        tthis->m_data.prefix.assign(tmp, 0, sep_pos);
        tthis->m_data.current_key.assign(tmp, sep_pos + 1,
            tmp.length() - sep_pos - 1);
    } else {
        tthis->m_data.current_key.assign(tthis->m_data.prefix);
        tthis->m_data.prefix = "";
    }

    return 1;
}


int JSON::addArgument(const std::string& value) {
    /**
     * If we do not have a prefix, we cannot create a variable name
     * to reference this argument; for now we simply ignore these
     */
    if (m_data.current_key.empty()) {
        debug(3, "Cannot add scalar value without an associated key");
        return 1;
    }

    if (m_data.prefix.empty()) {
        m_transaction->addArgument("JSON", m_data.current_key, value);
    } else {
        m_transaction->addArgument("JSON", m_data.prefix + "." + \
            m_data.current_key, value);
    }

    return 1;
}


bool JSON::init() {
    return true;
}


bool JSON::processChunk(const char *buf, unsigned int size, std::string *err) {
    /* Feed our parser and catch any errors */
    m_data.status = yajl_parse(m_data.handle,
        (const unsigned char *)buf, size);
    if (m_data.status != yajl_status_ok) {
        const unsigned char *e = yajl_get_error(m_data.handle, 0,
            (const unsigned char *)buf, size);
        /* We need to free the yajl error message later, how to do this? */
        err->assign((const char *)e);
        return false;
    }

    return true;
}


bool JSON::complete(std::string *err) {
    /* Wrap up the parsing process */
    m_data.status = yajl_complete_parse(m_data.handle);
    if (m_data.status  != yajl_status_ok) {
        const unsigned char *e = yajl_get_error(m_data.handle, 0, NULL, 0);
        /* We need to free the yajl error message later, how to do this? */
        err->assign((const char *)e);
        return false;
    }

    return true;
}


JSON::JSON(Transaction *transaction) : m_transaction(transaction) {
    /**
     * yajl configuration and callbacks
     */
    static yajl_callbacks callbacks = {
        yajl_null,
        yajl_boolean,
        NULL /* yajl_integer  */,
        NULL /* yajl_double */,
        yajl_number,
        yajl_string,
        yajl_start_map,
        yajl_map_key,
        yajl_end_map,
        NULL /* yajl_start_array */,
        NULL /* yajl_end_array  */
    };


    debug(4, "JSON parser initialization");

    /**
     * Prefix and current key are initially empty
     */
    m_data.prefix = "";
    m_data.current_key = "";

    /**
     * yajl initialization
     *
     * yajl_parser_config definition:
     * http://lloyd.github.io/yajl/yajl-2.0.1/yajl__parse_8h.html#aec816c5518264d2ac41c05469a0f986c
     *
     * TODO: make UTF8 validation optional, as it depends on Content-Encoding
     */
    debug(9, "yajl JSON parsing callback initialization");
    m_data.handle = yajl_alloc(&callbacks, NULL, this);

    yajl_config(m_data.handle, yajl_allow_partial_values, 0);
}


JSON::~JSON() {
    debug(4, "JSON: Cleaning up JSON results");
    yajl_free(m_data.handle);
}


}  // namespace RequestBodyProcessor
}  // namespace modsecurity
