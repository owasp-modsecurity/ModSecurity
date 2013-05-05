/**
 * Copyright 2012, Nick Galbreath
 * nickg@client9.com
 * BSD License - see COPYING.txt for details
 *
 * (setq-default indent-tabs-mode nil)
 * (setq c-default-style "k&r"
 *     c-basic-offset 4)
 *  indent -kr -nut
 */
#ifndef _SQLPARSE_PRIVATE_H
#define _SQLPARSE_PRIVATE_H

#include "sqlparse.h"

/***
 * The stdlib function 'strspn' assumes input to null-delimited.
 * This allows us to specifying and input length and allows
 * embedded nulls
 */
size_t strlenspn(const char *s, size_t len, const char *accept);

int streq(const char *a, const char *b);

void st_clear(stoken_t * st);
void st_assign_char(stoken_t * st, const char stype, const char value);
void st_set_type(stoken_t * st, const char stype);
void st_assign(stoken_t * st, const char stype, const char *value,
               size_t len);
void st_assign_cstr(stoken_t * st, const char stype, const char *value);
void st_copy(stoken_t * dest, const stoken_t * src);

int st_equals_cstr(const stoken_t * src, const char stype,
                    const char *value);

int st_is_empty(const stoken_t * st);
int st_is_arith_op(const stoken_t * st);
int st_is_unary_op(const stoken_t * st);
int st_is_english_op(const stoken_t * st);
int st_is_logical_op(const stoken_t * st);
int st_is_multiword_start(const stoken_t * st);

const char *bsearch_cstr(const char *key, const char *base[],
                         size_t nmemb);

typedef struct {
    const char *word;
    char type;
} keyword_t;

char bsearch_keyword_type(const char *key, const keyword_t keywords[],
                          size_t len);

int is_operator2(const char *key);

int is_sqli_pattern(const char *key);

size_t parse_none(sfilter * sf);
size_t parse_money(sfilter * sf);
size_t parse_other(sfilter * sf);
size_t parse_white(sfilter * sf);
size_t parse_operator1(sfilter *sf);
size_t parse_char(sfilter *sf);
size_t parse_eol_comment(sfilter *sf);
size_t parse_dash(sfilter *sf);
size_t is_mysql_comment(const char *cs, const size_t len, size_t pos);
size_t parse_slash(sfilter *sf);
size_t parse_backslash(sfilter * sf);
size_t parse_operator2(sfilter *sf);
size_t parse_string_core(const char *cs, const size_t len, size_t pos,
                         stoken_t * st, char delim, size_t offset);
size_t parse_string(sfilter *sf);
size_t parse_word(sfilter * sf);
size_t parse_var(sfilter * sf);

size_t parse_number(sfilter * sf);

int parse_token(sfilter * sf);

/**
 * Looks at syntax_last and syntax_current to see
 * if they can be merged into a multi-keyword
 */
int syntax_merge_words(stoken_t * a, stoken_t * b);

void sfilter_reset(sfilter * sf, const char *s, size_t slen);

/**
 * Takes a raw stream of SQL tokens and does the following:
 * * Merge mutliple strings into one "foo", "bar" --> "foo bar"
 * * Remove comments except last one 1, +, -- foo, 1 ->> 1,+,1
 * * Merge multi-word keywords and operators into one
 *   e.g. "UNION", "ALL" --> "UNION ALL"
 */
int sqli_tokenize(sfilter * sf, stoken_t * sout);

int filter_fold(sfilter * sf, stoken_t * sout);

int char2int(char c);
unsigned long long pat2int(const char *pat);

#endif /* _SQLPARSE_PRIVATE_H */
