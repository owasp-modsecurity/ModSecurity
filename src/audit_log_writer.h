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

#ifndef SRC_AUDIT_LOG_WRITER_H_
#define SRC_AUDIT_LOG_WRITER_H_

#ifdef __cplusplus
#include <string>
#endif

#include "modsecurity/assay.h"


#ifdef __cplusplus

namespace modsecurity {
class AuditLog;

/** @ingroup ModSecurity_CPP_API */
class AuditLogWriter {
 public:
    explicit AuditLogWriter(AuditLog *audit)
        : m_audit(audit),
        m_refereceCount(0) { }

    virtual ~AuditLogWriter() { }

    virtual void refCountIncrease() = 0;
    virtual void refCountDecreaseAndCheck() = 0;

    virtual bool init() { return true; }
    virtual bool write(Assay *assay, int parts);

    std::string file_name(const std::string& unique_id);

 protected:
    AuditLog *m_audit;
    int m_refereceCount;
};

}  // namespace modsecurity
#endif

#endif  // SRC_AUDIT_LOG_WRITER_H_
