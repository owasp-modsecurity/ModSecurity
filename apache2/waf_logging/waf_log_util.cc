#include "waf_log_util.h"

#include <cstdlib>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <experimental/optional>
#include <jsoncons/json.hpp>
#include <jsoncons/json_options.hpp>

namespace waf_logging {

struct detail
{
    jsoncons::string_view message;
    jsoncons::string_view data;
    jsoncons::string_view file;
    jsoncons::string_view line;
};

struct property
{
    jsoncons::string_view instanceId;
    jsoncons::string_view clientIp;
    jsoncons::string_view clientPort;
    jsoncons::string_view requestUri;
    jsoncons::string_view ruleSetType;
    jsoncons::string_view ruleSetVersion;
    jsoncons::string_view ruleId;
    jsoncons::string_view message;
    jsoncons::string_view action;
    jsoncons::string_view site;
    detail details;
    jsoncons::string_view hostname;
    jsoncons::string_view transactionId;
    jsoncons::string_view policyId;
    jsoncons::string_view policyScope;
    jsoncons::string_view policyScopeName;
};

struct waf_log
{
    jsoncons::string_view timeStamp;
    jsoncons::string_view resourceId;
    jsoncons::string_view operationName;
    jsoncons::string_view category;
    property properties;
};

}   // namespace waf_logging

JSONCONS_TYPE_TRAITS_DECL(waf_logging::detail, message, data, file, line);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::property, instanceId, clientIp, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, message, action, site, details, hostname, transactionId, policyId, policyScope, policyScopeName);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::waf_log, timeStamp, resourceId, operationName, category, properties);

static std::string to_json_string(const waf_logging::waf_log& log) {
    jsoncons::json_options options;
    std::string json_output;
    jsoncons::encode_json(jsoncons::ojson{}, log, json_output, options);
    return json_output;
};

enum class rule_set_types {
    TYPE_UNKNOWN,
    OWASP_CRS,
};

// Employing X-Macro trick (https://en.wikipedia.org/wiki/X_Macro) to keep the list of versions
// automatically synced up with the map of file paths
// If you need to add new OWASP version, just add it here and it will sync up in all the places
#define LIST_OF_VARIABLES \
    X(2_2_9) \
    X(3_0) \
    X(3_1)

enum class rule_set_versions {
    VERSION_UNKNOWN,
    #define X(name) VERSION_##name,
    LIST_OF_VARIABLES
    #undef X
};

using rule_set_info = std::pair<rule_set_types, rule_set_versions>;
#define X(name) static const rule_set_info OWASP_##name {std::make_pair(rule_set_types::OWASP_CRS, rule_set_versions::VERSION_##name)};
    LIST_OF_VARIABLES
#undef X

static rule_set_types parse_rule_set_type(const char* type) {
    if (!type) {
        return rule_set_types::TYPE_UNKNOWN;
    }

    return (std::strcmp(type, "OWASP_CRS") == 0) ? rule_set_types::OWASP_CRS : rule_set_types::TYPE_UNKNOWN;
}

static rule_set_versions parse_rule_set_version(const char* version) {
    if (!version) {
        return rule_set_versions::VERSION_UNKNOWN;
    }

    return (std::strcmp(version, "2.2.9") == 0) ? rule_set_versions::VERSION_2_2_9 :
        (std::strcmp(version, "3.0.0") == 0) ? rule_set_versions::VERSION_3_0 :
        (std::strcmp(version, "3.1.0") == 0) ? rule_set_versions::VERSION_3_1 :
        rule_set_versions::VERSION_UNKNOWN;
}

using rule_ids_vector = std::vector<int>;

static rule_ids_vector current_rule_ids;
static std::map<rule_set_info, rule_ids_vector> disableable_rule_ids_map;

static const std::string WAF_ACTION_BLOCKED {"Blocked"};
static const std::string WAF_ACTION_DETECTED {"Detected"};
static const std::string WAF_ACTION_MATCHED {"Matched"};
static const std::string WAF_ACTION_ALLOWED {"Allowed"};

static bool is_mandatory_rule(const rule_set_types type, const rule_set_versions version, const int current_id) {
    const auto contains_current_id = [current_id] (const rule_ids_vector& rule_ids) {
        return !std::binary_search(std::begin(rule_ids), std::end(rule_ids), current_id);
    };
    const auto rules_id_iter = disableable_rule_ids_map.find(std::make_pair(type, version));
    if (rules_id_iter != std::end(disableable_rule_ids_map)) {
        return contains_current_id(rules_id_iter->second);
    }

    return contains_current_id(current_rule_ids);
}

static std::string get_json_log_message(const char* timestamp, const char* resource_id, const char* operation_name, 
        const char* category, const char* instance_id, const char* client_ip, const char* client_port, 
        const char* request_uri, const char* rule_set_type, const char* rule_set_version, const char* rule_id, 
        const char* messages, const int action, const int site, const char* details_messages, const char* details_data,
        const char* details_file, const char* details_line, const char* hostname, const char* request_id,
        const char* waf_policy_id, const char* waf_policy_scope, const char* waf_policy_scope_name) {

    using str_view = jsoncons::string_view;
    const auto make_string_view_strip_quotes = [] (const char* str) -> str_view {
        if (!str) {
            return {};
        }
        auto length = strlen(str);
        if (length < 2) {
            return {};
        }

        return {str + 1, length - 2};
    };
    const str_view rule_id_str = make_string_view_strip_quotes(rule_id);
    const rule_set_types type = parse_rule_set_type(rule_set_type);
    const rule_set_versions version = parse_rule_set_version(rule_set_version);
    const int current_id = std::stoi(static_cast<std::string>(rule_id_str));
    const bool is_mandatory = is_mandatory_rule(type, version, current_id);

    const str_view action_str =
        (action == ACTION_ALLOW || action == ACTION_ALLOW_REQUEST || action == ACTION_ALLOW_PHASE) ? WAF_ACTION_ALLOWED :
        (version != rule_set_versions::VERSION_2_2_9 && !is_mandatory)                             ? WAF_ACTION_MATCHED :
        action == MODSEC_MODE_DETECT                                                               ? WAF_ACTION_DETECTED :
        action == MODSEC_MODE_PREVENT                                                              ? WAF_ACTION_BLOCKED :
        str_view{};

    #define MANDATORY_RULE_MESSAGE ("Mandatory rule. Cannot be disabled. ")
    #define MANDATORY_RULE_MESSAGE_LENGTH (sizeof(MANDATORY_RULE_MESSAGE) - 1)
    std::array<char, 1024> mandatory_message {MANDATORY_RULE_MESSAGE};
    str_view message_str;
    if (messages) {
        if (is_mandatory) {
            size_t truncated_length = std::min(strlen(messages) - 1, mandatory_message.size() - MANDATORY_RULE_MESSAGE_LENGTH);
            if (truncated_length > 1) {
                const str_view messages_view {messages + 1, truncated_length - 1};
                std::copy(messages_view.cbegin(), messages_view.cend(), mandatory_message.begin() + MANDATORY_RULE_MESSAGE_LENGTH);
            }
            message_str = {&mandatory_message.front(), MANDATORY_RULE_MESSAGE_LENGTH + truncated_length - 1};
        }
        else {
            message_str = make_string_view_strip_quotes(messages);
        }
    }

    /* For historical reasons, the "site" property is included in log messages.
     * However, it does not carry any useful meaning.
     * We keep it around for compatibility reasons but may consider removing it at some point in the future.
     */
    const str_view site_str = (site == 0) ? "Global" : str_view{};

    const str_view details_file_str = details_file ? str_view {details_file, strlen(details_file) - 1} : str_view{};
    const str_view details_data_str = make_string_view_strip_quotes(details_data);
    const str_view details_line_str = make_string_view_strip_quotes(details_line);

    return to_json_string({
        timestamp,
        resource_id,
        operation_name,
        category,
        waf_logging::property {
            instance_id,
            client_ip,
            client_port,
            request_uri,
            rule_set_type,
            rule_set_version,
            rule_id_str,
            message_str,
            action_str,
            site_str,
            waf_logging::detail {
                details_messages,
                details_data_str,
                details_file_str,
                details_line_str
            },
            hostname,
            request_id,
            waf_policy_id,
            waf_policy_scope,
            waf_policy_scope_name
        }
    });
}

// Get fields from ModSec and format them to JSON.
char* generate_json(const char* timestamp, const char* resource_id, const char* operation_name, const char* category,
        const char* instance_id, const char* client_ip, const char* client_port, const char* request_uri,
        const char* rule_set_type, const char* rule_set_version, const char* rule_id, const char* messages,
        const int action, const int site, const char* details_messages, const char* details_data,
        const char* details_file, const char* details_line, const char* hostname, const char* request_id,
        const char* waf_policy_id, const char* waf_policy_scope, const char* waf_policy_scope_name) {
    
    try {
        const std::string json_string = get_json_log_message(timestamp, resource_id, operation_name, category,
                instance_id, client_ip, client_port, request_uri, rule_set_type, rule_set_version,
                rule_id, messages, action, site, details_messages, details_data, details_file,
                details_line, hostname, request_id, waf_policy_id, waf_policy_scope, waf_policy_scope_name);
        char* result = static_cast<char*>(std::malloc(json_string.length() + 2)); // Two extra bytes for \n and \0
        std::copy(json_string.begin(), json_string.end(), result);
        result[json_string.length()] = '\n';
        result[json_string.length() + 1] = '\0';
        return result;
    } catch (...) {}

    return nullptr;
}

void free_json(char* str) {
    std::free(str);
}

namespace stdx = std::experimental;
static stdx::optional<rule_ids_vector> read_disableable_rule_ids(const std::string& filename) {
    std::ifstream ifs(filename);
    if (ifs) {
        rule_ids_vector rule_ids;
        std::copy(std::istream_iterator<int>(ifs),
                std::istream_iterator<int>(),
                std::back_inserter(rule_ids));
        std::sort(std::begin(rule_ids), std::end(rule_ids));
        return rule_ids;
    }

    return {};
}

int init_appgw_rules_id_hash() {
    const std::map<rule_set_info, std::string> disableableRulesPathsMap {
        #define X(name) {OWASP_##name, "/etc/nginx/modsec/OWASP/" #name "/disableableRules.conf"},
        LIST_OF_VARIABLES
        #undef X
    };
    for (const auto& item : disableableRulesPathsMap) {
        const auto vec = read_disableable_rule_ids(item.second);
        if (vec) {
            disableable_rule_ids_map[item.first] = *vec;
        }
    }
    if (disableable_rule_ids_map.empty()) {
        // If we haven't found type/version-specific files, we need to read the default (deprecated one)
        const auto vec = read_disableable_rule_ids("/etc/nginx/modsec/disableableRules.conf");
        if (!vec) {
            return -1;
        }

        current_rule_ids = *vec;
    }
    return 0;
}
