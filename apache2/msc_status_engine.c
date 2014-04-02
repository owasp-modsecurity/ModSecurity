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
#include "modsecurity_config.h"

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
#include <sys/types.h>
#include <sys/socket.h>
#ifndef IFT_ETHER
#define IFT_ETHER 0x6 /* Ethernet CSMACD */
#endif
#endif

#if (defined(__linux__) || defined(__gnu_linux__))
#include <linux/if.h>
#include <linux/sockios.h>
#endif
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

// Bese32 encode, based on:
// https://code.google.com/p/google-authenticator/source/browse/libpam/base32.c
int DSOLOCAL msc_status_engine_base32_encode(char *encoded,
    const char *data, int len) {
    int buffer;
    int count = 0;
    char *result = encoded;
    int length = strlen(data);

    buffer = data[0];

    if (encoded == NULL && len == 0) {
        len = length * 3;
        count++;
    }

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
            if (encoded != NULL) {
                result[count] = msc_status_engine_basis_32[index];
            }
            count++;
        }
    }
    if (count < len && encoded != NULL) {
        result[count] = '\000';
    }

    return count;
}

int DSOLOCAL msc_status_engine_fill_with_dots(char *encoded_with_dots,
    const char *data, int len, int space)
{
    int i;
    int count = 0;

    if (encoded_with_dots == NULL) {
        if (len == 0 && data != NULL) {
            len = strlen(data);
        }
        else if (len == 0 && data == NULL) {
            count = -1;
            goto return_length;
        }

        count = len/space + len + 1;
        goto return_length;
    }

    for (i = 0; i < strlen(data) && i < len; i++) {
        if (i % space == 0 && i != 0) {
            encoded_with_dots[count++] = '.';
        }
        encoded_with_dots[count++] = data[i];
    }
    encoded_with_dots[count] = '\0';

return_length:
    return count;
}


// Based on:
// http://stackoverflow.com/questions/16858782/how-to-obtain-almost-unique-system-identifier-in-a-cross-platform-way
int DSOLOCAL msc_status_engine_machine_name(char *machine_name, size_t len) {
#ifdef WIN32
    DWORD lenComputerName = len;
#endif

    memset(machine_name, '\0', sizeof(char) * len);

#ifdef WIN32
    if (GetComputerName(machine_name, &lenComputerName) == 0) {
        goto failed;
    }
#endif

#ifdef HAVE_SYS_UTSNAME_H
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

            apr_snprintf(mac, MAC_ADDRESS_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)LLADDR(sdl)[0],
                (unsigned char)LLADDR(sdl)[1],
                (unsigned char)LLADDR(sdl)[2],
                (unsigned char)LLADDR(sdl)[3],
                (unsigned char)LLADDR(sdl)[4],
                (unsigned char)LLADDR(sdl)[5]);
            goto end;
        }
    }

    freeifaddrs( ifaphead );
#endif

#if (defined(__linux__) || defined(__gnu_linux__))
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

            apr_snprintf(mac, MAC_ADDRESS_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)ifr->ifr_addr.sa_data[0],
                (unsigned char)ifr->ifr_addr.sa_data[1],
                (unsigned char)ifr->ifr_addr.sa_data[2],
                (unsigned char)ifr->ifr_addr.sa_data[3],
                (unsigned char)ifr->ifr_addr.sa_data[4],
                (unsigned char)ifr->ifr_addr.sa_data[5]);

            goto end;
        }
    }
    close( sock );
#endif

#if WIN32
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

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
            apr_snprintf(mac, MAC_ADDRESS_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)pAdapter->Address[0],
                (unsigned char)pAdapter->Address[1],
                (unsigned char)pAdapter->Address[2],
                (unsigned char)pAdapter->Address[3],
                (unsigned char)pAdapter->Address[4],
                (unsigned char)pAdapter->Address[5]);
            goto end;
        }
        pAdapter = pAdapter->Next;
    }

    free(pAdapterInfo);
#endif

end:
    return 0;
failed:
    return -1;
}

int DSOLOCAL msc_status_engine_unique_id (unsigned char *digest)
{
    unsigned char hex_digest[APR_SHA1_DIGESTSIZE];
    unsigned char *mac_address = NULL;
    char *machine_name = NULL;
    int ret = 0;
    int i = 0;
    apr_sha1_ctx_t context;

    mac_address = malloc(sizeof(char)*(MAC_ADDRESS_SIZE));
    if (!mac_address) {
        ret = -1;
        goto failed_mac_address;
    }
    memset(mac_address, '\0', sizeof(char)*(MAC_ADDRESS_SIZE));

    if (msc_status_engine_mac_address(mac_address)) {
        ret = -1;
        goto failed_set_mac_address;
    }

    machine_name = malloc(sizeof(char)*MAX_MACHINE_NAME_SIZE);
    if (!machine_name) {
        ret = -1;
        goto failed_machine_name;
    }
    memset(machine_name, '\0', sizeof(char)*(MAX_MACHINE_NAME_SIZE));
    if (msc_status_engine_machine_name(machine_name, MAC_ADDRESS_SIZE)) {
        ret = -1;
        goto failed_set_machine_name;
    }

    apr_sha1_init(&context);
    apr_sha1_update(&context, machine_name, strlen(machine_name));
    apr_sha1_update(&context, mac_address, strlen(mac_address));
    apr_sha1_final(hex_digest, &context);

    for (i = 0; i < APR_SHA1_DIGESTSIZE; i++)
    {
        sprintf(digest, "%s%02x", digest, hex_digest[i]);
    }

failed_set_machine_name:
    free(machine_name);
failed_machine_name:
failed_set_mac_address:
    free(mac_address);
failed_mac_address:
    return ret;
}

int DSOLOCAL msc_beacon_string (char *beacon_string, int beacon_string_max_len) {
    char *apr = NULL;
    const char *apr_loaded = NULL;
    char pcre[7];
    const char *pcre_loaded = NULL;
    char *lua = NULL;
    char *libxml = NULL;
    char *modsec = NULL;
    const char *apache = NULL;
    char id[(APR_SHA1_DIGESTSIZE*2) + 1];
    int beacon_string_len = -1;

    apr = APR_VERSION_STRING;
    apr_loaded = apr_version_string();
    apr_snprintf(pcre, 7, "%d.%d", PCRE_MAJOR, PCRE_MINOR);
    pcre_loaded = pcre_version();
#ifdef WITH_LUA
    lua = LUA_VERSION;
#endif
    libxml = LIBXML_DOTTED_VERSION;
    modsec = MODSEC_VERSION;
#ifdef WIN32
    apache = "IIS";
#else
    apache = apache_get_server_version();
#endif

    /* 6 represents: strlen("(null)") */
    beacon_string_len = (modsec ? strlen(modsec) : 6) +
        (apache ? strlen(apache) : 6) + (apr ? strlen(apr) : 6) +
        (apr_loaded ? strlen(apr_loaded) : 6) + (pcre ? strlen(pcre) : 6) +
        (pcre_loaded ? strlen(pcre_loaded) : 6) + (lua ? strlen(lua) : 6) +
        (libxml ? strlen(libxml) : 6) + (APR_SHA1_DIGESTSIZE * 2);

    beacon_string_len = beacon_string_len + /* null terminator: */ 1 +
            /* comma: */ 6 +
            /* slash: */ 2;

    if (beacon_string == NULL || beacon_string_max_len == 0) {
        goto return_length;
    }

    memset(id, '\0', sizeof(id));
    if (msc_status_engine_unique_id(id)) {
        sprintf(id, "no unique id");
    }

    apr_snprintf(beacon_string, beacon_string_max_len,
        "%.25s,%s,%s/%s,%s/%s,%s,%s,%s",
        modsec, apache, apr, apr_loaded, pcre, pcre_loaded, lua, libxml, id);

return_length:
    return beacon_string_len;
}

int DSOLOCAL msc_status_engine_prepare_hostname (char *hostname, const char *plain_data,
        int max_length)
{
    int str_enc_len = 0;
    int str_enc_spl_len = 0;
    char *tmp = NULL;
    int length = -1;
    time_t ltime;

    str_enc_len = msc_status_engine_base32_encode(NULL, plain_data, 0);

    str_enc_spl_len = msc_status_engine_fill_with_dots(NULL, NULL, str_enc_len,
            STATUS_ENGINE_DNS_IN_BETWEEN_DOTS);
    if (str_enc_spl_len < 0) {
        goto failed_enc_spl_len;
    }

    length = str_enc_spl_len + strlen(STATUS_ENGINE_DNS_SUFFIX) +
        /* epoch: */ 10 + /* dots: */ 2 + /* terminator: */ 1 -
        /* removed unsed terminators from str_enc and str_enc_spl: */ 2;

    if (hostname == NULL || max_length == 0) {
        goto return_length;
    }

    memset(hostname, '\0', sizeof(char) * max_length);

    msc_status_engine_base32_encode(hostname, plain_data, str_enc_len);

    tmp = strdup(hostname);
    if (tmp == NULL) {
        length = -1;
        goto failed_strdup;
    }

    str_enc_spl_len = msc_status_engine_fill_with_dots(hostname, tmp, max_length,
        STATUS_ENGINE_DNS_IN_BETWEEN_DOTS);
    if (str_enc_spl_len < 0) {
        length = -1;
        goto failed_enc_spl;
    }

    time ( &ltime );
    apr_snprintf(hostname, max_length, "%s.%ld.%s", hostname,
            (long) ltime, STATUS_ENGINE_DNS_SUFFIX);

failed_enc_spl:
    free(tmp);
failed_strdup:
return_length:
failed_enc_spl_len:
    return length;
}

int msc_status_engine_call (void) {
    char *beacon_str = NULL;
    int beacon_str_len = 0;
    char *hostname = NULL;
    int hostname_len = 0;
    int ret = -1;

    /* Retrieve the beacon string */
    beacon_str_len = msc_beacon_string(NULL, 0);

    beacon_str = malloc(sizeof(char) * beacon_str_len);
    if (beacon_str == NULL) {
        goto failed_beacon_string_malloc;
    }
    msc_beacon_string(beacon_str, beacon_str_len);

    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: StatusEngine call: \"%s\"", beacon_str);

    /* Get beacon string in the format of a hostname */
    hostname_len = msc_status_engine_prepare_hostname(NULL, beacon_str, 0);
    if (hostname_len < 0) {
        goto failed_hostname_len;
    }

    hostname = malloc(sizeof(char) * hostname_len);
    if (hostname == NULL) {
        goto failed_hostname_malloc;
    }
    hostname_len = msc_status_engine_prepare_hostname(hostname, beacon_str,
            hostname_len);
    if (hostname_len < 0) {
        goto failed_hostname;
    }

    /* Perform the DNS query. */
    if (gethostbyname(hostname)) {
        ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: StatusEngine call successfully sent. For more " \
            "information visit: http://%s/", STATUS_ENGINE_DNS_SUFFIX);
    } else {
        ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: StatusEngine call failed. Query: %s",
            hostname);
    }

    ret = 0;

failed_hostname:
    free(hostname);
failed_hostname_malloc:
failed_hostname_len:
    free(beacon_str);
failed_beacon_string_malloc:

    return ret;
}

