/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#include "msc_geo.h"


/* -- Lookup Tables -- */

static const char geo_country_code[GEO_COUNTRY_LAST + 1][4] = {
    "--",
    "AP","EU","AD","AE","AF","AG","AI","AL","AM","AN",
    "AO","AQ","AR","AS","AT","AU","AW","AZ","BA","BB",
    "BD","BE","BF","BG","BH","BI","BJ","BM","BN","BO",
    "BR","BS","BT","BV","BW","BY","BZ","CA","CC","CD",
    "CF","CG","CH","CI","CK","CL","CM","CN","CO","CR",
    "CU","CV","CX","CY","CZ","DE","DJ","DK","DM","DO",
    "DZ","EC","EE","EG","EH","ER","ES","ET","FI","FJ",
    "FK","FM","FO","FR","FX","GA","GB","GD","GE","GF",
    "GH","GI","GL","GM","GN","GP","GQ","GR","GS","GT",
    "GU","GW","GY","HK","HM","HN","HR","HT","HU","ID",
    "IE","IL","IN","IO","IQ","IR","IS","IT","JM","JO",
    "JP","KE","KG","KH","KI","KM","KN","KP","KR","KW",
    "KY","KZ","LA","LB","LC","LI","LK","LR","LS","LT",
    "LU","LV","LY","MA","MC","MD","MG","MH","MK","ML",
    "MM","MN","MO","MP","MQ","MR","MS","MT","MU","MV",
    "MW","MX","MY","MZ","NA","NC","NE","NF","NG","NI",
    "NL","NO","NP","NR","NU","NZ","OM","PA","PE","PF",
    "PG","PH","PK","PL","PM","PN","PR","PS","PT","PW",
    "PY","QA","RE","RO","RU","RW","SA","SB","SC","SD",
    "SE","SG","SH","SI","SJ","SK","SL","SM","SN","SO",
    "SR","ST","SV","SY","SZ","TC","TD","TF","TG","TH",
    "TJ","TK","TM","TN","TO","TL","TR","TT","TV","TW",
    "TZ","UA","UG","UM","US","UY","UZ","VA","VC","VE",
    "VG","VI","VN","VU","WF","WS","YE","YT","RS","ZA",
    "ZM","ME","ZW","A1","A2","O1","AX","GG","IM","JE"
};

static const char geo_country_code3[GEO_COUNTRY_LAST + 1][4] = {
    "--",
    "AP","EU","AND","ARE","AFG","ATG","AIA","ALB","ARM","ANT",
    "AGO","AQ","ARG","ASM","AUT","AUS","ABW","AZE","BIH","BRB",
    "BGD","BEL","BFA","BGR","BHR","BDI","BEN","BMU","BRN","BOL",
    "BRA","BHS","BTN","BV","BWA","BLR","BLZ","CAN","CC","COD",
    "CAF","COG","CHE","CIV","COK","CHL","CMR","CHN","COL","CRI",
    "CUB","CPV","CX","CYP","CZE","DEU","DJI","DNK","DMA","DOM",
    "DZA","ECU","EST","EGY","ESH","ERI","ESP","ETH","FIN","FJI",
    "FLK","FSM","FRO","FRA","FX","GAB","GBR","GRD","GEO","GUF",
    "GHA","GIB","GRL","GMB","GIN","GLP","GNQ","GRC","GS","GTM",
    "GUM","GNB","GUY","HKG","HM","HND","HRV","HTI","HUN","IDN",
    "IRL","ISR","IND","IO","IRQ","IRN","ISL","ITA","JAM","JOR",
    "JPN","KEN","KGZ","KHM","KIR","COM","KNA","PRK","KOR","KWT",
    "CYM","KAZ","LAO","LBN","LCA","LIE","LKA","LBR","LSO","LTU",
    "LUX","LVA","LBY","MAR","MCO","MDA","MDG","MHL","MKD","MLI",
    "MMR","MNG","MAC","MNP","MTQ","MRT","MSR","MLT","MUS","MDV",
    "MWI","MEX","MYS","MOZ","NAM","NCL","NER","NFK","NGA","NIC",
    "NLD","NOR","NPL","NRU","NIU","NZL","OMN","PAN","PER","PYF",
    "PNG","PHL","PAK","POL","SPM","PCN","PRI","PSE","PRT","PLW",
    "PRY","QAT","REU","ROU","RUS","RWA","SAU","SLB","SYC","SDN",
    "SWE","SGP","SHN","SVN","SJM","SVK","SLE","SMR","SEN","SOM",
    "SUR","STP","SLV","SYR","SWZ","TCA","TCD","TF","TGO","THA",
    "TJK","TKL","TKM","TUN","TON","TLS","TUR","TTO","TUV","TWN",
    "TZA","UKR","UGA","UM","USA","URY","UZB","VAT","VCT","VEN",
    "VGB","VIR","VNM","VUT","WLF","WSM","YEM","YT","SRB","ZAF",
    "ZMB","MNE","ZWE","A1","A2","O1","ALA","GGY","IMN","JEY"
};

static const char *const geo_country_name[GEO_COUNTRY_LAST + 1] = {
    "N/A",
    "Asia/Pacific Region","Europe","Andorra","United Arab Emirates","Afghanistan","Antigua and Barbuda","Anguilla","Albania","Armenia","Netherlands Antilles",
    "Angola","Antarctica","Argentina","American Samoa","Austria","Australia","Aruba","Azerbaijan","Bosnia and Herzegovina","Barbados",
    "Bangladesh","Belgium","Burkina Faso","Bulgaria","Bahrain","Burundi","Benin","Bermuda","Brunei Darussalam","Bolivia",
    "Brazil","Bahamas","Bhutan","Bouvet Island","Botswana","Belarus","Belize","Canada","Cocos (Keeling) Islands","Congo, The Democratic Republic of the",
    "Central African Republic","Congo","Switzerland","Cote D'Ivoire","Cook Islands","Chile","Cameroon","China","Colombia","Costa Rica",
    "Cuba","Cape Verde","Christmas Island","Cyprus","Czech Republic","Germany","Djibouti","Denmark","Dominica","Dominican Republic",
    "Algeria","Ecuador","Estonia","Egypt","Western Sahara","Eritrea","Spain","Ethiopia","Finland","Fiji",
    "Falkland Islands (Malvinas)","Micronesia, Federated States of","Faroe Islands","France","France, Metropolitan","Gabon","United Kingdom","Grenada","Georgia","French Guiana",
    "Ghana","Gibraltar","Greenland","Gambia","Guinea","Guadeloupe","Equatorial Guinea","Greece","South Georgia and the South Sandwich Islands","Guatemala",
    "Guam","Guinea-Bissau","Guyana","Hong Kong","Heard Island and McDonald Islands","Honduras","Croatia","Haiti","Hungary","Indonesia",
    "Ireland","Israel","India","British Indian Ocean Territory","Iraq","Iran, Islamic Republic of","Iceland","Italy","Jamaica","Jordan",
    "Japan","Kenya","Kyrgyzstan","Cambodia","Kiribati","Comoros","Saint Kitts and Nevis","Korea, Democratic People's Republic of","Korea, Republic of","Kuwait",
    "Cayman Islands","Kazakhstan","Lao People's Democratic Republic","Lebanon","Saint Lucia","Liechtenstein","Sri Lanka","Liberia","Lesotho","Lithuania",
    "Luxembourg","Latvia","Libyan Arab Jamahiriya","Morocco","Monaco","Moldova, Republic of","Madagascar","Marshall Islands","Macedonia","Mali",
    "Myanmar","Mongolia","Macau","Northern Mariana Islands","Martinique","Mauritania","Montserrat","Malta","Mauritius","Maldives",
    "Malawi","Mexico","Malaysia","Mozambique","Namibia","New Caledonia","Niger","Norfolk Island","Nigeria","Nicaragua",
    "Netherlands","Norway","Nepal","Nauru","Niue","New Zealand","Oman","Panama","Peru","French Polynesia",
    "Papua New Guinea","Philippines","Pakistan","Poland","Saint Pierre and Miquelon","Pitcairn Islands","Puerto Rico","Palestinian Territory","Portugal","Palau",
    "Paraguay","Qatar","Reunion","Romania","Russian Federation","Rwanda","Saudi Arabia","Solomon Islands","Seychelles","Sudan",
    "Sweden","Singapore","Saint Helena","Slovenia","Svalbard and Jan Mayen","Slovakia","Sierra Leone","San Marino","Senegal","Somalia","Suriname",
    "Sao Tome and Principe","El Salvador","Syrian Arab Republic","Swaziland","Turks and Caicos Islands","Chad","French Southern Territories","Togo","Thailand",
    "Tajikistan","Tokelau","Turkmenistan","Tunisia","Tonga","Timor-Leste","Turkey","Trinidad and Tobago","Tuvalu","Taiwan",
    "Tanzania, United Republic of","Ukraine","Uganda","United States Minor Outlying Islands","United States","Uruguay","Uzbekistan","Holy See (Vatican City State)","Saint Vincent and the Grenadines","Venezuela",
    "Virgin Islands, British","Virgin Islands, U.S.","Vietnam","Vanuatu","Wallis and Futuna","Samoa","Yemen","Mayotte","Serbia","South Africa",
    "Zambia","Montenegro","Zimbabwe","Anonymous Proxy","Satellite Provider","Other","Aland Islands","Guernsey","Isle of Man","Jersey"
};

static const char geo_country_continent[GEO_COUNTRY_LAST + 1][4] = {
    "--",
    "AS","EU","EU","AS","AS","SA","SA","EU","AS","SA",
    "AF","AN","SA","OC","EU","OC","SA","AS","EU","SA",
    "AS","EU","AF","EU","AS","AF","AF","SA","AS","SA",
    "SA","SA","AS","AF","AF","EU","SA","NA","AS","AF",
    "AF","AF","EU","AF","OC","SA","AF","AS","SA","SA",
    "SA","AF","AS","AS","EU","EU","AF","EU","SA","SA",
    "AF","SA","EU","AF","AF","AF","EU","AF","EU","OC",
    "SA","OC","EU","EU","EU","AF","EU","SA","AS","SA",
    "AF","EU","SA","AF","AF","SA","AF","EU","SA","SA",
    "OC","AF","SA","AS","AF","SA","EU","SA","EU","AS",
    "EU","AS","AS","AS","AS","AS","EU","EU","SA","AS",
    "AS","AF","AS","AS","OC","AF","SA","AS","AS","AS",
    "SA","AS","AS","AS","SA","EU","AS","AF","AF","EU",
    "EU","EU","AF","AF","EU","EU","AF","OC","EU","AF",
    "AS","AS","AS","OC","SA","AF","SA","EU","AF","AS",
    "AF","NA","AS","AF","AF","OC","AF","OC","AF","SA",
    "EU","EU","AS","OC","OC","OC","AS","SA","SA","OC",
    "OC","AS","AS","EU","SA","OC","SA","AS","EU","OC",
    "SA","AS","AF","EU","AS","AF","AS","OC","AF","AF",
    "EU","AS","AF","EU","EU","EU","AF","EU","AF","AF",
    "SA","AF","SA","AS","AF","SA","AF","AF","AF","AS",
    "AS","OC","AS","AF","OC","AS","AS","SA","OC","AS",
    "AF","EU","AF","OC","NA","SA","AS","EU","SA","SA",
    "SA","SA","AS","OC","OC","OC","AS","AF","EU","AF",
    "AF","EU","AF","--","--","--","EU","EU","EU","EU"
};

typedef enum {
GEOIP_COUNTRY_EDITION     = 1,
GEOIP_REGION_EDITION_REV0 = 7,
GEOIP_CITY_EDITION_REV0   = 6,
GEOIP_ORG_EDITION         = 5,
GEOIP_ISP_EDITION         = 4,
GEOIP_CITY_EDITION_REV1   = 2,
GEOIP_REGION_EDITION_REV1 = 3,
GEOIP_PROXY_EDITION       = 8,
GEOIP_ASNUM_EDITION       = 9,
GEOIP_NETSPEED_EDITION    = 10,
GEOIP_DOMAIN_EDITION      = 11
} GeoIPDBTypes;

static void create_segments(geo_db *geo) {
    int i, j;
    unsigned char delim[3];
    unsigned char buf[GEO_SEGMENT_RECORD_LENGTH];
    apr_size_t nbytes;
    apr_off_t offset;

    geo->ctry_offset = 0;

    geo->dbtype = GEOIP_COUNTRY_EDITION;
    offset = -3l;
    apr_file_seek(geo->db, APR_END, &offset);

    for (i = 0; i < GEO_STRUCT_INFO_MAX_SIZE; i++) {

        apr_file_read_full(geo->db, &delim, 3, &nbytes);

        if (delim[0] == 255 && delim[1] == 255 && delim[2] == 255) {
            apr_file_read_full(geo->db, &geo->dbtype, 1, &nbytes);
            if (geo->dbtype >= 106) {
                geo->dbtype -= 105;
            }

            if (geo->dbtype == GEOIP_REGION_EDITION_REV0) {
                geo->ctry_offset = GEO_STATE_BEGIN_REV0;
            } else if (geo->dbtype == GEOIP_REGION_EDITION_REV1) {
                geo->ctry_offset = GEO_STATE_BEGIN_REV1;
            } else if (geo->dbtype == GEOIP_CITY_EDITION_REV0 ||
                                 geo->dbtype == GEOIP_CITY_EDITION_REV1 ||
                                 geo->dbtype == GEOIP_ORG_EDITION ||
                                 geo->dbtype == GEOIP_ISP_EDITION ||
                                 geo->dbtype == GEOIP_ASNUM_EDITION) {
                geo->ctry_offset = 0;
                apr_file_read_full(geo->db, &buf, GEO_SEGMENT_RECORD_LENGTH, &nbytes);
                for (j = 0; j < GEO_SEGMENT_RECORD_LENGTH; j++) {
                    geo->ctry_offset += (buf[j] << (j * 8));
                }
            }
            break;
        } else {
            offset = -4l;
            apr_file_seek(geo->db, APR_CUR, &offset);
        }
    }
    if (geo->dbtype == GEOIP_COUNTRY_EDITION ||
            geo->dbtype == GEOIP_PROXY_EDITION ||
            geo->dbtype == GEOIP_NETSPEED_EDITION) {
        geo->ctry_offset = GEO_COUNTRY_BEGIN;
    }
}

static int db_open(directory_config *dcfg, char **error_msg)
{
    char errstr[1024];
    apr_pool_t *mp = dcfg->mp;
    geo_db *geo = dcfg->geo;
    apr_status_t rc;

    #ifdef DEBUG_CONF
    fprintf(stderr, "GEO: Initializing geo DB \"%s\".\n", geo->dbfn);
    #endif

    if ((rc = apr_file_open(&geo->db, geo->dbfn, APR_READ, APR_OS_DEFAULT, mp)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(mp, "Could not open geo database \"%s\": %s", geo->dbfn, apr_strerror(rc, errstr, 1024));
        return 0;
    }

    create_segments(geo);
    return 1;
}

static int field_length(const char *field, int maxlen)
{
    int i;

    if (field == NULL) {
        return 0;
    }

    for (i = 0; i < maxlen; i++) {
        if (field[i] == '\0') {
            break;
        }
    }

    return i;
}

/**
 * Initialise Geo data structure
 */
int geo_init(directory_config *dcfg, const char *dbfn, char **error_msg)
{
    *error_msg = NULL;

    if ((dcfg->geo == NULL) || (dcfg->geo == NOT_SET_P)) {
        dcfg->geo = apr_pcalloc(dcfg->mp, sizeof(geo_db));
    }

    dcfg->geo->db = NULL;
    dcfg->geo->dbfn = apr_pstrdup(dcfg->mp, dbfn);
    dcfg->geo->dbtype = 0;
    dcfg->geo->ctry_offset = 0;

    return db_open(dcfg, error_msg);
}

/**
 * Perform geographical lookup on target.
 */
int geo_lookup(modsec_rec *msr, geo_rec *georec, const char *target, char **error_msg)
{
    apr_sockaddr_t *addr;
    long ipnum = 0;
    char *targetip = NULL;
    geo_db *geo = msr->txcfg->geo;
    char errstr[1024];
    unsigned char buf[2* GEO_MAX_RECORD_LEN];
    const int reclen = 3; /* Algorithm needs changed if this changes */
    apr_size_t nbytes;
    unsigned int rec_val = 0;
    apr_off_t seekto = 0;
    apr_status_t ret;
    int rc;
    int country = 0;
    int level;
    double dtmp;
    int itmp;

    *error_msg = NULL;

    /* init */
    georec->country_code = geo_country_code[0];
    georec->country_code3 = geo_country_code3[0];
    georec->country_name = geo_country_name[0];
    georec->country_continent = geo_country_continent[0];
    georec->region = "";
    georec->city = "";
    georec->postal_code = "";
    georec->latitude = 0;
    georec->longitude = 0;
    georec->dma_code = 0;
    georec->area_code = 0;

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "GEO: Looking up \"%s\".", log_escape(msr->mp, target));
    }

    /* NOTE: This only works with ipv4 */
    if ((rc = apr_sockaddr_info_get(&addr, target, APR_INET, 0, 0, msr->mp)) != APR_SUCCESS) {

        *error_msg = apr_psprintf(msr->mp, "Geo lookup for \"%s\" failed: %s", log_escape(msr->mp, target), apr_strerror(rc, errstr, 1024));
        msr_log(msr, 4, "%s", *error_msg);
        return 0;
    }
    if ((rc = apr_sockaddr_ip_get(&targetip, addr)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(msr->mp, "Geo lookup for \"%s\" failed: %s", log_escape(msr->mp, target), apr_strerror(rc, errstr, 1024));
        msr_log(msr, 4, "%s", *error_msg);
        return 0;
    };

    /* Why is this in host byte order? */
    ipnum = ntohl(addr->sa.sin.sin_addr.s_addr);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "GEO: Using address \"%s\" (0x%08lx). %lu", targetip, ipnum, ipnum);
    }

    ret = apr_global_mutex_lock(msr->modsecurity->geo_lock);
    if (ret != APR_SUCCESS) {
        msr_log(msr, 1, "Geo Lookup: Failed to lock proc mutex: %s",
                get_apr_error(msr->mp, ret));
    }

    for (level = 31; level >= 0; level--) {
        /* Read the record */
        seekto = 2 * reclen * rec_val;
        apr_file_seek(geo->db, APR_SET, &seekto);
        /* TODO: check rc */
        rc = apr_file_read_full(geo->db, &buf, (2 * reclen), &nbytes);

        /* NOTE: This is hard-coded for size 3 records */
        /* Left */
        if ((ipnum & (1 << level)) == 0) {
            rec_val =   (buf[3*0 + 0] << (0*8)) +
                        (buf[3*0 + 1] << (1*8)) +
                        (buf[3*0 + 2] << (2*8));
        }
        /* Right */
        else {
            rec_val =   (buf[3*1 + 0] << (0*8)) +
                        (buf[3*1 + 1] << (1*8)) +
                        (buf[3*1 + 2] << (2*8));
        }

        /* If we are past the country offset, then we are done */
        if (rec_val >= geo->ctry_offset) {
            break;
        }
    }

    if (rec_val == geo->ctry_offset) {
        *error_msg = apr_psprintf(msr->mp, "No geo data for \"%s\").", log_escape(msr->mp, target));
        msr_log(msr, 4, "%s", *error_msg);

        ret = apr_global_mutex_unlock(msr->modsecurity->geo_lock);
        if (ret != APR_SUCCESS) {
            msr_log(msr, 1, "Geo Lookup: Failed to lock proc mutex: %s",
                    get_apr_error(msr->mp, ret));
        }

        return 0;
    }

    if (geo->dbtype == GEO_COUNTRY_DATABASE) {
        country = rec_val;
        country -= geo->ctry_offset;
        if ((country <= 0) || (country > GEO_COUNTRY_LAST)) {
            *error_msg = apr_psprintf(msr->mp, "No geo data for \"%s\" (country %d).", log_escape(msr->mp, target), country);
            msr_log(msr, 4, "%s", *error_msg);

            ret = apr_global_mutex_unlock(msr->modsecurity->geo_lock);
            if (ret != APR_SUCCESS) {
                msr_log(msr, 1, "Geo Lookup: Failed to lock proc mutex: %s",
                        get_apr_error(msr->mp, ret));
            }

            return 0;
        }

        /* Country */
        georec->country_code = geo_country_code[country];
        georec->country_code3 = geo_country_code3[country];
        georec->country_name = geo_country_name[country];
        georec->country_continent = geo_country_continent[country];
    }
    else {
        int field_len = 0;
        int rec_offset = 0;
        int remaining = GEO_CITY_RECORD_LEN;
        unsigned char cbuf[GEO_CITY_RECORD_LEN];

        seekto = rec_val + (2 * reclen - 1) * geo->ctry_offset;
        apr_file_seek(geo->db, APR_SET, &seekto);
        /* TODO: check rc */
        rc = apr_file_read_full(geo->db, &cbuf, sizeof(cbuf), &nbytes);

        country = cbuf[0];
        if ((country <= 0) || (country > GEO_COUNTRY_LAST)) {
            *error_msg = apr_psprintf(msr->mp, "No geo data for \"%s\" (country %d).", log_escape(msr->mp, target), country);
            msr_log(msr, 4, "%s", *error_msg);

            ret = apr_global_mutex_unlock(msr->modsecurity->geo_lock);
            if (ret != APR_SUCCESS) {
                msr_log(msr, 1, "Geo Lookup: Failed to lock proc mutex: %s",
                        get_apr_error(msr->mp, ret));
            }

            return 0;
        }
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: rec=\"%s\"", log_escape_raw(msr->mp, cbuf, sizeof(cbuf)));
        }

        /* Country */
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: country=\"%.*s\"", (1*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf)));
        }
        georec->country_code = geo_country_code[country];
        georec->country_code3 = geo_country_code3[country];
        georec->country_name = geo_country_name[country];
        georec->country_continent = geo_country_continent[country];
        rec_offset++;
        remaining -= rec_offset;

        /* Region */
        field_len = field_length((const char *)cbuf+rec_offset, remaining);
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: region=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        georec->region = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;

        /* City */
        field_len = field_length((const char *)cbuf+rec_offset, remaining);
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: city=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        georec->city = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;

        /* Postal Code */
        field_len = field_length((const char *)cbuf+rec_offset, remaining);
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: postal_code=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        georec->postal_code = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;

        /* Latitude */
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: latitude=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        dtmp = cbuf[rec_offset] +
               (cbuf[rec_offset+1] << 8) +
               (cbuf[rec_offset+2] << 16);
        georec->latitude = dtmp/10000 - 180;
        rec_offset += 3;
        remaining -= 3;


        /* Longitude */
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: longitude=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        dtmp = cbuf[rec_offset] +
              (cbuf[rec_offset+1] << 8) +
              (cbuf[rec_offset+2] << 16);
        georec->longitude = dtmp/10000 - 180;
        rec_offset += 3;
        remaining -= 3;

        /* dma/area codes are in city rev1 and US only */
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "GEO: dma/area=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        }
        if (geo->dbtype == GEO_CITY_DATABASE_1
            && georec->country_code[0] == 'U'
            && georec->country_code[1] == 'S')
        {
            /* DMA Code */
            itmp = cbuf[rec_offset] +
                  (cbuf[rec_offset+1] << 8) +
                  (cbuf[rec_offset+2] << 16);
            georec->dma_code = itmp / 1000;
            georec->area_code = itmp % 1000;
            rec_offset += 6;
            remaining -= 6;
        }

    }

    *error_msg = apr_psprintf(msr->mp, "Geo lookup for \"%s\" succeeded.", log_escape(msr->mp, target));

    ret = apr_global_mutex_unlock(msr->modsecurity->geo_lock);
    if (ret != APR_SUCCESS) {
        msr_log(msr, 1, "Geo Lookup: Failed to lock proc mutex: %s",
                get_apr_error(msr->mp, ret));
    }

    return 1;
}


