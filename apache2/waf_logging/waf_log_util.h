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

#define RULE_TYPE_OWASP_CRS "OWASP_CRS"

#define RULE_HASH_SIZE 499

#ifdef __cplusplus
extern "C" {
#endif
char* generate_json(const char* resourceId, const char* operationName, const char* category, const char* instanceId, const char* clientIP, const char* clientPort, const char* requestUri, const char* ruleSetType, const char* ruleSetVersion, const char* ruleId, const char* messages, const int action, const int site, const char* details_messages, const char* details_data, const char* details_file, const char* details_line, const char* hostname, const char* waf_unique_id);
void free_json(char* str);
// Returns 0 if succeeded, non-zero code otherwise
int init_appgw_rules_id_hash();
#ifdef __cplusplus
}
#endif

#endif
