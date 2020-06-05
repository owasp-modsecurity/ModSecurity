/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include <assert.h>

#endif


#ifndef HEADERS_MODSECURITY_ACTIONS_ACTION_H_
#define HEADERS_MODSECURITY_ACTIONS_ACTION_H_

#ifdef __cplusplus

namespace modsecurity {
class Transaction;
namespace actions {


class Action {
 public:
    Action()
        : m_parserPayload(""),
        m_name("")
    {
        assert(0);
    }


    explicit Action(const std::string& action)
        : m_parserPayload(sort_payload(action)),
        m_name(sort_name(action))
    { }


    Action(const Action &other)
        : m_parserPayload(other.m_parserPayload),
        m_name(other.m_name)
    { }


    Action &operator=(const Action& a) {
        m_name = a.m_name;
        m_parserPayload = a.m_parserPayload;
        return *this;
    }


    virtual ~Action()
    { }


    virtual bool init(std::string *error) {
        return true;
    }


    virtual bool execute(Transaction *transaction = nullptr) noexcept {
        return true;
    }


    const std::string *getName() const noexcept {
        return &m_name;
    }


 protected:
    std::string m_parserPayload;


 private:
    std::string m_name;

    static size_t get_payload_pos(const std::string& data) {
        size_t pos = data.find(":");
        std::string t = "t:";

        if (data.compare(0, t.length(), t) == 0) {
            pos = data.find(":", 2);
        }

        return pos;
    }


    static std::string sort_name(const std::string& data) {
        size_t pos = get_payload_pos(data);
        if (pos == std::string::npos) {
            return data;
        }

        std::string ret(data, 0, pos);
        return ret;
    }


    static std::string sort_payload(const std::string& data) {
        size_t pos = get_payload_pos(data);
        std::string ret("");
        if (pos != std::string::npos) {
            ret = std::string(data, pos + 1, data.length());

            if (ret.at(0) == '\'' && ret.size() > 2) {
                ret.erase(0, 1);
                ret.pop_back();
            }
        }

        return ret;
    }
};


}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // HEADERS_MODSECURITY_ACTIONS_ACTION_H_
