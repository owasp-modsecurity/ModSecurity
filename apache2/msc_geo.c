/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2006 Thinking Stone (http://www.thinkingstone.com)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Thinking Stone at contact@thinkingstone.com.
 *
 */
#include "msc_geo.h"


/* -- Lookup Tables -- */

static const char *geo_country_code[] = {
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

static const char *geo_country_code3[] = {
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

static const char *geo_country_name[] = {
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

static const char *geo_country_continent[] = {
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


static int db_open(directory_config *dcfg, char **error_msg)
{
    char errstr[1024];
    apr_pool_t *mp = dcfg->mp;
    geo_db *geo = dcfg->geo;
    apr_status_t rc;
    apr_size_t nbytes;
    apr_off_t offset;
    unsigned char buf[3];
    int i, j;

    #ifdef DEBUG_CONF
    fprintf(stderr, "GEO: Initializing geo DB \"%s\".\n", geo->dbfn);
    #endif

    if ((rc = apr_file_open(&geo->db, geo->dbfn, APR_READ, APR_OS_DEFAULT, mp)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(mp, "Could not open geo database \"%s\": %s", geo->dbfn, apr_strerror(rc, errstr, 1024));
        return 0;
    }

    offset = -3;
    apr_file_seek(geo->db, APR_END, &offset);
    /* TODO check offset */

    /* Defaults */
    geo->dbtype = GEO_COUNTRY_DATABASE;
    geo->ctry_offset = GEO_COUNTRY_OFFSET;

    for (i = 0; i < GEO_STRUCT_INFO_MAX_SIZE; i++) {
        memset(buf, 0, 3);
        rc = apr_file_read_full(geo->db, &buf, 3, &nbytes);
        #ifdef DEBUG_CONF
        fprintf(stderr, "GEO: read 0x%02x%02x%02x\n", buf[0], buf[1], buf[2]);
        #endif
        if ((rc != APR_SUCCESS) || (nbytes != 3)) {
            *error_msg = apr_psprintf(mp, "Could not read from geo database \"%s\" (%d/3 bytes read): %s", geo->dbfn, nbytes, apr_strerror(rc, errstr, 1024));
            return -1;
        }
        if ((buf[0] == 0xff) && (buf[1] == 0xff) && (buf[2] == 0xff)) {
            #ifdef DEBUG_CONF
            fprintf(stderr, "GEO: Found DB info marker at offset 0x%08x\n", (unsigned int)offset);
            #endif
            memset(buf, 0, 3);
            rc = apr_file_read_full(geo->db, &buf, 1, &nbytes);
            /* TODO: check rc */
            geo->dbtype = (int)buf[0];

            /* Backwards compat */
            if (geo->dbtype >= 106) {
                geo->dbtype -= 105;
            }
            #ifdef DEBUG_CONF
            fprintf(stderr, "GEO: DB type %d\n", geo->dbtype);
            #endif

            /* If a cities DB, then get country offset */
            if ((geo->dbtype == GEO_CITY_DATABASE_0) || (geo->dbtype == GEO_CITY_DATABASE_1)) {
                memset(buf, 0, 3);
                rc = apr_file_read_full(geo->db, &buf, 3, &nbytes);
                if ((rc != APR_SUCCESS) || (nbytes != 3)) {
                    *error_msg = apr_psprintf(mp, "Could not read geo database \"%s\" country offset (%d/3 bytes read): %s", geo->dbfn, nbytes, apr_strerror(rc, errstr, 1024));
                    return -1;
                }
                #ifdef DEBUG_CONF
                fprintf(stderr, "GEO: read 0x%02x%02x%02x\n", buf[0], buf[1], buf[2]);
                #endif
                geo->ctry_offset = 0;
                for (j = 0; j < 3; j++) {
                    geo->ctry_offset += (buf[j] << (j * 8));
                }
            }

            #ifdef DEBUG_CONF
            fprintf(stderr, "GEO: Country offset 0x%08x\n", geo->ctry_offset);
            #endif

            return 1;
        }
        /* Backup a byte from where we started */
        offset = -4;
        apr_file_seek(geo->db, APR_CUR, &offset);
        #ifdef DEBUG_CONF
        fprintf(stderr, "GEO: DB offset 0x%08x\n", (unsigned int)offset);
        #endif
    }

    if (geo->dbtype != GEO_COUNTRY_DATABASE) {
        *error_msg = apr_psprintf(mp, "Unknown database format");
        return 0;
    }

    #ifdef DEBUG_CONF
    fprintf(stderr, "GEO: DB type %d\n", geo->dbtype);
    #endif

    return 1;
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

    msr_log(msr, 9, "GEO: Looking up \"%s\".", target);

    /* NOTE: This only works with ipv4 */
    if ((rc = apr_sockaddr_info_get(&addr, target, APR_INET, 0, 0, msr->mp)) != APR_SUCCESS) {
        
        *error_msg = apr_psprintf(msr->mp, "Geo lookup of \"%s\" failed: %s", target, apr_strerror(rc, errstr, 1024));
        return 0;
    }
    if ((rc = apr_sockaddr_ip_get(&targetip, addr)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(msr->mp, "Geo lookup of \"%s\" failed: %s", target, apr_strerror(rc, errstr, 1024));
        return 0;
    };

    /* Why is this in host byte order? */
    ipnum = ntohl(addr->sa.sin.sin_addr.s_addr);

    msr_log(msr, 9, "GEO: Using address \"%s\" (0x%08x).", targetip, ipnum);

    for (level = 31; level >= 0; level--) {

        /* Read the record */
        seekto = 2 * reclen * rec_val;
        apr_file_seek(geo->db, APR_SET, &seekto);
        /* TODO: check rc */
        rc = apr_file_read_full(geo->db, &buf, (2 * reclen), &nbytes);



        /* NOTE: This is hard-coded for size 3 records */
        /* Left */
        if ((ipnum & (1 << level)) == 0) {
            rec_val = buf[0] +
                     (buf[1] <<  8) +
                     (buf[2] << 16);
        }
        /* Right */
        else {
            rec_val = buf[3] +
                     (buf[4] <<  8) +
                     (buf[5] << 16);
        }

        /* If we are past the country offset, then we are done */
        if (rec_val >= geo->ctry_offset) {
            break;
        }
    }

    if (geo->dbtype == GEO_COUNTRY_DATABASE) {
        country = rec_val;
        country -= geo->ctry_offset;
        if (country <= 0) {
            *error_msg = apr_psprintf(msr->mp, "No geo data for \"%s\".", target);
            return 0;
        }
        msr_log(msr, 9, "GEO: rec=\"%s\"", log_escape_raw(msr->mp, buf, sizeof(buf)));

        /* Country */
        msr_log(msr, 9, "GEO: country=\"%.*s\"", (1*4), log_escape_raw(msr->mp, (unsigned char *)&rec_val, 1));
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
        if (country <= 0) {
            *error_msg = apr_psprintf(msr->mp, "No geo data for \"%s\".", target);
            return 0;
        }
        msr_log(msr, 9, "GEO: rec=\"%s\"", log_escape_raw(msr->mp, cbuf, sizeof(cbuf)));

        /* Country */
        msr_log(msr, 9, "GEO: country=\"%.*s\"", (1*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf)));
        georec->country_code = geo_country_code[country];
        georec->country_code3 = geo_country_code3[country];
        georec->country_name = geo_country_name[country];
        georec->country_continent = geo_country_continent[country];
        rec_offset++;
        remaining -= rec_offset;

        /* Region */
        field_len = strnlen((const char *)cbuf+rec_offset,remaining);
        msr_log(msr, 9, "GEO: region=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        georec->region = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;
        
        /* City */
        field_len = strnlen((const char *)cbuf+rec_offset,remaining);
        msr_log(msr, 9, "GEO: city=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        georec->city = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;
        
        /* Postal Code */
        field_len = strnlen((const char *)cbuf+rec_offset,remaining);
        msr_log(msr, 9, "GEO: postal_code=\"%.*s\"", ((field_len+1)*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        georec->postal_code = apr_pstrmemdup(msr->mp, (const char *)cbuf+rec_offset, (remaining));
        rec_offset += field_len + 1;
        remaining -= field_len + 1;

        /* Latitude */
        msr_log(msr, 9, "GEO: latitude=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        dtmp = cbuf[rec_offset] +
               (cbuf[rec_offset+1] << 8) +
               (cbuf[rec_offset+2] << 16);
        georec->latitude = dtmp/10000 - 180;
        rec_offset += 3;
        remaining -= 3;

        
        /* Longitude */
        msr_log(msr, 9, "GEO: longitude=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
        dtmp = cbuf[rec_offset] +
              (cbuf[rec_offset+1] << 8) +
              (cbuf[rec_offset+2] << 16);
        georec->longitude = dtmp/10000 - 180;
        rec_offset += 3;
        remaining -= 3;

        /* dma/area codes are in city rev1 and US only */
        msr_log(msr, 9, "GEO: dma/area=\"%.*s\"", (3*4), log_escape_raw(msr->mp, cbuf, sizeof(cbuf))+(rec_offset*4));
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

    *error_msg = apr_psprintf(msr->mp, "Geo lookup of \"%s\" succeeded.", target);
    return 1;
}

/**
 * Frees the resources used for Geo lookups
 */
apr_status_t geo_cleanup(modsec_rec *msr)
{
    return APR_SUCCESS;
}


