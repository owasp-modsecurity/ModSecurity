/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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


#include "modsecurity/modsecurity.h"
#include "src/config.h"

#ifdef WITH_LIBXML2
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>
#endif
#ifdef MSC_WITH_CURL
#include <curl/curl.h>
#endif


#include <ctime>
#include <iostream>

#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"
#include "src/collection/backend/in_memory-per_process.h"
#include "src/collection/backend/lmdb.h"
#include "src/unique_id.h"
#include "src/utils/json_writer.h"
#include "src/utils/regex.h"
#include "src/utils/geo_lookup.h"
#include "src/actions/transformations/transformation.h"

namespace modsecurity {

/**
 * @name    ModSecurity
 * @brief   Initilizes ModSecurity CPP API
 *
 * ModSecurity initializer.
 *
 * Example Usage:
 * @code
 *
 * using ModSecurity::ModSecurity;
 *
 * ModSecurity *msc = new ModSecurity();
 *
 * @endcode
 */
ModSecurity::ModSecurity()
    :
#ifdef WITH_LMDB
    m_global_collection(new collection::backend::LMDB("GLOBAL")),
    m_resource_collection(new collection::backend::LMDB("RESOURCE")),
    m_ip_collection(new collection::backend::LMDB("IP")),
    m_session_collection(new collection::backend::LMDB("SESSION")),
    m_user_collection(new collection::backend::LMDB("USER")),
#else
    m_global_collection(new collection::backend::InMemoryPerProcess("GLOBAL")),
    m_resource_collection(
        new collection::backend::InMemoryPerProcess("RESOURCE")),
    m_ip_collection(new collection::backend::InMemoryPerProcess("IP")),
    m_session_collection(
        new collection::backend::InMemoryPerProcess("SESSION")),
    m_user_collection(new collection::backend::InMemoryPerProcess("USER")),
#endif
    m_connector(""),
    m_whoami(""),
    m_logCb(NULL),
    m_logProperties(0) {
    UniqueId::uniqueId();
    srand(time(NULL));
#ifdef MSC_WITH_CURL
    curl_global_init(CURL_GLOBAL_ALL);
#endif
#ifdef WITH_LIBXML2
    xmlInitParser();
#endif
}


ModSecurity::~ModSecurity() {
#ifdef MSC_WITH_CURL
    curl_global_cleanup();
#endif
#ifdef WITH_GEOIP
    Utils::GeoLookup::getInstance().cleanUp();
#endif
#ifdef WITH_LIBXML2
    xmlCleanupParser();
#endif
    delete m_global_collection;
    delete m_resource_collection;
    delete m_ip_collection;
    delete m_session_collection;
    delete m_user_collection;
}


/**
 * @name    whoAmI
 * @brief   Return information about this ModSecurity version and platform.
 *
 * Platform and version are two questions that community will ask prior to
 * provide support. Making it available internally and to the connector as
 * well.
 *
 * @note This information maybe will be used by a log parser. If you want to
 *       update it, make it in a fashion that won't break the existent parsers.
 *       (e.g. adding extra information _only_ to the end of the string)
 */
const std::string& ModSecurity::whoAmI() {
    std::string platform("Unknown platform");

#if AIX
    platform = "AIX";
#elif LINUX
    platform = "Linux";
#elif OPENBSD
    platform = "OpenBSD";
#elif SOLARIS
    platform = "Solaris";
#elif HPUX
    platform = "HPUX";
#elif MACOSX
    platform = "MacOSX";
#elif FREEBSD
    platform = "FreeBSD";
#elif DRAGONFLY
    platform = "DragonFlyBSD";
#elif NETBSD
    platform = "NetBSD";
#elif WIN32
    platform = "Windows";
#endif

    if (m_whoami.empty()) {
        m_whoami = "ModSecurity v" MODSECURITY_VERSION " (" + platform + ")";
    }

    return m_whoami;
}


/**
 * @name    setConnectorInformation
 * @brief   Set information about the connector that is using the library.
 *
 * For the purpose of log it is necessary for modsecurity to understand which
 * 'connector' is consuming the API.
 *
 * @note It is strongly recommended to set a information in the following
 *       pattern:
 *
 *       ConnectorName vX.Y.Z-tag (something else)
 *
 *       For instance: ModSecurity-nginx v0.0.1-alpha (Whee)
 *
 * @param connector Information about the connector.
 *
 */
void ModSecurity::setConnectorInformation(const std::string &connector) {
    m_connector = connector;
}


/**
 * @name    getConnectorInformation
 * @brief   Returns the connector information.
 *
 * Returns whatever was set by 'setConnectorInformation'. Check
 * setConnectorInformation documentation to understand the expected format.
 *
 * @retval "" Nothing was informed about the connector.
 * @retval !="" Connector information.
 */
const std::string& ModSecurity::getConnectorInformation() const {
    return m_connector;
}

void ModSecurity::serverLog(void *data, const RuleMessage &rm) {
    if (m_logCb == NULL) {
        std::cerr << "Server log callback is not set -- " << rm.errorLog();
        std::cerr << std::endl;
        return;
    }

    if (m_logProperties & TextLogProperty) {
        auto d = rm.log();
        auto a = static_cast<const void *>(d.c_str());
        m_logCb(data, a);
        return;
    }

    if (m_logProperties & RuleMessageLogProperty) {
        auto a = static_cast<const void *>(&rm);
        m_logCb(data, a);
        return;
    }
}


int ModSecurity::processContentOffset(const char *content, size_t len,
    const char *matchString, std::string *json, const char **err) {
    Utils::Regex variables("v([0-9]+),([0-9]+)");
    Utils::Regex operators("o([0-9]+),([0-9]+)");
    Utils::Regex transformations("t:(?:(?!t:).)+");
    std::string varValue;
    utils::JsonWriter writer(false);

    std::list<Utils::SMatch> vars = variables.searchAll(matchString);
    std::list<Utils::SMatch> ops = operators.searchAll(matchString);
    std::list<Utils::SMatch> trans = transformations.searchAll(matchString);

    writer.start_object();
    writer.key("match");
    writer.start_array();
    writer.start_object();
    writer.key("variable");
    writer.start_object();
    writer.key("highlight");
    writer.start_array();
    for(auto [it, pending] = std::tuple{vars.rbegin(), vars.size()}; pending > 3; pending -= 3) {
        writer.start_object();
        it++;
        const std::string &startingAt = it->str(); it++;
        const std::string &size = it->str(); it++;
        writer.key("startingAt");
        writer.string(startingAt);
        writer.key("size");
        writer.string(size);
        writer.end_object();

        if (stoi(startingAt) >= len) {
            *err = "Offset is out of the content limits.";
            return -1;
        }

        const auto value = std::string(content, stoi(startingAt), stoi(size));
        if (varValue.size() > 0) {
            varValue.append(" " + value);
        } else {
            varValue.append(value);
        }
    }
    writer.end_array();

    writer.key("value");
    writer.start_array();

    writer.start_object();
    writer.key("value");
    writer.string(varValue);
    writer.end_object();

    while (!trans.empty()) {
        modsecurity::actions::transformations::Transformation *t;
        writer.start_object();
        writer.key("transformation");
        writer.string(trans.back().str());

        t = modsecurity::actions::transformations::Transformation::instantiate(
            trans.back().str().c_str());
        t->transform(varValue, nullptr);
        trans.pop_back();

        writer.key("value");
        writer.string(varValue);
        writer.end_object();

        delete t;
    }

    writer.end_array();

    writer.key("operator");
    writer.start_object();

    for(auto [it, pending] = std::tuple{ops.rbegin(), ops.size()}; pending > 3; pending -= 3) {
        writer.key("highlight");
        writer.start_object();
        it++;
        const std::string &startingAt = it->str(); it++;
        const std::string &size = ops.back().str(); it++;
        writer.key("startingAt");
        writer.string(startingAt);
        writer.key("size");
        writer.string(size);
        writer.end_object();

        if (stoi(startingAt) >= varValue.size()) {
            *err = "Offset is out of the variable limits.";
            return -1;
        }

        const auto value = std::string(varValue, stoi(startingAt), stoi(size));

        writer.key("value");
        writer.string(value);
    }

    writer.end_object();
    writer.end_object();
    writer.end_array();
    writer.end_object();

    json->assign(writer.to_string());
    json->append("\n");
    return 0;
}


void ModSecurity::setServerLogCb(ModSecLogCb cb) {
    setServerLogCb(cb, TextLogProperty);
}


void ModSecurity::setServerLogCb(ModSecLogCb cb, int properties) {
    m_logCb = (ModSecLogCb) cb;
    m_logProperties = properties;
}

/**
 * @name    msc_set_log_cb
 * @brief   Set the log callback functiond
 *
 * It is neccessary to indicate to libModSecurity which function within the
 * connector should be called when logging is required.
 *
 * @oarm msc The current ModSecurity instance
 * @param ModSecLogCb The callback function to which a reference to the log msgs 
 * will be passed.
 *
 */
extern "C" void msc_set_log_cb(ModSecurity *msc, ModSecLogCb cb) {
    msc->setServerLogCb(cb);
}

/**
 * @name    msc_set_connector_info
 * @brief   Set information about the connector that is using the library.
 *
 * For the purpose of log it is necessary for modsecurity to understand which
 * 'connector' is consuming the API.
 *
 * @note It is strongly recommended to set a information in the following
 *       pattern:
 *
 *       ConnectorName vX.Y.Z-tag (something else)
 *
 *       For instance: ModSecurity-nginx v0.0.1-alpha
 *
 * @param connector Information about the connector.
 *
 */
extern "C" void msc_set_connector_info(ModSecurity *msc,
    const char *connector) {
    msc->setConnectorInformation(std::string(connector));
}


/**
 * @name    msc_who_am_i
 * @brief   Return information about this ModSecurity version and platform.
 *
 * Platform and version are two questions that community will ask prior to
 * provide support. Making it available internally and to the connector as
 * well.
 *
 * @note This information maybe will be used by a log parser. If you want to
 *       update it, make it in a fashion that won't break the existent parsers.
 *       (e.g. adding extra information _only_ to the end of the string)
 */
extern "C" const char *msc_who_am_i(ModSecurity *msc) {
    return msc->whoAmI().c_str();
}


/**
 * @name    msc_cleanup
 * @brief   Cleanup ModSecurity C API
 *
 * Cleanup ModSecurity instance.
 *
 */
extern "C" void msc_cleanup(ModSecurity *msc) {
    delete msc;
}


/**
 * @name    msc_init
 * @brief   Initilizes ModSecurity C API
 *
 * ModSecurity initializer.
 *
 * Example Usage:
 * @code
 *
 * ModSecurity msc = msc_init();
 *
 * @endcode
 */
extern "C" ModSecurity *msc_init(void) {
    return new ModSecurity();
}


}  // namespace modsecurity
