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

#include <ctime>
#include <iostream>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rule.h"
#include "modsecurity/rule_message.h"
#include "src/collection/backend/in_memory-per_process.h"
#include "src/collection/backend/lmdb.h"
#include "src/config.h"
#include "src/unique_id.h"
#include "src/utils/regex.h"
#ifdef MSC_WITH_CURL
#include <curl/curl.h>
#endif
#ifdef WITH_YAJL
#include <yajl/yajl_tree.h>
#include <yajl/yajl_gen.h>
#endif

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
    : m_connector(""),
#ifdef WITH_LMDB
    m_global_collection(new collection::backend::LMDB()),
    m_resource_collection(new collection::backend::LMDB()),
    m_ip_collection(new collection::backend::LMDB()),
    m_session_collection(new collection::backend::LMDB()),
    m_user_collection(new collection::backend::LMDB()),
#else
    m_global_collection(new collection::backend::InMemoryPerProcess()),
    m_resource_collection(new collection::backend::InMemoryPerProcess()),
    m_ip_collection(new collection::backend::InMemoryPerProcess()),
    m_session_collection(new collection::backend::InMemoryPerProcess()),
    m_user_collection(new collection::backend::InMemoryPerProcess()),
#endif
    m_logCb(NULL) {
    UniqueId::uniqueId();
    srand(time(NULL));
#ifdef MSC_WITH_CURL
    curl_global_init(CURL_GLOBAL_ALL);
#endif
}


ModSecurity::~ModSecurity() {
#ifdef MSC_WITH_CURL
    curl_global_cleanup();
#endif
#ifdef WITH_GEOIP
    Utils::GeoLookup::getInstance().cleanUp();
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
const std::string ModSecurity::whoAmI() {
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
#elif NETBSD
    platform = "NetBSD";
#elif WIN32
    platform = "Windows";
#endif

    return std::string("ModSecurity v" MODSECURITY_VERSION \
        " (" + platform + ")");
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
void ModSecurity::setConnectorInformation(std::string connector) {
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
const std::string& ModSecurity::getConnectorInformation() {
    return m_connector;
}

void ModSecurity::serverLog(void *data, std::shared_ptr<RuleMessage> rm) {
    if (m_logCb == NULL) {
        std::cerr << "Server log callback is not set -- " << rm->errorLog();
        std::cerr << std::endl;
        return;
    }

    if (rm == NULL) {
        return;
    }

    if (m_logProperties & TextLogProperty) {
        char *d = strdup(rm->log().c_str());
        const void *a = static_cast<const void *>(d);
        m_logCb(data, a);
        free(d);
        return;
    }

    if (m_logProperties & RuleMessageLogProperty) {
        const void *a = static_cast<const void *>(rm.get());
        if (m_logProperties & IncludeFullHighlightLogProperty) {
            const char *err = NULL;
            const char *buf = NULL;
            size_t z;
            int ret = processContentOffset(rm->m_buf.c_str(), rm->m_buf.size(),
                rm->m_reference.c_str(), &rm->m_highlightJSON, &err);
            if (ret < 0) {
#ifdef WITH_YAJL
                yajl_gen g;
                g = yajl_gen_alloc(NULL);
                if (g == NULL) {
                    rm->m_highlightJSON.append(err);
                    goto out;
                }
                yajl_gen_config(g, yajl_gen_beautify, 1);
                yajl_gen_map_open(g);
                yajl_gen_string(g, reinterpret_cast<const unsigned char*>("error"),
                    strlen("error"));
                yajl_gen_string(g, reinterpret_cast<const unsigned char*>(err),
                    strlen(err));
                yajl_gen_map_close(g);
                yajl_gen_get_buf(g, (const unsigned char**)&buf, &z);

                &rm->m_highlightJSON.append(buf);

                yajl_gen_free(g);
#else
                rm->m_highlightJSON.append(err);
#endif
            }
        }
out:
        m_logCb(data, a);
        return;
    }
}


int ModSecurity::processContentOffset(const char *content, size_t len,
    const char *matchString, std::string *json, const char **err) {
#ifdef WITH_YAJL
    Utils::Regex variables("v([0-9]+),([0-9]+)");
    Utils::Regex operators("o([0-9]+),([0-9]+)");
    Utils::Regex transformations("t:(?:(?!t:).)+");
    int i;
    yajl_gen g;
    std::string varValue;
    std::string opValue;
    const unsigned char *buf;
    size_t jsonSize;

    std::list<Utils::SMatch> vars = variables.searchAll(matchString);
    std::list<Utils::SMatch> ops = operators.searchAll(matchString);
    std::list<Utils::SMatch> trans = transformations.searchAll(matchString);

    g = yajl_gen_alloc(NULL);
    if (g == NULL) {
        *err = strdup("Failed to allocate memory for the JSON creation.");
        return -1;
    }

    yajl_gen_config(g, yajl_gen_beautify, 1);

    yajl_gen_map_open(g);
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("match"),
        strlen("match"));

    yajl_gen_array_open(g);
    yajl_gen_map_open(g);

    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("variable"),
            strlen("variable"));

    yajl_gen_map_open(g);
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("highlight"),
            strlen("highlight"));

        yajl_gen_array_open(g);
    while (vars.size() > 0) {
        std::string value;
        yajl_gen_map_open(g);
        vars.pop_back();
        std::string startingAt = vars.back().match;
        vars.pop_back();
        std::string size = vars.back().match;
        vars.pop_back();
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("startingAt"),
            strlen("startingAt"));
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(startingAt.c_str()),
            startingAt.size());
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("size"),
            strlen("size"));
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(size.c_str()),
            size.size());
        yajl_gen_map_close(g);

        if (stoi(startingAt) >= len) {
            std::stringstream e;
            e << "Offset for the variables are out of the content limits. " \
                "Trying to read position " << startingAt.c_str() << " from a buffer "\
                "with only " << len << " bytes. Buffer: " <<  content \
                << std::endl;
            *err = strdup(e.str().c_str());
            return -1;
        }

        value = std::string(content, stoi(startingAt), stoi(size));
        if (varValue.size() > 0) {
            varValue.append(" " + value);
        } else {
            varValue.append(value);
        }
    }
    yajl_gen_array_close(g);

    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("value"),
            strlen("value"));

    yajl_gen_array_open(g);

    yajl_gen_map_open(g);
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("value"),
            strlen("value"));
    yajl_gen_string(g, reinterpret_cast<const unsigned char*>(varValue.c_str()),
            varValue.size());
    yajl_gen_map_close(g);

    while (trans.size() > 0) {
        modsecurity::actions::transformations::Transformation *t;
        std::string varValueRes;
        yajl_gen_map_open(g);
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("transformation"),
            strlen("transformation"));

        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(trans.back().match.c_str()),
            trans.back().match.size());

        t = modsecurity::actions::transformations::Transformation::instantiate(trans.back().match.c_str());
        varValueRes = t->evaluate(varValue, NULL);
        varValue.assign(varValueRes);
        trans.pop_back();

        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("value"),
            strlen("value"));
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>(varValue.c_str()),
            varValue.size());
        yajl_gen_map_close(g);
    }

    yajl_gen_array_close(g);

    yajl_gen_string(g, reinterpret_cast<const unsigned char*>("operator"),
            strlen("operator"));

    yajl_gen_map_open(g);

    while (ops.size() > 0) {
        std::string value;
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("highlight"),
            strlen("highlight"));
        yajl_gen_map_open(g);
        ops.pop_back();
        std::string startingAt = ops.back().match;
        ops.pop_back();
        std::string size = ops.back().match;
        ops.pop_back();
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("startingAt"),
            strlen("startingAt"));
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(startingAt.c_str()),
            startingAt.size());
        yajl_gen_string(g, reinterpret_cast<const unsigned char*>("size"),
            strlen("size"));
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(size.c_str()),
            size.size());
        yajl_gen_map_close(g);

        if (stoi(startingAt) >= varValue.size()) {
            std::stringstream e;
            e << "Offset for the operator is out of the variable limit. " \
                "Trying to read " << startingAt.c_str() << " from a buffer with " \
                "only " << std::to_string(varValue.size()) << " bytes. Buffer: " \
                "" << varValue << std::endl;
                *err = strdup(e.str().c_str());
            return -1;
        }
        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>("value"),
            strlen("value"));

        value = std::string(varValue, stoi(startingAt), stoi(size));

        yajl_gen_string(g,
            reinterpret_cast<const unsigned char*>(value.c_str()),
            value.size());
    }

    yajl_gen_map_close(g);


    yajl_gen_map_close(g);
    yajl_gen_map_close(g);

    yajl_gen_array_close(g);
    yajl_gen_map_close(g);

    yajl_gen_get_buf(g, &buf, &jsonSize);

    json->assign(reinterpret_cast<const char*>(buf), jsonSize);

    yajl_gen_clear(g);
    yajl_gen_free(g);
#else
    *err = strdup("Without YAJL support, we cannot generate JSON.");
    return -1;
#endif
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
