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

#include "modsecurity/audit_log.h"

#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#include <fstream>

#include "modsecurity/rule_message.h"
#include "src/audit_log/writer/https.h"
#include "src/audit_log/writer/parallel.h"
#include "src/audit_log/writer/serial.h"
#include "src/audit_log/writer/writer.h"
#include "src/utils/regex.h"

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

#define AL_MERGE_STRING_CONF(a, c) \
    if (a.empty() == false) { \
        c = a; \
    }


namespace modsecurity {
namespace audit_log {


AuditLog::AuditLog()
    : m_path1(""),
    m_path2(""),
    m_storage_dir(""),
    m_filePermission(-1),
    m_directoryPermission(-1),
    m_parts(-1),
    m_status(NotSetLogStatus),
    m_type(NotSetAuditLogType),
    m_relevant(""),
    m_writer(NULL),
    m_refereceCount(1) { }


AuditLog::~AuditLog() {
    if (m_writer) {
        delete m_writer;
        m_writer = NULL;
    }
}


bool AuditLog::setStorageDirMode(int permission) {
    this->m_directoryPermission = permission;
    return true;
}


bool AuditLog::setFileMode(int permission) {
    this->m_filePermission = permission;
    return true;
}


int AuditLog::getFilePermission() {
    if (m_filePermission == -1) {
        return m_defaultFilePermission;
    }

    return m_filePermission;
}

int AuditLog::getDirectoryPermission() {
    if (m_directoryPermission == -1) {
        return m_defaultDirectoryPermission;
    }

    return m_directoryPermission;
}

bool AuditLog::setStatus(AuditLogStatus status) {
    this->m_status = status;
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
    int parts = 0;

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


int AuditLog::getParts() {
    if (m_parts == -1) {
        return m_defaultParts;
    }

    return m_parts;
}


bool AuditLog::setType(AuditLogType audit_type) {
    this->m_type = audit_type;
    return true;
}



bool AuditLog::init(std::string *error) {
    if (m_type == SerialAuditLogType) {
        m_writer = new audit_log::writer::Serial(this);
    } else if (m_type == HttpsAuditLogType) {
        m_writer = new audit_log::writer::Https(this);
    } else {
        /*
         * if (m_type == ParallelAuditLogType
         * || m_type == NotSetAuditLogType)
         *
         */
        m_writer = new audit_log::writer::Parallel(this);
    }

    if (m_status == OffAuditLogStatus || m_status == NotSetLogStatus) {
        return true;
    }

    if (m_writer == NULL || m_writer->init(error) == false) {
        return false;
    }

    /* Sanity check */
    if (m_status == RelevantOnlyAuditLogStatus) {
        if (m_relevant.empty()) {
            error->assign("m_relevant cannot be null while status is set to " \
                "RelevantOnly");
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


bool AuditLog::saveIfRelevant(Transaction *transaction) {
    return saveIfRelevant(transaction, -1);
}


bool AuditLog::saveIfRelevant(Transaction *transaction, int parts) {
    bool saveAnyway = false;
    if (m_status == OffAuditLogStatus || m_status == NotSetLogStatus) {
        return true;
    }

    for (RuleMessage &i : transaction->m_rulesMessages) {
        if (i.m_noAuditLog == false) {
            saveAnyway = true;
            break;
        }
    }

    if ((m_status == RelevantOnlyAuditLogStatus
        && this->isRelevant(transaction->m_httpCodeReturned) == false)
        && saveAnyway == false) {
        transaction->debug(9, "Return code `" +
            std::to_string(transaction->m_httpCodeReturned) + "'" \
            " is not interesting to audit logs, relevant code(s): `" +
            m_relevant + "'.");

        return false;
    }

    if (parts == -1) {
        parts = m_parts;
    }
    transaction->debug(5, "Saving this request as part " \
            "of the audit logs.");
    if (m_writer == NULL) {
        transaction->debug(1, "Internal error, audit log writer is null");
    } else {
        std::string error;
        bool a = m_writer->write(transaction, parts, &error);
        if (a == false) {
            transaction->debug(1, "Cannot save the audit log: " + error);
            return false;
        }
    }

    return true;
}


bool AuditLog::close() {
    return true;
}


bool AuditLog::merge(AuditLog *from, std::string *error) {
    AL_MERGE_STRING_CONF(from->m_path1, m_path1);
    AL_MERGE_STRING_CONF(from->m_path2, m_path2);
    AL_MERGE_STRING_CONF(from->m_storage_dir, m_storage_dir);
    AL_MERGE_STRING_CONF(from->m_relevant, m_relevant);

    if (from->m_filePermission != -1) {
        m_filePermission = from->m_filePermission;
    }

    if (from->m_directoryPermission != -1) {
        m_directoryPermission = from->m_directoryPermission;
    }

    if (from->m_type != NotSetAuditLogType) {
        m_type = from->m_type;
    }

    if (from->m_status != NotSetLogStatus) {
        m_status = from->m_status;
    }

    if (from->m_parts != -1) {
        m_parts = from->m_parts;
    }

    return init(error);
}


}  // namespace audit_log
}  // namespace modsecurity
