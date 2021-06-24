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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef __OpenBSD__
#include <glob.h>
#else
#include <wordexp.h>
#endif
#include <stdint.h>
#include <inttypes.h>

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "src/utils/system.h"
#include "src/config.h"

namespace modsecurity {
namespace utils {


double cpu_seconds(void) {
        return 0;
}


std::string find_resource(const std::string& resource,
    const std::string& config, std::string *err) {
    return std::string("");
}


std::string get_path(const std::string& file) {
    return std::string("");
}


std::list<std::string> expandEnv(const std::string& var, int flags) {
    std::list<std::string> vars;
    return vars;
}

bool createDir(std::string dir, int mode, std::string *error) {

    return true;
}


bool isFile(std::string f) {

    return true;
}


}  // namespace utils
}  // namespace modsecurity
