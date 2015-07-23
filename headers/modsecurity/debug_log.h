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
#include <iostream>
#include <fstream>
#include <string>
#endif

#ifndef HEADERS_MODSECURITY_DEBUG_LOG_H_
#define HEADERS_MODSECURITY_DEBUG_LOG_H_


#ifndef __cplusplus
typedef struct DebugLog_t DebugLog;
#endif

#ifdef __cplusplus

namespace ModSecurity {

/** @ingroup ModSecurity_CPP_API */
class DebugLog : public std::ofstream {
 public:
    /*
    static ModSecurityDebugLog& instance() {
        static ModSecurityDebugLog i;
        return i;
    }
    */
    DebugLog()
        : m_is_configured(false),
        m_debug_level(0) { }

    bool setOutputFile(const std::string& file);
    virtual bool write_log(int level, const std::string& data);
    bool setDebugLevel(int level);
    bool isConfigured();



    virtual DebugLog *new_instance();

 private:
        /*
        ModSecurityDebugLog(ModSecurityDebugLog const&);
        void operator=(ModSecurityDebugLog const&);
        */
        int m_debug_level;
        bool m_is_configured;
};

}  // namespace ModSecurity
#endif

#endif  // HEADERS_MODSECURITY_DEBUG_LOG_H_
