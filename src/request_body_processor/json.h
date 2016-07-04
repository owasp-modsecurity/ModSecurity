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


#include <yajl/yajl_parse.h>

#include <string>
#include <iostream>

#include "modsecurity/transaction.h"
#include "modsecurity/rules.h"

#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_H_


namespace modsecurity {
namespace RequestBodyProcessor {


struct json_data {
    /* yajl configuration and parser state */
    yajl_handle handle;
    yajl_status status;

    /* prefix is used to create data hierarchy (i.e., 'parent.child.value') */
    std::string prefix;
    std::string current_key;
};

typedef struct json_data json_data;


class JSON {
 public:
    explicit JSON(Transaction *transaction);
    ~JSON();

    bool init();
    bool processChunk(const char *buf, unsigned int size, std::string *err);
    bool complete(std::string *err);

    int addArgument(const std::string& value);

    static int yajl_end_map(void *ctx);
    static int yajl_start_map(void *ctx);
    static int yajl_number(void *ctx, const char *value, size_t length);
    static int yajl_string(void *ctx, const unsigned char *value,
        size_t length);
    static int yajl_boolean(void *ctx, int value);
    static int yajl_null(void *ctx);
    static int yajl_map_key(void *ctx, const unsigned char *key,
        size_t length);

#ifndef NO_LOGS
    void debug(int a, std::string str) {
        m_transaction->debug(a, str);
    }
#endif
    json_data m_data;

 private:
    Transaction *m_transaction;
    std::string m_header;
};


}  // namespace RequestBodyProcessor
}  // namespace modsecurity


#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_H_
