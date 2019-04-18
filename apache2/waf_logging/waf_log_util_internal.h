#ifndef _WAF_LOG_UTIL_INTERNAL_HEADER
#define _WAF_LOG_UTIL_INTERNAL_HEADER

#include <fstream>
#include <unordered_map>
#include <exception>
#include <jsoncons/json.hpp>
#include <jsoncons/json_options.hpp>

// For convenience
using jsoncons::ojson;
using jsoncons::json_options;
using jsoncons::line_split_kind;

namespace waf_logging {

    struct detail
    {
        std::string message;
        std::string data;
        std::string file;
        std::string line;
    };

    struct property
    {
        std::string instanceId;
        std::string clientIp;
        std::string clientPort;
        std::string requestUri;
        std::string ruleSetType;
        std::string ruleSetVersion;
        std::string ruleId;
        std::string message;
        std::string action;
        std::string site;
        detail details;
        std::string hostname;
        std::string transactionId;
    };

    struct waf_log
    {
        std::string resourceId;
        std::string operationName;
        std::string category;
        property properties;
    };
}

JSONCONS_TYPE_TRAITS_DECL(waf_logging::detail, message, data, file, line);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::property, instanceId, clientIp, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, message, action, site, details, hostname, transactionId);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::waf_log, resourceId, operationName, category, properties);

std::string to_json_string(const waf_logging::waf_log& log) {
    json_options options;
    std::string json_output;
    encode_json(ojson{}, log, json_output, options, jsoncons::indenting::indent);
    return json_output;
};

#endif 
