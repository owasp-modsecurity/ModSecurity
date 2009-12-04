/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2009 Breach Security, Inc. (http://www.breach.com/)
 *
 * This product is released under the terms of the General Public Licence,
 * version 2 (GPLv2). Please refer to the file LICENSE (included with this
 * distribution) which contains the complete text of the licence.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the exception in
 * file MODSECURITY_LICENSING_EXCEPTION in the directory of this software
 * distribution.
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Breach Security, Inc.
 * directly using the email address support@breach.com.
 *
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>
#include <apr_file_info.h>

#include "modsecurity.h"

int DSOLOCAL normalize_path_inplace(unsigned char *input, int len, int win, int *changed);

int DSOLOCAL parse_boolean(const char *input);

int DSOLOCAL parse_name_eq_value(apr_pool_t *mp, const char *input, char **name, char **value);

char DSOLOCAL *url_encode(apr_pool_t *mp, char *input, unsigned int input_len, int *changed);

char DSOLOCAL *strnurlencat(char *destination, char *source, unsigned int maxlen);

char DSOLOCAL *file_dirname(apr_pool_t *p, const char *filename);

char DSOLOCAL *file_basename(apr_pool_t *p, const char *filename);

int DSOLOCAL hex2bytes_inplace(unsigned char *data, int len);

char DSOLOCAL *bytes2hex(apr_pool_t *pool, unsigned char *data, int len);

int DSOLOCAL is_token_char(unsigned char c);

int DSOLOCAL remove_lf_crlf_inplace(char *text);

unsigned char DSOLOCAL x2c(unsigned char *what);

unsigned char DSOLOCAL xsingle2c(unsigned char *what);

char DSOLOCAL *guess_tmp_dir(apr_pool_t *p);

char DSOLOCAL *current_logtime(apr_pool_t *mp);

char DSOLOCAL *current_filetime(apr_pool_t *mp);

int DSOLOCAL msc_mkstemp_ex(char *template, int mode);

int DSOLOCAL msc_mkstemp(char *template);

char DSOLOCAL *strtolower_inplace(unsigned char *str);

unsigned char DSOLOCAL *c2x(unsigned what, unsigned char *where);

char DSOLOCAL *log_escape(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_nq(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_nq_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_header_name(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_hex(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_raw(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_nul(apr_pool_t *mp, const unsigned char *text, unsigned long int text_length);

char DSOLOCAL *_log_escape(apr_pool_t *p, const unsigned char *input,
    unsigned long int input_length, int escape_quotes, int escape_colon);

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

#endif
