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

#include "src/debug_log_writer_agent.h"

#include <stddef.h>

#include <fstream>

#include "debug_log_writer.h"



namespace ModSecurity {


DebugLogWriterAgent::DebugLogWriterAgent(const std::string& fileName) :
    m_referenceCount(0),
    m_fileName(fileName) {
    open(m_fileName, std::fstream::out | std::fstream::app);
}

void DebugLogWriterAgent::write(const std::string& msg) {
    if (!is_open()) {
        std::cout << "Agent: " << m_fileName << ": " << msg << std::endl;
        return;
    }

    *this << msg << std::endl;
    *this << flush();
}


}  // namespace ModSecurity