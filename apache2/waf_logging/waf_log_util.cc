#include "waf_log_util_internal.h"
#include "waf_log_util_external.h"

// This function fills in a waf format message based on modsec input.
void set_waf_format(waf_format::Waf_Format* waf_format, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, char* hostname, char* time) {
    waf_format::Properties *properties;
    waf_format::Details *details;

    properties = waf_format->mutable_properties();
    details = properties->mutable_details();

    if (resourceId != NULL) {
        waf_format->set_resourceid(resourceId);
    }

    if (operationName != NULL) {
        waf_format->set_operationname(operationName);
    }

    if (time != NULL) {
        waf_format->set_time(time);
    }

    if (category != NULL) {
        waf_format->set_category(category);
    }

    if (instanceId != NULL) {
        properties->set_instanceid(instanceId);
    }

    if (clientIP != NULL) {
        properties->set_clientip(clientIP);
    }
    
    if (clientPort != NULL) {
        properties->set_clientport(clientPort);
    }
    
    if (requestUri != NULL) {
        properties->set_requesturi(requestUri);
    }
    
    if (ruleSetType != NULL) {
        properties->set_rulesettype(ruleSetType);
    }
    
    if (ruleSetVersion != NULL) {
        properties->set_rulesetversion(ruleSetVersion);
    }
    
    if (ruleId != NULL) {
        properties->set_ruleid(ruleId);
    }
    
    if (messages != NULL) {
        properties->set_message(messages);
    }
    
    switch(action) {
        case 1:
            properties->set_action(waf_format::Properties::Detected);
            break;
        case 2:
            properties->set_action(waf_format::Properties::Blocked);
            break;
        default:
            break;
    }
    
    if (site == 0) {
        properties->set_site(waf_format::Properties::Global);
    }
    
    if (details_messages != NULL) {
        details->set_message(details_messages);
    }
    
    if (details_data != NULL) {
        details->set_data(details_data);
    }
    
    if (details_file != NULL) {
        details->set_file(details_file);
    }
    
    if (details_line != NULL) {
        details->set_line(details_line);
    }
    
    if (hostname != NULL) {
        properties->set_hostname(hostname);
    }
}

// Main function:  get fields from modsec, set the protobuf object and write to file in json.
int generate_json(char** result_json, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, char* hostname, char* time) {
    waf_format::Waf_Format waf_format;
    std::string json_string;
    google::protobuf::util::JsonPrintOptions options;
    google::protobuf::util::Status convert_result;
    char* json_str;
    
    try {
        // Verify that the version of the library that we linked against is
        // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        
        // Set Waf format.
        set_waf_format(&waf_format, resourceId, operationName, category, instanceId, clientIP, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, messages, action, site, details_messages, details_data, details_file, details_line, hostname, time); 
        
        options.add_whitespace = true;
        options.always_print_primitive_fields = true;
        options.preserve_proto_field_names = true;
        
        convert_result = MessageToJsonString(waf_format, &json_string, options);
        
        if (!convert_result.ok()) {
            return WAF_LOG_UTIL_FAILED;
        }
    }
    catch (...) {
        return WAF_LOG_UTIL_FAILED;
    }
    
    // Write the waf json string to disk.
    *result_json = strdup(json_string.c_str());
    return WAF_LOG_UTIL_SUCCESS; 
}

void free_json(char* str) {
    free(str);
}
