/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "msc_status_engine.h"
#include "apr_sha1.h"

#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#endif

#ifdef DARWIN
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#if ! defined(IFT_ETHER)
#define IFT_ETHER 0x6/* Ethernet CSMACD */
#endif
#endif

#ifdef __gnu_linux__
#include <sys/utsname.h>
#include <linux/if.h>
#include <linux/sockios.h>
#endif

// Bese32 encode, based on:
// https://code.google.com/p/google-authenticator/source/browse/libpam/base32.c
void DSOLOCAL msc_status_engine_base32_encode(char *encoded,
    const char *data, int len) {
    int buffer;
    int count = 0;
    char *result = encoded;
    int length = strlen(data);

    buffer = data[0];

    if (length > 0) {
        int next = 1;
        int bitsLeft = 8;
        while (count < len && (bitsLeft > 0 || next < length)) {
            int index;
            if (bitsLeft < 5) {
                if (next < length) {
                    buffer <<= 8;
                    buffer |= data[next++] & 0xff;
                    bitsLeft += 8;
                } else {
                    int pad = 5 - bitsLeft;
                    buffer <<= pad;
                    bitsLeft += pad;
                }
            }
            index = 0x1f & (buffer >> (bitsLeft - 5));
            bitsLeft -= 5;
            result[count++] = msc_status_engine_basis_32[index];
        }
    }
    if (count < len) {
        result[count] = '\000';
    }
}

void DSOLOCAL msc_status_engine_fill_with_dots(char *encoded_with_dots,
    const char *data, int len, int space)
{
    int i;
    int count = 0;

    for (i = 0; i < strlen(data) && i < len; i++) {
        if (i % space == 0 && i != 0) {
            encoded_with_dots[count++] = '.';
        }
        encoded_with_dots[count++] = data[i];
    }
    encoded_with_dots[count] = '\0';
}


// Based on:
// http://stackoverflow.com/questions/16858782/how-to-obtain-almost-unique-system-identifier-in-a-cross-platform-way
int DSOLOCAL msc_status_engine_machine_name(char *machine_name, size_t len) {
#ifdef WIN32
    DWORD lenComputerName = len;
#endif

    memset(machine_name, '\0', sizeof(char) * len);

#ifdef WIN32
    GetComputerName(machine_name, &lenComputerName);
#else
   static struct utsname u;

   if ( uname( &u ) < 0 ) {
      goto failed;
   }

   apr_snprintf(machine_name, len-1, "%s", u.nodename);
#endif

    return 0;

failed:
    return -1;
}

int DSOLOCAL msc_status_engine_mac_address (unsigned char *mac)
{
#ifdef DARWIN
    struct ifaddrs* ifaphead;
    struct ifaddrs* ifap;
    int i = 0;

    if ( getifaddrs( &ifaphead ) != 0 ) {
        goto failed;
    }

    // iterate over the net interfaces
    for ( ifap = ifaphead; ifap; ifap = ifap->ifa_next ) {
        struct sockaddr_dl* sdl = (struct sockaddr_dl*)ifap->ifa_addr;
        if ( sdl && ( sdl->sdl_family == AF_LINK ) && ( sdl->sdl_type == IFT_ETHER )
                && mac[0] && mac[1] && mac[2] && i < 6) {
            for (i = 0; i<6; i++) {
                sprintf(mac, "%s%s%02x", mac, (i == 0)?"":":",
                        (unsigned char)LLADDR(sdl)[i]);
            }
        }
    }

    freeifaddrs( ifaphead );
#endif

#if __gnu_linux__
    struct ifconf conf;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP );
    struct ifreq* ifr;
    if ( sock < 0 ) {
        goto failed;
    }

    char ifconfbuf[ 128 * sizeof(struct ifreq) ];
    memset( ifconfbuf, 0, sizeof( ifconfbuf ));
    conf.ifc_buf = ifconfbuf;
    conf.ifc_len = sizeof( ifconfbuf );
    if ( ioctl( sock, SIOCGIFCONF, &conf )) {
        close(sock);
        goto failed;
    }

    for ( ifr = conf.ifc_req; ifr < conf.ifc_req + conf.ifc_len; ifr++ ) {
        if ( ioctl( sock, SIOCGIFFLAGS, ifr )) {
            continue;  // failed to get flags, skip it
        }

        if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 ) {
            int i = 0;
            if (!ifr->ifr_addr.sa_data[0] && !ifr->ifr_addr.sa_data[1]
                && !ifr->ifr_addr.sa_data[2]) {
                continue;
            }

            for (i = 0; i<6; i++) {
                sprintf(mac, "%s%s%02x", mac, (i == 0)?"":":",
                    (unsigned char)ifr->ifr_addr.sa_data[i]);
            }
        }
    }
    close( sock );
#endif

#if WIN32
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    UINT i;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof (IP_ADAPTER_INFO));
    if (!pAdapterInfo) {
        goto failed;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (!pAdapterInfo) {
            goto failed;
        }
    }

    dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    if (dwRetVal != NO_ERROR) {
        free(pAdapterInfo);
        goto failed;
    }

    pAdapter = pAdapterInfo;
    while (pAdapter && !mac[0] && !mac[1] && !mac[2])
    {
        if (pAdapter->AddressLength > 4)
        {
            for (i = 0; i < pAdapter->AddressLength && i < 6; i++)
            {
                sprintf(mac, "%s%s%02x", mac, (i == 0) ? "" : ":",
                (unsigned char)pAdapter->Address[i]);
            }
        }
        pAdapter = pAdapter->Next;
    }

    free(pAdapterInfo);
#endif

done:
    return 0;

failed:
    return -1;
}

int DSOLOCAL msc_status_engine_unique_id (unsigned char *digest)
{
    unsigned char local_digest[APR_SHA1_DIGESTSIZE];
    apr_sha1_ctx_t context;
    char *input;
    int i = 0;
    unsigned char *mac_address = NULL;
    char *machine_name = NULL;
    int ret = 0;

    mac_address = malloc(sizeof(char)*(6+5+1));
    if (!mac_address) {
        ret = -1;
        goto failed_mac_address;
    }
    memset(mac_address, 0, sizeof(char)*(6+5+1));

    if (msc_status_engine_mac_address(mac_address)) {
        ret = -1;
        goto failed_set_mac_address;
    }

    machine_name = malloc(sizeof(char)*201);
    if (!machine_name) {
        ret = -1;
        goto failed_machine_name;
    }

    if (msc_status_engine_machine_name(machine_name, 200)) {
        ret = -1;
        goto failed_set_machine_name;
    }

    input = malloc(sizeof(char)*(strlen(machine_name) +
        strlen(mac_address)+1));
    if (!input) {
        goto failed_input;
    }

    apr_snprintf(input, strlen(machine_name)+strlen(mac_address)+1,
        "%s%s", machine_name, mac_address);

    apr_sha1_init(&context);
    apr_sha1_update(&context, input, strlen(input));
    apr_sha1_final(local_digest, &context);

    for (i = 0; i < APR_SHA1_DIGESTSIZE; i++) {
        sprintf(digest, "%s%02x", digest, local_digest[i]);
    }

    free(input);
failed_input:
failed_set_machine_name:
    free(machine_name);
failed_machine_name:
failed_set_mac_address:
    free(mac_address);
failed_mac_address:

    return ret;
}

int msc_status_engine_call (void) {
    char *apr = NULL;
    const char *apr_loaded = NULL;
    char pcre[7];
    const char *pcre_loaded = NULL;
    char *lua = NULL;
    char *lua_loaded = NULL;
    char *libxml = NULL;
    char *libxml_loaded = NULL;
    char *modsec = NULL;
    const char *apache = NULL;
    char *id = NULL;
    char *beacon_string = NULL;
    int beacon_string_len = 0;
    char *beacon_string_encoded = NULL;
    char *beacon_string_ready = NULL;
    char *beacon_string_encoded_splitted = NULL;
    int ret = 0;

    apr = APR_VERSION_STRING;
    apr_loaded = apr_version_string();

    pcre_loaded = pcre_version();
    apr_snprintf(pcre, 7, "%2d.%2d ", PCRE_MAJOR, PCRE_MINOR);
#ifdef WITH_LUA
    lua = LUA_VERSION;
#endif
    lua_loaded = 0;

    libxml = LIBXML_DOTTED_VERSION;
    libxml_loaded = 0;

    modsec = MODSEC_VERSION;
    apache = apache_get_server_version();

    id = malloc(sizeof(char)*((2*APR_SHA1_DIGESTSIZE)+1));
    if (!id) {
        ret = -1;
        goto failed_id;
    }

    memset(id, '\0', sizeof(char)*((2*APR_SHA1_DIGESTSIZE)+1));
    if (msc_status_engine_unique_id(id)) {
        sprintf(id, "unique id failed");
    }

    beacon_string_len = (modsec ? strlen(modsec) : 6) + (apache ? strlen(apache) : 6) +
        (apr ? strlen(apr) : 6) + (apr_loaded ? strlen(apr_loaded) : 6) +
        (pcre ? strlen(pcre) : 6) + (pcre_loaded ? strlen(pcre_loaded) : 6) +
        (lua ? strlen(lua) : 6) + (lua_loaded ? strlen(lua_loaded) : 6) +
        (libxml ? strlen(libxml) : 6) + (libxml_loaded ? strlen(libxml_loaded) : 6) +
        (id ? strlen(id) : 6);

#ifdef WIN32
    beacon_string = malloc(sizeof(char)*(beacon_string_len+1+10+4));
    if (!beacon_string) {
        goto failed_beacon_string;
    }

    apr_snprintf(beacon_string, beacon_string_len+1+10+4,
        "%s,%s/IIS,%s/%s,%s/%s,%s/%s,%s/%s,%s",
        modsec, apache, apr, apr_loaded, pcre, pcre_loaded, lua, lua_loaded,
        libxml, libxml_loaded, id);
#else
    beacon_string = malloc(sizeof(char)*(beacon_string_len+1+10));
    if (!beacon_string) {
        goto failed_beacon_string;
    }

    apr_snprintf(beacon_string, beacon_string_len+1+10,
        "%s,%s,%s/%s,%s/%s,%s/%s,%s/%s,%s",
        modsec, apache, apr, apr_loaded, pcre, pcre_loaded, lua, lua_loaded,
        libxml, libxml_loaded, id);
#endif

    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
        "ModSecurity: StatusEngine call: \"%s\"", beacon_string);

    beacon_string_encoded = malloc(sizeof(char)*(strlen(beacon_string)*3));
    if (!beacon_string_encoded) {
        goto failed_beacon_string_encoded;
    }
    msc_status_engine_base32_encode(beacon_string_encoded, beacon_string,
        strlen(beacon_string)*3);

    beacon_string_encoded_splitted = malloc (sizeof(char) *
        ((strlen(beacon_string_encoded)/STATUS_ENGINE_DNS_IN_BETWEEN_DOTS) +
         strlen(beacon_string_encoded) + 1 + 1));
    if (!beacon_string_encoded_splitted) {
        goto failed_beacon_string_encoded_splitted;
    }

    msc_status_engine_fill_with_dots(beacon_string_encoded_splitted,
        beacon_string_encoded,
        (strlen(beacon_string_encoded)/STATUS_ENGINE_DNS_IN_BETWEEN_DOTS) +
                     strlen(beacon_string_encoded) + 1 + 1,
                     STATUS_ENGINE_DNS_IN_BETWEEN_DOTS);

    beacon_string_ready = malloc (sizeof(char) *
        strlen(beacon_string_encoded_splitted) +
        strlen(STATUS_ENGINE_DNS_SUFFIX) + 10 + 1);

    if (!beacon_string_ready) {
        goto failed_beacon_string_ready;
    }

    apr_snprintf(beacon_string_ready, strlen(beacon_string_encoded_splitted) +
        strlen(STATUS_ENGINE_DNS_SUFFIX) + 12 + 1, "%s.%lu.%s",
        beacon_string_encoded_splitted, time(NULL),
        STATUS_ENGINE_DNS_SUFFIX);

    if (gethostbyname(beacon_string_ready)) {
        ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: StatusEngine call successfully sent. For more " \
            "information visit: http://%s/", STATUS_ENGINE_DNS_SUFFIX);
    } else {
        ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: StatusEngine call failed. Query: %s",
            beacon_string_ready);
    }

    free(beacon_string_ready);
failed_beacon_string_ready:
    free(beacon_string_encoded_splitted);
failed_beacon_string_encoded_splitted:
    free(beacon_string_encoded);
failed_beacon_string_encoded:
    free(beacon_string);
failed_beacon_string:
    free(id);
failed_id:

    return ret;
}

