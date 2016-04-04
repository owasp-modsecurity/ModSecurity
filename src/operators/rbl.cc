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

#include "operators/rbl.h"

#include <modsecurity/rules.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#include "operators/operator.h"

namespace modsecurity {
namespace operators {


std::string Rbl::mapIpToAddress(std::string ipStr, Transaction *trans) {
    std::string addr;
    unsigned int h0, h1, h2, h3;
    std::string key = trans->m_rules->m_httpbl_key;

    if (sscanf(ipStr.c_str(), "%d.%d.%d.%d", &h0, &h1, &h2, &h3) != 4) {
        debug(trans, 0, std::string("Failed to understand `" + ipStr +
            "' as a valid IP address, assuming domain format input"));

        addr = ipStr + "." + m_service;
        return addr;
    }

    if (m_demandsPassword && key.empty()) {
        debug(trans, 0, std::string("Missing RBL key, cannot continue " \
            "with the operator execution, please set the key using: " \
            "SecHttpBlKey"));
        return addr;
    }

    addr = std::to_string(h3) + "." +
        std::to_string(h2) + "." +
        std::to_string(h1) + "." +
        std::to_string(h0) + "." +
        m_service;

    if (m_demandsPassword) {
        addr = key + "." + addr;
    }

    return addr;
}


void Rbl::futherInfo_httpbl(struct sockaddr_in *sin, std::string ipStr,
    Transaction *trans) {
    char *respBl;
    int first, days, score, type;
    std::string ptype;

    respBl = inet_ntoa(sin->sin_addr);

    if (sscanf(respBl, "%d.%d.%d.%d", &first, &days, &score, &type) != 4) {
        debug(trans, 4, "RBL lookup of " + ipStr + " failed: bad response");
        return;
    }

    if (first != 127) {
        debug(trans, 4, "RBL lookup of " + ipStr + " failed: bad response");
        return;
    }

    switch (type) {
        case 0:
            ptype = "Search Engine";
            break;
        case 1:
            ptype = "Suspicious IP";
            break;
        case 2:
            ptype = "Harvester IP";
            break;
        case 3:
            ptype = "Suspicious harvester IP";
            break;
        case 4:
            ptype = "Comment spammer IP";
            break;
        case 5:
            ptype = "Suspicious comment spammer IP";
            break;
        case 6:
            ptype = "Harvester and comment spammer IP";
            break;
        case 7:
            ptype = "Suspicious harvester comment spammer IP";
            break;
        default:
            ptype = " ";
    }

    debug(trans, 4, "RBL lookup of " + ipStr + " succeeded. %s: " \
        + std::to_string(days) + " " \
        "days since last activity, threat score " \
        + std::to_string(score) +  ". Case: " + ptype);
}


void Rbl::futherInfo_spamhaus(unsigned int high8bits, std::string ipStr,
    Transaction *trans) {
    switch (high8bits) {
        case 2:
        case 3:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded " \
                "(Static UBE sources).");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded " \
                "(Illegal 3rd party exploits).");
            break;
        case 10:
        case 11:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded " \
                "(Delivering unauthenticated SMTP email).");
            break;
        default:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded ");
            break;
    }
}


void Rbl::futherInfo_uribl(unsigned int high8bits, std::string ipStr,
    Transaction *trans) {
    switch (high8bits) {
        case 2:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded (BLACK).");
        break;
        case 4:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded (GREY).");
            break;
        case 8:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded (RED).");
            break;
        case 14:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded " \
                "(BLACK,GREY,RED).");
            break;
        case 255:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded " \
                "(DNS IS BLOCKED).");
            break;
        default:
            debug(trans, 4, "RBL lookup of " + ipStr + " succeeded (WHITE).");
            break;
    }
}


void Rbl::furtherInfo(struct sockaddr_in *sin, std::string ipStr,
    Transaction *trans) {
    unsigned int high8bits = sin->sin_addr.s_addr >> 24;

    switch (m_provider) {
        case RblProvider::UnknownProvider:
            debug(trans, 2, "RBL lookup of " + ipStr + " succeeded.");
            break;
        case RblProvider::httpbl:
            futherInfo_httpbl(sin, ipStr, trans);
            break;
        case RblProvider::uribl:
            futherInfo_uribl(high8bits, ipStr, trans);
            break;
        case RblProvider::spamhaus:
            futherInfo_spamhaus(high8bits, ipStr, trans);
            break;
    }
}


bool Rbl::evaluate(Transaction *transaction, const std::string &ipStr) {
    struct addrinfo *info = NULL;
    std::string host = mapIpToAddress(ipStr, transaction);
    int rc = 0;

    if (host.empty()) {
        return false;
    }

    rc = getaddrinfo(host.c_str(), NULL, NULL, &info);

    if (rc != 0) {
        if (info != NULL) {
            freeaddrinfo(info);
        }
        debug(transaction, 5, "RBL lookup of " + ipStr + " failed.");
        return false;
    }

    struct sockaddr *addr = info->ai_addr;
    struct sockaddr_in *sin = (struct sockaddr_in *) addr;
    furtherInfo(sin, ipStr, transaction);

    freeaddrinfo(info);

    return true;
}


}  // namespace operators
}  // namespace modsecurity
