/**
 * Copyright 2012,2013  Nick Galbreath
 * nickg@client9.com
 * BSD License -- see COPYING.txt for details
 *
 * (setq-default indent-tabs-mode nil)
 * (setq c-default-style "k&r"
 *     c-basic-offset 4)
 *  indent -kr -nut
 */

/* for memmem */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#if 0
#define FOLD_DEBUG printf("%d: Fold state = %d, current=%c, last=%c\n", __LINE__, sf->fold_state, current->type, last->type == CHAR_NULL ? '~': last->type)
#else
#define FOLD_DEBUG
#endif

// order is important here
#include "sqlparse_private.h"
#include "sqlparse_data.h"

size_t strlenspn(const char *s, size_t len, const char *accept)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        /* likely we can do better by inlining this function
         * but this works for now
         */
        if (strchr(accept, s[i]) == NULL) {
            return i;
        }
    }
    return len;
}

/*
 * ASCII case insenstive compare only!
 */
int cstrcasecmp(const char *a, const char *b)
{
    int ca, cb;

    do {
        ca = *a++ & 0xff;
        cb = *b++ & 0xff;
        if (ca >= 'a' && ca <= 'z')
            ca -= 0x20;
        if (cb >= 'a' && cb <= 'z')
            cb -= 0x20;
    } while (ca == cb && ca != '\0');

    return ca - cb;
}

int streq(const char *a, const char *b)
{
    return cstrcasecmp(a, b) == 0;
}

void st_clear(stoken_t * st)
{
    st->type = CHAR_NULL;
    st->str_open = CHAR_NULL;
    st->str_close = CHAR_NULL;
    st->val[0] = CHAR_NULL;
}

int st_is_empty(const stoken_t * st)
{
    return st->type == CHAR_NULL;
}

void st_assign_char(stoken_t * st, const char stype, const char value)
{
    st->type = stype;
    st->val[0] = value;
    st->val[1] = CHAR_NULL;
}

void st_assign(stoken_t * st, const char stype, const char *value,
               size_t len)
{
    size_t last = len < (ST_MAX_SIZE - 1) ? len : (ST_MAX_SIZE - 1);
    st->type = stype;
    strncpy(st->val, value, last);
    st->val[last] = CHAR_NULL;
}

void st_assign_cstr(stoken_t * st, const char stype, const char *value)
{
    st->type = stype;
    strncpy(st->val, value, ST_MAX_SIZE - 1);
    st->val[ST_MAX_SIZE - 1] = CHAR_NULL;
}

int st_equals_cstr(const stoken_t * st, const char stype,
                    const char *value)
{
    return st->type == stype && !cstrcasecmp(value, st->val);
}

void st_copy(stoken_t * dest, const stoken_t * src)
{
    memcpy(dest, src, sizeof(stoken_t));
}

const char *bsearch_cstrcase(const char *key, const char *base[], size_t nmemb)
{
    int left = 0;
    int right = (int) nmemb - 1;

    while (left <= right) {
        int pos = (left + right) / 2;
        int cmp = cstrcasecmp(base[pos], key);
        if (cmp == 0) {
            return base[pos];
        } else if (cmp < 0) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    return NULL;
}

const char *bsearch_cstr(const char *key, const char *base[], size_t nmemb)
{
    int left = 0;
    int right = (int) nmemb - 1;

    while (left <= right) {
        int pos = (left + right) / 2;
        int cmp = strcmp(base[pos], key);
        if (cmp == 0) {
            return base[pos];
        } else if (cmp < 0) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    return NULL;
}

char bsearch_keyword_type(const char *key, const keyword_t * keywords,
                          size_t numb)
{
    int left = 0;
    int right = (int) numb - 1;

    while (left <= right) {
        int pos = (left + right) / 2;
        int cmp = cstrcasecmp(keywords[pos].word, key);
        if (cmp == 0) {
            return keywords[pos].type;
        } else if (cmp < 0) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    return CHAR_NULL;
}

int is_operator2(const char *key)
{
    return bsearch_cstrcase(key, operators2, operators2_sz) != NULL;
}

int st_is_multiword_start(const stoken_t * st)
{
    return bsearch_cstrcase(st->val,
                        multikeywords_start,
                        multikeywords_start_sz) != NULL;
}

int st_is_unary_op(const stoken_t * st)
{
    return (st->type == 'o' && !(strcmp(st->val, "+") &&
                                 strcmp(st->val, "-") &&
                                 strcmp(st->val, "!") &&
                                 strcmp(st->val, "!!") &&
                                 cstrcasecmp(st->val, "NOT") &&
                                 strcmp(st->val, "~")));
}

int st_is_arith_op(const stoken_t * st)
{
    return (st->type == 'o' && !(strcmp(st->val, "-") &&
                                 strcmp(st->val, "+") &&
                                 strcmp(st->val, "~") &&
                                 strcmp(st->val, "!") &&
                                 strcmp(st->val, "/") &&
                                 strcmp(st->val, "%") &&
                                 strcmp(st->val, "*") &&
                                 strcmp(st->val, "|") &&
                                 strcmp(st->val, "&") &&
                                 cstrcasecmp(st->val, "MOD") &&
                                 cstrcasecmp(st->val, "DIV")));
}

size_t parse_white(sfilter * sf)
{
    return sf->pos + 1;
}

size_t parse_operator1(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(current, 'o', cs[pos]);
    return pos + 1;
}

size_t parse_other(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(current, '?', cs[pos]);
    return pos + 1;
}

size_t parse_char(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(current, cs[pos], cs[pos]);
    return pos + 1;
}

size_t parse_eol_comment(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    const char *endpos =
        (const char *) memchr((const void *) (cs + pos), '\n', slen - pos);
    if (endpos == NULL) {
        st_assign_cstr(current, 'c', cs + pos);
        return slen;
    } else {
        st_assign(current, 'c', cs + pos, endpos - cs - pos);
        return (endpos - cs) + 1;
    }
}

size_t parse_dash(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;


    size_t pos1 = pos + 1;
    if (pos1 < slen && cs[pos1] == '-') {
        return parse_eol_comment(sf);
    } else {
        st_assign_char(current, 'o', '-');
        return pos1;
    }
}

size_t is_mysql_comment(const char *cs, const size_t len, size_t pos)
{
    size_t i;

    if (pos + 2 >= len) {
        return 0;
    }
    if (cs[pos + 2] != '!') {
        return 0;
    }
    // this is a mysql comment
    // got "/*!"
    if (pos + 3 >= len) {
        return 3;
    }

    if (!isdigit(cs[pos + 3])) {
        return 3;
    }
    // handle odd case of /*!0SELECT
    if (!isdigit(cs[pos + 4])) {
        return 4;
    }

    if (pos + 7 >= len) {
        return 4;
    }

    for (i = pos + 5; i <= pos + 7; ++i) {
        if (!isdigit(cs[i])) {
            return 3;
        }
    }
    return 8;
}

size_t parse_slash(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;
    const char* cur = cs + pos;

    size_t pos1 = pos + 1;
    if (pos1 == slen || cs[pos1] != '*') {
        return parse_operator1(sf);
    }

    size_t inc = is_mysql_comment(cs, slen, pos);
    if (inc == 0) {

        // skip over initial '/*'
        const char *ptr =
            (const char *) memmem(cur + 2, slen - (pos + 2), "*/", 2);
        if (ptr == NULL) {
            // unterminated comment
            st_assign_cstr(current, 'c', cs + pos);
            return slen;
        } else {
            // postgresql allows nested comments which makes
            // this is incompatible with parsing so
            // if we find a '/*' inside the coment, then
            // make a new token.
            char ctype = 'c';
            const size_t clen = (ptr + 2) - (cur);
            if (memmem(cur + 2, ptr - (cur + 2), "/*", 2) !=
                NULL) {
                ctype = 'X';
            }
            st_assign(current, ctype, cs + pos, clen);

            return pos + clen;
        }
    } else {
        // MySQL Comment
        sf->in_comment = TRUE;
        st_clear(current);
        return pos + inc;
    }
}

size_t parse_backslash(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    if (pos + 1 < slen && cs[pos + 1] == 'N') {
        st_assign_cstr(current, '1', "NULL");
        return pos + 2;
    } else {
        return parse_other(sf);
    }
}

size_t parse_operator2(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    if (pos + 1 >= slen) {
        return parse_operator1(sf);
    }
    char op2[3] = { cs[pos], cs[pos + 1], CHAR_NULL };

    // Special Hack for MYSQL style comments
    // instead of turning:
    // /*! FOO */  into FOO by rewriting the string, we
    // turn it into FOO */ and ignore the ending comment
    if (sf->in_comment && op2[0] == '*' && op2[1] == '/') {
        sf->in_comment = FALSE;
        st_clear(current);
        return pos + 2;
    } else if (pos + 2 < slen && op2[0] == '<' && op2[1] == '='
               && cs[pos + 2] == '>') {
        // special 3-char operator

        st_assign_cstr(current, 'o', "<=>");
        return pos + 3;
    } else if (is_operator2(op2)) {
        if (streq(op2, "&&") || streq(op2, "||")) {
            st_assign_cstr(current, '&', op2);
        } else {
            // normal 2 char operator
            st_assign_cstr(current, 'o', op2);
        }
        return pos + 2;
    } else {
        // must be a single char operator
        return parse_operator1(sf);
    }
}

size_t parse_string_core(const char *cs, const size_t len, size_t pos,
                         stoken_t * st, char delim, size_t offset)
{
    // offset is to skip the perhaps first quote char
    const char *qpos =
        (const char *) memchr((const void *) (cs + pos + offset), delim,
                              len - pos - offset);

    // then keep string open/close info
    if (offset == 1) {
        // this is real quote
        st->str_open = delim;
    } else {
        // this was a simulated quote
        st->str_open = CHAR_NULL;
    }

    while (TRUE) {
        if (qpos == NULL) {
            // string ended with no trailing quote
            // assign what we have
            st_assign_cstr(st, 's', cs + pos + offset);
            st->str_close = CHAR_NULL;
            return len;
        } else if (*(qpos - 1) != '\\') {
            // ending quote is not escaped.. copy and end
            st_assign(st, 's', cs + pos + offset,
                      qpos - (cs + pos + offset));
            st->str_close = delim;
            return qpos - cs + 1;
        } else {
            qpos =
                (const char *) memchr((const void *) (qpos + 1), delim,
                                      (cs + len) - (qpos + 1));
        }
    }
}

/**
 * Used when first char is a ' or "
 */
size_t parse_string(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    // assert cs[pos] == single or double quote
    return parse_string_core(cs, slen, pos, current, cs[pos], 1);
}

size_t parse_word(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    size_t pos = sf->pos;

    size_t slen =
        strlenspn(cs + pos, sf->slen - pos,
                  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.$");

    st_assign(current, 'n', cs + pos, slen);
    if (slen < ST_MAX_SIZE) {
        char ch = bsearch_keyword_type(current->val, sql_keywords,
                                       sql_keywords_sz);
        if (ch == CHAR_NULL) {
            ch = 'n';
        }
        current->type = ch;
    }
    return pos + slen;
}

size_t parse_var(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    size_t pos1 = pos + 1;

    // move past optional other '@'
    if (pos1 < slen && cs[pos1] == '@') {
        pos1 += 1;
    }

    size_t xlen =
        strlenspn(cs + pos1, slen - pos1,
                  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.$");
    if (xlen == 0) {
        st_assign(current, 'v', cs + pos, (pos1 - pos));
        return pos1;
    } else {
        st_assign(current, 'v', cs + pos, xlen + (pos1 - pos));
        return pos1 + xlen;
    }
}

size_t parse_number(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    if (pos + 1 < slen && cs[pos] == '0' && (cs[pos + 1] == 'X' || cs[pos + 1] == 'x')) {
        // TBD compare if isxdigit
        size_t xlen =
            strlenspn(cs + pos + 2, slen - pos - 2, "0123456789ABCDEFabcdef");
        if (xlen == 0) {
            st_assign_cstr(current, 'n', "0X");
            return pos + 2;
        } else {
            st_assign(current, '1', cs + pos, 2 + xlen);
            return pos + 2 + xlen;
        }
    }
    size_t start = pos;

    while (isdigit(cs[pos])) {
        pos += 1;
    }
    if (cs[pos] == '.') {
        pos += 1;
        while (pos < slen && isdigit(cs[pos])) {
            pos += 1;
        }
        if (pos - start == 1) {
            st_assign_char(current, 'n', '.');
            return pos;
        }
    }

    if (cs[pos] == 'E' || cs[pos] == 'e') {
        pos += 1;
        if (pos < slen && (cs[pos] == '+' || cs[pos] == '-')) {
            pos += 1;
        }
        while (isdigit(cs[pos])) {
            pos += 1;
        }
    } else if (isalpha(cs[pos])) {
        // oh no, we have something like '6FOO'
        // use microsoft style parsing and take just
        // the number part and leave the rest to be
        // parsed later
        st_assign(current, '1', cs + start, pos - start);
        return pos;
    }

    st_assign(current, '1', cs + start, pos - start);
    return pos;
}

int parse_token(sfilter * sf)
{
    stoken_t *current = &sf->syntax_current;
    const char *s = sf->s;
    const size_t slen = sf->slen;
    size_t *pos = &sf->pos;

    st_clear(current);

    if (*pos == 0 && sf->delim != CHAR_NULL) {
        *pos = parse_string_core(s, slen, 0, current, sf->delim, 0);
        return TRUE;
    }

    while (*pos < slen) {
        const int ch = (int) (s[*pos]);
        if (ch < 0 || ch > 127) {
            *pos += 1;
            continue;
        }
        pt2Function fnptr = char_parse_map[ch];
        *pos = (*fnptr) (sf);
        if (current->type != CHAR_NULL) {
            return TRUE;
        }
    }
    return FALSE;
}

void sfilter_reset(sfilter * sf, const char *s, size_t len)
{
    memset(sf, 0, sizeof(sfilter));
    sf->s = s;
    sf->slen = len;
}

int syntax_merge_words(stoken_t * a, stoken_t * b)
{
    if (!
        (a->type == 'k' || a->type == 'n' || a->type == 'o'
         || a->type == 'U')) {
        return FALSE;
    }

    size_t sz1 = strlen(a->val);
    size_t sz2 = strlen(b->val);
    size_t sz3 = sz1 + sz2 + 1;
    if (sz3 >= ST_MAX_SIZE) {
        return FALSE;
    }
    // oddly annoying  last.val + ' ' + current.val
    char tmp[ST_MAX_SIZE];
    memcpy(tmp, a->val, sz1);
    tmp[sz1] = ' ';
    memcpy(tmp + sz1 + 1, b->val, sz2);
    tmp[sz3] = CHAR_NULL;

    char ch = bsearch_keyword_type(tmp, multikeywords, multikeywords_sz);
    if (ch != CHAR_NULL) {
        // -1, don't copy the null byte
        st_assign(a, ch, tmp, sz3);
        return TRUE;
    } else {
        return FALSE;
    }
}

int sqli_tokenize(sfilter * sf, stoken_t * sout)
{
    stoken_t *last = &sf->syntax_last;
    stoken_t *current = &sf->syntax_current;

    while (parse_token(sf)) {
        char ttype = current->type;
        if (ttype == 'c') {
            st_copy(&sf->syntax_comment, current);
            continue;
        }
        st_clear(&sf->syntax_comment);

        //
        // If we don't have a saved token
        //
        if (last->type == CHAR_NULL) {
            switch (ttype) {

                // items that have special needs
            case 's':
                st_copy(last, current);
                continue;
            case 'n':
            case 'k':
            case 'U':
            case '&':
            case 'o':
                if (st_is_multiword_start(current)) {
                    st_copy(last, current);
                    continue;
                } else if (current->type == 'o' || current->type == '&') {
                    //} else if (st_is_unary_op(current)) {
                    st_copy(last, current);
                    continue;
                } else {
                    // copy to out
                    st_copy(sout, current);
                    return TRUE;
                }
            default:
                // copy to out
                st_copy(sout, current);
                return TRUE;
            }
        }
        //
        // We have a saved token
        //

        switch (ttype) {
        case 's':
            if (last->type == 's') {
                // "FOO" "BAR" == "FOO" (skip second string)
                continue;
            } else {
                st_copy(sout, last);
                st_copy(last, current);
                return TRUE;
            }
            break;

        case 'o':
            // first case to handle "IS" + "NOT"
            if (syntax_merge_words(last, current)) {
                continue;
            } else if (st_is_unary_op(current)
                       && (last->type == 'o' || last->type == '&'
                           || last->type == 'U')) {
                // if an operator is followed by a unary operator, skip it.
                // 1, + ==> "+" is not unary, it's arithmetic
                // AND, + ==> "+" is unary
                continue;
            } else {
                // no match
                st_copy(sout, last);
                st_copy(last, current);
                return TRUE;
            }
            break;

        case 'n':
        case 'k':
            if (syntax_merge_words(last, current)) {
                continue;
            } else {
                // total no match
                st_copy(sout, last);
                st_copy(last, current);
                return TRUE;
            }
            break;

        default:
            // fix up for ambigous "IN"
            // handle case where IN is typically a function
            // but used in compound "IN BOOLEAN MODE" jive
            if (last->type == 'n' && !cstrcasecmp(last->val, "IN")) {
                st_copy(last, current);
                st_assign_cstr(sout, 'f', "IN");
                return TRUE;
            } else {
                // no match at all
                st_copy(sout, last);
                st_copy(last, current);
                return TRUE;
            }
            break;
        }
    }

    // final cleanup
    if (last->type) {
        st_copy(sout, last);
        st_clear(last);
        return TRUE;
    } else if (sf->syntax_comment.type) {
        st_copy(sout, &sf->syntax_comment);
        st_clear(&sf->syntax_comment);
        return TRUE;
    } else {
        return FALSE;
    }
}

int filter_fold(sfilter * sf, stoken_t * sout)
{
    stoken_t *last = &sf->fold_last;
    stoken_t *current = &sf->fold_current;

    if (sf->fold_state == 4 && !st_is_empty(last)) {
        st_copy(sout, last);
        sf->fold_state = 2;
        st_clear(last);
        return TRUE;
    }

    while (sqli_tokenize(sf, current)) {
        // 0 = start of statement
        // skip ( and unary ops
        if (sf->fold_state == 0) {
            if (current->type == '(') {
                continue;
            }
            if (st_is_unary_op(current)) {
                continue;
            }
            sf->fold_state = 1;
        }

        if (st_is_empty(last)) {
            FOLD_DEBUG;
            if (current->type == '1' || current->type == 'n'
                || current->type == '(') {
                sf->fold_state = 2;
                st_copy(last, current);
            }
            st_copy(sout, current);
            return TRUE;
        } else if (last->type == '(' && st_is_unary_op(current)) {
            // similar to beginning of statement
            // an opening '(' resets state, and we should skip all
            // unary operators
            continue;
        } else if (last->type == '(' && current->type == '(') {
            // if we get another '(' after another
            // emit 1, but keep state
            st_copy(sout, current);
            return TRUE;
        } else if ((last->type == '1' || last->type == 'n')
                   && st_is_arith_op(current)) {
            FOLD_DEBUG;
            st_copy(last, current);
        } else if (last->type == 'o'
                   && (current->type == '1' || current->type == 'n')) {
            FOLD_DEBUG;
            st_copy(last, current);
        } else {
            if (sf->fold_state == 2) {
                if (last->type != '1' && last->type != '('
                    && last->type != 'n') {
                    FOLD_DEBUG;
                    st_copy(sout, last);
                    st_copy(last, current);
                    sf->fold_state = 4;
                } else {
                    FOLD_DEBUG;
                    st_copy(sout, current);
                    st_clear(last);
                }
                return TRUE;
            } else {
                if (last->type == 'o') {
                    st_copy(sout, last);
                    st_copy(last, current);
                    sf->fold_state = 4;
                } else {
                    sf->fold_state = 2;
                    st_copy(sout, current);
                    st_clear(last);
                }
                return TRUE;
            }
        }
    }

    if (!st_is_empty(last)) {
        if (st_is_arith_op(last)) {
            st_copy(sout, last);
            st_clear(last);
            return TRUE;
        } else {
            st_clear(last);
        }
    }

    return FALSE;
}

int is_string_sqli(sfilter * sql_state, const char *s, size_t slen,
                    const char delim, ptr_fingerprints_fn fn)
{
    sfilter_reset(sql_state, s, slen);
    sql_state->delim = delim;

    int all_done = 0;
    int tlen = 0;
    while (tlen < MAX_TOKENS) {
        all_done = filter_fold(sql_state, &(sql_state->tokenvec[tlen]));
        if (!all_done) {
            break;
        }

        sql_state->pat[tlen] = sql_state->tokenvec[tlen].type;
        tlen += 1;
    }
    sql_state->pat[tlen] = CHAR_NULL;

    // if token 5 (last) looks like a functino word (such as ABS or ASCII)
    // then check token 6 to see if it's a "(".
    // if NOT then, it's not a function.

    if (tlen == MAX_TOKENS && !all_done
        && sql_state->pat[MAX_TOKENS - 1] == 'f') {

        stoken_t tmp;
        all_done = filter_fold(sql_state, &tmp);
        if (!all_done && tmp.type != '(') {
            sql_state->reason = __LINE__;
            return FALSE;
        }
    }
    // check for 'X' in pattern
    // this means parsing could not be done
    // accurately due to pgsql's double comments
    // or other syntax that isn't consistent
    // should be very rare false positive
    if (strchr(sql_state->pat, 'X')) {
        return TRUE;
    }

    int patmatch = fn(sql_state->pat);

    if (!patmatch) {
        sql_state->reason = __LINE__;
        return FALSE;
    }
    switch (tlen) {
    case 2:{
            // if 'comment' is '#' ignore.. too many FP
            if (sql_state->tokenvec[1].val[0] == '#') {
                sql_state->reason = __LINE__;
                return FALSE;
            }
            // detect obvious sqli scans.. many people put '--' in plain text
            // so only detect if input ends with '--', e.g. 1-- but not 1-- foo
            if ((strlen(sql_state->tokenvec[1].val) > 2)
                && sql_state->tokenvec[1].val[0] == '-') {
                sql_state->reason = __LINE__;
                return FALSE;
            }
            break;
        }
    case 3:{
            // ...foo' + 'bar...
            // no opening quote, no closing quote
            // and each string has data
            if (streq(sql_state->pat, "sos")
                || streq(sql_state->pat, "s&s")) {
                if ((sql_state->tokenvec[0].str_open == CHAR_NULL)
                    && (sql_state->tokenvec[2].str_close == CHAR_NULL)) {

                    // if ....foo" + "bar....
                    return TRUE;
                } else {
                    // not sqli
                    sql_state->reason = __LINE__;
                    return FALSE;
                }
                break;
            }
        }                       /* case 3 */
    }                           /* end switch */
    return TRUE;
}

int is_sqli(sfilter * sql_state, const char *s, size_t slen,
            ptr_fingerprints_fn fn)
{

    if (slen == 0) {
        return FALSE;
    }

    if (is_string_sqli(sql_state, s, slen, CHAR_NULL, fn)) {
        return TRUE;
    }

    if (memchr(s, CHAR_SINGLE, slen)
        && is_string_sqli(sql_state, s, slen, CHAR_SINGLE, fn)) {
        return TRUE;
    }

    if (memchr(s, CHAR_DOUBLE, slen)
        && is_string_sqli(sql_state, s, slen, CHAR_DOUBLE, fn)) {
        return TRUE;
    }

    return FALSE;
}

/*
 not used yet

// [('o', 228), ('k', 220), ('1', 217), (')', 157), ('(', 156), ('s', 154), ('n', 77), ('f', 73), (';', 59), (',', 35), ('v', 17), ('c', 15),
int char2int(char c)
{
    const char *map = "ok1()snf;,";
    const char *pos = strchr(map, c);
    if (pos == NULL) {
        return 15;
    } else {
        return (int) (pos - map) + 1;
    }
}

unsigned long long pat2int(const char *pat)
{
    unsigned long long val = 0;
    while (*pat) {
        val = (val << 4) + char2int(*pat);
        pat += 1;
    }
    return val;
}
*/
