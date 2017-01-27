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

#include "src/operators/geo_lookup.h"

#ifdef WITH_GEOIP
#include <GeoIPCity.h>
#endif

#include <string>
#include <functional>

#include "src/operators/operator.h"
#include "src/utils/geo_lookup.h"


namespace modsecurity {
namespace operators {


bool GeoLookup::evaluate(Transaction *trans, const std::string &exp) {
    using std::placeholders::_1;
    using std::placeholders::_2;
    bool ret = true;

#ifdef WITH_GEOIP
    GeoIPRecord *gir;

    if (trans) {
        ret = Utils::GeoLookup::getInstance().lookup(exp, &gir,
            std::bind(&GeoLookup::debug, this, trans, _1, _2));
    } else {
        ret = Utils::GeoLookup::getInstance().lookup(exp, &gir,
            nullptr);
    }
    if (ret && gir) {
        if (gir->country_code) {
            trans->m_variableGeo.set("COUNTRY_CODE",
                std::string(gir->country_code), 0);
        }
        if (gir->country_code3) {
            trans->m_variableGeo.set("COUNTRY_CODE3",
                std::string(gir->country_code3), 0);
        }
        if (gir->country_name) {
            trans->m_variableGeo.set("COUNTRY_NAME",
                std::string(gir->country_name), 0);
        }
        if (gir->continent_code) {
            trans->m_variableGeo.set("COUNTRY_CONTINENT",
                std::string(gir->continent_code), 0);
        }
        if (gir->country_code && gir->region) {
            trans->m_variableGeo.set("REGION",
                std::string(GeoIP_region_name_by_code(gir->country_code,
                    gir->region)), 0);
        }
        if (gir->city) {
            trans->m_variableGeo.set("CITY", std::string(gir->city), 0);
        }
        if (gir->postal_code) {
            trans->m_variableGeo.set("POSTAL_CODE",
                std::string(gir->postal_code), 0);
        }
        if (gir->latitude) {
            trans->m_variableGeo.set("LATITUDE",
                std::to_string(gir->latitude), 0);
        }
        if (gir->longitude) {
            trans->m_variableGeo.set("LONGITUDE",
                std::to_string(gir->longitude), 0);
        }
        if (gir->metro_code) {
            trans->m_variableGeo.set("DMA_CODE",
                std::to_string(gir->metro_code), 0);
        }
        if (gir->area_code) {
            trans->m_variableGeo.set("AREA_CODE",
                std::to_string(gir->area_code), 0);
        }

        GeoIPRecord_delete(gir);
    }
#endif  // WITH_GEOIP

    return ret;
}


}  // namespace operators
}  // namespace modsecurity
