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

#include "src/audit_log/writer/serial.h"

// #include <mutex>

#include "src/audit_log/audit_log.h"

namespace modsecurity {
namespace audit_log {
namespace writer {
// static std::mutex serialLoggingMutex;


Serial::~Serial() {
    m_log.close();
}


void Serial::generateBoundary(std::string *boundary) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < SERIAL_AUDIT_LOG_BOUNDARY_LENGTH; ++i) {
        boundary->append(1, alphanum[rand() % (sizeof(alphanum) - 1)]);
    }
}


bool Serial::init() {
    m_log.open(m_audit->m_path1, std::fstream::out | std::fstream::app);
    return true;
}


bool Serial::write(Transaction *transaction, int parts) {
    std::string boundary;

    generateBoundary(&boundary);

    // serialLoggingMutex.lock();

    m_log << transaction->toOldAuditLogFormat(parts, "-" + boundary + "--");
    m_log.flush();

    // serialLoggingMutex.unlock();

    return true;
}

}  // namespace writer
}  // namespace audit_log
}  // namespace modsecurity
