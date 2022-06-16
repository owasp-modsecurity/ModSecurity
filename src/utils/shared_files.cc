/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/utils/shared_files.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <utility>
#include <fstream>
#include <string>

namespace modsecurity {
namespace utils {


std::pair<msc_file_handler *, FILE *> SharedFiles::find_handler(
    const std::string &fileName) {
    return std::pair<modsecurity::utils::msc_file_handler *,
        FILE *>(NULL, NULL);
}


std::pair<msc_file_handler *, FILE *> SharedFiles::add_new_handler(
    const std::string &fileName, std::string *error) {
    return std::pair<modsecurity::utils::msc_file_handler *,
        FILE *>(NULL, NULL);
}


bool SharedFiles::open(const std::string& fileName, std::string *error) {
    bool ret = true;

    return ret;
}


void SharedFiles::close(const std::string& fileName) {
    return;
}


bool SharedFiles::write(const std::string& fileName,
		const std::string &msg, std::string *error) {
    bool ret = true;

    return ret;
}


}  // namespace utils
}  // namespace modsecurity
