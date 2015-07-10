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

#include "src/audit_log_writer_parallel.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>

#include "src/audit_log.h"
#include "modsecurity/assay.h"
#include "src/utils.h"

namespace ModSecurity {


inline std::string AuditLogWriterParallel::logFilePath(time_t *t,
    int part) {
    struct tm timeinfo;
    char tstr[300];
    size_t len;
    std::string name("");

    localtime_r(t, &timeinfo);

    if (part & YearMonthDayDirectory) {
        memset(tstr, '\0', 300);
        strftime(tstr, 299, "/%Y%m%d", &timeinfo);
        name = tstr;
    }

    if (part & YearMonthDayAndTimeDirectory) {
        memset(tstr, '\0', 300);
        strftime(tstr, 299, "/%Y%m%d-%H%M", &timeinfo);
        name = name + tstr;
    }

    if (part & YearMonthDayAndTimeFileName) {
        memset(tstr, '\0', 300);
        strftime(tstr, 299, "/%Y%m%d-%H%M%S", &timeinfo);
        name = name + tstr;
    }

    return name;
}


bool AuditLogWriterParallel::init() {
    /** TODO:: Check if the directory exists. */
    /** TODO:: Checking if we have permission to write in the target dir */
    return true;
}


bool AuditLogWriterParallel::close() {
    return true;
}


bool AuditLogWriterParallel::write(Assay *assay) {
    FILE *fp;
    int fd;
    std::string log = assay->to_json(0);
    std::string fileName = logFilePath(&assay->timeStamp,
        YearMonthDayDirectory | YearMonthDayAndTimeDirectory
        | YearMonthDayAndTimeFileName);

    std::string logPath = m_audit->m_storage_dir;
    fileName = logPath + fileName + "-" + assay->id;

    if (logPath.empty()) {
      return false;
    }

    createDir((logPath +
        logFilePath(&assay->timeStamp, YearMonthDayDirectory)).c_str(),
        m_audit->directoryPermission);
    createDir((logPath +
        logFilePath(&assay->timeStamp, YearMonthDayDirectory
            | YearMonthDayAndTimeDirectory)).c_str(),
        m_audit->directoryPermission);

    fd = open(fileName.c_str(), O_CREAT | O_WRONLY, m_audit->filePermission);
    if (fd < 0) {
        return false;
    }
    fp = fdopen(fd, "w");
    fwrite(log.c_str(), log.length(), 1, fp);
    fclose(fp);

    return true;
}

}  // namespace ModSecurity
