#ifndef _WAF_LOG_UTIL_EXTERNAL_HEADER
#define _WAF_LOG_UTIL_EXTERNAL_HEADER

#define WAF_LOG_UTIL_FAILED -1
#define WAF_LOG_UTIL_SUCCESS 0
#define WAF_RULESET_PREFIX "/RuleSets/"
#define WAF_LOG_UTIL_FILE "waf_json.log"
#define WAF_LOG_UTIL_OPERATION_NAME "ApplicationGatewayFirewall"
#define WAF_LOG_UTIL_CATEGORY "ApplicationGatewayFirewallLog"

#ifdef __cplusplus
extern "C" {
#endif
int generate_json(char** result, char* resourceId, char* operationName, char* category, char* instanceId, char* clientIP, char* clientPort, char* requestUri, char* ruleSetType, char* ruleSetVersion, char* ruleId, char* messages, int action, int site, char* details_messages, char* details_data, char* details_file, char* details_line, char* hostname, char* time);
void free_json(char* str);
#ifdef __cplusplus
}
#endif

#endif
