#include "waf_log_util_internal.h"
#include "waf_log_util_external.h"

using namespace std;
unordered_map<int, bool> appgw_ruleid_hash;

// This function fills in a waf format message based on modsec input.
void set_waf_format(waf_format::Waf_Format* waf_format, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, const char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, const char* hostname, char* waf_unique_id) {
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
		ruleId[strlen(ruleId) - 1] = '\0';
		properties->set_ruleid(ruleId+1);
    }
    
	bool is_mandatory = false;
    try {
		int tmpid = atoi(ruleId+1);
		is_mandatory = rule_is_mandatory(tmpid);

		if (ruleSetVersion[0] != '2' && !is_mandatory)
			properties->set_action(WAF_ACTION_MATCHED);
		else {
			switch (action) {
			case MODSEC_MODE_DETECT:
				properties->set_action(WAF_ACTION_DETECTED);
				break;
			case MODSEC_MODE_PREVENT:
				properties->set_action(WAF_ACTION_BLOCKED);
				break;
			default:
				break;
			}
		}
	}
    catch (...) {
		properties->set_action("");
	}
    
	if (messages != NULL) {
		if (is_mandatory) {
			char mandatory_message[1024] = "Mandatory rule. Cannot be disabled. ";
			int ind = strlen(mandatory_message) - 1;
			int i;
			for (i = 1; i < strlen(messages) - 1; i++) {
				if (i + ind < 1023) {
					mandatory_message[ind + i] = messages[i];
				}
				else {
					break;
				}
			}
			mandatory_message[ind + i] = '\0';
			properties->set_message(mandatory_message);
		}
		else {
			messages[strlen(messages) - 1] = '\0';
			properties->set_message(messages+1);
		}		
	}

	if (site == 0) {
        properties->set_site(waf_format::Properties::Global);
    }
    
    if (details_messages != NULL) {
        details->set_message(details_messages);
    }
    
    if (details_data != NULL) {
		details_data[strlen(details_data) - 1] = '\0';
        details->set_data(details_data+1);
    }
    
    if (details_file != NULL) {
        details->set_file(details_file);
    }
    
    if (details_line != NULL) {
		details_line[strlen(details_line) - 1] = '\0';
        details->set_line(details_line+1);
    }
    
	if (hostname != NULL) {
		properties->set_hostname(hostname);
	}

	if (waf_unique_id != NULL) {
		waf_unique_id[strlen(waf_unique_id) - 1] = '\0';
		properties->set_transactionid(waf_unique_id+1);
	}
}

// Main function:  get fields from modsec, set the protobuf object and write to file in json.
int generate_json(char** result_json, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, const char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, const char* hostname, char* waf_unique_id) {
    waf_format::Waf_Format waf_format;
    string json_string;
    google::protobuf::util::JsonPrintOptions options;
    google::protobuf::util::Status convert_result;
    char* json_str;
    
    try {
        // Verify that the version of the library that we linked against is
        // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        
        // Set Waf format.
        set_waf_format(&waf_format, resourceId, operationName, category, instanceId, clientIP, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, messages, action, site, details_messages, details_data, details_file, details_line, hostname, waf_unique_id); 
        
        options.add_whitespace = false;
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
    *result_json = strdup((json_string + "\n").c_str());
    return WAF_LOG_UTIL_SUCCESS; 
}

void free_json(char* str) {
    free(str);
}

void init_appgw_rules_id_hash() {
	ifstream infile(RULES_ID_FILE);
	string line;

	while (getline(infile, line)) {
		int rule_id = stoi(line);
		appgw_ruleid_hash[rule_id] = true;
	}
	infile.close();

	return;
}

bool rule_is_mandatory(int rule_id) {
	return (appgw_ruleid_hash.find(rule_id) == appgw_ruleid_hash.end());
}