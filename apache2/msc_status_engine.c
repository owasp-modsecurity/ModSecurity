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
#else
#include <sys/ioctl.h>
#include <netdb.h>
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

//MST
#ifdef APLOG_USE_MODULE
APLOG_USE_MODULE(security2);
#endif

// Bese32 encode, based on:
// https://code.google.com/p/google-authenticator/source/browse/libpam/base32.c
int DSOLOCAL msc_status_engine_base32_encode(char* encoded,
    const char* data, int len) {
    int buffer;
    int count = 0;
    char* result = encoded;
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
                }
                else {
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

int DSOLOCAL msc_status_engine_fill_with_dots(char* encoded_with_dots,
    const char* data, int len, int space)
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

        count = len / space + len + 1;
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
int DSOLOCAL msc_status_engine_machine_name(char* machine_name, size_t len) {
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

    if (uname(&u) < 0) {
        goto failed;
    }

    apr_snprintf(machine_name, len - 1, "%s", u.nodename);
#endif

    return 0;

failed:
    return -1;
}

int DSOLOCAL msc_status_engine_mac_address(unsigned char* mac)
{
#ifdef DARWIN
    struct ifaddrs* ifaphead;
    struct ifaddrs* ifap;
    int i = 0;

    if (getifaddrs(&ifaphead) != 0) {
        goto failed;
    }

    // iterate over the net interfaces
    for (ifap = ifaphead; ifap; ifap = ifap->ifa_next) {
        struct sockaddr_dl* sdl = (struct sockaddr_dl*)ifap->ifa_addr;
        if (sdl && (sdl->sdl_family == AF_LINK) && (sdl->sdl_type == IFT_ETHER)
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

    freeifaddrs(ifaphead);
#endif

#if (defined(__linux__) || defined(__gnu_linux__))
    struct ifconf conf;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    struct ifreq* ifr;
    if (sock < 0) {
        goto failed;
    }

    char ifconfbuf[128 * sizeof(struct ifreq)];
    memset(ifconfbuf, 0, sizeof(ifconfbuf));
    conf.ifc_buf = ifconfbuf;
    conf.ifc_len = sizeof(ifconfbuf);
    if (ioctl(sock, SIOCGIFCONF, &conf)) {
        close(sock);
        goto failed;
    }

    for (ifr = conf.ifc_req; ifr < conf.ifc_req + conf.ifc_len; ifr++) {
        if (ioctl(sock, SIOCGIFFLAGS, ifr)) {
            continue;  // failed to get flags, skip it
        }

        if (ioctl(sock, SIOCGIFHWADDR, ifr) == 0) {
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
    close(sock);
#endif

#if WIN32
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if (!pAdapterInfo) {
        goto failed;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
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

int DSOLOCAL msc_status_engine_unique_id(unsigned char* digest)
{
    unsigned char hex_digest[APR_SHA1_DIGESTSIZE];
    unsigned char* mac_address = NULL;
    char* machine_name = NULL;
    int ret = 0;
    int i = 0;
    apr_sha1_ctx_t context;

    mac_address = malloc(sizeof(char) * (MAC_ADDRESS_SIZE));
    if (!mac_address) {
        ret = -1;
        goto failed_mac_address;
    }
    memset(mac_address, '\0', sizeof(char) * (MAC_ADDRESS_SIZE));

    if (msc_status_engine_mac_address(mac_address)) {
        ret = -1;
        goto failed_set_mac_address;
    }

    machine_name = malloc(sizeof(char) * MAX_MACHINE_NAME_SIZE);
    if (!machine_name) {
        ret = -1;
        goto failed_machine_name;
    }
    memset(machine_name, '\0', sizeof(char) * (MAX_MACHINE_NAME_SIZE));
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
