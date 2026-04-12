/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_H_

#include <deque>
#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/rules_set.h"
#include "src/request_body_processor/json_backend.h"


namespace modsecurity::RequestBodyProcessor {


class JSONContainer {
 public:
    explicit JSONContainer(const std::string &name) : m_name(name) { }
    virtual ~JSONContainer() { }
    std::string m_name;
};


class JSONContainerArray : public JSONContainer {
 public:
    explicit JSONContainerArray(const std::string &name) : JSONContainer(name),
        m_elementCounter(0) { }
    size_t m_elementCounter;
};


class JSONContainerMap : public JSONContainer {
 public:
     explicit JSONContainerMap(const std::string &name) : JSONContainer(name) { }
};


class JSON : public JsonEventSink {
 public:
    explicit JSON(Transaction *transaction);
    ~JSON() override;

    bool init();
    bool processChunk(const char *buf, unsigned int size,
        const std::string *err);
    bool complete(std::string *err);

    int addArgument(const std::string& value);

    JsonSinkStatus on_start_object() override;
    JsonSinkStatus on_end_object() override;
    JsonSinkStatus on_start_array() override;
    JsonSinkStatus on_end_array() override;
    JsonSinkStatus on_key(std::string_view value) override;
    JsonSinkStatus on_string(std::string_view value) override;
    JsonSinkStatus on_number(std::string_view value) override;
    JsonSinkStatus on_boolean(bool value) override;
    JsonSinkStatus on_null() override;

    bool isPreviousArray() const {
        const JSONContainerArray *prev = NULL;
        if (m_containers.size() < 1) {
            return false;
        }
        prev = dynamic_cast<JSONContainerArray *>(
            m_containers[m_containers.size() - 1]);
        return prev != NULL;
    }

    std::string getCurrentKey(bool emptyIsNull = false) {
        std::string ret(m_current_key);
        if (m_containers.size() == 0) {
            return "json";
        }
        if (m_current_key.empty() == true) {
            if (isPreviousArray() || emptyIsNull == true) {
                return "";
            }
            return "empty-key";
        }
        m_current_key = "";
        return ret;
    }

    void setMaxDepth(double max_depth) {
        m_max_depth = max_depth;
    }

 private:
    void clearContainers();

    std::deque<JSONContainer *> m_containers;
    Transaction *m_transaction;
    std::string m_current_key;
    std::string m_data;
    double m_max_depth;
    int64_t m_current_depth;
    bool m_depth_limit_exceeded;
};


}  // namespace modsecurity::RequestBodyProcessor

#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_H_
