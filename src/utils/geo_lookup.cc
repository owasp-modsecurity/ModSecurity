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

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include <fstream>
#include <iostream>

#include "utils/geo_lookup.h"

#include <GeoIPCity.h>

namespace ModSecurity {
namespace Utils {


GeoLookup::~GeoLookup() {
    cleanUp();
}

void GeoLookup::cleanUp() {
    if (m_gi != NULL) {
        GeoIP_delete(m_gi);
        m_gi = NULL;
    }
}


bool GeoLookup::setDataBase(const std::string& filePath) {
    m_gi = GeoIP_open(filePath.c_str(), GEOIP_INDEX_CACHE);

    if (m_gi == NULL) {
        return false;
    }

    return true;
}


bool GeoLookup::lookup(const std::string& target, GeoIPRecord **gir,
    std::function<bool(int, std::string)> debug) {
    if (m_gi == NULL) {
        if (debug) {
            debug(4, "GeoIP: Database is not open. " \
                "Use: SecGeoLookupDb directive.");
        }
        return false;
    }

    *gir = GeoIP_record_by_name(m_gi, target.c_str());
    if (*gir == NULL) {
        return false;
    }

    return true;
}


}  // namespace Utils
}  // namespace ModSecurity

