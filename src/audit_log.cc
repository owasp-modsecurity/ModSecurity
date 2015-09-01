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

#include "src/audit_log.h"

#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#include <fstream>

#include "src/audit_log_writer_parallel.h"
#include "src/audit_log_writer_serial.h"
#include "utils/regex.h"

#define PARTS_CONSTAINS(a, c) \
    if (new_parts.find(toupper(a)) != std::string::npos \
        || new_parts.find(tolower(a)) != std::string::npos) { \
          parts = parts | c; \
    }

#define PARTS_CONSTAINS_REM(a, c) \
    if (new_parts.find(toupper(a)) != std::string::npos \
        || new_parts.find(tolower(a)) != std::string::npos) { \
          parts = parts & ~c; \
    }

namespace ModSecurity {

AuditLog::AuditLog()
    : m_status(OffAuditLogStatus),
    m_path1(""),
    m_path2(""),
    m_storage_dir(""),
    m_parts(AAuditLogPart | BAuditLogPart | CAuditLogPart | FAuditLogPart
            | HAuditLogPart | ZAuditLogPart),
    m_type(ParallelAuditLogType),
    m_writer(NULL),
    m_relevant(""),
    filePermission(0600),
    directoryPermission(0766),
    m_refereceCount(0) { }

AuditLog::~AuditLog() {
    if (m_writer) {
        m_writer->refCountDecreaseAndCheck();
    }
}

void AuditLog::refCountIncrease() {
    m_refereceCount++;
}


void AuditLog::refCountDecreaseAndCheck() {
    m_refereceCount--;
    if (m_refereceCount == 0) {
        delete this;
    }
}

bool AuditLog::setStorageDirMode(int permission) {
    this->directoryPermission = permission;
    return true;
}


bool AuditLog::setFileMode(int permission) {
    this->filePermission = permission;
    return true;
}


bool AuditLog::setStatus(AuditLogStatus new_status) {
    this->m_status = new_status;
    return true;
}


bool AuditLog::setRelevantStatus(const std::basic_string<char>& status) {
    this->m_relevant = std::string(status);
    return true;
}


bool AuditLog::setStorageDir(const std::basic_string<char>& path) {
    this->m_storage_dir = path;
    return true;
}


bool AuditLog::setFilePath1(const std::basic_string<char>& path) {
    this->m_path1 = path;
    return true;
}


bool AuditLog::setFilePath2(const std::basic_string<char>& path) {
    this->m_path2 = path;
    return true;
}


int AuditLog::addParts(int parts, const std::string& new_parts) {
    PARTS_CONSTAINS('A', AAuditLogPart)
    PARTS_CONSTAINS('B', BAuditLogPart)
    PARTS_CONSTAINS('C', CAuditLogPart)
    PARTS_CONSTAINS('D', DAuditLogPart)
    PARTS_CONSTAINS('E', EAuditLogPart)
    PARTS_CONSTAINS('F', FAuditLogPart)
    PARTS_CONSTAINS('G', GAuditLogPart)
    PARTS_CONSTAINS('H', HAuditLogPart)
    PARTS_CONSTAINS('I', IAuditLogPart)
    PARTS_CONSTAINS('J', JAuditLogPart)
    PARTS_CONSTAINS('K', KAuditLogPart)
    PARTS_CONSTAINS('Z', ZAuditLogPart)

    return parts;
}


int AuditLog::removeParts(int parts, const std::string& new_parts) {
    PARTS_CONSTAINS_REM('A', AAuditLogPart)
    PARTS_CONSTAINS_REM('B', BAuditLogPart)
    PARTS_CONSTAINS_REM('C', CAuditLogPart)
    PARTS_CONSTAINS_REM('D', DAuditLogPart)
    PARTS_CONSTAINS_REM('E', EAuditLogPart)
    PARTS_CONSTAINS_REM('F', FAuditLogPart)
    PARTS_CONSTAINS_REM('G', GAuditLogPart)
    PARTS_CONSTAINS_REM('H', HAuditLogPart)
    PARTS_CONSTAINS_REM('I', IAuditLogPart)
    PARTS_CONSTAINS_REM('J', JAuditLogPart)
    PARTS_CONSTAINS_REM('K', KAuditLogPart)
    PARTS_CONSTAINS_REM('Z', ZAuditLogPart)

    return parts;
}


bool AuditLog::setParts(const std::basic_string<char>& new_parts) {
    int parts = m_parts;

    PARTS_CONSTAINS('A', AAuditLogPart)
    PARTS_CONSTAINS('B', BAuditLogPart)
    PARTS_CONSTAINS('C', CAuditLogPart)
    PARTS_CONSTAINS('D', DAuditLogPart)
    PARTS_CONSTAINS('E', EAuditLogPart)
    PARTS_CONSTAINS('F', FAuditLogPart)
    PARTS_CONSTAINS('G', GAuditLogPart)
    PARTS_CONSTAINS('H', HAuditLogPart)
    PARTS_CONSTAINS('I', IAuditLogPart)
    PARTS_CONSTAINS('J', JAuditLogPart)
    PARTS_CONSTAINS('K', KAuditLogPart)
    PARTS_CONSTAINS('Z', ZAuditLogPart)

    m_parts = parts;
    return true;
}


bool AuditLog::setType(AuditLogType audit_type) {
    this->m_type = audit_type;
    return true;
}


bool AuditLog::init() {
    if (m_type == ParallelAuditLogType) {
        m_writer = new AuditLogWriterParallel(this);
    }
    if (m_type == SerialAuditLogType) {
        m_writer = new AuditLogWriterSerial(this);
    }
    m_writer->refCountIncrease();

    if (m_writer == NULL || m_writer->init() == false) {
        std::cout << "not able to open the log for write." << std::endl;
        return false;
    }

    /* Sanity check */
    if (m_status == RelevantOnlyAuditLogStatus) {
        if (m_relevant.empty()) {
            std::cout << "m_relevant cannot be null while status is " << \
                "RelevantOnly" << std::endl;
            return false;
        }
    }

    return true;
}


bool AuditLog::isRelevant(int status) {
    std::string sstatus = std::to_string(status);

    if (m_relevant.empty()) {
        return false;
    }

    if (sstatus.empty()) {
        return true;
    }

    return Utils::regex_search(sstatus,
        Utils::Regex(m_relevant)) != 0;
}


bool AuditLog::saveIfRelevant(Assay *assay) {
    return saveIfRelevant(assay, -1);
}


bool AuditLog::saveIfRelevant(Assay *assay, int parts) {
    if (this->isRelevant(assay->httpCodeReturned) == false &&
        assay->save_in_auditlog == false) {
        return false;
    }

    /**
     * Even if it is relevant, if it is marked not to be save,
     * we won't save it.
     *
     */
    if (assay->do_not_save_in_auditlog == true) {
        return false;
    }

    if (parts == -1)
    {
        parts = m_parts;
    }
    m_writer->write(assay, parts);

    return true;
}


bool AuditLog::close() {
    return true;
}


}  // namespace ModSecurity
