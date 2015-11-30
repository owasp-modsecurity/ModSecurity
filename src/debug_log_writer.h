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

#include <iostream>
#include <map>
#include <string>

#ifndef SRC_DEBUG_LOG_WRITER_H_
#define SRC_DEBUG_LOG_WRITER_H_

#include "src/debug_log_writer_agent.h"


namespace modsecurity {

/** @ingroup ModSecurity_CPP_API */
class DebugLogWriter {
 public:
    static DebugLogWriter& getInstance() {
        static DebugLogWriter instance;
        return instance;
    }

    void write(const std::string& file, const std::string& msg);
    void close(const std::string& m_fileName);
    void open(const std::string& m_fileName);
 private:
    DebugLogWriter() {}

    // C++ 03
    // ========
    // Dont forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    DebugLogWriter(DebugLogWriter const&);
    void operator=(DebugLogWriter const&);


    std::map<std::string, DebugLogWriterAgent *> agents;
};


}  // namespace ModSecurity

#endif  // SRC_DEBUG_LOG_WRITER_H_
