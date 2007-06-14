/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2007 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include "modsecurity.h"

int DSOLOCAL normalise_path_inplace(unsigned char *input, int len, int win);

int DSOLOCAL parse_boolean(const char *input);

int DSOLOCAL parse_name_eq_value(apr_pool_t *mp, const char *input, char **name, char **value);

char DSOLOCAL *url_encode(apr_pool_t *mp, char *input, unsigned int input_len);

char DSOLOCAL *strnurlencat(char *destination, char *source, unsigned int maxlen);

char DSOLOCAL *file_dirname(apr_pool_t *p, const char *filename);

char DSOLOCAL *file_basename(apr_pool_t *p, const char *filename);

int DSOLOCAL hex2bytes_inplace(unsigned char *data, int len);

char DSOLOCAL *bytes2hex(apr_pool_t *pool, unsigned char *data, int len);

int DSOLOCAL is_token_char(unsigned char c);

int DSOLOCAL remove_lf_crlf_inplace(char *text);

unsigned DSOLOCAL char x2c(unsigned char *what);

char DSOLOCAL *guess_tmp_dir(apr_pool_t *p);

char DSOLOCAL *current_logtime(apr_pool_t *mp);

char DSOLOCAL *current_filetime(apr_pool_t *mp);

int DSOLOCAL msc_mkstemp(char *template);

char DSOLOCAL *strtolower_inplace(unsigned char *str);

unsigned char DSOLOCAL *c2x(unsigned what, unsigned char *where);

char DSOLOCAL *log_escape(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_nq(apr_pool_t *p, const char *text);

char DSOLOCAL *log_escape_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_nq_ex(apr_pool_t *p, const char *text, unsigned long int text_length);

char DSOLOCAL *log_escape_header_name(apr_pool_t *p, const char *text);

char DSOLOCAL *_log_escape(apr_pool_t *p, const unsigned char *input,
    unsigned long int input_length, int escape_quotes, int escape_colon);

int DSOLOCAL urldecode_uni_nonstrict_inplace_ex(unsigned char *input, long int input_length);

int DSOLOCAL urldecode_nonstrict_inplace_ex(unsigned char *input, long int input_length, int *invalid_count);

int DSOLOCAL html_entities_decode_inplace(apr_pool_t *mp, unsigned char *input, int len);

int DSOLOCAL ansi_c_sequences_decode_inplace(unsigned char *input, int len);

#endif
