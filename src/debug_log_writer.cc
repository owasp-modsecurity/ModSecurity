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

#include "src/debug_log_writer.h"

#include <stddef.h>

#include <fstream>

#include "src/debug_log_writer_agent.h"

namespace ModSecurity {


void DebugLogWriter::open(const std::string& fileName) {
    std::map<std::string, DebugLogWriterAgent *>::iterator it;
    DebugLogWriterAgent *agent;

    it = agents.find(fileName);
    if (it != agents.end()) {
        agent = it->second;
    } else {
        agent = new DebugLogWriterAgent(fileName);
        agents[fileName] = agent;
    }
    agent->refCountIncrease();
}


void DebugLogWriter::close(const std::string& fileName) {
#if 0
    std::map<std::string, DebugLogWriterAgent *>::iterator it;
    DebugLogWriterAgent *agent;
    it = agents.find(fileName);
    if (it != agents.end()) {
        agent = it->second;
        if (agent->refCountDecreaseAndCheck()) {
            agents.erase(it);
        }
    }
#endif
}


void DebugLogWriter::write(const std::string& file, const std::string &msg) {
    std::map<std::string, DebugLogWriterAgent *>::iterator it;
    DebugLogWriterAgent *agent;

    it = agents.find(file);
    if (it != agents.end()) {
        agent = it->second;
        agent->write(msg);
    } else {
        std::cout << file << ": " << msg << std::endl;
    }
}


}  // namespace ModSecurity

