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

#ifndef SRC_OPERATORS_VALIDATE_SCHEMA_H_
#define SRC_OPERATORS_VALIDATE_SCHEMA_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

#include <string>

#include "operators/operator.h"

#ifdef __cplusplus
namespace modsecurity {
namespace operators {

class ValidateSchema : public Operator {
 public:
    /** @ingroup ModSecurity_Operator */
    ValidateSchema(std::string o, std::string p, bool i)
        : Operator(o, p, i),
        m_schema(NULL),
        m_validCtx(NULL),
        m_parserCtx(NULL) { }
    ~ValidateSchema() {
        /*
        if (m_schema != NULL) {
            xmlSchemaFree(m_schema);
            m_schema = NULL;
        }
        */
        if (m_validCtx != NULL) {
            xmlSchemaFreeValidCtxt(m_validCtx);
            m_validCtx = NULL;
        }
    }

    bool evaluate(Transaction *transaction, const std::string  &str) override;
    bool init(const std::string &file, std::string *error) override;


    static void error_load(void *ctx, const char *msg, ...) {
        std::string *t = reinterpret_cast<std::string *>(ctx);
        char buf[1024];
        va_list args;

        va_start(args, msg);
        int len = vsnprintf(buf, sizeof(buf), msg, args);
        va_end(args);

        if (len > 0) {
            t->append("XML Error: " + std::string(buf));
        }
    }


    static void warn_load(void *ctx, const char *msg, ...) {
        std::string *t = reinterpret_cast<std::string *>(ctx);
        char buf[1024];
        va_list args;

        va_start(args, msg);
        int len = vsnprintf(buf, sizeof(buf), msg, args);
        va_end(args);

        if (len > 0) {
            t->append("XML Warning: " + std::string(buf));
        }
    }


    static void error_runtime(void *ctx, const char *msg, ...) {
        Transaction *t = reinterpret_cast<Transaction *>(ctx);
        char buf[1024];
        std::string s;
        va_list args;

        va_start(args, msg);
        int len = vsnprintf(buf, sizeof(buf), msg, args);
        va_end(args);

        if (len > 0) {
            s = "XML Error: " + std::string(buf);
        }
        t->debug(4, s);
    }


    static void warn_runtime(void *ctx, const char *msg, ...) {
        Transaction *t = reinterpret_cast<Transaction *>(ctx);
        char buf[1024];
        std::string s;
        va_list args;

        va_start(args, msg);
        int len = vsnprintf(buf, sizeof(buf), msg, args);
        va_end(args);

        if (len > 0) {
            s = "XML Warning: " + std::string(buf);
        }
        t->debug(4, s);
    }

    static void null_error(void *ctx, const char *msg, ...) {
    }

 private:
    xmlSchemaParserCtxtPtr m_parserCtx;
    xmlSchemaValidCtxtPtr m_validCtx;
    xmlSchemaPtr m_schema;
    std::string m_resource;
    std::string m_err;
};

}  // namespace operators
}  // namespace modsecurity
#endif


#endif  // SRC_OPERATORS_VALIDATE_SCHEMA_H_
