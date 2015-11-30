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

#ifdef WITH_GEOIP  // WITH_GEOIP
#include <GeoIPCity.h>
#endif

#ifndef SRC_UTILS_GEO_LOOKUP_H_
#define SRC_UTILS_GEO_LOOKUP_H_

#include "modsecurity/assay.h"

namespace modsecurity {
namespace Utils {


class GeoLookup {
 public:
    static GeoLookup& getInstance() {
        static GeoLookup instance;
        return instance;
    }
#ifdef WITH_GEOIP
    bool setDataBase(const std::string& filePath);
    bool lookup(const std::string& target, GeoIPRecord **georec,
        std::function<bool(int, std::string)> callback);
    void cleanUp();
#endif  // WITH_GEOIP

 private:
    GeoLookup()
        : m_gi(NULL) { }
    ~GeoLookup();
    GeoLookup(GeoLookup const&);
    void operator=(GeoLookup const&);
#ifdef WITH_GEOIP
    GeoIP *m_gi;
#else  // WITH_GEOIP
    void *m_gi;
#endif  // WITH_GEOIP
};


}  // namespace Utils
}  // namespace ModSecurity

#endif  // SRC_UTILS_GEO_LOOKUP_H_
