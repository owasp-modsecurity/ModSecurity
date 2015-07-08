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

#ifndef SRC_AUDIT_LOG_WRITER_SERIAL_H_
#define SRC_AUDIT_LOG_WRITER_SERIAL_H_

#include "src/audit_log_writer.h"

#ifdef __cplusplus

namespace ModSecurity {

/** @ingroup ModSecurity_CPP_API */
class AuditLogWriterSerial : public AuditLogWriter {
 public:
    AuditLogWriterSerial() { }

    bool init() override;
    bool close() override;
    bool write(const std::string& log) override;
};

}  // namespace ModSecurity
#endif

#endif  // SRC_AUDIT_LOG_WRITER_SERIAL_H_
