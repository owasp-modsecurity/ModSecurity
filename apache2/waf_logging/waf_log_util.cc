#include "waf_log_util_internal.h"
#include "waf_log_util_external.h"

using namespace std;
unordered_map<int, bool> appgw_ruleid_hash;

// This function fills in a waf format message based on modsec input.
string get_json_log_message(char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, const char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, const char* hostname, char* waf_unique_id) {
    string action_str;
    string message_str;
    string site_str;

    if (ruleId != NULL) {
		ruleId[strlen(ruleId) - 1] = '\0';
    }
    
	bool is_mandatory = false;
    try {
		int tmpid = atoi(ruleId+1);
		is_mandatory = rule_is_mandatory(tmpid);

		if (ruleSetVersion[0] != '2' && !is_mandatory)
			action_str = WAF_ACTION_MATCHED;
		else {
			switch (action) {
			case MODSEC_MODE_DETECT:
				action_str = WAF_ACTION_DETECTED;
				break;
			case MODSEC_MODE_PREVENT:
				action_str = WAF_ACTION_BLOCKED;
				break;
			default:
				break;
			}
		}
	}
    catch (...) {
		action_str = "";
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
			message_str = mandatory_message;
		}
		else {
			messages[strlen(messages) - 1] = '\0';
			message_str = messages+1;
		}		
	}

	if (site == 0) {
        site_str = "Global";
    }

    if (details_file != NULL) {
		details_file[strlen(details_file) - 1] = '\0';
    }

    if (details_data != NULL) {
		details_data[strlen(details_data) - 1] = '\0';
    }
    
    if (details_line != NULL) {
		details_line[strlen(details_line) - 1] = '\0';
    }

	if (waf_unique_id != NULL) {
		waf_unique_id[strlen(waf_unique_id) - 1] = '\0';
	}

    return to_json_string({resourceId, operationName, category, waf_logging::property{ instanceId, clientIP, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId+1, message_str, action_str, site_str, waf_logging::detail{ details_messages, details_data+1, details_file+1, details_line+1 }, hostname, waf_unique_id+1}});
}

// Main function:  get fields from modsec, set the protobuf object and write to file in json.
int generate_json(char** result_json, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, const char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, const char* hostname, char* waf_unique_id) {
    string json_string;
    char* json_str;
    
    try {
        // Set Waf format.
        json_string = get_json_log_message(resourceId, operationName, category, instanceId, clientIP, clientPort, requestUri, ruleSetType, ruleSetVersion, ruleId, messages, action, site, details_messages, details_data, details_file, details_line, hostname, waf_unique_id); 
    }
    catch (...) {
        return WAF_LOG_UTIL_FAILED;
    }

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