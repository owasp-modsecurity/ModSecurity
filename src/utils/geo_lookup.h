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

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include <GeoIPCity.h>

#ifndef SRC_UTILS_GEO_LOOKUP_H_
#define SRC_UTILS_GEO_LOOKUP_H_

#include "modsecurity/assay.h"

namespace ModSecurity {
namespace Utils {


class GeoLookup {
 public:
    static GeoLookup& getInstance() {
        static GeoLookup instance;
        return instance;
    }

    bool setDataBase(std::string file_path);
    bool lookup(const std::string& target, GeoIPRecord **georec,
        std::function<bool(int, std::string)> callback);

 private:
    GeoLookup() : m_gi(NULL) {}

    GeoLookup(GeoLookup const&);
    void operator=(GeoLookup const&);

    GeoIP *m_gi;
};


}  // namespace Utils
}  // namespace ModSecurity

#endif  // SRC_UTILS_GEO_LOOKUP_H_
