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

#include "operators/geo_lookup.h"

#ifdef WITH_GEOIP
#include <GeoIPCity.h>
#endif

#include <string>
#include <functional>

#include "operators/operator.h"
#include "utils/geo_lookup.h"


namespace modsecurity {
namespace operators {


bool GeoLookup::evaluate(Assay *assay, const std::string &exp) {
    using std::placeholders::_1;
    using std::placeholders::_2;
    bool ret = true;

#ifdef WITH_GEOIP
    GeoIPRecord *gir;

    if (assay) {
        ret = Utils::GeoLookup::getInstance().lookup(exp, &gir,
            std::bind(&GeoLookup::debug, this, assay, _1, _2));
    } else {
        ret = Utils::GeoLookup::getInstance().lookup(exp, &gir,
            nullptr);
    }
    if (ret && gir) {
        if (gir->country_code) {
            assay->m_collections.store("GEO:COUNTRY_CODE", gir->country_code);
        }
        if (gir->country_code3) {
            assay->m_collections.store("GEO:COUNTRY_CODE3", gir->country_code3);
        }
        if (gir->country_name) {
            assay->m_collections.store("GEO:COUNTRY_NAME", gir->country_name);
        }
        if (gir->continent_code) {
            assay->m_collections.store("GEO:COUNTRY_CONTINENT",
                gir->continent_code);
        }
        if (gir->country_code && gir->region) {
            assay->m_collections.store("GEO:REGION",
                GeoIP_region_name_by_code(gir->country_code, gir->region));
        }
        if (gir->city) {
            assay->m_collections.store("GEO:CITY", gir->city);
        }
        if (gir->postal_code) {
            assay->m_collections.store("GEO:POSTAL_CODE", gir->postal_code);
        }
        if (gir->latitude) {
            assay->m_collections.store("GEO:LATITUDE",
                std::to_string(gir->latitude));
        }
        if (gir->longitude) {
            assay->m_collections.store("GEO:LONGITUDE",
                std::to_string(gir->longitude));
        }
        if (gir->metro_code) {
            assay->m_collections.store("GEO:DMA_CODE",
                std::to_string(gir->metro_code));
        }
        if (gir->area_code) {
            assay->m_collections.store("GEO:AREA_CODE",
                std::to_string(gir->area_code));
        }

        GeoIPRecord_delete(gir);
    }
#endif  // WITH_GEOIP

    return ret;
}

GeoLookup::GeoLookup(std::string op, std::string param,
bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace ModSecurity
