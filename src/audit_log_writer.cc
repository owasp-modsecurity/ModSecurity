/**
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

#include "src/audit_log_writer.h"

#include <string>

#include "src/audit_log.h"

namespace ModSecurity {

std::string AuditLogWriter::file_name(const std::string& unique_id) {
    time_t timer;
    time(&timer);

    /** TODO: return file with time stamp and etc.  */
    return std::string("/tmp/temp_audit_log_file.txt");
}
/**
 *
 * Temporary print the log into the std::cout to debug purposes.
 *
 */
bool AuditLogWriter::write(Assay *assay, int parts) {
    std::cout << assay->to_json(0) << std::endl;
    return true;
}


}  // namespace ModSecurity
