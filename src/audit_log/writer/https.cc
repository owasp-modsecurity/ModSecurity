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

#include "audit_log/writer/https.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
#include <mutex>

#include "audit_log/audit_log.h"
#include "modsecurity/transaction.h"
#include "src/utils.h"
#include "utils/md5.h"
#include "utils/https_client.h"

namespace modsecurity {
namespace audit_log {
namespace writer {


Https::~Https() {
}


bool Https::init() {
    return true;
}


bool Https::write(Transaction *transaction, int parts) {
    Utils::HttpsClient m_http_client;
    transaction->debug(7, "Sending logs to: " + m_audit->m_path1);

    std::string log = transaction->toJSON(parts);
    m_http_client.setRequestType("application/json");
    m_http_client.setRequestBody(log.c_str());
    m_http_client.download(m_audit->m_path1);
    return true;
}

}  // namespace writer
}  // namespace audit_log
}  // namespace modsecurity
