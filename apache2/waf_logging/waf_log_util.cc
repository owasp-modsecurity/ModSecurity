#include "waf_log_util.h"

#include <cstdlib>
#include <fstream>
#include <vector>
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
};

struct waf_log
{
    jsoncons::string_view resourceId;
    jsoncons::string_view operationName;
    jsoncons::string_view category;
    property properties;
};

}   // namespace waf_logging

JSONCONS_TYPE_TRAITS_DECL(waf_logging::detail, message, data, file, line);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::property, instanceId, clientIp, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, message, action, site, details, hostname, transactionId);
JSONCONS_TYPE_TRAITS_DECL(waf_logging::waf_log, resourceId, operationName, category, properties);

static std::string to_json_string(const waf_logging::waf_log& log) {
    jsoncons::json_options options;
    std::string json_output;
    jsoncons::encode_json(jsoncons::ojson{}, log, json_output, options, jsoncons::indenting::indent);
    return json_output;
};

static std::vector<int> default_disableable_rule_ids;

static const std::string WAF_ACTION_BLOCKED {"Blocked"};
static const std::string WAF_ACTION_DETECTED {"Detected"};
static const std::string WAF_ACTION_MATCHED {"Matched"};

static std::string get_json_log_message(const char* resource_id, const char* operation_name, const char* category,
        const char* instance_id, const char* client_ip, const char* client_port, const char* request_uri,
        const char* rule_set_type, const char* rule_set_version, const char* rule_id, const char* messages,
        const int action, const int site, const char* details_messages, const char* details_data,
        const char* details_file, const char* details_line, const char* hostname, const char* waf_unique_id) {

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
    const int current_id = std::stoi(static_cast<std::string>(rule_id_str));
    std::cout << "Current id" << current_id << std::endl;
    const bool is_mandatory = !std::binary_search(default_disableable_rule_ids.begin(), default_disableable_rule_ids.end(),
            current_id);
    const str_view action_str =
        (rule_set_version[0] != '2' && !is_mandatory) ? WAF_ACTION_MATCHED :
        action == MODSEC_MODE_DETECT                ? WAF_ACTION_DETECTED :
        action == MODSEC_MODE_PREVENT               ? WAF_ACTION_BLOCKED :
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

    const str_view site_str = (site == 0) ? "Global" : str_view{};

    const str_view details_file_str = details_file ? str_view {details_file, strlen(details_file) - 1} : str_view{};
    const str_view details_data_str = make_string_view_strip_quotes(details_data);
    const str_view details_line_str = make_string_view_strip_quotes(details_line);
    const str_view waf_unique_id_str = make_string_view_strip_quotes(waf_unique_id);

    return to_json_string({
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
            waf_unique_id_str
        }
    });
}

// Get fields from ModSec and format them to JSON.
char* generate_json(const char* resource_id, const char* operation_name, const char* category,
        const char* instance_id, const char* client_ip, const char* client_port, const char* request_uri,
        const char* rule_set_type, const char* rule_set_version, const char* rule_id, const char* messages,
        const int action, const int site, const char* details_messages, const char* details_data,
        const char* details_file, const char* details_line, const char* hostname, const char* waf_unique_id) {
    
    try {
        const std::string json_string = get_json_log_message(resource_id, operation_name, category,
                instance_id, client_ip, client_port, request_uri, rule_set_type, rule_set_version,
                rule_id, messages, action, site, details_messages, details_data, details_file,
                details_line, hostname, waf_unique_id);
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

void init_appgw_rules_id_hash() {
    std::ifstream ifs("/etc/nginx/modsec/disableableRules.conf");
    if(ifs) {
        std::copy(std::istream_iterator<int>(ifs),
                std::istream_iterator<int>(),
                std::back_inserter(default_disableable_rule_ids));
        std::sort(default_disableable_rule_ids.begin(), default_disableable_rule_ids.end());
    }
}
