/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>
#include <apr_file_info.h>

#ifndef APR_WSTICKY
/* Add extra flags added to APR in 0.9.5 */
#define APR_USETID      0x8000 /**< Set user id */
#define APR_GSETID      0x4000 /**< Set group id */
#define APR_WSTICKY     0x2000 /**< Sticky bit */
#endif

#include "modsecurity.h"
#include "re.h"

#ifdef WIN32
#include <ws2tcpip.h>
// This is a trick: for ModSecurity modules this will declare inet_pton,
// but for mymodule.cpp (IIS module) this will skip, because we include
// windows.h before including msc_util.h
// Without the trick we have redefinition conflict.
//
#if !(NTDDI_VERSION >= NTDDI_VISTA)
int DSOLOCAL inet_pton(int family, const char *src, void *dst);
#endif
#endif

#define UNICODE_ERROR_CHARACTERS_MISSING    -1
#define UNICODE_ERROR_INVALID_ENCODING      -2
#define UNICODE_ERROR_OVERLONG_CHARACTER    -3
#define UNICODE_ERROR_RESTRICTED_CHARACTER  -4
#define UNICODE_ERROR_DECODING_ERROR        -5

#ifdef LINUX_S390
int DSOLOCAL swap_int32(int x);
#endif


char DSOLOCAL *utf8_unicode_inplace_ex(apr_pool_t *mp, unsigned char *input, long int input_len, int *changed);

char DSOLOCAL *m_strcasestr(const char *haystack, const char *needle);

int DSOLOCAL normalize_path_inplace(unsigned char *input, int len, int win, int *changed);

int DSOLOCAL parse_boolean(const char *input);

char DSOLOCAL *remove_quotes(apr_pool_t *mptmp, const char *input, int input_len);

char DSOLOCAL *parse_pm_content(const char *op_parm, unsigned short int op_len, msre_rule *rule, char **error_msg);

char DSOLOCAL *remove_escape(apr_pool_t *mptmp, const char *input, int input_len);

int DSOLOCAL parse_name_eq_value(apr_pool_t *mp, const char *input, char **name, char **value);

char DSOLOCAL *url_encode(apr_pool_t *mp, char *input, unsigned int input_len, int *changed);

char DSOLOCAL *strnurlencat(char *destination, char *source, unsigned int maxlen);

char DSOLOCAL *file_dirname(apr_pool_t *p, const char *filename);

char DSOLOCAL *file_basename(apr_pool_t *p, const char *filename);

int DSOLOCAL sql_hex2bytes_inplace(unsigned char *data, int len);

int DSOLOCAL hex2bytes_inplace(unsigned char *data, int len);

char DSOLOCAL *bytes2hex(apr_pool_t *pool, unsigned char *data, int len);

int DSOLOCAL is_token_char(unsigned char c);

int DSOLOCAL remove_lf_crlf_inplace(char *text);

char DSOLOCAL *guess_tmp_dir(apr_pool_t *p);

char DSOLOCAL *current_logtime(apr_pool_t *mp);

char DSOLOCAL *current_filetime(apr_pool_t *mp);

int DSOLOCAL msc_mkstemp_ex(char *templat, int mode);

int DSOLOCAL msc_mkstemp(char *templat);

char DSOLOCAL *strtolower_inplace(unsigned char *str);

char DSOLOCAL *log_escape_re(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_nq(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_nq_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_hex(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_raw(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_nul(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

int DSOLOCAL decode_base64_ext(char *plain_text, const unsigned char *input, int input_len);

int DSOLOCAL convert_to_int(const char c);

int DSOLOCAL set_match_to_tx(modsec_rec *msr, int capture, const char *match, int tx_n);

int DSOLOCAL js_decode_nonstrict_inplace(unsigned char *input, long int input_len);

int DSOLOCAL urldecode_uni_nonstrict_inplace_ex(unsigned char *input, long int input_length, int * changed);

int DSOLOCAL urldecode_nonstrict_inplace_ex(unsigned char *input, long int input_length, int *invalid_count, int *changed);

int DSOLOCAL html_entities_decode_inplace(apr_pool_t *mp, unsigned char *input, int len);

int DSOLOCAL ansi_c_sequences_decode_inplace(unsigned char *input, int len);

char DSOLOCAL *modsec_build(apr_pool_t *mp);

int DSOLOCAL is_empty_string(const char *string);

char DSOLOCAL *resolve_relative_path(apr_pool_t *pool, const char *parent_filename, const char *filename);

int DSOLOCAL css_decode_inplace(unsigned char *input, long int input_len);

apr_fileperms_t DSOLOCAL mode2fileperms(int mode);

char DSOLOCAL *construct_single_var(modsec_rec *msr, char *name);

char DSOLOCAL *format_all_performance_variables(modsec_rec *msr, apr_pool_t *mp);

unsigned char DSOLOCAL is_netmask_v4(char *ip_strv4);

unsigned char DSOLOCAL is_netmask_v6(char *ip_strv6);
#endif
