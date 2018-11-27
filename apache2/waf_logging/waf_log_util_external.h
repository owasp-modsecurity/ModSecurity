#ifndef _WAF_LOG_UTIL_EXTERNAL_HEADER
#define _WAF_LOG_UTIL_EXTERNAL_HEADER

#define WAF_LOG_UTIL_FAILED -1
#define WAF_LOG_UTIL_SUCCESS 0
#define MODSEC_MODE_DETECT 1
#define MODSEC_MODE_PREVENT 2
#define WAF_RULESET_PREFIX "/RuleSets/"
#define WAF_LOG_UTIL_FILE "waf_json.log"
#define WAF_LOG_UTIL_OPERATION_NAME "ApplicationGatewayFirewall"
#define WAF_LOG_UTIL_CATEGORY "ApplicationGatewayFirewallLog"

#define WAF_ACTION_BLOCKED "Blocked"
#define WAF_ACTION_DETECTED "Detected"
#define WAF_ACTION_MATCHED "Matched"

#define RULE_TYPE_OWASP_CRS "OWASP_CRS"

#define RULE_HASH_SIZE 499
#define RULES_ID_FILE "/etc/nginx/modsec/disableableRules.conf"

#ifdef __cplusplus
extern "C" {
#endif
int generate_json(char** result, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, const char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, const char* hostname, char* waf_unique_id);
void free_json(char* str);
bool rule_is_mandatory(int ruleId);
void init_appgw_rules_id_hash();
void strip_number(char * in);
#ifdef __cplusplus
}
#endif

#endif
