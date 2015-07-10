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
    std::string log = assay->to_json(0);
    std::string fileName = logFilePath(&assay->timeStamp,
        YearMonthDayDirectory | YearMonthDayAndTimeDirectory
        | YearMonthDayAndTimeFileName);
    fileName = fileName + "-" + assay->id;

    std::string logPath = m_audit->m_storage_dir;

    createDir((logPath +
        logFilePath(&assay->timeStamp, YearMonthDayDirectory)).c_str());
    createDir((logPath +
        logFilePath(&assay->timeStamp, YearMonthDayDirectory
            | YearMonthDayAndTimeDirectory)).c_str());

    std::ofstream f;
    f.open(logPath + fileName, std::fstream::out | std::fstream::app);
    f << log;
    f.close();

    return true;
}

}  // namespace ModSecurity
